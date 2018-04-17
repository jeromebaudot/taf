#include "MRax.h"
ClassImp(MRax)

//--------------------------------------------------------
//TODO      To Do List
// -> Check "TODO" keyword in this file






//______________________________________________________________________________________________________
//
//	MRAX configuration
//
//  Most of MRax methods [must/can be] configured (display options, ...)
//  Some of this configuration is centralised in this section.
//  Browse the whole code / search the "[C/c]onfiguration" keyword to find other method-configuration-sections 
//______________________________________________________________________________________________________
  //****************************************************************
  //		General configuration
  //****************************************************************
  //
  //  This method contains general and misc configuration
  //
  //****************************************************************
Int_t MRax::ConfigureGeneral(void)
{
  
  fSession->SetEvents(pow(2.,31)-1);// max value, to not be a limit because never done after !
  
  //------------------------------
  // Verbose mode
  //------------------------------
  // This boolean variable enable/disable the verbosity (infos printed on the terminal) 
  // Can be change via GUI
  // Select one of the following :
  configBoolVerbose = kFALSE;
  //configBoolVerbose = kTRUE;
  //------------------------------
  
  fSession->SetEventsToDo(pow(2.,31)-1); // MAX number, to be sure that it's not a limit :-)
  
  // Output files configuration
  resultsDirPath = fSession->GetResultDirName();
  
  FileNamePrefix = fSession->GetOutputFilesPrefix();
  
  FileNameSuffix = fSession->GetOutputFilesSuffix();
  
  FileNameTreeTracks = "TAFtreeTracksV02";// Preffix and suffix will be added !
  FileNameTreeVertex = "TAFtreeVertexV01";
  
  return 0;
}

  //****************************************************************
  //		Displays configuration
  //****************************************************************
  //
  //  This method contains configuration about displays
  //   like planes draw orders, ...
  //
  //****************************************************************
Int_t MRax::ConfigureDisplays(void)
{
  
  gStyle->SetPalette(1);
  //gStyle->SetTitleSize(0.6); VReith : X axis label doesn't work with this option
  //gStyle->SetPadLeftMargin(0.15);
  //gStyle->SetTitleH(.085);
  
  
  //------------------------------
  // PAD rows and columns number
  //------------------------------
  // Number of columns (X) and rows (Y) for the pads where all sensors are displayed 
  //  theses parameters are used like : padDisplayXXX->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY) in display methods
  //  change following values according to your display preferences
  if (nbPlanes ==  1) { configDisplaysPadDivideX =  1; configDisplaysPadDivideY =  1;}
  if (nbPlanes ==  2) { configDisplaysPadDivideX =  2; configDisplaysPadDivideY =  1;}
  if (nbPlanes ==  3) { configDisplaysPadDivideX =  2; configDisplaysPadDivideY =  2;}
  if (nbPlanes ==  4) { configDisplaysPadDivideX =  2; configDisplaysPadDivideY =  2;}
  if (nbPlanes ==  5) { configDisplaysPadDivideX =  3; configDisplaysPadDivideY =  2;}
  if (nbPlanes ==  6) { configDisplaysPadDivideX =  3; configDisplaysPadDivideY =  2;}
  if (nbPlanes ==  7) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  2;}
  if (nbPlanes ==  8) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  2;}
  if (nbPlanes ==  9) { configDisplaysPadDivideX =  3; configDisplaysPadDivideY =  3;}
  if (nbPlanes == 10) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  3;}
  if (nbPlanes == 11) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  3;}
  if (nbPlanes == 12) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  3;}
  if (nbPlanes == 13) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  4;}
  if (nbPlanes == 14) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  4;}
  if (nbPlanes == 15) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  4;}
  if (nbPlanes == 16) { configDisplaysPadDivideX =  4; configDisplaysPadDivideY =  4;}
  if (nbPlanes >= 17) { configDisplaysPadDivideX =  5; configDisplaysPadDivideY =  5;}
  if (nbPlanes >= 24) { configDisplaysPadDivideX =  6; configDisplaysPadDivideY =  6;}
  //------------------------------
  // Check for errors
  // (DO NOT modify)
  if (nbPlanes > configDisplaysPadDivideX*configDisplaysPadDivideY)
  {
    printf_warning("ConfigureDisplays");
    printf("The pads rows and columns number configuration is not coherent with the number of planes to display ! \n");
    printf("You asked %d columns and %d rows for %d planes \n",configDisplaysPadDivideX,configDisplaysPadDivideY,nbPlanes);
    printf("A default configuration has been set \n");
    configDisplaysPadDivideX =  10;
    configDisplaysPadDivideY =  10;
  }
  //------------------------------
  
  
  //------------------------------
  // Planes draw order 
  //------------------------------
  // The order of displayed planes (in pads where all sensors are displayed) can be configured 
  // Different patterns are proposed/can be adjusted here
  // For each display method, the pattern selection is made in the configuration section of the xxx_Init() method
  // Some diplays methods need to know if a plane is a left or right one (horizontal boards with 2 sensors glued on opposite side)
  //------------------------------
  // 1) DEFAULT : Same order as in the config file, without orientation information
  // -> The planes will be drawn in the same order as in the config file  
  // -> The orientation is set to a default value 
  // (DO NOT modify)
  configPlanesDrawOrder_default.clear();
  configPlanesLeftRight_default.clear();
  for(iPlane=1; iPlane<=nbPlanes; iPlane++ )
  {
    configPlanesDrawOrder_default.push_back(iPlane);
    configPlanesLeftRight_default.push_back('?');
  }
  //------------------------------
  // 2) Custom pattern 1 
  // -> In this exemple, 8 planes are drawn in a specific order (user preferences)
  // Can be adjusted : you must have as many push_back() as the number of planes !
  configPlanesDrawOrder_custom1.clear();
  configPlanesLeftRight_custom1.clear();
  configPlanesDrawOrder_custom1.push_back(1); configPlanesLeftRight_custom1.push_back('l');// position 1 in the pad
  configPlanesDrawOrder_custom1.push_back(5); configPlanesLeftRight_custom1.push_back('r');// position 2 in the pad
  configPlanesDrawOrder_custom1.push_back(2); configPlanesLeftRight_custom1.push_back('l');// position 3 in the pad
  configPlanesDrawOrder_custom1.push_back(6); configPlanesLeftRight_custom1.push_back('r');// position 4 in the pad
  configPlanesDrawOrder_custom1.push_back(3); configPlanesLeftRight_custom1.push_back('l');// position 5 in the pad
  configPlanesDrawOrder_custom1.push_back(7); configPlanesLeftRight_custom1.push_back('r');// position 6 in the pad
  configPlanesDrawOrder_custom1.push_back(4); configPlanesLeftRight_custom1.push_back('l');// position 7 in the pad
  configPlanesDrawOrder_custom1.push_back(8); configPlanesLeftRight_custom1.push_back('r');// position 8 in the pad
  // Test if the previous configuration is coherent with the number of planes
  // (DO NOT modify)
  if ((nbPlanes != int(configPlanesDrawOrder_custom1.size())) || (nbPlanes != int(configPlanesLeftRight_custom1.size()))  )
  {
      printf_warning("ConfigureDisplays");
      printf("The display draw order configuration is not coherent with the number of planes ! \n");
      printf("There is %d planes procesed by TAF and your configPlanesXX_custom1 vectors contains %d and %d values",nbPlanes,int(configPlanesDrawOrder_custom1.size()),int(configPlanesLeftRight_custom1.size()));
      printf("A default configuration has been set");
      
      configPlanesDrawOrder_custom1.clear();
      configPlanesLeftRight_custom1.clear();
      for(iPlane=1; iPlane<=nbPlanes; iPlane++) 
      {
	configPlanesDrawOrder_custom1.push_back(iPlane);
	configPlanesLeftRight_custom1.push_back('?');
      }
  }
  //------------------------------
  
  //------------------------------
  // Markers style and color
  //------------------------------
  
  // Style to draw a hit on Pixels
  HitMarkerStyle_Cluster[0]=24;//1-Left
  HitMarkerStyle_Cluster[1]=25;//2-Left
  HitMarkerStyle_Cluster[2]=26;//3-Left
  HitMarkerStyle_Cluster[3]=27;//4-Left
  HitMarkerStyle_Cluster[4]=24;//1-Right
  HitMarkerStyle_Cluster[5]=25;//2-Right
  HitMarkerStyle_Cluster[6]=26;//3-Right
  HitMarkerStyle_Cluster[7]=27;//4-Right

  // Style to draw a hit associated to a track
  HitMarkerStyle_Tracked[0]=24;//1-Left
  HitMarkerStyle_Tracked[1]=25;//2-Left
  HitMarkerStyle_Tracked[2]=26;//3-Left
  HitMarkerStyle_Tracked[3]=27;//4-Left
  HitMarkerStyle_Tracked[4]=24;//1-Right
  HitMarkerStyle_Tracked[5]=25;//2-Right
  HitMarkerStyle_Tracked[6]=26;//3-Right
  HitMarkerStyle_Tracked[7]=27;//4-Right

  // Style to draw a hit no associated to a track
  HitMarkerStyle_UnTracked[0]=3;//1-Left
  HitMarkerStyle_UnTracked[1]=3;//2-Left
  HitMarkerStyle_UnTracked[2]=3;//3-Left
  HitMarkerStyle_UnTracked[3]=3;//4-Left
  HitMarkerStyle_UnTracked[4]=3;//1-Right
  HitMarkerStyle_UnTracked[5]=3;//2-Right
  HitMarkerStyle_UnTracked[6]=3;//3-Right
  HitMarkerStyle_UnTracked[7]=3;//4-Right

  // Color to draw hit
  HitMarkerColor[0]=1;//1-Left
  HitMarkerColor[1]=6;//2-Left
  HitMarkerColor[2]=3;//3-Left
  HitMarkerColor[3]=4;//4-Left
  HitMarkerColor[4]=1;//1-Right
  HitMarkerColor[5]=6;//2-Right
  HitMarkerColor[6]=3;//3-Right
  HitMarkerColor[7]=4;//4-Right

  // Color of lines used to draw planes borders :
  LineColor[0]=1;
  LineColor[1]=2;
  LineColor[2]=3;
  LineColor[3]=4;
  LineColor[4]=1;
  LineColor[5]=2;
  LineColor[6]=3;
  LineColor[7]=4;

  // Color of lines used to draw tracks (cyclic)
  TracksLinesColor[0]=1;
  TracksLinesColor[1]=2;
  TracksLinesColor[2]=3;
  TracksLinesColor[3]=4;
  TracksLinesColor[4]=5;
  TracksLinesColor[5]=6;
  TracksLinesColor[6]=7;
  TracksLinesColor[7]=8;
  TracksLinesColor[8]=9;
  
  
  
  
  //------------------------------
  // OverLap position for boards with 2 planes
  //------------------------------  
  // For board with 2 planes, like FIRST planes (2*Mi26), an overlap exists between 2 sensors
  // This overlap is along columns = fixed row number
  // This parameter is used for some display methods
  // See ComputeOverlap() method to compute
  //------------------------------
  configOverlapLineRowNumber=16; 
  //------------------------------
  
  return 0;
}

//______________________________________________________________________________________________________
//
//	Display pixels, hits, tracks, vertices
//______________________________________________________________________________________________________

  //****************************************************************
  //		Pixels in Cmos Frame [Pixels]
  //
  // Display pixels, for each plane, with axis unit in [Pixels]
  //  in the CMOS natural frame :
  //   X axis = plane columns
  //   Y axis = plane rows
  // Hits can be shown as markers around pixels
  //
  //****************************************************************
Int_t MRax::DisplayPixelsInCmosFrameInPixel_Init(void)
{
  if (flagDisplayPixelsInCmosFrameInPixel_Init) return -1;
  eventsProcessedDisplayPixelsInCmosFrameInPixel=0;
  //---------------------
  // Configuration
  //---------------------
  // Planes draw order 
  //DisplayPixelsInCmosFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  DisplayPixelsInCmosFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);
  //---------------------
  
  // Canvas
  canvasDisplayPixelsInCmosFrameInPixel = new TCanvas("canvasDisplayPixelsInCmosFrameInPixel", "Pixels - CMOS frame [pixels]", 5, 5, 800, 700);
  canvasDisplayPixelsInCmosFrameInPixel->Clear();
  canvasDisplayPixelsInCmosFrameInPixel->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Pixels - CMOS  frame [pixels] - Run %d", RunNumber);
  labelDisplayPixelsInCmosFrameInPixel = new TPaveLabel();
  labelDisplayPixelsInCmosFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayPixelsInCmosFrameInPixel = new TPad("padDisplayPixelsInCmosFrameInPixel","",0.,0.,1.,0.965);
  padDisplayPixelsInCmosFrameInPixel->Draw();
  padDisplayPixelsInCmosFrameInPixel->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);
  
  // Histos creation and setting
  histoDisplayPixelsInCmosFrameInPixel_RAW = new TH2I*[nbPlanes];
  histoDisplayPixelsInCmosFrameInPixel_HIT = new TH2I*[nbPlanes];
  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX, nbBinY; //   Axis size 

  // OverLap Lines
  lineDisplayPixelsInCmosFrameInPixel = new TLine*[nbPlanes];

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause iPlane!=PlanesDrawOrder[iPlane-1];
  {
    padDisplayPixelsInCmosFrameInPixel->cd(iPlane);
    jPlane = DisplayPixelsInCmosFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    nbBinX = tPlane->GetStripsNu(); // Number of columns
    nbBinY = tPlane->GetStripsNv(); // Number of rows
    
    // Histo creation & parameters
    //   Pixels
    sprintf( name, "histoDisplayPixelsInCmosFrameInPixel_RAW_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->SetStats(kFALSE);
    histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->GetYaxis()->SetTitleOffset(1.5);
    histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->GetXaxis()->SetTitle("Columns # [pix]");
    histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->GetYaxis()->SetTitle("Rows # [pix]");
    //   Hits
    sprintf( name, "histoDisplayPixelsInCmosFrameInPixel_HIT_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayPixelsInCmosFrameInPixel_HIT[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayPixelsInCmosFrameInPixel_HIT[jPlane-1]->SetStats(kFALSE);
    histoDisplayPixelsInCmosFrameInPixel_HIT[jPlane-1]->SetMarkerSize(1.8);
    histoDisplayPixelsInCmosFrameInPixel_HIT[jPlane-1]->SetMarkerStyle(HitMarkerStyle_Cluster[jPlane-1]);
    histoDisplayPixelsInCmosFrameInPixel_HIT[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);
    //	OverLap Lines
    lineDisplayPixelsInCmosFrameInPixel[jPlane-1] = new TLine();
  }
  flagDisplayPixelsInCmosFrameInPixel_Init=1;
  return 0;
}

Int_t MRax::DisplayPixelsInCmosFrameInPixel_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayPixelsInCmosFrameInPixel")) return -1;
  eventsProcessedDisplayPixelsInCmosFrameInPixel+=1;
  // Declarations
  Int_t Xbin,Ybin; // Positions of fired pixels or hits in 2D histogram

  // Filling
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayPixelsInCmosFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    aList  = tPlane->GetListOfPixels();
    // Pixels
    for(Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++)
    {
      Xbin = aList->at(iPix)->GetPixelColumn();// +1 because the first bin is (1,1) but the first pixel is (0,0)
      Ybin = aList->at(iPix)->GetPixelLine();
      histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->Fill( Xbin, Ybin, 1);
    }
    // Hits
    for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
    {
      aHit = (DHit*)tPlane->GetHit(iHit);
      Xbin = (aHit->GetPositionUhit()/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2);
      Ybin = (aHit->GetPositionVhit()/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2);
      histoDisplayPixelsInCmosFrameInPixel_HIT[jPlane-1]->Fill(Xbin, Ybin, 1);
    }
  }
  return 0;
}

Int_t MRax::DisplayPixelsInCmosFrameInPixel_Show(Bool_t WithHits, Bool_t WithOverlapLine,Bool_t WithZbar, Int_t minPixOccu)
{
  if (!gROOT->FindObject("canvasDisplayPixelsInCmosFrameInPixel")) return -1;

  // Parameters
  Char_t DrawOption[10];
  if (WithZbar) sprintf(DrawOption,"colZ");
  else sprintf(DrawOption,"col");

  // Declarations
  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX;
  //Int_t nbBinY; //   Axis size 

  // Histos
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    padDisplayPixelsInCmosFrameInPixel->cd(iPlane);
    jPlane = DisplayPixelsInCmosFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    nbBinX = tPlane->GetStripsNu(); // Number of columns
    //nbBinY = tPlane->GetStripsNv(); // Number of rows
    
    // Pixels
    histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->SetMinimum(minPixOccu);
    histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->Draw(DrawOption);// _VR_ "col" needed no show pixels as squares in TH2F, "AH" = Axis Hiden
    // Hits
    if ( WithHits )
    {
      histoDisplayPixelsInCmosFrameInPixel_HIT[jPlane-1]->Draw("Psame");// DrawCopy ?
    }
    if ( WithOverlapLine )
    {
      lineDisplayPixelsInCmosFrameInPixel[jPlane-1]->DrawLine(0,configOverlapLineRowNumber,nbBinX,configOverlapLineRowNumber);
    }
  }
  
  canvasDisplayPixelsInCmosFrameInPixel->cd();
  sprintf(canvasTitle, "Pixels - CMOS  frame [pixels] - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayPixelsInCmosFrameInPixel);
  labelDisplayPixelsInCmosFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  
  canvasDisplayPixelsInCmosFrameInPixel->Update();

  return 0;
}

Int_t MRax::DisplayPixelsInCmosFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayPixelsInCmosFrameInPixel";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayPixelsInCmosFrameInPixel_SaveHistos()
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-histograms in root file in the "results" path
  //-------------
  // Arguments :
  //-------------
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayPixelsInCmosFrameInPixel";
  histosName = "histoPixelsInCmosFrameInPixel_RAW";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  histosFileFullPath =  resultsDirPath;
  histosFileFullPath += "/";
  histosFileFullPath += FileNamePrefix;
  histosFileFullPath += histosName;
  histosFileFullPath += FileNameSuffix;
  histosFileFullPath += ".root";
  
  histosFile = new TFile(histosFileFullPath,"RECREATE");
  
  // Loop on histos to save them
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause iPlane!=PlanesDrawOrder[iPlane-1];
  {
    histoDisplayPixelsInCmosFrameInPixel_RAW[iPlane-1]->Write();
  }

  histosFile->Write();
  
  printf_info("DisplayPixelsInCmosFrameInPixel");
  printf("Histograms saved in file %s\n",histosFileFullPath.Data());
  
  histosFile->Close();
  delete histosFile;
  
  return 0;
}

Int_t MRax::DisplayPixelsInCmosFrameInPixel_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayPixelsInCmosFrameInPixel")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    // Pixels
    histoDisplayPixelsInCmosFrameInPixel_RAW[iPlane-1]->Reset();
    // Hits
    histoDisplayPixelsInCmosFrameInPixel_HIT[iPlane-1]->Reset();
  }
  eventsProcessedDisplayPixelsInCmosFrameInPixel=0;
  return 0;
}

Int_t MRax::DisplayPixelsInCmosFrameInPixel_Kill(void)
{
  if (flagDisplayPixelsInCmosFrameInPixel_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Pixels
      delete histoDisplayPixelsInCmosFrameInPixel_RAW[iPlane-1];
      // Hits
      delete histoDisplayPixelsInCmosFrameInPixel_HIT[iPlane-1];
      // Lines
      delete lineDisplayPixelsInCmosFrameInPixel[iPlane-1];
    }
    delete [] histoDisplayPixelsInCmosFrameInPixel_RAW;
    delete [] histoDisplayPixelsInCmosFrameInPixel_HIT;
    delete [] lineDisplayPixelsInCmosFrameInPixel;
  }
  if (gROOT->FindObject("canvasDisplayPixelsInCmosFrameInPixel"))
  {
    delete padDisplayPixelsInCmosFrameInPixel;
    delete labelDisplayPixelsInCmosFrameInPixel;
    canvasDisplayPixelsInCmosFrameInPixel->Close();
    delete canvasDisplayPixelsInCmosFrameInPixel;
  }
  flagDisplayPixelsInCmosFrameInPixel_Init=0;
  eventsProcessedDisplayPixelsInCmosFrameInPixel=0;
  return 0;
}

  //****************************************************************
  //            Noise Study and Veto Pixels code generator
  //
  // Need DisplayPixelsInCmosFrameInPixel
  //
  //****************************************************************
Int_t MRax::GenerateVetoPixelsCode(UInt_t absoluteThreshold)
{
  if (!gROOT->FindObject("canvasDisplayPixelsInCmosFrameInPixel")) 
  {
    printf_error("GenerateVetoPixelsCode"); printf("DisplayPixelsInCmosFrameInPixel must exists !\n");
    return -1;
  }
  
  Double_t relThreshold = Double_t(absoluteThreshold) / Double_t(eventsProcessedDisplayPixelsInCmosFrameInPixel) ;
  Char_t CharRelThreshold[20];
  sprintf(CharRelThreshold,"%.3e",relThreshold);
  
  // Output file name
  aFileName = fSession->GetResultDirName();
  aFileName += "/";
  aFileName += "vetoPixelsCode_run";
  aFileName += RunNumber;
  aFileName += "_Threshold";
  aFileName += CharRelThreshold;
  aFileName += ".cxx";
  
  fTool.LocalizeDirName(&aFileName);
  
  FILE *vetoPixelsFile = fopen(aFileName.Data(),"w");
  if (!vetoPixelsFile) 
  {
    printf_error("GenerateVetoPixelsCode");  cout << "Can not create file: "<< aFileName << ", quit !" << endl;
    return -1;
  }
  printf_info("GenerateVetoPixelsCode"); cout << "Generating veto pixels code in file: "<< aFileName << endl;
  
  TDatime current;
  
  
  fprintf( vetoPixelsFile, "///////////////////////////////////////////////////\n");
  fprintf( vetoPixelsFile, "//               Veto Pixels Code                //\n");
  fprintf( vetoPixelsFile, "///////////////////////////////////////////////////\n");
  fprintf( vetoPixelsFile, "// Instructions: \n");
  fprintf( vetoPixelsFile, "// * Copy/paste this function in file:\n");
  fprintf( vetoPixelsFile, "//     $TAFDIR/code/include/vetoPixels.c\n");
  fprintf( vetoPixelsFile, "// * Rename this function name\n");
  fprintf( vetoPixelsFile, "// * Edit function DGlobalTools::SetVetoPixel(int noiseRun)\n");
  fprintf( vetoPixelsFile, "//     in file $TAFDIR/code/src/DGlobalTools.cxx\n");
  fprintf( vetoPixelsFile, "// * Build TAF\n");
  fprintf( vetoPixelsFile, "//     make clean is MANDATORY !\n");
  fprintf( vetoPixelsFile, "// * Update your config file (NoiseRun)\n");
  fprintf( vetoPixelsFile, "//\n");
  fprintf( vetoPixelsFile, "// Tips:\n");
  fprintf( vetoPixelsFile, "// * You should factorise this code\n");
  fprintf( vetoPixelsFile, "//     for common noisy rows & columns !\n");
  fprintf( vetoPixelsFile, "///////////////////////////////////////////////////\n"); 
  fprintf( vetoPixelsFile, "\n\n");
  
  fprintf( vetoPixelsFile, "bool vetoPixel_VERSION ( int chipNb, int row, int col)\n");
  fprintf( vetoPixelsFile, "{\n");
  
  fprintf( vetoPixelsFile, "///////////////////////////////////////////////////\n");
  fprintf( vetoPixelsFile, "//            Veto Pixels List Code              //\n");
  fprintf( vetoPixelsFile, "// Generated with MRax::GenerateVetoPixelsCode() //\n");
  fprintf( vetoPixelsFile, "///////////////////////////////////////////////////\n");
  fprintf( vetoPixelsFile, "// Generated on %u/%u/%u at %u:%u:%u\n",current.GetDay(),current.GetMonth(),current.GetYear(),current.GetHour(),current.GetMinute(),current.GetSecond());
  fprintf( vetoPixelsFile, "// * RUN: %d\n",RunNumber);
  fprintf( vetoPixelsFile, "// * Events cumulated: %d\n",eventsProcessedDisplayPixelsInCmosFrameInPixel);
  fprintf( vetoPixelsFile, "// * Absolute threshold: %d\n", absoluteThreshold);
  fprintf( vetoPixelsFile, "// * Relative threshold: %.3e\n",relThreshold);
  fprintf( vetoPixelsFile, "///////////////////////////////////////////////////\n"); 
  fprintf( vetoPixelsFile, "\n\n");
  
  fprintf( vetoPixelsFile,"\treturn\n");
  fprintf( vetoPixelsFile,"\t\t0\n");

  Int_t row, col, nbRow, nbCol;
  Int_t acqInputNumber;
  Int_t chipNumber;
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ )
  {
    jPlane = DisplayPixelsInCmosFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    nbCol = tPlane->GetStripsNu(); // Number of columns
    nbRow = tPlane->GetStripsNv(); // Number of rows
    acqInputNumber = tPlane->GetAcqInputNumber();
    chipNumber = acqInputNumber - 1; 
    
    fprintf( vetoPixelsFile, "//////////////////////////////////////////\n");
    fprintf( vetoPixelsFile, "//  Plane %d ; name = %s ; InputNumber = %d ; ChipNumber = %d\n", jPlane,tPlane->GetPlanePurpose(),acqInputNumber, chipNumber );     
    fprintf( vetoPixelsFile, "//////////////////////////////////////////\n"); 
    fprintf( vetoPixelsFile, "\n");
    
    fprintf( vetoPixelsFile, "\t\t|| chipNb==%d &&\n",chipNumber);
    fprintf( vetoPixelsFile, "\t\t\t(0\n");

    for (row = 0; row < nbRow; row++)
    {
      for (col = 0; col < nbCol; col++)
      {
        if (histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->GetBinContent(col+1, row+1) >= absoluteThreshold)
        {
          fprintf( vetoPixelsFile, "\t\t\t|| (row==%d  && col==%d)\n",row,col);
        }
      }
    }
     
    fprintf( vetoPixelsFile, "\t\t\t)\n");  
        
    fprintf( vetoPixelsFile, "\n");
    fprintf( vetoPixelsFile, "//////////////////////////////////////////\n"); 
    fprintf( vetoPixelsFile, "\n");
  }
  
  fprintf( vetoPixelsFile, "\t;\n");
  fprintf( vetoPixelsFile, "}\n");  
  
  fclose(vetoPixelsFile);
  printf_info("GenerateVetoPixelsCode"); cout << "Finished !"<< endl;
  return 0;
}

Int_t MRax::DisplayNoiseStudy_Init(void)
{
  if (flagDisplayNoiseStudy_Init) return -1;
  //---------------------
  // Configuration
  //---------------------
  // Planes draw order 
  //DisplayNoiseStudy_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  DisplayNoiseStudy_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);
  //---------------------
  
  // Canvas
  canvasDisplayNoiseStudy = new TCanvas("canvasDisplayNoiseStudy", "Noise Study", 5, 5, 800, 700);
  canvasDisplayNoiseStudy->Clear();
  canvasDisplayNoiseStudy->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Noise Study - Run %d", RunNumber);
  labelDisplayNoiseStudy = new TPaveLabel();
  labelDisplayNoiseStudy->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayNoiseStudy = new TPad("padDisplayNoiseStudy","",0.,0.,1.,0.965);
  padDisplayNoiseStudy->Draw();
  //padDisplayNoiseStudy->SetLogx(1);
  padDisplayNoiseStudy->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);
  
  // graphs creation and setting
  graphDisplayNoiseStudy = new TGraph*[nbPlanes];

  flagDisplayNoiseStudy_Init=1;
  return 0;
}

Int_t MRax::DisplayNoiseStudy_Show(Float_t relOccur_min, Float_t relOccur_max, UInt_t relOccur_steps, Bool_t xAxisLog0_Lin1)
{
  
  if (!gROOT->FindObject("canvasDisplayNoiseStudy")) return -1;
  
  if(relOccur_min <= 0 || relOccur_max < 0 || relOccur_max <= relOccur_min || relOccur_steps < 2)
  {
    printf_error("DisplayNoiseStudy_Show");
    printf("The relative occurrences must be >= 0, min must be < max and the number of step must be > 1 ! \n");
  }
  
  if(eventsProcessedDisplayPixelsInCmosFrameInPixel <= 2)
  {
    printf_error("DisplayNoiseStudy_Show");
    printf("The known number of proccessed events is very low ! (%d) \n", eventsProcessedDisplayPixelsInCmosFrameInPixel);
  }
  // Parameters
  

  // prepare X axis
  unsigned int iStep;
  //Float_t occurence;
  Float_t Xstep     = (relOccur_max-relOccur_min)/(Float_t(relOccur_steps)-1);
  Float_t Xstep_log = log10(relOccur_max/relOccur_min)/Float_t(relOccur_steps-1);
  Float_t *XstepsR = new Float_t[relOccur_steps];//relative
  Int_t   *XstepsA = new Int_t[relOccur_steps];//absolute
  
  if(configBoolVerbose) 
  {
    printf_info("DisplayNoiseStudy_Show");
    cout << "Steps = " << relOccur_steps << " ; Relative min threshold = " << relOccur_min << " ; max = " << relOccur_max << " ; events = " << eventsProcessedDisplayPixelsInCmosFrameInPixel << endl;
  }
  
  for (iStep=0; iStep < int(relOccur_steps) ; iStep++)
  {
    //log
    if(!xAxisLog0_Lin1) 
    {
      XstepsR[iStep] = relOccur_min * pow(10,Xstep_log*iStep);
      XstepsA[iStep] = XstepsR[iStep] * eventsProcessedDisplayPixelsInCmosFrameInPixel ; 
      if(configBoolVerbose) cout << "Step # " << iStep << " : Relative threshold : " << XstepsR[iStep] << " ; Absolute threshold : " << XstepsA[iStep] << endl;
    }
    //linear
    else
    {
      XstepsR[iStep] = relOccur_min + Xstep * iStep;
      XstepsA[iStep] = XstepsR[iStep] * eventsProcessedDisplayPixelsInCmosFrameInPixel ;
      if(configBoolVerbose) cout << "Step # " << iStep << " : Relative threshold : " << XstepsR[iStep] << " ; Absolute threshold : " << XstepsA[iStep] << endl;
    }
  }
  
  // prepare Y axis
  Float_t **nbPixInPlanOverThresh = new Float_t*[nbPlanes];
  for( Int_t iPlane=1; iPlane<=nbPlanes; iPlane++) nbPixInPlanOverThresh[iPlane-1] = new Float_t[relOccur_steps];
  
  // Fill Arrays
  Int_t row, col, nbRow, nbCol;
  for( iPlane=1; iPlane<=nbPlanes; iPlane++ )
  {
    jPlane = DisplayNoiseStudy_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    nbCol = tPlane->GetStripsNu(); // Number of columns
    nbRow = tPlane->GetStripsNv(); // Number of rows
    
    for (iStep = 0; iStep < int(relOccur_steps) ; iStep++)
    {
      nbPixInPlanOverThresh[jPlane-1][iStep]=0;
      for (row = 0; row < nbRow; row++)
      {
        for (col = 0; col < nbCol; col++)
        {
          if (histoDisplayPixelsInCmosFrameInPixel_RAW[jPlane-1]->GetBinContent(col+1, row+1) >= XstepsA[iStep])
          {
            nbPixInPlanOverThresh[jPlane-1][iStep]+=1;
          }
        }
      }
    }
  }
  
  // Display
  for(iPlane=1; iPlane<=nbPlanes; iPlane++)
  {
    jPlane = DisplayNoiseStudy_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    
    padDisplayNoiseStudy->cd(jPlane);
    if(!xAxisLog0_Lin1) padDisplayNoiseStudy->SetLogx(1);
    graphDisplayNoiseStudy[jPlane-1] = new TGraph(relOccur_steps,XstepsR,nbPixInPlanOverThresh[jPlane-1]);

    sprintf( name, "histoDisplayNoiseStudy_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    
    graphDisplayNoiseStudy[jPlane-1]->SetName(name);
    graphDisplayNoiseStudy[jPlane-1]->SetTitle( title);
    graphDisplayNoiseStudy[jPlane-1]->GetXaxis()->SetTitle("relative occurence");
    graphDisplayNoiseStudy[jPlane-1]->GetYaxis()->SetTitle("nb. of pixels above relative occurence");
    graphDisplayNoiseStudy[jPlane-1]->Draw("AL*");
  }
 
  for( Int_t iPlane=1; iPlane<=nbPlanes; iPlane++) delete[] nbPixInPlanOverThresh[iPlane-1];
  delete[] nbPixInPlanOverThresh;
  
  delete[] XstepsA;
  delete[] XstepsR;
  
  canvasDisplayNoiseStudy->cd();
  sprintf(canvasTitle, "Pixels - CMOS  frame [pixels] - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayPixelsInCmosFrameInPixel);
  labelDisplayNoiseStudy->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  
  canvasDisplayNoiseStudy->Update();

  return 0;
}

Int_t MRax::DisplayNoiseStudy_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayNoiseStudy";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }
  
  return 0;
}

Int_t MRax::DisplayNoiseStudy_SaveHistos()
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-histograms in root file in the "results" path
  //-------------
  // Arguments :
  //-------------
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayNoiseStudy";
  histosName = "histosNoiseStudy";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  histosFileFullPath =  resultsDirPath;
  histosFileFullPath += "/";
  histosFileFullPath += FileNamePrefix;
  histosFileFullPath += histosName;
  histosFileFullPath += FileNameSuffix;
  histosFileFullPath += ".root";
  
  histosFile = new TFile(histosFileFullPath,"RECREATE");
  
  // Loop on histos to save them
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause iPlane!=PlanesDrawOrder[iPlane-1];
  {
    graphDisplayNoiseStudy[iPlane-1]->Write();
  }

  histosFile->Write();
  
  printf_info("DisplayNoiseStudy");
  printf("Histograms saved in file %s\n",histosFileFullPath.Data());
  
  histosFile->Close();
  delete histosFile;
  
  return 0;
}

Int_t MRax::DisplayNoiseStudy_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayNoiseStudy")) return -1;

  delete[] graphDisplayNoiseStudy;
  
  return 0;
}

Int_t MRax::DisplayNoiseStudy_Kill(void)
{
  if (gROOT->FindObject("canvasDisplayNoiseStudy"))
  {
    delete [] graphDisplayNoiseStudy;
    delete padDisplayNoiseStudy;
    canvasDisplayNoiseStudy->Close();
    delete canvasDisplayNoiseStudy;
  }
  flagDisplayNoiseStudy_Init=0;
  return 0;
}



//****************************************************************
  //		Pixels in FIRST board Frame [Pixels] 
  //	!!!! SPECIFIC to FIRST BOARDS with 2*Mi26 Boards !!!!
  //
  // Display pixels, for each plane, with axis unit in [Pixels]
  //  in the frame corresponding to the view of a FIRST board
  //   X and Y axis and origin correspond to the front view of a board
  // Hits can be shown as markers around pixels
  //
  //****************************************************************
Int_t MRax::DisplayPixelsInFirstBoardFrameInPixel_Init(void)
{
  if (flagDisplayPixelsInFirstBoardFrameInPixel_Init) return -1;
  eventsProcessedDisplayPixelsInFirstBoardFrameInPixel=0;
  //---------------------
  // Configuration
  //---------------------
  // Planes draw order 
  //DisplayPixelsInFirstBoardFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  //DisplayPixelsInFirstBoardFrameInPixel_PlanePosition   = (Char_t *)(&configPlanesLeftRight_default[0]);
  
  DisplayPixelsInFirstBoardFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);
  DisplayPixelsInFirstBoardFrameInPixel_PlanePosition   = (Char_t *)(&configPlanesLeftRight_custom1[0]);
  //---------------------
  // New axis max values = numbers of rows and columns of planes, actualy specific to Mi26 sensors !
  Int_t nbRow = 576;
  Int_t nbCol = 1152;
  //---------------------
  
  
  
  // Canvas
  canvasDisplayPixelsInFirstBoardFrameInPixel = new TCanvas("canvasDisplayPixelsInFirstBoardFrameInPixel", "Pixels - Tracker frame [pixels]", 5, 5, 800, 700);
  canvasDisplayPixelsInFirstBoardFrameInPixel->Clear();
  canvasDisplayPixelsInFirstBoardFrameInPixel->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Pixels - Tracker frame [pixels] - Run %d", RunNumber);
  labelDisplayPixelsInFirstBoardFrameInPixel = new TPaveLabel();
  labelDisplayPixelsInFirstBoardFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayPixelsInFirstBoardFrameInPixel = new TPad("padDisplayPixelsInFirstBoardFrameInPixel","",0.,0.,1.,0.965);
  padDisplayPixelsInFirstBoardFrameInPixel->Draw();
  padDisplayPixelsInFirstBoardFrameInPixel->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);

  // New Axis definition
  axisDisplayPixelsInFirstBoardFrameInPixel=new TGaxis*[nbPlanes*2];
  // For Left planes
  faxisDisplayPixelsInFirstBoardFrameInPixel_XLeft=new TF1("faxisDisplayPixelsInFirstBoardFrameInPixel_XLeft","-x",nbRow,0);
  faxisDisplayPixelsInFirstBoardFrameInPixel_YLeft=new TF1("faxisDisplayPixelsInFirstBoardFrameInPixel_YLeft","-x",nbCol,0);
  // For Right planes
  faxisDisplayPixelsInFirstBoardFrameInPixel_XRight=new TF1("faxisDisplayPixelsInFirstBoardFrameInPixel_XRight","x",nbRow,0);
  faxisDisplayPixelsInFirstBoardFrameInPixel_YRight=new TF1("faxisDisplayPixelsInFirstBoardFrameInPixel_YRight","-x",nbCol,0);
  //	OverLap Lines
  lineDisplayPixelsInFirstBoardFrameInPixel = new TLine*[nbPlanes];
  // Histos creation and setting
  histoDisplayPixelsInFirstBoardFrameInPixel_RAW = new TH2I*[nbPlanes];
  histoDisplayPixelsInFirstBoardFrameInPixel_HIT = new TH2I*[nbPlanes];

  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX, nbBinY; //   Axis size 
  
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    padDisplayPixelsInFirstBoardFrameInPixel->cd(iPlane);
    jPlane = DisplayPixelsInFirstBoardFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Axis correspondance & size
    //  This is a specific configuration for FIRST 2*Mi26 Boards
    //  For THIS representation (CMOS turned : (0,0)pixel is at Up Right corner)
    nbBinX = tPlane->GetStripsNv(); // X axis (horizontal) = rows ;
    nbBinY = tPlane->GetStripsNu(); // Y axis (vertical)   = columns ;
    
    // Histo creation & parameters
    //   Pixels
    sprintf( name, "histoDisplayPixelsInFirstBoardFrameInPixel_RAW_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->SetStats(kFALSE);
    histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->GetYaxis()->SetTitleOffset(1.5);
    //   Hits
    sprintf( name, "histoDisplayPixelsInFirstBoardFrameInPixel_HIT_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1]->SetStats(kFALSE);
    histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1]->SetMarkerSize(1.8);
    histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1]->SetMarkerStyle(HitMarkerStyle_Cluster[jPlane-1]);
    histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);
    //	OverLap Lines
    lineDisplayPixelsInFirstBoardFrameInPixel[jPlane-1] = new TLine();

    if (DisplayPixelsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='l')
    {
      histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->GetXaxis()->SetTitle("(576 - row #)");
      histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->GetYaxis()->SetTitle("(1152 - column #)");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]   = new TGaxis(0,nbBinY,nbBinX,nbBinY,"faxisDisplayPixelsInFirstBoardFrameInPixel_XLeft",510,"-");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1] = new TGaxis(nbBinX,0,nbBinX,nbBinY,"faxisDisplayPixelsInFirstBoardFrameInPixel_YLeft",510,"+L");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]  ->SetTitle("<-- rows #");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitle("<-- columns #");
    }
    else if (DisplayPixelsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='r')
    {
      histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->GetXaxis()->SetTitle("(row #)");
      histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->GetYaxis()->SetTitle("(1152 - column #)");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]   = new TGaxis(0,nbBinY,nbBinX,nbBinY,"faxisDisplayPixelsInFirstBoardFrameInPixel_XRight",510,"-");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1] = new TGaxis(nbBinX,0,nbBinX,nbBinY,"faxisDisplayPixelsInFirstBoardFrameInPixel_YRight",510,"+L");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]  ->SetTitle("rows # -->");
      axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitle("<-- columns #");
    }
  }
  flagDisplayPixelsInFirstBoardFrameInPixel_Init=1;
  return 0;
}

Int_t MRax::DisplayPixelsInFirstBoardFrameInPixel_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayPixelsInFirstBoardFrameInPixel")) return -1;
  eventsProcessedDisplayPixelsInFirstBoardFrameInPixel+=1;
  // Declarations

  Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX, nbBinY; //   Axis size 

  // Filling
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayPixelsInFirstBoardFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Axis correspondance & size
    //  This is a specific configuration for FIRST 2*Mi26 Boards
    //  For THIS representation (CMOS turned : (0,0)pixel is at Up Right corner)
    nbBinX = tPlane->GetStripsNv(); // X axis (horizontal) = rows ;
    nbBinY = tPlane->GetStripsNu(); // Y axis (vertical)   = columns ;
    aList  = tPlane->GetListOfPixels();

    if (DisplayPixelsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='l')
    {
      // pixs
      for(int iPix=0; iPix<(int)aList->size(); iPix++)
      {
	Xbin = nbBinX - aList->at(iPix)->GetPixelLine();
	Ybin = nbBinY - aList->at(iPix)->GetPixelColumn();
	histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->Fill( Xbin, Ybin, 1);
      }
      // Hits
      for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
      {
	aHit = (DHit*)tPlane->GetHit(iHit);
	Xbin = nbBinX - (aHit->GetPositionVhit()/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2);
	Ybin = nbBinY - (aHit->GetPositionUhit()/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2);
	histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1]->Fill(Xbin, Ybin, 1);
      }
    }
    else if (DisplayPixelsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='r')
    {
      // pixs
      for(int iPix=0; iPix<(int)aList->size(); iPix++)
      {
	Xbin = aList->at(iPix)->GetPixelLine();
	Ybin = nbBinY - aList->at(iPix)->GetPixelColumn();
	histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->Fill( Xbin, Ybin, 1);
      }
    // Hits
      for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
      {
	aHit = (DHit*)tPlane->GetHit(iHit);
	Xbin = (aHit->GetPositionVhit()/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2);
	Ybin = nbBinY - (aHit->GetPositionUhit()/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2);
	histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1]->Fill(Xbin, Ybin, 1);
      }
    }
  }
  return 0;
}

Int_t MRax::DisplayPixelsInFirstBoardFrameInPixel_Show(Bool_t WithHits, Bool_t WithOverlapLine,Bool_t WithZbar, Int_t minPixOccu)
{
  if (!gROOT->FindObject("canvasDisplayPixelsInFirstBoardFrameInPixel")) return -1;

  // Parameters
  Char_t DrawOption[10];
  if (WithZbar) sprintf(DrawOption,"colZ");
  else sprintf(DrawOption,"col");

  // Declarations
  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX, nbBinY; //   Axis size 

  // Histos
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    padDisplayPixelsInFirstBoardFrameInPixel->cd(iPlane);
    jPlane = DisplayPixelsInFirstBoardFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Axis correspondance & size
    //  This is a specific configuration for FIRST 2*Mi26 Boards
    //  For THIS representation (CMOS turned : (0,0)pixel is at Up Right corner)
    nbBinX = tPlane->GetStripsNv(); // X axis (horizontal) = rows ;
    nbBinY = tPlane->GetStripsNu(); // Y axis (vertical)   = columns ;
    // Pixels
    histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->SetMinimum(minPixOccu);
    histoDisplayPixelsInFirstBoardFrameInPixel_RAW[jPlane-1]->Draw(DrawOption);// _VR_ "col" needed no show pixels as squares in TH2F, "AH" = Axis Hiden

    // Hits
    if ( WithHits )
    {
      histoDisplayPixelsInFirstBoardFrameInPixel_HIT[jPlane-1]->Draw("Psame");
    }
    if ( WithOverlapLine )
    {
      if (DisplayPixelsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='l') lineDisplayPixelsInFirstBoardFrameInPixel[jPlane-1]->DrawLine(nbBinX-configOverlapLineRowNumber,0,nbBinX-configOverlapLineRowNumber,nbBinY);
      if (DisplayPixelsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='r') lineDisplayPixelsInFirstBoardFrameInPixel[jPlane-1]->DrawLine(configOverlapLineRowNumber,0,configOverlapLineRowNumber,nbBinY);
    }

    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetLabelSize(0.03);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetLabelOffset(-0.001);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetLabelColor(2);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetTitleSize(0.03);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetTitleOffset(1.2);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetTitleColor(2);

    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetLabelSize(0.03);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetLabelOffset(0.01);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetLabelColor(2);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitleSize(0.03);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitleOffset(1.3);
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitleColor(2);

    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)]  ->Draw();
    axisDisplayPixelsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->Draw();

  }
  
  canvasDisplayPixelsInFirstBoardFrameInPixel->cd();
  sprintf(canvasTitle, "Pixels - Tracker frame [pixels] - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayPixelsInFirstBoardFrameInPixel);
  labelDisplayPixelsInFirstBoardFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  
  canvasDisplayPixelsInFirstBoardFrameInPixel->Update();

  return 0;
}

Int_t MRax::DisplayPixelsInFirstBoardFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayPixelsInFirstBoardFrameInPixel";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayPixelsInFirstBoardFrameInPixel_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayPixelsInFirstBoardFrameInPixel")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    // Pixels
    histoDisplayPixelsInFirstBoardFrameInPixel_RAW[iPlane-1]->Reset();
    // Hits
    histoDisplayPixelsInFirstBoardFrameInPixel_HIT[iPlane-1]->Reset();
  }
  eventsProcessedDisplayPixelsInFirstBoardFrameInPixel=0;
  return 0;
}

Int_t MRax::DisplayPixelsInFirstBoardFrameInPixel_Kill(void)
{
  if (flagDisplayPixelsInFirstBoardFrameInPixel_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Pixels
      delete histoDisplayPixelsInFirstBoardFrameInPixel_RAW[iPlane-1];
      // Hits
      delete histoDisplayPixelsInFirstBoardFrameInPixel_HIT[iPlane-1];
      // Lines
      delete lineDisplayPixelsInFirstBoardFrameInPixel[iPlane-1];
      // Axis
      delete axisDisplayPixelsInFirstBoardFrameInPixel[2*(iPlane-1)];
      delete axisDisplayPixelsInFirstBoardFrameInPixel[2*(iPlane-1)+1];
    }
    delete faxisDisplayPixelsInFirstBoardFrameInPixel_XLeft;
    delete faxisDisplayPixelsInFirstBoardFrameInPixel_YLeft;
    delete faxisDisplayPixelsInFirstBoardFrameInPixel_XRight;
    delete faxisDisplayPixelsInFirstBoardFrameInPixel_YRight;
    delete [] axisDisplayPixelsInFirstBoardFrameInPixel;
    delete [] histoDisplayPixelsInFirstBoardFrameInPixel_RAW;
    delete [] histoDisplayPixelsInFirstBoardFrameInPixel_HIT;
    delete [] lineDisplayPixelsInFirstBoardFrameInPixel;
  }

  if (gROOT->FindObject("canvasDisplayPixelsInFirstBoardFrameInPixel"))
  {
    delete padDisplayPixelsInFirstBoardFrameInPixel;
    delete labelDisplayPixelsInFirstBoardFrameInPixel;
    canvasDisplayPixelsInFirstBoardFrameInPixel->Close();
    delete canvasDisplayPixelsInFirstBoardFrameInPixel;
  }

  flagDisplayPixelsInFirstBoardFrameInPixel_Init=0;
  eventsProcessedDisplayPixelsInFirstBoardFrameInPixel=0;
  return 0;
}

  //****************************************************************
  //		Hits in Cmos Frame [Pixels] 
  //
  // Display hits, for each plane, with axis unit in [Pixels]
  //  in the CMOS natural frame :
  //   X axis = plane columns
  //   Y axis = plane rows
  // Tracked hits can be shown as markers around pixels
  //
  //****************************************************************
Int_t MRax::DisplayHitsInCmosFrameInPixel_Init(void)
{
  if(flagDisplayHitsInCmosFrameInPixel_Init) return -1;
  eventsProcessedDisplayHitsInCmosFrameInPixel=0;
  //---------------------
  // Configuration
  //---------------------
  // Planes draw order 
  //DisplayHitsInCmosFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  DisplayHitsInCmosFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);
  //---------------------
  
  // Canvas
  canvasDisplayHitsInCmosFrameInPixel = new TCanvas("canvasDisplayHitsInCmosFrameInPixel", "Hits - CMOS frame [pixels]", 5, 5, 800, 700);
  canvasDisplayHitsInCmosFrameInPixel->Clear();
  canvasDisplayHitsInCmosFrameInPixel->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Hits - CMOS frame [pixels] - Run %d", RunNumber);
  labelDisplayHitsInCmosFrameInPixel = new TPaveLabel();
  labelDisplayHitsInCmosFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayHitsInCmosFrameInPixel = new TPad("padDisplayHitsInCmosFrameInPixel","",0.,0.,1.,0.965);
  padDisplayHitsInCmosFrameInPixel->Draw();
  padDisplayHitsInCmosFrameInPixel->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);

  // Histo creation and setting
  histoDisplayHitsInCmosFrameInPixel_unTrack = new TH2I*[nbPlanes];
  histoDisplayHitsInCmosFrameInPixel_Track = new TH2I*[nbPlanes];

  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX;
  Int_t nbBinY; //  Axis correspondance & size

  // OverLap Lines
  lineDisplayHitsInCmosFrameInPixel = new TLine*[nbPlanes];

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    padDisplayHitsInCmosFrameInPixel->cd(iPlane);
    jPlane = DisplayHitsInCmosFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    //   Axis correspondance & size
    nbBinX = tPlane->GetStripsNu(); // Number of columns
    nbBinY = tPlane->GetStripsNv(); // Number of rows
  
    // Histo creation & parameters
    sprintf( name, "histoDisplayHitsInCmosFrameInPixel_unTrack_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->SetStats(kFALSE);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->SetMarkerStyle(HitMarkerStyle_UnTracked[jPlane-1]);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->SetMarkerSize(0.7);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->GetYaxis()->SetTitleOffset(1.5);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->GetXaxis()->SetTitle("Columns # [pix]");
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->GetYaxis()->SetTitle("Rows # [pix]");

    sprintf( name, "histoDisplayHitsInCmosFrameInPixel_Track_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->SetStats(kFALSE);
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->SetMarkerStyle(HitMarkerStyle_Tracked[jPlane-1]);
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->SetMarkerSize(1.5);
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->GetYaxis()->SetTitleOffset(1.5);
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->GetXaxis()->SetTitle("Columns # [pix]");
    histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->GetYaxis()->SetTitle("Rows # [pix]");

    //	OverLap Lines
    lineDisplayHitsInCmosFrameInPixel[jPlane-1] = new TLine();
  }
  flagDisplayHitsInCmosFrameInPixel_Init=1;
  return 0;
}

Int_t MRax::DisplayHitsInCmosFrameInPixel_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInCmosFrameInPixel")) return -1;
  eventsProcessedDisplayHitsInCmosFrameInPixel+=1;
  // Declarations

  Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram

  // Filling
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayHitsInCmosFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    aList  = tPlane->GetListOfPixels();
    // Hits
    for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
    {
      aHit = (DHit*)tPlane->GetHit(iHit);
      Xbin = (aHit->GetPositionUhit()/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2);
      Ybin = (aHit->GetPositionVhit()/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2);
      histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->Fill(Xbin, Ybin, 1);

      if( aHit->GetFound() )
      {
	histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->Fill(Xbin, Ybin, 1);
      }
    }
  }
  return 0;
}

Int_t MRax::DisplayHitsInCmosFrameInPixel_Show(Bool_t ShowAsso2TrackHits, Bool_t WithOverlapLine,Bool_t HitShowedAsPix, Int_t minHitOccu)
{
  if (!gROOT->FindObject("canvasDisplayHitsInCmosFrameInPixel")) return -1;

  // Parameters
  //Char_t DrawOption[10];
  TString DrawOption;
  //if (HitShowedAsPix) sprintf(DrawOption,"colZ");
  //else sprintf(DrawOption,"");
  if (HitShowedAsPix) DrawOption = TString("colZ");
  else DrawOption = TString("");

  // Declarations

  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX;
  //Int_t nbBinY; //  Axis correspondance & size

  // Histos
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    padDisplayHitsInCmosFrameInPixel->cd(iPlane);
    jPlane = DisplayHitsInCmosFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    nbBinX = tPlane->GetStripsNu(); // Number of columns
    //nbBinY = tPlane->GetStripsNv(); // Number of rows
    
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->SetMinimum(minHitOccu);
    histoDisplayHitsInCmosFrameInPixel_unTrack[jPlane-1]->Draw(DrawOption);
    if (ShowAsso2TrackHits) histoDisplayHitsInCmosFrameInPixel_Track[jPlane-1]->Draw("same");
    if ( WithOverlapLine )
    {
      lineDisplayHitsInCmosFrameInPixel[jPlane-1]->DrawLine(0,configOverlapLineRowNumber,nbBinX,configOverlapLineRowNumber);
    }
  }
  
  canvasDisplayHitsInCmosFrameInPixel->cd();
  sprintf(canvasTitle, "Hits - CMOS frame [pixels] - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayHitsInCmosFrameInPixel);
  labelDisplayHitsInCmosFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  canvasDisplayHitsInCmosFrameInPixel->Update();

  return 0;
}

Int_t MRax::DisplayHitsInCmosFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayHitsInCmosFrameInPixel";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayHitsInCmosFrameInPixel_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInCmosFrameInPixel")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    // Hits
    histoDisplayHitsInCmosFrameInPixel_unTrack[iPlane-1]->Reset();
    histoDisplayHitsInCmosFrameInPixel_Track[iPlane-1]->Reset();
  }
  eventsProcessedDisplayHitsInCmosFrameInPixel=0;
  return 0;
}

Int_t MRax::DisplayHitsInCmosFrameInPixel_Kill(void)
{
  if (flagDisplayHitsInCmosFrameInPixel_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayHitsInCmosFrameInPixel_unTrack[iPlane-1];
      delete histoDisplayHitsInCmosFrameInPixel_Track[iPlane-1];
      // Lines
      delete lineDisplayHitsInCmosFrameInPixel[iPlane-1];
    }
    delete [] histoDisplayHitsInCmosFrameInPixel_unTrack;
    delete [] histoDisplayHitsInCmosFrameInPixel_Track;
    delete [] lineDisplayHitsInCmosFrameInPixel;
  }

  if (gROOT->FindObject("canvasDisplayHitsInCmosFrameInPixel"))
  {
    delete padDisplayHitsInCmosFrameInPixel;
    delete labelDisplayHitsInCmosFrameInPixel;
    canvasDisplayHitsInCmosFrameInPixel->Close();
    delete canvasDisplayHitsInCmosFrameInPixel;
  }
  flagDisplayHitsInCmosFrameInPixel_Init=0;
  eventsProcessedDisplayHitsInCmosFrameInPixel=0;
  return 0;
}

  //****************************************************************
  //		Hits in FIRST Frame [pixels]
  //	!!!! SPECIFIC to FIRST BOARDS with 2*Mi26 Boards !!!!
  //
  // Display hits, for each plane, with axis unit in [Pixels]
  //  in the frame corresponding to the view of a FIRST board
  //   X and Y axis and origin correspond to the front view of a board
  // Tracked hits can be shown as markers around pixels
  //
  //****************************************************************
Int_t MRax::DisplayHitsInFirstBoardFrameInPixel_Init(void)
{
  if(flagDisplayHitsInFirstBoardFrameInPixel_Init) return -1;
  eventsProcessedDisplayHitsInFirstBoardFrameInPixel=0;
  //---------------------
  // Configuration
  //---------------------
  // Planes draw order 
  //DisplayHitsInFirstBoardFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  //DisplayHitsInFirstBoardFrameInPixel_PlanePosition   = (Char_t *)(&configPlanesLeftRight_default[0]);
  
  DisplayHitsInFirstBoardFrameInPixel_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);
  DisplayHitsInFirstBoardFrameInPixel_PlanePosition   = (Char_t *)(&configPlanesLeftRight_custom1[0]);
  //---------------------
  // New axis max values = numbers of rows and columns of planes, actualy specific to Mi26 sensors !
  Int_t nbRow = 576;
  Int_t nbCol = 1152;
  //---------------------
  
  // Canvas
  canvasDisplayHitsInFirstBoardFrameInPixel = new TCanvas("canvasDisplayHitsInFirstBoardFrameInPixel", "Hits - Tracker frame [pixels]", 5, 5, 800, 700);
  canvasDisplayHitsInFirstBoardFrameInPixel->Clear();
  canvasDisplayHitsInFirstBoardFrameInPixel->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Hits - Tracker frame [pixels] - Run %d", RunNumber);
  labelDisplayHitsInFirstBoardFrameInPixel = new TPaveLabel();
  labelDisplayHitsInFirstBoardFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayHitsInFirstBoardFrameInPixel = new TPad("padDisplayHitsInFirstBoardFrameInPixel","",0.,0.,1.,0.965);
  padDisplayHitsInFirstBoardFrameInPixel->Draw();
  padDisplayHitsInFirstBoardFrameInPixel->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);

  // New Axis definition
  axisDisplayHitsInFirstBoardFrameInPixel=new TGaxis*[nbPlanes*2];
  // For Left planes
  faxisDisplayHitsInFirstBoardFrameInPixel_XLeft=new TF1("faxisDisplayHitsInFirstBoardFrameInPixel_XLeft","-x",nbRow,0);
  faxisDisplayHitsInFirstBoardFrameInPixel_YLeft=new TF1("faxisDisplayHitsInFirstBoardFrameInPixel_YLeft","-x",nbCol,0);
  // For Right planes
  faxisDisplayHitsInFirstBoardFrameInPixel_XRight=new TF1("faxisDisplayHitsInFirstBoardFrameInPixel_XRight","x",nbRow,0);
  faxisDisplayHitsInFirstBoardFrameInPixel_YRight=new TF1("faxisDisplayHitsInFirstBoardFrameInPixel_YRight","-x",nbCol,0);
  //	OverLap Lines
  lineDisplayHitsInFirstBoardFrameInPixel = new TLine*[nbPlanes];
  // Histos creation and setting
  histoDisplayHitsInFirstBoardFrameInPixel_unTrack = new TH2I*[nbPlanes];
  histoDisplayHitsInFirstBoardFrameInPixel_Track 	= new TH2I*[nbPlanes];

  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX, nbBinY; //   Axis size 
  
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    padDisplayHitsInFirstBoardFrameInPixel->cd(iPlane);
    jPlane = DisplayHitsInFirstBoardFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Axis correspondance & size
    //  This is a specific configuration for FIRST 2*Mi26 Boards
    //  For THIS representation (CMOS turned : (0,0)pixel is at Up Right corner)
    nbBinX = tPlane->GetStripsNv(); // X axis (horizontal) = rows ;
    nbBinY = tPlane->GetStripsNu(); // Y axis (vertical)   = columns ;
    
    // Histo creation & parameters
    //   Hits
    sprintf( name, "histoDisplayHitsInFirstBoardFrameInPixel_unTrack_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->SetStats(kFALSE);
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->GetYaxis()->SetTitleOffset(1.5);
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->SetMarkerStyle(HitMarkerStyle_UnTracked[jPlane-1]);
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->SetMarkerSize(0.7);

    //   Hits associated to a track
    sprintf( name, "histoDisplayHitsInFirstBoardFrameInPixel_Track_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1] = new TH2I(name, title, nbBinX, 0, nbBinX, nbBinY, 0, nbBinY);
    histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1]->SetStats(kFALSE);
    histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1]->SetMarkerSize(1.5);
    histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1]->SetMarkerStyle(HitMarkerStyle_Tracked[jPlane-1]);
    histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);


    //	OverLap Lines
    lineDisplayHitsInFirstBoardFrameInPixel[jPlane-1] = new TLine();

    if (DisplayHitsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='l')
    {
      histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->GetXaxis()->SetTitle("(576 - row #)");
      histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->GetYaxis()->SetTitle("(1152 - column #)");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]   = new TGaxis(0,nbBinY,nbBinX,nbBinY,"faxisDisplayHitsInFirstBoardFrameInPixel_XLeft",510,"-");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1] = new TGaxis(nbBinX,0,nbBinX,nbBinY,"faxisDisplayHitsInFirstBoardFrameInPixel_YLeft",510,"+L");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]  ->SetTitle("<-- rows #");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitle("<-- columns #");
    }
    else if (DisplayHitsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='r')
    {
      histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->GetXaxis()->SetTitle("(row #)");
      histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->GetYaxis()->SetTitle("(1152 - column #)");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]   = new TGaxis(0,nbBinY,nbBinX,nbBinY,"faxisDisplayHitsInFirstBoardFrameInPixel_XRight",510,"-");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1] = new TGaxis(nbBinX,0,nbBinX,nbBinY,"faxisDisplayHitsInFirstBoardFrameInPixel_YRight",510,"+L");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]  ->SetTitle("rows # -->");
      axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitle("<-- columns #");
    }
  }
  flagDisplayHitsInFirstBoardFrameInPixel_Init=1;
  return 0;
}

Int_t MRax::DisplayHitsInFirstBoardFrameInPixel_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInFirstBoardFrameInPixel")) return -1;
  eventsProcessedDisplayHitsInFirstBoardFrameInPixel+=1;
  // Declarations
  Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX, nbBinY; //   Axis size 

  // Filling
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayHitsInFirstBoardFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Axis correspondance & size
    //  This is a specific configuration for FIRST 2*Mi26 Boards
    //  For THIS representation (CMOS turned : (0,0)pixel is at Up Right corner)
    nbBinX = tPlane->GetStripsNv(); // X axis (horizontal) = rows ;
    nbBinY = tPlane->GetStripsNu(); // Y axis (vertical)   = columns ;
    aList  = tPlane->GetListOfPixels();

    if (DisplayHitsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='l')
    {
      // hits
      for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
      {
	aHit = (DHit*)tPlane->GetHit(iHit);
	Xbin = nbBinX - (aHit->GetPositionVhit()/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2);
	Ybin = nbBinY - (aHit->GetPositionUhit()/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2);
	histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->Fill(Xbin, Ybin, 1);
	if (aHit->GetFound()) histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1]->Fill(Xbin, Ybin, 1);
      }
    }
    else if (DisplayHitsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='r')
    {
      // hits
      for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
      {
	aHit = (DHit*)tPlane->GetHit(iHit);
	Xbin = (aHit->GetPositionVhit()/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2);
	Ybin = nbBinY - (aHit->GetPositionUhit()/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2);
	histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->Fill( Xbin, Ybin, 1);
	if (aHit->GetFound()) histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1]->Fill(Xbin, Ybin, 1);
      }
    }
  }
  return 0;
}

Int_t MRax::DisplayHitsInFirstBoardFrameInPixel_Show(Bool_t ShowAsso2TrackHits, Bool_t WithOverlapLine,Bool_t HitShowedAsPix, Int_t minHitOccu)
{
  if (!gROOT->FindObject("canvasDisplayHitsInFirstBoardFrameInPixel")) return -1;

  // Parameters
  //Char_t DrawOption[10];
  TString DrawOption;
  //if (HitShowedAsPix) sprintf(DrawOption,"colZ");
  //else sprintf(DrawOption,"");
  if (HitShowedAsPix) DrawOption = TString("colZ");
  else DrawOption = TString("");

  // Declarations
  //Int_t Xbin,Ybin; // Positions of fired fixels or hit in 2D histogram
  Int_t nbBinX, nbBinY; //   Axis size 

  // Histos
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    padDisplayHitsInFirstBoardFrameInPixel->cd(iPlane);
    jPlane = DisplayHitsInFirstBoardFrameInPixel_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Axis correspondance & size
    //  This is a specific configuration for FIRST 2*Mi26 Boards
    //  For THIS representation (CMOS turned : (0,0)pixel is at Up Right corner)
    nbBinX = tPlane->GetStripsNv(); // X axis (horizontal) = rows ;
    nbBinY = tPlane->GetStripsNu(); // Y axis (vertical)   = columns ;
    
    // Hits
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->SetMinimum(minHitOccu);
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[jPlane-1]->Draw(DrawOption.Data());// _VR_ "col" needed no show pixels as squares in TH2F, "AH" = Axis Hiden
    // Hits assoc. with tracks
    if ( ShowAsso2TrackHits ) histoDisplayHitsInFirstBoardFrameInPixel_Track[jPlane-1]->Draw("same");
    if ( WithOverlapLine )
    {
      if (DisplayHitsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='l') lineDisplayHitsInFirstBoardFrameInPixel[jPlane-1]->DrawLine(nbBinX-configOverlapLineRowNumber,0,nbBinX-configOverlapLineRowNumber,nbBinY);
      if (DisplayHitsInFirstBoardFrameInPixel_PlanePosition[iPlane-1]=='r') lineDisplayHitsInFirstBoardFrameInPixel[jPlane-1]->DrawLine(configOverlapLineRowNumber,0,configOverlapLineRowNumber,nbBinY);
    }

    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetLabelSize(0.03);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetLabelOffset(-0.001);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetLabelColor(2);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetTitleSize(0.03);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetTitleOffset(1.2);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]->SetTitleColor(2);

    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetLabelSize(0.03);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetLabelOffset(0.01);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetLabelColor(2);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitleSize(0.03);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitleOffset(1.3);
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->SetTitleColor(2);

    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)]  ->Draw();
    axisDisplayHitsInFirstBoardFrameInPixel[2*(jPlane-1)+1]->Draw();

  }
  
  canvasDisplayHitsInFirstBoardFrameInPixel->cd();
  sprintf(canvasTitle, "Hits - Tracker frame [pixels] - Run %d - over %d event(s)", RunNumber,eventsProcessedDisplayHitsInFirstBoardFrameInPixel);
  labelDisplayHitsInFirstBoardFrameInPixel->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  canvasDisplayHitsInFirstBoardFrameInPixel->Update();

  return 0;
}

Int_t MRax::DisplayHitsInFirstBoardFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayHitsInFirstBoardFrameInPixel";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayHitsInFirstBoardFrameInPixel_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInFirstBoardFrameInPixel")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    histoDisplayHitsInFirstBoardFrameInPixel_unTrack[iPlane-1]->Reset();
    histoDisplayHitsInFirstBoardFrameInPixel_Track[iPlane-1]->Reset();
  }
  eventsProcessedDisplayHitsInFirstBoardFrameInPixel=0;
  return 0;
}

Int_t MRax::DisplayHitsInFirstBoardFrameInPixel_Kill(void)
{
  if (flagDisplayHitsInFirstBoardFrameInPixel_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayHitsInFirstBoardFrameInPixel_unTrack[iPlane-1];
      delete histoDisplayHitsInFirstBoardFrameInPixel_Track[iPlane-1];
      // Lines
      delete lineDisplayHitsInFirstBoardFrameInPixel[iPlane-1];
      // Axis
      delete axisDisplayHitsInFirstBoardFrameInPixel[2*(iPlane-1)];
      delete axisDisplayHitsInFirstBoardFrameInPixel[2*(iPlane-1)+1];
    }
    delete faxisDisplayHitsInFirstBoardFrameInPixel_XLeft;
    delete faxisDisplayHitsInFirstBoardFrameInPixel_YLeft;
    delete faxisDisplayHitsInFirstBoardFrameInPixel_XRight;
    delete faxisDisplayHitsInFirstBoardFrameInPixel_YRight;
    delete [] axisDisplayHitsInFirstBoardFrameInPixel;
    delete [] histoDisplayHitsInFirstBoardFrameInPixel_unTrack;
    delete [] histoDisplayHitsInFirstBoardFrameInPixel_Track;
    delete [] lineDisplayHitsInFirstBoardFrameInPixel;
  }

  if (gROOT->FindObject("canvasDisplayHitsInFirstBoardFrameInPixel"))
  {
    delete padDisplayHitsInFirstBoardFrameInPixel;
    delete labelDisplayHitsInFirstBoardFrameInPixel;
    canvasDisplayHitsInFirstBoardFrameInPixel->Close();
    delete canvasDisplayHitsInFirstBoardFrameInPixel;
  }
  flagDisplayHitsInFirstBoardFrameInPixel_Init=0;
  eventsProcessedDisplayHitsInFirstBoardFrameInPixel=0;
  return 0;
}

  //****************************************************************
  //		Hits in Tracker Frame [um]
  //****************************************************************
  // 
  // Display hits and plane'borders, for each plane, with axis unit in [um]
  //  in the tracker frame 
  // Tracked hits are shown as markers around pixels
  // Intersection between tracks and planes can be shown
  //
  //****************************************************************
Int_t MRax::DisplayHitsInTrackerFrameInUm_Init(void)
{
  if(flagDisplayHitsInTrackerFrameInUm_Init) return -1;
  eventsProcessedDisplayHitsInTrackerFrameInUm=0;
  //---------------------
  // Configuration
  //---------------------
  //   Display bounds
  Double_t xminh,xmaxh,yminh,ymaxh;
  xminh=-12000; // in um
  yminh=xminh;
  xmaxh=-xminh;
  ymaxh=xmaxh;
  //---------------------
  // Planes draw order 
  //DisplayHitsInTrackerFrameInUm_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  DisplayHitsInTrackerFrameInUm_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);
  //--------------------

  // Canvas
  canvasDisplayHitsInTrackerFrameInUm = new TCanvas("canvasDisplayHitsInTrackerFrameInUm", "Hits - tracker frame [um]", 5, 5, 800, 700);
  canvasDisplayHitsInTrackerFrameInUm->Clear();
  canvasDisplayHitsInTrackerFrameInUm->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Hits - tracker frame [um] - Run %d", RunNumber);
  labelDisplayHitsInTrackerFrameInUm = new TPaveLabel();
  labelDisplayHitsInTrackerFrameInUm->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayHitsInTrackerFrameInUm = new TPad("padDisplayHitsInTrackerFrameInUm","",0.,0.,1.,0.965);
  padDisplayHitsInTrackerFrameInUm->Draw();
  padDisplayHitsInTrackerFrameInUm->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);

  // Histo creation and setting
  histoDisplayHitsInTrackerFrameInUm_HIT 			= new TH2I*[nbPlanes];
  histoDisplayHitsInTrackerFrameInUm_HITasso2track  		= new TH2I*[nbPlanes];
  histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane	= new TH2I*[nbPlanes];

  // Create the histograms
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    padDisplayHitsInTrackerFrameInUm->cd(iPlane);
    jPlane = DisplayHitsInTrackerFrameInUm_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);

     // Histo for hits position in um in telescope frame
    sprintf( name, "histoDisplayHitsInTrackerFrameInUm_HIT_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1] = new TH2I(name, title, 100, xminh, xmaxh, 100, yminh, ymaxh);
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->SetMarkerSize(0.7);
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->SetMarkerStyle(HitMarkerStyle_UnTracked[jPlane-1]);
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->SetStats(kFALSE);
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->GetYaxis()->SetTitleOffset(1.5);
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->GetXaxis()->SetTitle("#vec{x} [um]");
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->GetYaxis()->SetTitle("#vec{y} [um]");

    // Histo for associated with tracks hits
    sprintf( name, "histoDisplayHitsInTrackerFrameInUm_HITasso2track_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    histoDisplayHitsInTrackerFrameInUm_HITasso2track[jPlane-1] = new TH2I(name, title, 100, xminh, xmaxh, 100, yminh, ymaxh);
    histoDisplayHitsInTrackerFrameInUm_HITasso2track[jPlane-1]->SetStats(kFALSE);
    histoDisplayHitsInTrackerFrameInUm_HITasso2track[jPlane-1]->SetMarkerSize(1.5);
    histoDisplayHitsInTrackerFrameInUm_HITasso2track[jPlane-1]->SetMarkerStyle(HitMarkerStyle_Tracked[jPlane-1]);
    histoDisplayHitsInTrackerFrameInUm_HITasso2track[jPlane-1]->SetMarkerColor(HitMarkerColor[jPlane-1]);

    // Histo for tracks intersection with plane
    sprintf( name, "histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[jPlane-1] = new TH2I(name, title, 100, xminh, xmaxh, 100, yminh, ymaxh);
    histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[jPlane-1]->SetMarkerStyle(2);//croix
    histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[jPlane-1]->SetMarkerSize(2);
    histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[jPlane-1]->SetStats(kFALSE);
    histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[jPlane-1]->SetMarkerColor(6);//violette

  }

  flagDisplayHitsInTrackerFrameInUm_Init=1;
  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUm_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUm")) return -1;
  eventsProcessedDisplayHitsInTrackerFrameInUm+=1;

  // Fill
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++)
  {
    jPlane = DisplayHitsInTrackerFrameInUm_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    //padDisplayHitsInTrackerFrameInUm->cd(jPlane);
    tPlane = tTracker->GetPlane(jPlane);

    if( tPlane->GetHitsN() ) // if some hits present
    {
      for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++) //loop on hits (starts at 1 !!)
      {
	aHit = (DHit*)tPlane->GetHit( iHit);
	//printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane)
	histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]->Fill( \
	  tPlane->PlaneToTracker(*(aHit->GetPosition()))(0),\
	  tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
	if( aHit->GetFound() )
	{
	  histoDisplayHitsInTrackerFrameInUm_HITasso2track[jPlane-1]->Fill( \
	    tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), \
	    tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
	}
	if(configBoolVerbose) printf("MRax::DisplayHitsInTrackerFrameInUm_Fill()  pl %d, hit[%d=(%d,%d)=(%f,%f)]%f\n", \
	  iPlane, iHit, aHit->GetIndexSeed()%tPlane->GetStripsNu(), aHit->GetIndexSeed()/tPlane->GetStripsNu(), \
	  aHit->GetPositionUhit(), aHit->GetPositionVhit(), aHit->GetClusterPulseSum());
      }
    }

    for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ )
    {
      aTrack = tTracker->GetTrack(iTrack);
      DR3 posInPlane = aTrack->Intersection(tPlane);
      histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[jPlane-1]->Fill( \
        tPlane->PlaneToTracker(posInPlane)(0), \
        tPlane->PlaneToTracker(posInPlane)(1));
    }
  }
  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUm_Show(Int_t TrackDisplayMode)
{
  // Parameters :
  //     0 : Trackes no displayed
  //     1 : displayed as circles
  //     2 : displayed as text : track nb

  if (!gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUm")) return -1;

  // CMOS Box Drawing
  //   Coordinates : 4/plane
  
  // FIXME, Antonin 7 oct 2015 : proper ISO C++11 declaration to make clang 3.4 happy
  std::vector<std::vector<DR3> >    posPlaneCornerPlanFrame;   posPlaneCornerPlanFrame.resize(nbPlanes);
  std::vector<std::vector<DR3> >    posPlaneCornerTracFrame;   posPlaneCornerTracFrame.resize(nbPlanes);
  std::vector<std::vector<TLine> >  lineBoxPlane;              lineBoxPlane           .resize(nbPlanes);
  for (iPlane = 0; iPlane < nbPlanes; ++iPlane) {// corners
        posPlaneCornerPlanFrame[iPlane].resize( 4 );
        posPlaneCornerTracFrame[iPlane].resize( 4 );
        lineBoxPlane[iPlane].resize( 4 );
  }// end set-up size of the 2D arrays
 
  
  //   Corners indexes
  Short_t BL=0; // Bottom Left
  Short_t BR=1; // Bottom Right
  Short_t TL=2; // Top Left
  Short_t TR=3; // Top Right
  //   Lines indexes
  Short_t L=0; // Left
  Short_t R=1; // Right
  //Short_t T=2; // Top
  //Short_t B=3; // Bottom
  //   Line points
  Double_t xmin,xmax,ymin,ymax;
  Int_t nbCol, nbRow;
  Double_t pitchC, pitchR;

  for ( iPlane=1;iPlane<=nbPlanes;iPlane++ )
  {
    padDisplayHitsInTrackerFrameInUm->cd(iPlane);
    jPlane = DisplayHitsInTrackerFrameInUm_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    nbCol  = tPlane->GetStripsNu();
    nbRow  = tPlane->GetStripsNv(); 
    pitchC = tPlane->GetStripPitch()(0);
    pitchR = tPlane->GetStripPitch()(1);
    
    histoDisplayHitsInTrackerFrameInUm_HIT[jPlane-1]          ->Draw("P");
    histoDisplayHitsInTrackerFrameInUm_HITasso2track[jPlane-1]->Draw("Psame");
    if     ( TrackDisplayMode==1 ) histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[jPlane-1]->Draw("Psame");

    // Boxs Drawing
    posPlaneCornerPlanFrame[jPlane-1][BL].SetValue(-nbCol/2*pitchC,+nbRow/2*pitchR,0);
    posPlaneCornerPlanFrame[jPlane-1][BR].SetValue(+nbCol/2*pitchC,+nbRow/2*pitchR,0);
    posPlaneCornerPlanFrame[jPlane-1][TL].SetValue(-nbCol/2*pitchC,-nbRow/2*pitchR,0);
    posPlaneCornerPlanFrame[jPlane-1][TR].SetValue(+nbCol/2*pitchC,-nbRow/2*pitchR,0);

    posPlaneCornerTracFrame[jPlane-1][BL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[jPlane-1][BL]);
    posPlaneCornerTracFrame[jPlane-1][BR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[jPlane-1][BR]);
    posPlaneCornerTracFrame[jPlane-1][TL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[jPlane-1][TL]);
    posPlaneCornerTracFrame[jPlane-1][TR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[jPlane-1][TR]);

    // Left line (left in plane frame ... anyway)
    xmin = posPlaneCornerTracFrame[jPlane-1][BL](0);
    ymin = posPlaneCornerTracFrame[jPlane-1][BL](1);
    xmax = posPlaneCornerTracFrame[jPlane-1][TL](0);
    ymax = posPlaneCornerTracFrame[jPlane-1][TL](1);

    lineBoxPlane[jPlane-1][L].SetLineColor(LineColor[jPlane-1]);
    lineBoxPlane[jPlane-1][L].DrawLine(xmin,ymin,xmax,ymax);

    // Right line (right in plane frame ... anyway)
    xmin = posPlaneCornerTracFrame[jPlane-1][BR](0);
    ymin = posPlaneCornerTracFrame[jPlane-1][BR](1);
    xmax = posPlaneCornerTracFrame[jPlane-1][TR](0);
    ymax = posPlaneCornerTracFrame[jPlane-1][TR](1);

    lineBoxPlane[jPlane-1][R].SetLineColor(LineColor[jPlane-1]);
    lineBoxPlane[jPlane-1][R].DrawLine(xmin,ymin,xmax,ymax);

    // TOP line (top in plane frame ... anyway)
    xmin = posPlaneCornerTracFrame[jPlane-1][TL](0);
    ymin = posPlaneCornerTracFrame[jPlane-1][TL](1);
    xmax = posPlaneCornerTracFrame[jPlane-1][TR](0);
    ymax = posPlaneCornerTracFrame[jPlane-1][TR](1);

    lineBoxPlane[jPlane-1][R].SetLineColor(LineColor[jPlane-1]);
    lineBoxPlane[jPlane-1][R].DrawLine(xmin,ymin,xmax,ymax);

    // Bottom line (bottom in plane frame ... anyway)
    xmin = posPlaneCornerTracFrame[jPlane-1][BL](0);
    ymin = posPlaneCornerTracFrame[jPlane-1][BL](1);
    xmax = posPlaneCornerTracFrame[jPlane-1][BR](0);
    ymax = posPlaneCornerTracFrame[jPlane-1][BR](1);

    lineBoxPlane[jPlane-1][R].SetLineColor(LineColor[jPlane-1]);
    lineBoxPlane[jPlane-1][R].DrawLine(xmin,ymin,xmax,ymax);
   
  }

  canvasDisplayHitsInTrackerFrameInUm->cd();
  sprintf(canvasTitle, "Hits - tracker frame [um] - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayHitsInTrackerFrameInUm);
  labelDisplayHitsInTrackerFrameInUm->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  canvasDisplayHitsInTrackerFrameInUm->Update();

  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUm_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayHitsInTrackerFrameInUm";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUm_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUm")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    // Hits
    histoDisplayHitsInTrackerFrameInUm_HIT[iPlane-1]                  ->Reset();
    histoDisplayHitsInTrackerFrameInUm_HITasso2track[iPlane-1]        ->Reset();
    histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[iPlane-1]->Reset();
  }
  eventsProcessedDisplayHitsInTrackerFrameInUm=0;
  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUm_Kill(void)
{
  if (flagDisplayHitsInTrackerFrameInUm_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayHitsInTrackerFrameInUm_HIT[iPlane-1];
      delete histoDisplayHitsInTrackerFrameInUm_HITasso2track[iPlane-1];
      delete histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane[iPlane-1];
    }
    delete [] histoDisplayHitsInTrackerFrameInUm_HIT;
    delete [] histoDisplayHitsInTrackerFrameInUm_HITasso2track;
    delete [] histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane;
  }
    
  if (gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUm"))
  {
    delete padDisplayHitsInTrackerFrameInUm;
    delete labelDisplayHitsInTrackerFrameInUm;
    canvasDisplayHitsInTrackerFrameInUm->Close();
    delete canvasDisplayHitsInTrackerFrameInUm;
  }
  flagDisplayHitsInTrackerFrameInUm_Init=0;
  eventsProcessedDisplayHitsInTrackerFrameInUm=0;
  return 0;
}

  //****************************************************************
  //		Hits in Tracker Frame Overlay view [um] 
  //****************************************************************
  // 
  // Display an overlayed view of plane'borders and hits, with axis unit in [um]
  //  in the tracker frame 
  // Hits are shown as empty markers
  //
  //****************************************************************
Int_t MRax::DisplayHitsInTrackerFrameInUmOverlayed_Init(Bool_t alignMode)
{
  if(flagDisplayHitsInTrackerFrameInUmOverlayed_Init) return -1;
  eventsProcessedDisplayHitsInTrackerFrameInUmOverlayed=0;
  // Canvas
  canvasDisplayHitsInTrackerFrameInUmOverlayed = new TCanvas("canvasDisplayHitsInTrackerFrameInUmOverlayed", "Overlayed hits - tracker frame [um]", 5, 5, 800, 700);
  canvasDisplayHitsInTrackerFrameInUmOverlayed->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Overlayed hits - tracker frame [um] - Run %d", RunNumber);
  labelDisplayHitsInTrackerFrameInUmOverlayed = new TPaveLabel();
  labelDisplayHitsInTrackerFrameInUmOverlayed->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayHitsInTrackerFrameInUmOverlayed = new TPad("padDisplayHitsInTrackerFrameInUmOverlayed","",0.,0.,1.,0.965);
  padDisplayHitsInTrackerFrameInUmOverlayed->Draw();
  padDisplayHitsInTrackerFrameInUmOverlayed->cd();
  if(alignMode)
  {
    padDisplayHitsInTrackerFrameInUmOverlayed->SetGridx();
    padDisplayHitsInTrackerFrameInUmOverlayed->SetGridy();
  }
  // Histo creation and setting
  histoDisplayHitsInTrackerFrameInUmOverlayed = new TH2I*[nbPlanes];

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ )
  {
    tPlane = tTracker->GetPlane(iPlane);

    // Histo for hits position in um in telescope frame
    sprintf( name, "histoDisplayHitsInTrackerFrameInUmOverlayed_pl%d", iPlane);
    //sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), iPlane);
    //sprintf(title,"");
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1] = new TH2I(name,"", 1000, -15000, 15000, 1000, -15000, 15000);
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->SetMarkerStyle(HitMarkerStyle_Tracked[iPlane-1]);//All hits are showed, but "Tracked" style is used for comodity
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->SetMarkerSize(1.5);
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->SetMarkerColor(HitMarkerColor[iPlane-1]);
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->SetStats(kFALSE);

    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetXaxis()->SetTitle("#vec{x} [um]");
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetXaxis()->SetLabelSize(0.02);
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetXaxis()->SetTitleOffset(1.3);
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetXaxis()->SetLabelOffset();
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetXaxis()->SetTitleSize(histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetXaxis()->GetTitleSize()/1.5);
    if(alignMode) histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetXaxis()->SetNdivisions(055);

    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetYaxis()->SetTitle("#vec{y} [um]");
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetYaxis()->SetLabelSize(0.02);
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetYaxis()->SetTitleOffset(1.3);
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetYaxis()->SetLabelOffset();
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetYaxis()->SetTitleSize(histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetYaxis()->GetTitleSize()/1.5);
    if(alignMode) histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->GetYaxis()->SetNdivisions(055);
  }

  flagDisplayHitsInTrackerFrameInUmOverlayed_Init=1;
  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUmOverlayed_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUmOverlayed")) return -1;
  eventsProcessedDisplayHitsInTrackerFrameInUmOverlayed+=1;
  // Declarations

  // Fill
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++)
  {
    tPlane = tTracker->GetPlane(iPlane);
    if( tPlane->GetHitsN()>0 )
    {
      for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++) //loop on hits (starts at 1 !!)
      {
	aHit = (DHit*)tPlane->GetHit( iHit); 
	//printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane) 
	histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->Fill(
	  tPlane->PlaneToTracker(*(aHit->GetPosition()))(0),
	  tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1); 

	if(configBoolVerbose) printf("MRax::DisplayHitsInTrackerFrameInUmOverlayed_Fill()  pl %d, hit[%d=(%d,%d)=(%f,%f)]%f\n", \
	  iPlane, iHit, aHit->GetIndexSeed()%tPlane->GetStripsNu(), aHit->GetIndexSeed()/tPlane->GetStripsNu(), \
	  aHit->GetPositionUhit(), aHit->GetPositionVhit(), aHit->GetClusterPulseSum());
      }
    }
  }
  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUmOverlayed_Show(Bool_t *planes2display)
{
  if (!gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUmOverlayed")) return -1;

  // Parameters :
  if(planes2display==NULL)
  {
    planes2display = new Bool_t[nbPlanes];
    for (iPlane=0; iPlane<nbPlanes; iPlane++) planes2display[iPlane]=1;
  }

  // CMOS Box Drawing
  //Int_t Xbin,Ybin;

  // Coordinates
  // FIXME, Antonin 7 oct 2015 : proper ISO C++11 declaration to make clang 3.4 happy
  std::vector<std::vector<DR3> >    posPlaneCornerPlanFrame;   posPlaneCornerPlanFrame.resize(nbPlanes);
  std::vector<std::vector<DR3> >    posPlaneCornerTracFrame;   posPlaneCornerTracFrame.resize(nbPlanes);
  std::vector<std::vector<TLine> >  lineBoxPlane;              lineBoxPlane           .resize(nbPlanes);
  for (iPlane = 0; iPlane < nbPlanes; ++iPlane) {// corners
        posPlaneCornerPlanFrame[iPlane].resize( 4 );
        posPlaneCornerTracFrame[iPlane].resize( 4 );
        lineBoxPlane[iPlane]           .resize( 4 );
  }// end set-up size of the 2D arrays

  
  // Corners indexes
  Int_t BL=0; // Bottom Left
  Int_t BR=1; // Bottom Right
  Int_t TL=2; // Top Left
  Int_t TR=3; // Top Right
  // Lines indexes
  Int_t L=0; // Left
  Int_t R=1; // Right
  //Int_t T=2; // Top
  //Int_t B=3; // Bottom
  //   Line points
  Double_t xmin,xmax,ymin,ymax;
  Int_t nbCol, nbRow;
  Double_t pitchC, pitchR;

  padDisplayHitsInTrackerFrameInUmOverlayed->cd();

  Bool_t firstDraw=1;
  
  for (iPlane=1;iPlane<=nbPlanes;iPlane++)
  {
    tPlane = tTracker->GetPlane(iPlane);
    nbCol  = tPlane->GetStripsNu();
    nbRow  = tPlane->GetStripsNv(); 
    pitchC = tPlane->GetStripPitch()(0);
    pitchR = tPlane->GetStripPitch()(1);
    if (planes2display[iPlane-1])
    {
      // Hits draw
      if (firstDraw)
      {
	histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->Draw("P");
	firstDraw=0;
      }
      else histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->Draw("Psame");

      // Boxs Drawing
      posPlaneCornerPlanFrame[iPlane-1][BL].SetValue(-nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][BR].SetValue(+nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TL].SetValue(-nbCol/2*pitchC,-nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TR].SetValue(+nbCol/2*pitchC,-nbRow/2*pitchR,0);

      posPlaneCornerTracFrame[iPlane-1][BL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BL]);
      posPlaneCornerTracFrame[iPlane-1][BR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BR]);
      posPlaneCornerTracFrame[iPlane-1][TL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TL]);
      posPlaneCornerTracFrame[iPlane-1][TR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TR]);

      // Left line (left in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][BL](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][BL](1);
      xmax = posPlaneCornerTracFrame[iPlane-1][TL](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][TL](1);

      lineBoxPlane[iPlane-1][L].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][L].DrawLine(xmin,ymin,xmax,ymax);

      // Right line (right in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][BR](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][BR](1);
      xmax = posPlaneCornerTracFrame[iPlane-1][TR](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][TR](1);

      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(xmin,ymin,xmax,ymax);

      // TOP line (top in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][TL](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][TL](1);
      xmax = posPlaneCornerTracFrame[iPlane-1][TR](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][TR](1);

      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(xmin,ymin,xmax,ymax);

      // Bottom line (bottom in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][BL](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][BL](1);
      xmax = posPlaneCornerTracFrame[iPlane-1][BR](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][BR](1);

      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(xmin,ymin,xmax,ymax);
    }
  }

  canvasDisplayHitsInTrackerFrameInUmOverlayed->cd();
  sprintf(canvasTitle, "Overlayed hits - tracker frame [um] - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayHitsInTrackerFrameInUmOverlayed);
  labelDisplayHitsInTrackerFrameInUmOverlayed->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  canvasDisplayHitsInTrackerFrameInUmOverlayed->Update();

  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUmOverlayed_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayHitsInTrackerFrameInUmOverlayed";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUmOverlayed_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUmOverlayed")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1]->Reset();
  }
  eventsProcessedDisplayHitsInTrackerFrameInUmOverlayed=0;
  return 0;
}

Int_t MRax::DisplayHitsInTrackerFrameInUmOverlayed_Kill(void)
{
  if (flagDisplayHitsInTrackerFrameInUmOverlayed_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayHitsInTrackerFrameInUmOverlayed[iPlane-1];
    }
    delete [] histoDisplayHitsInTrackerFrameInUmOverlayed;
  }

  if (gROOT->FindObject("canvasDisplayHitsInTrackerFrameInUmOverlayed"))
  {
    delete padDisplayHitsInTrackerFrameInUmOverlayed;
    delete labelDisplayHitsInTrackerFrameInUmOverlayed;
    canvasDisplayHitsInTrackerFrameInUmOverlayed->Close();
    delete canvasDisplayHitsInTrackerFrameInUmOverlayed;
  }
  flagDisplayHitsInTrackerFrameInUmOverlayed_Init=0;
  eventsProcessedDisplayHitsInTrackerFrameInUmOverlayed=0;
  return 0;
}

  //****************************************************************
  //		Tracks in 3D (obviously in Tracker Frame [um])
  //****************************************************************
  // 
  // Display in 3D plane'borders, hits and tracks
  // [un]tracked hits, boarders and tracks can be showed according to the configuration
  // All informations are read for the "tracksVector"
  //****************************************************************
Int_t MRax::DisplayTracksIn3D_Init(void)
{
  if(flagDisplayTracksIn3D_Init) return -1;
  StoreTracks_Init();

// Configuration :
  // Display limits
  DisplayTracksIn3DViewLimitZmin =   -5e3;
  DisplayTracksIn3DViewLimitZmax = +120e3;
  DisplayTracksIn3DViewLimitXmin =  -15e3;
  DisplayTracksIn3DViewLimitXmax =  +15e3;
  DisplayTracksIn3DViewLimitYmin =  -15e3;
  DisplayTracksIn3DViewLimitYmax =  +15e3;


  // Canvas
  canvasDisplayTracksIn3D = new TCanvas("canvasDisplayTracksIn3D", "Tracks 3D - tracker frame [um]", 5, 5, 800, 700);
  canvasDisplayTracksIn3D->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Tracks 3D - tracker frame [um] - Run %d", RunNumber);
  labelDisplayTracksIn3D = new TPaveLabel();
  labelDisplayTracksIn3D->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayTracksIn3D = new TPad("padDisplayTracksIn3D","",0.,0.,1.,0.965);
  padDisplayTracksIn3D->Draw();
  padDisplayTracksIn3D->cd();

  // Histo creation and setting
  sprintf( name, "histoDisplayTracksIn3D");
  //sprintf( title,"");
  histoDisplayTracksIn3D = new TH3I(name,"", 100, DisplayTracksIn3DViewLimitXmin, DisplayTracksIn3DViewLimitXmax, 100, DisplayTracksIn3DViewLimitYmin, DisplayTracksIn3DViewLimitYmax, 100, DisplayTracksIn3DViewLimitZmin, DisplayTracksIn3DViewLimitZmax);
  histoDisplayTracksIn3D->SetStats(kFALSE);

  histoDisplayTracksIn3D->SetXTitle("#vec{x} [um]");
  histoDisplayTracksIn3D->GetXaxis()->SetLabelSize(0.02);
  histoDisplayTracksIn3D->GetXaxis()->SetTitleOffset();
  histoDisplayTracksIn3D->GetXaxis()->SetLabelOffset();
  histoDisplayTracksIn3D->GetXaxis()->SetTitleSize(histoDisplayTracksIn3D->GetXaxis()->GetTitleSize()/2);

  histoDisplayTracksIn3D->SetYTitle("#vec{y} [um]");
  histoDisplayTracksIn3D->GetYaxis()->SetLabelSize(0.02);
  histoDisplayTracksIn3D->GetYaxis()->SetTitleOffset();
  histoDisplayTracksIn3D->GetYaxis()->SetLabelOffset();
  histoDisplayTracksIn3D->GetYaxis()->SetTitleSize(histoDisplayTracksIn3D->GetYaxis()->GetTitleSize()/2);

  histoDisplayTracksIn3D->SetZTitle("#vec{z} [um]");
  histoDisplayTracksIn3D->GetZaxis()->SetLabelSize(0.02);
  histoDisplayTracksIn3D->GetZaxis()->SetTitleOffset();
  histoDisplayTracksIn3D->GetZaxis()->SetLabelOffset();
  histoDisplayTracksIn3D->GetZaxis()->SetTitleSize(histoDisplayTracksIn3D->GetZaxis()->GetTitleSize()/2);


  flagDisplayTracksIn3D_Init=1;
  return 0;
}

Int_t MRax::DisplayTracksIn3D_Show(Bool_t DrawTracks, Bool_t DrawSensors, Bool_t DrawTrackedHits, Bool_t DrawAllHits)
{
  if (!gROOT->FindObject("canvasDisplayTracksIn3D")) return -1;

  padDisplayTracksIn3D->cd();
  histoDisplayTracksIn3D->Draw();

  Double_t x[2], y[2], z[2];
  Double_t HitsPositions[3];

  for (iTrack=0; iTrack<int(tracksVector.size()); iTrack++)
  {
    aTrackStruct=tracksVector.at(iTrack);

    if (DrawTracks)
    {
      z[0] = DisplayTracksIn3DViewLimitZmin;
      z[1] = DisplayTracksIn3DViewLimitZmax;
      x[0] = aTrackStruct.TrackOrigin(0)+aTrackStruct.TrackSlopeZ(0)*z[0];
      x[1] = aTrackStruct.TrackOrigin(0)+aTrackStruct.TrackSlopeZ(0)*z[1];
      y[0] = aTrackStruct.TrackOrigin(1)+aTrackStruct.TrackSlopeZ(1)*z[0];
      y[1] = aTrackStruct.TrackOrigin(1)+aTrackStruct.TrackSlopeZ(1)*z[1];

      TPolyLine3D *line3D = new TPolyLine3D( 2, x, y, z);
      line3D->SetLineWidth(2);
      line3D->SetLineColor(iTrack+1);
      line3D->Draw();
    }


    ///////////////////////////////////
    // Mark Hits used for the track
    //////////////////////////////////
    if (DrawTrackedHits)
    {
      for (iHit=0; iHit<int(aTrackStruct.HitsPositions.size()); iHit++)
      {
	aHitPos=aTrackStruct.HitsPositions.at(iHit);
	HitsPositions[0]=aHitPos(0);
	HitsPositions[1]=aHitPos(1);
	HitsPositions[2]=aHitPos(2);
	TPolyMarker3D *hit = new TPolyMarker3D(1,HitsPositions,4);
	//hit->SetMarkerColor(HitMarkerColor[aTrackStruct.HitsPlanes.at(iHit)-1]);
	hit->SetMarkerColor(iTrack+1);
	//hit->SetMarkerStyle(4);
	//hit->SetMarkerSize(1);
	hit->Draw();
      }
    }
  }
  ///////////////////////////////////
  // Mark All Hits
  //////////////////////////////////
  if (DrawAllHits)
  {
    for (iEventWithHits=0; iEventWithHits<int(hitsVector.size()); iEventWithHits++)
    {
      aHitsStruct=hitsVector.at(iEventWithHits);
      for (iHit=0; iHit<int(aHitsStruct.HitsPositions.size()); iHit++)
      {
	aHitPos=aHitsStruct.HitsPositions.at(iHit);
	HitsPositions[0]=aHitPos(0);
	HitsPositions[1]=aHitPos(1);
	HitsPositions[2]=aHitPos(2);
	TPolyMarker3D *hit = new TPolyMarker3D(1,HitsPositions,3);
	hit->SetMarkerColor(HitMarkerColor[aHitsStruct.HitsPlanes.at(iHit)-1]);
	//hit->SetMarkerSize(1);
	//hit->SetMarkerStyle(3);
	hit->Draw();
      }
    }

  }


  //////////////////
  // CMOS Box Draw
  ///////////////////
  // Coordinates
  // FIXME, Antonin 7 oct 2015 : proper ISO C++11 declaration to make clang 3.4 happy
  std::vector<std::vector<DR3> >            posPlaneCornerPlanFrame;    posPlaneCornerPlanFrame.resize(nbPlanes);
  std::vector<std::vector<DR3> >            posPlaneCornerTracFrame;    posPlaneCornerTracFrame.resize(nbPlanes);
  std::vector<std::vector<TPolyLine3D*> >    lineBoxPlane;               lineBoxPlane           .resize(nbPlanes);
  for (iPlane = 0; iPlane < nbPlanes; ++iPlane) {// corners
        posPlaneCornerPlanFrame[iPlane].resize( 4 );
        posPlaneCornerTracFrame[iPlane].resize( 4 );
        lineBoxPlane[iPlane].resize( 4 );
  }// end set-up size of the 2D arrays
   
  // Corners indexes
  Int_t BL=0; // Bottom Left
  Int_t BR=1; // Bottom Right
  Int_t TL=2; // Top Left
  Int_t TR=3; // Top Right
  // Lines indexes
  Int_t L=0; // Left
  Int_t R=1; // Right
  Int_t T=2; // Top
  Int_t B=3; // Bottom
  
  Int_t nbCol, nbRow;
  Double_t pitchC, pitchR;

  if (DrawSensors)
  {
    for (iPlane=1;iPlane<=nbPlanes;iPlane++)
    {
      tPlane = tTracker->GetPlane(iPlane);
      nbCol  = tPlane->GetStripsNu();
      nbRow  = tPlane->GetStripsNv(); 
      pitchC = tPlane->GetStripPitch()(0);
      pitchR = tPlane->GetStripPitch()(1);

      z[0]=tPlane->GetPosition()(2);
      z[1]=z[0];

      posPlaneCornerPlanFrame[iPlane-1][BL].SetValue(-nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][BR].SetValue(+nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TL].SetValue(-nbCol/2*pitchC,-nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TR].SetValue(+nbCol/2*pitchC,-nbRow/2*pitchR,0);

      posPlaneCornerTracFrame[iPlane-1][BL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BL]);
      posPlaneCornerTracFrame[iPlane-1][BR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BR]);
      posPlaneCornerTracFrame[iPlane-1][TL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TL]);
      posPlaneCornerTracFrame[iPlane-1][TR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TR]);

      // Left line (left in plane frame ... anyway)
      x[0] = posPlaneCornerTracFrame[iPlane-1][BL](0);
      y[0] = posPlaneCornerTracFrame[iPlane-1][BL](1);
      x[1] = posPlaneCornerTracFrame[iPlane-1][TL](0);
      y[1] = posPlaneCornerTracFrame[iPlane-1][TL](1);

      lineBoxPlane[iPlane-1][L]=new TPolyLine3D( 2, x, y, z);
      lineBoxPlane[iPlane-1][L]->SetLineWidth(2);
      lineBoxPlane[iPlane-1][L]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][L]->Draw();

      // Right line (right in plane frame ... anyway)
      x[0] = posPlaneCornerTracFrame[iPlane-1][BR](0);
      y[0] = posPlaneCornerTracFrame[iPlane-1][BR](1);
      x[1] = posPlaneCornerTracFrame[iPlane-1][TR](0);
      y[1] = posPlaneCornerTracFrame[iPlane-1][TR](1);

      lineBoxPlane[iPlane-1][R]=new TPolyLine3D( 2, x, y, z);
      lineBoxPlane[iPlane-1][R]->SetLineWidth(2);
      lineBoxPlane[iPlane-1][R]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R]->Draw();

      // TOP line (top in plane frame ... anyway)
      x[0] = posPlaneCornerTracFrame[iPlane-1][TL](0);
      y[0] = posPlaneCornerTracFrame[iPlane-1][TL](1);
      x[1] = posPlaneCornerTracFrame[iPlane-1][TR](0);
      y[1] = posPlaneCornerTracFrame[iPlane-1][TR](1);

      lineBoxPlane[iPlane-1][T]=new TPolyLine3D( 2, x, y, z);
      lineBoxPlane[iPlane-1][T]->SetLineWidth(2);
      lineBoxPlane[iPlane-1][T]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][T]->Draw();

      // Bottom line (bottom in plane frame ... anyway)
      x[0] = posPlaneCornerTracFrame[iPlane-1][BL](0);
      y[0] = posPlaneCornerTracFrame[iPlane-1][BL](1);
      x[1] = posPlaneCornerTracFrame[iPlane-1][BR](0);
      y[1] = posPlaneCornerTracFrame[iPlane-1][BR](1);

      lineBoxPlane[iPlane-1][B]=new TPolyLine3D( 2, x, y, z);
      lineBoxPlane[iPlane-1][B]->SetLineWidth(2);
      lineBoxPlane[iPlane-1][B]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][B]->Draw();
      }
  }

  
  canvasDisplayTracksIn3D->cd();
  sprintf(canvasTitle, "Tracks 3D - tracker frame [um] - Run %d - over %d event(s)", RunNumber, eventsProcessedStoreTracks);
  labelDisplayTracksIn3D->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayTracksIn3D->Update();

  return 0;
}

Int_t MRax::DisplayTracksIn3D_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayTracksIn3D";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayTracksIn3D_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayTracksIn3D")) return -1;
  histoDisplayTracksIn3D->Reset();

  return 0;
}

Int_t MRax::DisplayTracksIn3D_Kill(void)
{
  if (flagDisplayTracksIn3D_Init)
  {
    delete histoDisplayTracksIn3D;
  }

  if (gROOT->FindObject("canvasDisplayTracksIn3D"))
  {
    delete padDisplayTracksIn3D;
    delete labelDisplayTracksIn3D;
    canvasDisplayTracksIn3D->Close();
    delete canvasDisplayTracksIn3D;
  }
  flagDisplayTracksIn3D_Init=0;
  return 0;
}


  //****************************************************************
  //		Tracks in 2D (obviously in Tracker Frame [um])
  //****************************************************************
  // 
  // Display in 2D plane'borders, hits and tracks
  // [un]tracked hits, boarders and tracks can be showed according to the configuration
  // All informations are read for the "tracksVector"
  //****************************************************************

Int_t MRax::DisplayTracksIn2D_Init(void)
{
  if(flagDisplayTracksIn2D_Init) return -1;
  StoreTracks_Init();

// Configuration :
  // Display limits
  DisplayTracksIn2DViewLimitZmin =   -5e3;
  DisplayTracksIn2DViewLimitZmax = +120e3;
  DisplayTracksIn2DViewLimitXmin =  -15e3;
  DisplayTracksIn2DViewLimitXmax =  +15e3;
  DisplayTracksIn2DViewLimitYmin =  -15e3;
  DisplayTracksIn2DViewLimitYmax =  +15e3;


  // Canvas
  canvasDisplayTracksIn2D = new TCanvas("canvasDisplayTracksIn2D", "Tracks 2D - tracker frame [um]", 5, 5, 800, 700);
  canvasDisplayTracksIn2D->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Tracks 2D - tracker frame [um] - Run %d", RunNumber);
  labelDisplayTracksIn2D = new TPaveLabel();
  labelDisplayTracksIn2D->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayTracksIn2D = new TPad("padDisplayTracksIn2D","",0.,0.,1.,0.965);
  padDisplayTracksIn2D->Draw();
  padDisplayTracksIn2D->Divide(2,1);
  
  /////////////////////////////////////////////////////////////
  // Histo creation and setting
  /////////////////////////////////////////////////////////////
 
  //---------------------
  // XZ
  //---------------------
  padDisplayTracksIn2D->cd(1);

  histoDisplayTracksIn2D_XZ = new TH2I*[3];
  
  // Planes + Tracks
  sprintf( name, "histoDisplayTracksIn2D_XZ");
  //sprintf( title,"");
  histoDisplayTracksIn2D_XZ[0] = new TH2I(name,"", 1000, DisplayTracksIn2DViewLimitXmin, DisplayTracksIn2DViewLimitXmax, 1000, DisplayTracksIn2DViewLimitZmin, DisplayTracksIn2DViewLimitZmax);
  histoDisplayTracksIn2D_XZ[0]->SetStats(kFALSE);
  histoDisplayTracksIn2D_XZ[0]->SetXTitle("#vec{x} [um]");
//   histoDisplayTracksIn2D_XZ[0]->GetXaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_XZ[0]->GetXaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_XZ[0]->GetXaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_XZ[0]->GetXaxis()->SetTitleSize(histoDisplayTracksIn2D_XZ[0]->GetXaxis()->GetTitleSize()/2);
  histoDisplayTracksIn2D_XZ[0]->SetYTitle("#vec{z} [um]");
//   histoDisplayTracksIn2D_XZ[0]->GetYaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_XZ[0]->GetYaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_XZ[0]->GetYaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_XZ[0]->GetYaxis()->SetTitleSize(histoDisplayTracksIn2D_XZ[0]->GetYaxis()->GetTitleSize()/2);
  
  // All Hits
  sprintf( name, "histoDisplayTracksIn2D_XZ_allHits");
  //sprintf( title,"");
  histoDisplayTracksIn2D_XZ[1] = new TH2I(name,"", 1000, DisplayTracksIn2DViewLimitXmin, DisplayTracksIn2DViewLimitXmax, 1000, DisplayTracksIn2DViewLimitZmin, DisplayTracksIn2DViewLimitZmax);
  histoDisplayTracksIn2D_XZ[1]->SetStats(kFALSE);
  histoDisplayTracksIn2D_XZ[1]->SetMarkerStyle(3);
  histoDisplayTracksIn2D_XZ[1]->SetMarkerColor(1);
  histoDisplayTracksIn2D_XZ[1]->SetXTitle("#vec{x} [um]");
//   histoDisplayTracksIn2D_XZ[1]->GetXaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_XZ[1]->GetXaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_XZ[1]->GetXaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_XZ[1]->GetXaxis()->SetTitleSize(histoDisplayTracksIn2D_XZ[1]->GetXaxis()->GetTitleSize()/2);
  histoDisplayTracksIn2D_XZ[1]->SetYTitle("#vec{z} [um]");
//   histoDisplayTracksIn2D_XZ[1]->GetYaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_XZ[1]->GetYaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_XZ[1]->GetYaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_XZ[1]->GetYaxis()->SetTitleSize(histoDisplayTracksIn2D_XZ[1]->GetYaxis()->GetTitleSize()/2);

  // Tracked Hits
  sprintf( name, "histoDisplayTracksIn2D_XZ_trackedHits");
  //sprintf( title,"");
  histoDisplayTracksIn2D_XZ[2] = new TH2I(name,"", 1000, DisplayTracksIn2DViewLimitXmin, DisplayTracksIn2DViewLimitXmax, 1000, DisplayTracksIn2DViewLimitZmin, DisplayTracksIn2DViewLimitZmax);
  histoDisplayTracksIn2D_XZ[2]->SetStats(kFALSE);
  histoDisplayTracksIn2D_XZ[2]->SetMarkerStyle(4);
  histoDisplayTracksIn2D_XZ[2]->SetMarkerColor(1);
  histoDisplayTracksIn2D_XZ[2]->SetXTitle("#vec{x} [um]");
//   histoDisplayTracksIn2D_XZ[2]->GetXaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_XZ[2]->GetXaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_XZ[2]->GetXaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_XZ[2]->GetXaxis()->SetTitleSize(histoDisplayTracksIn2D_XZ[2]->GetXaxis()->GetTitleSize()/2);
  histoDisplayTracksIn2D_XZ[2]->SetYTitle("#vec{z} [um]");
//   histoDisplayTracksIn2D_XZ[2]->GetYaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_XZ[2]->GetYaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_XZ[2]->GetYaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_XZ[2]->GetYaxis()->SetTitleSize(histoDisplayTracksIn2D_XZ[2]->GetYaxis()->GetTitleSize()/2);
  
  //---------------------
  // YZ
  //---------------------
  padDisplayTracksIn2D->cd(2);
  
  histoDisplayTracksIn2D_YZ = new TH2I*[3];
  
  // Planes + Tracks
  sprintf( name, "histoDisplayTracksIn2DYZ");
  //sprintf( title,"");
  histoDisplayTracksIn2D_YZ[0] = new TH2I(name,"", 1000, DisplayTracksIn2DViewLimitXmin, DisplayTracksIn2DViewLimitXmax, 1000, DisplayTracksIn2DViewLimitZmin, DisplayTracksIn2DViewLimitZmax);
  histoDisplayTracksIn2D_YZ[0]->SetStats(kFALSE);
  histoDisplayTracksIn2D_YZ[0]->SetXTitle("#vec{y} [um]");
//   histoDisplayTracksIn2D_YZ[0]->GetXaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_YZ[0]->GetXaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_YZ[0]->GetXaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_YZ[0]->GetXaxis()->SetTitleSize(histoDisplayTracksIn2D_YZ[0]->GetXaxis()->GetTitleSize()/2);
  histoDisplayTracksIn2D_YZ[0]->SetYTitle("#vec{z} [um]");
//   histoDisplayTracksIn2D_YZ[0]->GetYaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_YZ[0]->GetYaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_YZ[0]->GetYaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_YZ[0]->GetYaxis()->SetTitleSize(histoDisplayTracksIn2D_YZ[0]->GetYaxis()->GetTitleSize()/2);
  
  // All Hits
  sprintf( name, "histoDisplayTracksIn2D_YZ_allHits");
  //sprintf( title,"");
  histoDisplayTracksIn2D_YZ[1] = new TH2I(name,"", 1000, DisplayTracksIn2DViewLimitXmin, DisplayTracksIn2DViewLimitXmax, 1000, DisplayTracksIn2DViewLimitZmin, DisplayTracksIn2DViewLimitZmax);
  histoDisplayTracksIn2D_YZ[1]->SetStats(kFALSE);
  histoDisplayTracksIn2D_YZ[1]->SetMarkerStyle(3);
  histoDisplayTracksIn2D_YZ[1]->SetMarkerColor(1);
  histoDisplayTracksIn2D_YZ[1]->SetXTitle("#vec{x} [um]");
//   histoDisplayTracksIn2D_YZ[1]->GetXaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_YZ[1]->GetXaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_YZ[1]->GetXaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_YZ[1]->GetXaxis()->SetTitleSize(histoDisplayTracksIn2D_YZ[1]->GetXaxis()->GetTitleSize()/2);
  histoDisplayTracksIn2D_YZ[1]->SetYTitle("#vec{z} [um]");
//   histoDisplayTracksIn2D_YZ[1]->GetYaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_YZ[1]->GetYaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_YZ[1]->GetYaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_YZ[1]->GetYaxis()->SetTitleSize(histoDisplayTracksIn2D_YZ[1]->GetYaxis()->GetTitleSize()/2);

  // Tracked Hits
  sprintf( name, "histoDisplayTracksIn2D_YZ_trackedHits");
  //sprintf( title,"");
  histoDisplayTracksIn2D_YZ[2] = new TH2I(name,"", 1000, DisplayTracksIn2DViewLimitXmin, DisplayTracksIn2DViewLimitXmax, 1000, DisplayTracksIn2DViewLimitZmin, DisplayTracksIn2DViewLimitZmax);
  histoDisplayTracksIn2D_YZ[2]->SetStats(kFALSE);
  histoDisplayTracksIn2D_YZ[2]->SetMarkerStyle(4);
  histoDisplayTracksIn2D_YZ[2]->SetMarkerColor(1);
  histoDisplayTracksIn2D_YZ[2]->SetXTitle("#vec{x} [um]");
//   histoDisplayTracksIn2D_YZ[2]->GetXaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_YZ[2]->GetXaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_YZ[2]->GetXaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_YZ[2]->GetXaxis()->SetTitleSize(histoDisplayTracksIn2D_YZ[2]->GetXaxis()->GetTitleSize()/2);
  histoDisplayTracksIn2D_YZ[2]->SetYTitle("#vec{z} [um]");
//   histoDisplayTracksIn2D_YZ[2]->GetYaxis()->SetLabelSize(0.02);
//   histoDisplayTracksIn2D_YZ[2]->GetYaxis()->SetTitleOffset();
//   histoDisplayTracksIn2D_YZ[2]->GetYaxis()->SetLabelOffset();
//   histoDisplayTracksIn2D_YZ[2]->GetYaxis()->SetTitleSize(histoDisplayTracksIn2D_YZ[2]->GetYaxis()->GetTitleSize()/2); 
  
  flagDisplayTracksIn2D_Init=1;
  return 0;
}

Int_t MRax::DisplayTracksIn2D_Show(Bool_t DrawTracks, Bool_t DrawSensors, Bool_t DrawTrackedHits, Bool_t DrawAllHits)
{
  if (!gROOT->FindObject("canvasDisplayTracksIn2D")) return -1;

  
  Double_t x[2], y[2], z[2];
  //Double_t HitsPositions[3];
  //---------------------
  // XZ
  //---------------------  
  padDisplayTracksIn2D->cd(1);
  histoDisplayTracksIn2D_XZ[0]->Draw();


  for (iTrack=0; iTrack<int(tracksVector.size()); iTrack++)
  {
    aTrackStruct=tracksVector.at(iTrack);

    if (DrawTracks)
    {
      z[0] = DisplayTracksIn2DViewLimitZmin;
      z[1] = DisplayTracksIn2DViewLimitZmax;
      x[0] = aTrackStruct.TrackOrigin(0)+aTrackStruct.TrackSlopeZ(0)*z[0];
      x[1] = aTrackStruct.TrackOrigin(0)+aTrackStruct.TrackSlopeZ(0)*z[1];
      y[0] = aTrackStruct.TrackOrigin(1)+aTrackStruct.TrackSlopeZ(1)*z[0];
      y[1] = aTrackStruct.TrackOrigin(1)+aTrackStruct.TrackSlopeZ(1)*z[1];

      TLine lineTrack;
      lineTrack.SetLineColor(iTrack+1);
      lineTrack.DrawLine(x[0],z[0],x[1],z[1]);      
    }


    ///////////////////////////////////
    // Mark Hits used for the track
    //////////////////////////////////
    if (DrawTrackedHits)
    {
      for (iHit=0; iHit<int(aTrackStruct.HitsPositions.size()); iHit++)
      {
	histoDisplayTracksIn2D_XZ[2]->Fill(aTrackStruct.HitsPositions.at(iHit)(0),aTrackStruct.HitsPositions.at(iHit)(2),1);
      }
      histoDisplayTracksIn2D_XZ[2]->Draw("same");
    }
  }
  ///////////////////////////////////
  // Mark All Hits
  //////////////////////////////////
  if (DrawAllHits)
  {
    for (iEventWithHits=0; iEventWithHits<int(hitsVector.size()); iEventWithHits++)
    {
      aHitsStruct=hitsVector.at(iEventWithHits);
      for (iHit=0; iHit<int(aHitsStruct.HitsPositions.size()); iHit++)
      {
	aHitPos=aHitsStruct.HitsPositions.at(iHit);
	histoDisplayTracksIn2D_XZ[1]->Fill(aHitPos(0),aHitPos(2));
      }
    }
    histoDisplayTracksIn2D_XZ[1]->Draw("same");
  }

  //////////////////
  // CMOS Box Draw
  ///////////////////
  if (DrawSensors)
  {
    // Coordinates
    // FIXME, Antonin 7 oct 2015 : proper ISO C++11 declaration to make clang 3.4 happy
    std::vector<std::vector<DR3> >    posPlaneCornerPlanFrame;   posPlaneCornerPlanFrame.resize(nbPlanes);
    std::vector<std::vector<DR3> >    posPlaneCornerTracFrame;   posPlaneCornerTracFrame.resize(nbPlanes);
    std::vector<std::vector<TLine> >  lineBoxPlane;              lineBoxPlane           .resize(nbPlanes);
    for (Int_t iPlane = 0; iPlane < nbPlanes; ++iPlane) {// corners
            posPlaneCornerPlanFrame[iPlane].resize( 4 );
            posPlaneCornerTracFrame[iPlane].resize( 4 );
            lineBoxPlane[iPlane]           .resize( 4 );
    }// end set-up size of the 2D arrays
   
    // Corners indexes
    Int_t BL=0; // Bottom Left
    Int_t BR=1; // Bottom Right
    Int_t TL=2; // Top Left
    Int_t TR=3; // Top Right
    // Lines indexes
    Int_t L=0; // Left
    Int_t R=1; // Right
    //Int_t T=2; // Top
    //Int_t B=3; // Bottom
    //   Line points
    Double_t xmin,xmax,zmin,zmax;
    //Double ymin,ymax;
    Int_t nbCol, nbRow;
    Double_t pitchC, pitchR;

    
    for (iPlane=1;iPlane<=nbPlanes;iPlane++)
    {
      tPlane = tTracker->GetPlane(iPlane);
      nbCol  = tPlane->GetStripsNu();
      nbRow  = tPlane->GetStripsNv(); 
      pitchC = tPlane->GetStripPitch()(0);
      pitchR = tPlane->GetStripPitch()(1);

      // Boxs Drawing
      posPlaneCornerPlanFrame[iPlane-1][BL].SetValue(-nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][BR].SetValue(+nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TL].SetValue(-nbCol/2*pitchC,-nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TR].SetValue(+nbCol/2*pitchC,-nbRow/2*pitchR,0);

      posPlaneCornerTracFrame[iPlane-1][BL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BL]);
      posPlaneCornerTracFrame[iPlane-1][BR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BR]);
      posPlaneCornerTracFrame[iPlane-1][TL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TL]);
      posPlaneCornerTracFrame[iPlane-1][TR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TR]);

      // Left line (left in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][BL](0);
      //ymin = posPlaneCornerTracFrame[iPlane-1][BL](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][BL](2);
      xmax = posPlaneCornerTracFrame[iPlane-1][TL](0);
      //ymax = posPlaneCornerTracFrame[iPlane-1][TL](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][TL](2);
      
      lineBoxPlane[iPlane-1][L].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][L].DrawLine(xmin,zmin,xmax,zmax);

      // Right line (right in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][BR](0);
      //ymin = posPlaneCornerTracFrame[iPlane-1][BR](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][BR](2);
      xmax = posPlaneCornerTracFrame[iPlane-1][TR](0);
      //ymax = posPlaneCornerTracFrame[iPlane-1][TR](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][TR](2);
      
      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(xmin,zmin,xmax,zmax);

      // TOP line (top in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][TL](0);
      //ymin = posPlaneCornerTracFrame[iPlane-1][TL](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][TL](2);
      xmax = posPlaneCornerTracFrame[iPlane-1][TR](0);
      //ymax = posPlaneCornerTracFrame[iPlane-1][TR](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][TR](2);
      
      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(xmin,zmin,xmax,zmax);

      // Bottom line (bottom in plane frame ... anyway)
      xmin = posPlaneCornerTracFrame[iPlane-1][BL](0);
      //ymin = posPlaneCornerTracFrame[iPlane-1][BL](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][BL](2);
      xmax = posPlaneCornerTracFrame[iPlane-1][BR](0);
      //ymax = posPlaneCornerTracFrame[iPlane-1][BR](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][BR](2);
      
      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(xmin,zmin,xmax,zmax);
    }
  }

  //---------------------
  // YZ
  //---------------------  
  padDisplayTracksIn2D->cd(2);
  histoDisplayTracksIn2D_YZ[0]->Draw();

  for (iTrack=0; iTrack<int(tracksVector.size()); iTrack++)
  {
    aTrackStruct=tracksVector.at(iTrack);

    if (DrawTracks)
    {
      z[0] = DisplayTracksIn2DViewLimitZmin;
      z[1] = DisplayTracksIn2DViewLimitZmax;
      x[0] = aTrackStruct.TrackOrigin(0)+aTrackStruct.TrackSlopeZ(0)*z[0];
      x[1] = aTrackStruct.TrackOrigin(0)+aTrackStruct.TrackSlopeZ(0)*z[1];
      y[0] = aTrackStruct.TrackOrigin(1)+aTrackStruct.TrackSlopeZ(1)*z[0];
      y[1] = aTrackStruct.TrackOrigin(1)+aTrackStruct.TrackSlopeZ(1)*z[1];

      TLine lineTrack;
      lineTrack.SetLineColor(iTrack+1);
      lineTrack.DrawLine(y[0],z[0],y[1],z[1]);      
    }
    ///////////////////////////////////
    // Mark Hits used for the track
    //////////////////////////////////
    if (DrawTrackedHits)
    {
      for (iHit=0; iHit<int(aTrackStruct.HitsPositions.size()); iHit++)
      {
	histoDisplayTracksIn2D_YZ[2]->Fill(aTrackStruct.HitsPositions.at(iHit)(1),aTrackStruct.HitsPositions.at(iHit)(2),1);
      }
      histoDisplayTracksIn2D_YZ[2]->Draw("same");
    }
  }
  ///////////////////////////////////
  // Mark All Hits
  //////////////////////////////////
  if (DrawAllHits)
  {
    for (iEventWithHits=0; iEventWithHits<int(hitsVector.size()); iEventWithHits++)
    {
      aHitsStruct=hitsVector.at(iEventWithHits);
      for (iHit=0; iHit<int(aHitsStruct.HitsPositions.size()); iHit++)
      {
	aHitPos=aHitsStruct.HitsPositions.at(iHit);
	histoDisplayTracksIn2D_YZ[1]->Fill(aHitPos(1),aHitPos(2));
      }
    }
    histoDisplayTracksIn2D_YZ[1]->Draw("same");
  }


  //////////////////
  // CMOS Box Draw
  ///////////////////
  if (DrawSensors)
  {
    // Coordinates
    // FIXME, Antonin 7 oct 2015 : proper ISO C++11 declaration to make clang 3.4 happy
    std::vector<std::vector<DR3> >    posPlaneCornerPlanFrame;   posPlaneCornerPlanFrame.resize(nbPlanes);
    std::vector<std::vector<DR3> >    posPlaneCornerTracFrame;   posPlaneCornerTracFrame.resize(nbPlanes);
    std::vector<std::vector<TLine> >  lineBoxPlane;              lineBoxPlane           .resize(nbPlanes);
    for (iPlane = 0; iPlane < nbPlanes; ++iPlane) {// corners
            posPlaneCornerPlanFrame[iPlane].resize( 4 );
            posPlaneCornerTracFrame[iPlane].resize( 4 );
            lineBoxPlane[iPlane]           .resize( 4 );
    }// end set-up size of the 2D arrays

    // Corners indexes
    Int_t BL=0; // Bottom Left
    Int_t BR=1; // Bottom Right
    Int_t TL=2; // Top Left
    Int_t TR=3; // Top Right
    // Lines indexes
    Int_t L=0; // Left
    Int_t R=1; // Right
    //Int_t T=2; // Top
    //Int_t B=3; // Bottom
    //   Line points
    //Double_t xmin,xmax;
    Double_t ymin,ymax,zmin,zmax;
    Int_t nbCol, nbRow;
    Double_t pitchC, pitchR;

    
    for (iPlane=1;iPlane<=nbPlanes;iPlane++)
    {
      tPlane = tTracker->GetPlane(iPlane);
      nbCol  = tPlane->GetStripsNu();
      nbRow  = tPlane->GetStripsNv(); 
      pitchC = tPlane->GetStripPitch()(0);
      pitchR = tPlane->GetStripPitch()(1);

      // Boxs Drawing
      posPlaneCornerPlanFrame[iPlane-1][BL].SetValue(-nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][BR].SetValue(+nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TL].SetValue(-nbCol/2*pitchC,-nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TR].SetValue(+nbCol/2*pitchC,-nbRow/2*pitchR,0);

      posPlaneCornerTracFrame[iPlane-1][BL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BL]);
      posPlaneCornerTracFrame[iPlane-1][BR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BR]);
      posPlaneCornerTracFrame[iPlane-1][TL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TL]);
      posPlaneCornerTracFrame[iPlane-1][TR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TR]);

      // Left line (left in plane frame ... anyway)
      //xmin = posPlaneCornerTracFrame[iPlane-1][BL](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][BL](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][BL](2);
      //xmax = posPlaneCornerTracFrame[iPlane-1][TL](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][TL](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][TL](2);
      
      lineBoxPlane[iPlane-1][L].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][L].DrawLine(ymin,zmin,ymax,zmax);

      // Right line (right in plane frame ... anyway)
      //xmin = posPlaneCornerTracFrame[iPlane-1][BR](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][BR](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][BR](2);
      //xmax = posPlaneCornerTracFrame[iPlane-1][TR](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][TR](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][TR](2);
      
      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(ymin,zmin,ymax,zmax);

      // TOP line (top in plane frame ... anyway)
      //xmin = posPlaneCornerTracFrame[iPlane-1][TL](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][TL](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][TL](2);
      //xmax = posPlaneCornerTracFrame[iPlane-1][TR](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][TR](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][TR](2);
      
      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(ymin,zmin,ymax,zmax);

      // Bottom line (bottom in plane frame ... anyway)
      //xmin = posPlaneCornerTracFrame[iPlane-1][BL](0);
      ymin = posPlaneCornerTracFrame[iPlane-1][BL](1);
      zmin = posPlaneCornerTracFrame[iPlane-1][BL](2);
      //xmax = posPlaneCornerTracFrame[iPlane-1][BR](0);
      ymax = posPlaneCornerTracFrame[iPlane-1][BR](1);
      zmax = posPlaneCornerTracFrame[iPlane-1][BR](2);
      
      lineBoxPlane[iPlane-1][R].SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[iPlane-1][R].DrawLine(ymin,zmin,ymax,zmax);
    }
  }
  
  
  
  canvasDisplayTracksIn2D->cd(1);
  sprintf(canvasTitle, "Tracks 2D - tracker frame [um] - Run %d - over %d event(s)", RunNumber, eventsProcessedStoreTracks);
  labelDisplayTracksIn2D->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayTracksIn2D->Update();
  return 0;
}


Int_t MRax::DisplayTracksIn2D_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayTracksIn2D";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }
  return 0;
}


Int_t MRax::DisplayTracksIn2D_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayTracksIn2D")) return -1;
  histoDisplayTracksIn2D_XZ[0]->Reset();
  histoDisplayTracksIn2D_XZ[1]->Reset();
  histoDisplayTracksIn2D_XZ[2]->Reset();
  histoDisplayTracksIn2D_YZ[0]->Reset();
  histoDisplayTracksIn2D_YZ[1]->Reset();
  histoDisplayTracksIn2D_YZ[2]->Reset();
  return 0;
}


Int_t MRax::DisplayTracksIn2D_Kill(void)
{
  if (flagDisplayTracksIn2D_Init)
  {
    delete histoDisplayTracksIn2D_XZ[0];
    delete histoDisplayTracksIn2D_XZ[1];
    delete histoDisplayTracksIn2D_XZ[2];
    delete histoDisplayTracksIn2D_YZ[0];
    delete histoDisplayTracksIn2D_YZ[1];
    delete histoDisplayTracksIn2D_YZ[2];
  }

  if (gROOT->FindObject("canvasDisplayTracksIn2D"))
  {
    delete padDisplayTracksIn2D;
    delete labelDisplayTracksIn2D;
    canvasDisplayTracksIn2D->Close();
    delete canvasDisplayTracksIn2D;
  }
  flagDisplayTracksIn2D_Init=0;
  return 0;
}

  //****************************************************************
  //            Vertices Projections (obviously in Beam Frame [mm])
  //****************************************************************
  // 
  // Display in vertices projections with options
  // All informations are read for the "verticesVector"
  //****************************************************************
  
Int_t MRax::DisplayVerticesProjs_Init(void)
{
  if(flagDisplayVerticesProjs_Init) return -1;

  ComputeVertices_Init();
  ///////////////////
  // Configuration :
  ///////////////////
    //////////////////////
    // Display limits and binning
    //////////////////////
    confDisplayVerticesProjsImax =   90.;
    confDisplayVerticesProjsImin =   -confDisplayVerticesProjsImax;
    confDisplayVerticesProjsIbin =   50;
    
    confDisplayVerticesProjsJmax =    confDisplayVerticesProjsImax;
    confDisplayVerticesProjsJmin =   -confDisplayVerticesProjsImax;
    confDisplayVerticesProjsJbin =   50;
    
    confDisplayVerticesProjsKmax =   +250;
    confDisplayVerticesProjsKmin =   -100;
    confDisplayVerticesProjsKbin =   50;
    
///////////////////
// Code :
///////////////////
  // Canvas

  canvasDisplayVerticesProjs = new TCanvas("canvasDisplayVerticesProjs", "Vertices Projections - beam frame [mm]", 5, 5, 800, 700);
  canvasDisplayVerticesProjs->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Vertices Projections - beam frame [mm] - Run %d", RunNumber);
  labelDisplayVerticesProjs = new TPaveLabel();
  labelDisplayVerticesProjs->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayVerticesProjs = new TPad("padDisplayVerticesProjs","",0.,0.,1.,0.965);
  padDisplayVerticesProjs->Draw();
  padDisplayVerticesProjs->Divide(2,2);
  // Histo creation and setting
  
  histoDisplayVerticesProjections = new TH1F*[4];
  
  sprintf( name, "histoDisplayVerticesProjections_I");
  sprintf( title," Vertices along #vec{i}");
  histoDisplayVerticesProjections[0] = new TH1F(name,title, confDisplayVerticesProjsIbin, confDisplayVerticesProjsImin, confDisplayVerticesProjsImax);
  histoDisplayVerticesProjections[0]->SetStats(kTRUE);
  histoDisplayVerticesProjections[0]->SetXTitle("#vec{i} [mm]");
  histoDisplayVerticesProjections[0]->SetYTitle("vertices");

  sprintf( name, "histoDisplayVerticesProjections_J");
  sprintf( title," Vertices along #vec{j}");
  histoDisplayVerticesProjections[1] = new TH1F(name,title, confDisplayVerticesProjsJbin, confDisplayVerticesProjsJmin, confDisplayVerticesProjsJmax);
  histoDisplayVerticesProjections[1]->SetStats(kTRUE);
  histoDisplayVerticesProjections[1]->SetXTitle("#vec{j} [mm]");
  histoDisplayVerticesProjections[1]->SetYTitle("vertices");
  
  sprintf( name, "histoDisplayVerticesProjections_K");
  sprintf( title," Vertices along #vec{k}");
  histoDisplayVerticesProjections[2] = new TH1F(name,title, confDisplayVerticesProjsKbin, confDisplayVerticesProjsKmin, confDisplayVerticesProjsKmax);
  histoDisplayVerticesProjections[2]->SetStats(kTRUE);
  histoDisplayVerticesProjections[2]->SetXTitle("#vec{k} [mm]");
  histoDisplayVerticesProjections[2]->SetYTitle("vertices");
  
  sprintf( name, "histoDisplayVerticesProjections_length");
  sprintf( title,"Segment length");
  histoDisplayVerticesProjections[3] = new TH1F(name,title, 50,0,50);
  histoDisplayVerticesProjections[3]->SetStats(kTRUE);
  histoDisplayVerticesProjections[3]->SetXTitle("segment length [mm]");
  histoDisplayVerticesProjections[3]->SetYTitle("vertices");

  flagDisplayVerticesProjs_Init=1;
  
  return 0;
}

Int_t MRax::DisplayVerticesProjs_Show(Bool_t DrawTarget)
{

  if (!gROOT->FindObject("canvasDisplayVerticesProjs")) return -1;

  ComputeVertices_Do();
  /////////////////////////////////
  //    Vertices
  /////////////////////////////////
  DR3 Intersection;
  Double_t intersection[3];

  for (iTrack=0; iTrack<int(verticesVector.size()); iTrack++) {
    aVertexStruct=verticesVector.at(iTrack);
    aVertexStruct.VertexPositionBeamFr.Convert2DoubleArray(intersection);
    histoDisplayVerticesProjections[0]->Fill(intersection[0]);
    histoDisplayVerticesProjections[1]->Fill(intersection[1]);
    histoDisplayVerticesProjections[2]->Fill(intersection[2]);
    histoDisplayVerticesProjections[3]->Fill(aVertexStruct.VertexSegmentSize);
  }

  padDisplayVerticesProjs->cd(1);
  histoDisplayVerticesProjections[2]->Draw();
  padDisplayVerticesProjs->cd(2);
  histoDisplayVerticesProjections[3]->Draw();
  padDisplayVerticesProjs->cd(3);
  histoDisplayVerticesProjections[0]->Draw();
  padDisplayVerticesProjs->cd(4);
  histoDisplayVerticesProjections[1]->Draw();
  
  /////////////////////////////////
  //    Target drawing
  /////////////////////////////////


  canvasDisplayVerticesProjs->cd();
  sprintf(canvasTitle, "Vertices Projections - beam frame [mm] - Run %d - over %d event(s)", RunNumber, eventsProcessedStoreTracks);
  labelDisplayVerticesProjs->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayVerticesProjs->Update();

  return 0;
}

Int_t MRax::DisplayVerticesProjs_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayVerticesProjs";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayVerticesProjs_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayVerticesProjs")) return -1;

  histoDisplayVerticesProjections[0]->Reset();
  histoDisplayVerticesProjections[1]->Reset();
  histoDisplayVerticesProjections[2]->Reset();
  histoDisplayVerticesProjections[3]->Reset();
  return 0;
}

Int_t MRax::DisplayVerticesProjs_Kill(void)
{
  if (flagDisplayVerticesProjs_Init)
  {
    ComputeVertices_Reset();
    delete histoDisplayVerticesProjections[0];
    delete histoDisplayVerticesProjections[1];
    delete histoDisplayVerticesProjections[2];
    delete [] histoDisplayVerticesProjections;
    delete histoDisplayVerticesProjections[3];
  }

  if (gROOT->FindObject("canvasDisplayVerticesProjs"))
  {
    delete padDisplayVerticesProjs;
    delete labelDisplayVerticesProjs;
    canvasDisplayVerticesProjs->Close();
    delete canvasDisplayVerticesProjs;
  }
  flagDisplayVerticesProjs_Init=0;

  return 0;
}



  //****************************************************************
  //		Vertices in 3D (obviously in Beam Frame [mm])
  //****************************************************************
  // 
  // Display in 3D vertices with options
  // All informations are read for the "verticesVector"
  //****************************************************************
Int_t MRax::DisplayVerticesIn3D_Init(void)
{
  if(flagDisplayVerticesIn3D_Init) return -1;

  ComputeVertices_Init();
  ///////////////////
  // Configuration :
  ///////////////////
    //////////////////////
    // Display limits
    //////////////////////
    DispVer3DViewLimitImax =   250;
    DispVer3DViewLimitImin =   -DispVer3DViewLimitImax;
    DispVer3DViewLimitJmax =    DispVer3DViewLimitImax;
    DispVer3DViewLimitJmin =   -DispVer3DViewLimitImax;
    DispVer3DViewLimitKmax =   +400;
    DispVer3DViewLimitKmin =   -200;


///////////////////
// Code :
///////////////////

  // Canvas
  canvasDisplayVerticesIn3D = new TCanvas("canvasDisplayVerticesIn3D", "Vertices 3D - beam frame [mm]", 5, 5, 800, 700);
  canvasDisplayVerticesIn3D->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Vertices 3D - beam frame [mm] - Run %d", RunNumber);
  labelDisplayVerticesIn3D = new TPaveLabel();
  labelDisplayVerticesIn3D->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayVerticesIn3D = new TPad("padDisplayVerticesIn3D","",0.,0.,1.,0.965);
  padDisplayVerticesIn3D->Draw();
  padDisplayVerticesIn3D->cd();

  // Histo creation and setting
  sprintf( name, "histoDisplayVerticesIn3D");
  //sprintf( title,"");
  histoDisplayVerticesIn3D = new TH3I(name,"", 10, DispVer3DViewLimitImin, DispVer3DViewLimitImax, 10, DispVer3DViewLimitJmin, DispVer3DViewLimitJmax, 10, DispVer3DViewLimitKmin, DispVer3DViewLimitKmax);
  histoDisplayVerticesIn3D->SetStats(kFALSE);

  histoDisplayVerticesIn3D->SetXTitle("#vec{i} [mm]");
  histoDisplayVerticesIn3D->GetXaxis()->SetLabelSize(0.02);
  histoDisplayVerticesIn3D->GetXaxis()->SetTitleOffset();
  histoDisplayVerticesIn3D->GetXaxis()->SetLabelOffset();
  histoDisplayVerticesIn3D->GetXaxis()->SetTitleSize(histoDisplayVerticesIn3D->GetXaxis()->GetTitleSize()/2);

  histoDisplayVerticesIn3D->SetYTitle("#vec{j} [mm]");
  histoDisplayVerticesIn3D->GetYaxis()->SetLabelSize(0.02);
  histoDisplayVerticesIn3D->GetYaxis()->SetTitleOffset();
  histoDisplayVerticesIn3D->GetYaxis()->SetLabelOffset();
  histoDisplayVerticesIn3D->GetYaxis()->SetTitleSize(histoDisplayVerticesIn3D->GetYaxis()->GetTitleSize()/2);

  histoDisplayVerticesIn3D->SetZTitle("#vec{k} [mm]");
  histoDisplayVerticesIn3D->GetZaxis()->SetLabelSize(0.02);
  histoDisplayVerticesIn3D->GetZaxis()->SetTitleOffset();
  histoDisplayVerticesIn3D->GetZaxis()->SetLabelOffset();
  histoDisplayVerticesIn3D->GetZaxis()->SetTitleSize(histoDisplayVerticesIn3D->GetZaxis()->GetTitleSize()/2);


  flagDisplayVerticesIn3D_Init=1;

  return 0;
}

Int_t MRax::DisplayVerticesIn3D_Show(Bool_t DrawSensors, Bool_t DrawTracks, Bool_t DrawTrackerDir, Bool_t DrawTarget, Bool_t DrawBeam, Bool_t DrawVerticesPoints, Bool_t DrawVerticesPointsSmall, Bool_t DrawAcceptance)
{
  if (!gROOT->FindObject("canvasDisplayVerticesIn3D")) return -1;

  ComputeVertices_Do();

  padDisplayVerticesIn3D->cd();
  histoDisplayVerticesIn3D->Draw();

  DR3 Intersection;
  distDisplayedTracks = 200; //[mm] tracks length

  /////////////////////////////////
  //	Target drawing
  /////////////////////////////////
  if (DrawTarget)
  {    
    if (ComputeVertices_TargetType == 1) //cuboid
    {
      // Point Coordinates
      DR3 TargetPoints[8]; // 8 points to define the targer
      // Corners indexes
      Int_t ijk=0; // point at minimum i value, minimum j value, minimum k value (min value=lower case, max value=upper case)
      Int_t ijK=1;
      Int_t iJk=2;
      Int_t iJK=3;
      Int_t Ijk=4;
      Int_t IjK=5;
      Int_t IJk=6;
      Int_t IJK=7;

      // Corners position calcul
      TargetPoints[ijk].SetValue(ComputeVertices_TargetCenter(id_i) - ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) - ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) - ComputeVertices_TargetSize(id_k)/2 );
      TargetPoints[ijK].SetValue(ComputeVertices_TargetCenter(id_i) - ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) - ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetSize(id_k)/2 );
      TargetPoints[iJk].SetValue(ComputeVertices_TargetCenter(id_i) - ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) - ComputeVertices_TargetSize(id_k)/2 );
      TargetPoints[iJK].SetValue(ComputeVertices_TargetCenter(id_i) - ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetSize(id_k)/2 );
      TargetPoints[Ijk].SetValue(ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) - ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) - ComputeVertices_TargetSize(id_k)/2 );
      TargetPoints[IjK].SetValue(ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) - ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetSize(id_k)/2 );
      TargetPoints[IJk].SetValue(ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) - ComputeVertices_TargetSize(id_k)/2 );
      TargetPoints[IJK].SetValue(ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetSize(id_i)/2 ,\
				 ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetSize(id_j)/2 ,\
				 ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetSize(id_k)/2 );

      // Lines
      Int_t IndexLinkToDraw[12]={02,26,64,40,13,37,75,51,01,45,67,23};//exemple 26 : draw line between TargetPoints[2] and TargetPoints[6]
      // Drawing target lines
      for (Int_t id=0; id<12; id++)
      {
	coord_i[0]=TargetPoints[IndexLinkToDraw[id]%10](0);
	coord_i[1]=TargetPoints[IndexLinkToDraw[id]/10](0);
	coord_j[0]=TargetPoints[IndexLinkToDraw[id]%10](1);
	coord_j[1]=TargetPoints[IndexLinkToDraw[id]/10](1);
	coord_k[0]=TargetPoints[IndexLinkToDraw[id]%10](2);
	coord_k[1]=TargetPoints[IndexLinkToDraw[id]/10](2);

	linesTraget[id] = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	linesTraget[id] -> SetLineWidth(1);
	linesTraget[id] -> SetLineColor(14);
	linesTraget[id] -> Draw();
      }
    }
    
    else if (ComputeVertices_TargetType == 2) //cylinder
    {
      
      Int_t nbpoints=800;
      TPolyLine3D *lines;
      
      if(!strcmp(ComputeVertices_TargetAxis,"I"))
      {
	Double_t Teta = 0;
	coord_k[0] = ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetRadius * TMath::Cos(Teta);
	coord_j[0] = ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetRadius * TMath::Sin(Teta);
	Double_t i1 = ComputeVertices_TargetCenter(id_i) - ComputeVertices_TargetLength/2;
	Double_t i2 = ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetLength/2;

	for (Int_t ii=0; ii<=nbpoints;ii++)
	{
	  Teta = (Double_t)ii * TMath::DegToRad()*360 / (Double_t)nbpoints;
	  coord_k[1] = coord_k[0];
	  coord_j[1] = coord_j[0];
	  coord_k[0] = ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetRadius * TMath::Cos(Teta);
	  coord_j[0] = ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetRadius * TMath::Sin(Teta);
	  coord_i[0] = coord_i[1] = i1;

	  lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	  lines-> SetLineWidth(1);
	  lines-> SetLineColor(14);
	  lines-> Draw();

	  coord_i[0] = coord_i[1] = i2;
	  lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	  lines-> SetLineWidth(1);
	  lines-> SetLineColor(14);
	  lines-> Draw();

	  if (!(ii%10))
	  {
	    coord_k[1] = coord_k[0];
	    coord_j[1] = coord_j[0];
	    coord_i[0] = i1;
	    coord_i[1] = i2;
	    lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	    lines-> SetLineWidth(1);
	    lines-> SetLineColor(14);
	    lines-> Draw();
	  }
	}
      }
      
      if(!strcmp(ComputeVertices_TargetAxis,"J"))
      {
	Double_t Teta = 0;
	coord_i[0] = ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetRadius * TMath::Cos(Teta);
	coord_k[0] = ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetRadius * TMath::Sin(Teta);
	Double_t j1 = ComputeVertices_TargetCenter(id_j) - ComputeVertices_TargetLength/2;
	Double_t j2 = ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetLength/2;

	for (Int_t ii=0; ii<=nbpoints;ii++)
	{
	  Teta = (Double_t)ii * TMath::DegToRad()*360 / (Double_t)nbpoints;
	  coord_i[1] = coord_i[0];
	  coord_k[1] = coord_k[0];
	  coord_i[0] = ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetRadius * TMath::Cos(Teta);
	  coord_k[0] = ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetRadius * TMath::Sin(Teta);
	  coord_j[0] = coord_j[1] = j1;

	  lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	  lines-> SetLineWidth(1);
	  lines-> SetLineColor(14);
	  lines-> Draw();

	  coord_j[0] = coord_j[1] = j2;
	  lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	  lines-> SetLineWidth(1);
	  lines-> SetLineColor(14);
	  lines-> Draw();

	  if (!(ii%10))
	  {
	    coord_i[1] = coord_i[0];
	    coord_k[1] = coord_k[0];
	    coord_j[0] = j1;
	    coord_j[1] = j2;
	    lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	    lines-> SetLineWidth(1);
	    lines-> SetLineColor(14);
	    lines-> Draw();
	  }
	}
      }      
      
      if(!strcmp(ComputeVertices_TargetAxis,"K"))
      {
	Double_t Teta = 0;
	coord_i[0] = ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetRadius * TMath::Cos(Teta);
	coord_j[0] = ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetRadius * TMath::Sin(Teta);
	Double_t k1 = ComputeVertices_TargetCenter(id_k) - ComputeVertices_TargetLength/2;
	Double_t k2 = ComputeVertices_TargetCenter(id_k) + ComputeVertices_TargetLength/2;

	for (Int_t ii=0; ii<=nbpoints;ii++)
	{
	  Teta = (Double_t)ii * TMath::DegToRad()*360 / (Double_t)nbpoints;
	  coord_i[1] = coord_i[0];
	  coord_j[1] = coord_j[0];
	  coord_i[0] = ComputeVertices_TargetCenter(id_i) + ComputeVertices_TargetRadius * TMath::Cos(Teta);
	  coord_j[0] = ComputeVertices_TargetCenter(id_j) + ComputeVertices_TargetRadius * TMath::Sin(Teta);
	  coord_k[0] = coord_k[1] = k1;

	  lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	  lines-> SetLineWidth(1);
	  lines-> SetLineColor(14);
	  lines-> Draw();

	  coord_k[0] = coord_k[1] = k2;
	  lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	  lines-> SetLineWidth(1);
	  lines-> SetLineColor(14);
	  lines-> Draw();

	  if (!(ii%10))
	  {
	    coord_i[1] = coord_i[0];
	    coord_j[1] = coord_j[0];
	    coord_k[0] = k1;
	    coord_k[1] = k2;
	    lines = new TPolyLine3D(2,coord_i,coord_j,coord_k);
	    lines-> SetLineWidth(1);
	    lines-> SetLineColor(14);
	    lines-> Draw();
	  }
	}
      }
	
	
	
    }
    else
    {
      printf_error("DisplayVerticesIn3D_Show") ; cout << "target type is unknown !!" << endl; 
    }
  }

  /////////////////////////////////
  //	Beam drawing
  /////////////////////////////////
  if (DrawBeam)
  {

    // BeamDisplayStrong
    coord_i[0]= fSetup->GetIviGeometryParameter().BeamDisplayStrongBegin(0);
    coord_j[0]= fSetup->GetIviGeometryParameter().BeamDisplayStrongBegin(1);
    coord_k[0]= fSetup->GetIviGeometryParameter().BeamDisplayStrongBegin(2);
    coord_i[1]= fSetup->GetIviGeometryParameter().BeamDisplayStrongStop(0);
    coord_j[1]= fSetup->GetIviGeometryParameter().BeamDisplayStrongStop(1);
    coord_k[1]= fSetup->GetIviGeometryParameter().BeamDisplayStrongStop(2);

    lineBeamDisplayStrong = new TPolyLine3D(2,coord_i,coord_j,coord_k);
    lineBeamDisplayStrong -> SetLineWidth(5);
    lineBeamDisplayStrong -> SetLineColor(46);
    lineBeamDisplayStrong -> SetLineStyle(1);
    lineBeamDisplayStrong -> Draw();

    // BeamDisplayMedium
    coord_i[0]= fSetup->GetIviGeometryParameter().BeamDisplayMediumBegin(0);
    coord_j[0]= fSetup->GetIviGeometryParameter().BeamDisplayMediumBegin(1);
    coord_k[0]= fSetup->GetIviGeometryParameter().BeamDisplayMediumBegin(2);
    coord_i[1]= fSetup->GetIviGeometryParameter().BeamDisplayMediumStop(0);
    coord_j[1]= fSetup->GetIviGeometryParameter().BeamDisplayMediumStop(1);
    coord_k[1]= fSetup->GetIviGeometryParameter().BeamDisplayMediumStop(2);
    
    lineBeamDisplayMedium = new TPolyLine3D(2,coord_i,coord_j,coord_k);
    lineBeamDisplayMedium -> SetLineWidth(3);
    lineBeamDisplayMedium -> SetLineColor(46);
    lineBeamDisplayMedium -> SetLineStyle(2);
    lineBeamDisplayMedium -> Draw();
    
    // BeamDisplayLight
    coord_i[0]= fSetup->GetIviGeometryParameter().BeamDisplayLightBegin(0);
    coord_j[0]= fSetup->GetIviGeometryParameter().BeamDisplayLightBegin(1);
    coord_k[0]= fSetup->GetIviGeometryParameter().BeamDisplayLightBegin(2);
    coord_i[1]= fSetup->GetIviGeometryParameter().BeamDisplayLightStop(0);
    coord_j[1]= fSetup->GetIviGeometryParameter().BeamDisplayLightStop(1);
    coord_k[1]= fSetup->GetIviGeometryParameter().BeamDisplayLightStop(2);
    
    lineBeamDisplayLight = new TPolyLine3D(2,coord_i,coord_j,coord_k);
    lineBeamDisplayLight -> SetLineWidth(1);
    lineBeamDisplayLight -> SetLineColor(46);
    lineBeamDisplayLight -> SetLineStyle(1);
    lineBeamDisplayLight -> Draw();
    
  }

  /////////////////////////////////
  //	CMOS sensors drawing
  /////////////////////////////////
  if (DrawSensors)
  {
    // Coordinates
    // FIXME, Antonin 7 oct 2015 : proper ISO C++11 declaration to make clang 3.4 happy
    std::vector<std::vector<DR3> >  posPlaneCornerPlanFrame;    posPlaneCornerPlanFrame.resize(nbPlanes);
    std::vector<std::vector<DR3> >  posPlaneCornerTracFrame;    posPlaneCornerTracFrame.resize(nbPlanes);
    std::vector<std::vector<DR3> >  posPlaneCornerBeamFrame;    posPlaneCornerBeamFrame.resize(nbPlanes);
    //for (Int_t iPlane = 0; iPlane < nbPlanes; ++iPlane) {// corners
    for (iPlane = 0; iPlane < nbPlanes; ++iPlane) {// corners
            posPlaneCornerPlanFrame[iPlane].resize( 4 );
            posPlaneCornerTracFrame[iPlane].resize( 4 );
            posPlaneCornerBeamFrame[iPlane].resize( 4 );
    }// end set-up size of the 2D arrays
    lineBoxPlane = new TPolyLine3D*[nbPlanes*4];
    
    
    
    
    //Double_t x[2],y[2],z[2];
    // Corners indexes
    Int_t BL=0; // Bottom Left
    Int_t BR=1; // Bottom Right
    Int_t TL=2; // Top Left
    Int_t TR=3; // Top Right
    // Lines indexes
    Int_t L=0; // Left
    Int_t R=1; // Right
    Int_t T=2; // Top
    Int_t B=3; // Bottom
    
    Int_t nbCol, nbRow;
    Double_t pitchC, pitchR;

    for (iPlane=1;iPlane<=nbPlanes;iPlane++)
    {
      tPlane = tTracker->GetPlane(iPlane);
      nbCol  = tPlane->GetStripsNu();
      nbRow  = tPlane->GetStripsNv(); 
      pitchC = tPlane->GetStripPitch()(0);
      pitchR = tPlane->GetStripPitch()(1);

      posPlaneCornerPlanFrame[iPlane-1][BL].SetValue(-nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][BR].SetValue(+nbCol/2*pitchC,+nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TL].SetValue(-nbCol/2*pitchC,-nbRow/2*pitchR,0);
      posPlaneCornerPlanFrame[iPlane-1][TR].SetValue(+nbCol/2*pitchC,-nbRow/2*pitchR,0);

      posPlaneCornerTracFrame[iPlane-1][BL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BL])/1000.;
      posPlaneCornerTracFrame[iPlane-1][BR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][BR])/1000.;
      posPlaneCornerTracFrame[iPlane-1][TL]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TL])/1000.;
      posPlaneCornerTracFrame[iPlane-1][TR]=tPlane->PlaneToTracker(posPlaneCornerPlanFrame[iPlane-1][TR])/1000.;

      posPlaneCornerBeamFrame[iPlane-1][BL]=frameChange_Expe2Tracker->TransformHitToTracker(posPlaneCornerTracFrame[iPlane-1][BL]);
      posPlaneCornerBeamFrame[iPlane-1][BR]=frameChange_Expe2Tracker->TransformHitToTracker(posPlaneCornerTracFrame[iPlane-1][BR]);
      posPlaneCornerBeamFrame[iPlane-1][TL]=frameChange_Expe2Tracker->TransformHitToTracker(posPlaneCornerTracFrame[iPlane-1][TL]);
      posPlaneCornerBeamFrame[iPlane-1][TR]=frameChange_Expe2Tracker->TransformHitToTracker(posPlaneCornerTracFrame[iPlane-1][TR]);

      // Left line (left in plane frame ... anyway)
      coord_i[0] = posPlaneCornerBeamFrame[iPlane-1][BL](0);
      coord_j[0] = posPlaneCornerBeamFrame[iPlane-1][BL](1);
      coord_k[0] = posPlaneCornerBeamFrame[iPlane-1][BL](2);
      coord_i[1] = posPlaneCornerBeamFrame[iPlane-1][TL](0);
      coord_j[1] = posPlaneCornerBeamFrame[iPlane-1][TL](1);
      coord_k[1] = posPlaneCornerBeamFrame[iPlane-1][TL](2);

      lineBoxPlane[(iPlane-1)*L]=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
      lineBoxPlane[(iPlane-1)*L]->SetLineWidth(2);
      lineBoxPlane[(iPlane-1)*L]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[(iPlane-1)*L]->Draw();

      // Right line (right in plane frame ... anyway)
      coord_i[0] = posPlaneCornerBeamFrame[iPlane-1][BR](0);
      coord_j[0] = posPlaneCornerBeamFrame[iPlane-1][BR](1);
      coord_k[0] = posPlaneCornerBeamFrame[iPlane-1][BR](2);
      coord_i[1] = posPlaneCornerBeamFrame[iPlane-1][TR](0);
      coord_j[1] = posPlaneCornerBeamFrame[iPlane-1][TR](1);
      coord_k[1] = posPlaneCornerBeamFrame[iPlane-1][TR](2);

      lineBoxPlane[(iPlane-1)*R]=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
      lineBoxPlane[(iPlane-1)*R]->SetLineWidth(2);
      lineBoxPlane[(iPlane-1)*R]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[(iPlane-1)*R]->Draw();

      // TOP line (top in plane frame ... anyway)
      coord_i[0] = posPlaneCornerBeamFrame[iPlane-1][TL](0);
      coord_j[0] = posPlaneCornerBeamFrame[iPlane-1][TL](1);
      coord_k[0] = posPlaneCornerBeamFrame[iPlane-1][TL](2);
      coord_i[1] = posPlaneCornerBeamFrame[iPlane-1][TR](0);
      coord_j[1] = posPlaneCornerBeamFrame[iPlane-1][TR](1);
      coord_k[1] = posPlaneCornerBeamFrame[iPlane-1][TR](2);

      lineBoxPlane[(iPlane-1)*T]=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
      lineBoxPlane[(iPlane-1)*T]->SetLineWidth(2);
      lineBoxPlane[(iPlane-1)*T]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[(iPlane-1)*T]->Draw();

      // Bottom line (bottom in plane frame ... anyway)
      coord_i[0] = posPlaneCornerBeamFrame[iPlane-1][BL](0);
      coord_j[0] = posPlaneCornerBeamFrame[iPlane-1][BL](1);
      coord_k[0] = posPlaneCornerBeamFrame[iPlane-1][BL](2);
      coord_i[1] = posPlaneCornerBeamFrame[iPlane-1][BR](0);
      coord_j[1] = posPlaneCornerBeamFrame[iPlane-1][BR](1);
      coord_k[1] = posPlaneCornerBeamFrame[iPlane-1][BR](2);

      lineBoxPlane[(iPlane-1)*B]=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
      lineBoxPlane[(iPlane-1)*B]->SetLineWidth(2);
      lineBoxPlane[(iPlane-1)*B]->SetLineColor(LineColor[iPlane-1]);
      lineBoxPlane[(iPlane-1)*B]->Draw();
      }
  }


  /////////////////////////////////
  //	Tracker acceptance using extreme planes
  /////////////////////////////////
  if (DrawAcceptance)
  {
    // Coordinates
    DR3 pointsTrackerAcceptTracFrame[2];
    DR3 pointsTrackerAcceptBeamFrame[2];
    //Double_t x[2],y[2],z[2];

    Double_t zTrMin = 1e5; //just a big value
    Double_t zTrMax = 0;   //just a small value
    for (iPlane=1; iPlane<=nbPlanes; iPlane++)
    {
      if(zTrMin>tTracker->GetPlane(iPlane)->GetPosition()(2)) zTrMin = tTracker->GetPlane(iPlane)->GetPosition()(2);
      if(zTrMax<tTracker->GetPlane(iPlane)->GetPosition()(2)) zTrMax = tTracker->GetPlane(iPlane)->GetPosition()(2);
    }

    Double_t Lcmos     = 21.2;//[mm]detection area of one plane, supposed to be a square
    Double_t distDecal = 1.e3 * Lcmos * (distDisplayedTracks) / (zTrMax-zTrMin); // 1.e3 is for um->mm conversion

    pointsTrackerAcceptTracFrame[0].SetValue(+ Lcmos/2            ,+ Lcmos/2            ,zTrMin);
    pointsTrackerAcceptTracFrame[1].SetValue(+ Lcmos/2 - distDecal,+ Lcmos/2 - distDecal,zTrMin+distDisplayedTracks);
    pointsTrackerAcceptBeamFrame[0]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[0]);
    pointsTrackerAcceptBeamFrame[1]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[1]);
    coord_i[0] = pointsTrackerAcceptBeamFrame[0](0);
    coord_j[0] = pointsTrackerAcceptBeamFrame[0](1);
    coord_k[0] = pointsTrackerAcceptBeamFrame[0](2);
    coord_i[1] = pointsTrackerAcceptBeamFrame[1](0);
    coord_j[1] = pointsTrackerAcceptBeamFrame[1](1);
    coord_k[1] = pointsTrackerAcceptBeamFrame[1](2);
    lineTrackDir=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
    lineTrackDir->SetLineWidth(1);
    lineTrackDir->SetLineColor(7);
    lineTrackDir->Draw();

    pointsTrackerAcceptTracFrame[0].SetValue(- Lcmos/2            ,+ Lcmos/2            ,zTrMin);
    pointsTrackerAcceptTracFrame[1].SetValue(- Lcmos/2 + distDecal,+ Lcmos/2 - distDecal,zTrMin+distDisplayedTracks);
    pointsTrackerAcceptBeamFrame[0]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[0]);
    pointsTrackerAcceptBeamFrame[1]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[1]);
    coord_i[0] = pointsTrackerAcceptBeamFrame[0](0);
    coord_j[0] = pointsTrackerAcceptBeamFrame[0](1);
    coord_k[0] = pointsTrackerAcceptBeamFrame[0](2);
    coord_i[1] = pointsTrackerAcceptBeamFrame[1](0);
    coord_j[1] = pointsTrackerAcceptBeamFrame[1](1);
    coord_k[1] = pointsTrackerAcceptBeamFrame[1](2);
    lineTrackDir=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
    lineTrackDir->SetLineWidth(1);
    lineTrackDir->SetLineColor(7);
    lineTrackDir->Draw();

    pointsTrackerAcceptTracFrame[0].SetValue(- Lcmos/2            ,- Lcmos/2            ,zTrMin);
    pointsTrackerAcceptTracFrame[1].SetValue(- Lcmos/2 + distDecal,- Lcmos/2 + distDecal,zTrMin+distDisplayedTracks);
    pointsTrackerAcceptBeamFrame[0]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[0]);
    pointsTrackerAcceptBeamFrame[1]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[1]);
    coord_i[0] = pointsTrackerAcceptBeamFrame[0](0);
    coord_j[0] = pointsTrackerAcceptBeamFrame[0](1);
    coord_k[0] = pointsTrackerAcceptBeamFrame[0](2);
    coord_i[1] = pointsTrackerAcceptBeamFrame[1](0);
    coord_j[1] = pointsTrackerAcceptBeamFrame[1](1);
    coord_k[1] = pointsTrackerAcceptBeamFrame[1](2);
    lineTrackDir=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
    lineTrackDir->SetLineWidth(1);
    lineTrackDir->SetLineColor(7);
    lineTrackDir->Draw();

    pointsTrackerAcceptTracFrame[0].SetValue(+ Lcmos/2            ,- Lcmos/2            ,zTrMin);
    pointsTrackerAcceptTracFrame[1].SetValue(+ Lcmos/2 - distDecal,- Lcmos/2 + distDecal,zTrMin+distDisplayedTracks);
    pointsTrackerAcceptBeamFrame[0]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[0]);
    pointsTrackerAcceptBeamFrame[1]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerAcceptTracFrame[1]);
    coord_i[0] = pointsTrackerAcceptBeamFrame[0](0);
    coord_j[0] = pointsTrackerAcceptBeamFrame[0](1);
    coord_k[0] = pointsTrackerAcceptBeamFrame[0](2);
    coord_i[1] = pointsTrackerAcceptBeamFrame[1](0);
    coord_j[1] = pointsTrackerAcceptBeamFrame[1](1);
    coord_k[1] = pointsTrackerAcceptBeamFrame[1](2);
    lineTrackDir=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
    lineTrackDir->SetLineWidth(1);
    lineTrackDir->SetLineColor(7);
    lineTrackDir->Draw();

  }


  /////////////////////////////////
  //	Tracker direction drawing
  /////////////////////////////////
  if (DrawTrackerDir)
  {
    // Coordinates
    DR3 pointsTrackerDirTracFrame[2];
    DR3 pointsTrackerDirBeamFrame[2];
    //Double_t x[2],y[2],z[2];

    pointsTrackerDirTracFrame[0].SetValue(0,0,0);    // Origin of Tracker
    pointsTrackerDirTracFrame[1].SetValue(0,0,distDisplayedTracks);// Another point of the tracker direction

    pointsTrackerDirBeamFrame[0]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerDirTracFrame[0]);
    pointsTrackerDirBeamFrame[1]=frameChange_Expe2Tracker->TransformHitToTracker(pointsTrackerDirTracFrame[1]);

    coord_i[0] = pointsTrackerDirBeamFrame[0](0);
    coord_j[0] = pointsTrackerDirBeamFrame[0](1);
    coord_k[0] = pointsTrackerDirBeamFrame[0](2);
    coord_i[1] = pointsTrackerDirBeamFrame[1](0);
    coord_j[1] = pointsTrackerDirBeamFrame[1](1);
    coord_k[1] = pointsTrackerDirBeamFrame[1](2);

    lineTrackDir=new TPolyLine3D( 2, coord_i, coord_j, coord_k);
    lineTrackDir->SetLineWidth(1);
    lineTrackDir->SetLineColor(8);
    lineTrackDir->Draw();

  }

  /////////////////////////////////
  //	Tracks
  /////////////////////////////////
  if (DrawTracks)
  {
    for (iTrack=0; iTrack<int(verticesVector.size()); iTrack++) {
      aVertexStruct=verticesVector.at(iTrack);

      // 1) Track origin in tracker frame and expe frame
      Track2points_TrackerFrame[0]=aVertexStruct.TrackOriginTrFr;
      Track2points_ExpermtFrame[0]=frameChange_Expe2Tracker->TransformHitToTracker(Track2points_TrackerFrame[0]);
      
      // 2) Vertex position in expe frame is in aVertexStruct.VertexPositionBeamFr(0,1,2);
      
      // 3) Length from track origin to vertex point is :
      Double_t length = sqrt( pow((Track2points_ExpermtFrame[0](0) - aVertexStruct.VertexPositionBeamFr(0)),2) \
                            + pow((Track2points_ExpermtFrame[0](1) - aVertexStruct.VertexPositionBeamFr(1)),2) \
                            + pow((Track2points_ExpermtFrame[0](2) - aVertexStruct.VertexPositionBeamFr(2)),2) );


      // 4) Compute the 2nd point of the track, from track origine, tacking into acount track slope and length
      Track2points_TrackerFrame[1]=Track2points_TrackerFrame[0].ComputeWithSlopeAndDistance(aVertexStruct.TrackSlopeZTrFr,length);
      
      // 5) Convert 2nd point coordinates into experimental frame
      Track2points_ExpermtFrame[1]=frameChange_Expe2Tracker->TransformHitToTracker(Track2points_TrackerFrame[1]);


      coord_i[0] = Track2points_ExpermtFrame[0](0);
      coord_j[0] = Track2points_ExpermtFrame[0](1);
      coord_k[0] = Track2points_ExpermtFrame[0](2);
      
      coord_i[1] = Track2points_ExpermtFrame[1](0);
      coord_j[1] = Track2points_ExpermtFrame[1](1);
      coord_k[1] = Track2points_ExpermtFrame[1](2);

      lineTrack  = new TPolyLine3D( 2, coord_i, coord_j, coord_k);
      lineTrack -> SetLineWidth(1);
      lineTrack -> SetLineColor(6);
      lineTrack -> Draw();
    }
  }

  /////////////////////////////////
  //	Vertices points
  /////////////////////////////////
  if (DrawVerticesPoints)
  {
    Double_t vertexposition[3];
    for (iTrack=0; iTrack<int(verticesVector.size()); iTrack++) {
      aVertexStruct=verticesVector.at(iTrack);
      aVertexStruct.VertexPositionBeamFr.Convert2DoubleArray(vertexposition);

      TPolyMarker3D *vertex = new TPolyMarker3D(1,vertexposition,3);
      vertex->SetMarkerStyle((DrawVerticesPointsSmall)?1:3);
      vertex->SetMarkerColor((DrawVerticesPointsSmall)?1:4);
      vertex->SetMarkerSize((DrawVerticesPointsSmall)?1:2);
      vertex->Draw();
    }
  }

  canvasDisplayVerticesIn3D->cd();
  sprintf(canvasTitle, "Vertices 3D - beam frame [mm] - Run %d - over %d event(s)", RunNumber, eventsProcessedStoreTracks);
  labelDisplayVerticesIn3D->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayVerticesIn3D->Update();

  return 0;
}

Int_t MRax::DisplayVerticesIn3D_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayVerticesIn3D";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayVerticesIn3D_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayVerticesIn3D")) return -1;

  histoDisplayVerticesIn3D->Reset();

  return 0;
}

Int_t MRax::DisplayVerticesIn3D_Kill(void)
{
  if (flagDisplayVerticesIn3D_Init)
  {
    ComputeVertices_Reset();
    delete histoDisplayVerticesIn3D;
  }

  if (gROOT->FindObject("canvasDisplayVerticesIn3D"))
  {
    delete padDisplayVerticesIn3D;
    delete labelDisplayVerticesIn3D;
    canvasDisplayVerticesIn3D->Close();
    delete canvasDisplayVerticesIn3D;
  }
  flagDisplayVerticesIn3D_Init=0;

  return 0;
}

//______________________________________________________________________________________________________
//
//	Display statistics
//______________________________________________________________________________________________________
  //****************************************************************
  //            CMOS OverFlow Rows
  //****************************************************************
Int_t MRax::DisplayCmosOverFlow_Init(void)
{
  if(flagDisplayCmosOverFlow_Init) return -1;
  eventsProcessedDisplayCmosOverFlow=0;
  //configuration:
  Int_t histoDisplayCmosOverFlowRowsNbXMax = 50; // if -1, the row nb is used
  
  //    correspondence between canvas position (pad nb) & plane
  //DisplayCmosOverFlow_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  DisplayCmosOverFlow_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);

  // Canvas
  gStyle->SetOptStat("");
  canvasDisplayCmosOverFlow = new TCanvas("canvasDisplayCmosOverFlow", "CMOS Overflow Rows", 5, 5, 800, 700);
  canvasDisplayCmosOverFlow->Clear();
  canvasDisplayCmosOverFlow->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "CMOS Overflow Rows - Run %d", RunNumber);
  labelDisplayCmosOverFlow = new TPaveLabel();
  labelDisplayCmosOverFlow->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayCmosOverFlow = new TPad("padDisplayCmosOverFlow","",0.,0.,1.,0.965);
  padDisplayCmosOverFlow->Draw();
  padDisplayCmosOverFlow->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY*2);


  // Histos creation and setting
  histoDisplayCmosOverFlowRows   = new TH1I*[nbPlanes];
  histoDisplayCmosOverFlowRowsNb = new TH1I*[nbPlanes];

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayCmosOverFlow_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
   
    Int_t rownb = tPlane->GetStripsNv();
    
    padDisplayCmosOverFlow->cd(iPlane);
    // Histo creation & parameters   
    sprintf( name, "histoDisplayCmosOverFlowRows_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayCmosOverFlowRows[jPlane-1] = new TH1I(name, title, rownb, 0, rownb);
    histoDisplayCmosOverFlowRows[jPlane-1]->SetXTitle("Row nb.");
    histoDisplayCmosOverFlowRows[jPlane-1]->SetYTitle("Events");
    histoDisplayCmosOverFlowRows[jPlane-1]->SetStats(0);
    //histoDisplayCmosOverFlowRows[jPlane-1]->SetLineColor(3);//green
    //histoDisplayCmosOverFlowRows[jPlane-1]->SetLineWidth(2);

    padDisplayCmosOverFlow->cd(iPlane + configDisplaysPadDivideX*configDisplaysPadDivideY);
    if (histoDisplayCmosOverFlowRowsNbXMax > 1) rownb = histoDisplayCmosOverFlowRowsNbXMax;
    sprintf( name, "histoDisplayCmosOverFlowRowsNb_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayCmosOverFlowRowsNb[jPlane-1] = new TH1I(name, title, rownb, 0, rownb);
    histoDisplayCmosOverFlowRowsNb[jPlane-1]->SetXTitle("Overflow Rows by event");
    histoDisplayCmosOverFlowRowsNb[jPlane-1]->SetYTitle("Events");
    histoDisplayCmosOverFlowRowsNb[jPlane-1]->SetStats(0);
    //histoDisplayCmosOverFlowRowsNb[jPlane-1]->SetLineColor(1);//black

  }
  flagDisplayCmosOverFlow_Init=1;
  return 0;
}

Int_t MRax::DisplayCmosOverFlow_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayCmosOverFlow")) return -1;
  eventsProcessedDisplayCmosOverFlow+=1;
  
  // Filling
 std::vector<int> *ListOfLineOverflow = tDAcq->GetLineOverflow();
  Int_t nbOf, iOf, chip;
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayCmosOverFlow_PlanesDrawOrder[iPlane-1]; 
    tPlane = tTracker->GetPlane(jPlane);
    chip = tPlane->GetAcqInputNumber();
    nbOf = ListOfLineOverflow[chip-1].size();

    if(nbOf>0) histoDisplayCmosOverFlowRowsNb[jPlane-1]->Fill(nbOf,1);
    for(iOf=0 ; iOf<nbOf ; iOf++)
    {
      histoDisplayCmosOverFlowRows[jPlane-1]->Fill(ListOfLineOverflow[chip-1].at(iOf),1);
    }
  }
  return 0;
}

Int_t MRax::DisplayCmosOverFlow_Show(void)
{
  if (!gROOT->FindObject("canvasDisplayCmosOverFlow")) return -1;

  // Parameters

  // Declarations

  // Histos

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ )
  {
    jPlane = DisplayCmosOverFlow_PlanesDrawOrder[iPlane-1];
    
    padDisplayCmosOverFlow->cd(iPlane);
    histoDisplayCmosOverFlowRows[jPlane-1]->Draw();
    
    padDisplayCmosOverFlow->cd(iPlane + configDisplaysPadDivideX*configDisplaysPadDivideY);
    histoDisplayCmosOverFlowRowsNb[jPlane-1]->Draw();
  }
  
  canvasDisplayCmosOverFlow->cd();
  sprintf(canvasTitle, "CMOS Overflow Rows - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayCmosOverFlow);
  labelDisplayCmosOverFlow->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayCmosOverFlow->Update();

  return 0;
}

Int_t MRax::DisplayCmosOverFlow_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayCmosOverFlow";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayCmosOverFlow_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayCmosOverFlow")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    histoDisplayCmosOverFlowRows[iPlane-1]      ->Reset();
    histoDisplayCmosOverFlowRowsNb[iPlane-1]  ->Reset();
  }
  eventsProcessedDisplayCmosOverFlow=0;
  return 0;
}

Int_t MRax::DisplayCmosOverFlow_Kill(void)
{
  if (flagDisplayCmosOverFlow_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayCmosOverFlowRows[iPlane-1];
      delete histoDisplayCmosOverFlowRowsNb[iPlane-1];
    }
    delete [] histoDisplayCmosOverFlowRows;
    delete [] histoDisplayCmosOverFlowRowsNb;
  }

  if (gROOT->FindObject("canvasDisplayCmosOverFlow"))
  {
    delete padDisplayCmosOverFlow;
    delete labelDisplayCmosOverFlow;
    canvasDisplayCmosOverFlow->Close();
    delete canvasDisplayCmosOverFlow;
  }

  flagDisplayCmosOverFlow_Init=0;
  eventsProcessedDisplayCmosOverFlow=0;
  return 0;
}



  //****************************************************************
  //		Clusters size 
  //****************************************************************
Int_t MRax::DisplayClustersSize_Init(Int_t nbBinX)
{
  if(flagDisplayClustersSize_Init) return -1;
  eventsProcessedDisplayClustersSize=0;
  
  if(fSession->GetSetup()->GetTrackerPar().KeepUnTrackedHitsBetw2evts) 
  {
    printf_warning("DisplayClustersSize");
    printf("KeepUnTrackedHitsBetw2evts is enabled ! Take care to yields interpretation (factor 2) ! \n");
  }
  
  //configuration:
  
  //	correspondence between canvas position (pad nb) & plane
  //DisplayClustersSize_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_default[0]);
  DisplayClustersSize_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);

  // Canvas
  gStyle->SetOptStat("nemruo");
  canvasDisplayClustersSize = new TCanvas("canvasDisplayClustersSize", "Clusters size [pixels]", 5, 5, 800, 700);
  canvasDisplayClustersSize->Clear();
  canvasDisplayClustersSize->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Clusters size [pixels] - Run %d", RunNumber);
  labelDisplayClustersSize = new TPaveLabel();
  labelDisplayClustersSize->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayClustersSize = new TPad("padDisplayClustersSize","",0.,0.,1.,0.965);
  padDisplayClustersSize->Draw();
  padDisplayClustersSize->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);


  // Histos creation and setting
  histoDisplayClustersSizeAll 	= new TH1I*[nbPlanes];
  histoDisplayClustersSizeTracked 	= new TH1I*[nbPlanes];
  histoDisplayClustersSizeUnTracked	= new TH1I*[nbPlanes];

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    padDisplayClustersSize->cd(iPlane);
    jPlane = DisplayClustersSize_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
   
    // Histo creation & parameters
    //   Hits
    sprintf( name, "histoDisplayClustersSizeAll_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayClustersSizeAll[jPlane-1] = new TH1I(name, title, nbBinX, 0, nbBinX);
    histoDisplayClustersSizeAll[jPlane-1]->SetStats(kTRUE);
    histoDisplayClustersSizeAll[jPlane-1]->SetLineColor(3);//green
    histoDisplayClustersSizeAll[jPlane-1]->SetLineWidth(2);
    histoDisplayClustersSizeAll[jPlane-1]->SetXTitle("nb pixels in cluster [pixels]");
    histoDisplayClustersSizeAll[jPlane-1]->SetYTitle("clusters nb");

    sprintf( name, "histoDisplayClustersSizeTracked_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayClustersSizeTracked[jPlane-1] = new TH1I(name, title, nbBinX, 0, nbBinX);
    histoDisplayClustersSizeTracked[jPlane-1]->SetStats(kFALSE);
    histoDisplayClustersSizeTracked[jPlane-1]->SetLineColor(1);//black
    histoDisplayClustersSizeTracked[jPlane-1]->SetXTitle("nb pixels in cluster [pixels]");
    histoDisplayClustersSizeTracked[jPlane-1]->SetYTitle("clusters nb");

    sprintf( name, "histoDisplayClustersSizeUnTracked_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayClustersSizeUnTracked[jPlane-1] = new TH1I(name, title, nbBinX, 0, nbBinX);
    histoDisplayClustersSizeUnTracked[jPlane-1]->SetStats(kFALSE);
    histoDisplayClustersSizeUnTracked[jPlane-1]->SetLineColor(2);// red
    histoDisplayClustersSizeUnTracked[jPlane-1]->SetXTitle("nb pixels in cluster [pixels]");
    histoDisplayClustersSizeUnTracked[jPlane-1]->SetYTitle("clusters nb");

  }
  flagDisplayClustersSize_Init=1;
  return 0;
}

Int_t MRax::DisplayClustersSize_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayClustersSize")) return -1;
  eventsProcessedDisplayClustersSize+=1;

  // Filling
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayClustersSize_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    aList  = tPlane->GetListOfPixels();

    for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
    {
      aHit = (DHit*)tPlane->GetHit(iHit);
      histoDisplayClustersSizeAll[jPlane-1]->Fill(aHit->GetStripsInCluster());
      if (aHit->GetFound()) histoDisplayClustersSizeTracked[jPlane-1]->Fill(aHit->GetStripsInCluster());
      else histoDisplayClustersSizeUnTracked[jPlane-1]->Fill(aHit->GetStripsInCluster());
    }
  }
  return 0;
}

Int_t MRax::DisplayClustersSize_Show(Bool_t All,Bool_t Tracked, Bool_t Untracked)
{
  if (!gROOT->FindObject("canvasDisplayClustersSize")) return -1;

  // Parameters

  // Declarations

  // Histos
  Bool_t boolFirstDisplHitsSize = 1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    padDisplayClustersSize->cd(iPlane);
    
    jPlane = DisplayClustersSize_PlanesDrawOrder[iPlane-1];
    // Hits
    if(All)
    {
      histoDisplayClustersSizeAll[jPlane-1]->Draw();
      boolFirstDisplHitsSize = 0;
    }
    if(Tracked)
    {
      histoDisplayClustersSizeTracked[jPlane-1]->Draw((boolFirstDisplHitsSize) ? "":"same");
      boolFirstDisplHitsSize = 0;
    }
    if(Untracked)
    {
      histoDisplayClustersSizeUnTracked[jPlane-1]->Draw((boolFirstDisplHitsSize) ? "":"same");
    }

  }
  
  canvasDisplayClustersSize->cd();
  sprintf(canvasTitle, "Clusters size [pixels] - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayClustersSize);
  labelDisplayClustersSize->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayClustersSize->Update();

  return 0;
}

Int_t MRax::DisplayClustersSize_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayClustersSize";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayClustersSize_SaveHistos()
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-histograms in root file in the "results" path
  //-------------
  // Arguments :
  //-------------
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayClustersSize";
  histosName = "histosClustersSize";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  histosFileFullPath =  resultsDirPath;
  histosFileFullPath += "/";
  histosFileFullPath += FileNamePrefix;
  histosFileFullPath += histosName;
  histosFileFullPath += FileNameSuffix;
  histosFileFullPath += ".root";
  
  histosFile = new TFile(histosFileFullPath,"RECREATE");
  
  // Loop on histos to save them
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause iPlane!=PlanesDrawOrder[iPlane-1];
  {
    histoDisplayClustersSizeAll[iPlane-1]->Write();
    histoDisplayClustersSizeTracked[iPlane-1]->Write();
    histoDisplayClustersSizeUnTracked[iPlane-1]->Write();
  }

  histosFile->Write();
  
  printf_info("DisplayClustersSize");
  printf("Histograms saved in file %s\n",histosFileFullPath.Data());
  
  histosFile->Close();
  delete histosFile;
  
  return 0;
}

Int_t MRax::DisplayClustersSize_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayClustersSize")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    histoDisplayClustersSizeAll[iPlane-1]      ->Reset();
    histoDisplayClustersSizeTracked[iPlane-1]  ->Reset();
    histoDisplayClustersSizeUnTracked[iPlane-1]->Reset();
  }
  eventsProcessedDisplayClustersSize=0;
  return 0;
}

Int_t MRax::DisplayClustersSize_Kill(void)
{
  if (flagDisplayClustersSize_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayClustersSizeAll[iPlane-1];
      delete histoDisplayClustersSizeTracked[iPlane-1];
      delete histoDisplayClustersSizeUnTracked[iPlane-1];
    }
    delete [] histoDisplayClustersSizeAll;
  }

  if (gROOT->FindObject("canvasDisplayClustersSize"))
  {
    delete padDisplayClustersSize;
    delete labelDisplayClustersSize;
    canvasDisplayClustersSize->Close();
    delete canvasDisplayClustersSize;
  }

  flagDisplayClustersSize_Init=0;
  eventsProcessedDisplayClustersSize=0;
  return 0;
}

  //****************************************************************
  //		Hits By Event
  //****************************************************************
Int_t MRax::DisplayHitsByEvent_Init(Int_t nbBinX)
{
  if(flagDisplayHitsByEvent_Init) return -1;
  eventsProcessedDisplayHitsByEvent=0;

  if(fSession->GetSetup()->GetTrackerPar().KeepUnTrackedHitsBetw2evts) 
  {
    printf_warning("DisplayHitsByEvent");
    printf("KeepUnTrackedHitsBetw2evts is enabled ! Take care to yields interpretation (factor 2) ! \n");
  }
  
  //	correspondence between canvas position (pad nb) & plane
  DisplayHitsByEvent_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);


  // Canvas
  canvasDisplayHitsByEvent = new TCanvas("canvasDisplayHitsByEvent", "Hits by event", 5, 5, 800, 700);
  canvasDisplayHitsByEvent->Clear();
  canvasDisplayHitsByEvent->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Hits by event - Run %d", RunNumber);
  labelDisplayHitsByEvent = new TPaveLabel();
  labelDisplayHitsByEvent->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayHitsByEvent = new TPad("padDisplayHitsByEvent","",0.,0.,1.,0.965);
  padDisplayHitsByEvent->Draw();
  padDisplayHitsByEvent->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);

  // Histos creation and setting
  histoDisplayHitsByEvent 	= new TH1I*[nbPlanes];

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    padDisplayHitsByEvent->cd(iPlane);
    jPlane = DisplayHitsByEvent_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Histo creation & parameters
    //   Hits
    sprintf( name, "histoDisplayHitsByEventTotal_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsByEvent[jPlane-1] = new TH1I(name, title, nbBinX, 0, nbBinX);
    histoDisplayHitsByEvent[jPlane-1]->SetStats(kTRUE);
    histoDisplayHitsByEvent[jPlane-1]->SetXTitle("nb hits by event");
    histoDisplayHitsByEvent[jPlane-1]->SetYTitle("nb events");
    histoDisplayHitsByEvent[jPlane-1]-> SetLineColor(1);//Noir
  }
  flagDisplayHitsByEvent_Init=1;
  return 0;
}

Int_t MRax::DisplayHitsByEvent_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsByEvent")) return -1;
  eventsProcessedDisplayHitsByEvent+=1;

  // Filling
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayHitsByEvent_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    aList  = tPlane->GetListOfPixels();

    // all hits
    histoDisplayHitsByEvent[jPlane-1]->Fill(tPlane->GetHitsN());
  }
  return 0;
}

Int_t MRax::DisplayHitsByEvent_Show(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsByEvent")) return -1;

  // Parameters

  // Declarations

  // Histos
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    padDisplayHitsByEvent->cd(iPlane);
    jPlane = DisplayHitsByEvent_PlanesDrawOrder[iPlane-1];
    // Hits
    histoDisplayHitsByEvent[jPlane-1]->Draw();
  }
  
  canvasDisplayHitsByEvent->cd();
  sprintf(canvasTitle, "Hits by event - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayHitsByEvent);
  labelDisplayHitsByEvent->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayHitsByEvent->Update();

  return 0;
}

Int_t MRax::DisplayHitsByEvent_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayHitsByEvent";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayHitsByEvent_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsByEvent")) return -1;

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    histoDisplayHitsByEvent[iPlane-1]->Reset();
  }
  eventsProcessedDisplayHitsByEvent=0;
  return 0;
}

Int_t MRax::DisplayHitsByEvent_Kill(void)
{
  if (flagDisplayHitsByEvent_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayHitsByEvent[iPlane-1];
    }
    delete [] histoDisplayHitsByEvent;
  }

  if (gROOT->FindObject("canvasDisplayHitsByEvent"))
  {
    delete padDisplayHitsByEvent;
    delete labelDisplayHitsByEvent;
    canvasDisplayHitsByEvent->Close();
    delete canvasDisplayHitsByEvent;
  }

  flagDisplayHitsByEvent_Init=0;
  eventsProcessedDisplayHitsByEvent=0;
  return 0;
}

  //****************************************************************
  //		Association of hits to tracks
  //****************************************************************
Int_t MRax::DisplayHitsTracksAssociation_Init(Int_t nbBinX)
{
  if(flagDisplayHitsTracksAssociation_Init) return -1;
  eventsProcessedDisplayHitsTracksAssociation=0;
  
  if(fSession->GetSetup()->GetTrackerPar().KeepUnTrackedHitsBetw2evts) 
  {
    printf_warning("DisplayHitsTracksAssociation");
    printf("KeepUnTrackedHitsBetw2evts is enabled ! Take care to yields interpretation (factor 2) ! \n");
  }
  
  // Configuration
  //	correspondence between canvas position (pad nb) & plane
  DisplayHitsTracksAssociation_PlanesDrawOrder = (Int_t *) (&configPlanesDrawOrder_custom1[0]);



  // Canvas
  canvasDisplayHitsTracksAssociation = new TCanvas("canvasDisplayHitsTracksAssociation", "Hits Association", 5, 5, 800, 700);
  canvasDisplayHitsTracksAssociation->Clear();
  canvasDisplayHitsTracksAssociation->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Hits association - Run %d", RunNumber);
  labelDisplayHitsTracksAssociation = new TPaveLabel();
  labelDisplayHitsTracksAssociation->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayHitsTracksAssociation = new TPad("padDisplayHitsTracksAssociation","",0.,0.,1.,0.965);
  padDisplayHitsTracksAssociation->Draw();
  padDisplayHitsTracksAssociation->Divide(configDisplaysPadDivideX,configDisplaysPadDivideY);

  // Histos creation and setting
  histoDisplayHitsTracksAssociation 	= new TH1I*[nbPlanes*2];

  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    padDisplayHitsTracksAssociation->cd(iPlane);
    jPlane = DisplayHitsTracksAssociation_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    // Histo creation & parameters
    //   Hits
    padDisplayHitsTracksAssociation->cd(iPlane);
    sprintf( name, "histoDisplayHitsTracksAssociationAssoTrack_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsTracksAssociation[jPlane-1 + 0*nbPlanes] = new TH1I(name, title, nbBinX, 0, nbBinX);
    histoDisplayHitsTracksAssociation[jPlane-1 + 0*nbPlanes]->SetStats(kTRUE);
    histoDisplayHitsTracksAssociation[jPlane-1 + 0*nbPlanes]->SetXTitle("nb hits");
    histoDisplayHitsTracksAssociation[jPlane-1 + 0*nbPlanes]->SetYTitle("nb events");
    histoDisplayHitsTracksAssociation[jPlane-1 + 0*nbPlanes]-> SetLineColor(3);//Vert
    histoDisplayHitsTracksAssociation[jPlane-1 + 0*nbPlanes]-> SetLineWidth(3);

    sprintf( name, "histoDisplayHitsTracksAssociationUnassoTrack_pl%d_%s", jPlane,tPlane->GetPlanePurpose());
    sprintf( title, "Plane %s (#%d)",tPlane->GetPlanePurpose(), jPlane);
    histoDisplayHitsTracksAssociation[jPlane-1 + 1*nbPlanes] = new TH1I(name, title, nbBinX, 0, nbBinX);
    histoDisplayHitsTracksAssociation[jPlane-1 + 1*nbPlanes]->SetStats(kTRUE);
    histoDisplayHitsTracksAssociation[jPlane-1 + 1*nbPlanes]->SetXTitle("nb hits");
    histoDisplayHitsTracksAssociation[jPlane-1 + 1*nbPlanes]->SetYTitle("nb events");
    histoDisplayHitsTracksAssociation[jPlane-1 + 1*nbPlanes]-> SetLineColor(2);//Rouge
  }
  flagDisplayHitsTracksAssociation_Init=1;
  return 0;
}

Int_t MRax::DisplayHitsTracksAssociation_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsTracksAssociation")) return -1;
  eventsProcessedDisplayHitsTracksAssociation+=1;

  // Filling
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    jPlane = DisplayHitsTracksAssociation_PlanesDrawOrder[iPlane-1];
    tPlane = tTracker->GetPlane(jPlane);
    aList  = tPlane->GetListOfPixels();

    if (tPlane->GetHitsN())
    {
      // hits (un)associated to a track
      Int_t nbassociated=0;
      Int_t nbunassociated=0;
      for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++)
      {
	aHit = (DHit*)tPlane->GetHit(iHit);
	if(aHit->GetFound()) nbassociated++;
	else nbunassociated++;
      }
      histoDisplayHitsTracksAssociation[jPlane-1 + 0*nbPlanes]->Fill(nbassociated);
      histoDisplayHitsTracksAssociation[jPlane-1 + 1*nbPlanes]->Fill(nbunassociated);
    }
  }
  return 0;
}

Int_t MRax::DisplayHitsTracksAssociation_Show(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsTracksAssociation")) return -1;

  // Parameters

  // Declarations

  // Histos
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    padDisplayHitsTracksAssociation->cd(iPlane);
    jPlane = DisplayHitsTracksAssociation_PlanesDrawOrder[iPlane-1];
    // Hits
    histoDisplayHitsTracksAssociation[jPlane-1+ 0*nbPlanes]->Draw();
    histoDisplayHitsTracksAssociation[jPlane-1+ 1*nbPlanes]->Draw("same");

  }
  
  canvasDisplayHitsTracksAssociation->cd();
  sprintf(canvasTitle, "Hits association - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayHitsTracksAssociation);
  labelDisplayHitsTracksAssociation->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);  
  canvasDisplayHitsTracksAssociation->Update();

  return 0;
}

Int_t MRax::DisplayHitsTracksAssociation_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayHitsTracksAssociation";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayHitsTracksAssociation_SaveHistos()
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-histograms in root file in the "results" path
  //-------------
  // Arguments :
  //-------------
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayHitsTracksAssociation";
  histosName = "histosHitsTracksAssociation";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  histosFileFullPath =  resultsDirPath;
  histosFileFullPath += "/";
  histosFileFullPath += FileNamePrefix;
  histosFileFullPath += histosName;
  histosFileFullPath += FileNameSuffix;
  histosFileFullPath += ".root";
  
  histosFile = new TFile(histosFileFullPath,"RECREATE");
  
  // Loop on histos to save them
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause iPlane!=PlanesDrawOrder[iPlane-1];
  {
    histoDisplayHitsTracksAssociation[iPlane-1 + 0*nbPlanes]->Write();
    histoDisplayHitsTracksAssociation[iPlane-1 + 1*nbPlanes]->Write();
  }

  histosFile->Write();
  
  printf_info("DisplayClustersSize");
  printf("Histograms saved in file %s\n",histosFileFullPath.Data());
  
  histosFile->Close();
  delete histosFile;
  
  return 0;
}

Int_t MRax::DisplayHitsTracksAssociation_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayHitsTracksAssociation")) return -1;

  for(  iPlane=1; iPlane<=2*nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
  {
    histoDisplayHitsTracksAssociation[iPlane-1]->Reset();
  }
  eventsProcessedDisplayHitsTracksAssociation=0;
  return 0;
}

Int_t MRax::DisplayHitsTracksAssociation_Kill(void)
{
  if (flagDisplayHitsTracksAssociation_Init)
  {
    // Histos
    for(  iPlane=1; iPlane<=2*nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane=PlanesDrawOrder[i-1];
    {
      // Hits
      delete histoDisplayHitsTracksAssociation[iPlane-1];
    }
    delete [] histoDisplayHitsTracksAssociation;
  }

  if (gROOT->FindObject("canvasDisplayHitsTracksAssociation"))
  {
    delete padDisplayHitsTracksAssociation;
    delete labelDisplayHitsTracksAssociation;
    canvasDisplayHitsTracksAssociation->Close();
    delete canvasDisplayHitsTracksAssociation;
  }

  flagDisplayHitsTracksAssociation_Init=0;
  eventsProcessedDisplayHitsTracksAssociation=0;
  return 0;
}

  //****************************************************************
  //		Tracks and Triggers Properties
  //****************************************************************
  //  * Tracks :
  //	  * hits / track
  //	  * track chi2
  //	  * track-hit mean dist
  //	  * X slope
  //	  * Y slope
  //      * tracks / event
  //  * Triggers
  //	  * triggers / event
  //	  * triggers posiitons
  //****************************************************************
Int_t MRax::DisplayTracksAndTriggersProperties_Init(Bool_t withTriggers, Int_t Chi2Max,Int_t SlopeMax )
{
  if(flagDisplayTracksAndTriggersProperties_Init) return -1;
  eventsProcessedDisplayTracksAndTriggersProperties=0;
  
  flagDisplayTracksAndTriggersProperties_withTrigger = withTriggers;
  // Canvas
  canvasDisplayTracksAndTriggersProperties = new TCanvas("canvasDisplayTracksAndTriggersProperties", "Statistics", 5, 5, 800, 700);
  canvasDisplayTracksAndTriggersProperties->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Events Statistics - Run %d", RunNumber);
  labelDisplayTracksAndTriggersProperties = new TPaveLabel();
  labelDisplayTracksAndTriggersProperties->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayTracksAndTriggersProperties = new TPad("padDisplayTracksAndTriggersProperties","",0.,0.,1.,0.965);
  padDisplayTracksAndTriggersProperties->Draw();
  if(flagDisplayTracksAndTriggersProperties_withTrigger) padDisplayTracksAndTriggersProperties->Divide(5,2);
  else padDisplayTracksAndTriggersProperties->Divide(4,2);
  
  ///////////////////////////////
  // Histo creation and setting
  ///////////////////////////////
  gStyle->SetOptStat("nemruo");

  // HitsByTrack
  sprintf( name, "histoDisplayTracksAndTriggersProperties_HitsByTrack");
  sprintf( title,"Hits / Track");
  //padDisplayTracksAndTriggersProperties->cd(1);
  histoDisplayTracksAndTriggersProperties_HitsByTrack = new TH1I(name,title, nbPlanes+1, 0 ,nbPlanes+1);
  histoDisplayTracksAndTriggersProperties_HitsByTrack->SetStats(kTRUE);
  histoDisplayTracksAndTriggersProperties_HitsByTrack->SetXTitle("Hits");
  histoDisplayTracksAndTriggersProperties_HitsByTrack->SetYTitle("Tracks");
  
  // Track with Memorized Hits                
  sprintf( name, "histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack");
  sprintf( title,"Memorized Hits / Track");
  //padDisplayTracksAndTriggersProperties->cd(2);
  histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack = new TH1I(name,title, 6, 0 ,6);
  histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack->SetStats(kTRUE);
  histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack->SetXTitle("Hits");
  histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack->SetYTitle("Tracks");
  
  // Tracks with Memorized Hits
  sprintf( name, "histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits");
  sprintf( title,"Tracks w(1)/wo(0) Memorized Hits");
  //padDisplayTracksAndTriggersProperties->cd(3);
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits = new TH1I(name,title, 2, 0 ,2);
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->SetStats(kTRUE);
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->SetXTitle("with Memorized Hits");
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->SetYTitle("Tracks");
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->SetMinimum(0);

  // Chi2
  sprintf( name, "histoDisplayTracksAndTriggersProperties_Chi2");
  sprintf( title,"Tracks Chi Square");
  //padDisplayTracksAndTriggersProperties->cd(4);
  histoDisplayTracksAndTriggersProperties_Chi2 = new TH1D(name,title, 50, 0, Chi2Max);
  gStyle->SetOptStat("nemruo");
  histoDisplayTracksAndTriggersProperties_Chi2->SetStats(kTRUE);
  histoDisplayTracksAndTriggersProperties_Chi2->SetXTitle("Chi Square");
  histoDisplayTracksAndTriggersProperties_Chi2->SetYTitle("Tracks");

  // Mean distance track to hits
  sprintf( name, "histoDisplayTracksAndTriggersProperties_DistTr2Hit");
  sprintf( title,"Mean distance Track-Hits");
  //padDisplayTracksAndTriggersProperties->cd(5);
  histoDisplayTracksAndTriggersProperties_DistTr2Hit = new TH1D(name,title, 100, 0, Chi2Max);
  histoDisplayTracksAndTriggersProperties_DistTr2Hit->SetStats(kTRUE);
  histoDisplayTracksAndTriggersProperties_DistTr2Hit->SetXTitle("distance [mm]");
  histoDisplayTracksAndTriggersProperties_DistTr2Hit->SetYTitle("Tracks");

  // Track Slope X
  sprintf( name, "histoDisplayTracksAndTriggersProperties_SlopesX");
  sprintf( title,"X track's slope");
  //padDisplayTracksAndTriggersProperties->cd(6);
  histoDisplayTracksAndTriggersProperties_SlopesX = new TH1D( name, title, 100, -SlopeMax, SlopeMax);
  histoDisplayTracksAndTriggersProperties_SlopesX->SetXTitle("Track's slope around X [degrees]");
  histoDisplayTracksAndTriggersProperties_SlopesX->SetYTitle("Tracks");
  histoDisplayTracksAndTriggersProperties_SlopesX->SetStats(kTRUE);

  // Track Slope Y
  sprintf( name, "histoDisplayTracksAndTriggersProperties_SlopesY");
  sprintf( title,"Y track's slope");
  //padDisplayTracksAndTriggersProperties->cd(7);
  histoDisplayTracksAndTriggersProperties_SlopesY = new TH1D( name, title, 100, -SlopeMax, SlopeMax);
  histoDisplayTracksAndTriggersProperties_SlopesY->SetXTitle("Track's slope around Y [degrees]");
  histoDisplayTracksAndTriggersProperties_SlopesY->SetYTitle("Tracks");
  histoDisplayTracksAndTriggersProperties_SlopesY->SetStats(kTRUE);

  // Track by event
  sprintf( name, "histoDisplayTracksAndTriggersProperties_TracksByEvent");
  sprintf( title,"Tracks by event");
  //padDisplayTracksAndTriggersProperties->cd(8);
  histoDisplayTracksAndTriggersProperties_TracksByEvent = new TH1I( name, title, 10, 0, 10);
  histoDisplayTracksAndTriggersProperties_TracksByEvent->SetXTitle("Tracks by event");
  histoDisplayTracksAndTriggersProperties_TracksByEvent->SetYTitle("Events");
  histoDisplayTracksAndTriggersProperties_TracksByEvent->SetStats(kTRUE);

  if(flagDisplayTracksAndTriggersProperties_withTrigger)
  {
    // Triggers by event
    sprintf( name, "histoDisplayTracksAndTriggersProperties_TriggersByEvent");
    sprintf( title,"Triggers by event");
    //padDisplayTracksAndTriggersProperties->cd(9);
    histoDisplayTracksAndTriggersProperties_TriggersByEvent = new TH1I( name, title, 10, 0, 10);
    histoDisplayTracksAndTriggersProperties_TriggersByEvent->SetXTitle("Triggers by event");
    histoDisplayTracksAndTriggersProperties_TriggersByEvent->SetYTitle("Events");
    histoDisplayTracksAndTriggersProperties_TriggersByEvent->SetStats(kTRUE);

    // Triggers position (line number)
    sprintf( name, "histoDisplayTracksAndTriggersProperties_TriggersPosition");
    sprintf( title,"Triggers position");
    //padDisplayTracksAndTriggersProperties->cd(10);
    tPlane = tTracker->GetPlane(1);
    Int_t nbRow  = tPlane->GetStripsNv();  
    histoDisplayTracksAndTriggersProperties_TriggersPosition = new TH1I( name, title, nbRow+1, 0, nbRow+1);
    histoDisplayTracksAndTriggersProperties_TriggersPosition->SetXTitle("Triggers positions [CMOS line]");
    histoDisplayTracksAndTriggersProperties_TriggersPosition->SetYTitle("Triggers");
    histoDisplayTracksAndTriggersProperties_TriggersPosition->SetStats(kTRUE);
  }

  flagDisplayTracksAndTriggersProperties_Init=1;
  return 0;
}

Int_t MRax::DisplayTracksAndTriggersProperties_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayTracksAndTriggersProperties")) return -1;
  eventsProcessedDisplayTracksAndTriggersProperties+=1;
  
// Tracks
  nbTracks = tTracker->GetTracksN();
  histoDisplayTracksAndTriggersProperties_TracksByEvent->Fill(nbTracks);
  for(iTrack=0; iTrack<nbTracks; iTrack++)
  {
    aTrack=tTracker->GetTrack(iTrack+1);

    if(aTrack->GetChiSquare()) histoDisplayTracksAndTriggersProperties_Chi2       ->Fill(aTrack->GetChiSquare());
    histoDisplayTracksAndTriggersProperties_DistTr2Hit ->Fill(aTrack->GetDistTr2Hit());
    histoDisplayTracksAndTriggersProperties_SlopesX    ->Fill(-TMath::RadToDeg()*TMath::ATan(aTrack->GetLinearFit().GetSlopeZ()(1)));// checked !
    histoDisplayTracksAndTriggersProperties_SlopesY    ->Fill(+TMath::RadToDeg()*TMath::ATan(aTrack->GetLinearFit().GetSlopeZ()(0)));// checked !
    histoDisplayTracksAndTriggersProperties_HitsByTrack->Fill(aTrack->GetHitsNumber());
    if(aTrack->GetHitsMemorizedNumber()==0) histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->Fill(0);
    else histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->Fill(1);
    histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack->Fill(aTrack->GetHitsMemorizedNumber());
  }
  
  if(flagDisplayTracksAndTriggersProperties_withTrigger)
  {
  // Triggers
      // fSession->GetDataAcquisition()->GetTriggers() is a vector filled with triger positions (line)
      // lines are numbered starting 1 to nbRow
    if ((fSession->GetDataAcquisition()->GetTriggers()->at(0)) != 0) 
      nbTriggers = fSession->GetDataAcquisition()->GetTriggers()->size();
    else nbTriggers = -1;

    if (nbTriggers!=-1)
    {
      histoDisplayTracksAndTriggersProperties_TriggersByEvent->Fill(nbTriggers);
      for (int iTrig = 0 ; iTrig < nbTriggers ; iTrig++) \
        histoDisplayTracksAndTriggersProperties_TriggersPosition->Fill(fSession->GetDataAcquisition()->GetTriggers()->at(iTrig));
      //if (nbTriggers==1) nbUnicTriger+=1;
      //else nbMultiTriger+=(nbTriggers-1);
    }
    else histoDisplayTracksAndTriggersProperties_TriggersByEvent->Fill(0);
  }
  
  return 0;
}

Int_t MRax::DisplayTracksAndTriggersProperties_Show(void)
{
  if (!gROOT->FindObject("canvasDisplayTracksAndTriggersProperties")) return -1;

  padDisplayTracksAndTriggersProperties->cd(1);
  histoDisplayTracksAndTriggersProperties_HitsByTrack	->Draw();
  padDisplayTracksAndTriggersProperties->cd(2);
  histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack->Draw();  
  padDisplayTracksAndTriggersProperties->cd(3);
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits   ->Draw();
  padDisplayTracksAndTriggersProperties->cd(4);
  histoDisplayTracksAndTriggersProperties_Chi2	->Draw();
  padDisplayTracksAndTriggersProperties->cd(5);
  histoDisplayTracksAndTriggersProperties_DistTr2Hit  ->Draw();
  padDisplayTracksAndTriggersProperties->cd(6);
  histoDisplayTracksAndTriggersProperties_SlopesX	->Draw();
  padDisplayTracksAndTriggersProperties->cd(7);
  histoDisplayTracksAndTriggersProperties_SlopesY	->Draw();
  padDisplayTracksAndTriggersProperties->cd(8);
  histoDisplayTracksAndTriggersProperties_TracksByEvent->Draw();

  if(flagDisplayTracksAndTriggersProperties_withTrigger)
  {
    padDisplayTracksAndTriggersProperties->cd(9);
    histoDisplayTracksAndTriggersProperties_TriggersByEvent	->Draw();
    padDisplayTracksAndTriggersProperties->cd(10);
    histoDisplayTracksAndTriggersProperties_TriggersPosition	->Draw();
  }
  
  canvasDisplayTracksAndTriggersProperties->cd();
  sprintf(canvasTitle, "Events Statistics - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayTracksAndTriggersProperties);
  labelDisplayTracksAndTriggersProperties->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  canvasDisplayTracksAndTriggersProperties->Update();

  return 0;
}

Int_t MRax::DisplayTracksAndTriggersProperties_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayTracksAndTriggersProperties";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayTracksAndTriggersProperties_SaveHistos()
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-histograms in root file in the "results" path
  //-------------
  // Arguments :
  //-------------
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayTracksAndTriggersProperties";
  histosName = "histosTracksAndTriggersProperties";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  histosFileFullPath =  resultsDirPath;
  histosFileFullPath += "/";
  histosFileFullPath += FileNamePrefix;
  histosFileFullPath += histosName;
  histosFileFullPath += FileNameSuffix;
  histosFileFullPath += ".root";
  
  histosFile = new TFile(histosFileFullPath,"RECREATE");
  
  // HitsByTrack
  histoDisplayTracksAndTriggersProperties_HitsByTrack->Write();

  // Tracks with Memorized Hits
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->Write();
  
  // Tracks with Memorized Hits
  histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack->Write();
  
  // Chi2
  histoDisplayTracksAndTriggersProperties_Chi2->Write();

  // Mean distance track to hits
  histoDisplayTracksAndTriggersProperties_DistTr2Hit->Write();

  // Track Slope X
  histoDisplayTracksAndTriggersProperties_SlopesX->Write();

  // Track Slope Y
  histoDisplayTracksAndTriggersProperties_SlopesY->Write();


  // Track by event
  histoDisplayTracksAndTriggersProperties_TracksByEvent->Write();

  if(flagDisplayTracksAndTriggersProperties_withTrigger)
  {
    // Triggers by event
    histoDisplayTracksAndTriggersProperties_TriggersByEvent->Write();

    // Triggers position (line number)
    histoDisplayTracksAndTriggersProperties_TriggersPosition->Write();
  }

  histosFile->Write();
  
  printf_info("DisplayTracksAndTriggersProperties");
  printf("Histograms saved in file %s\n",histosFileFullPath.Data());
  
  histosFile->Close();
  delete histosFile;
  
  return 0;
}

Int_t MRax::DisplayTracksAndTriggersProperties_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayTracksAndTriggersProperties")) return -1;

  histoDisplayTracksAndTriggersProperties_HitsByTrack      ->Reset();
  histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits->Reset();
  histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack->Reset();
  histoDisplayTracksAndTriggersProperties_Chi2             ->Reset();
  histoDisplayTracksAndTriggersProperties_DistTr2Hit       ->Reset();
  histoDisplayTracksAndTriggersProperties_SlopesX          ->Reset();
  histoDisplayTracksAndTriggersProperties_SlopesY          ->Reset();
  histoDisplayTracksAndTriggersProperties_TracksByEvent    ->Reset();
  if(flagDisplayTracksAndTriggersProperties_withTrigger)
  {
    histoDisplayTracksAndTriggersProperties_TriggersByEvent  ->Reset();
    histoDisplayTracksAndTriggersProperties_TriggersPosition ->Reset();
  }
  eventsProcessedDisplayTracksAndTriggersProperties=0;
  return 0;
}

Int_t MRax::DisplayTracksAndTriggersProperties_Kill(void)
{
  if (flagDisplayTracksAndTriggersProperties_Init)
  {
    delete histoDisplayTracksAndTriggersProperties_HitsByTrack;
    delete histoDisplayTracksAndTriggersProperties_TrackWithMemorizedHits;
    delete histoDisplayTracksAndTriggersProperties_MemorizedHitsByTrack;
    delete histoDisplayTracksAndTriggersProperties_Chi2;
    delete histoDisplayTracksAndTriggersProperties_DistTr2Hit;
    delete histoDisplayTracksAndTriggersProperties_SlopesX;
    delete histoDisplayTracksAndTriggersProperties_SlopesY;
    delete histoDisplayTracksAndTriggersProperties_TracksByEvent;
    if(flagDisplayTracksAndTriggersProperties_withTrigger)
    {
      delete histoDisplayTracksAndTriggersProperties_TriggersByEvent;
      delete histoDisplayTracksAndTriggersProperties_TriggersPosition;
    }
  }

  if (gROOT->FindObject("canvasDisplayTracksAndTriggersProperties"))
  {
    delete padDisplayTracksAndTriggersProperties;
    delete labelDisplayTracksAndTriggersProperties;
    canvasDisplayTracksAndTriggersProperties->Close();
    delete canvasDisplayTracksAndTriggersProperties;
  }
  eventsProcessedDisplayTracksAndTriggersProperties=0;
  flagDisplayTracksAndTriggersProperties_Init=0;
  return 0;
}

  //****************************************************************
  //		Display Chronogram : Hits Tracks Triggers numbers vs time/events
  //****************************************************************
Int_t MRax::DisplayChronogram_Init(Bool_t withTriggers)
{
  if(flagDisplayChronogram_Init) return -1;
  eventsProcessedDisplayChronogram=0;
  maxOFlinesDisplayChronogram=0;
  
  flagDisplayChronogramWithTriggers = withTriggers;
  
  if(fSession->GetSetup()->GetTrackerPar().KeepUnTrackedHitsBetw2evts) 
  {
    printf_warning("DisplayChronogram");
    printf("KeepUnTrackedHitsBetw2evts is enabled ! Take care to yields interpretation (factor 2) ! \n");
  }
  
  // Canvas
  canvasDisplayChronogram = new TCanvas("canvasDisplayChronogram", "Statistics along events", 5, 5, 800, 700);
  canvasDisplayChronogram->UseCurrentStyle();

  // Pave Label
  sprintf(canvasTitle, "Statistics along events - Run %d", RunNumber);
  labelDisplayChronogram = new TPaveLabel();
  labelDisplayChronogram->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  // Pads
  padDisplayChronogram = new TPad("padDisplayChronogram","",0.,0.,1.,0.965);
  padDisplayChronogram->Draw();
  if(flagDisplayChronogramWithTriggers) padDisplayChronogram->Divide(1,5);
  else padDisplayChronogram->Divide(1,4);
  
  // Null Pointers
  graphDisplayChronogram_TracksByEvent   = NULL;
  graphDisplayChronogram_TriggersByEvent = NULL;
  
  graphDisplayChronogram_HitsByEvent = new TGraph*[nbPlanes];
  for (iPlane = 0 ; iPlane < nbPlanes+1 ; iPlane++)
  {
    graphDisplayChronogram_HitsByEvent[iPlane]= NULL;
  }
  
  graphDisplayChronogram_OFlinesNbByEvent = new TGraph*[nbPlanes];
  for (iPlane = 0 ; iPlane < nbPlanes+1 ; iPlane++)
  {
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]= NULL;
  }
  
  vectorStatsAlongEvents_eventNb.clear();
  vectorStatsAlongEvents_nbOfTracks.clear();
  vectorStatsAlongEvents_nbOfTriggers.clear();
  vectorStatsAlongEvents_HitsNb.clear();
  for (iPlane = 0 ; iPlane < nbPlanes+1 ; iPlane++) 
  {
    vectorStatsAlongEvents_HitsNb.push_back((vector<Int_t>)NULL);
    vectorStatsAlongEvents_HitsNb.at(iPlane).clear();
  }
  vectorStatsAlongEvents_OFlinesNb.clear();
  for (iPlane = 0 ; iPlane < nbPlanes+1 ; iPlane++) 
  {
    vectorStatsAlongEvents_OFlinesNb.push_back((vector<Int_t>)NULL);
    vectorStatsAlongEvents_OFlinesNb.at(iPlane).clear();
  }
  
  legendDisplayChronogram_HitsByEvent=NULL;
  legendDisplayChronogram_OFlinesNbByEvent=NULL;
  
  flagDisplayChronogram_Init=1;
  return 0;
}

Int_t MRax::DisplayChronogram_Fill(void)
{
  if (!gROOT->FindObject("canvasDisplayChronogram")) return -1;
  eventsProcessedDisplayChronogram+=1;
  
  vectorStatsAlongEvents_eventNb.push_back(GetCurrentEventNumber());

  Int_t totalHitsNbTemp=0;
  for (iPlane=1; iPlane<=nbPlanes; iPlane++)
  {
    totalHitsNbTemp += tTracker->GetPlane(iPlane)->GetHitsN();
    vectorStatsAlongEvents_HitsNb.at(iPlane).push_back(tTracker->GetPlane(iPlane)->GetHitsN());
  }
  vectorStatsAlongEvents_HitsNb.at(0).push_back(totalHitsNbTemp);

  vectorStatsAlongEvents_nbOfTracks.push_back(tTracker->GetTracksN());

  // Triggers
    // if TrigerMode = 1 : fSession->GetDataAcquisition()->GetTriggers() is a vector filled with triger positions (line)
    // if TrigerMode = 0 : the vector contains only a value : 0
    // lines are numbered starting 1 to nbRow
  if(flagDisplayChronogramWithTriggers)
  {
    if (fSession->GetDataAcquisition()->GetTriggers()->at(0)!=0) \
      vectorStatsAlongEvents_nbOfTriggers.push_back(fSession->GetDataAcquisition()->GetTriggers()->size());
    else vectorStatsAlongEvents_nbOfTriggers.push_back(0);
  }
  
  //OverFlow
 std::vector<int> *ListOfLineOverflow = tDAcq->GetLineOverflow();
  Int_t nb;
  for(  iPlane=1; iPlane<=nbPlanes; iPlane++ ) // For all Planes (maybe not in order cause Plane!=PlanesDrawOrder[i-1];
  {
    tPlane = tTracker->GetPlane(iPlane);
    nb = ListOfLineOverflow[iPlane].size();
    vectorStatsAlongEvents_OFlinesNb.at(iPlane-1).push_back(nb);
    if (nb>maxOFlinesDisplayChronogram && nb<1e3/*due to a bug*/ ) maxOFlinesDisplayChronogram=nb; 
  }

  return 0;
}

Int_t MRax::DisplayChronogram_Show(void)
{
  if (!gROOT->FindObject("canvasDisplayChronogram")) return -1;
  if (vectorStatsAlongEvents_eventNb.size()<1) return -1;

  canvasDisplayChronogram->cd();
  Int_t nbevents = (int)vectorStatsAlongEvents_eventNb.size();
  Int_t *xAxis = (Int_t *)(&vectorStatsAlongEvents_eventNb[0]);
  //Int_t *axeInUseconds;
  Char_t xlabel[20]="event nb";
  
 
//////////////////////////////////
  padDisplayChronogram->cd(1); //Total Hit NB
  if(graphDisplayChronogram_HitsByEvent[0]) delete graphDisplayChronogram_HitsByEvent[0];
  graphDisplayChronogram_HitsByEvent[0] = new TGraph(nbevents,xAxis,(Int_t *)(&vectorStatsAlongEvents_HitsNb.at(0)[0]));
  graphDisplayChronogram_HitsByEvent[0]->SetLineColor(1);
  graphDisplayChronogram_HitsByEvent[0]->SetLineWidth(2);
  graphDisplayChronogram_HitsByEvent[0]->SetMinimum(0);
  graphDisplayChronogram_HitsByEvent[0]->Draw("AL");
  graphDisplayChronogram_HitsByEvent[0]->GetXaxis()->SetTitle(xlabel);
  graphDisplayChronogram_HitsByEvent[0]->GetXaxis()->SetTitleSize(0.06);
  graphDisplayChronogram_HitsByEvent[0]->GetXaxis()->SetTitleOffset(0.5);
  graphDisplayChronogram_HitsByEvent[0]->GetXaxis()->SetLabelSize(0.06);
  graphDisplayChronogram_HitsByEvent[0]->GetYaxis()->SetTitle("total hits nb / event ");
  graphDisplayChronogram_HitsByEvent[0]->GetYaxis()->SetTitleSize(0.06);
  graphDisplayChronogram_HitsByEvent[0]->GetYaxis()->SetTitleOffset(0.5);
  graphDisplayChronogram_HitsByEvent[0]->GetYaxis()->SetLabelSize(0.06);
  graphDisplayChronogram_HitsByEvent[0]->SetTitle("Total hits nb / event");
  
//////////////////////////////////  
  padDisplayChronogram->cd(2);
  Bool_t firstDraw=1;
  
  Double_t posX = 0.90;
  Double_t posY = 0.89;
  Double_t lenX = 0.07;
  Double_t lenY = 0.78;
  legendDisplayChronogram_HitsByEvent = new TLegend(posX-lenX,posY-lenY,posX,posY);

  for (iPlane = 1 ; iPlane < nbPlanes+1 ; iPlane++)
  {
    if(graphDisplayChronogram_HitsByEvent[iPlane]) delete graphDisplayChronogram_HitsByEvent[iPlane];
    
    graphDisplayChronogram_HitsByEvent[iPlane] = new TGraph(nbevents,xAxis,(Int_t *)(&vectorStatsAlongEvents_HitsNb.at(iPlane)[0]));
    if (firstDraw) 
    {
      graphDisplayChronogram_HitsByEvent[iPlane]->Draw("AL");
      firstDraw=0;
    }
    else graphDisplayChronogram_HitsByEvent[iPlane]->Draw("Lsame");
    graphDisplayChronogram_HitsByEvent[iPlane]->SetLineColor(iPlane+1);
    graphDisplayChronogram_HitsByEvent[iPlane]->SetLineWidth(2);
    graphDisplayChronogram_HitsByEvent[iPlane]->SetLineStyle(iPlane);
    graphDisplayChronogram_HitsByEvent[iPlane]->GetXaxis()->SetTitle(xlabel);
    graphDisplayChronogram_HitsByEvent[iPlane]->GetXaxis()->SetTitleSize(0.06);
    graphDisplayChronogram_HitsByEvent[iPlane]->GetXaxis()->SetTitleOffset(0.5);
    graphDisplayChronogram_HitsByEvent[iPlane]->GetXaxis()->SetLabelSize(0.06);
    graphDisplayChronogram_HitsByEvent[iPlane]->GetYaxis()->SetTitle("hits nb / event ");
    graphDisplayChronogram_HitsByEvent[iPlane]->GetYaxis()->SetTitleSize(0.06);
    graphDisplayChronogram_HitsByEvent[iPlane]->GetYaxis()->SetTitleOffset(0.5);
    graphDisplayChronogram_HitsByEvent[iPlane]->GetYaxis()->SetLabelSize(0.06);
    graphDisplayChronogram_HitsByEvent[iPlane]->SetTitle("Hits / event");
    graphDisplayChronogram_HitsByEvent[iPlane]->SetMinimum(0);
    
    legendDisplayChronogram_HitsByEvent->AddEntry(graphDisplayChronogram_HitsByEvent[iPlane],tTracker->GetPlane(iPlane)->GetPlanePurpose() , "L");
  }
  legendDisplayChronogram_HitsByEvent->Draw();
    
  
//////////////////////////////////
  padDisplayChronogram->cd(3);
  if(graphDisplayChronogram_TracksByEvent) delete graphDisplayChronogram_TracksByEvent;
  graphDisplayChronogram_TracksByEvent = new TGraph(nbevents,xAxis,(Int_t *)(&vectorStatsAlongEvents_nbOfTracks[0]));
  graphDisplayChronogram_TracksByEvent->SetLineColor(1);
  graphDisplayChronogram_TracksByEvent->SetMarkerStyle(5);
  graphDisplayChronogram_TracksByEvent->Draw("AL");
  graphDisplayChronogram_TracksByEvent->GetXaxis()->SetTitle(xlabel);
  graphDisplayChronogram_TracksByEvent->GetXaxis()->SetTitleSize(0.06);
  graphDisplayChronogram_TracksByEvent->GetXaxis()->SetTitleOffset(0.5);
  graphDisplayChronogram_TracksByEvent->GetXaxis()->SetLabelSize(0.06);
  graphDisplayChronogram_TracksByEvent->GetYaxis()->SetTitle("tracks nb / event");
  graphDisplayChronogram_TracksByEvent->GetYaxis()->SetTitleSize(0.06);
  graphDisplayChronogram_TracksByEvent->GetYaxis()->SetTitleOffset(0.5);
  graphDisplayChronogram_TracksByEvent->GetYaxis()->SetLabelSize(0.06);
  graphDisplayChronogram_TracksByEvent->SetTitle("Tracks / event");

//////////////////////////////////  
  padDisplayChronogram->cd(4);
  firstDraw=1;
  
  posX = 0.90;
  posY = 0.89;
  lenX = 0.07;
  lenY = 0.78;
  legendDisplayChronogram_OFlinesNbByEvent = new TLegend(posX-lenX,posY-lenY,posX,posY);

  for (iPlane = 0 ; iPlane < nbPlanes ; iPlane++)
  {
    if(graphDisplayChronogram_OFlinesNbByEvent[iPlane]) delete graphDisplayChronogram_OFlinesNbByEvent[iPlane];
    
    graphDisplayChronogram_OFlinesNbByEvent[iPlane] = new TGraph(nbevents,xAxis,(Int_t *)(&vectorStatsAlongEvents_OFlinesNb.at(iPlane)[0]));
    if (firstDraw) 
    {
      graphDisplayChronogram_OFlinesNbByEvent[iPlane]->Draw("AL");
      firstDraw=0;
    }
    else graphDisplayChronogram_OFlinesNbByEvent[iPlane]->Draw("Lsame");
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->SetLineColor(iPlane+1);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->SetLineWidth(2);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->SetLineStyle(iPlane);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetXaxis()->SetTitle(xlabel);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetXaxis()->SetTitleSize(0.06);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetXaxis()->SetTitleOffset(0.5);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetXaxis()->SetLabelSize(0.06);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetYaxis()->SetTitle("OverFlow lines / event ");
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetYaxis()->SetTitleSize(0.06);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetYaxis()->SetTitleOffset(0.5);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->GetYaxis()->SetLabelSize(0.06);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->SetTitle("Overflow Lines / event");
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->SetMinimum(0);
    graphDisplayChronogram_OFlinesNbByEvent[iPlane]->SetMaximum(Double_t(maxOFlinesDisplayChronogram));
    legendDisplayChronogram_OFlinesNbByEvent->AddEntry(graphDisplayChronogram_OFlinesNbByEvent[iPlane],tTracker->GetPlane(iPlane+1)->GetPlanePurpose() , "L");
  }
  legendDisplayChronogram_OFlinesNbByEvent->Draw();  
  
  
//////////////////////////////////  
  if(flagDisplayChronogramWithTriggers)
  {
    padDisplayChronogram->cd(5);
    if(graphDisplayChronogram_TriggersByEvent) delete graphDisplayChronogram_TriggersByEvent;
    graphDisplayChronogram_TriggersByEvent = new TGraph(nbevents,xAxis,(Int_t *)(&vectorStatsAlongEvents_nbOfTriggers[0]));
    graphDisplayChronogram_TriggersByEvent->SetLineColor(3);
    graphDisplayChronogram_TriggersByEvent->SetMarkerStyle(5);
    graphDisplayChronogram_TriggersByEvent->Draw("AL");
    graphDisplayChronogram_TriggersByEvent->GetXaxis()->SetTitle(xlabel);
    graphDisplayChronogram_TriggersByEvent->GetXaxis()->SetTitleSize(0.06);
    graphDisplayChronogram_TriggersByEvent->GetXaxis()->SetTitleOffset(0.5);
    graphDisplayChronogram_TriggersByEvent->GetXaxis()->SetLabelSize(0.06);
    graphDisplayChronogram_TriggersByEvent->GetYaxis()->SetTitle("triggers / event ");
    graphDisplayChronogram_TriggersByEvent->GetYaxis()->SetTitleSize(0.06);
    graphDisplayChronogram_TriggersByEvent->GetYaxis()->SetTitleOffset(0.5);
    graphDisplayChronogram_TriggersByEvent->GetYaxis()->SetLabelSize(0.06);
    graphDisplayChronogram_TriggersByEvent->SetTitle("Triggers / event");
  }
  
  canvasDisplayChronogram->cd();
  sprintf(canvasTitle, "Statistics along events - Run %d - over %d event(s)", RunNumber, eventsProcessedDisplayChronogram);
  labelDisplayChronogram->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  canvasDisplayChronogram->Update();

  return 0;
}

Int_t MRax::DisplayChronogram_Save(vector<TString> &vectorStringFilesExtensions)
{
  //-------------
  // Description :
  //-------------
  //   Save the method-corresponding-canvas in file(s) in the "results" path
  //   Files extensions is given in argument
  //-------------
  // Arguments :
  //-------------
  //  std::vector<TString> &vectorStringFilesExtensions : a TString vector that should contains extensions, like "pdf","root",etc
  //
  //-------------
  // Configuration :
  //-------------
  //  Canvas Name related to this method, mandatory !
  canvasName = "canvasDisplayChronogram";
  //----------------------------------------------------------------------------------------------------


  // Test if the canvas exists, if no, quit
  if (! (aCanvas = (TCanvas*)gROOT->FindObject(canvasName.Data())) ) return -1;

  // Full name of the file to create, without file extension
  canvasFileFullPath =  resultsDirPath;
  canvasFileFullPath += "/";
  canvasFileFullPath += FileNamePrefix;
  canvasFileFullPath += canvasName;
  canvasFileFullPath += FileNameSuffix;
  canvasFileFullPath += ".";

  // Loop on extensions vector to save canvas
  for(vector<TString>::const_iterator it=vectorStringFilesExtensions.begin(); it!=vectorStringFilesExtensions.end(); ++it)
  {
    aFileName = canvasFileFullPath;
    aFileName += *it;
    aCanvas->Print(aFileName.Data());
  }

  return 0;
}

Int_t MRax::DisplayChronogram_Reset(void)
{
  if (!gROOT->FindObject("canvasDisplayChronogram")) return -1;

  vectorStatsAlongEvents_eventNb.clear();
  vectorStatsAlongEvents_nbOfTracks.clear();
  vectorStatsAlongEvents_nbOfTriggers.clear();

  for (iPlane = 0 ; iPlane < nbPlanes+1 ; iPlane++)
  {
    vectorStatsAlongEvents_HitsNb.at(iPlane).clear();
  }
  for (iPlane = 0 ; iPlane < nbPlanes ; iPlane++)
  {
    vectorStatsAlongEvents_OFlinesNb.at(iPlane).clear();
  }

  if(graphDisplayChronogram_HitsByEvent)
  {
    for (iPlane = 0 ; iPlane < nbPlanes+1 ; iPlane++)
    { 
      delete graphDisplayChronogram_HitsByEvent[iPlane];
      graphDisplayChronogram_HitsByEvent[iPlane]= NULL;
    }
    delete legendDisplayChronogram_HitsByEvent;
    legendDisplayChronogram_HitsByEvent = NULL;
  }
  
  if(graphDisplayChronogram_OFlinesNbByEvent)
  {
    for (iPlane = 0 ; iPlane < nbPlanes ; iPlane++)
    { 
      delete graphDisplayChronogram_OFlinesNbByEvent[iPlane];
      graphDisplayChronogram_OFlinesNbByEvent[iPlane]= NULL;
    }
    delete legendDisplayChronogram_OFlinesNbByEvent;
    legendDisplayChronogram_OFlinesNbByEvent= NULL;
  }
  
  if(graphDisplayChronogram_TracksByEvent)
  {
    delete graphDisplayChronogram_TracksByEvent;
    graphDisplayChronogram_TracksByEvent=NULL;
  }
  
  if(graphDisplayChronogram_TriggersByEvent)
  {
    delete graphDisplayChronogram_TriggersByEvent;
    graphDisplayChronogram_TriggersByEvent=NULL;
  }
  eventsProcessedDisplayChronogram=0;
  maxOFlinesDisplayChronogram=0;
  return 0;
}

Int_t MRax::DisplayChronogram_Kill(void)
{
  DisplayChronogram_Reset();

  if (gROOT->FindObject("canvasDisplayChronogram"))
  {
    delete padDisplayChronogram;
    delete labelDisplayChronogram;
    canvasDisplayChronogram->Close();
    delete canvasDisplayChronogram;
  }
  flagDisplayChronogram_Init=0;
  eventsProcessedDisplayChronogram=0;
  maxOFlinesDisplayChronogram=0;
  return 0;
}

//______________________________________________________________________________________________________
//
//	Store and compute functions
//______________________________________________________________________________________________________
  //****************************************************************
  //		Store tracks in memory / save on HDD if asked
  //****************************************************************
  // Common function for
  //	* DisplayTracksIn3D DisplayTracksIn2D
  //	* ComputeVertices
  // Analyse events :
  //	* Hits (if asked)
  //	* Tracks
  // Store results in vectors of structs
  //****************************************************************
Int_t MRax::StoreTracks_Init(void)
{
  if (flagStoreTracks_Init) return -1;
  flagStoreTracks_Init=1;
  eventsProcessedStoreTracks=0;
  return 0;
}

Int_t MRax::StoreTracks_Fill(Bool_t FillHitsVector)
{
  //DEBUG ON/OFF :
  //#define DEBUG_StoreTracks_Fill 1
  
  if (!flagStoreTracks_Init) return -1;
  
  #ifdef DEBUG_StoreTracks_Fill
    cout << endl <<  "*** DEBUG_StoreTracks_Fill ENABLED ***" << endl;
  #endif
  
  flagComputeVertices_Done=0; // If new values in tracksVector, verticesVector must be recompute !
  eventsProcessedStoreTracks+=1;
 
 // Fill hits structure if needed
  if (FillHitsVector)
  {
    aHitsStruct.HitsPositions.clear();
    aHitsStruct.HitsPlanes.clear();

    for(iPlane=1; iPlane<=nbPlanes; iPlane++)
    {
      tPlane = tTracker->GetPlane(iPlane);
      if( tPlane->GetHitsN()>0 )
      {
        for( iHit=1; iHit<=tPlane->GetHitsN(); iHit++) //loop on hits (starts at 1 !!)
        {
          aHit = (DHit*)tPlane->GetHit( iHit);
          aHitsStruct.HitsPositions.push_back(tPlane->PlaneToTracker(*(aHit->GetPosition())));
          aHitsStruct.HitsPlanes.push_back(iPlane);
        }
      }
    }
    hitsVector.push_back(aHitsStruct);
  }

  // Fill track structure
  nbTracks = tTracker->GetTracksN();
  for(iTrack=0; iTrack<nbTracks; iTrack++)
  {
    aTrack=tTracker->GetTrack(iTrack+1);

    aTrackStruct.TracksNbInEvent = nbTracks;
    aTrackStruct.TrackOrigin     = aTrack->GetLinearFit().GetOrigin();
    aTrackStruct.TrackSlopeZ     = aTrack->GetLinearFit().GetSlopeZ();
    aTrackStruct.TrackChi2       = aTrack->GetChiSquare();
    aTrackStruct.NbMemorizedHits = aTrack->GetHitsMemorizedNumber();

    aTrackStruct.HitsPositions.clear();
    aTrackStruct.HitsPlanes.clear();
    aTrackStruct.HitsPixsNb.clear();
    aTrackStruct.HitsMemorized.clear();
    
    for (iHit=0; iHit < aTrack->GetHitsNumber(); iHit++)
    {
      aTrackStruct.HitsPositions.push_back( aTrack->GetHit(iHit)->GetPlane()->PlaneToTracker(*aTrack->GetHit(iHit)->GetPosition()));
      aTrackStruct.HitsPlanes.push_back(    aTrack->GetHit(iHit)->GetPlane()->GetPlaneNumber());
      aTrackStruct.HitsPixsNb.push_back(    aTrack->GetHit(iHit)->GetStripsInCluster());
      aTrackStruct.HitsMemorized.push_back( aTrack->GetHit(iHit)->GetIsFromPreviousEvent()); 
    }
    tracksVector.push_back(aTrackStruct);
  }
  
  //debug
  #ifdef DEBUG_StoreTracks_Fill
    cout  << "DEBUG : Loop iTrack<tracksVector.size() starts" << endl ;
    for(iTrack=0; iTrack<(int)tracksVector.size(); iTrack++)
    {
      cout << " * track #" << iTrack+1 << " : #hits = " << tracksVector.at(iTrack).HitsPositions.size() << " =?= " << tracksVector.at(iTrack).HitsPlanes.size() << " =?= " << tracksVector.at(iTrack).HitsPixsNb.size() << endl;
      for (iHit=0; iHit < (int)tracksVector.at(iTrack).HitsPlanes.size() ; iHit++)
      {
        cout << "    -> Plane# " << tracksVector.at(iTrack).HitsPlanes.at(iHit) << " : pixels=" << tracksVector.at(iTrack).HitsPixsNb.at(iHit) << endl ;
      }
    }
  #endif

  return 0;
}

Int_t MRax::StoreTracks_Reset(void)
{
  if(!flagStoreTracks_Init) return -1;

  // Hits
  for(iEventWithHits=0; iEventWithHits<int(hitsVector.size()); iEventWithHits++) {
    hitsVector.at(iEventWithHits).HitsPositions.clear();
    hitsVector.at(iEventWithHits).HitsPlanes.clear();
  }
  hitsVector.clear();

  // Tracks
  for(iTrack=0; iTrack<int(tracksVector.size()); iTrack++) {
    tracksVector.at(iTrack).HitsPositions.clear();
    tracksVector.at(iTrack).HitsPlanes.clear();
    tracksVector.at(iTrack).HitsPixsNb.clear();
    tracksVector.at(iTrack).HitsMemorized.clear();
  }
  tracksVector.clear();

  eventsProcessedStoreTracks=0;
  return 0;

}

Int_t MRax::StoreTracks_Kill(void)
{
  if(!flagStoreTracks_Init) return -1;

  StoreTracks_Reset();
  flagStoreTracks_Init=0;
  eventsProcessedStoreTracks=0;
  return 0;
}

Int_t MRax::StoreTracks_SaveTracks(TString &filenameSaveTracks)
{
	// VERSION 20

  //DEBUG ON/OFF :
  //#define DEBUG_StoreTracks_SaveTracks 1
  
  if (!flagStoreTracks_Init) return -1;
  
  #ifdef DEBUG_StoreTracks_SaveTracks
  cout << endl <<  "*** DEBUG_StoreTracks_SaveTracks ENABLED ***" << endl;
  #endif
  
  printf_info("StoreTracks_SaveTracks"); printf("-> Creating the ROOT file with tracks data ...\n");
  
  //create a Tree file
  // File
  TFile treeFile( filenameSaveTracks.Data(), "recreate");

  //Tree and branches
  TTree treeTracks("TafTrackV02","Tree with tracks data from TAF, export version 2.0");

  treeTracks.Branch("TracksNbInEvent"   ,&aTrackStructExportV20.TracksNbInEvent  ,"TracksNbInEvent/I");
  treeTracks.Branch("TrackOrigin_x"     ,&aTrackStructExportV20.TrackOrigin_x    ,"TrackOrigin_x/D");
  treeTracks.Branch("TrackOrigin_y"     ,&aTrackStructExportV20.TrackOrigin_y    ,"TrackOrigin_y/D");
	treeTracks.Branch("TrackOrigin_z"     ,&aTrackStructExportV20.TrackOrigin_z    ,"TrackOrigin_z/D");
	treeTracks.Branch("TrackSlope_x"      ,&aTrackStructExportV20.TrackSlope_x     ,"TrackSlope_x/D");
  treeTracks.Branch("TrackSlope_y"      ,&aTrackStructExportV20.TrackSlope_y     ,"TrackSlope_y/D");
	treeTracks.Branch("TrackSlope_z"      ,&aTrackStructExportV20.TrackSlope_z     ,"TrackSlope_z/D");
	treeTracks.Branch("TrackChi2"         ,&aTrackStructExportV20.TrackChi2        ,"TrackChi2/D");
	treeTracks.Branch("TotalHitsNb"       ,&aTrackStructExportV20.TotalHitsNb      ,"TotalHitsNb/I");
  treeTracks.Branch("MemorizedHitsNb"   ,&aTrackStructExportV20.MemorizedHitsNb  ,"MemorizedHitsNb/I");
	treeTracks.Branch("CMOS_1L_PixInClust",&aTrackStructExportV20.CMOS_1L_PixInClust,"CMOS_1L_PixInClust/I");
	treeTracks.Branch("CMOS_2L_PixInClust",&aTrackStructExportV20.CMOS_2L_PixInClust,"CMOS_2L_PixInClust/I");
	treeTracks.Branch("CMOS_3L_PixInClust",&aTrackStructExportV20.CMOS_3L_PixInClust,"CMOS_3L_PixInClust/I");
	treeTracks.Branch("CMOS_4L_PixInClust",&aTrackStructExportV20.CMOS_4L_PixInClust,"CMOS_4L_PixInClust/I");
	treeTracks.Branch("CMOS_1R_PixInClust",&aTrackStructExportV20.CMOS_1R_PixInClust,"CMOS_1R_PixInClust/I");
	treeTracks.Branch("CMOS_2R_PixInClust",&aTrackStructExportV20.CMOS_2R_PixInClust,"CMOS_2R_PixInClust/I");
	treeTracks.Branch("CMOS_3R_PixInClust",&aTrackStructExportV20.CMOS_3R_PixInClust,"CMOS_3R_PixInClust/I");
	treeTracks.Branch("CMOS_4R_PixInClust",&aTrackStructExportV20.CMOS_4R_PixInClust,"CMOS_4R_PixInClust/I");
  treeTracks.Branch("CMOS_1L_HitMemorized",&aTrackStructExportV20.CMOS_1L_HitMemorized,"CMOS_1L_HitMemorized/O");
  treeTracks.Branch("CMOS_2L_HitMemorized",&aTrackStructExportV20.CMOS_2L_HitMemorized,"CMOS_2L_HitMemorized/O");
  treeTracks.Branch("CMOS_3L_HitMemorized",&aTrackStructExportV20.CMOS_3L_HitMemorized,"CMOS_3L_HitMemorized/O");
  treeTracks.Branch("CMOS_4L_HitMemorized",&aTrackStructExportV20.CMOS_4L_HitMemorized,"CMOS_4L_HitMemorized/O");
  treeTracks.Branch("CMOS_1R_HitMemorized",&aTrackStructExportV20.CMOS_1R_HitMemorized,"CMOS_1R_HitMemorized/O");
  treeTracks.Branch("CMOS_2R_HitMemorized",&aTrackStructExportV20.CMOS_2R_HitMemorized,"CMOS_2R_HitMemorized/O");
  treeTracks.Branch("CMOS_3R_HitMemorized",&aTrackStructExportV20.CMOS_3R_HitMemorized,"CMOS_3R_HitMemorized/O");
  treeTracks.Branch("CMOS_4R_HitMemorized",&aTrackStructExportV20.CMOS_4R_HitMemorized,"CMOS_4R_HitMemorized/O");

  // Check correspondance between plane NB and plane name :
  printf("-> Please check that these correspondances are true (if not, edit MRax.cxx MRax::StoreTracks_SaveTracks method) :\n");
  
  cout << " *** Planes names order via config file ***" << endl;
  for (iPlane=1 ; iPlane<=nbPlanes; iPlane++)
  {
    cout << "  " << iPlane << ") " << tTracker->GetPlane(iPlane)->GetPlanePurpose() << endl; 
  }
  
  cout << " *** Planes names order hardcoded for ROOT file ***" << endl;
  cout << "  1) CMOS_1L" << endl; 
  cout << "  2) CMOS_2L" << endl; 
  cout << "  3) CMOS_3L" << endl; 
  cout << "  4) CMOS_4L" << endl; 
  cout << "  5) CMOS_1R" << endl; 
  cout << "  6) CMOS_2R" << endl; 
  cout << "  7) CMOS_3R" << endl; 
  cout << "  8) CMOS_4R" << endl; 
  
  cout << endl;
  
 
  #ifdef DEBUG_StoreTracks_SaveTracks
      cout  << "DEBUG : Loop iTrack<tracksVector.size() starts" << endl ;
  #endif 
  // Saving data via scan
  for (iTrack=0; iTrack<int(tracksVector.size()); iTrack++) {
    aTrackStruct = tracksVector.at(iTrack);
		
		aTrackStructExportV20.TracksNbInEvent    = aTrackStruct.TracksNbInEvent ;
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TracksNbInEvent " << aTrackStructExportV20.TracksNbInEvent << " =?= " << aTrackStruct.TracksNbInEvent << endl ;
    #endif 
    
    aTrackStructExportV20.TrackOrigin_x      = aTrackStruct.TrackOrigin(0)/1000.;
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TrackOrigin_x " << aTrackStructExportV20.TrackOrigin_x << " =?= " << aTrackStruct.TrackOrigin(0) << endl ;
    #endif 
    
    aTrackStructExportV20.TrackOrigin_y      = aTrackStruct.TrackOrigin(1)/1000.;
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TrackOrigin_y " << aTrackStructExportV20.TrackOrigin_y << " =?= " << aTrackStruct.TrackOrigin(1) << endl ;
    #endif 
    
		aTrackStructExportV20.TrackOrigin_z      = aTrackStruct.TrackOrigin(2)/1000.;
		#ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TrackOrigin_z " << aTrackStructExportV20.TrackOrigin_z << " =?= " << aTrackStruct.TrackOrigin(2) << endl ;
    #endif 
    
    aTrackStructExportV20.TrackSlope_x       = aTrackStruct.TrackSlopeZ(0);
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TrackSlope_x " << aTrackStructExportV20.TrackSlope_x << " =?= " << aTrackStruct.TrackSlopeZ(0) << endl ;
    #endif 
      
		aTrackStructExportV20.TrackSlope_y       = aTrackStruct.TrackSlopeZ(1);
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TrackSlope_y " << aTrackStructExportV20.TrackSlope_y << " =?= " << aTrackStruct.TrackSlopeZ(1) << endl ;
    #endif 
      
		aTrackStructExportV20.TrackSlope_z       = aTrackStruct.TrackSlopeZ(2);
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TrackSlope_z " << aTrackStructExportV20.TrackSlope_z << " =?= " << aTrackStruct.TrackSlopeZ(2) << endl ;
    #endif 
      
		aTrackStructExportV20.TrackChi2          = aTrackStruct.TrackChi2;
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TrackChi2 " << aTrackStructExportV20.TrackChi2 << " =?= " << aTrackStruct.TrackChi2 << endl ;
    #endif
      
		aTrackStructExportV20.TotalHitsNb    = aTrackStruct.HitsPlanes.size();
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.TotalHitsNb " << aTrackStructExportV20.TotalHitsNb << " =?= " << aTrackStruct.HitsPlanes.size() << endl ;
    #endif
      
    aTrackStructExportV20.MemorizedHitsNb    = aTrackStruct.NbMemorizedHits;
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * aTrackStructExportV20.MemorizedHitsNb " << aTrackStructExportV20.MemorizedHitsNb << " =?= " << aTrackStruct.NbMemorizedHits << endl ;
    #endif
      
    // init : 0 means plane not touched
    aTrackStructExportV20.CMOS_1L_PixInClust = 0;
    aTrackStructExportV20.CMOS_2L_PixInClust = 0;
    aTrackStructExportV20.CMOS_3L_PixInClust = 0;
    aTrackStructExportV20.CMOS_4L_PixInClust = 0;
    aTrackStructExportV20.CMOS_1R_PixInClust = 0;
    aTrackStructExportV20.CMOS_2R_PixInClust = 0;
    aTrackStructExportV20.CMOS_3R_PixInClust = 0;
    aTrackStructExportV20.CMOS_4R_PixInClust = 0;
    
    // 0 means his is not from previous event
    aTrackStructExportV20.CMOS_1L_HitMemorized = 0;
    aTrackStructExportV20.CMOS_2L_HitMemorized = 0;
    aTrackStructExportV20.CMOS_3L_HitMemorized = 0;
    aTrackStructExportV20.CMOS_4L_HitMemorized = 0;
    aTrackStructExportV20.CMOS_1R_HitMemorized = 0;
    aTrackStructExportV20.CMOS_2R_HitMemorized = 0;
    aTrackStructExportV20.CMOS_3R_HitMemorized = 0;
    aTrackStructExportV20.CMOS_4R_HitMemorized = 0;
    
    // fill
    #ifdef DEBUG_StoreTracks_SaveTracks
      cout << " * DEBUG : Loop iTouchedPlane < aTrackStruct.HitsPlanes.size() = " << aTrackStruct.HitsPlanes.size() << " starts" << endl ;
    #endif 
    for (Int_t iTouchedPlane=0 ; iTouchedPlane < int(aTrackStruct.HitsPlanes.size()); iTouchedPlane++) {
      #ifdef DEBUG_StoreTracks_SaveTracks
        cout << "   * iTouchedPlane+1 = " << iTouchedPlane+1 << " / " << aTrackStruct.HitsPlanes.size() << " : Plane " << aTrackStruct.HitsPlanes.at(iTouchedPlane) << " = " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " pixels" << endl;
      #endif
        
      //iPlane = 1 : 1L
      if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 1)
      {
        aTrackStructExportV20.CMOS_1L_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_1L_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #1-1L " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_1L_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_1L_HitMemorized << endl;
        #endif
      }
      
      //iPlane = 2 : 2L
      else if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 2)
      {
        aTrackStructExportV20.CMOS_2L_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_2L_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #2-2L " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_2L_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_2L_HitMemorized << endl;
        #endif
      }
      
      //iPlane = 3 : 3L
      else if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 3)
      {
        aTrackStructExportV20.CMOS_3L_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_3L_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #3-3L " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_3L_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_3L_HitMemorized << endl;
        #endif
      }
      
      //iPlane = 4 : 4L
      else if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 4)
      {
        aTrackStructExportV20.CMOS_4L_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_4L_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #4-4L " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_4L_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_4L_HitMemorized << endl;
        #endif
      }
      
      //iPlane = 5 : 1R
      else if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 5)
      {
        aTrackStructExportV20.CMOS_1R_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_1R_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #5-1R " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_1R_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_1R_HitMemorized << endl;
        #endif
      }
      
      //iPlane = 6 : 2R
      else if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 6)
      {
        aTrackStructExportV20.CMOS_2R_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_2R_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #6-2R " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_2R_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_2R_HitMemorized << endl;
        #endif
      }
      
      //iPlane = 7 : 3R
      else if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 7)
      {
        aTrackStructExportV20.CMOS_3R_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_3R_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #7-3R " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_3R_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_3R_HitMemorized << endl;
        #endif
      }
      
      //iPlane = 8 : 4R
      else if(aTrackStruct.HitsPlanes.at(iTouchedPlane) == 8)
      {
        aTrackStructExportV20.CMOS_4R_PixInClust   = aTrackStruct.HitsPixsNb.at(iTouchedPlane);
        aTrackStructExportV20.CMOS_4R_HitMemorized = aTrackStruct.HitsMemorized.at(iTouchedPlane);
        #ifdef DEBUG_StoreTracks_SaveTracks
          cout << "          plane #8-4R " << aTrackStruct.HitsPixsNb.at(iTouchedPlane) << " =?= " << aTrackStructExportV20.CMOS_4R_PixInClust << " ; hit memorized = " << aTrackStructExportV20.CMOS_4R_HitMemorized << endl;
        #endif
      }
    }


    treeTracks.Fill();
  }

  if (!treeFile.Write())
  {
    printf_error("StoreTracks_SaveTracks"); printf("ERROR while saving Tracks in TTree file : %s \n",filenameSaveTracks.Data());
    return -1;
  }

  printf_info("StoreTracks_SaveTracks"); printf("-> %d Tracks saved in a TTree in ROOT file : %s \n",int(treeTracks.GetEntries()),filenameSaveTracks.Data());

  return 0;
}


  //****************************************************************
  //		Compute Vertices
  //****************************************************************
Int_t MRax::ComputeVertices_Init(void)
{
  if(flagComputeVertices_Init) return -1;
  StoreTracks_Init();

  //////////////////////
  // Coordinates indexes
  //////////////////////
  id_i=0;id_j=1;id_k=2;
  
  ////////////////////////////////
  // Configuration from DSetup
  ////////////////////////////////
  printf_info ("ComputeVertices_Init") ;
  cout << "Geometrical parameters:" << endl ;
//   in DSetup : struct IviGeometryParameter_t 
//   {
//       enum        {tpsz = 256};
//       Char_t	GeometryName[tpsz];
//       Char_t	GeometryVersion[tpsz];
//       DR3		BeamOrigin;
//       DR3		BeamSlope;
//       Char_t	TargetType[tpsz];
//       DR3		TargetSize;
//       Float_t     TargetRadius;
//       Float_t     TargetLength;
//       Char_t      TargetAxis[1];    
//       DR3		TargetCenter;
//       DR3		TrackerOrigin;
//       DR3		TrackerTilt;
//       Char_t	VertexingMethod[tpsz];
//   };
//
//   in MRax.h  
//     TString ComputeVertices_GeometryName;
//     TString ComputeVertices_GeometryVersion;
//     DR3     ComputeVertices_BeamOrigin;
//     DR3     ComputeVertices_BeamSlope;
//     Int_t   ComputeVertices_TargetType;
//     TString ComputeVertices_TargetTypeString;
//     DR3     ComputeVertices_TargetSize;
//     Float_t ComputeVertices_TargetRadius;
//     Float_t ComputeVertices_TargetLength;
//     TString ComputeVertices_TargetAxis;
//     DR3     ComputeVertices_TargetCenter;
//     DR3     ComputeVertices_TrackerOrigin;
//     DR3     ComputeVertices_TrackerTilt;
//     Int_t   ComputeVertices_VertexingMethod;
//     TString ComputeVertices_VertexingMethodString;
  

  //////////////////////
  // Geometry description
  //////////////////////
  ComputeVertices_GeometryName    = fSetup->GetIviGeometryParameter().GeometryName;
  cout << "      -> GeometryName:           " << ComputeVertices_GeometryName << endl ;
  
  ComputeVertices_GeometryVersion = fSetup->GetIviGeometryParameter().GeometryVersion;  
  cout << "      -> GeometryVersion:        " << ComputeVertices_GeometryVersion << endl ;

  //////////////////////////
  // Beam
  //////////////////////////
  ComputeVertices_BeamOrigin      = fSetup->GetIviGeometryParameter().BeamOrigin;
  cout << "      -> BeamOrigin [mm]:    I:  " << ComputeVertices_BeamOrigin(0) << endl ;
  cout << "                             J:  " << ComputeVertices_BeamOrigin(1) << endl ;
  cout << "                             K:  " << ComputeVertices_BeamOrigin(2) << endl ;
    
  ComputeVertices_BeamSlope       = fSetup->GetIviGeometryParameter().BeamSlope;
  cout << "      -> BeamSlope [/k]:     I:  " << ComputeVertices_BeamSlope(0) << endl ;
  cout << "                             J:  " << ComputeVertices_BeamSlope(1) << endl ;
  cout << "                             K:  " << ComputeVertices_BeamSlope(2) << endl ;
  
  Beam2points_ExperimentFrame[0]=ComputeVertices_BeamOrigin;
  Double_t i2 = ComputeVertices_BeamOrigin(0) + ComputeVertices_BeamSlope(0)*ComputeVertices_BeamSlope(2);
  Double_t j2 = ComputeVertices_BeamOrigin(1) + ComputeVertices_BeamSlope(1)*ComputeVertices_BeamSlope(2);
  Double_t k2 = ComputeVertices_BeamOrigin(2) + ComputeVertices_BeamSlope(2);
  Beam2points_ExperimentFrame[1].SetValue(i2,j2,k2); 
  
  cout << "        -> BeamPoint 1:      I:  " << Beam2points_ExperimentFrame[0](0) << endl ;
  cout << "                             J:  " << Beam2points_ExperimentFrame[0](1) << endl ;
  cout << "                             K:  " << Beam2points_ExperimentFrame[0](2) << endl ;
  cout << "        -> BeamPoint 2:      I:  " << Beam2points_ExperimentFrame[1](0) << endl ;
  cout << "                             J:  " << Beam2points_ExperimentFrame[1](1) << endl ;
  cout << "                             K:  " << Beam2points_ExperimentFrame[1](2) << endl ;
  
  
  //////////////////////
  // Target
  //////////////////////
  ComputeVertices_TargetTypeString      = fSetup->GetIviGeometryParameter().TargetType;
  cout << "      -> TargetType:             " << ComputeVertices_TargetTypeString << endl ;
  if(!strcmp(ComputeVertices_TargetTypeString,"cuboid"))
  {
    ComputeVertices_TargetType = 1;
    cout << "        -> Is supported, OK" << endl ;
    
    ComputeVertices_TargetSize       = fSetup->GetIviGeometryParameter().TargetSize;
    cout << "        -> TargetSize [mm]   I:  " << ComputeVertices_TargetSize(0) << endl ;
    cout << "                             J:  " << ComputeVertices_TargetSize(1) << endl ;
    cout << "                             K:  " << ComputeVertices_TargetSize(2) << endl ; 
    if (ComputeVertices_TargetSize(0) <=0 || ComputeVertices_TargetSize(1) <=0 || ComputeVertices_TargetSize(2) <=0)
    {
      cout << "          -> Values must be >0 !!! Default values are set:" << endl;
      ComputeVertices_TargetSize(0) = 100;
      ComputeVertices_TargetSize(1) = 100;
      ComputeVertices_TargetSize(2) = 100;
      cout << "          -> TargetSize [mm]   I:  " << ComputeVertices_TargetSize(0) << endl ;
      cout << "                               J:  " << ComputeVertices_TargetSize(1) << endl ;
      cout << "                               K:  " << ComputeVertices_TargetSize(2) << endl ;
    }

    ComputeVertices_TargetCenter    = fSetup->GetIviGeometryParameter().TargetCenter;
    cout << "        -> TargetCenter [mm]:I:  " << ComputeVertices_TargetCenter(0) << endl ;
    cout << "                             J:  " << ComputeVertices_TargetCenter(1) << endl ;
    cout << "                             K:  " << ComputeVertices_TargetCenter(2) << endl ;
  }
  
  else if(!strcmp(ComputeVertices_TargetTypeString,"cylinder"))
  {
    ComputeVertices_TargetType = 2;
    cout << "        -> Is supported, OK" << endl ;
    
    ComputeVertices_TargetRadius    = fSetup->GetIviGeometryParameter().TargetRadius;
    cout << "        -> TargetRadius [mm]:    " << ComputeVertices_TargetRadius << endl ;
    if (ComputeVertices_TargetRadius <=0)
    {
      cout << "          -> Value must be >0 !!! Default value is set:" << endl;
      ComputeVertices_TargetRadius = 100; 
      cout << "          -> TargetRadius [mm]:    " << ComputeVertices_TargetRadius << endl ;
    }
    
    ComputeVertices_TargetLength    = fSetup->GetIviGeometryParameter().TargetLength;
    cout << "        -> TargetLength [mm]:    " << ComputeVertices_TargetLength << endl ;
    if (ComputeVertices_TargetLength <=0)
    {
      cout << "          -> Value must be >0 !!! Default value is set:" << endl;
      ComputeVertices_TargetLength = 100; 
      cout << "          -> TargetLength [mm]:    " << ComputeVertices_TargetLength << endl ;
    }

    ComputeVertices_TargetAxis      = fSetup->GetIviGeometryParameter().TargetAxis;
    cout << "        -> TargetAxis:           " << ComputeVertices_TargetAxis << endl ;
    if ( strcmp(ComputeVertices_TargetAxis,"I") && strcmp(ComputeVertices_TargetAxis,"J") && strcmp(ComputeVertices_TargetAxis,"K"))
    {
      
      cout << "          -> Value must be 'I' 'J' or 'K' !!! Default value is set:" << endl;
      ComputeVertices_TargetAxis = "I"; 
      cout << "          -> TargetAxis:           " << ComputeVertices_TargetAxis << endl ;
    }
    
    ComputeVertices_TargetCenter    = fSetup->GetIviGeometryParameter().TargetCenter;
    cout << "        -> TargetCenter [mm]:I:  " << ComputeVertices_TargetCenter(0) << endl ;
    cout << "                             J:  " << ComputeVertices_TargetCenter(1) << endl ;
    cout << "                             K:  " << ComputeVertices_TargetCenter(2) << endl ;
  }
  else
  {
    cout << "        -> !!! Is unknown !!!" << endl;
    cout << "        -> Supported target types: 'cuboid', 'cylinder' " << endl;
    ComputeVertices_TargetTypeString = "unknown";
    ComputeVertices_VertexingMethod = -1;
  }


  //////////////////////
  // Tracker
  //////////////////////
  ComputeVertices_TrackerOrigin   = fSetup->GetIviGeometryParameter().TrackerOrigin;
  cout << "      -> TrackerOrigin [mm]: I:  " << ComputeVertices_TrackerOrigin(0) << endl ;
  cout << "                             J:  " << ComputeVertices_TrackerOrigin(1) << endl ;
  cout << "                             K:  " << ComputeVertices_TrackerOrigin(2) << endl ;
    
  ComputeVertices_TrackerTilt     = fSetup->GetIviGeometryParameter().TrackerTilt;
  cout << "      -> TrackerTilt [deg]   I:  " << ComputeVertices_TrackerTilt(0) << endl ;
  cout << "                             J:  " << ComputeVertices_TrackerTilt(1) << endl ;
  cout << "                             K:  " << ComputeVertices_TrackerTilt(2) << endl ;  
  
  ComputeVertices_TrackerTilt(0)*=TMath::DegToRad();
  ComputeVertices_TrackerTilt(1)*=TMath::DegToRad();
  ComputeVertices_TrackerTilt(2)*=TMath::DegToRad();
  
  cout << "                     [rad]   I:  " << ComputeVertices_TrackerTilt(0) << endl ;
  cout << "                             J:  " << ComputeVertices_TrackerTilt(1) << endl ;
  cout << "                             K:  " << ComputeVertices_TrackerTilt(2) << endl ; 
  
  //////////////////////
  // Vertexing Method
  //////////////////////
  ComputeVertices_VertexingMethodString = fSetup->GetIviGeometryParameter().VertexingMethod;
  cout << "      -> VertexingMethod:        " << ComputeVertices_VertexingMethodString << endl ;
  if(!strcmp(ComputeVertices_VertexingMethodString,"IntersecBeamLine"))
  {
    ComputeVertices_VertexingMethod = 1;
    cout << "        -> Is supported, OK" << endl ;
  }
  else if(!strcmp(ComputeVertices_VertexingMethodString,"IntersecBeamPlaneJK"))
  {
    ComputeVertices_VertexingMethod = 2;
    cout << "        -> Is supported, OK" << endl ;
  }
  else
  {
    cout << "        -> !!! Is unknown !!!" << endl;
    cout << "        -> Supported vertexing methods: 'IntersecBeamLine', 'IntersecBeamPlaneJK' " << endl;
    cout << "        -> Use 'IntersecBeamLine' instead !!! " << endl ;
    ComputeVertices_VertexingMethod = 1;
    ComputeVertices_VertexingMethodString = "IntersecBeamLine";
  }

  cout << endl << endl ;
   
 
//////////////////////
// Build frame changer
//////////////////////
  Int_t DPrecAlignMethod = fSession->GetSetup()->GetTrackerPar().DPrecAlignMethod;   // LC 2015/01/31;
  frameChange_Expe2Tracker =  new DPrecAlign( DPrecAlignMethod );

  frameChange_Expe2Tracker->SetTranslation(ComputeVertices_TrackerOrigin(0),ComputeVertices_TrackerOrigin(1),ComputeVertices_TrackerOrigin(2));
  frameChange_Expe2Tracker->SetRotations(-ComputeVertices_TrackerTilt(2),-ComputeVertices_TrackerTilt(1),-ComputeVertices_TrackerTilt(0));// Z,Y,X !
  //frameChange_Expe2Tracker->TransformHitToTracker must be understood as TransformPointInTrackerFrame2PointInBeamFrame

  flagComputeVertices_Init=1;
  flagComputeVertices_Done=0;
  return 0;

  ///// OLD VALUES
  /*
  Double_t teta=50.;
  if (!strcmp(ComputeVertices_experimentGeometry,"HIT_2011-12-11_model01"))
  {
    ComputeVertices_TargetSize(id_i) = 100;
    ComputeVertices_TargetSize(id_j) = 100;
    ComputeVertices_TargetSize(id_k) = 250;
    
    ComputeVertices_TargetCenter(id_i)=0;// supposed to be centered
    ComputeVertices_TargetCenter(id_j)=0;// supposed to be centered
    ComputeVertices_TargetCenter(id_k)=ComputeVertices_TargetSize(id_k)/2;//k=0 is the begin of the target = target beam entry position

    // Angles FROM beam frame TO tracker frame, around (i,j,k) axis
    TiltI = 0;
    TiltJ = +TMath::Pi()-TMath::DegToRad()*teta;
    TiltK = 0;

    // Translations
    Double_t dist_Plane1_boxBext = 14; // Distance between CMOS sensor 1L (nearest the target) and external aluminum face of CMOS Box B (nearest the target)
    Double_t dist_L=100; // See handmade scheme
    Double_t dist_d=(ComputeVertices_TargetSize(id_k)/2+dist_L)/TMath::Cos(TMath::DegToRad()*teta);
    Double_t dist_Tar2Trak = dist_d + dist_Plane1_boxBext + tTracker->GetPlane(1)->GetPosition()(2)*1e-3;//distance between target origin and tracker origin
    
    // Distances FROM beam origin TO tracker origin, along (i,j,k) axis
    TransI = -dist_Tar2Trak * TMath::Sin(TMath::DegToRad()*teta);
    TransJ = +1;// Tracker is 1mm above k axis, estimated with vertices distributions in plane (O,i,j)
    TransK = ComputeVertices_TargetSize(id_k)/2 + dist_Tar2Trak * TMath::Cos(TMath::DegToRad()*teta);

  // Tracks length (only for display)
    distDisplayedTracks = dist_Tar2Trak; //length of the displayed tracks, strarting from tracker origin plane
  }
  
  if (!strcmp(ComputeVertices_experimentGeometry,"GSI_2012-08-23_model01"))
  {
    ComputeVertices_TargetSize(id_i) = 150;
    ComputeVertices_TargetSize(id_j) = 150;
    ComputeVertices_TargetSize(id_k) = 150;
    
    ComputeVertices_TargetCenter(id_i)=0;// supposed to be centered
    ComputeVertices_TargetCenter(id_j)=9;// target center above beam 9mm
    ComputeVertices_TargetCenter(id_k)=ComputeVertices_TargetSize(id_k)/2;//k=0 is the begin of the target = target beam entry position

  // Tilts = Rotations
    printf_info ("ComputeVertices_Init") ; cout << "angle is " << teta << " degrees"<< endl;
    // Angles FROM beam frame TO tracker frame, around (i,j,k) axis
    TiltI = 0;
    TiltJ = +TMath::Pi()-TMath::DegToRad()*teta;
    TiltK = 0;


  // Distances FROM beam origin TO tracker origin, along (i,j,k) axis
    Double_t distRotCenter2LastCMOS = 258;
    TransI = - distRotCenter2LastCMOS * TMath::Sin(TMath::DegToRad()*teta);
    TransJ = 9;//cmos are 9mm above beam (see shemes and pictures with laser)
    TransK = 90 + distRotCenter2LastCMOS * TMath::Cos(TMath::DegToRad()*teta);

  // Tracks length (only for display)
    distDisplayedTracks = 300;
  }
*/
}

Int_t MRax::ComputeVertices_Do(void)
{
  if(configBoolVerbose) {printf_info("ComputeVertices_Do"); cout << "starts" << endl;}
    
  if (!flagComputeVertices_Init)
  {
    if(configBoolVerbose) cout << " !! ComputeVertices not Init !! return" << endl ; 
    return -1;
  }
  
  if (flagComputeVertices_Done)
  {
    if(configBoolVerbose) cout << " !! ComputeVertices ever made !! return" << endl ;
    return -1;
  }
  
  if(configBoolVerbose) cout << "  -> Reseting ComputeVertices data ... " << endl ;
  ComputeVertices_Reset();
  
  if(configBoolVerbose)
  {
    if (ComputeVertices_VertexingMethod == 1) // IntersecBeamLine
    {
      cout << "  -> Compute vertice position with IntersecBeamLine method" << endl;
    }
    else if (ComputeVertices_VertexingMethod == 2) //IntersecBeamPlaneJK 
    {
      cout << "  -> Compute vertice position with IntersecBeamPlaneJK method" << endl;
    }
    else
    {
      cout << "  !!! Compute vertice method is unknown !!!" << endl;
    }
  }
  
  if (ComputeVertices_VertexingMethod <1 || ComputeVertices_VertexingMethod >2)
  {
    printf_error("ComputeVertices_Do");
    cout << "unknown VertexingMethod !, return" << endl ;
    return -1;
  }  
      

  Int_t skipedVertex = 0 ;
 
  for(iTrack=0; iTrack<int(tracksVector.size()); iTrack++) {
    aTrackStruct = tracksVector.at(iTrack);
    // some simple copies
    aVertexStruct.TracksNbInEvent = aTrackStruct.TracksNbInEvent;
    aVertexStruct.TrackOriginTrFr = aTrackStruct.TrackOrigin/1000.;
    aVertexStruct.TrackSlopeZTrFr = aTrackStruct.TrackSlopeZ;
    aVertexStruct.TrackChi2	  = aTrackStruct.TrackChi2;
    aVertexStruct.HitsNbInTrack   = aTrackStruct.HitsPositions.size();

    // Vertex compute in Target frame
    Track2points_TrackerFrame[0] = aVertexStruct.TrackOriginTrFr;
    Track2points_TrackerFrame[1] = Track2points_TrackerFrame[0].ComputeWithSlopeAndDistance(aVertexStruct.TrackSlopeZTrFr,1e5);//TODO check variable format limitations ?!
    Track2points_ExpermtFrame[0] = frameChange_Expe2Tracker->TransformHitToTracker(Track2points_TrackerFrame[0]);
    Track2points_ExpermtFrame[1] = frameChange_Expe2Tracker->TransformHitToTracker(Track2points_TrackerFrame[1]);


    if (ComputeVertices_VertexingMethod == 1) // IntersecBeamLine
    {
      //Looking for the intersection between Track and Beam line in Experiment frame
	
      aVertexStruct.VertexSegmentSize=fTool.BuildVertex( Beam2points_ExperimentFrame[0], Beam2points_ExperimentFrame[1], Track2points_ExpermtFrame[0], Track2points_ExpermtFrame[1], aVertexStruct.VertexPositionBeamFr);
      if (aVertexStruct.VertexSegmentSize==-1) // test if vertexing works well !
      {
          printf_error("ComputeVertices_Do"); cout << "Wrong calculation of vertex position, skip a vertex" << endl;
          skipedVertex++;
          continue;
      }
    }


    else if (ComputeVertices_VertexingMethod == 2)
    {
      //Looking for the intersection of Tracks with the JK plane which contains the beam center direction (i=??) in experimental frame

      // Vertex coordinate I
      aVertexStruct.VertexPositionBeamFr(id_i) = ComputeVertices_BeamOrigin(id_i) ; // the beam is used to define the beam frame, so beam plane is defined as (0,all,all) : DO NOT CHANGE
      
      // Vertex coordinate J
      aVertexStruct.VertexPositionBeamFr(id_j) = Track2points_ExpermtFrame[0](id_j) + \
          (ComputeVertices_BeamOrigin(id_i)   - Track2points_ExpermtFrame[0](id_i)) * \
          (Track2points_ExpermtFrame[1](id_j) - Track2points_ExpermtFrame[0](id_j)) / \
	  (Track2points_ExpermtFrame[1](id_i) - Track2points_ExpermtFrame[0](id_i)) ;
      
      // Vertex coordinate K
      aVertexStruct.VertexPositionBeamFr(id_k) = Track2points_ExpermtFrame[0](id_k) + \
          (ComputeVertices_BeamOrigin(id_i)   - Track2points_ExpermtFrame[0](id_i)) * \
          (Track2points_ExpermtFrame[1](id_k) - Track2points_ExpermtFrame[0](id_k)) / \
	  (Track2points_ExpermtFrame[1](id_i) - Track2points_ExpermtFrame[0](id_i)) ;
	  
      aVertexStruct.VertexSegmentSize = TMath::Abs(aVertexStruct.VertexPositionBeamFr(id_j) - ComputeVertices_BeamOrigin(id_j)) ; 
      // the shorter distance from the vertex point to the beam line is the vertex point j coordinate      
            
    }
    
    else
    {
      printf_error("ComputeVertices_Do"); cout << "Method unknown !" << endl;
      skipedVertex++;
      continue;
    }


    verticesVector.push_back(aVertexStruct);
  }
  
  if(configBoolVerbose)
  {
    cout << "  -> Calculation on all tracks finished:" << endl;
    cout << "    -> Tracks nb:         " << tracksVector.size() << endl;
    cout << "    -> Vertices computed: " << verticesVector.size() << endl;
    cout << "    -> Vertices skiped  : " << skipedVertex << " (" << tracksVector.size()-verticesVector.size() << ")" << endl;
  }
  
  flagComputeVertices_Done=1;
  if(configBoolVerbose) {printf_info("ComputeVertices_Do"); cout << "finished" << endl;}
  return 0;
}

Int_t MRax::ComputeVertices_Reset(void)
{
  verticesVector.clear();
  flagComputeVertices_Done=0;
  return 0;
}

Int_t MRax::ComputeVertices_Kill(void)
{
  if (!flagComputeVertices_Init) return -1;

  ComputeVertices_Reset();
  flagComputeVertices_Init=0;
  return 0;
}

Int_t MRax::ComputeVertices_SaveVertices(TString &filenameSaveVertices)
{

  if (ComputeVertices_Do()) return -1;

  //create a Tree file
  // File
  TFile treeFile(filenameSaveVertices.Data(), "recreate");

  //Tree and branches
  TTree treeVertices("treeVertices","Tree with vertices data");

  treeVertices.Branch("TracksNbInEvent"	    ,&aVertexStructExport.TracksNbInEvent     ,"TracksNbInEvent/I");
  treeVertices.Branch("TrackOriginTrFr"	    , aVertexStructExport.TrackOriginTrFr     ,"TrackOriginTrFr[3]/D");
  treeVertices.Branch("TrackSlopeZTrFr"	    , aVertexStructExport.TrackSlopeZTrFr     ,"TrackSlopeZTrFr[3]/D");
  treeVertices.Branch("TrackChi2"	    ,&aVertexStructExport.TrackChi2	      ,"TrackChi2/D");
  treeVertices.Branch("HitsNbInTrack"	    ,&aVertexStructExport.HitsNbInTrack       ,"HitsNbInTrack/I");
  treeVertices.Branch("VertexPositionBeamFr", aVertexStructExport.VertexPositionBeamFr,"VertexPositionBeamFr[3]/D");
  treeVertices.Branch("VertexSegmentSize"   ,&aVertexStructExport.VertexSegmentSize   ,"VertexSegmentSize/D");

  for (iTrack=0; iTrack<int(verticesVector.size()); iTrack++) {
    aVertexStruct=verticesVector.at(iTrack);

    aVertexStructExport.TracksNbInEvent	     = aVertexStruct.TracksNbInEvent ;
    aVertexStructExport.TrackOriginTrFr[0]   = aVertexStruct.TrackOriginTrFr(0)/1000;
    aVertexStructExport.TrackOriginTrFr[1]   = aVertexStruct.TrackOriginTrFr(1)/1000;
    aVertexStructExport.TrackOriginTrFr[2]   = aVertexStruct.TrackOriginTrFr(2)/1000;
    aVertexStructExport.TrackSlopeZTrFr[0]   = aVertexStruct.TrackSlopeZTrFr(0)/1000;
    aVertexStructExport.TrackSlopeZTrFr[1]   = aVertexStruct.TrackSlopeZTrFr(1)/1000;
    aVertexStructExport.TrackSlopeZTrFr[2]   = aVertexStruct.TrackSlopeZTrFr(2)/1000;
    aVertexStructExport.TrackChi2	     = aVertexStruct.TrackChi2;
    aVertexStructExport.HitsNbInTrack        = aVertexStruct.HitsNbInTrack;
    aVertexStructExport.VertexPositionBeamFr[0]= aVertexStruct.VertexPositionBeamFr(0)/1000;
    aVertexStructExport.VertexPositionBeamFr[1]= aVertexStruct.VertexPositionBeamFr(1)/1000;
    aVertexStructExport.VertexPositionBeamFr[2]= aVertexStruct.VertexPositionBeamFr(2)/1000;
    aVertexStructExport.VertexSegmentSize    = aVertexStruct.VertexSegmentSize/1000;

    treeVertices.Fill();
  }

  if (!treeFile.Write())
  {
    printf_error("ComputeVertices_SaveVertices"); printf("ERROR while saving Vertices in TTree file : %s \n",filenameSaveVertices.Data());
    return -1;
  }

  printf_info("ComputeVertices_SaveVertices"); printf("Vertices saved in TTree file : %s \n",filenameSaveVertices.Data());
  return 0;
}

  //****************************************************************
  //		Misc
  //****************************************************************
void MRax::ComputeOverlap(void)
{
  // Description :
  //

// Arguments :


// Configuration - Arguments :
  //
  Int_t  PlanesAssossiation[4][2]={{1,5},{2,6},{3,7},{4,8}}; // {1-Left 1-Right} ; {2-Left 2-Right} ; ...

// Declarations
  //DTracker *tTracker = fSession->GetTracker();
  DPlane *tPlaneL,*tPlaneR;
  Float_t XPlaneL,XPlaneR,XPlaneSize,XOverLapum;
  Int_t XOverLappix;
  for (Int_t i=1;i<=4;i++)
  {

    tPlaneL = tTracker->GetPlane(PlanesAssossiation[i-1][0]);
    tPlaneR = tTracker->GetPlane(PlanesAssossiation[i-1][1]);

    XPlaneL=tPlaneL->GetPosition()(0);
    XPlaneR=tPlaneR->GetPosition()(0);
    XPlaneSize=tPlaneL->GetStripsNv()*tPlaneL->GetStripPitch()(0);

    XOverLapum = XPlaneSize+XPlaneL-XPlaneR;
    XOverLappix = floor(XOverLapum/tPlaneL->GetStripPitch()(0));

    cout << "Planes " << tPlaneL->GetPlanePurpose() << " and " << tPlaneR->GetPlanePurpose() << " are overlaped of " << XOverLapum << " um ~ " << XOverLappix << " pixels" << endl;
  }

}


//______________________________________________________________________________________________________
//
//	PRINT Statistics & Numbers
//______________________________________________________________________________________________________

  //****************************************************************
  //		Print Cumulated Statistics Along events
  //****************************************************************
  //
  // Print Statistics along events : hits, tracks, triggers, ...
  //****************************************************************
void MRax::PrintCumulatedStatistics_Reset(void)
{
/*struct structStatsCumulatedAlongEvents
  {
     Int_t		eventsAnalysed;
     Int_t		triggersTotal;
     Int_t		triggersEventsWith;
     Int_t		triggersEventsWithout;
     Int_t		tracksTotal;
     Int_t		tracksEventsWith;
     Int_t		tracksEventsWithout;
    Int_t		hitsTotalAllPlanes;
    Int_t 		hitsTotalByPlane[8];
  };
  structStatsCumulatedAlongEvents astructStatsCumulatedAlongEvents ;
*/
  astructStatsCumulatedAlongEvents.eventsAnalysed=0;
  astructStatsCumulatedAlongEvents.triggersTotal=0;
  astructStatsCumulatedAlongEvents.triggersEventsWith=0;
  astructStatsCumulatedAlongEvents.triggersEventsWithout=0;
  astructStatsCumulatedAlongEvents.tracksTotal=0;
  astructStatsCumulatedAlongEvents.tracksEventsWith=0;
  astructStatsCumulatedAlongEvents.tracksEventsWithout=0;
  astructStatsCumulatedAlongEvents.hitsTotalAllPlanes=0;
  for (iPlane=1 ; iPlane<=nbPlanes; iPlane++) astructStatsCumulatedAlongEvents.hitsTotalByPlane[iPlane-1]=0;

}

void MRax::PrintCumulatedStatistics_Fill(void)
{
  astructStatsCumulatedAlongEvents.eventsAnalysed +=1;

  if (fSession->GetDataAcquisition()->GetTriggers()->at(0)!=0) // si les triggers ont un sens
  {
    nbTriggers = fSession->GetDataAcquisition()->GetTriggers()->size();
    astructStatsCumulatedAlongEvents.triggersTotal += nbTriggers; // que ce soit 0 ou +
    if (nbTriggers >  0) astructStatsCumulatedAlongEvents.triggersEventsWith    += 1;
    if (nbTriggers == 0) astructStatsCumulatedAlongEvents.triggersEventsWithout +=1;
  }
  else astructStatsCumulatedAlongEvents.triggersEventsWithout +=1 ;

  nbTracks=tTracker->GetTracksN();
  astructStatsCumulatedAlongEvents.tracksTotal += nbTracks;
  if (nbTracks) astructStatsCumulatedAlongEvents.tracksEventsWith    +=1;
  else          astructStatsCumulatedAlongEvents.tracksEventsWithout +=1;

  for(iPlane=1; iPlane<=nbPlanes; iPlane++)
  {
    nbHits = tTracker->GetPlane(iPlane)->GetHitsN();
    astructStatsCumulatedAlongEvents.hitsTotalByPlane[iPlane-1] += nbHits;
    astructStatsCumulatedAlongEvents.hitsTotalAllPlanes += nbHits;
  }
}

void MRax::PrintCumulatedStatistics_Print(void)
{
/*struct structStatsCumulatedAlongEvents
  {
     Int_t		eventsAnalysed;
     Int_t		triggersTotal;
     Int_t		triggersEventsWith;
     Int_t		triggersEventsWithout;
     Int_t		tracksTotal;
     Int_t		tracksEventsWith;
     Int_t		tracksEventsWithout;
    Int_t		hitsTotalAllPlanes;
    Int_t 		hitsTotalByPlane[8];
  };
  structStatsCumulatedAlongEvents astructStatsCumulatedAlongEvents ;
*/
  printf_info("PrintPrintCumulatedStatistics");
  cout << astructStatsCumulatedAlongEvents.eventsAnalysed                   << " events were analysed : " << endl;
  cout << "  " << astructStatsCumulatedAlongEvents.triggersTotal            << " total triggers :"                 << endl;
  cout << "    " << astructStatsCumulatedAlongEvents.triggersEventsWith     << " events with at least a trigger" << endl;
  cout << "    " << astructStatsCumulatedAlongEvents.triggersEventsWithout  << " events without any trigger"     << endl;
  cout << "  " << astructStatsCumulatedAlongEvents.tracksTotal              << " total tracks :"                 << endl;
  cout << "    " << astructStatsCumulatedAlongEvents.tracksEventsWith       << " events with at least a track" << endl;
  cout << "    " << astructStatsCumulatedAlongEvents.tracksEventsWithout    << " events without any track"     << endl;
  cout << "  " << astructStatsCumulatedAlongEvents.hitsTotalAllPlanes	    << " total hits : "		       << endl;
  for (iPlane = 1 ; iPlane <= nbPlanes ; iPlane++)
  {
    cout << "    " << astructStatsCumulatedAlongEvents.hitsTotalByPlane[iPlane-1];
    cout << " total hits on plane " << tTracker->GetPlane(iPlane)->GetPlanePurpose() << endl;
  }
}

  //****************************************************************
  //		Print Event Informations
  //****************************************************************

void MRax::PrintEventInfos(void)
{
  cout << "******************************************************" << endl;
  printf(" event %d infos",GetCurrentEventNumber());

  cout << "  **** Planes Informations **** " << endl;
  for(  jPlane=1; jPlane<=nbPlanes; jPlane++ )
  {
    tPlane = tTracker->GetPlane(jPlane);
    aList = tPlane->GetListOfPixels();

    printf("    ** Plane %s (#%d) : \n",tPlane->GetPlanePurpose(),jPlane);
    printf("      * %d fired pixels \n",int(aList->size()));
    printf("      * %d hits \n",tPlane->GetHitsN());
  }

  cout << endl;
  cout << "  **** Tracks Informations **** " << endl;
  tTracker->PrintStatistics();


  cout << "******************************************************" << endl;
  cout << "******************************************************" << endl;
  fSession->GetDataAcquisition()->PrintStatistics();


}

void MRax::PrintPXIeStats(void)
{
  fSession->GetDataAcquisition()->PrintStatistics();
}

void MRax::PrintCurrentEventNumber(void)
{
  printf_info("Event manager");printf("Current event number is %d\n",GetCurrentEventNumber());
  
  return;
  
}

Int_t MRax::GetCurrentEventNumber(void)
{
  return fSession->GetCurrentEventNumber();
}

Int_t MRax::PrintLogFile(void)
{
  TDatime current;
  // Output file name
  aFileName = fSession->GetResultsDirParent();
  aFileName += "/";
  aFileName += "Analyses.log";
  
  fTool.LocalizeDirName(&aFileName);
  
  FILE *AnalyseLogFile = fopen(aFileName.Data(),"a");
  if (!AnalyseLogFile) 
  {
    printf_error("PrintEventsNumberInFile");  cout << "Can not create file: "<< aFileName << ", quit !" << endl;
    return -1;
  }
  
  fprintf(AnalyseLogFile , "RUN\t%d\tAnalysedOn\t%u/%u/%u_%u:%u:%u\tEventsEnd\t\%d\n",RunNumber,current.GetDay(),current.GetMonth(),current.GetYear(),current.GetHour(),current.GetMinute(),current.GetSecond(),GetCurrentEventNumber());
  
  fclose(AnalyseLogFile);
  
  return fSession->GetCurrentEventNumber();
}


//______________________________________________________________________________________________________
//
//	      Events navigation and processing
//______________________________________________________________________________________________________
//
  //****************************************************************
  //	Events navigation (without processing !)
  //****************************************************************
Int_t MRax::GoToNextEvent(Bool_t updateTracker)
{
  if (fSession->GoToNextEvent() < 0) return -1;
  if(updateTracker) tTracker->Update();
  return GetCurrentEventNumber();
}

Int_t MRax::GoToAnEvent(Int_t EventNbToGo, Bool_t updateTracker)
{
  if (fSession->GoToEvent(EventNbToGo) <0 ) return -1;
  if(updateTracker) tTracker->Update();
  return GetCurrentEventNumber();
}

Int_t MRax::GoToNextEventWithHits(Int_t hitsNbMinimumToStop)
{
  // Init
  Int_t eventNbBegin = GetCurrentEventNumber();
  
  // Process events
  Int_t hits;
  while (1)
  {
    hits=0;
    if (fSession->GoToNextEvent() < 0)
    {
      printf_error("GoToNextEventWithHits");
      printf("fSession->GoToNextEvent() return an error : end reached ? Events scanning stoped, actual event : %d\n",GetCurrentEventNumber());
      return -1;
    }
    tTracker->Update();
    
    for(  iPlane=1; iPlane<=nbPlanes; iPlane++) hits += tTracker->GetPlane(iPlane)->GetHitsN() ;
    if (hits >= hitsNbMinimumToStop) break;
  }

  Int_t eventNbFind = GetCurrentEventNumber();
  if (configBoolVerbose)
  {
    printf_info("GoToNextEventWithHits") ;
    printf("skip %d events from # %d to # %d to find an event with at least %d hit(s) : %d hit(s) \n",eventNbFind-eventNbBegin,eventNbBegin,eventNbFind,hitsNbMinimumToStop,hits);
  }
  return 0;
}

Int_t MRax::GoToNextEventWithTracks(Int_t tracksNbMinimumToStop)
{
  // Init
  Int_t eventNbBegin = GetCurrentEventNumber();
  
  // Process events
  while (1)
  {
    if (fSession->GoToNextEvent() < 0)
    {
      printf_error("GoToNextEventWithTracks");
      printf("fSession->GoToNextEvent() return an error : end reached ? Events scanning stoped, actual event : %d\n",GetCurrentEventNumber());
      return -1;
    }
    tTracker->Update();
    if (tTracker->GetTracksN()>=tracksNbMinimumToStop) break;
  }

  Int_t eventNbFind = GetCurrentEventNumber();
  if (configBoolVerbose)
  {
    printf_info("GoToNextEventWithTracks") ;
    printf("skip %d events from # %d to # %d to find an event with at least %d track(s) : %d track(s) \n",eventNbFind-eventNbBegin,eventNbBegin,eventNbFind,tracksNbMinimumToStop,tTracker->GetTracksN());
  }

  return 0;
}

Int_t MRax::SkipEvents(Int_t nbEventsToSkip, Bool_t updateTracker)
{
  // Simply skip the required number of events
  // skip = read them but don't analyse them, else at the end

  if (nbEventsToSkip<0)
  {
    printf_error("SkipEvents");printf("Enter a positive value !\n");
    return -1;
  }
  for( Int_t iEvent=0; iEvent<nbEventsToSkip; iEvent++ )
  {
    if (fSession->GoToNextEvent() < 0 )
    {
      printf_error("SkipEvents"); printf("Can't go to the next event : end reached ? \n");
      return -1;
    }
  }
  if(updateTracker) tTracker->Update();
  return GetCurrentEventNumber();
}

//****************************************************************
  //	Events processing 
  //****************************************************************
Int_t MRax::ProcessEvents(Int_t aNbOfEvents, Bool_t resetBefore, Bool_t showDisplays)
{
  if (aNbOfEvents < 0) return -1;
  
  
  if(resetBefore) ResetAllDisplays();
  if (aNbOfEvents == 0)
  {
    //tTracker->Update(); Supposed done before in the case aNbOfEvents == 0, if do again, all clusters cleared!
    FillAllDisplays();
  }
  for (Int_t iEvent=0; iEvent<aNbOfEvents ; iEvent++)
  {
    if (fSession->GoToNextEvent() < 0)
    {
      printf_error("ProcessEvents");
      printf("fSession->GoToNextEvent() return an error : end reached ? Events scanning stoped, actual event : %d\n",GetCurrentEventNumber());
      break;
    }
    tTracker->Update();
    FillAllDisplays();
  }
  if(showDisplays) ShowAllDisplays();
  return 0;
}

Int_t MRax::ProcessAllEvents(Int_t refreshDisplaysEveryXevents, Bool_t resetBefore)
{
  return ProcessEvents((Int_t)(pow(2.,31))-1, resetBefore, refreshDisplaysEveryXevents);
}

  //****************************************************************
  //	Global Actions
  //****************************************************************
Int_t MRax::FillAllDisplays(void)
{
    
  //Filled if needed : if initialised

  DisplayPixelsInCmosFrameInPixel_Fill();
  DisplayPixelsInFirstBoardFrameInPixel_Fill();
  DisplayHitsInCmosFrameInPixel_Fill();
  DisplayHitsInFirstBoardFrameInPixel_Fill();
  DisplayHitsInTrackerFrameInUm_Fill();
  DisplayHitsInTrackerFrameInUmOverlayed_Fill();

  //DisplayTracksIn3D_Fill(); DOES NOT EXISTS
  //DisplayTracksIn2D_Fill(); DOES NOT EXISTS
  StoreTracks_Fill(kTRUE);//Done if StoreTracks initialised

  //DisplayVerticesProjs_Fill();DO NO EXISTS
  //DisplayVerticesIn3D_Fill();DO NO EXISTS
  //ComputeVertices_Do is made if needed by concerned functions

  DisplayCmosOverFlow_Fill();
  DisplayClustersSize_Fill();
  DisplayHitsByEvent_Fill();
  DisplayHitsTracksAssociation_Fill();
  DisplayTracksAndTriggersProperties_Fill();
  DisplayChronogram_Fill();

  return 0;
}

Int_t MRax::ShowAllDisplays(void)
{
  // Showed is initialised, with default options !

  DisplayPixelsInCmosFrameInPixel_Show();
  DisplayPixelsInFirstBoardFrameInPixel_Show();
  DisplayHitsInCmosFrameInPixel_Show();
  DisplayHitsInFirstBoardFrameInPixel_Show();
  DisplayHitsInTrackerFrameInUm_Show();
  DisplayHitsInTrackerFrameInUmOverlayed_Show();

  DisplayTracksIn3D_Show();
  DisplayTracksIn2D_Show();
  DisplayVerticesProjs_Show();
  DisplayVerticesIn3D_Show();

  DisplayCmosOverFlow_Show();
  DisplayClustersSize_Show();
  DisplayHitsByEvent_Show();
  DisplayHitsTracksAssociation_Show();
  DisplayTracksAndTriggersProperties_Show();
  DisplayChronogram_Show();
  
  DisplayNoiseStudy_Show();

  return 0;
}

Int_t MRax::SaveAllDisplays(const Char_t *fileExtension1, const Char_t *fileExtension2, const Char_t *fileExtension3, const Char_t *fileExtension4, const Char_t *fileExtension5, const Char_t *fileExtension6)
{
  // Save initialised canvas, with given file extensions

  // If no extensions given, quit
  if (fileExtension1 == NULL) return -1;

  // Create a strings-vector with file extensions
 std::vector<TString> aVectorStringFilesExtensions;
  aVectorStringFilesExtensions.push_back(fileExtension1);
  if (fileExtension2 != NULL) aVectorStringFilesExtensions.push_back(fileExtension2);
  if (fileExtension3 != NULL) aVectorStringFilesExtensions.push_back(fileExtension3);
  if (fileExtension4 != NULL) aVectorStringFilesExtensions.push_back(fileExtension4);
  if (fileExtension5 != NULL) aVectorStringFilesExtensions.push_back(fileExtension5);
  if (fileExtension6 != NULL) aVectorStringFilesExtensions.push_back(fileExtension6);

  // call SaveAllDisplays(const vector<TString> aVectorStringFilesExtensions)
  SaveAllDisplays(aVectorStringFilesExtensions);

  // Clear the vector
  aVectorStringFilesExtensions.clear();

  return 0;
}

Int_t MRax::SaveAllDisplays(vector<TString> &aVectorStringFilesExtensions)
{
  // Save initialised canvas, with given file extensions

  // If nothing to do (=empty extensions vector), quit
  if (! aVectorStringFilesExtensions.size()) return -1;

  // Call the Display*_Save() functions;
  DisplayPixelsInCmosFrameInPixel_Save(aVectorStringFilesExtensions);
  DisplayPixelsInFirstBoardFrameInPixel_Save(aVectorStringFilesExtensions);
  DisplayHitsInCmosFrameInPixel_Save(aVectorStringFilesExtensions);
  DisplayHitsInFirstBoardFrameInPixel_Save(aVectorStringFilesExtensions);
  DisplayHitsInTrackerFrameInUm_Save(aVectorStringFilesExtensions);
  DisplayHitsInTrackerFrameInUmOverlayed_Save(aVectorStringFilesExtensions);

  DisplayTracksIn3D_Save(aVectorStringFilesExtensions);
  DisplayTracksIn2D_Save(aVectorStringFilesExtensions);
  DisplayVerticesProjs_Save(aVectorStringFilesExtensions);
  DisplayVerticesIn3D_Save(aVectorStringFilesExtensions);

  DisplayCmosOverFlow_Save(aVectorStringFilesExtensions);
  DisplayClustersSize_Save(aVectorStringFilesExtensions);
  DisplayHitsByEvent_Save(aVectorStringFilesExtensions);
  DisplayHitsTracksAssociation_Save(aVectorStringFilesExtensions);
  DisplayTracksAndTriggersProperties_Save(aVectorStringFilesExtensions);
  DisplayChronogram_Save(aVectorStringFilesExtensions);
  
  DisplayNoiseStudy_Save(aVectorStringFilesExtensions);

  return 0;
}

Int_t MRax::SaveAllHistograms(void)
{

  // Call the Display*_SaveHistos() functions;
   DisplayPixelsInCmosFrameInPixel_SaveHistos(); 
//   DisplayPixelsInFirstBoardFrameInPixel_SaveHistos();
//   DisplayHitsInCmosFrameInPixel_SaveHistos();
//   DisplayHitsInFirstBoardFrameInPixel_SaveHistos();
//   DisplayHitsInTrackerFrameInUm_SaveHistos();
//   DisplayHitsInTrackerFrameInUmOverlayed_SaveHistos();
// 
//   DisplayTracksIn3D_SaveHistos();
//   DisplayTracksIn2D_SaveHistos();
//   DisplayVerticesProjs_SaveHistos();
//   DisplayVerticesIn3D_SaveHistos();
// 
//   DisplayCmosOverFlow_SaveHistos();
   DisplayClustersSize_SaveHistos();
//   DisplayHitsByEvent_SaveHistos();
   DisplayHitsTracksAssociation_SaveHistos();
   DisplayTracksAndTriggersProperties_SaveHistos();
//   DisplayChronogram_SaveHistos();
//   
   DisplayNoiseStudy_SaveHistos();

  return 0;
}

Int_t MRax::SaveResults(void)
{
  treeTracksFileFullPath  = resultsDirPath;
  treeTracksFileFullPath += "/";
  treeTracksFileFullPath += FileNamePrefix;
  treeTracksFileFullPath += FileNameTreeTracks;
  treeTracksFileFullPath += FileNameSuffix;
  treeTracksFileFullPath += ".root";
  
  StoreTracks_SaveTracks(treeTracksFileFullPath);
  

  treeVertexFileFullPath = resultsDirPath;
  treeVertexFileFullPath += "/";
  treeVertexFileFullPath += FileNamePrefix;
  treeVertexFileFullPath += FileNameTreeVertex;
  treeVertexFileFullPath += FileNameSuffix;
  treeVertexFileFullPath += ".root";
  
  ComputeVertices_SaveVertices(treeVertexFileFullPath);
  
  return 0;
}

Int_t MRax::ResetAllDisplays(void)
{
  
  // Reseted if initialised

  DisplayPixelsInCmosFrameInPixel_Reset();
  DisplayPixelsInFirstBoardFrameInPixel_Reset();
  DisplayHitsInCmosFrameInPixel_Reset();
  DisplayHitsInFirstBoardFrameInPixel_Reset();
  DisplayHitsInTrackerFrameInUm_Reset();
  DisplayHitsInTrackerFrameInUmOverlayed_Reset();

  DisplayTracksIn3D_Reset();
  DisplayTracksIn2D_Reset();
  DisplayVerticesProjs_Reset();
  DisplayVerticesIn3D_Reset();

  StoreTracks_Reset();
  ComputeVertices_Reset();

  DisplayCmosOverFlow_Reset();
  DisplayClustersSize_Reset();
  DisplayHitsByEvent_Reset();
  DisplayHitsTracksAssociation_Reset();
  DisplayTracksAndTriggersProperties_Reset();
  DisplayChronogram_Reset();
  
  DisplayNoiseStudy_Reset();

  return 0;
}

Int_t MRax::KillAllDisplays(void)
{  
  //Killed if open/initialised
  DisplayPixelsInCmosFrameInPixel_Kill();
  DisplayPixelsInFirstBoardFrameInPixel_Kill();
  DisplayHitsInCmosFrameInPixel_Kill();
  DisplayHitsInFirstBoardFrameInPixel_Kill();
  DisplayHitsInTrackerFrameInUm_Kill();
  DisplayHitsInTrackerFrameInUmOverlayed_Kill();

  DisplayTracksIn3D_Kill();
  DisplayTracksIn2D_Kill();
  DisplayVerticesProjs_Kill();
  DisplayVerticesIn3D_Kill();

  StoreTracks_Kill();
  ComputeVertices_Kill();

  DisplayCmosOverFlow_Kill();
  DisplayClustersSize_Kill();
  DisplayHitsByEvent_Kill();
  DisplayHitsTracksAssociation_Kill();
  DisplayTracksAndTriggersProperties_Kill();
  DisplayChronogram_Kill();
  
  DisplayNoiseStudy_Kill();

  return 0;
}



//______________________________________________________________________________________________________
//
//	      GUI
//______________________________________________________________________________________________________
//
  //****************************************************************
  //		GUI fuctions <-> button
  //****************************************************************
  // Analysis control tab
  //	Analysis control group frame
  //		Process Events zone
  //----------------------------------------------------------------
Int_t MRax::GuiButtonProcess1event(void)
{
  GuiProcessEvents(1,fNumberEntry_RefreshFreq->GetNumberEntry()->GetIntNumber());
  return 0;
}

Int_t MRax::GuiButtonProcess10events(void)
{
  GuiProcessEvents(10,fNumberEntry_RefreshFreq->GetNumberEntry()->GetIntNumber());
  return 0;
}

Int_t MRax::GuiButtonProcess100events(void)
{
  GuiProcessEvents(100,fNumberEntry_RefreshFreq->GetNumberEntry()->GetIntNumber());
  return 0;
}

Int_t MRax::GuiButtonProcessXevents(void)
{
  GuiProcessEvents(fNumberEntry_EventsToProcess->GetNumberEntry()->GetIntNumber(),fNumberEntry_RefreshFreq->GetNumberEntry()->GetIntNumber());
  return 0;
}

Int_t MRax::GuiButtonProcessAllEvents(void)
{
  GuiProcessEvents(pow(2.,31)-1,fNumberEntry_RefreshFreq->GetNumberEntry()->GetIntNumber());
  return 0;
}

Int_t MRax::GuiButtonRefreshDisplays(void)
{
  GuiInitDisplays();
  GuiShowDisplays();
  GuiActualizeTextsDislays();
  return 0;
}

  //----------------------------------------------------------------
  //  Go to event with ... zone 
  //----------------------------------------------------------------
Int_t MRax::GuiButtonGoToEventWithNtracks(void)
{
  // Init
  GuiInitDisplays();
  
  // Reset, if no cumulate
  GuiResetDisplays();

  // Process events
  GoToNextEventWithTracks(fNumberEntryGoToEventWithNtracks->GetNumberEntry()->GetIntNumber());
  
  FillAllDisplays();
  GuiShowDisplays();
  GuiActualizeEventNumber();

  return 0;
}

Int_t MRax::GuiButtonGoToEventWithNhits(void)
{
  // Init
  GuiInitDisplays();
  
  // Reset, if no cumulate
  GuiResetDisplays();

  // Process events
  GoToNextEventWithHits(fNumberEntryGoToEventWithNhits->GetNumberEntry()->GetIntNumber());
  
  FillAllDisplays();
  GuiShowDisplays();
  GuiActualizeEventNumber();

  return 0;
}

Int_t MRax::GuiButtonGoToEventNb(void)
{
  // Init
  GuiInitDisplays();
  
  // Reset, if no cumulate
  GuiResetDisplays();
  
  // Process events
  if (GoToAnEvent(Int_t(fNumberEntry_GoToEvent->GetNumberEntry()->GetIntNumber()),1) < 0)
  {
    printf_error("GuiButtonGoToEventNb");
    printf("Event asked can't be reached, nothing is done\n");
    GuiActualizeEventNumber();
    return -1;
  }
  
  FillAllDisplays();

  GuiActualizeEventNumber();
  GuiShowDisplays(); 
  
  return 0;
}
  //----------------------------------------------------------------
  //		Actions zone
  //****************************************************************

Int_t MRax::GuiButtonToggleVerbosity(void)
{
  configBoolVerbose    = !configBoolVerbose;
  Char_t text[100];
  sprintf(text,"%s",(configBoolVerbose) ? "Turn verbose OFF":"Turn verbose ON");
  fTextButtonTurnVerbose->SetText(text);
  printf_info("GuiButtonToggleVerbosity") ; printf("verbose mode is %s\n", configBoolVerbose ? "ON":"OFF");
  
  return 0;
  
}

Int_t MRax::GuiButtonSetTafDebugLevel(void)
{
  Char_t tafcommand[100];
  sprintf(tafcommand,"gTAF->SetDebug(%d);",int(fNumberEntryTafDebugLevel->GetNumberEntry()->GetIntNumber()));
  if(configBoolVerbose) 
  {
    printf_info("GuiButtonSetTafDebugLevel");
    printf(" Process command : %s \n",tafcommand);
  }
  return gROOT->ProcessLineSync(tafcommand);
}

Int_t MRax::GuiButtonResetDisplays(void)
{
  // Reseted 

  DisplayPixelsInCmosFrameInPixel_Reset();
  DisplayPixelsInFirstBoardFrameInPixel_Reset();
  DisplayHitsInCmosFrameInPixel_Reset();
  DisplayHitsInFirstBoardFrameInPixel_Reset();
  DisplayHitsInTrackerFrameInUm_Reset();
  DisplayHitsInTrackerFrameInUmOverlayed_Reset();

  DisplayTracksIn3D_Reset();
  DisplayTracksIn2D_Reset();
  StoreTracks_Reset();
  
  DisplayVerticesProjs_Reset();
  DisplayVerticesIn3D_Reset();
  ComputeVertices_Reset();
  
  DisplayCmosOverFlow_Reset();
  DisplayClustersSize_Reset();
  DisplayHitsByEvent_Reset();
  DisplayHitsTracksAssociation_Reset();
  DisplayTracksAndTriggersProperties_Reset();
  DisplayChronogram_Reset();
  
  DisplayNoiseStudy_Reset();
  
  
  
  
  GuiButtonRefreshDisplays();

  return 0;
}

Int_t MRax::GuiButtonSaveDisplays(void)
{
  fTextButtonSaveDisplays->SetText("Wait ...");
    
  FileNamePrefix = fTextEntry_FilesPrefix->GetText();
  FileNameSuffix = fTextEntry_FilesSuffix->GetText();
  
 std::vector<TString> aVectorStringFilesExtensions;

  if (fTextButtonSaveCanvas_inPDF ->GetState()) aVectorStringFilesExtensions.push_back("pdf");
  if (fTextButtonSaveCanvas_inPNG->GetState()) aVectorStringFilesExtensions.push_back("png");
  if (fTextButtonSaveCanvas_inROOT->GetState()) aVectorStringFilesExtensions.push_back("root");

  SaveAllDisplays(aVectorStringFilesExtensions);

  aVectorStringFilesExtensions.clear();
   
  fTextButtonSaveDisplays->SetText("Save Displays");

  return 0;
}

Int_t MRax::GuiButtonSaveHistograms(void)
{
  fTextButtonSaveHistograms->SetText("Wait ...");
    
  FileNamePrefix = fTextEntry_FilesPrefix->GetText();
  FileNameSuffix = fTextEntry_FilesSuffix->GetText();
  
  SaveAllHistograms();
   
  fTextButtonSaveHistograms->SetText("Save Histograms");

  return 0;
}

Int_t MRax::GuiButtonSaveResults(void)
{
  if (fTextButtonSaveTracks->GetState())
  {
    treeTracksFileFullPath  = resultsDirPath;
    treeTracksFileFullPath += "/";
    treeTracksFileFullPath += FileNamePrefix;
    treeTracksFileFullPath += FileNameTreeTracks;
    treeTracksFileFullPath += FileNameSuffix;
    treeTracksFileFullPath += ".root";
    
    StoreTracks_SaveTracks(treeTracksFileFullPath);
  }

  if (fTextButtonSaveVertice->GetState())
  {
    treeVertexFileFullPath = resultsDirPath;
    treeVertexFileFullPath += "/";
    treeVertexFileFullPath += FileNamePrefix;
    treeVertexFileFullPath += FileNameTreeVertex;
    treeVertexFileFullPath += FileNameSuffix;
    treeVertexFileFullPath += ".root";
    
    ComputeVertices_SaveVertices(treeVertexFileFullPath);
  }
  return 0;
}

Int_t MRax::GuiButtonQuitTaf(void)
{
  printf_info("GuiButtonQuitTaf") ; printf("Bye Bye ! \n\n");
  gApplication->Terminate();
  return 0;
}


  //****************************************************************
  //		GUI SubFunctions
  //****************************************************************
Int_t MRax::GuiProcessEvents(Int_t nbEventsToProcess, Int_t refreshDisplaysEveryXevents)
{
  if (nbEventsToProcess<0) return -1;
  
  // Init
  GuiInitDisplays();
  // Reset, if no cumulate
  GuiResetDisplays();
  // Process events
  //fSession->SetEvents(nbEventsToProcess);
  for (Int_t iEvent=0; iEvent<nbEventsToProcess; iEvent++)
  {
    if (fSession->GoToNextEvent() <0 )
    {
      printf_error("GuiProcessEvents");
      printf("fSession->GoToNextEvent() return an error : end reached ? Events scanning stoped, actual event : %d\n",GetCurrentEventNumber());
      break;
    }
    tTracker->Update();
    FillAllDisplays();
    if(refreshDisplaysEveryXevents>0 && iEvent>0) 
    {
      if(!(iEvent%refreshDisplaysEveryXevents))
      {
        GuiShowDisplays();
        GuiActualizeEventNumber();
      }
    }
  }
  if(refreshDisplaysEveryXevents>=0) GuiShowDisplays();
  GuiActualizeEventNumber();
  GuiActualizeTextsDislays();
  
  if (fTextButtonBeepWhenFinished->GetState()) 
  {
    if(gSystem->Exec("beep -l 350 -f 392 -D 100 --new -l 350 -f 392 -D 100 --new -l 350 -f 392 -D 100 --new -l 250 -f 311.1 -D 100 --new -l 25 -f 466.2 -D 100 --new -l 350 -f 392 -D 100 --new -l 250 -f 311.1 -D 100 --new -l 25 -f 466.2 -D 100 --new -l 700 -f 392 -D 100"))
    {
      printf_error("ProcessEvents");
      printf("'beep' program is not found on this computer! See http://www.johnath.com/beep/ to install\n");
    }
  }

  return 0;
}

Int_t MRax::GuiInitDisplays(void)
{
// Displays
  // Pixels
  if(fTextButtonDisRawDatCmoNatGeoInPix->GetState() || fTextButtonNoiseDisRawDatCmoNatGeoInPix->GetState() || fTextButtonDisNoiseStudy->GetState())   DisplayPixelsInCmosFrameInPixel_Init();
  else DisplayPixelsInCmosFrameInPixel_Kill();

  if(fTextButtonDisRawDatTrackerFraInPix->GetState())   DisplayPixelsInFirstBoardFrameInPixel_Init();
  else DisplayPixelsInFirstBoardFrameInPixel_Kill();

  // Hits Pixels
  if(fTextButtonDisHitsCmoNatGeoInPix->GetState())      DisplayHitsInCmosFrameInPixel_Init();
  else DisplayHitsInCmosFrameInPixel_Kill();

  if(fTextButtonDisHitsInTrackerFrameInPix->GetState()) DisplayHitsInFirstBoardFrameInPixel_Init();
  else DisplayHitsInFirstBoardFrameInPixel_Kill();

  // Hits Um
  if(fTextButtonDisHitsInTrackerFrameInUm->GetState())  DisplayHitsInTrackerFrameInUm_Init();
  else DisplayHitsInTrackerFrameInUm_Kill();
  
  // Hits OV
  if(fTextButtonDisHitsInTrackerFrameInUmOv->GetState() || fTextButtonAlignViewOverHits->GetState()) DisplayHitsInTrackerFrameInUmOverlayed_Init(fTextButtonAlignViewOverHits->GetState());
  else DisplayHitsInTrackerFrameInUmOverlayed_Kill();
  
  // Tracks 3D
  if(fTextButtonDisTracks3D->GetState())		DisplayTracksIn3D_Init();
  else DisplayTracksIn3D_Kill();
  
  // Tracks 2D
   if(fTextButtonDisTracks2D->GetState())		DisplayTracksIn2D_Init();
   else DisplayTracksIn2D_Kill();

  // Vertices projections
  if(fTextButtonDisVerticesProjs->GetState())                DisplayVerticesProjs_Init();
  else DisplayVerticesProjs_Kill();
  
  // Vertices 3D
  if(fTextButtonDisVertices3D->GetState()) 		DisplayVerticesIn3D_Init();
  else DisplayVerticesIn3D_Kill();

  // Statistics
  if(fTextButtonDisCMOSoverflowLines->GetState())       DisplayCmosOverFlow_Init();
  else DisplayCmosOverFlow_Kill();
  
  if(fTextButtonDisHitsSize->GetState())		DisplayClustersSize_Init(fNumberEntry_ClustersBinMax->GetNumberEntry()->GetIntNumber());
  else DisplayClustersSize_Kill();

  if(fTextButtonDisHitsByEvent->GetState())		DisplayHitsByEvent_Init(fNumberEntry_HitsByEventBinMax->GetNumberEntry()->GetIntNumber());
  else DisplayHitsByEvent_Kill();

  if(fTextButtonDisHitsAssociation->GetState()) 	DisplayHitsTracksAssociation_Init(fNumberEntry_ClustersBinMax->GetNumberEntry()->GetIntNumber());
  else DisplayHitsTracksAssociation_Kill();

  if(fTextButtonDisStats->GetState()) 			DisplayTracksAndTriggersProperties_Init(fTextButtonDisStats_withTriggers->GetState(), \
    fNumberEntry_TracksHistoChi2Max->GetNumberEntry()->GetIntNumber(),\
    fNumberEntry_TracksHistoSlopeMax->GetNumberEntry()->GetIntNumber());
  else DisplayTracksAndTriggersProperties_Kill();

  if(fTextButtonDisStatsAlongEvents->GetState())	DisplayChronogram_Init(fTextButtonDisStatsAlongEvents_withTriggers->GetState());
  else DisplayChronogram_Kill();

// Store tracks / computeVertices IN THIS ORDER ! AND AT THE END !
  if(fTextButtonSaveTracks->GetState())			StoreTracks_Init();
  else if((!fTextButtonDisTracks3D->GetState())&&(!fTextButtonDisVertices3D->GetState()))	StoreTracks_Kill();

  if(fTextButtonSaveVertice->GetState())		ComputeVertices_Init();
  else if(!fTextButtonDisVertices3D->GetState()) 		ComputeVertices_Kill();
  
// Noise Study
  if(fTextButtonDisNoiseStudy->GetState())
  {
    //DisplayPixelsInCmosFrameInPixel_Init(); Made above
    DisplayNoiseStudy_Init();
  }
  else DisplayNoiseStudy_Kill();
  
  return 0;

  return 0;
}

void MRax::GuiShowDisplays(void)
{
  
  // Pixels
  //DisplayPixelsInCmosFrameInPixel_Show(Bool_t WithHits, Bool_t WithOverlapLine,Bool_t WithZbar, Int_t minPixOccu)
    DisplayPixelsInCmosFrameInPixel_Show( \
	      (fTextButtonDisRawDatCmoNatGeoInPix      ->GetState() * fTextButtonDisRawDatCmoNatGeoInPix_Hit        ->GetState() +
	       fTextButtonNoiseDisRawDatCmoNatGeoInPix ->GetState() * fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit   ->GetState()),
	      (fTextButtonDisRawDatCmoNatGeoInPix      ->GetState() * fTextButtonDisRawDatCmoNatGeoInPix_OlLine     ->GetState()),
	      (fTextButtonDisRawDatCmoNatGeoInPix      ->GetState() * fTextButtonDisRawDatCmoNatGeoInPix_ColBar     ->GetState() +
	       fTextButtonNoiseDisRawDatCmoNatGeoInPix ->GetState() * fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar->GetState()),
	      (fTextButtonDisRawDatCmoNatGeoInPix      ->GetState() * fNumberEntry_RawMinPixOccu->GetNumberEntry()->GetIntNumber() +
	       fTextButtonNoiseDisRawDatCmoNatGeoInPix ->GetState() * fNumberEntry_NoiseRawMinPixOccu->GetNumberEntry()->GetIntNumber()));
  //DisplayPixelsInFirstBoardFrameInPixel_Show(Bool_t WithHits, Bool_t WithOverlapLine,Bool_t WithZbar, Int_t minPixOccu)
    DisplayPixelsInFirstBoardFrameInPixel_Show( \
	      fTextButtonDisRawDatTrackerFraInPix_Hit    ->GetState(), \
	      fTextButtonDisRawDatTrackerFraInPix_OlLine ->GetState(), \
	      fTextButtonDisRawDatTrackerFraInPix_ColBar ->GetState(), \
	      fNumberEntry_RawMinPixOccu->GetNumberEntry()->GetIntNumber());

  // Hits Pixels
  //DisplayHitsInCmosFrameInPixel_Show(Bool_t ShowAsso2TrackHits, Bool_t WithOverlapLine,Bool_t HitShowedAsPix, Int_t minHitOccu)
    DisplayHitsInCmosFrameInPixel_Show( \
	      fTextButtonDisHitsCmoNatGeoInPix_AssoTrack 	->GetState(), \
	      fTextButtonDisHitsCmoNatGeoInPix_OlLine 		->GetState(), \
	      fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix 	->GetState(), \
	      fNumberEntry_HitMinOccu->GetNumberEntry()		->GetIntNumber());

  //DisplayHitsInFirstBoardFrameInPixel_Show(Bool_t ShowAsso2TrackHits, Bool_t WithOverlapLine,Bool_t HitShowedAsPix, Int_t minHitOccu)
    DisplayHitsInFirstBoardFrameInPixel_Show( \
	      fTextButtonDisHitsInTrackerFrameInPix_AssoTrack	->GetState(), \
	      fTextButtonDisHitsInTrackerFrameInPix_OlLine 		->GetState(), \
	      fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix 	->GetState(), \
	      fNumberEntry_HitMinOccu->GetNumberEntry()		->GetIntNumber());

  // Hits Um
  Int_t param = 0 * fGRadioButton_HitTrackInter_none->GetState() + \
	        1 * fGRadioButton_HitTrackInter_cross->GetState() ;
  //DisplayHitsInTrackerFrameInUm_Show(Int_t TrackDisplayMode)
    DisplayHitsInTrackerFrameInUm_Show( \
	      param);

 // Overlay view
  for (iPlane=1;iPlane<=nbPlanes;iPlane++) \
	  flagsDisplayHitsInTrackerFrameInUmOverlayed_plan2disp[iPlane-1] = (fListBoxOverlayHitsInTrackerFrameInUm->GetSelection(iPlane) || fListBoxAlignPlanesToDisp->GetSelection(iPlane));
  //DisplayHitsInTrackerFrameInUmOverlayed_Show(Bool_t *planes2display)
    DisplayHitsInTrackerFrameInUmOverlayed_Show(\
	      flagsDisplayHitsInTrackerFrameInUmOverlayed_plan2disp);


  // Tracks 3D
  //DisplayTracksIn3D_Show(Bool_t DrawTracks, Bool_t DrawSensors, Bool_t DrawTrackedHits, Bool_t DrawAllHits)
    DisplayTracksIn3D_Show( \
	    fTextButtonDisTracks3D_DrawTracks->GetState(), \
	    fTextButtonDisTracks3D_DrawSensor->GetState(), \
	    fTextButtonDisTracks3D_DrawTrackedHits->GetState(), \
	    fTextButtonDisTracks3D_DrawAllHits->GetState());
    
  // Tracks 2D
//   Bool_t DrawTracks, Bool_t DrawSensors, Bool_t DrawTrackedHits, Bool_t DrawAllHits)
       DisplayTracksIn2D_Show( \
	    fTextButtonDisTracks2D_DrawTracks->GetState(), \
	    fTextButtonDisTracks2D_DrawSensor->GetState(), \
	    fTextButtonDisTracks2D_DrawTrackedHits->GetState(), \
	    fTextButtonDisTracks2D_DrawAllHits->GetState());    

  // Vertices projections
  //DisplayVerticesProjs_Show(Bool_t DrawTarget)
  DisplayVerticesProjs_Show(\
            fTextButtonDisVerticesProjs_DrawTarget                   ->GetState());       
       
  // Vertices 3D
  //DisplayVerticesIn3D_Show(Bool_t DrawSensors, Bool_t DrawTracks, Bool_t DrawTrackerDir, Bool_t DrawTarget, Bool_t DrawBeam, Bool_t DrawVerticesPoints, Bool_t DrawVerticesPointsSmall, Bool_t DrawAcceptance)
  DisplayVerticesIn3D_Show( \
	    fTextButtonDisVertices3D_DrawSensor			->GetState(), \
	    fTextButtonDisVertices3D_DrawTracks			->GetState(), \
	    fTextButtonDisVertices3D_DrawTrackDir		->GetState(), \
	    fTextButtonDisVertices3D_DrawTarget			->GetState(), \
	    fTextButtonDisVertices3D_DrawBeam			->GetState(), \
	    fTextButtonDisVertices3D_DrawVerticesPoints 	->GetState(), \
	    fTextButtonDisVertices3D_DrawVerticesPointsSmall	->GetState(), \
	    fTextButtonDisVertices3D_DrawAcceptance		->GetState());

  // Statistics
  DisplayCmosOverFlow_Show();
  //DisplayClustersSize_Show(Bool_t All,Bool_t Tracked, Bool_t Untracked)
  DisplayClustersSize_Show( \
	    fTextButtonDisHitsSizeAll		->GetState(), \
	    fTextButtonDisHitsSizeTracked	->GetState(), \
	    fTextButtonDisHitsSizeUnTracked	->GetState());

  //DisplayHitsByEvent_Show()
  DisplayHitsByEvent_Show();

  //DisplayHitsTracksAssociation_Show()
  DisplayHitsTracksAssociation_Show();

  //DisplayTracksAndTriggersProperties_Show()
  DisplayTracksAndTriggersProperties_Show();

  //DisplayChronogram_Show(void)
  DisplayChronogram_Show();
    
  // Noise Study
  if(fTextButtonDisNoiseStudy->GetState() && flagDisplayPixelsInCmosFrameInPixel_Init==1)
  {
    DisplayNoiseStudy_Show(fNumberEntry_NoiseThresholdsMin->GetNumber(),fNumberEntry_NoiseThresholdsMax->GetNumber(), fNumberEntry_NoiseThresholdsNb->GetNumberEntry()->GetIntNumber());
  }
  
  return;
  
}

Int_t MRax::GuiResetDisplays(void)
{
  
  // Reseted if initialised

  if(!fTextButtonDisRawDatCmoNatGeoInPix_Cumul->GetState())     DisplayPixelsInCmosFrameInPixel_Reset();
  if(!fTextButtonDisRawDatTrackerFraInPix_Cumul->GetState())    DisplayPixelsInFirstBoardFrameInPixel_Reset();
  if(!fTextButtonDisHitsCmoNatGeoInPix_Cumul->GetState())       DisplayHitsInCmosFrameInPixel_Reset();
  if(!fTextButtonDisHitsInTrackerFrameInPix_Cumul->GetState())  DisplayHitsInFirstBoardFrameInPixel_Reset();
  if(!fTextButtonDisHitsInTrackerFrameInUm_Cumul->GetState())   DisplayHitsInTrackerFrameInUm_Reset();
  if(!(fTextButtonDisHitsInTrackerFrameInUmOv_Cumul->GetState() || fTextButtonAlignViewOverHits_Cumul->GetState())) DisplayHitsInTrackerFrameInUmOverlayed_Reset();

  if(!fTextButtonDisTracks_Cumul->GetState())
  {
    DisplayTracksIn3D_Reset();
    DisplayTracksIn2D_Reset();
    StoreTracks_Reset();
  }
  
  if(!fTextButtonDisVertices_Cumul->GetState())
  {
    DisplayVerticesProjs_Reset();
    DisplayVerticesIn3D_Reset();
    ComputeVertices_Reset();
  }

  if(!fTextButtonDisCMOSoverflowLines_Cumul->GetState()) DisplayCmosOverFlow_Reset();
  if(!fTextButtonDisHitsSize_Cumul->GetState())          DisplayClustersSize_Reset();
  if(!fTextButtonDisHitsByEvent_Cumul->GetState())       DisplayHitsByEvent_Reset();
  if(!fTextButtonDisHitsAssociation_Cumul->GetState())   DisplayHitsTracksAssociation_Reset();
  if(!fTextButtonDisStats_Cumul->GetState())             DisplayTracksAndTriggersProperties_Reset();
  if(!fTextButtonDisStatsAlongEvents_Cumul->GetState())  DisplayChronogram_Reset();
  
  DisplayNoiseStudy_Reset();

  return 0;
}

Int_t MRax::GuiReProcessLastEvent(void)
{
  //tTracker->Update(); DO NOT DO THAT, hits will disapear !
  GuiResetDisplays();
  FillAllDisplays();
  GuiShowDisplays();
  GuiActualizeEventNumber();
  return 0;
}

Int_t MRax::GuiActualizeEventNumber(void)
{
  fNumberEntry_GoToEvent->SetIntNumber(GetCurrentEventNumber());
  
  return 0;
  
}

Int_t MRax::GuiActualizeTextsDislays(void)
{
  sprintf(name,"%d",eventsProcessedDisplayPixelsInCmosFrameInPixel);
  fLabelNoiseNbOfEvents->SetText(name);

  Double_t relaOccu = fNumberEntry_NoiseRawMinPixOccu->GetNumber() / eventsProcessedDisplayPixelsInCmosFrameInPixel;
  if (relaOccu <= 1) sprintf(name,"%.3e",relaOccu);
  else sprintf(name,"!! %.3e > 1 !!",relaOccu);
  fLabelNoiseMinPixOccuRelValue->SetText(name);

  return 0;
  
  return 0;
}
  //****************************************************************
  // 		Allignment tab 
  //****************************************************************
Int_t MRax::GuiButtonAlignResetValues(void)
{

  fNumberEntry_AlignTranslatX->SetNumber(0);
  fNumberEntry_AlignTranslatY->SetNumber(0);
  fNumberEntry_AlignTranslatZ->SetNumber(0);


  fNumberEntry_AlignTurnX->SetNumber(0);
  fNumberEntry_AlignTurnY->SetNumber(0);
  fNumberEntry_AlignTurnZ->SetNumber(0);

  return 0;
}

Int_t MRax::GuiButtonAlignDoTranslateTurn(void)
{
  //Bool_t *flagsAlignPlanesToMove = new Bool_t[nbPlanes];

  DR3 tran_um (fNumberEntry_AlignTranslatX->GetNumber()*1000 ,fNumberEntry_AlignTranslatY->GetNumber()*1000,fNumberEntry_AlignTranslatZ->GetNumber()*1000);
  DR3 turn_rad(TMath::DegToRad()*fNumberEntry_AlignTurnZ->GetNumber(),TMath::DegToRad()*fNumberEntry_AlignTurnY->GetNumber(),TMath::DegToRad()*fNumberEntry_AlignTurnX->GetNumber());

  for (iPlane=1;iPlane<=nbPlanes;iPlane++)
  {
    if (fListBoxAlignPlanesToMove->GetSelection(iPlane))
    {
      tTracker->GetPlane(iPlane)->ChangePositionByOffset(tran_um);
      tTracker->GetPlane(iPlane)->ChangeTiltByOffset(turn_rad);
    }
  }
  GuiReProcessLastEvent();
  //GuiButtonRefreshDisplays();

  return 0;
}

Int_t MRax::GuiButtonAlignDisplayPosTilt(void)
{
  Int_t  nbPlanesToMove=0, plane=0;
  for (iPlane=1;iPlane<=nbPlanes;iPlane++) if (fListBoxAlignPlanesToMove->GetSelection(iPlane)) {nbPlanesToMove+=1;plane=iPlane;};

  if (nbPlanesToMove == 1)
  {
    DR3 actualPosition = tTracker->GetPlane(plane)->GetPosition();
    fNumberEntry_AlignPosX->SetNumber(actualPosition(0)/1000);
    fNumberEntry_AlignPosY->SetNumber(actualPosition(1)/1000);
    fNumberEntry_AlignPosZ->SetNumber(actualPosition(2)/1000);

    DR3 actualTilt = tTracker->GetPlane(plane)->GetTilt();
    fNumberEntry_AlignTiltX->SetNumber(actualTilt(2)*TMath::RadToDeg());
    fNumberEntry_AlignTiltY->SetNumber(actualTilt(1)*TMath::RadToDeg());
    fNumberEntry_AlignTiltZ->SetNumber(actualTilt(0)*TMath::RadToDeg());

    return 0;
  }
  else
  {
    fNumberEntry_AlignPosX->SetNumber(0);
    fNumberEntry_AlignPosY->SetNumber(0);
    fNumberEntry_AlignPosZ->SetNumber(0);

    fNumberEntry_AlignTiltX->SetNumber(0);
    fNumberEntry_AlignTiltY->SetNumber(0);
    fNumberEntry_AlignTiltZ->SetNumber(0);

    printf_error("GuiButtonAlignDisplayPosTilt");
    printf("Only 1 plane must be selected ! Can't display actual positions tilts \n");
    return 0;
  }
}

Int_t MRax::GuiButtonAlignUpdatePosTilt(void)
{
  Int_t  nbPlanesToMove=0, plane=0;
  for (iPlane=1;iPlane<=nbPlanes;iPlane++) if (fListBoxAlignPlanesToMove->GetSelection(iPlane)) {nbPlanesToMove+=1;plane=iPlane;};

  if (nbPlanesToMove == 1)
  {
    DR3 pos_um (fNumberEntry_AlignPosX->GetNumber()*1000 ,fNumberEntry_AlignPosY->GetNumber()*1000,fNumberEntry_AlignPosZ->GetNumber()*1000);
    DR3 tilt_rad(TMath::DegToRad()*fNumberEntry_AlignTiltZ->GetNumber(),TMath::DegToRad()*fNumberEntry_AlignTiltY->GetNumber(),TMath::DegToRad()*fNumberEntry_AlignTiltX->GetNumber());
    tTracker->GetPlane(plane)->ChangePositionByValue(pos_um);
    tTracker->GetPlane(plane)->ChangeTiltByValue(tilt_rad);

    GuiReProcessLastEvent();
    //GuiButtonRefreshDisplays();
    GuiButtonAlignDisplayPosTilt();
    return 0;
  }
  else
  {
    printf_error("GuiButtonAlignUpdatePosTilt");
    printf("More than 1 plane selected ! Can't update positions tilts \n");
    return -1;
  }

}

Int_t MRax::GuiButtonAlignUpdateCfgFile(void)
{
  
  if(configBoolVerbose) printf_info("GuiButtonAlignUpdateCfgFile"); cout << endl;
     
  //Int_t exec_result;
  TString shell_command;
  TString cfg_file_actual ="";
  TString cfg_file_backup ="";
  TString cfg_file_new    ="";
  TString cfg_file_2write ="";
  TDatime currentDateTime;

  cfg_file_actual  = fSession->GetConfigPath();
  cfg_file_actual += "/";
  cfg_file_actual += fSession->GetConfigFileName();
  
  if(fGRadioButton_AnalyseModeUpWithBck->GetState())
  {
    cfg_file_backup  = cfg_file_actual;
    cfg_file_backup.Remove(cfg_file_backup.Sizeof()-5,4); //Remove extension
    cfg_file_backup += "_BackupBefManuAlignOn";
    cfg_file_backup += currentDateTime.GetDate();
    cfg_file_backup += "-";
    cfg_file_backup += currentDateTime.GetHour();
    cfg_file_backup += "-";
    cfg_file_backup += currentDateTime.GetMinute();
    cfg_file_backup += "-";
    cfg_file_backup += currentDateTime.GetSecond();
    cfg_file_backup += ".cfg";    
    
    shell_command  = "cp ";
    shell_command += cfg_file_actual;
    shell_command += " ";
    shell_command += cfg_file_backup;
 
    if(configBoolVerbose)
    {
      cout << "cfg_file_actual: "  << cfg_file_actual << endl << endl;
      cout << "cfg_file_backup: "  << cfg_file_backup << endl << endl;
      cout << "cp shell command: " << shell_command << endl << endl;
    }
    
    if (gSystem->Exec(shell_command))
    {
      printf_error("GuiButtonAlignUpdateCfgFile");
      printf("Config file \n%s \nwas NOT successfully backuped in \n%s \nShell command was \n%s \nNo changes written !\n",cfg_file_actual.Data(), cfg_file_backup.Data(), shell_command.Data());
      return -1;
    }
    
    cfg_file_2write = cfg_file_actual;
  }
    
  else if(fGRadioButton_AnalyseModeNewFile->GetState())
  {
    TString GivenFile   = fTextEntry_NewCfgFileName->GetText();
    TString ActualNoExt = fSession->GetConfigFileName();
    ActualNoExt.Remove(ActualNoExt.Sizeof()-5,4); //Remove extension
    
    if (GivenFile.Index(ActualNoExt)==0 && GivenFile.Sizeof() == ActualNoExt.Sizeof()) //same name
    {
      GivenFile += "_autoRenamed";
      printf_warning("GuiButtonAlignUpdateCfgFile");printf("You give a cfg file name identic to the actual! %s is used\n",GivenFile.Data());
      fTextEntry_NewCfgFileName->SetText(GivenFile.Data());
    }
      
    cfg_file_new  = fSession->GetConfigPath();
    cfg_file_new += "/";
    cfg_file_new += GivenFile;
    cfg_file_new += ".cfg";    
    
    shell_command  = "cp ";
    shell_command += cfg_file_actual;
    shell_command += " ";
    shell_command += cfg_file_new;
 
    if(configBoolVerbose)
    {
      cout << "cfg_file_actual: "  << cfg_file_actual << endl << endl;
      cout << "cfg_file_new: "  << cfg_file_new << endl << endl;
      cout << "cp shell command: " << shell_command << endl << endl;
    }
    
    if (gSystem->Exec(shell_command))
    {
      printf_error("GuiButtonAlignUpdateCfgFile");
      printf("Config file \n%s \nwas NOT successfully copied in \n%s \nShell command was \n%s \nNo changes written !\n",cfg_file_actual.Data(), cfg_file_new.Data(), shell_command.Data());
      return -1;
    }
    
    cfg_file_2write = cfg_file_new;
  }
  
  else if(fGRadioButton_AnalyseModeUpNoBck->GetState())
  {
    cfg_file_2write = cfg_file_actual;
  }
  
  else
  {
    printf_error("GuiButtonAlignUpdateCfgFile");
    printf("Unkown update mode, quit");
    return -1;
  }
  

// Update cfg file
  Char_t shell_command2[5000];
  sprintf(shell_command2,"Update_conf_align2D %s",cfg_file_2write.Data());
  for(iPlane=1 ; iPlane<=nbPlanes ; iPlane++)
  {
    sprintf( shell_command2, "%s %d %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f",\
	shell_command2, \
	iPlane, \
	tTracker->GetPlane(iPlane)->GetPosition()(0)/1000., \
	tTracker->GetPlane(iPlane)->GetPosition()(1)/1000., \
	tTracker->GetPlane(iPlane)->GetPosition()(2)/1000., \
	tTracker->GetPlane(iPlane)->GetTilt()(0)*TMath::RadToDeg(), \
	tTracker->GetPlane(iPlane)->GetTilt()(1)*TMath::RadToDeg(), \
	tTracker->GetPlane(iPlane)->GetTilt()(2)*TMath::RadToDeg() \
	   );
  }
  
  if(configBoolVerbose) 
  {
    sprintf(shell_command2,"%s -d", shell_command2);
    cout << "update shell command: "  << shell_command2 << endl;
  }
  
  if(gSystem->Exec(shell_command2))
  {
    printf_error("GuiButtonAlignUpdateCfgFile");
    printf("Config file \n%s \nwas NOT successfully updated ! \nShell command was: %s",cfg_file_2write.Data(), shell_command2);
    return -1;
  }
  printf_info("GuiButtonAlignUpdateCfgFile");
  printf("\nConfig file \n%s \nwas successfully updated ! \n",cfg_file_2write.Data());


  return 0;
}

  //****************************************************************
  //            Noise tab 
  //****************************************************************
Int_t MRax::GuiButtonGenerateVetoPixelsCode(void)
{
  GenerateVetoPixelsCode(fNumberEntry_NoiseRawMinPixOccu->GetIntNumber());
  
  return 0;
  
}

  //****************************************************************
  //            GUI window : Lauch, Display, Connect
  //****************************************************************
Int_t MRax::GuiLauch(void)
{
  GuiCreate();
  GuiConnectButtons();
  GuiActualizeEventNumber();
  GuiInitParameters();
  return 0;
}

Int_t MRax::GuiCreate(void)
{

  // main frame
  TGMainFrame *fMainFrame = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
  fMainFrame->SetName("fMainFrame");
  fMainFrame->SetLayoutBroken(kTRUE);
//#############################################################################################################
//#############################################################################################################
//                                      TOP TAB
//#############################################################################################################
//#############################################################################################################
  ///////////////////
  Int_t length_X = 1005;
  TGTab *fTabTop = new TGTab(fMainFrame,length_X,464);
  Int_t sizeYfTabTop=0;

//*********************************************************************************************************  
//                           container of Tab "Analysis"
//*********************************************************************************************************
    TGCompositeFrame *fCompositeFrameTabAnalysis;
    fCompositeFrameTabAnalysis = fTabTop->AddTab("Display events");
    fCompositeFrameTabAnalysis->SetLayoutManager(new TGVerticalLayout(fCompositeFrameTabAnalysis));
    Int_t sizeYfCompositeFrameTabAnalysis=0;

// "Views selection" group frame
      TGGroupFrame *fGroupFrameViewsSelector = new TGGroupFrame(fCompositeFrameTabAnalysis,"Views selection");
      fGroupFrameViewsSelector->SetLayoutBroken(kTRUE);
      Int_t sizeYfGroupFrameViewsSelector=0;

  // "Pixels" zone
        Int_t pos_VS_RD_X=5;
        Int_t pos_VS_RD_Y=18;
        Int_t pos_VS_deltaY=20;

        TGLabel *fLabelZoneRawdata = new TGLabel(fGroupFrameViewsSelector,"Pixels [pixels]");
        fLabelZoneRawdata->SetTextJustify(36);
        fLabelZoneRawdata->SetMargins(0,0,0,0);
        fLabelZoneRawdata->SetWrapLength(-1);
        fGroupFrameViewsSelector->AddFrame(fLabelZoneRawdata, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneRawdata->MoveResize(pos_VS_RD_X,pos_VS_RD_Y,150,18);

          //DisplayPixelsInCmosFrameInPixel
          fTextButtonDisRawDatCmoNatGeoInPix = new TGCheckButton(fGroupFrameViewsSelector,"CMOS frame");
          fTextButtonDisRawDatCmoNatGeoInPix->SetTextJustify(36);
          fTextButtonDisRawDatCmoNatGeoInPix->SetMargins(0,0,0,0);
          fTextButtonDisRawDatCmoNatGeoInPix->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatCmoNatGeoInPix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisRawDatCmoNatGeoInPix->MoveResize(pos_VS_RD_X,pos_VS_RD_Y+=1.*pos_VS_deltaY,150,19); 
          
            fTextButtonDisRawDatCmoNatGeoInPix_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisRawDatCmoNatGeoInPix_Cumul->SetTextJustify(36);
            fTextButtonDisRawDatCmoNatGeoInPix_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisRawDatCmoNatGeoInPix_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatCmoNatGeoInPix_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatCmoNatGeoInPix_Cumul->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,60,19);
          
            fTextButtonDisRawDatCmoNatGeoInPix_Hit = new TGCheckButton(fGroupFrameViewsSelector,"with hits");
            fTextButtonDisRawDatCmoNatGeoInPix_Hit->SetTextJustify(36);
            fTextButtonDisRawDatCmoNatGeoInPix_Hit->SetMargins(0,0,0,0);
            fTextButtonDisRawDatCmoNatGeoInPix_Hit->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatCmoNatGeoInPix_Hit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatCmoNatGeoInPix_Hit->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisRawDatCmoNatGeoInPix_OlLine = new TGCheckButton(fGroupFrameViewsSelector,"with overlap line");
            fTextButtonDisRawDatCmoNatGeoInPix_OlLine->SetTextJustify(36);
            fTextButtonDisRawDatCmoNatGeoInPix_OlLine->SetMargins(0,0,0,0);
            fTextButtonDisRawDatCmoNatGeoInPix_OlLine->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatCmoNatGeoInPix_OlLine, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatCmoNatGeoInPix_OlLine->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisRawDatCmoNatGeoInPix_ColBar = new TGCheckButton(fGroupFrameViewsSelector,"with color bar");
            fTextButtonDisRawDatCmoNatGeoInPix_ColBar->SetTextJustify(36);
            fTextButtonDisRawDatCmoNatGeoInPix_ColBar->SetMargins(0,0,0,0);
            fTextButtonDisRawDatCmoNatGeoInPix_ColBar->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatCmoNatGeoInPix_ColBar, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatCmoNatGeoInPix_ColBar->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,116,19);
            

          //DisplayPixelsInFirstBoardFrameInPixel
          fTextButtonDisRawDatTrackerFraInPix = new TGCheckButton(fGroupFrameViewsSelector,"FIRST board frame");
          fTextButtonDisRawDatTrackerFraInPix->SetTextJustify(36);
          fTextButtonDisRawDatTrackerFraInPix->SetMargins(0,0,0,0);
          fTextButtonDisRawDatTrackerFraInPix->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatTrackerFraInPix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisRawDatTrackerFraInPix->MoveResize(pos_VS_RD_X,pos_VS_RD_Y+=1.5*pos_VS_deltaY,150,19);
          
            fTextButtonDisRawDatTrackerFraInPix_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisRawDatTrackerFraInPix_Cumul->SetTextJustify(36);
            fTextButtonDisRawDatTrackerFraInPix_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisRawDatTrackerFraInPix_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatTrackerFraInPix_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatTrackerFraInPix_Cumul->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,60,19);

            fTextButtonDisRawDatTrackerFraInPix_Hit = new TGCheckButton(fGroupFrameViewsSelector,"with hits");
            fTextButtonDisRawDatTrackerFraInPix_Hit->SetTextJustify(36);
            fTextButtonDisRawDatTrackerFraInPix_Hit->SetMargins(0,0,0,0);
            fTextButtonDisRawDatTrackerFraInPix_Hit->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatTrackerFraInPix_Hit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatTrackerFraInPix_Hit->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisRawDatTrackerFraInPix_OlLine = new TGCheckButton(fGroupFrameViewsSelector,"with overlap line");
            fTextButtonDisRawDatTrackerFraInPix_OlLine->SetTextJustify(36);
            fTextButtonDisRawDatTrackerFraInPix_OlLine->SetMargins(0,0,0,0);
            fTextButtonDisRawDatTrackerFraInPix_OlLine->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatTrackerFraInPix_OlLine, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatTrackerFraInPix_OlLine->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisRawDatTrackerFraInPix_ColBar = new TGCheckButton(fGroupFrameViewsSelector,"with color bar");
            fTextButtonDisRawDatTrackerFraInPix_ColBar->SetTextJustify(36);
            fTextButtonDisRawDatTrackerFraInPix_ColBar->SetMargins(0,0,0,0);
            fTextButtonDisRawDatTrackerFraInPix_ColBar->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisRawDatTrackerFraInPix_ColBar, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisRawDatTrackerFraInPix_ColBar->MoveResize(pos_VS_RD_X+20,pos_VS_RD_Y+=pos_VS_deltaY,116,19);

            TGLabel *fLabelMinPixOccu = new TGLabel(fGroupFrameViewsSelector,"min pix occur.");
            fLabelMinPixOccu->SetTextJustify(36);
            fLabelMinPixOccu->SetMargins(0,0,0,0);
            fLabelMinPixOccu->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fLabelMinPixOccu, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelMinPixOccu->MoveResize(pos_VS_RD_X,pos_VS_RD_Y+=1.5*pos_VS_deltaY,85,18);

            fNumberEntry_RawMinPixOccu = new TGNumberEntry(fGroupFrameViewsSelector,0, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 10000);
            fGroupFrameViewsSelector->AddFrame(fNumberEntry_RawMinPixOccu, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_RawMinPixOccu->MoveResize(pos_VS_RD_X+85,pos_VS_RD_Y,65,25);

            pos_VS_RD_Y+=25;

        sizeYfGroupFrameViewsSelector = (sizeYfGroupFrameViewsSelector < pos_VS_RD_Y) ? pos_VS_RD_Y : sizeYfGroupFrameViewsSelector;

  // "Hits in pixels" zone
        Int_t pos_VS_HP_X=165;
        Int_t pos_VS_HP_Y=18;


        // Hits pixels label
        TGLabel *fLabelZoneHits = new TGLabel(fGroupFrameViewsSelector,"Hits [pixels]");
        fLabelZoneHits->SetTextJustify(36);
        fLabelZoneHits->SetMargins(0,0,0,0);
        fLabelZoneHits->SetWrapLength(-1);
        fGroupFrameViewsSelector->AddFrame(fLabelZoneHits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneHits->MoveResize(pos_VS_HP_X,pos_VS_HP_Y,150,18);

          //DisplayHitsInCmosFrameInPixel
          fTextButtonDisHitsCmoNatGeoInPix = new TGCheckButton(fGroupFrameViewsSelector,"CMOS frame");
          fTextButtonDisHitsCmoNatGeoInPix->SetTextJustify(36);
          fTextButtonDisHitsCmoNatGeoInPix->SetMargins(0,0,0,0);
          fTextButtonDisHitsCmoNatGeoInPix->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsCmoNatGeoInPix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisHitsCmoNatGeoInPix->MoveResize(pos_VS_HP_X,pos_VS_HP_Y+=1.*pos_VS_deltaY,150,19);
          
            fTextButtonDisHitsCmoNatGeoInPix_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisHitsCmoNatGeoInPix_Cumul->SetTextJustify(36);
            fTextButtonDisHitsCmoNatGeoInPix_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisHitsCmoNatGeoInPix_Cumul->SetWrapLength(-1);
            fTextButtonDisHitsCmoNatGeoInPix_Cumul->SetForegroundColor(5);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsCmoNatGeoInPix_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsCmoNatGeoInPix_Cumul->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,60,19);

            fTextButtonDisHitsCmoNatGeoInPix_AssoTrack = new TGCheckButton(fGroupFrameViewsSelector,"track assoc.");
            fTextButtonDisHitsCmoNatGeoInPix_AssoTrack->SetTextJustify(36);
            fTextButtonDisHitsCmoNatGeoInPix_AssoTrack->SetMargins(0,0,0,0);
            fTextButtonDisHitsCmoNatGeoInPix_AssoTrack->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsCmoNatGeoInPix_AssoTrack, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsCmoNatGeoInPix_AssoTrack->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisHitsCmoNatGeoInPix_OlLine = new TGCheckButton(fGroupFrameViewsSelector,"with overlap line");
            fTextButtonDisHitsCmoNatGeoInPix_OlLine->SetTextJustify(36);
            fTextButtonDisHitsCmoNatGeoInPix_OlLine->SetMargins(0,0,0,0);
            fTextButtonDisHitsCmoNatGeoInPix_OlLine->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsCmoNatGeoInPix_OlLine, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsCmoNatGeoInPix_OlLine->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix = new TGCheckButton(fGroupFrameViewsSelector,"group in pixels");
            fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix->SetTextJustify(36);
            fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix->SetMargins(0,0,0,0);
            fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,116,19);

          //DisplayHitsInTrackerFrameInPix
          fTextButtonDisHitsInTrackerFrameInPix = new TGCheckButton(fGroupFrameViewsSelector,"FIRST board frame");
          fTextButtonDisHitsInTrackerFrameInPix->SetTextJustify(36);
          fTextButtonDisHitsInTrackerFrameInPix->SetMargins(0,0,0,0);
          fTextButtonDisHitsInTrackerFrameInPix->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInPix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisHitsInTrackerFrameInPix->MoveResize(pos_VS_HP_X,pos_VS_HP_Y+=1.5*pos_VS_deltaY,150,19);
          
            fTextButtonDisHitsInTrackerFrameInPix_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisHitsInTrackerFrameInPix_Cumul->SetTextJustify(36);
            fTextButtonDisHitsInTrackerFrameInPix_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisHitsInTrackerFrameInPix_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInPix_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsInTrackerFrameInPix_Cumul->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,60,19);          

            fTextButtonDisHitsInTrackerFrameInPix_AssoTrack = new TGCheckButton(fGroupFrameViewsSelector,"track assoc.");
            fTextButtonDisHitsInTrackerFrameInPix_AssoTrack->SetTextJustify(36);
            fTextButtonDisHitsInTrackerFrameInPix_AssoTrack->SetMargins(0,0,0,0);
            fTextButtonDisHitsInTrackerFrameInPix_AssoTrack->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInPix_AssoTrack, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsInTrackerFrameInPix_AssoTrack->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisHitsInTrackerFrameInPix_OlLine = new TGCheckButton(fGroupFrameViewsSelector,"with overlap line");
            fTextButtonDisHitsInTrackerFrameInPix_OlLine->SetTextJustify(36);
            fTextButtonDisHitsInTrackerFrameInPix_OlLine->SetMargins(0,0,0,0);
            fTextButtonDisHitsInTrackerFrameInPix_OlLine->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInPix_OlLine, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsInTrackerFrameInPix_OlLine->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,116,19);

            fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix = new TGCheckButton(fGroupFrameViewsSelector,"group in pixels");
            fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix->SetTextJustify(36);
            fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix->SetMargins(0,0,0,0);
            fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix->MoveResize(pos_VS_HP_X+20,pos_VS_HP_Y+=pos_VS_deltaY,116,19);

            TGLabel *fLabelMinHitOccu = new TGLabel(fGroupFrameViewsSelector,"min hits occur.");
            fLabelMinHitOccu->SetTextJustify(36);
            fLabelMinHitOccu->SetMargins(0,0,0,0);
            fLabelMinHitOccu->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fLabelMinHitOccu, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelMinHitOccu->MoveResize(pos_VS_HP_X,pos_VS_HP_Y+=1.5*pos_VS_deltaY,85,18);

            fNumberEntry_HitMinOccu = new TGNumberEntry(fGroupFrameViewsSelector,0, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 10000);
            fGroupFrameViewsSelector->AddFrame(fNumberEntry_HitMinOccu, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_HitMinOccu->MoveResize(pos_VS_HP_X+85,pos_VS_HP_Y,65,25);

            pos_VS_HP_Y+=25;

        sizeYfGroupFrameViewsSelector = (sizeYfGroupFrameViewsSelector < pos_VS_HP_Y) ? pos_VS_HP_Y : sizeYfGroupFrameViewsSelector;

  // "Hits in um" zone
        Int_t pos_VS_HU_X=2*pos_VS_HP_X;
        Int_t pos_VS_HU_Y=18;


        // "Hits in um" label
        TGLabel *fLabelZoneHitsUm = new TGLabel(fGroupFrameViewsSelector,"Hits in tracker frame [um]");
        fLabelZoneHitsUm->SetTextJustify(36);
        fLabelZoneHitsUm->SetMargins(0,0,0,0);
        fLabelZoneHitsUm->SetWrapLength(-1);
        fGroupFrameViewsSelector->AddFrame(fLabelZoneHitsUm, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneHitsUm->MoveResize(pos_VS_HU_X,pos_VS_HU_Y,150,18);

          //DisplayHitsInTrackerFrameInUm
          fTextButtonDisHitsInTrackerFrameInUm = new TGCheckButton(fGroupFrameViewsSelector,"Separate views");
          fTextButtonDisHitsInTrackerFrameInUm->SetTextJustify(36);
          fTextButtonDisHitsInTrackerFrameInUm->SetMargins(0,0,0,0);
          fTextButtonDisHitsInTrackerFrameInUm->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInUm, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisHitsInTrackerFrameInUm->MoveResize(pos_VS_HU_X,pos_VS_HU_Y+=1.*pos_VS_deltaY,150,19);

              fTextButtonDisHitsInTrackerFrameInUm_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
              fTextButtonDisHitsInTrackerFrameInUm_Cumul->SetTextJustify(36);
              fTextButtonDisHitsInTrackerFrameInUm_Cumul->SetMargins(0,0,0,0);
              fTextButtonDisHitsInTrackerFrameInUm_Cumul->SetWrapLength(-1);
              fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInUm_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
              fTextButtonDisHitsInTrackerFrameInUm_Cumul->MoveResize(pos_VS_HU_X+20,pos_VS_HU_Y+=pos_VS_deltaY,60,19); 
          
            fGButtonGroupHitTrackInter = new TGButtonGroup(fGroupFrameViewsSelector, "Tracks planes intersec.");
            fGButtonGroupHitTrackInter->SetTitlePos(TGGroupFrame::kCenter);
            fGRadioButton_HitTrackInter_none =new TGRadioButton(fGButtonGroupHitTrackInter, "none", 0);
            fGRadioButton_HitTrackInter_cross=new TGRadioButton(fGButtonGroupHitTrackInter, "cross", 1);
            fGButtonGroupHitTrackInter->SetButton(1);
            fGButtonGroupHitTrackInter->MoveResize(pos_VS_HU_X,pos_VS_HU_Y+=1*pos_VS_deltaY);
            fGroupFrameViewsSelector->AddFrame(fGButtonGroupHitTrackInter, new TGLayoutHints(kLHintsExpandX));

          //DisplayHitsInTrackerFrameInUmOverlayed
          fTextButtonDisHitsInTrackerFrameInUmOv = new TGCheckButton(fGroupFrameViewsSelector,"Overlay view");
          fTextButtonDisHitsInTrackerFrameInUmOv->SetTextJustify(36);
          fTextButtonDisHitsInTrackerFrameInUmOv->SetMargins(0,0,0,0);
          fTextButtonDisHitsInTrackerFrameInUmOv->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInUmOv, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisHitsInTrackerFrameInUmOv->MoveResize(pos_VS_HU_X,pos_VS_HU_Y+=3*pos_VS_deltaY,150,19);
          
              fTextButtonDisHitsInTrackerFrameInUmOv_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
              fTextButtonDisHitsInTrackerFrameInUmOv_Cumul->SetTextJustify(36);
              fTextButtonDisHitsInTrackerFrameInUmOv_Cumul->SetMargins(0,0,0,0);
              fTextButtonDisHitsInTrackerFrameInUmOv_Cumul->SetWrapLength(-1);
              fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsInTrackerFrameInUmOv_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
              fTextButtonDisHitsInTrackerFrameInUmOv_Cumul->MoveResize(pos_VS_HU_X+20,pos_VS_HU_Y+=pos_VS_deltaY,116,19);          

            //Planes2displaySelect
            fListOverlayHitsInTrackerFrameInUm = new TList;
            fListBoxOverlayHitsInTrackerFrameInUm = new TGListBox(fGroupFrameViewsSelector, 89);
            //TList  *fSelected = new TList;
            for(  iPlane=1; iPlane<=nbPlanes; iPlane++ )
            {
              tPlane = tTracker->GetPlane(iPlane);
              sprintf(title, "Plane %d : %s",iPlane,tPlane->GetPlanePurpose());
              fListBoxOverlayHitsInTrackerFrameInUm->AddEntry(title, iPlane);
            }
            fListBoxOverlayHitsInTrackerFrameInUm->SetMultipleSelections(1);
            fGroupFrameViewsSelector->AddFrame(fListBoxOverlayHitsInTrackerFrameInUm, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
            fListBoxOverlayHitsInTrackerFrameInUm->MoveResize(pos_VS_HU_X,pos_VS_HU_Y+=pos_VS_deltaY,155,140);

            pos_VS_HU_Y+=135;

        sizeYfGroupFrameViewsSelector = (sizeYfGroupFrameViewsSelector < pos_VS_HU_Y) ? pos_VS_HU_Y: sizeYfGroupFrameViewsSelector;

  // "Tracks " zone
        Int_t pos_VS_TR_X = 3*pos_VS_HP_X;
        Int_t pos_VS_TR_Y = 18;

        // "Tracks " label
        TGLabel *fLabelZoneTracks = new TGLabel(fGroupFrameViewsSelector,"Tracks");
        fLabelZoneTracks->SetTextJustify(36);
        fLabelZoneTracks->SetMargins(0,0,0,0);
        fLabelZoneTracks->SetWrapLength(-1);
        fGroupFrameViewsSelector->AddFrame(fLabelZoneTracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneTracks->MoveResize(pos_VS_TR_X,pos_VS_TR_Y,150,18);
	
	fTextButtonDisTracks_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
	fTextButtonDisTracks_Cumul->SetTextJustify(36);
	fTextButtonDisTracks_Cumul->SetMargins(0,0,0,0);
	fTextButtonDisTracks_Cumul->SetWrapLength(-1);
	fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fTextButtonDisTracks_Cumul->MoveResize(pos_VS_TR_X,pos_VS_TR_Y+=1*pos_VS_deltaY,150,19);

  //DisplayTracksIn3D
          fTextButtonDisTracks3D = new TGCheckButton(fGroupFrameViewsSelector,"3D View");
          fTextButtonDisTracks3D->SetTextJustify(36);
          fTextButtonDisTracks3D->SetMargins(0,0,0,0);
          fTextButtonDisTracks3D->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks3D, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisTracks3D->MoveResize(pos_VS_TR_X,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);
          
            fTextButtonDisTracks3D_DrawTracks = new TGCheckButton(fGroupFrameViewsSelector,"draw tracks");
            fTextButtonDisTracks3D_DrawTracks->SetTextJustify(36);
            fTextButtonDisTracks3D_DrawTracks->SetMargins(0,0,0,0);
            fTextButtonDisTracks3D_DrawTracks->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks3D_DrawTracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks3D_DrawTracks->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisTracks3D_DrawSensor = new TGCheckButton(fGroupFrameViewsSelector,"draw sensors");
            fTextButtonDisTracks3D_DrawSensor->SetTextJustify(36);
            fTextButtonDisTracks3D_DrawSensor->SetMargins(0,0,0,0);
            fTextButtonDisTracks3D_DrawSensor->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks3D_DrawSensor, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks3D_DrawSensor->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisTracks3D_DrawTrackedHits = new TGCheckButton(fGroupFrameViewsSelector,"draw tracked hits");
            fTextButtonDisTracks3D_DrawTrackedHits->SetTextJustify(36);
            fTextButtonDisTracks3D_DrawTrackedHits->SetMargins(0,0,0,0);
            fTextButtonDisTracks3D_DrawTrackedHits->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks3D_DrawTrackedHits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks3D_DrawTrackedHits->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisTracks3D_DrawAllHits = new TGCheckButton(fGroupFrameViewsSelector,"draw all hits");
            fTextButtonDisTracks3D_DrawAllHits->SetTextJustify(36);
            fTextButtonDisTracks3D_DrawAllHits->SetMargins(0,0,0,0);
            fTextButtonDisTracks3D_DrawAllHits->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks3D_DrawAllHits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks3D_DrawAllHits->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);


  //DisplayTracksIn2D
          fTextButtonDisTracks2D = new TGCheckButton(fGroupFrameViewsSelector,"2D View");
          fTextButtonDisTracks2D->SetTextJustify(36);
          fTextButtonDisTracks2D->SetMargins(0,0,0,0);
          fTextButtonDisTracks2D->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks2D, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisTracks2D->MoveResize(pos_VS_TR_X,pos_VS_TR_Y+=1.5*pos_VS_deltaY,150,19);
          
            fTextButtonDisTracks2D_DrawTracks = new TGCheckButton(fGroupFrameViewsSelector,"draw tracks");
            fTextButtonDisTracks2D_DrawTracks->SetTextJustify(36);
            fTextButtonDisTracks2D_DrawTracks->SetMargins(0,0,0,0);
            fTextButtonDisTracks2D_DrawTracks->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks2D_DrawTracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks2D_DrawTracks->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisTracks2D_DrawSensor = new TGCheckButton(fGroupFrameViewsSelector,"draw sensors");
            fTextButtonDisTracks2D_DrawSensor->SetTextJustify(36);
            fTextButtonDisTracks2D_DrawSensor->SetMargins(0,0,0,0);
            fTextButtonDisTracks2D_DrawSensor->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks2D_DrawSensor, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks2D_DrawSensor->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisTracks2D_DrawTrackedHits = new TGCheckButton(fGroupFrameViewsSelector,"draw tracked hits");
            fTextButtonDisTracks2D_DrawTrackedHits->SetTextJustify(36);
            fTextButtonDisTracks2D_DrawTrackedHits->SetMargins(0,0,0,0);
            fTextButtonDisTracks2D_DrawTrackedHits->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks2D_DrawTrackedHits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks2D_DrawTrackedHits->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisTracks2D_DrawAllHits = new TGCheckButton(fGroupFrameViewsSelector,"draw all hits");
            fTextButtonDisTracks2D_DrawAllHits->SetTextJustify(36);
            fTextButtonDisTracks2D_DrawAllHits->SetMargins(0,0,0,0);
            fTextButtonDisTracks2D_DrawAllHits->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisTracks2D_DrawAllHits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisTracks2D_DrawAllHits->MoveResize(pos_VS_TR_X+20,pos_VS_TR_Y+=1.2*pos_VS_deltaY,150,19);

            pos_VS_TR_Y+=25;
	    
	    

        sizeYfGroupFrameViewsSelector = (sizeYfGroupFrameViewsSelector < pos_VS_TR_Y) ? pos_VS_TR_Y : sizeYfGroupFrameViewsSelector;

        // "Vertices" zone
        Int_t pos_VS_VT_X = 4*pos_VS_HP_X;
        Int_t pos_VS_VT_Y = 18;

// "Vertices" label
        TGLabel *fLabelZoneVertex = new TGLabel(fGroupFrameViewsSelector,"Vertices");
        fLabelZoneVertex->SetTextJustify(36);
        fLabelZoneVertex->SetMargins(0,0,0,0);
        fLabelZoneVertex->SetWrapLength(-1);
        fGroupFrameViewsSelector->AddFrame(fLabelZoneVertex, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneVertex->MoveResize(pos_VS_VT_X,pos_VS_VT_Y,150,18);

          fTextButtonDisVertices_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
          fTextButtonDisVertices_Cumul->SetTextJustify(36);
          fTextButtonDisVertices_Cumul->SetMargins(0,0,0,0);
          fTextButtonDisVertices_Cumul->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisVertices_Cumul->MoveResize(pos_VS_VT_X,pos_VS_VT_Y+=1.2*pos_VS_deltaY,60,19);
        
  //DisplayVertices 3D
          fTextButtonDisVertices3D = new TGCheckButton(fGroupFrameViewsSelector,"3D View");
          fTextButtonDisVertices3D->SetTextJustify(36);
          fTextButtonDisVertices3D->SetMargins(0,0,0,0);
          fTextButtonDisVertices3D->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisVertices3D->MoveResize(pos_VS_VT_X,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);
   
            fTextButtonDisVertices3D_DrawVerticesPoints = new TGCheckButton(fGroupFrameViewsSelector,"draw vertices points");
            fTextButtonDisVertices3D_DrawVerticesPoints->SetTextJustify(36);
            fTextButtonDisVertices3D_DrawVerticesPoints->SetMargins(0,0,0,0);
            fTextButtonDisVertices3D_DrawVerticesPoints->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawVerticesPoints, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVertices3D_DrawVerticesPoints->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

              fTextButtonDisVertices3D_DrawVerticesPointsSmall = new TGCheckButton(fGroupFrameViewsSelector,"small points");
              fTextButtonDisVertices3D_DrawVerticesPointsSmall->SetTextJustify(36);
              fTextButtonDisVertices3D_DrawVerticesPointsSmall->SetMargins(0,0,0,0);
              fTextButtonDisVertices3D_DrawVerticesPointsSmall->SetWrapLength(-1);
              fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawVerticesPointsSmall, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
              fTextButtonDisVertices3D_DrawVerticesPointsSmall->MoveResize(pos_VS_VT_X+20+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisVertices3D_DrawSensor = new TGCheckButton(fGroupFrameViewsSelector,"draw sensors");
            fTextButtonDisVertices3D_DrawSensor->SetTextJustify(36);
            fTextButtonDisVertices3D_DrawSensor->SetMargins(0,0,0,0);
            fTextButtonDisVertices3D_DrawSensor->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawSensor, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVertices3D_DrawSensor->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisVertices3D_DrawTracks = new TGCheckButton(fGroupFrameViewsSelector,"draw tracks");
            fTextButtonDisVertices3D_DrawTracks->SetTextJustify(36);
            fTextButtonDisVertices3D_DrawTracks->SetMargins(0,0,0,0);
            fTextButtonDisVertices3D_DrawTracks->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawTracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVertices3D_DrawTracks->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisVertices3D_DrawTrackDir = new TGCheckButton(fGroupFrameViewsSelector,"draw tracker direction");
            fTextButtonDisVertices3D_DrawTrackDir->SetTextJustify(36);
            fTextButtonDisVertices3D_DrawTrackDir->SetMargins(0,0,0,0);
            fTextButtonDisVertices3D_DrawTrackDir->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawTrackDir, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVertices3D_DrawTrackDir->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisVertices3D_DrawTarget = new TGCheckButton(fGroupFrameViewsSelector,"draw target");
            fTextButtonDisVertices3D_DrawTarget->SetTextJustify(36);
            fTextButtonDisVertices3D_DrawTarget->SetMargins(0,0,0,0);
            fTextButtonDisVertices3D_DrawTarget->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawTarget, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVertices3D_DrawTarget->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisVertices3D_DrawBeam = new TGCheckButton(fGroupFrameViewsSelector,"draw beam");
            fTextButtonDisVertices3D_DrawBeam->SetTextJustify(36);
            fTextButtonDisVertices3D_DrawBeam->SetMargins(0,0,0,0);
            fTextButtonDisVertices3D_DrawBeam->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawBeam, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVertices3D_DrawBeam->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisVertices3D_DrawAcceptance = new TGCheckButton(fGroupFrameViewsSelector,"draw acceptance");
            fTextButtonDisVertices3D_DrawAcceptance->SetTextJustify(36);
            fTextButtonDisVertices3D_DrawAcceptance->SetMargins(0,0,0,0);
            fTextButtonDisVertices3D_DrawAcceptance->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVertices3D_DrawAcceptance, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVertices3D_DrawAcceptance->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);

            
  //DisplayVertices projections
          fTextButtonDisVerticesProjs = new TGCheckButton(fGroupFrameViewsSelector,"Projections");
          fTextButtonDisVerticesProjs->SetTextJustify(36);
          fTextButtonDisVerticesProjs->SetMargins(0,0,0,0);
          fTextButtonDisVerticesProjs->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisVerticesProjs, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisVerticesProjs->MoveResize(pos_VS_VT_X,pos_VS_VT_Y+=1.5*pos_VS_deltaY,150,19);

            fTextButtonDisVerticesProjs_DrawTarget = new TGCheckButton(fGroupFrameViewsSelector,"draw target");
            fTextButtonDisVerticesProjs_DrawTarget->SetTextJustify(36);
            fTextButtonDisVerticesProjs_DrawTarget->SetMargins(0,0,0,0);
            fTextButtonDisVerticesProjs_DrawTarget->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisVerticesProjs_DrawTarget, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisVerticesProjs_DrawTarget->MoveResize(pos_VS_VT_X+20,pos_VS_VT_Y+=1.2*pos_VS_deltaY,150,19);
            
          pos_VS_VT_Y+=10;
          
        sizeYfGroupFrameViewsSelector = (sizeYfGroupFrameViewsSelector < pos_VS_VT_Y) ? pos_VS_VT_Y : sizeYfGroupFrameViewsSelector;


// "Statistics" zone
        Int_t pos_VS_ST_X=5*pos_VS_HP_X+10;
        Int_t pos_VS_ST_Y=18;

        // Statistics label
        TGLabel *fLabelZoneStatistics = new TGLabel(fGroupFrameViewsSelector,"Statistics");
        fLabelZoneStatistics->SetTextJustify(36);
        fLabelZoneStatistics->SetMargins(0,0,0,0);
        fLabelZoneStatistics->SetWrapLength(-1);
        fGroupFrameViewsSelector->AddFrame(fLabelZoneStatistics, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneStatistics->MoveResize(pos_VS_ST_X,pos_VS_ST_Y,150,18);

//         TGLabel *fLabelZoneStatisticsCMOS = new TGLabel(fGroupFrameViewsSelector,"CMOS");
//         fLabelZoneStatisticsCMOS->SetTextJustify(36);
//         fLabelZoneStatisticsCMOS->SetMargins(0,0,0,0);
//         fLabelZoneStatisticsCMOS->SetWrapLength(-1);
//         fGroupFrameViewsSelector->AddFrame(fLabelZoneStatisticsCMOS, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
//         fLabelZoneStatisticsCMOS->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1*pos_VS_deltaY,150,18);
          
  //DisplayLinesOverflow
          fTextButtonDisCMOSoverflowLines = new TGCheckButton(fGroupFrameViewsSelector,"Lines Overflow");
          fTextButtonDisCMOSoverflowLines->SetTextJustify(36);
          fTextButtonDisCMOSoverflowLines->SetMargins(0,0,0,0);
          fTextButtonDisCMOSoverflowLines->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisCMOSoverflowLines, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisCMOSoverflowLines->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.*pos_VS_deltaY,116,19);

            fTextButtonDisCMOSoverflowLines_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisCMOSoverflowLines_Cumul->SetTextJustify(36);
            fTextButtonDisCMOSoverflowLines_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisCMOSoverflowLines_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisCMOSoverflowLines_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisCMOSoverflowLines_Cumul->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,60,19);

  //DisplayClustersSize
          fTextButtonDisHitsSize = new TGCheckButton(fGroupFrameViewsSelector,"Clusters size");
          fTextButtonDisHitsSize->SetTextJustify(36);
          fTextButtonDisHitsSize->SetMargins(0,0,0,0);
          fTextButtonDisHitsSize->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsSize, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisHitsSize->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.2*pos_VS_deltaY,116,19);

            fNumberEntry_ClustersBinMax = new TGNumberEntry(fGroupFrameViewsSelector,30, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,1, 100);
            fGroupFrameViewsSelector->AddFrame(fNumberEntry_ClustersBinMax, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_ClustersBinMax->MoveResize(pos_VS_ST_X+95,pos_VS_ST_Y,50,25);
          
            fTextButtonDisHitsSize_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisHitsSize_Cumul->SetTextJustify(36);
            fTextButtonDisHitsSize_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisHitsSize_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsSize_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsSize_Cumul->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,60,19);          
          

  //DisplayClustersSize : ALL
            fTextButtonDisHitsSizeAll = new TGCheckButton(fGroupFrameViewsSelector,"All");
            fTextButtonDisHitsSizeAll->SetTextJustify(36);
            fTextButtonDisHitsSizeAll->SetMargins(0,0,0,0);
            fTextButtonDisHitsSizeAll->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsSizeAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsSizeAll->MoveResize(pos_VS_ST_X + 20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,45,19);
            //DisplayClustersSize : TRACKED
            fTextButtonDisHitsSizeTracked = new TGCheckButton(fGroupFrameViewsSelector,"Tck");
            fTextButtonDisHitsSizeTracked->SetTextJustify(36);
            fTextButtonDisHitsSizeTracked->SetMargins(0,0,0,0);
            fTextButtonDisHitsSizeTracked->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsSizeTracked, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsSizeTracked->MoveResize(pos_VS_ST_X + 60,pos_VS_ST_Y,45,19);
            //DisplayClustersSize : UNTRACKED
            fTextButtonDisHitsSizeUnTracked = new TGCheckButton(fGroupFrameViewsSelector,"Untck");
            fTextButtonDisHitsSizeUnTracked->SetTextJustify(36);
            fTextButtonDisHitsSizeUnTracked->SetMargins(0,0,0,0);
            fTextButtonDisHitsSizeUnTracked->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsSizeUnTracked, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsSizeUnTracked->MoveResize(pos_VS_ST_X + 105,pos_VS_ST_Y,55,19);

  //DisplayHitsTracksAssociation
          fTextButtonDisHitsAssociation = new TGCheckButton(fGroupFrameViewsSelector,"Hits-Track association");
          fTextButtonDisHitsAssociation->SetTextJustify(36);
          fTextButtonDisHitsAssociation->SetMargins(0,0,0,0);
          fTextButtonDisHitsAssociation->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsAssociation, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisHitsAssociation->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.2*pos_VS_deltaY,145,19);
            
            fTextButtonDisHitsAssociation_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisHitsAssociation_Cumul->SetTextJustify(36);
            fTextButtonDisHitsAssociation_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisHitsAssociation_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsAssociation_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsAssociation_Cumul->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,60,19);            
            
  //DisplayHitsByEvent
          fTextButtonDisHitsByEvent = new TGCheckButton(fGroupFrameViewsSelector,"Hits / event ");
          fTextButtonDisHitsByEvent->SetTextJustify(36);
          fTextButtonDisHitsByEvent->SetMargins(0,0,0,0);
          fTextButtonDisHitsByEvent->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsByEvent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisHitsByEvent->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.2*pos_VS_deltaY,116,19);
            
            fNumberEntry_HitsByEventBinMax = new TGNumberEntry(fGroupFrameViewsSelector,10, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,10, 100000);
            fGroupFrameViewsSelector->AddFrame(fNumberEntry_HitsByEventBinMax, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_HitsByEventBinMax->MoveResize(pos_VS_ST_X+95,pos_VS_ST_Y,50,25);
          
          
            fTextButtonDisHitsByEvent_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisHitsByEvent_Cumul->SetTextJustify(36);
            fTextButtonDisHitsByEvent_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisHitsByEvent_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisHitsByEvent_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisHitsByEvent_Cumul->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,60,19);          

  //Tracks & Triggers
          fTextButtonDisStats = new TGCheckButton(fGroupFrameViewsSelector,"Tracks + triggers");
          fTextButtonDisStats->SetTextJustify(36);
          fTextButtonDisStats->SetMargins(0,0,0,0);
          fTextButtonDisStats->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisStats, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisStats->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.2*pos_VS_deltaY,150,19);

            fTextButtonDisStats_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisStats_Cumul->SetTextJustify(36);
            fTextButtonDisStats_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisStats_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisStats_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisStats_Cumul->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,60,19);
            
            fTextButtonDisStats_withTriggers = new TGCheckButton(fGroupFrameViewsSelector,"with triggers");
            fTextButtonDisStats_withTriggers->SetTextJustify(36);
            fTextButtonDisStats_withTriggers->SetMargins(0,0,0,0);
            fTextButtonDisStats_withTriggers->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisStats_withTriggers, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisStats_withTriggers->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,90,19);
       
            TGLabel *fLabelHistoChi2Max = new TGLabel(fGroupFrameViewsSelector,"histo Chi2 limit");
            fLabelHistoChi2Max->SetTextJustify(36);
            fLabelHistoChi2Max->SetMargins(0,0,0,0);
            fLabelHistoChi2Max->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fLabelHistoChi2Max, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelHistoChi2Max->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1*pos_VS_deltaY,85,18);

            fNumberEntry_TracksHistoChi2Max = new TGNumberEntry(fGroupFrameViewsSelector,10, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,10, 100000);
            fGroupFrameViewsSelector->AddFrame(fNumberEntry_TracksHistoChi2Max, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_TracksHistoChi2Max->MoveResize(pos_VS_ST_X+90,pos_VS_ST_Y,60,20);

            TGLabel *fLabelHistoSlopeMax = new TGLabel(fGroupFrameViewsSelector,"histo Slope limit");
            fLabelHistoSlopeMax->SetTextJustify(36);
            fLabelHistoSlopeMax->SetMargins(0,0,0,0);
            fLabelHistoSlopeMax->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fLabelHistoSlopeMax, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelHistoSlopeMax->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.*pos_VS_deltaY,85,18);

            fNumberEntry_TracksHistoSlopeMax = new TGNumberEntry(fGroupFrameViewsSelector,30, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,1, 90);
            fGroupFrameViewsSelector->AddFrame(fNumberEntry_TracksHistoSlopeMax, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_TracksHistoSlopeMax->MoveResize(pos_VS_ST_X+90,pos_VS_ST_Y,60,20);

//      // Statistics Along events
//      TGLabel *fLabelZoneStatisticsAlongEvents = new TGLabel(fGroupFrameViewsSelector,"Along events");
//      fLabelZoneStatisticsAlongEvents->SetTextJustify(36);
//      fLabelZoneStatisticsAlongEvents->SetMargins(0,0,0,0);
//      fLabelZoneStatisticsAlongEvents->SetWrapLength(-1);
//      fGroupFrameViewsSelector->AddFrame(fLabelZoneStatisticsAlongEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
//      fLabelZoneStatisticsAlongEvents->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.2*pos_VS_deltaY,150,18);

  //Stats along events
          fTextButtonDisStatsAlongEvents = new TGCheckButton(fGroupFrameViewsSelector,"Along events");//Hits Tracks Triggers");
          fTextButtonDisStatsAlongEvents->SetTextJustify(36);
          fTextButtonDisStatsAlongEvents->SetMargins(0,0,0,0);
          fTextButtonDisStatsAlongEvents->SetWrapLength(-1);
          fGroupFrameViewsSelector->AddFrame(fTextButtonDisStatsAlongEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisStatsAlongEvents->MoveResize(pos_VS_ST_X,pos_VS_ST_Y+=1.2*pos_VS_deltaY,150,19);
          
            fTextButtonDisStatsAlongEvents_Cumul = new TGCheckButton(fGroupFrameViewsSelector,"cumul");
            fTextButtonDisStatsAlongEvents_Cumul->SetTextJustify(36);
            fTextButtonDisStatsAlongEvents_Cumul->SetMargins(0,0,0,0);
            fTextButtonDisStatsAlongEvents_Cumul->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisStatsAlongEvents_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisStatsAlongEvents_Cumul->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,60,19);
        
            fTextButtonDisStatsAlongEvents_withTriggers = new TGCheckButton(fGroupFrameViewsSelector,"with triggers");//Hits Tracks Triggers");
            fTextButtonDisStatsAlongEvents_withTriggers->SetTextJustify(36);
            fTextButtonDisStatsAlongEvents_withTriggers->SetMargins(0,0,0,0);
            fTextButtonDisStatsAlongEvents_withTriggers->SetWrapLength(-1);
            fGroupFrameViewsSelector->AddFrame(fTextButtonDisStatsAlongEvents_withTriggers, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonDisStatsAlongEvents_withTriggers->MoveResize(pos_VS_ST_X+20,pos_VS_ST_Y+=0.8*pos_VS_deltaY,90,19);

            pos_VS_ST_Y+=10;

        sizeYfGroupFrameViewsSelector = (sizeYfGroupFrameViewsSelector < pos_VS_ST_Y) ? pos_VS_ST_Y : sizeYfGroupFrameViewsSelector;



        // Separation 1
        TGVertical3DLine *fVertical3DLine1354 = new TGVertical3DLine(fGroupFrameViewsSelector,8,200);
        fGroupFrameViewsSelector->AddFrame(fVertical3DLine1354, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1354->MoveResize(pos_VS_HP_X-5,16,4,sizeYfGroupFrameViewsSelector-10);

        // separation 2
        TGVertical3DLine *fVertical3DLine1355 = new TGVertical3DLine(fGroupFrameViewsSelector,8,200);
        fGroupFrameViewsSelector->AddFrame(fVertical3DLine1355, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1355->MoveResize(pos_VS_HU_X-5,16,4,sizeYfGroupFrameViewsSelector-10);

        // Separation 3
        TGVertical3DLine *fVertical3DLine1356 = new TGVertical3DLine(fGroupFrameViewsSelector,8,200);
        fGroupFrameViewsSelector->AddFrame(fVertical3DLine1356, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1356->MoveResize(pos_VS_TR_X-5,16,4,sizeYfGroupFrameViewsSelector-10);

        // Separation 4
        TGVertical3DLine *fVertical3DLine1357 = new TGVertical3DLine(fGroupFrameViewsSelector,8,200);
        fGroupFrameViewsSelector->AddFrame(fVertical3DLine1357, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1357->MoveResize(pos_VS_VT_X-5,16,4,sizeYfGroupFrameViewsSelector-10);

        // Separation 5
        TGVertical3DLine *fVertical3DLine1358 = new TGVertical3DLine(fGroupFrameViewsSelector,8,200);
        fGroupFrameViewsSelector->AddFrame(fVertical3DLine1358, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1358->MoveResize(pos_VS_ST_X-5,16,4,sizeYfGroupFrameViewsSelector-10);


      sizeYfGroupFrameViewsSelector += 20;
      fGroupFrameViewsSelector->SetLayoutManager(new TGVerticalLayout(fGroupFrameViewsSelector));
      fGroupFrameViewsSelector->Resize(length_X-10,sizeYfGroupFrameViewsSelector);
      fCompositeFrameTabAnalysis->AddFrame(fGroupFrameViewsSelector, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

    sizeYfCompositeFrameTabAnalysis = sizeYfGroupFrameViewsSelector + 10;

    
//*********************************************************************************************************  
//                           container of Tab "Noise"
//*********************************************************************************************************
    TGCompositeFrame *fCompositeFrameTabNoise;
    fCompositeFrameTabNoise = fTabTop->AddTab("Noise");
    fCompositeFrameTabNoise->SetLayoutManager(new TGVerticalLayout(fCompositeFrameTabNoise));
    Int_t sizeYfCompositeFrameTabNoise = 0;
    
      // "Views selection" group frame
      //Int_t pos_VS_Noise_Ybottom;
      TGGroupFrame *fGroupFrameViewsNoiseSelector = new TGGroupFrame(fCompositeFrameTabNoise,"Views selection");
      fGroupFrameViewsNoiseSelector->SetLayoutBroken(kTRUE);
      Int_t sizeYfGroupFrameViewsNoiseSelector=0;

        // "Pixels" zone
        Int_t pos_VSNoise_RD_X=5;
        Int_t pos_VSNoise_RD_Y=18;
        Int_t pos_Noise_deltaY=20;

        TGLabel *fLabelNoiseZoneRawdata = new TGLabel(fGroupFrameViewsNoiseSelector,"Pixels [pixels]");
        fLabelNoiseZoneRawdata->SetTextJustify(36);
        fLabelNoiseZoneRawdata->SetMargins(0,0,0,0);
        fLabelNoiseZoneRawdata->SetWrapLength(-1);
        fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseZoneRawdata, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelNoiseZoneRawdata->MoveResize(pos_VSNoise_RD_X,pos_VSNoise_RD_Y,150,18);

          //DisplayPixelsInCmosFrameInPixel
          fTextButtonNoiseDisRawDatCmoNatGeoInPix = new TGCheckButton(fGroupFrameViewsNoiseSelector,"CMOS frame");
          fTextButtonNoiseDisRawDatCmoNatGeoInPix->SetTextJustify(36);
          fTextButtonNoiseDisRawDatCmoNatGeoInPix->SetMargins(0,0,0,0);
          fTextButtonNoiseDisRawDatCmoNatGeoInPix->SetWrapLength(-1);
          fGroupFrameViewsNoiseSelector->AddFrame(fTextButtonNoiseDisRawDatCmoNatGeoInPix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonNoiseDisRawDatCmoNatGeoInPix->MoveResize(pos_VSNoise_RD_X,pos_VSNoise_RD_Y+=1.2*pos_Noise_deltaY,150,19);

            fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit = new TGCheckButton(fGroupFrameViewsNoiseSelector,"with hits");
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit->SetTextJustify(36);
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit->SetMargins(0,0,0,0);
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y+=pos_Noise_deltaY,116,19);


            fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar = new TGCheckButton(fGroupFrameViewsNoiseSelector,"with color bar");
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar->SetTextJustify(36);
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar->SetMargins(0,0,0,0);
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y+=pos_Noise_deltaY,116,19);
            

            TGLabel *fLabelNoiseMinPixOccu = new TGLabel(fGroupFrameViewsNoiseSelector,"display threshold: \n(absolute number)");
            fLabelNoiseMinPixOccu->SetTextJustify(36);
            fLabelNoiseMinPixOccu->SetMargins(0,0,0,0);
            fLabelNoiseMinPixOccu->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseMinPixOccu, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseMinPixOccu->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y += 1.4 * pos_Noise_deltaY,105,25);

            fNumberEntry_NoiseRawMinPixOccu = new TGNumberEntry(fGroupFrameViewsNoiseSelector,2, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 10000);
            fGroupFrameViewsNoiseSelector->AddFrame(fNumberEntry_NoiseRawMinPixOccu, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_NoiseRawMinPixOccu->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y+=1.45*pos_Noise_deltaY,100,25);
         
            TGLabel *fLabelNoiseMinPixOccuRel = new TGLabel(fGroupFrameViewsNoiseSelector,"relative threshold:");
            fLabelNoiseMinPixOccuRel->SetTextJustify(36);
            fLabelNoiseMinPixOccuRel->SetMargins(0,0,0,0);
            fLabelNoiseMinPixOccuRel->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseMinPixOccuRel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseMinPixOccuRel->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y += 1.6 * pos_Noise_deltaY,105,20);
            
            fLabelNoiseMinPixOccuRelValue = new TGLabel(fGroupFrameViewsNoiseSelector,"???");
            fLabelNoiseMinPixOccuRelValue->SetTextJustify(36);
            fLabelNoiseMinPixOccuRelValue->SetMargins(0,0,0,0);
            fLabelNoiseMinPixOccuRelValue->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseMinPixOccuRelValue, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseMinPixOccuRelValue->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y += 0.8 * pos_Noise_deltaY,105,20);

            TGLabel *fLabelNoiseNbEvents = new TGLabel(fGroupFrameViewsNoiseSelector,"based on # events:");
            fLabelNoiseNbEvents->SetTextJustify(36);
            fLabelNoiseNbEvents->SetMargins(0,0,0,0);
            fLabelNoiseNbEvents->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseNbEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseNbEvents->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y += 1.3 * pos_Noise_deltaY,105,20); 
            
            fLabelNoiseNbOfEvents = new TGLabel(fGroupFrameViewsNoiseSelector,"???");
            fLabelNoiseNbOfEvents->SetTextJustify(36);
            fLabelNoiseNbOfEvents->SetMargins(0,0,0,0);
            fLabelNoiseNbOfEvents->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseNbOfEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseNbOfEvents->MoveResize(pos_VSNoise_RD_X+20,pos_VSNoise_RD_Y += 0.8 * pos_Noise_deltaY,105,20);
            
            fTextButtonGenerateVetoPixelsCode = new TGTextButton(fGroupFrameViewsNoiseSelector,"Write Veto Piels Code");
            fTextButtonGenerateVetoPixelsCode->SetTextJustify(36);
            fTextButtonGenerateVetoPixelsCode->SetMargins(0,0,0,0);
            fTextButtonGenerateVetoPixelsCode->SetWrapLength(-1);
            fTextButtonGenerateVetoPixelsCode->Resize(130,25);
            fGroupFrameViewsNoiseSelector->AddFrame(fTextButtonGenerateVetoPixelsCode, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fTextButtonGenerateVetoPixelsCode->MoveResize(pos_VSNoise_RD_X,pos_VSNoise_RD_Y += 1.2 * pos_Noise_deltaY,130,25);

            

            pos_VSNoise_RD_Y+=25;

       sizeYfGroupFrameViewsNoiseSelector = (sizeYfGroupFrameViewsNoiseSelector < pos_VSNoise_RD_Y) ? pos_VSNoise_RD_Y : sizeYfGroupFrameViewsNoiseSelector;      
       
        // "Noise Study" zone
        Int_t pos_VSNoise_NS_X=160;
        Int_t pos_VSNoise_NS_Y=18;

        
        // Noise study label
        TGLabel *fLabelZoneNoiseStudy = new TGLabel(fGroupFrameViewsNoiseSelector,"Noise Study");
        fLabelZoneNoiseStudy->SetTextJustify(36);
        fLabelZoneNoiseStudy->SetMargins(0,0,0,0);
        fLabelZoneNoiseStudy->SetWrapLength(-1);
        fGroupFrameViewsNoiseSelector->AddFrame(fLabelZoneNoiseStudy, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneNoiseStudy->MoveResize(pos_VSNoise_NS_X,pos_VSNoise_NS_Y,150,18);

          //DisplayHitsInCmosFrameInPixel
          fTextButtonDisNoiseStudy = new TGCheckButton(fGroupFrameViewsNoiseSelector,"Analyse noise");
          fTextButtonDisNoiseStudy->SetTextJustify(36);
          fTextButtonDisNoiseStudy->SetMargins(0,0,0,0);
          fTextButtonDisNoiseStudy->SetWrapLength(-1);
          fGroupFrameViewsNoiseSelector->AddFrame(fTextButtonDisNoiseStudy, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonDisNoiseStudy->MoveResize(pos_VSNoise_NS_X,pos_VSNoise_NS_Y+=1.2*pos_Noise_deltaY,150,19);  
          
            TGLabel *fLabelNoiseStudyThresholds = new TGLabel(fGroupFrameViewsNoiseSelector,"Thresholds: \n(relative numbers)");
            fLabelNoiseStudyThresholds->SetTextJustify(36);
            fLabelNoiseStudyThresholds->SetMargins(0,0,0,0);
            fLabelNoiseStudyThresholds->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseStudyThresholds, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseStudyThresholds->MoveResize(pos_VSNoise_NS_X+5,pos_VSNoise_NS_Y += 1.4 * pos_Noise_deltaY,105,25);
            
            TGLabel *fLabelNoiseStudyThresholdMin = new TGLabel(fGroupFrameViewsNoiseSelector,"Min.");
            fLabelNoiseStudyThresholdMin->SetTextJustify(36);
            fLabelNoiseStudyThresholdMin->SetMargins(0,0,0,0);
            fLabelNoiseStudyThresholdMin->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseStudyThresholdMin, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseStudyThresholdMin->MoveResize(pos_VSNoise_NS_X+5,pos_VSNoise_NS_Y += 1.4 * pos_Noise_deltaY,30,25);

            fNumberEntry_NoiseThresholdsMin = new TGNumberEntry(fGroupFrameViewsNoiseSelector,0.001, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 1);
            fGroupFrameViewsNoiseSelector->AddFrame(fNumberEntry_NoiseThresholdsMin, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_NoiseThresholdsMin->MoveResize(pos_VSNoise_NS_X+40,pos_VSNoise_NS_Y,80,25);
            
            TGLabel *fLabelNoiseStudyThresholdMax = new TGLabel(fGroupFrameViewsNoiseSelector,"Max.");
            fLabelNoiseStudyThresholdMax->SetTextJustify(36);
            fLabelNoiseStudyThresholdMax->SetMargins(0,0,0,0);
            fLabelNoiseStudyThresholdMax->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseStudyThresholdMax, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseStudyThresholdMax->MoveResize(pos_VSNoise_NS_X+5,pos_VSNoise_NS_Y += 1.4 * pos_Noise_deltaY,30,25);
            
            fNumberEntry_NoiseThresholdsMax = new TGNumberEntry(fGroupFrameViewsNoiseSelector,1, 9,999, TGNumberFormat::kNESRealFour,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 1);
            fGroupFrameViewsNoiseSelector->AddFrame(fNumberEntry_NoiseThresholdsMax, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_NoiseThresholdsMax->MoveResize(pos_VSNoise_NS_X+40,pos_VSNoise_NS_Y,80,25);

            TGLabel *fLabelNoiseStudyThresholdNb = new TGLabel(fGroupFrameViewsNoiseSelector,"Steps");
            fLabelNoiseStudyThresholdNb->SetTextJustify(36);
            fLabelNoiseStudyThresholdNb->SetMargins(0,0,0,0);
            fLabelNoiseStudyThresholdNb->SetWrapLength(-1);
            fGroupFrameViewsNoiseSelector->AddFrame(fLabelNoiseStudyThresholdNb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fLabelNoiseStudyThresholdNb->MoveResize(pos_VSNoise_NS_X+5,pos_VSNoise_NS_Y += 1.4 * pos_Noise_deltaY,30,25); 
            
            fNumberEntry_NoiseThresholdsNb = new TGNumberEntry(fGroupFrameViewsNoiseSelector,20, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,1, 100);
            fGroupFrameViewsNoiseSelector->AddFrame(fNumberEntry_NoiseThresholdsNb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
            fNumberEntry_NoiseThresholdsNb->MoveResize(pos_VSNoise_NS_X+40,pos_VSNoise_NS_Y,80,25);
       
       
      sizeYfGroupFrameViewsNoiseSelector = (sizeYfGroupFrameViewsNoiseSelector < pos_VSNoise_NS_Y) ? pos_VSNoise_NS_Y : sizeYfGroupFrameViewsNoiseSelector; 
       
        // Separation 1
        TGVertical3DLine *fVertical3DLine13540 = new TGVertical3DLine(fGroupFrameViewsNoiseSelector,8,200);
        fGroupFrameViewsNoiseSelector->AddFrame(fVertical3DLine13540, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine13540->MoveResize(pos_VSNoise_NS_X-5,16,4,sizeYfGroupFrameViewsNoiseSelector-10);


      sizeYfGroupFrameViewsNoiseSelector += 20;
      fGroupFrameViewsNoiseSelector ->SetLayoutManager(new TGVerticalLayout(fGroupFrameViewsNoiseSelector));
      fGroupFrameViewsNoiseSelector ->Resize(pos_VSNoise_NS_X*2,sizeYfGroupFrameViewsNoiseSelector);
      fCompositeFrameTabNoise ->AddFrame(fGroupFrameViewsNoiseSelector, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

    sizeYfCompositeFrameTabNoise = sizeYfGroupFrameViewsNoiseSelector + 10;

 
//*********************************************************************************************************  
//                           container of Tab "Alignment"
//*********************************************************************************************************
    TGCompositeFrame *fCompositeFrameTabAlignment;
    fCompositeFrameTabAlignment = fTabTop->AddTab("Manual alignment");
    fCompositeFrameTabAlignment->SetLayoutManager(new TGVerticalLayout(fCompositeFrameTabAlignment));
    Int_t sizeYfCompositeFrameTabAlignment = 0;

      // "Control" group frame
      TGGroupFrame *fGroupFrameAlignDisplayMove = new TGGroupFrame(fCompositeFrameTabAlignment,"Display - Move");
      fGroupFrameAlignDisplayMove->SetLayoutBroken(kTRUE);
      Int_t sizeYfGroupFrameAlignDisplayMove = 0 ;

        // Planes to display zone
        Int_t pos_AL_CO_DI_X=10;
        Int_t pos_AL_CO_DI_Y=18;
        Int_t pos_AL_deltaY=15;
	//Int_t pos_CO_Ybottom=200;
        Int_t pos_AL_CO_DI_Xlength=120;

        TGLabel *fLabelZoneAlPlanesToDisp = new TGLabel(fGroupFrameAlignDisplayMove,"Planes to display");
        fLabelZoneAlPlanesToDisp->SetTextJustify(36);
        fLabelZoneAlPlanesToDisp->SetMargins(0,0,0,0);
        fLabelZoneAlPlanesToDisp->SetWrapLength(-1);
        fGroupFrameAlignDisplayMove->AddFrame(fLabelZoneAlPlanesToDisp, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneAlPlanesToDisp->MoveResize(pos_AL_CO_DI_X,pos_AL_CO_DI_Y,pos_AL_CO_DI_Xlength,18);

          fListAlignPlanesToDisp = new TList;
          fListBoxAlignPlanesToDisp = new TGListBox(fGroupFrameAlignDisplayMove, 89);
          //TList  *fSelected = new TList;
          for(  iPlane=1; iPlane<=nbPlanes; iPlane++ )
          {
            tPlane = tTracker->GetPlane(iPlane);
            sprintf(title, "Plane %d : %s",iPlane,tPlane->GetPlanePurpose());
            fListBoxAlignPlanesToDisp->AddEntry(title, iPlane);
          }
          fListBoxAlignPlanesToDisp->SetMultipleSelections(1);
          fGroupFrameAlignDisplayMove->AddFrame(fListBoxAlignPlanesToDisp, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
          fListBoxAlignPlanesToDisp->MoveResize(pos_AL_CO_DI_X,pos_AL_CO_DI_Y+=1.5*pos_AL_deltaY,pos_AL_CO_DI_Xlength,150);

          pos_AL_CO_DI_Y+=150;


        sizeYfGroupFrameAlignDisplayMove = (sizeYfGroupFrameAlignDisplayMove < pos_AL_CO_DI_Y ) ? pos_AL_CO_DI_Y : sizeYfGroupFrameAlignDisplayMove;

        // Planes to move zone
        Int_t pos_AL_CO_MO_X = pos_AL_CO_DI_X + pos_AL_CO_DI_Xlength + 10;
        Int_t pos_AL_CO_MO_Y=18;
        //Int_t pos_AL_CO_MO_Xlength=120;


          TGLabel *fLabelZoneAlPlanesToMove = new TGLabel(fGroupFrameAlignDisplayMove,"Planes to move");
          fLabelZoneAlPlanesToMove->SetTextJustify(36);
          fLabelZoneAlPlanesToMove->SetMargins(0,0,0,0);
          fLabelZoneAlPlanesToMove->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelZoneAlPlanesToMove, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelZoneAlPlanesToMove->MoveResize(pos_AL_CO_MO_X,pos_AL_CO_MO_Y,pos_AL_CO_DI_Xlength,18);

          fListAlignPlanesToMove = new TList;
          fListBoxAlignPlanesToMove = new TGListBox(fGroupFrameAlignDisplayMove, 89);
          //TList  *fSelected = new TList;
          for(  iPlane=1; iPlane<=nbPlanes; iPlane++ )
          {
            tPlane = tTracker->GetPlane(iPlane);
            sprintf(title, "Plane %d : %s",iPlane,tPlane->GetPlanePurpose());
            fListBoxAlignPlanesToMove->AddEntry(title, iPlane);
          }
          fListBoxAlignPlanesToMove->SetMultipleSelections(1);
          fGroupFrameAlignDisplayMove->AddFrame(fListBoxAlignPlanesToMove, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
          fListBoxAlignPlanesToMove->MoveResize(pos_AL_CO_MO_X,pos_AL_CO_MO_Y+=1.5*pos_AL_deltaY,pos_AL_CO_DI_Xlength,150);

          pos_AL_CO_MO_Y += 150;

        sizeYfGroupFrameAlignDisplayMove = (sizeYfGroupFrameAlignDisplayMove < pos_AL_CO_MO_Y ) ? pos_AL_CO_MO_Y : sizeYfGroupFrameAlignDisplayMove;

        // Translation zone
        Int_t pos_AL_CO_TR_X = pos_AL_CO_MO_X + pos_AL_CO_DI_Xlength + 10;
        Int_t pos_AL_CO_TR_Y=18;
        Int_t pos_AL_CO_TR_Xlength=120;


        TGLabel *fLabelZoneAlTransl = new TGLabel(fGroupFrameAlignDisplayMove,"Translate ...");
        fLabelZoneAlTransl->SetTextJustify(36);
        fLabelZoneAlTransl->SetMargins(0,0,0,0);
        fLabelZoneAlTransl->SetWrapLength(-1);
        fGroupFrameAlignDisplayMove->AddFrame(fLabelZoneAlTransl, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneAlTransl->MoveResize(pos_AL_CO_TR_X,pos_AL_CO_TR_Y,pos_AL_CO_TR_Xlength,18);

          TGLabel *fLabelAliCoTranX = new TGLabel(fGroupFrameAlignDisplayMove,"along X axis [mm]");
          fLabelAliCoTranX->SetTextJustify(36);
          fLabelAliCoTranX->SetMargins(0,0,0,0);
          fLabelAliCoTranX->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTranX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTranX->MoveResize(pos_AL_CO_TR_X,pos_AL_CO_TR_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TR_Xlength,18);

          fNumberEntry_AlignTranslatX = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTranslatX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTranslatX->MoveResize(pos_AL_CO_TR_X+10,pos_AL_CO_TR_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TR_Xlength-20,25);

          TGLabel *fLabelAliCoTranY = new TGLabel(fGroupFrameAlignDisplayMove,"along Y axis [mm]");
          fLabelAliCoTranY->SetTextJustify(36);
          fLabelAliCoTranY->SetMargins(0,0,0,0);
          fLabelAliCoTranY->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTranY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTranY->MoveResize(pos_AL_CO_TR_X,pos_AL_CO_TR_Y+=2*pos_AL_deltaY,pos_AL_CO_TR_Xlength,18);

          fNumberEntry_AlignTranslatY = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTranslatY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTranslatY->MoveResize(pos_AL_CO_TR_X+10,pos_AL_CO_TR_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TR_Xlength-20,25);

          TGLabel *fLabelAliCoTranZ = new TGLabel(fGroupFrameAlignDisplayMove,"along Z axis [mm]");
          fLabelAliCoTranZ->SetTextJustify(36);
          fLabelAliCoTranZ->SetMargins(0,0,0,0);
          fLabelAliCoTranZ->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTranZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTranZ->MoveResize(pos_AL_CO_TR_X,pos_AL_CO_TR_Y+=2*pos_AL_deltaY,pos_AL_CO_TR_Xlength,18);

          fNumberEntry_AlignTranslatZ = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealOne,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTranslatZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTranslatZ->MoveResize(pos_AL_CO_TR_X+10,pos_AL_CO_TR_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TR_Xlength-20,25);

          pos_AL_CO_TR_Y+=25;

        sizeYfGroupFrameAlignDisplayMove = (sizeYfGroupFrameAlignDisplayMove < pos_AL_CO_TR_Y) ? pos_AL_CO_TR_Y : sizeYfGroupFrameAlignDisplayMove;

        // Tilts zone
        Int_t pos_AL_CO_TU_X = pos_AL_CO_TR_X + pos_AL_CO_TR_Xlength + 10;
        Int_t pos_AL_CO_TU_Y=18;
        Int_t pos_AL_CO_TU_Xlength=120;

        TGLabel *fLabelZoneAlTurn = new TGLabel(fGroupFrameAlignDisplayMove,"Turn ...");
        fLabelZoneAlTurn->SetTextJustify(36);
        fLabelZoneAlTurn->SetMargins(0,0,0,0);
        fLabelZoneAlTurn->SetWrapLength(-1);
        fGroupFrameAlignDisplayMove->AddFrame(fLabelZoneAlTurn, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneAlTurn->MoveResize(pos_AL_CO_TU_X,pos_AL_CO_TU_Y,pos_AL_CO_TU_Xlength,18);

          TGLabel *fLabelAliCoTurnX = new TGLabel(fGroupFrameAlignDisplayMove,"around X axis [deg]");
          fLabelAliCoTurnX->SetTextJustify(36);
          fLabelAliCoTurnX->SetMargins(0,0,0,0);
          fLabelAliCoTurnX->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTurnX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTurnX->MoveResize(pos_AL_CO_TU_X,pos_AL_CO_TU_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TU_Xlength,18);

          fNumberEntry_AlignTurnX = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTurnX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTurnX->MoveResize(pos_AL_CO_TU_X+10,pos_AL_CO_TU_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TU_Xlength-20,25);

          TGLabel *fLabelAliCoTurnY = new TGLabel(fGroupFrameAlignDisplayMove,"around Y axis [deg]");
          fLabelAliCoTurnY->SetTextJustify(36);
          fLabelAliCoTurnY->SetMargins(0,0,0,0);
          fLabelAliCoTurnY->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTurnY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTurnY->MoveResize(pos_AL_CO_TU_X,pos_AL_CO_TU_Y+=2*pos_AL_deltaY,pos_AL_CO_TU_Xlength,18);

          fNumberEntry_AlignTurnY = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTurnY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTurnY->MoveResize(pos_AL_CO_TU_X+10,pos_AL_CO_TU_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TU_Xlength-20,25);

          TGLabel *fLabelAliCoTurnZ = new TGLabel(fGroupFrameAlignDisplayMove,"around Z axis [deg]");
          fLabelAliCoTurnZ->SetTextJustify(36);
          fLabelAliCoTurnZ->SetMargins(0,0,0,0);
          fLabelAliCoTurnZ->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTurnZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTurnZ->MoveResize(pos_AL_CO_TU_X,pos_AL_CO_TU_Y+=2*pos_AL_deltaY,pos_AL_CO_TU_Xlength,18);

          fNumberEntry_AlignTurnZ = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTurnZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTurnZ->MoveResize(pos_AL_CO_TU_X+10,pos_AL_CO_TU_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TU_Xlength-20,25);

          pos_AL_CO_TU_Y+=25;

        sizeYfGroupFrameAlignDisplayMove = (sizeYfGroupFrameAlignDisplayMove < pos_AL_CO_TU_Y) ? pos_AL_CO_TU_Y : sizeYfGroupFrameAlignDisplayMove;

        // Position zone
        Int_t pos_AL_CO_PO_X = pos_AL_CO_TU_X + pos_AL_CO_TU_Xlength + 10;
        Int_t pos_AL_CO_PO_Y=18;
        Int_t pos_AL_CO_PO_Xlength=120;


        TGLabel *fLabelZoneAlPosition = new TGLabel(fGroupFrameAlignDisplayMove,"actual positions");
        fLabelZoneAlPosition->SetTextJustify(36);
        fLabelZoneAlPosition->SetMargins(0,0,0,0);
        fLabelZoneAlPosition->SetWrapLength(-1);
        fGroupFrameAlignDisplayMove->AddFrame(fLabelZoneAlPosition, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneAlPosition->MoveResize(pos_AL_CO_PO_X,pos_AL_CO_PO_Y,pos_AL_CO_PO_Xlength,18);

          TGLabel *fLabelAliCoPosX = new TGLabel(fGroupFrameAlignDisplayMove,"along X axis [mm]");
          fLabelAliCoPosX->SetTextJustify(36);
          fLabelAliCoPosX->SetMargins(0,0,0,0);
          fLabelAliCoPosX->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoPosX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoPosX->MoveResize(pos_AL_CO_PO_X,pos_AL_CO_PO_Y+=1.5*pos_AL_deltaY,pos_AL_CO_PO_Xlength,18);

          fNumberEntry_AlignPosX = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealThree,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignPosX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignPosX->MoveResize(pos_AL_CO_PO_X+10,pos_AL_CO_PO_Y+=1.5*pos_AL_deltaY,pos_AL_CO_PO_Xlength-20,25);

          TGLabel *fLabelAliCoPosY = new TGLabel(fGroupFrameAlignDisplayMove,"along Y axis [mm]");
          fLabelAliCoPosY->SetTextJustify(36);
          fLabelAliCoPosY->SetMargins(0,0,0,0);
          fLabelAliCoPosY->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoPosY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoPosY->MoveResize(pos_AL_CO_PO_X,pos_AL_CO_PO_Y+=2*pos_AL_deltaY,pos_AL_CO_PO_Xlength,18);

          fNumberEntry_AlignPosY = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealThree,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignPosY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignPosY->MoveResize(pos_AL_CO_PO_X+10,pos_AL_CO_PO_Y+=1.5*pos_AL_deltaY,pos_AL_CO_PO_Xlength-20,25);

          TGLabel *fLabelAliCoPosZ = new TGLabel(fGroupFrameAlignDisplayMove,"along Z axis [mm]");
          fLabelAliCoPosZ->SetTextJustify(36);
          fLabelAliCoPosZ->SetMargins(0,0,0,0);
          fLabelAliCoPosZ->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoPosZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoPosZ->MoveResize(pos_AL_CO_PO_X,pos_AL_CO_PO_Y+=2*pos_AL_deltaY,pos_AL_CO_PO_Xlength,18);

          fNumberEntry_AlignPosZ = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealThree,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignPosZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignPosZ->MoveResize(pos_AL_CO_PO_X+10,pos_AL_CO_PO_Y+=1.5*pos_AL_deltaY,pos_AL_CO_PO_Xlength-20,25);

          pos_AL_CO_PO_Y+=25;

        sizeYfGroupFrameAlignDisplayMove = (sizeYfGroupFrameAlignDisplayMove < pos_AL_CO_PO_Y) ? pos_AL_CO_PO_Y : sizeYfGroupFrameAlignDisplayMove;

        // Tilts zone
        Int_t pos_AL_CO_TI_X = pos_AL_CO_PO_X + pos_AL_CO_PO_Xlength + 10;
        Int_t pos_AL_CO_TI_Y=18;
        Int_t pos_AL_CO_TI_Xlength=120;

        TGLabel *fLabelZoneAlTilts = new TGLabel(fGroupFrameAlignDisplayMove,"actual tilts");
        fLabelZoneAlTilts->SetTextJustify(36);
        fLabelZoneAlTilts->SetMargins(0,0,0,0);
        fLabelZoneAlTilts->SetWrapLength(-1);
        fGroupFrameAlignDisplayMove->AddFrame(fLabelZoneAlTilts, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelZoneAlTilts->MoveResize(pos_AL_CO_TI_X,pos_AL_CO_TI_Y,pos_AL_CO_TI_Xlength,18);

          TGLabel *fLabelAliCoTiltX = new TGLabel(fGroupFrameAlignDisplayMove,"around X axis [deg]");
          fLabelAliCoTiltX->SetTextJustify(36);
          fLabelAliCoTiltX->SetMargins(0,0,0,0);
          fLabelAliCoTiltX->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTiltX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTiltX->MoveResize(pos_AL_CO_TI_X,pos_AL_CO_TI_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TI_Xlength,18);

          fNumberEntry_AlignTiltX = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealThree,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTiltX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTiltX->MoveResize(pos_AL_CO_TI_X+10,pos_AL_CO_TI_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TI_Xlength-20,25);

          TGLabel *fLabelAliCoTiltY = new TGLabel(fGroupFrameAlignDisplayMove,"around Y axis [deg]");
          fLabelAliCoTiltY->SetTextJustify(36);
          fLabelAliCoTiltY->SetMargins(0,0,0,0);
          fLabelAliCoTiltY->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTiltY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTiltY->MoveResize(pos_AL_CO_TI_X,pos_AL_CO_TI_Y+=2*pos_AL_deltaY,pos_AL_CO_TI_Xlength,18);

          fNumberEntry_AlignTiltY = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealThree,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTiltY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTiltY->MoveResize(pos_AL_CO_TI_X+10,pos_AL_CO_TI_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TI_Xlength-20,25);

          TGLabel *fLabelAliCoTiltZ = new TGLabel(fGroupFrameAlignDisplayMove,"around Z axis [deg]");
          fLabelAliCoTiltZ->SetTextJustify(36);
          fLabelAliCoTiltZ->SetMargins(0,0,0,0);
          fLabelAliCoTiltZ->SetWrapLength(-1);
          fGroupFrameAlignDisplayMove->AddFrame(fLabelAliCoTiltZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fLabelAliCoTiltZ->MoveResize(pos_AL_CO_TI_X,pos_AL_CO_TI_Y+=2*pos_AL_deltaY,pos_AL_CO_TI_Xlength,18);

          fNumberEntry_AlignTiltZ = new TGNumberEntry(fGroupFrameAlignDisplayMove,0, 9,999, TGNumberFormat::kNESRealThree,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELNoLimits);
          fGroupFrameAlignDisplayMove->AddFrame(fNumberEntry_AlignTiltZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_AlignTiltZ->MoveResize(pos_AL_CO_TI_X+10,pos_AL_CO_TI_Y+=1.5*pos_AL_deltaY,pos_AL_CO_TI_Xlength-20,25);

          pos_AL_CO_TI_Y+=25;

        sizeYfGroupFrameAlignDisplayMove = (sizeYfGroupFrameAlignDisplayMove < pos_AL_CO_TI_Y) ? pos_AL_CO_TI_Y : sizeYfGroupFrameAlignDisplayMove;

        // Separation 1
        TGVertical3DLine *fVertical3DLine1359 = new TGVertical3DLine(fGroupFrameAlignDisplayMove,8,200);
        fGroupFrameAlignDisplayMove->AddFrame(fVertical3DLine1359, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1359->MoveResize(pos_AL_CO_MO_X-5,16,4,sizeYfGroupFrameAlignDisplayMove-10);

        // Separation 2
        TGVertical3DLine *fVertical3DLine1360 = new TGVertical3DLine(fGroupFrameAlignDisplayMove,8,200);
        fGroupFrameAlignDisplayMove->AddFrame(fVertical3DLine1360, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1360->MoveResize(pos_AL_CO_TR_X-5,16,4,sizeYfGroupFrameAlignDisplayMove-10);

        // Separation 3
        TGVertical3DLine *fVertical3DLine1361 = new TGVertical3DLine(fGroupFrameAlignDisplayMove,8,200);
        fGroupFrameAlignDisplayMove->AddFrame(fVertical3DLine1361, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1361->MoveResize(pos_AL_CO_TU_X-5,16,4,sizeYfGroupFrameAlignDisplayMove-10);

        // Separation 4
        TGVertical3DLine *fVertical3DLine1362 = new TGVertical3DLine(fGroupFrameAlignDisplayMove,8,200);
        fGroupFrameAlignDisplayMove->AddFrame(fVertical3DLine1362, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1362->MoveResize(pos_AL_CO_PO_X-5,16,4,sizeYfGroupFrameAlignDisplayMove-10);

        // Separation 5
        TGVertical3DLine *fVertical3DLine1365 = new TGVertical3DLine(fGroupFrameAlignDisplayMove,8,200);
        fGroupFrameAlignDisplayMove->AddFrame(fVertical3DLine1365, new TGLayoutHints(kLHintsNormal));
        fVertical3DLine1365->MoveResize(pos_AL_CO_TI_X-5,16,4,sizeYfGroupFrameAlignDisplayMove-10);

      sizeYfGroupFrameAlignDisplayMove+= 20;
      fGroupFrameAlignDisplayMove->SetLayoutManager(new TGVerticalLayout(fGroupFrameAlignDisplayMove));
      fGroupFrameAlignDisplayMove->Resize(872,sizeYfGroupFrameAlignDisplayMove);
      fCompositeFrameTabAlignment->AddFrame(fGroupFrameAlignDisplayMove, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


      // "Control" group frame
      TGGroupFrame *fGroupFrameAlignControl = new TGGroupFrame(fCompositeFrameTabAlignment,"Alignement Control");
      fGroupFrameAlignControl->SetLayoutBroken(kTRUE);
      Int_t sizeYfGroupFrameAlignControl = 0 ;

      Int_t pos_AL_AC_X = 5 ;
      Int_t pos_AL_AC_Y = 18 ;
      Int_t pos_AL_AC_deltaY = 30;

      //Enable View
      fTextButtonAlignViewOverHits= new TGCheckButton(fGroupFrameAlignControl,"Overlay Hits View");
      fTextButtonAlignViewOverHits->SetTextJustify(36);
      fTextButtonAlignViewOverHits->SetMargins(0,0,0,0);
      fTextButtonAlignViewOverHits->SetWrapLength(-1);
      fGroupFrameAlignControl->AddFrame(fTextButtonAlignViewOverHits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      fTextButtonAlignViewOverHits->MoveResize(pos_AL_AC_X,pos_AL_AC_Y ,150,19);
      
        fTextButtonAlignViewOverHits_Cumul= new TGCheckButton(fGroupFrameAlignControl,"Cumul");
        fTextButtonAlignViewOverHits_Cumul->SetTextJustify(36);
        fTextButtonAlignViewOverHits_Cumul->SetMargins(0,0,0,0);
        fTextButtonAlignViewOverHits_Cumul->SetWrapLength(-1);
        fGroupFrameAlignControl->AddFrame(fTextButtonAlignViewOverHits_Cumul, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextButtonAlignViewOverHits_Cumul->MoveResize(pos_AL_AC_X+20,pos_AL_AC_Y+20 ,150,19);
        pos_AL_AC_X += 130 ;

      fTextButtonAlignResetValues = new TGTextButton(fGroupFrameAlignControl,"Reset values");
      fTextButtonAlignResetValues->SetTextJustify(36);
      fTextButtonAlignResetValues->SetMargins(0,0,0,0);
      fTextButtonAlignResetValues->SetWrapLength(-1);
      fTextButtonAlignResetValues->Resize(130,25);
      fGroupFrameAlignControl->AddFrame(fTextButtonAlignResetValues, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      fTextButtonAlignResetValues->MoveResize(pos_AL_AC_X,pos_AL_AC_Y,130,25);
      pos_AL_AC_X += 130 ;

      fTextButtonAlignDisplayActualPosTilt = new TGTextButton(fGroupFrameAlignControl,"View Pos / Tilt");
      fTextButtonAlignDisplayActualPosTilt->SetTextJustify(36);
      fTextButtonAlignDisplayActualPosTilt->SetMargins(0,0,0,0);
      fTextButtonAlignDisplayActualPosTilt->SetWrapLength(-1);
      fTextButtonAlignDisplayActualPosTilt->Resize(130,25);
      fGroupFrameAlignControl->AddFrame(fTextButtonAlignDisplayActualPosTilt, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      fTextButtonAlignDisplayActualPosTilt->MoveResize(pos_AL_AC_X,pos_AL_AC_Y,130,25);
      pos_AL_AC_X += 140 ;


      TGLabel *fLabelAlignUpdate = new TGLabel(fGroupFrameAlignControl,"Update CURRENT event :");
      fLabelAlignUpdate->SetTextJustify(36);
      fLabelAlignUpdate->SetMargins(0,0,0,0);
      fLabelAlignUpdate->SetWrapLength(-1);
      fGroupFrameAlignControl->AddFrame(fLabelAlignUpdate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      fLabelAlignUpdate->MoveResize(pos_AL_AC_X,pos_AL_AC_Y,140,25);
      pos_AL_AC_X += 145 ;

      fTextButtonAlignDoTranslatTurn = new TGTextButton(fGroupFrameAlignControl,"Translate / Turn");
      fTextButtonAlignDoTranslatTurn->SetTextJustify(36);
      fTextButtonAlignDoTranslatTurn->SetMargins(0,0,0,0);
      fTextButtonAlignDoTranslatTurn->SetWrapLength(-1);
      fTextButtonAlignDoTranslatTurn->Resize(130,25);
      fGroupFrameAlignControl->AddFrame(fTextButtonAlignDoTranslatTurn, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      fTextButtonAlignDoTranslatTurn->MoveResize(pos_AL_AC_X,pos_AL_AC_Y,130,25);
      pos_AL_AC_X += 130 ;


      fTextButtonAlignUpdateActualPosTilt = new TGTextButton(fGroupFrameAlignControl,"Update Pos / Tilt");
      fTextButtonAlignUpdateActualPosTilt->SetTextJustify(36);
      fTextButtonAlignUpdateActualPosTilt->SetMargins(0,0,0,0);
      fTextButtonAlignUpdateActualPosTilt->SetWrapLength(-1);
      fTextButtonAlignUpdateActualPosTilt->Resize(130,25);
      fGroupFrameAlignControl->AddFrame(fTextButtonAlignUpdateActualPosTilt, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      fTextButtonAlignUpdateActualPosTilt->MoveResize(pos_AL_AC_X,pos_AL_AC_Y,130,25);

      sizeYfGroupFrameAlignControl = pos_AL_AC_Y+=pos_AL_AC_deltaY;


      pos_AL_AC_X = 5 ;
      pos_AL_AC_Y = 30 +pos_AL_AC_deltaY ;

      fTextButtonAlignUpdateCfgFile = new TGTextButton(fGroupFrameAlignControl,"Write config file");
      fTextButtonAlignUpdateCfgFile->SetTextJustify(36);
      fTextButtonAlignUpdateCfgFile->SetMargins(0,0,0,0);
      fTextButtonAlignUpdateCfgFile->SetWrapLength(-1);
      fTextButtonAlignUpdateCfgFile->Resize(130,25);
      fGroupFrameAlignControl->AddFrame(fTextButtonAlignUpdateCfgFile, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      fTextButtonAlignUpdateCfgFile->MoveResize(pos_AL_AC_X,pos_AL_AC_Y+25,130,25);
      pos_AL_AC_X += 142 ;
      
        fTextButtonAlignUpdateCfgFile_UpdateMode = new TGButtonGroup(fGroupFrameAlignControl, "Write mode");                 
        fTextButtonAlignUpdateCfgFile_UpdateMode->SetTitlePos(TGGroupFrame::kCenter);             
        fGRadioButton_AnalyseModeUpWithBck =new TGRadioButton(fTextButtonAlignUpdateCfgFile_UpdateMode, "Backup and update current cfg file", 0);              
        fGRadioButton_AnalyseModeUpNoBck   =new TGRadioButton(fTextButtonAlignUpdateCfgFile_UpdateMode, "Only update current cfg file", 0);  
        fGRadioButton_AnalyseModeNewFile   =new TGRadioButton(fTextButtonAlignUpdateCfgFile_UpdateMode, "Create new cfg file  --------->", 0);
        fTextButtonAlignUpdateCfgFile_UpdateMode->SetButton(0);                   
        fTextButtonAlignUpdateCfgFile_UpdateMode->MoveResize(pos_AL_AC_X,pos_AL_AC_Y-5);                 
        fGroupFrameAlignControl->AddFrame(fTextButtonAlignUpdateCfgFile_UpdateMode, new TGLayoutHints(kLHintsExpandX)); 
        
        TString temp = fSession->GetConfigFileName();
        temp.Remove(temp.Sizeof()-5,4); //Remove extension
        temp +="_ManuAlign_";
        TDatime currentDateTime;
        temp += currentDateTime.GetDate();
        temp += "-";
        temp += currentDateTime.GetHour();
        temp += "-";
        temp += currentDateTime.GetMinute();
        temp += "-";
        temp += currentDateTime.GetSecond();
                
        fTextEntry_NewCfgFileName = new TGTextEntry(fGroupFrameAlignControl,temp.Data());
        fGroupFrameAlignControl->AddFrame(fTextEntry_NewCfgFileName, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextEntry_NewCfgFileName->MoveResize(pos_AL_AC_X+250,pos_AL_AC_Y+40,300,25);

        TGLabel *fLabelCfgExt = new TGLabel(fGroupFrameAlignControl,".cfg");
        fLabelCfgExt->SetTextJustify(36);
        fLabelCfgExt->SetMargins(0,0,0,0);
        fLabelCfgExt->SetWrapLength(-1);
        fGroupFrameAlignControl->AddFrame(fLabelCfgExt, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelCfgExt->MoveResize(pos_AL_AC_X+250+300,pos_AL_AC_Y+40,20,25);
        pos_AL_AC_X += 145 ;
        
        
        pos_AL_AC_Y += 50 ;
        
      sizeYfGroupFrameAlignControl = pos_AL_AC_Y+=pos_AL_AC_deltaY;

      sizeYfGroupFrameAlignControl+= 20;
      fGroupFrameAlignControl->SetLayoutManager(new TGVerticalLayout(fGroupFrameAlignControl));
      fGroupFrameAlignControl->Resize(872,sizeYfGroupFrameAlignControl);
      fCompositeFrameTabAlignment->AddFrame(fGroupFrameAlignControl, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


    sizeYfCompositeFrameTabAlignment = sizeYfGroupFrameAlignDisplayMove + sizeYfGroupFrameAlignControl + 10;
    
//#############################################################################################################
  sizeYfTabTop = TMath::Max( sizeYfCompositeFrameTabAnalysis,sizeYfCompositeFrameTabNoise);
  sizeYfTabTop = TMath::Max( sizeYfTabTop , sizeYfCompositeFrameTabAlignment );
  sizeYfTabTop += 20;
  fTabTop->SetTab(0);
  fTabTop->Resize(fTabTop->GetDefaultSize());
  fMainFrame->AddFrame(fTabTop, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTabTop->MoveResize(2,2,length_X,sizeYfTabTop);

//#############################################################################################################
//#############################################################################################################
//                                      BOTTOM TAB
//#############################################################################################################
//#############################################################################################################

//*********************************************************************************************************  
//                           container of Tab "Analysis control"
//*********************************************************************************************************
  TGTab *fTabBottom = new TGTab(fMainFrame,length_X,464);
  Int_t sizeYfTabBottom=0;

    // container of "Analysis control"
    TGCompositeFrame *fCompositeFrameTabAnalysisControl;
    fCompositeFrameTabAnalysisControl = fTabBottom->AddTab("Analysis control");
    fCompositeFrameTabAnalysisControl->SetLayoutManager(new TGVerticalLayout(fCompositeFrameTabAnalysisControl));
    Int_t sizeYfCompositeFrameTabAnalysisControl=0;
    Int_t sizeXfCompositeFrameTabAnalysisControl=0;

       // "Analysis control" group frame
      //Int_t pos_AC_Ybottom;
      TGGroupFrame *fGroupFrameAnalysisControl = new TGGroupFrame(fCompositeFrameTabAnalysisControl,"Analysis control");
      fGroupFrameAnalysisControl->SetLayoutBroken(kTRUE);

        // "Process Events" zone
        Int_t pos_AC_PE_X=5;
        Int_t pos_AC_PE_Y=18;
        Int_t pos_AC_deltaY=25;

        TGLabel *fLabelEvents = new TGLabel(fGroupFrameAnalysisControl,"Process Events");
        fLabelEvents->SetTextJustify(36);
        fLabelEvents->SetMargins(0,0,0,0);
        fLabelEvents->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelEvents->MoveResize(pos_AC_PE_X+11,pos_AC_PE_Y,100,18);

          fTextButtonProcess1event = new TGTextButton(fGroupFrameAnalysisControl,"Process 1 event");
          fTextButtonProcess1event->SetTextJustify(36);
          fTextButtonProcess1event->SetMargins(0,0,0,0);
          fTextButtonProcess1event->SetWrapLength(-1);
          fTextButtonProcess1event->Resize(130,25);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonProcess1event, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonProcess1event->MoveResize(pos_AC_PE_X,pos_AC_PE_Y+=1*pos_AC_deltaY,130,25);

          fTextButtonProcess10events = new TGTextButton(fGroupFrameAnalysisControl,"Process 10 events");
          fTextButtonProcess10events->SetTextJustify(36);
          fTextButtonProcess10events->SetMargins(0,0,0,0);
          fTextButtonProcess10events->SetWrapLength(-1);
          fTextButtonProcess10events->Resize(130,25);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonProcess10events, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonProcess10events->MoveResize(pos_AC_PE_X,pos_AC_PE_Y+=pos_AC_deltaY,130,25);

          fTextButtonProcess100events = new TGTextButton(fGroupFrameAnalysisControl,"Process 100 events");
          fTextButtonProcess100events->SetTextJustify(36);
          fTextButtonProcess100events->SetMargins(0,0,0,0);
          fTextButtonProcess100events->SetWrapLength(-1);
          fTextButtonProcess100events->Resize(130,25);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonProcess100events, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonProcess100events->MoveResize(pos_AC_PE_X,pos_AC_PE_Y+=pos_AC_deltaY,130,25);

          fNumberEntry_EventsToProcess = new TGNumberEntry(fGroupFrameAnalysisControl,1000, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 1000000);
          fGroupFrameAnalysisControl->AddFrame(fNumberEntry_EventsToProcess, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_EventsToProcess->MoveResize(pos_AC_PE_X,pos_AC_PE_Y+=pos_AC_deltaY,75,25);

          fTextButtonProcessXevents = new TGTextButton(fGroupFrameAnalysisControl,"Process");
          fTextButtonProcessXevents->SetTextJustify(36);
          fTextButtonProcessXevents->SetMargins(0,0,0,0);
          fTextButtonProcessXevents->SetWrapLength(-1);
          fTextButtonProcessXevents->Resize(47,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonProcessXevents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonProcessXevents->MoveResize(pos_AC_PE_X+78,pos_AC_PE_Y,52,24);

          fTextButtonProcessAllEvents = new TGTextButton(fGroupFrameAnalysisControl,"Process ALL events");
          fTextButtonProcessAllEvents->SetTextJustify(36);
          fTextButtonProcessAllEvents->SetMargins(0,0,0,0);
          fTextButtonProcessAllEvents->SetWrapLength(-1);
          fTextButtonProcessAllEvents->Resize(130,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonProcessAllEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonProcessAllEvents->MoveResize(pos_AC_PE_X,pos_AC_PE_Y+=pos_AC_deltaY,130,24);

          pos_AC_PE_Y+=30;

        if (pos_AC_PE_Y > sizeYfCompositeFrameTabAnalysisControl) sizeYfCompositeFrameTabAnalysisControl = pos_AC_PE_Y ;

        // Go to ...  zone
        Int_t pos_AC_GT_X=155;
        Int_t pos_AC_GT_Y=18;

        // Action Label
        TGLabel *fLabelGoTo = new TGLabel(fGroupFrameAnalysisControl,"Go to event with at least...");
        fLabelGoTo->SetTextJustify(36);
        fLabelGoTo->SetMargins(0,0,0,0);
        fLabelGoTo->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelGoTo, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelGoTo->MoveResize(pos_AC_GT_X,pos_AC_GT_Y,145,18);

          fTextButtonGoToEventWithNhits = new TGTextButton(fGroupFrameAnalysisControl,"hit(s)");
          fTextButtonGoToEventWithNhits->SetTextJustify(36);
          fTextButtonGoToEventWithNhits->SetMargins(0,0,0,0);
          fTextButtonGoToEventWithNhits->SetWrapLength(-1);
          fTextButtonGoToEventWithNhits->Resize(130,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonGoToEventWithNhits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonGoToEventWithNhits->MoveResize(pos_AC_GT_X+80,pos_AC_GT_Y+=pos_AC_deltaY,60,24);
          
          fNumberEntryGoToEventWithNhits = new TGNumberEntry(fGroupFrameAnalysisControl,1, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 1000);
          fGroupFrameAnalysisControl->AddFrame(fNumberEntryGoToEventWithNhits, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntryGoToEventWithNhits->MoveResize(pos_AC_GT_X,pos_AC_GT_Y,80,25);

          fTextButtonGoToEventWithNtracks = new TGTextButton(fGroupFrameAnalysisControl,"track(s)");
          fTextButtonGoToEventWithNtracks->SetTextJustify(36);
          fTextButtonGoToEventWithNtracks->SetMargins(0,0,0,0);
          fTextButtonGoToEventWithNtracks->SetWrapLength(-1);
          fTextButtonGoToEventWithNtracks->Resize(130,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonGoToEventWithNtracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonGoToEventWithNtracks->MoveResize(pos_AC_GT_X+80,pos_AC_GT_Y+=pos_AC_deltaY,60,24);
          
          fNumberEntryGoToEventWithNtracks = new TGNumberEntry(fGroupFrameAnalysisControl,1, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 1000);
          fGroupFrameAnalysisControl->AddFrame(fNumberEntryGoToEventWithNtracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntryGoToEventWithNtracks->MoveResize(pos_AC_GT_X,pos_AC_GT_Y,80,25);          

          
        TGLabel *fLabelGoTo2 = new TGLabel(fGroupFrameAnalysisControl,"Go to event");
        fLabelGoTo2->SetTextJustify(36);
        fLabelGoTo2->SetMargins(0,0,0,0);
        fLabelGoTo2->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelGoTo2, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelGoTo2->MoveResize(pos_AC_GT_X,pos_AC_GT_Y+=2*pos_AC_deltaY,145,18);
        
          fTextButtonGoToEventNb = new TGTextButton(fGroupFrameAnalysisControl,"number:");
          fTextButtonGoToEventNb->SetTextJustify(36);
          fTextButtonGoToEventNb->SetMargins(0,0,0,0);
          fTextButtonGoToEventNb->SetWrapLength(-1);
          fTextButtonGoToEventNb->Resize(60,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonGoToEventNb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonGoToEventNb->MoveResize(pos_AC_GT_X,pos_AC_GT_Y+=pos_AC_deltaY,60,24);
          
          fNumberEntry_GoToEvent = new TGNumberEntry(fGroupFrameAnalysisControl,1000, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0, 1000000);
          fGroupFrameAnalysisControl->AddFrame(fNumberEntry_GoToEvent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntry_GoToEvent->MoveResize(pos_AC_GT_X+61,pos_AC_GT_Y,82,25);
                   
          
          
          pos_AC_GT_Y+=24;

        if (pos_AC_GT_Y > sizeYfCompositeFrameTabAnalysisControl) sizeYfCompositeFrameTabAnalysisControl = pos_AC_GT_Y ;


        // Analyse zone
        Int_t pos_AC_SC_X=pos_AC_GT_X*2;
        Int_t pos_AC_SC_Y=18;

        // Label
        TGLabel *fLabelDisplayZone = new TGLabel(fGroupFrameAnalysisControl,"Display");
        fLabelDisplayZone->SetTextJustify(36);
        fLabelDisplayZone->SetMargins(0,0,0,0);
        fLabelDisplayZone->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelDisplayZone, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelDisplayZone->MoveResize(pos_AC_SC_X+10,pos_AC_SC_Y,110,18);


        TGLabel *fLabelDisplayRefreshEvery = new TGLabel(fGroupFrameAnalysisControl,"Refr. every");
        fLabelDisplayRefreshEvery->SetTextJustify(36);
        fLabelDisplayRefreshEvery->SetMargins(0,0,0,0);
        fLabelDisplayRefreshEvery->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelDisplayRefreshEvery, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelDisplayRefreshEvery->MoveResize(pos_AC_SC_X,pos_AC_SC_Y+=pos_AC_deltaY,60,18);

        fNumberEntry_RefreshFreq = new TGNumberEntry(fGroupFrameAnalysisControl,100000, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-1, 1000000);
        fGroupFrameAnalysisControl->AddFrame(fNumberEntry_RefreshFreq, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fNumberEntry_RefreshFreq->MoveResize(pos_AC_SC_X+70,pos_AC_SC_Y,75,25);

        
        fTextButtonRefreshView = new TGTextButton(fGroupFrameAnalysisControl,"Refresh view(s)");
        fTextButtonRefreshView->SetTextJustify(36);
        fTextButtonRefreshView->SetMargins(0,0,0,0);
        fTextButtonRefreshView->SetWrapLength(-1);
        fTextButtonRefreshView->Resize(130,24);
        fGroupFrameAnalysisControl->AddFrame(fTextButtonRefreshView, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextButtonRefreshView->MoveResize(pos_AC_SC_X,pos_AC_SC_Y+=1*pos_AC_deltaY,130,24);
        
        fTextButtonResetView = new TGTextButton(fGroupFrameAnalysisControl,"Reset view(s)");
        fTextButtonResetView->SetTextJustify(36);
        fTextButtonResetView->SetMargins(0,0,0,0);
        fTextButtonResetView->SetWrapLength(-1);
        fTextButtonResetView->Resize(130,24);
        fGroupFrameAnalysisControl->AddFrame(fTextButtonResetView, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextButtonResetView->MoveResize(pos_AC_SC_X,pos_AC_SC_Y+=1*pos_AC_deltaY,130,24);
        
        fTextButtonSaveDisplays = new TGTextButton(fGroupFrameAnalysisControl,"Save Displays -->");
        fTextButtonSaveDisplays->SetTextJustify(36);
        fTextButtonSaveDisplays->SetMargins(0,0,0,0);
        fTextButtonSaveDisplays->SetWrapLength(-1);
        fTextButtonSaveDisplays->Resize(130,24);
        fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveDisplays, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextButtonSaveDisplays->MoveResize(pos_AC_SC_X,pos_AC_SC_Y+=pos_AC_deltaY,130,24);
        
        fTextButtonSaveHistograms = new TGTextButton(fGroupFrameAnalysisControl,"Save Histograms -->");
        fTextButtonSaveHistograms->SetTextJustify(36);
        fTextButtonSaveHistograms->SetMargins(0,0,0,0);
        fTextButtonSaveHistograms->SetWrapLength(-1);
        fTextButtonSaveHistograms->Resize(130,24);
        fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveHistograms, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextButtonSaveHistograms->MoveResize(pos_AC_SC_X,pos_AC_SC_Y+=pos_AC_deltaY,130,24);

        if (pos_AC_SC_Y > sizeYfCompositeFrameTabAnalysisControl) sizeYfCompositeFrameTabAnalysisControl = pos_AC_SC_Y ;

        
        
        // Save zone
        Int_t pos_AC_SA_X = pos_AC_GT_X*3;
        Int_t pos_AC_SA_Y = 18;
        Int_t pos_AC_SA_Y2;
        Int_t pos_AC_SA_X2 = pos_AC_GT_X*3.6;

        // Label
        TGLabel *fLabelSaveResultsZone = new TGLabel(fGroupFrameAnalysisControl,"Save");
        fLabelSaveResultsZone->SetTextJustify(36);
        fLabelSaveResultsZone->SetMargins(0,0,0,0);
        fLabelSaveResultsZone->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelSaveResultsZone, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelSaveResultsZone->MoveResize(pos_AC_SA_X+30,pos_AC_SA_Y,110,18);

        TGLabel *fLabelSaveResultsPrefixZone = new TGLabel(fGroupFrameAnalysisControl,"Files prefix");
        fLabelSaveResultsPrefixZone->SetTextJustify(36);
        fLabelSaveResultsPrefixZone->SetMargins(0,0,0,0);
        fLabelSaveResultsPrefixZone->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelSaveResultsPrefixZone, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelSaveResultsPrefixZone->MoveResize(pos_AC_SA_X,pos_AC_SA_Y+=pos_AC_deltaY,60,18);

        fTextEntry_FilesPrefix = new TGTextEntry(fGroupFrameAnalysisControl,FileNamePrefix.Data()); 
        fGroupFrameAnalysisControl->AddFrame(fTextEntry_FilesPrefix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextEntry_FilesPrefix->MoveResize(pos_AC_SA_X+65,pos_AC_SA_Y,130,25);
        
        
        TGLabel *fLabelSaveResultsSuffixZone = new TGLabel(fGroupFrameAnalysisControl,"Files suffix");
        fLabelSaveResultsSuffixZone->SetTextJustify(36);
        fLabelSaveResultsSuffixZone->SetMargins(0,0,0,0);
        fLabelSaveResultsSuffixZone->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelSaveResultsSuffixZone, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelSaveResultsSuffixZone->MoveResize(pos_AC_SA_X,pos_AC_SA_Y+=pos_AC_deltaY,60,18);
        

        fTextEntry_FilesSuffix = new TGTextEntry(fGroupFrameAnalysisControl,FileNameSuffix.Data());
        fGroupFrameAnalysisControl->AddFrame(fTextEntry_FilesSuffix, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fTextEntry_FilesSuffix->MoveResize(pos_AC_SA_X+65,pos_AC_SA_Y,130,25);
        
        TGLabel *fLabelSaveCanvas = new TGLabel(fGroupFrameAnalysisControl,"canvas");
        fLabelSaveCanvas->SetTextJustify(36);
        fLabelSaveCanvas->SetMargins(0,0,0,0);
        fLabelSaveCanvas->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelSaveCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelSaveCanvas->MoveResize(pos_AC_SA_X-10,pos_AC_SA_Y2=pos_AC_SA_Y+=1*pos_AC_deltaY,110,18);

          fTextButtonSaveCanvas_inPDF = new TGCheckButton(fGroupFrameAnalysisControl,"*.PDF");
          fTextButtonSaveCanvas_inPDF->SetTextJustify(36);
          fTextButtonSaveCanvas_inPDF->SetMargins(0,0,0,0);
          fTextButtonSaveCanvas_inPDF->SetWrapLength(-1);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveCanvas_inPDF, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonSaveCanvas_inPDF->MoveResize(pos_AC_SA_X,pos_AC_SA_Y+=0.6*pos_AC_deltaY,150,19);
          
          fTextButtonSaveCanvas_inPNG = new TGCheckButton(fGroupFrameAnalysisControl,"*.PNG");
          fTextButtonSaveCanvas_inPNG->SetTextJustify(36);
          fTextButtonSaveCanvas_inPNG->SetMargins(0,0,0,0);
          fTextButtonSaveCanvas_inPNG->SetWrapLength(-1);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveCanvas_inPNG, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonSaveCanvas_inPNG->MoveResize(pos_AC_SA_X,pos_AC_SA_Y+=0.6*pos_AC_deltaY,150,19);

          fTextButtonSaveCanvas_inROOT = new TGCheckButton(fGroupFrameAnalysisControl,"*.ROOT");
          fTextButtonSaveCanvas_inROOT->SetTextJustify(36);
          fTextButtonSaveCanvas_inROOT->SetMargins(0,0,0,0);
          fTextButtonSaveCanvas_inROOT->SetWrapLength(-1);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveCanvas_inROOT, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonSaveCanvas_inROOT->MoveResize(pos_AC_SA_X,pos_AC_SA_Y+=0.6*pos_AC_deltaY,150,19);

        // Save results
        // Label
        TGLabel *fLabelSaveResults = new TGLabel(fGroupFrameAnalysisControl,"results");
        fLabelSaveResults->SetTextJustify(36);
        fLabelSaveResults->SetMargins(0,0,0,0);
        fLabelSaveResults->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelSaveResults, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelSaveResults->MoveResize(pos_AC_SA_X2-10,pos_AC_SA_Y2,110,18);

          fTextButtonSaveTracks = new TGCheckButton(fGroupFrameAnalysisControl,"Tree Tracks");
          fTextButtonSaveTracks->SetTextJustify(36);
          fTextButtonSaveTracks->SetMargins(0,0,0,0);
          fTextButtonSaveTracks->SetWrapLength(-1);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveTracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonSaveTracks->MoveResize(pos_AC_SA_X2,pos_AC_SA_Y2+=0.6*pos_AC_deltaY,100,19);

          fTextButtonSaveVertice = new TGCheckButton(fGroupFrameAnalysisControl,"Tree Vertices");
          fTextButtonSaveVertice->SetTextJustify(36);
          fTextButtonSaveVertice->SetMargins(0,0,0,0);
          fTextButtonSaveVertice->SetWrapLength(-1);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveVertice, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonSaveVertice->MoveResize(pos_AC_SA_X2,pos_AC_SA_Y2+=0.6*pos_AC_deltaY,95,19);
          
                  
          fTextButtonSaveResults = new TGTextButton(fGroupFrameAnalysisControl,"Save");
          fTextButtonSaveResults->SetTextJustify(36);
          fTextButtonSaveResults->SetMargins(0,0,0,0);
          fTextButtonSaveResults->SetWrapLength(-1);
          fTextButtonSaveResults->Resize(130,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonSaveResults, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonSaveResults->MoveResize(pos_AC_SA_X2,pos_AC_SA_Y2+=0.8*pos_AC_deltaY,90,24);
          
          
        pos_AC_SA_Y+=24;

        if (pos_AC_SA_Y > sizeYfCompositeFrameTabAnalysisControl) sizeYfCompositeFrameTabAnalysisControl = pos_AC_SA_Y ;


        // Action zone
        Int_t pos_AC_AC_X=pos_AC_SA_X+(65+130+15);
        Int_t pos_AC_AC_Y=18;

        // Action Label
        TGLabel *fLabelAction = new TGLabel(fGroupFrameAnalysisControl,"Actions");
        fLabelAction->SetTextJustify(36);
        fLabelAction->SetMargins(0,0,0,0);
        fLabelAction->SetWrapLength(-1);
        fGroupFrameAnalysisControl->AddFrame(fLabelAction, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fLabelAction->MoveResize(pos_AC_AC_X+10,pos_AC_AC_Y,110,18);

	    fNumberEntryTafDebugLevel = new TGNumberEntry(fGroupFrameAnalysisControl,0, 9,999, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-10, 10);
          fGroupFrameAnalysisControl->AddFrame(fNumberEntryTafDebugLevel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fNumberEntryTafDebugLevel->MoveResize(pos_AC_AC_X,pos_AC_AC_Y+=pos_AC_deltaY,50,25);
          
          fTextButtonSetTafDebugLevel = new TGTextButton(fGroupFrameAnalysisControl,"set debug");
          fTextButtonSetTafDebugLevel->SetTextJustify(36);
          fTextButtonSetTafDebugLevel->SetMargins(0,0,0,0);
          fTextButtonSetTafDebugLevel->SetWrapLength(-1);
          fTextButtonSetTafDebugLevel->Resize(130,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonSetTafDebugLevel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonSetTafDebugLevel->MoveResize(pos_AC_AC_X+50,pos_AC_AC_Y,80,24);
          
          fTextButtonTurnVerbose = new TGTextButton(fGroupFrameAnalysisControl,"Turn verbose ON");
          fTextButtonTurnVerbose->SetTextJustify(36);
          fTextButtonTurnVerbose->SetMargins(0,0,0,0);
          fTextButtonTurnVerbose->SetWrapLength(-1);
          fTextButtonTurnVerbose->Resize(130,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonTurnVerbose, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonTurnVerbose->MoveResize(pos_AC_AC_X,pos_AC_AC_Y+=pos_AC_deltaY,130,24);

          fTextButtonQuitTaf = new TGTextButton(fGroupFrameAnalysisControl,"Quit TAF");
          fTextButtonQuitTaf->SetTextJustify(36);
          fTextButtonQuitTaf->SetMargins(0,0,0,0);
          fTextButtonQuitTaf->SetWrapLength(-1);
          fTextButtonQuitTaf->Resize(130,24);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonQuitTaf, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonQuitTaf->MoveResize(pos_AC_AC_X,pos_AC_AC_Y+=pos_AC_deltaY,130,24);

          fTextButtonBeepWhenFinished= new TGCheckButton(fGroupFrameAnalysisControl,"Beep when finished");
          fTextButtonBeepWhenFinished->SetTextJustify(36);
          fTextButtonBeepWhenFinished->SetMargins(0,0,0,0);
          fTextButtonBeepWhenFinished->SetWrapLength(-1);
          fGroupFrameAnalysisControl->AddFrame(fTextButtonBeepWhenFinished, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
          fTextButtonBeepWhenFinished->MoveResize(pos_AC_AC_X,pos_AC_AC_Y+=pos_AC_deltaY,130,24);
          
          pos_AC_AC_Y+=24;

        if (pos_AC_AC_Y > sizeYfCompositeFrameTabAnalysisControl) sizeYfCompositeFrameTabAnalysisControl = pos_AC_AC_Y ;


        sizeYfCompositeFrameTabAnalysisControl -= 20;
        
        
        // Separation 1
        TGVertical3DLine *fVertical3DLine1453 = new TGVertical3DLine(fGroupFrameAnalysisControl,8,152);
        fGroupFrameAnalysisControl->AddFrame(fVertical3DLine1453, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fVertical3DLine1453->MoveResize(pos_AC_GT_X-10,16,4,sizeYfCompositeFrameTabAnalysisControl);

        // Separation 2
        TGVertical3DLine *fVertical3DLine14530 = new TGVertical3DLine(fGroupFrameAnalysisControl,8,152);
        fGroupFrameAnalysisControl->AddFrame(fVertical3DLine14530, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fVertical3DLine14530->MoveResize(pos_AC_AC_X-10,16,4,sizeYfCompositeFrameTabAnalysisControl);

        // Separation 3
        TGVertical3DLine *fVertical3DLine14532 = new TGVertical3DLine(fGroupFrameAnalysisControl,8,152);
        fGroupFrameAnalysisControl->AddFrame(fVertical3DLine14532, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fVertical3DLine14532->MoveResize(pos_AC_SA_X-10,16,4,sizeYfCompositeFrameTabAnalysisControl);

        // Separation 4
        TGVertical3DLine *fVertical3DLine14531 = new TGVertical3DLine(fGroupFrameAnalysisControl,8,152);
        fGroupFrameAnalysisControl->AddFrame(fVertical3DLine14531, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        fVertical3DLine14531->MoveResize(pos_AC_SC_X-10,16,4,sizeYfCompositeFrameTabAnalysisControl);


      sizeYfCompositeFrameTabAnalysisControl += 25;
      sizeXfCompositeFrameTabAnalysisControl = pos_AC_AC_X + pos_AC_GT_X -15 ;
      fGroupFrameAnalysisControl->SetLayoutManager(new TGVerticalLayout(fGroupFrameAnalysisControl));
      fGroupFrameAnalysisControl->Resize(sizeXfCompositeFrameTabAnalysisControl,sizeYfCompositeFrameTabAnalysisControl);
      fCompositeFrameTabAnalysisControl->AddFrame(fGroupFrameAnalysisControl, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
      
//#############################################################################################################
  sizeYfTabBottom = sizeYfCompositeFrameTabAnalysisControl + 23 ;
  fTabBottom->SetTab(1);
  fTabBottom->Resize(fTabBottom->GetDefaultSize());
  fMainFrame->AddFrame(fTabBottom, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fTabBottom->MoveResize(2,sizeYfTabTop+1,length_X,sizeYfTabBottom);
//#############################################################################################################
//#############################################################################################################       

  fMainFrame->SetMWMHints(kMWMDecorAll,
  kMWMFuncAll,
  kMWMInputModeless);
  fMainFrame->MapSubwindows();

  fMainFrame->Resize(fMainFrame->GetDefaultSize());
  fMainFrame->MapWindow();
  fMainFrame->Resize(length_X+10,sizeYfTabTop + sizeYfTabBottom + 5);

  
  sprintf(title,"TAF GUI (run %d - config file %s)",RunNumber,fSession->GetConfigFileName().Data());
  fMainFrame->SetWindowName(title);

  return 0;
}

Int_t MRax::GuiConnectButtons(void)
{
  // Analysis control tab
  //    Analysis control group frame
  //            Process Events zone
  fTextButtonProcess1event              ->Connect("Clicked()","MRax",this,"GuiButtonProcess1event()");
  fTextButtonProcess10events            ->Connect("Clicked()","MRax",this,"GuiButtonProcess10events()");
  fTextButtonProcess100events           ->Connect("Clicked()","MRax",this,"GuiButtonProcess100events()");
  fTextButtonProcessXevents             ->Connect("Clicked()","MRax",this,"GuiButtonProcessXevents()");
  fTextButtonProcessAllEvents           ->Connect("Clicked()","MRax",this,"GuiButtonProcessAllEvents()");
  //            Go to event with ... zone
  fTextButtonGoToEventWithNhits         ->Connect("Clicked()","MRax",this,"GuiButtonGoToEventWithNhits()");
  fTextButtonGoToEventWithNtracks       ->Connect("Clicked()","MRax",this,"GuiButtonGoToEventWithNtracks()");
  fTextButtonGoToEventNb                ->Connect("Clicked()","MRax",this,"GuiButtonGoToEventNb()");
  //            Actions zone
  fTextButtonTurnVerbose                ->Connect("Clicked()","MRax",this,"GuiButtonToggleVerbosity()");
  fTextButtonSetTafDebugLevel           ->Connect("Clicked()","MRax",this,"GuiButtonSetTafDebugLevel()");
  fTextButtonSaveDisplays               ->Connect("Clicked()","MRax",this,"GuiButtonSaveDisplays()");
  fTextButtonSaveHistograms             ->Connect("Clicked()","MRax",this,"GuiButtonSaveHistograms()");
  fTextButtonQuitTaf                    ->Connect("Clicked()","MRax",this,"GuiButtonQuitTaf()");
  //            Display zone
  fTextButtonRefreshView                ->Connect("Clicked()","MRax",this,"GuiButtonRefreshDisplays()");
  fTextButtonResetView                  ->Connect("Clicked()","MRax",this,"GuiButtonResetDisplays()");
  //            Save zone
  fTextButtonSaveResults                ->Connect("Clicked()","MRax",this,"GuiButtonSaveResults()");
  // Alignment Tab
  fTextButtonAlignResetValues           ->Connect("Clicked()","MRax",this,"GuiButtonAlignResetValues()");
  fTextButtonAlignDoTranslatTurn        ->Connect("Clicked()","MRax",this,"GuiButtonAlignDoTranslateTurn()");
  fTextButtonAlignDisplayActualPosTilt  ->Connect("Clicked()","MRax",this,"GuiButtonAlignDisplayPosTilt()");
  fTextButtonAlignUpdateActualPosTilt   ->Connect("Clicked()","MRax",this,"GuiButtonAlignUpdatePosTilt()");
  fTextButtonAlignUpdateCfgFile         ->Connect("Clicked()","MRax",this,"GuiButtonAlignUpdateCfgFile()");
  // Noise Tab
  fTextButtonGenerateVetoPixelsCode      ->Connect("Clicked()","MRax",this,"GuiButtonGenerateVetoPixelsCode()");

  return 0;
}

Int_t MRax::GuiInitParameters(void)
{
  //#################
  // Display events
  //#################
    //****************************************
    //  Pixels [pixels
    //****************************************
      //-------------------------------------------
      // CMOS frame
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisRawDatCmoNatGeoInPix->SetOn();
      // cumul
      if(0) fTextButtonDisRawDatCmoNatGeoInPix_Cumul->SetOn();
      // with hits
      if(0) fTextButtonDisRawDatCmoNatGeoInPix_Hit->SetOn();
      // with overlap line
      if(0) fTextButtonDisRawDatCmoNatGeoInPix_OlLine->SetOn();
      // with color bar
      if(0) fTextButtonDisRawDatCmoNatGeoInPix_ColBar->SetOn();
      //-------------------------------------------
      // FIRST board frame
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisRawDatTrackerFraInPix->SetOn();
      // cumul
      if(0) fTextButtonDisRawDatTrackerFraInPix_Cumul->SetOn();
      // with hits
      if(0) fTextButtonDisRawDatTrackerFraInPix_Hit->SetOn();
      // with overlap line
      if(0) fTextButtonDisRawDatTrackerFraInPix_OlLine->SetOn();
      // with color bar
      if(0) fTextButtonDisRawDatTrackerFraInPix_ColBar->SetOn();
      //-------------------------------------------
      // ALL
      //-------------------------------------------
      // min pix occurence
      if(0) fNumberEntry_RawMinPixOccu->SetNumber(2);
    //****************************************
    //  Hits [pixels]
    //****************************************
      //-------------------------------------------
      // CMOS frame
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisHitsCmoNatGeoInPix->SetOn();
      // cumul
      if(0) fTextButtonDisHitsCmoNatGeoInPix_Cumul->SetOn();
      // track assoc.
      if(0) fTextButtonDisHitsCmoNatGeoInPix_AssoTrack->SetOn();
      // with overlap line
      if(0) fTextButtonDisHitsCmoNatGeoInPix_OlLine->SetOn();
      // group in pixels
      if(0) fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix->SetOn();
      //-------------------------------------------
      // FIRST board frame
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisHitsInTrackerFrameInPix->SetOn();
      // cumul
      if(0) fTextButtonDisHitsInTrackerFrameInPix_Cumul->SetOn();
      // track assoc.
      if(0) fTextButtonDisHitsInTrackerFrameInPix_AssoTrack->SetOn();
      // with overlap line
      if(0) fTextButtonDisHitsInTrackerFrameInPix_OlLine->SetOn();
      // group in pixels
      if(0) fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix->SetOn();
      //-------------------------------------------
      // ALL
      //-------------------------------------------
      // min pix occurence
      if(0) fNumberEntry_HitMinOccu->SetNumber(2);
    //****************************************
    //  Hits in tracker frame [um]
    //****************************************
      //-------------------------------------------
      // Separate view
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisHitsInTrackerFrameInUm->SetOn();
      // cumul
      if(0) fTextButtonDisHitsInTrackerFrameInUm_Cumul->SetOn();
      // Tracks planes intersection
      if(0) /*fGRadioButton_HitTrackInter_none->SetOn(); //by default*/fGRadioButton_HitTrackInter_cross->SetOn();
      //-------------------------------------------
      // Overlay view
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisHitsInTrackerFrameInUmOv->SetOn();
      // cumul
      if(0) fTextButtonDisHitsInTrackerFrameInUmOv_Cumul->SetOn();
      // Planes ON : 
      if(1)
      {
        for (iPlane=1;iPlane<=nbPlanes;iPlane++) \
          fListBoxOverlayHitsInTrackerFrameInUm->Select(iPlane);
      }
    //****************************************
    //  Tracks
    //****************************************
      //-------------------------------------------
      // All
      //-------------------------------------------
      // cumul
      if(0) fTextButtonDisTracks_Cumul->SetOn();
      //-------------------------------------------
      // 3D view
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisTracks3D->SetOn();
      // draw tracks
      if(1) fTextButtonDisTracks3D_DrawTracks->SetOn();
      // draw sensors
      if(1) fTextButtonDisTracks3D_DrawSensor->SetOn();
      // draw tracked hits
      if(1) fTextButtonDisTracks3D_DrawTrackedHits->SetOn();
      // draw all hits
      if(1) fTextButtonDisTracks3D_DrawAllHits->SetOn();
      //-------------------------------------------
      // 2D view
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisTracks2D->SetOn();
      // draw tracks
      if(1) fTextButtonDisTracks2D_DrawTracks->SetOn();
      // draw sensors
      if(1) fTextButtonDisTracks2D_DrawSensor->SetOn();
      // draw tracked hits
      if(1) fTextButtonDisTracks2D_DrawTrackedHits->SetOn();
      // draw all hits
      if(1) fTextButtonDisTracks2D_DrawAllHits->SetOn();
    //****************************************
    //  Vertices
    //****************************************
      //-------------------------------------------
      // All
      //-------------------------------------------
      // cumul
      if(0) fTextButtonDisVertices_Cumul->SetOn();
      //-------------------------------------------
      // 3D view
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisVertices3D->SetOn();
      // draw vertices points
      if(1) fTextButtonDisVertices3D_DrawVerticesPoints->SetOn();
	// small points
        if(0) fTextButtonDisVertices3D_DrawVerticesPointsSmall->SetOn();
      // draw sensors
      if(1) fTextButtonDisVertices3D_DrawSensor->SetOn();
      // draw tracks
      if(1) fTextButtonDisVertices3D_DrawTracks->SetOn();
      // draw tracker direction
      if(1) fTextButtonDisVertices3D_DrawTrackDir->SetOn();
      // draw target
      if(1) fTextButtonDisVertices3D_DrawTarget->SetOn();
      // draw beam
      if(1) fTextButtonDisVertices3D_DrawBeam->SetOn();
      // draw acceptance
      if(1) fTextButtonDisVertices3D_DrawAcceptance->SetOn();
      //-------------------------------------------
      // 2D view
      //-------------------------------------------
      // ON ?
      if(0) fTextButtonDisVerticesProjs->SetOn();
      // draw target
      if(1) fTextButtonDisVerticesProjs_DrawTarget->SetOn();
    //****************************************
    //  Statistics
    //****************************************
      //-------------------------------------------
      // Lines overflow
      //-------------------------------------------
      // on
      if(0) fTextButtonDisCMOSoverflowLines->SetOn();
      // cumul
      if(0) fTextButtonDisCMOSoverflowLines_Cumul->SetOn();
      //-------------------------------------------
      // Clusters size
      //-------------------------------------------
      // on
      if(0) fTextButtonDisHitsSize->SetOn();
      // cumul
      if(0) fTextButtonDisHitsSize_Cumul->SetOn();
      // max size 
      if(0) fNumberEntry_ClustersBinMax->SetNumber(25);
      // All
      if(0) fTextButtonDisHitsSizeAll->SetOn();
      // Tracked
      if(0) fTextButtonDisHitsSizeTracked->SetOn();
      // Untracked
      if(0) fTextButtonDisHitsSizeUnTracked->SetOn();
      //-------------------------------------------
      // Hits-Tracks association
      //-------------------------------------------
      // on
      if(0) fTextButtonDisHitsAssociation->SetOn();
      // cumul
      if(0) fTextButtonDisHitsAssociation_Cumul->SetOn();
      //-------------------------------------------
      // Hits / event
      //-------------------------------------------
      // on
      if(0) fTextButtonDisHitsByEvent->SetOn();
      // cumul
      if(0) fTextButtonDisHitsByEvent_Cumul->SetOn();
      // max 
      if(0) fNumberEntry_HitsByEventBinMax->SetNumber(20);
      //-------------------------------------------
      // Tracks + triggers
      //-------------------------------------------
      // on
      if(0) fTextButtonDisStats->SetOn();
      // cumul
      if(0) fTextButtonDisStats_Cumul->SetOn();
      // with triggers
      if(0) fTextButtonDisStats_withTriggers->SetOn();
      // histo chi2 limit
      if(0) fNumberEntry_TracksHistoChi2Max->SetNumber(20);
      // histo slope limit
      if(0) fNumberEntry_TracksHistoSlopeMax->SetNumber(10);
      //-------------------------------------------
      // Along events
      //-------------------------------------------
      // on
      if(0) fTextButtonDisStatsAlongEvents->SetOn();
      // cumul
      if(0) fTextButtonDisStatsAlongEvents_Cumul->SetOn();
      // with triggers
      if(0) fTextButtonDisStatsAlongEvents_withTriggers->SetOn();

      
  //#################
  // Analysis control
  //#################
    //****************************************
    //  Process Events
    //****************************************
    // Custom event number 
    if(0) fNumberEntry_EventsToProcess->SetNumber(200);
    //****************************************
    //  Go to event with at least 
    //****************************************
    // Hit(s)
    if(0) fNumberEntryGoToEventWithNhits->SetNumber(2);
    // Track(s)
    if(0) fNumberEntryGoToEventWithNtracks->SetNumber(4);
    // Go to event
    if(0) fNumberEntry_GoToEvent->SetNumber(16);
    //****************************************
    //  Display
    //****************************************
    // Refresh every
    if(0) fNumberEntry_RefreshFreq->SetNumber(200);
    //****************************************
    //  Save
    //****************************************
    // Files prefix
    if(0) fTextEntry_FilesPrefix->SetText("pre");
    // Files suffix
    if(0) fTextEntry_FilesSuffix->SetText("suff");
    // Canvas
      // PDF
      if(0) fTextButtonSaveCanvas_inPDF->SetOn();
      // PNG
      if(0) fTextButtonSaveCanvas_inPNG->SetOn();
      // ROOT
      if(0) fTextButtonSaveCanvas_inROOT->SetOn();
    // Results 
      // Tree Tracks
      if(0) fTextButtonSaveTracks->SetOn();
      // Tree Vertices
      if(0) fTextButtonSaveVertice->SetOn();
    //****************************************
    //  Actions
    //****************************************
    //set debug level
    if(0) {fNumberEntryTafDebugLevel->SetNumber(4); GuiButtonSetTafDebugLevel();}
    // Verbose
    if(0) GuiButtonToggleVerbosity();// By default OFF
    // Beep when finished ?
    if(0) fTextButtonBeepWhenFinished->SetOn();
  
  return 0;
}

//______________________________________________________________________________________________________
//
//	MISC TOOLS
//______________________________________________________________________________________________________
//

  //****************************************************************
  //		Print messages with colors
  //****************************************************************
void MRax::printf_info(const Char_t *text) //green
{
  PRINTF_GREEN_HL
  printf("MRax info (%s) : ",text);
  PRINTF_DEFAULT
}

void MRax::printf_warning(const Char_t *text) //yellow
{
  PRINTF_YELLOW_HL
  printf("MRax warning (%s) : ",text);
  PRINTF_DEFAULT
}

void MRax::printf_error(const Char_t *text) //red
{
  PRINTF_RED_HL
  printf("MRax error (%s) : ",text);
  PRINTF_DEFAULT
}

//______________________________________________________________________________________________________
//
//		MRax class gestion (contructor, init functions, ...)
//______________________________________________________________________________________________________
//

MRax* MRax::fgInstanceRax = 0; // returns pointer to global object

MRax::MRax(DSession *aSession)
{
  if (fgInstanceRax) Warning("MRax", "object already instantiated");
  else fgInstanceRax = this;
  
  eventsProcessed = 0;
  
  // Get pointeurs and values from TAF
  fSession  	= aSession;
  tTracker  	= fSession->GetTracker();
  tDAcq         = fSession->GetDataAcquisition();
  RunNumber 	= fSession->GetRunNumber();
  nbPlanes  	= tTracker->GetPlanesN();
  
  fSetup        = fSession->GetSetup();

  
  // Configuration methods (central configuration)
  ConfigureGeneral();
  ConfigureDisplays();
  if (!gROOT->IsBatch()) GuiLauch();
  
  // StatsCumulatedAlongEvents RESET
  PrintCumulatedStatistics_Reset();
  //----------------------------------------------------------------
  // Display functions initialisation
  //----------------------------------------------------------------
  flagDisplayPixelsInCmosFrameInPixel_Init=0;
  flagDisplayPixelsInFirstBoardFrameInPixel_Init=0;
  
  flagDisplayHitsInCmosFrameInPixel_Init=0;
  flagDisplayHitsInFirstBoardFrameInPixel_Init=0;
  
  flagDisplayHitsInTrackerFrameInUm_Init=0;
  flagDisplayHitsInTrackerFrameInUmOverlayed_Init=0;
  
  flagDisplayTracksIn3D_Init=0;
  flagDisplayTracksIn2D_Init=0;
  flagDisplayVerticesProjs_Init=0;
  flagDisplayVerticesIn3D_Init=0;
  
  flagDisplayCmosOverFlow_Init=0;
  flagDisplayClustersSize_Init=0;
  flagDisplayHitsByEvent_Init=0;
  flagDisplayHitsTracksAssociation_Init=0;
  flagDisplayTracksAndTriggersProperties_Init=0;
  flagDisplayChronogram_Init=0;
  
  flagStoreTracks_Init=0;

  flagComputeVertices_Done=0;
  flagComputeVertices_Init=0;
  
  flagDisplayNoiseStudy_Init=0;

  // Tables for GUI
  // List of planes to display allocation
  flagsDisplayHitsInTrackerFrameInUmOverlayed_plan2disp = new Bool_t[nbPlanes];
  

}

MRax*& MRax::InstanceRax( DSession *aSession)
{
  if (!fgInstanceRax)
  {
    cout<<"No current Mimosa Raw session detected! Creating a new one..."<<endl;
    fgInstanceRax = new MRax( aSession);
  }
  return fgInstanceRax;
}


