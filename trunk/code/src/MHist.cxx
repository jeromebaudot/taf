// @(#)maf/maf:$Name:  $:$Id: MHist.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 
// Last Modified JB 2009/07/20
// Last Modified JB 2009/08/25
// Last Modified JB 2009/09/07
// Last Modified JB 2009/09/14 Histo title like tu-hu
// Last Modified JB 2011/07/05 Rebinned histo for #hits/event (hnGOODhits)
// Last Modified MG 2011/07/08 new histos for efficiency map
// Last Modified JB 2011/10/30 New track angle histos
// Last Modified JB 2012/11/22 introduce pitchU & V and track-hit distance
// Last Modified CLM 2013/01/23 New histos for charge PSF
// Last Modified JB 2013/05/01 New hit map histos in tracker frame
// Last Modified JB 2013/07/16 New alignment check histos
// Last Modified JB 2013/09/12 Set of histo ranges (SNR,S,N) from config
// Last Modified JB 2013/11/13 new histo npixCumul
// Last Modified JB 2014/01/11 histo declaration depends on analysis goal

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Contains all analysis  histograms and canvas           //
  //  (those which are filled by MimosaPro())                //
  //                                                         //   
  ////////////////////////////////////////////////////////////


#include "TROOT.h"
#include "MHist.h"

ClassImp(MHist)

//______________________________________________________________________________
//
  MHist::MHist()
{
   // Create histogram manager object. Histograms will be created
   // in the "dir" directory.

   // Save current directory and cd to "dir".
  gROOT->GetListOfCleanups()->Add(this);
  theHist = this;
}


//______________________________________________________________________________
//
 MHist::~MHist()
{
   // Clean up all histograms.

   // Nothing to do. Histograms will be deleted when the directory
   // in which tey are stored is closed.
  Dir();
  Clear();

}

//______________________________________________________________________________
//
void MHist::Dir() {
  if (!dir) return;
  dir->cd();
  return;
}

//______________________________________________________________________________
//
TH1F* MHist::AutoZoom(TH1F* H,Option_t* aType, Int_t EntryMin/*=0*/)
{
  
  // Redefine the display range of the histogram according to 
  //  an option and a minimal number of entries in a bin (EntryMin):
  // - aType="all" subrange where left and right exclude wings are below EntryMin
  // - aType="sym" same as before but right limit is symetric
  // - aType="calib" subrange around a peak located at the end of the spectrum
  //
  // Default is aType="all", EntryMin=0 which basically finds the subrange
  //  of the histo where entries are not 0.
  
  Int_t shift = (Int_t)(H->GetNbinsX()/50.);
  
  TString opt = aType;
  opt.ToLower();
  
  int min =0;
  int max = H->GetNbinsX();
  int New_min = min;
  int New_max = max;

  if (opt.Contains("all")) opt = TString("min,max");
  if (opt.Contains("sym")) opt = TString("min,sym");
  
  if (opt.Contains("calib"))
  {
    for (New_max=max;New_max>=min;New_max--)
    {Stat_t c = H->GetBinContent(New_max);  if (c>EntryMin) break;}
    
    Stat_t  EntryMax=H->GetEntries()-H->GetEntries()/30; // previously /100
    
    Stat_t t = 0;
    for  (New_min=min; New_min<=New_max;New_min++)
    {t+=H->GetBinContent(New_min); if (t>EntryMax) break;}

    shift = (Int_t)(H->GetNbinsX()/20.); // changed for calib
  }

  else {
    
    if (opt.Contains("min"))
    {
      for  (New_min=min; New_min<=max;New_min++)   
      {Stat_t c = H->GetBinContent(New_min);  if (c>EntryMin) break;}
    }
    
    if (opt.Contains("max"))
    {
      for (New_max=max;New_max>=min;New_max--) 
      {Stat_t c = H->GetBinContent(New_max);  if (c>EntryMin) break;}
    }
    
    else if (opt.Contains("sym")) 
    {
      New_max = H->FindBin(-1*H->GetXaxis()->GetBinCenter(New_min));
    }
    
  }
  
  H->GetXaxis()->SetRange(New_min - shift  , New_max + shift);  
  return H;
}


//_______________________________________________________________________________________
//
TH2F* MHist::AutoZoom(TH2F* H,Option_t* aType/*="all"*/, Int_t EntryMin/*=0*/)
{

  Int_t shiftX = (Int_t)(H->GetNbinsX()/30.);
  Int_t shiftY = (Int_t)(H->GetNbinsY()/30.); 
  
  TString opt = aType;
  opt.ToLower();
 
  int minX =0;
  int maxX = H->GetNbinsX();
  int New_minX = minX;
  int New_maxX = maxX;
  
  int minY =0;
  int maxY = H->GetNbinsY();
  int New_minY = minY;
  int New_maxY = maxY;
  
  if (opt.Contains("all")) opt = TString("minx,maxx,miny,maxy");
  
  if (opt.Contains("maxx"))
    {
      for  (New_maxX = maxX;New_maxX >=minX; New_maxX--)
	{  Stat_t c = 0;
	for  (int i_y = maxY; i_y >= minY;i_y--)
	  { c = H->GetBinContent(New_maxX,i_y);  if (c>EntryMin) break;}
	if (c>EntryMin) break;
	}
    }
  
  if (opt.Contains("maxy"))
    {
      
      for  (New_maxY = maxY;New_maxY >=minY;New_maxY--) 
	{  Stat_t c = 0;
	for  (int i_x=maxX; i_x>=minX;i_x--)   
	  { c = H->GetBinContent(i_x, New_maxY );  if (c>EntryMin) break;}
	if (c>EntryMin) break;
	}
 
    }
  
  if (opt.Contains("minx"))
    {
     
      for  (New_minX = minX;New_minX <=maxX; New_minX++)
	{  Stat_t c = 0;
	for  (int i_y = minY; i_y <= maxY;i_y++)
	  { c = H->GetBinContent(New_minX,i_y);  if (c>EntryMin) break;}
	if (c>EntryMin) break;
	}
    }
  
  

if (opt.Contains("miny"))
    {
      for  (New_minY = minY;New_minY <=maxY;New_minY++) 
	{  Stat_t c = 0;
	for  (int i_x=minX; i_x<=maxX;i_x++)   
	  { c = H->GetBinContent(i_x, New_minY );  if (c>EntryMin) break;}
	if (c>EntryMin) break;
	}      
    }
  

  H->GetXaxis()->SetRange(New_minX - shiftX  , New_maxX + shiftX);  
  H->GetYaxis()->SetRange(New_minY - shiftY  , New_maxY + shiftY);  
   
  return H;
}

//______________________________________________________________________________
//
void  MHist::BookingHistograms(Int_t RunNumber, Float_t PixelSizeU, Float_t PixelSizeV, Int_t NofPixelInRaw, Int_t NofPixelInColumn, Float_t TrackToHitDistanceLimit, DSetup *aSetup, Bool_t ifReferenceTrack,
			       Bool_t ifCalibration, Bool_t ifLaserSpot, Bool_t ifMiniVector, Bool_t ifVertex, Bool_t ifFake, Bool_t ifImaging)
{
  // Y.G.&  A.B declaration of all the histograms filled in MAnalysis.
  //
  // Note that ranges are defined by a number of global variables in the
  //  section 'Chip specification setting'.
  //
  // The same kind of Histogram could appear several times for various
  //  hit status: ALL, GOOD, SELECTED or MATCHED.
  // Below, histograms are grouped by such hit status.
  //
  // See modifications on top of this file
   
  Bool_t HistDebug = kTRUE;
  
  // Save current directory and cd to "dir".
  saved = gDirectory;
  //   if(!dir) 
  //     gDirectory = gROOT;
  dir = new TDirectory("histograms", "all histograms"); 
  dir->cd();
  
  Int_t NofPixels = NofPixelInRaw*NofPixelInColumn; // JB 2010/07/26
  
  Info("MHist","Booking histograms..."); 
  
  //----------------------------------------------------------------------------
  //----- Setting of binnings
  //----------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: settings" << endl;
  
  Int_t Mtype=RunNumber/1000;
  if(RunNumber==2110)Mtype=4;
  
  //Float_t scf  =  2.0;  // removed -> info now in config file, JB 2013/08/21
  /*
   // this will not work anymore because tSession is not anymore a global var, JB 2011/07/21
  Int_t Readout = (tSession->GetSetup()->GetPlanePar(9)).Readout ;
  if((Readout == 12)||(Readout == 13)){ // ADC
    scf = 0.5;
  }
  */

  Float_t PixelSize = (PixelSizeU+PixelSizeV)/2.; // JB 2012/11/22
  Float_t ResRange=10*(PixelSizeU+PixelSizeV)/2.; // propto average pitch
  Int_t   ResNbins = ceil(2*ResRange); // binnin is 1um
  // the 3 ranges below are read from configuration file, JB 2013/09/12
  Float_t ChargeRange = aSetup->GetAnalysisPar().HistoChargeRange; // cluster wise
  Int_t   ChargeNbins = 400;
  Float_t ChargePixRange = ChargeRange/2.; // pixel wise
  Int_t   ChargePixNbins = ChargeNbins/2;
  Float_t SNRRange = aSetup->GetAnalysisPar().HistoSNRRange;
  Int_t   SNRNbins = 280;
  Float_t NoiseRange = aSetup->GetAnalysisPar().HistoNoiseRange;
  Int_t   NoiseNbins = 100;
  Float_t dumin= -20.0;
  Float_t dumax= +20.0;
  Float_t dvmin= -20.0;
  Float_t dvmax= +20.0;
  Float_t tumin = -(NofPixelInRaw*2)*PixelSizeU - PixelSizeU/2;
  Float_t tumax = (NofPixelInRaw*2)*PixelSizeU + PixelSizeU/2;
  Float_t tvmin = -(NofPixelInColumn*2)*PixelSizeV - PixelSizeV/2;
  Float_t tvmax = (NofPixelInColumn*2)*PixelSizeV + PixelSizeV/2;
  Int_t H_Nbintu = int((tumax - tumin)/PixelSizeU);
  Int_t H_Nbintv = int((tvmax - tvmin)/PixelSizeV);

  Float_t humin, humax, hvmin, hvmax;
  Int_t H_Nbinhu, H_Nbinhv;
  // If LaserSpot option on, have a 1um bin width in hit maps
  //  and the map area is defined by geomatrix
  if( ifLaserSpot ) {
    humin = geomUmin;
    humax = geomUmax;
    hvmin = geomVmin;
    hvmax = geomVmax;
    H_Nbinhu = int((humax - humin));
    H_Nbinhv = int((hvmax - hvmin));
  }
  // If no track option on, have a bin width = pitch/2
  else if( !ifReferenceTrack ) {
    humin = -(NofPixelInRaw*2)*PixelSizeU - PixelSizeU/2;
    humax = (NofPixelInRaw*2)*PixelSizeU + PixelSizeU/2;
    hvmin = -(NofPixelInColumn*2)*PixelSizeV - PixelSizeV/2;
    hvmax = (NofPixelInColumn*2)*PixelSizeV + PixelSizeV/2;
    H_Nbinhu = int((humax - humin)/PixelSizeU)*2;
    H_Nbinhv = int((hvmax - hvmin)/PixelSizeV)*2;
  }
  // Otherwise bin width is the pixel size
  else {
    humin = -(NofPixelInRaw*2)*PixelSizeU - PixelSizeU/2;
    humax = (NofPixelInRaw*2)*PixelSizeU + PixelSizeU/2;
    hvmin = -(NofPixelInColumn*2)*PixelSizeV - PixelSizeV/2;
    hvmax = (NofPixelInColumn*2)*PixelSizeV + PixelSizeV/2;

    H_Nbinhu = int((humax - humin)/PixelSizeU);
    H_Nbinhv = int((hvmax - hvmin)/PixelSizeV);
  }
  printf("HistoBooking: U,V axis: hits Nbins(%d,%d) for u(%.0f,%.0f)-v(%.0f,%.0f), tracks Nbins(%d,%d)  for u(%.0f,%.0f)-v(%.0f,%.0f)\n", H_Nbinhu, H_Nbinhv, humin, humax, hvmin, hvmax, H_Nbintu, H_Nbintv, tumin, tumax, tvmin, tvmax); 

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------


  //------------------------------------------------------------------------------
  //----- general control
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: general control" << endl;
  

 selection = new TH1F("selection","Event selection",10,0.,10.);
 selection->SetYTitle("# counts");
  selection->SetStats(0);
  // Special display option for the "selection" histo
  selection->SetFillColor(43);
  Float_t xinit=1.5; // .26
  Float_t xstep=1.; // .07
  Float_t yinit=0.; // 0.3
  textLabel[0] = new TText(xinit,yinit,"all events");
  textLabel[1] = new TText(xinit+1*xstep,yinit,"events with tracks and hits");
  textLabel[2] = new TText(xinit+2*xstep,yinit,"all tracks");
  textLabel[3] = new TText(xinit+3*xstep,yinit,"good tracks");
  textLabel[4] = new TText(xinit+4*xstep,yinit,"good tracks in submatrix");
  textLabel[5] = new TText(xinit+5*xstep,yinit,"all hits in plane");
  textLabel[6] = new TText(xinit+6*xstep,yinit,"good hits in plane");
  textLabel[7] = new TText(xinit+7*xstep,yinit,"hits associated to track");
  textLabel[8] = new TText(xinit+8*xstep,yinit,"final match");
  for( Int_t il=0; il<9; il++) {
    textLabel[il]->SetNDC(0); // 1
    textLabel[il]->SetTextAngle(90.);
  }


  //------------------------------------------------------------------------------
  //----- Counters of GOOD/ALL hits.
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: GOOD/ALL hits" << endl;
  
  hnhit  = new TH1F("nhit","# hits per event, ALL hits",600,-0.5,599.5); 
  hnhitievt = new TH1F("hnhitievt","# ALL hits vs event number",20000,0,20000);
  hnahitievt = new TH2F("hnahitievt","# GOOD hits in plane vs event number",500,0,500000,510,0.0,510.0);
  hnGOODhit  = new TH1F("hnGOODhit","# GOOD hits per event in plane",600,-0.5,599.5); // changed for correct mean, JB 2011/07/05
  hnGOODhitwhentrack  = new TH1F("hnGOODhitwhentrack","# GOOD hits in plane when a GOOD track is in GEOMATRIX",600,-0.5,599.5);

  hnTracksinSensorPerEvent = new TH1F("hnTracksinSensorPerEvent",
				      "# Tracks per event in Sensor",
				      101,
				      -0.5,100.5);
  hnTracksinSensorPerEvent->SetXTitle("# tracks / event");
  hnTracksinSensorPerEvent->SetYTitle("Events");

  hTrackDensity = new TH1F("hTrackDensity","Track density per event in Sensor",
			   50,
			   0.0,100.0);
  hTrackDensity->SetXTitle("# tracks / cm^{2} / event");
  hTrackDensity->SetYTitle("Events");  

  hnTracksInGeomatrixVsTrackPerEvent  = new TH1F("hnTracksInGeomatrixVsTrackPerEvent",
						 "Total # tracks inside geomatrix vs Tracks per event",
						 100,0.5,100.5); // AP 2014/10/029
  hnTracksInGeomatrixVsTrackPerEvent->SetXTitle("# tracks / event");
  hnTracksInGeomatrixVsTrackPerEvent->SetYTitle("Tot # tracks");
  hnGOODhitInGeomatrixVsTrackPerEvent = new TH1F("hnGOODhitInGeomatrixVsTrackPerEvent",
						 "Total # GOOD hits inside geomatrix vs Tracks per event",
						 100,0.5,100.5); // AP 2014/10/029
  hnGOODhitInGeomatrixVsTrackPerEvent->SetXTitle("# tracks / event");
  hnGOODhitInGeomatrixVsTrackPerEvent->SetYTitle("Tot # GOOD hits");

  hEfficiencyInGeomatrixVsTrackPerEvent = new TH1F("hEfficiencyInGeomatrixVsTrackPerEvent",
						   "Efficiency in Geomatrix vs Track per event",
						   100,0.5,100.5);
  hEfficiencyInGeomatrixVsTrackPerEvent->SetXTitle("# tracks / event");
  hEfficiencyInGeomatrixVsTrackPerEvent->SetYTitle("Efficiency (%)");
  hEfficiencyInGeomatrixVsTrackPerEvent->SetLineColor(4);
  hEfficiencyInGeomatrixVsTrackPerEvent->SetLineWidth(2);
  hEfficiencyInGeomatrixVsTrackPerEvent->SetMarkerColor(4);
  hEfficiencyInGeomatrixVsTrackPerEvent->SetMarkerStyle(20);
  hEfficiencyInGeomatrixVsTrackPerEvent->SetStats(false);

  hResidueUInGeomatrixVsTrackPerEvent = new TH1F("hResidueUInGeomatrixVsTrackPerEvent",
						 "Residue in Geomatrix vs Track per event",
						 100,0.5,100.5);
  hResidueUInGeomatrixVsTrackPerEvent->SetXTitle("# tracks / event");
  hResidueUInGeomatrixVsTrackPerEvent->SetYTitle("Residue (#mum)");
  hResidueUInGeomatrixVsTrackPerEvent->SetLineColor(4);
  hResidueUInGeomatrixVsTrackPerEvent->SetLineWidth(2);
  hResidueUInGeomatrixVsTrackPerEvent->SetMarkerColor(4);
  hResidueUInGeomatrixVsTrackPerEvent->SetMarkerStyle(20);
  hResidueUInGeomatrixVsTrackPerEvent->SetStats(false);

  hResidueVInGeomatrixVsTrackPerEvent = new TH1F("hResidueVInGeomatrixVsTrackPerEvent",
						 "Residue in Geomatrix vs Track per event",
						 100,0.5,100.5);
  hResidueVInGeomatrixVsTrackPerEvent->SetXTitle("# tracks / event");
  hResidueVInGeomatrixVsTrackPerEvent->SetYTitle("Residue (#mum)");
  hResidueUInGeomatrixVsTrackPerEvent->SetLineColor(2);
  hResidueUInGeomatrixVsTrackPerEvent->SetLineWidth(2);
  hResidueUInGeomatrixVsTrackPerEvent->SetMarkerColor(2);
  hResidueUInGeomatrixVsTrackPerEvent->SetMarkerStyle(20);
  hResidueUInGeomatrixVsTrackPerEvent->SetStats(false);

  //----------------------------------------------------------------------------
  //----- Charges, noise, S/N, pixel multiplicity plots for ALL hits
  //----------------------------------------------------------------------------
  
  hseedQvsS2NAll = new TH2F("hseedQvsS2NAll","Qseed vs S/N seed, ALL hits",ChargePixNbins,0,ChargePixRange,SNRNbins,0,SNRRange);

  hQseedAll = new TH1F("hQseedAll","Pixel charge in seeds, ALL hits",ChargePixNbins,0.,ChargePixRange);

  hAllS2N = new TH1F("hAllS2N","Seed S/N, ALL HITS",SNRNbins,0.,SNRRange);
  
  hallSNneighbour =  new TH1F("hallSNneighbour"," S/N Neighbours (wo seed) for ALL hits",SNRNbins/2,0.0,SNRRange/2); 
  
  hallSN_vs_SNN = new TH2F("hallSN_vs_SNN","S/N vs S/N Neigh for ALL hits",SNRNbins,0.0,SNRRange,SNRNbins,0.0,SNRRange);
  
  hallSN_minus_hgoodSN_vs_SNN = new TH2F("hallSN-hgoodSN_vs_SNN","substract ALL hits and GOOD hits",SNRNbins,0.0,SNRRange,SNRNbins,0.0,SNRRange);

  hS2N2All = new TH2F("hS2N2All","Signal-to-noise of center vs 2nd, ALL hits",SNRNbins,0,SNRRange,SNRNbins,0,SNRRange);

  hS2N2nd = new TH1F("hS2N2nd","Signal-to-noise for 2nd pixel, ALL hits",SNRNbins,0,SNRRange);

  Char_t titre1[50] ; 
  Char_t nom1[50]; 
  for (Int_t i=0 ; i<2 ; i++) {
    sprintf(nom1,"etal%d",i+1); 
    sprintf(titre1,"Neighbours (wo seed) charge (%d), ALL hits ",i+1);  // ??
    etal[i]=new TH1F (nom1,titre1,ChargeNbins,-100.0,ChargeRange); 
    etal[i]->SetXTitle("Charge (electrons)");
    sprintf(nom1,"etal1%d",i+1); 
    sprintf(titre1,"Cluster charge / Seed charge - 1. (%d), ALL hits",i+1); 
    etal1[i]=new TH1F (nom1,titre1,20,0.0,3.0); 
    etal1[i]->SetXTitle("ratio of charges");
  }

  //------------------------------------------------------------------------------
  //----- Hit position in pixel matrix for ALL hits
  //------------------------------------------------------------------------------

  h2dallhits   = new TH2F("h2dallhits","ALL hits HUdig HVdig",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
  h2dallhits->SetXTitle("X (#mum)");
  h2dallhits->SetYTitle("X (#mum)");
  

  //----------------------------------------------------------------------------
  //----- Charges, noise, S/N, pixel multiplicity plots for GOOD hits
  //----------------------------------------------------------------------------
  
  hnpix  = new TH1F("npix","Nb of pixels in GOOD cluster",MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5);
  hnpix->SetXTitle("# pixels / cluster");

  hgoodSNneighbour =  new TH1F("hgoodSNneighbour"," S/N Neighbours (wo seed) for GOOD hits",SNRNbins/2,0.0,SNRRange/2); 

  hgoodSN_vs_SNN = new TH2F("hgoodSN_vs_SNN","S/N vs S/N Neighbours for GOOD hits",SNRNbins,0.0,SNRRange,SNRNbins/2,0.0,SNRRange/2);

  //------------------------------------------------------------------------------
  //----- Hit position in pixel matrix for GOOD hits
  //------------------------------------------------------------------------------

  h2dgoodhits   = new TH2F("h2dgoodhits","GOOD hits map",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
  h2dgoodhits->SetXTitle("Hit U digital (#mum)");
  h2dgoodhits->SetYTitle("Hit V digital (#mum)");

  hgoodSeedPixel = new TH1F("hgoodSeed","Seed Pixel index for GOOD clusters",20000,0,NofPixels);

  h2DgoodSeedPixel = new TH2F("h2DgoodSeedPixel","Seed pixel map for GOOD clusters",NofPixelInRaw,0,NofPixelInRaw,NofPixelInColumn,0,NofPixelInColumn);
  h2DgoodSeedPixel->SetXTitle("U pixel index");
  h2DgoodSeedPixel->SetYTitle("V pixel index");
  
  hSeedBetweenDist = new TH1F( "hseedbetweendist", "Distance between seed pix of same event", (Int_t)(TMath::Min(128, NofPixelInRaw)*sqrt(2.)), 0, NofPixelInRaw*sqrt(2.));
  hSeedBetweenDist->SetXTitle("distance in pixel index");
  hSeedBetweenDist->SetFillColor(29);

  
  //------------------------------------------------------------------------------
  //----- Charges, noise, S/N, pixel multiplicity plots for SELECTED/MATCHED clusters
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: matched hits" << endl;
  
  hdummy  = new TH1F("hdummy","",10,0.,10.0);

  // seed
  
  hChargeInSeed = new TH1F("hChargeInSeed","Seed pixel charge, MATCHED hits",ChargePixNbins,0.,ChargePixRange);
  hChargeInSeed->SetFillColor(29);
  hChargeInSeed->SetXTitle("Charge (electrons)");
  hRealTrackNoise = new TH1F("hRTN","Seed pixel noise, MATCHED hits", NoiseNbins, 0.,NoiseRange);
  hRealTrackNoise->SetFillColor(29);
  hRealTrackNoise->SetXTitle("Noise (electrons)");
  hRealTrackNoise_time = new TH2F("hRealTrackNoise_time","Seed Noise vs ievt, MATCHED hits",500,0,500000,NoiseNbins,0.0,NoiseRange);
  hRealTrackNoise_time->SetXTitle("Noise (electrons)");
  hSNReal = new TH1F("hSNReal","Seed (highest S/N) pixel S/N, MATCHED hits",SNRNbins,0.,SNRRange); 
  hSNReal->SetFillColor(29);
  hSNReal->SetXTitle("Seed pixel SNR");
  
  // 2nd higest S/N pixel
  
  hS2N2ndRH = new TH1F("hS2N2ndRH","S/N for 2nd highest charge pixel, MATCHED hits",SNRNbins,0.,SNRRange);
  hS2N2ndRH->SetFillColor(29);
  hSNNReal =  new TH1F("hSNNReal","S/N for 2nd highest S/N pixel, MATCHED hits",SNRNbins,0.,SNRRange); 
  hSNNReal->SetXTitle("S/N pixel with 2nd highest S/N");
  hSNNReal->SetFillColor(29);
  
  hind  = new TH1F("hind","Index in cluster pixel1- pixel0, MATCHED hits",200,-100,100);
    
  // all pixels mixed
  
  hChargeInCluster = new TH1F("hChargeInCluster","all pixel charges in MATCHED hits",ChargeNbins,0.,ChargeRange);
  hChargeInCluster->SetFillColor(29);
  
  // each pixel separately
  
  for (Int_t i=0 ; i<MaxNofPixelsInCluster ; i++) {
    sprintf(nom,"hqc%d",i+1); 
    sprintf(titre," Sum of %d charge ordered pixels, MATCHED hits",i+1); 
    hqcn[i]=new TH1F (nom,titre,ChargeNbins,.0,ChargeRange);
    hqcn[i]->SetXTitle("charge (electrons)");
    hqcn[i]->SetFillColor(29);

    sprintf(nom,"hqcgeom%d",i+1); 
    sprintf(titre," Sum of %d geometrically ordered pixels, MATCHED hits",i+1); 
    hqcngeom[i]=new TH1F (nom,titre,ChargeNbins,.0,ChargeRange); 
    hqcngeom[i]->SetXTitle("charge (electrons)");
    hqcngeom[i]->SetFillColor(29);
    
    sprintf(nom,"hindivq%d",i+1); 
    if(i==0){
      sprintf(titre,"Charge seed pixel ordered by charge, MATCHED hits"); 
    }else{
      sprintf(titre,"Charge %dth pixel ordered by charge, MATCHED hits",i+1); 
    }
    sprintf(titre,"Charge in %dth pixel ordered by charge, MATCHED hits",i+1); 
    hindivq[i]=new TH1F (nom,titre,1.05*ChargePixNbins,-0.05*ChargePixRange,ChargePixRange); 
    hindivq[i]->SetXTitle("pixel charge (electrons)");
    hindivq[i]->SetFillColor(29);
    
    sprintf(nom,"hsn%d",i+1); 
    if(i==0){
      sprintf(titre,"S/N seed pixel ordered by S/N, MATCHED hits"); 
    }else{
      sprintf(titre,"S/N %dth pixel ordered by S/N, MATCHED hits",i); 
    }
    hsnn[i]=new TH1F (nom,titre,SNRNbins,0.0,SNRRange);
    hsnn[i]->SetXTitle("pixel SNR");
    hsnn[i]->SetFillColor(29);
  }
  
  hChargeCor_1_2 = new TH2F("hChargeCor_1_2","Ch pixel 1 vs pixel 2 (q order)",ChargePixNbins,.0,ChargePixRange,ChargePixNbins,.0,ChargePixRange);
  hChargeCor_1_2->SetXTitle("Q 2nd highest (electrons");
  hChargeCor_1_2->SetYTitle("Q 1st highest (electrons");
  hChargeCor_1_3 = new TH2F("hChargeCor_1_3","Ch pixel 1 vs pixel 3 (q order)",ChargePixNbins,.0,ChargePixRange,ChargePixNbins,.0,ChargePixRange);
  hChargeCor_1_3->SetXTitle("Q 3rd highest (electrons");
  hChargeCor_1_3->SetYTitle("Q 1st highest (electrons");
  hChargeCor_1_4 = new TH2F("hChargeCor_1_4","Ch pixel 1 vs pixel 4 (q order)",ChargePixNbins,.0,ChargePixRange,ChargePixNbins,.0,ChargePixRange);
  hChargeCor_1_4->SetXTitle("Q 4th highest (electrons");
  hChargeCor_1_4->SetYTitle("Q 1st highest (electrons");
  hChargeCor_2_3 = new TH2F("hChargeCor_2_3","Ch pixel 2 vs pixel 3 (q order)",ChargePixNbins,.0,ChargePixRange,ChargePixNbins,.0,ChargePixRange);
  hChargeCor_2_3->SetXTitle("Q 3rd highest (electrons");
  hChargeCor_1_3->SetYTitle("Q 2nd highest (electrons");

  hChargeRap_1_over_2 = new TH1F("hChargeRap_1_over_2","Ratio of 1st over 2nd pixel(q order)",100,0.0,10.0);
  hChargeRap_1_over_2->SetFillColor(29);
  hChargeRap_1_over_3 = new TH1F("hChargeRap_1_over_3","Ratio of 1st over 3rd pixel(q order)",100,0.0,10.0);
  hChargeRap_1_over_3->SetFillColor(29);
  hChargeRap_1_over_4 = new TH1F("hChargeRap_1_over_4","Ratio of 1st over 4th pixel(q order)",100,0.0,10.0);
  hChargeRap_1_over_4->SetFillColor(29);
  hChargeRap_2_over_3 = new TH1F("hChargeRap_2_over_3","Ratio of 2nd over 3rd pixel(q order)",100,0.0,10.0);
  hChargeRap_2_over_3->SetFillColor(29);
  
  h_SNRratioL           = new TH1F("h_SNRratioL","Ratio SNR(seed)/SNR(2nd highest q)",250,-10,10);
  
  hQofPix3x3 = new TH1F("hQofPix3x3","Mixed pixel charge in 3x3",ChargeNbins,.0,ChargeRange);
  hQ3x34 = new TH1F("hQ4","Q3x3[4]",ChargeNbins,.0,ChargeRange);
  hQ3x35 = new TH1F("hQ5","Q3x3[4]",ChargeNbins,.0,ChargeRange);
  hQ3x345 = new TH1F("hQ45","Q3x3[4]-Q3x3[5]",40,-400,400);
  hQ3x327 = new TH1F("hQ27","Q3x3[2]-Q3x3[7]",40,-400,400);
  hQ3x345r = new TH1F("hQ45r","(q4-q5)/(q4+q5)",40,-1,1);
  hQ3x327r = new TH1F("hQ27r","(q2-q7)/q2+q7)",40,-1,1);

  
  // cluster wise 
  
  hIndex2x2  = new TH1F("hIndex2x2","Index of cluster 2x2",5,0.0,5.0);
  hsnc = new TH1F ("hsnc","S/N optimized, cluster restricted for highest S/N track-MATCHED",SNRNbins,0.,SNRRange); 
  hsnc->SetXTitle("S/N seed");
  hsnc->SetFillColor(29);
  hChargeSum_4 = new TH1F("hChargeSum_4","Sum of the 4 first pixel from TTree",ChargeNbins,0.0,ChargeRange);
  hChargeSum_4->SetXTitle("Charge (electrons)");
  hqc_c    = new TH1F("qc_c","Cluster charge for MATCHED hits",ChargeNbins,.0,ChargeRange);
  hqc_c->SetXTitle("Charge (ADC)");
  hqc_c->SetFillColor(29);
  hqc    = new TH1F("qc","Cluster charge (calib factor) for MATCHED hits",ChargeNbins,.0,ChargeRange);
  hqc->SetXTitle("Charge (electrons)");
  hChargeInCross = new TH1F("hChargeInCross", "Charge in the 4 direct neighbours to the seed (cross) for MATCHED",ChargeNbins,.0,ChargeRange);
  hChargeInCross->SetXTitle("Charges (electrons)");
  hChargeInCross->SetFillColor(29);
  hChargeInX = new TH1F("hChargeInX", "Charge in the 4 corners to the seed (X) for MATCHED",ChargeNbins,.0,ChargeRange);
  hChargeInCrown1 = new TH1F("hChargeInCrown1", "Charge in the first crown for MATCHED",ChargeNbins,.0,ChargeRange);
  hChargeInCrown2 = new TH1F("hChargeInCrown2", "Charge in the second crown for MATCHED",ChargeNbins,.0,ChargeRange);
  hsnc1  = new TH1F("snc1","Cluster signal / seed noise",SNRNbins,0.,SNRRange);
  hqcel  = new TH1F("qcel","Charge in optimized(largest S/N) cluster track-MATCHED",ChargeNbins,.0,ChargeRange);
  hoptimalsize  = new TH1F("optimalsize","Nb of pixels in optimized(largest S/N) cluster track-MATCHED",MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5);
  hSNneighbour =  new TH1F("hSNneighbour"," S/N Neighbours (wo seed) for track-MATCHED hits",SNRNbins/2,0.,SNRRange/2); 
  hSNneighbour->SetXTitle("S/N neighbours");
  
  hnpix_c  = new TH1F("npix_c",
		      "nb of pixels in MATCHED hits",
		      MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5); // space replaced by _, JB 2009/09/01  YV 03/11/09 changed the upper limit of the histo - 11 was too low
  hnpix_c->SetXTitle("# pixels / cluster");
  hnpix_c->SetFillColor(29);
  
  hNomEffic = new TH1F("hNomEffic","Nominal value of corrected efficiency",1,0.0,1.0);

  //AP 2015/05/30, Histograms with the number of withdows (2x4) needed for each associated cluster
  int    Nbins_Windows = 10;
  double R_Windows[2];
  R_Windows[0] = -0.5;
  R_Windows[1] =  Nbins_Windows + 0.5;
  Nbins_Windows++;
  hnWindows_c  = new TH1F("hnWindows_c",
			  "nb of 2x4 pixels window to contain a MATCHED hit",
			  Nbins_Windows,R_Windows[0],R_Windows[1]);
  hnWindows_c->SetXTitle("# windows / cluster");
  hnWindows_c->SetLineColor(4);
  hnWindows_c->SetLineWidth(2);

  int NcolWindow = 4;
  int NlinWindow = 2;
  int Nbins_2DWindows_col = NofPixelInColumn/NcolWindow;
  double R_2DWindows_col[2];
  R_2DWindows_col[0] = -0.5;
  R_2DWindows_col[1] = -0.5 + Nbins_2DWindows_col;
  int Nbins_2DWindows_lin = NofPixelInRaw/NlinWindow;
  double R_2DWindows_lin[2];
  R_2DWindows_lin[0] = -0.5;
  R_2DWindows_lin[1] = -0.5 + Nbins_2DWindows_lin;
  h2DWindows = new TH2F("h2DWindows",
			"2D super bins for Suze window",
			Nbins_2DWindows_col,R_2DWindows_col[0],R_2DWindows_col[1],
			Nbins_2DWindows_lin,R_2DWindows_lin[0],R_2DWindows_lin[1]);
  h2DWindows->SetXTitle("col(super-window)");
  h2DWindows->SetYTitle("lin(super-window)");

  hnpixCumul_c  = new TH1F("npixcumul_c","rate of hits with at least #pixels in MATCHED hits",hnpix_c->GetNbinsX(), hnpix_c->GetXaxis()->GetXmin(), hnpix_c->GetXaxis()->GetXmax()); // JB, 2014/11/13 
  hnpixCumul_c->SetXTitle("# pixels / cluster");
  hnpixCumul_c->SetFillColor(29);


  //Histograms related with duplicated hits associated to a track: try to quantify the effect of te duplicated hits (same position but different time-stamps)
  hDuplicate_2DMult = new TH2F("hDuplicate_2DMult",
			       "Cluster multiplicity of earliest vs latest duplicated hits",
			       MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5,
			       MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5);
  hDuplicate_2DMult->SetXTitle("# pixels/cluster (Earliest)");
  hDuplicate_2DMult->SetYTitle("# pixels/cluster (Latest)");

  hDuplicate_DeltaTS = new TH1F("hDuplicate_DeltaTS",
				"#DeltaT_{Stamp} of duplicated hits",
				21,
				-0.5,20.5);
  hDuplicate_DeltaTS->SetXTitle("Duplicated hits #DeltaT_{Stamp}");

  hDuplicate_npixc = new TH1F("hDuplicate_npixc",
			      "Probability of duplicated hits vs cluster multiplicity earliest MATCHED hit",
			      MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5);
  hDuplicate_npixc->SetXTitle("# pixels/cluster");
  hDuplicate_npixc->SetYTitle("Prob. (100%)");

  hDuplicate_npixc_vs_TrkDistToDiode = new TH2F("hDuplicate_npixc_vs_TrkDistToDiode",
						"Probability of duplicated hits vs cluster multiplicity vs Track-distance to diode of earliest MATCHED hit",
						MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5,
						50,0.0,0.7*sqrt(pow(PixelSizeU,2) + pow(PixelSizeV,2)));
  hDuplicate_npixc_vs_TrkDistToDiode->SetXTitle("# pixels/cluster");
  hDuplicate_npixc_vs_TrkDistToDiode->SetYTitle("Distance(trk - closest diode) (#mum)");

  // Correlation between different charges, S/N and noises
  
  hSN_vs_SNNReal = new TH2F("hSN_vs_SNNReal","Seed S/N vs 2nd highest S/N, MATCHED hits",SNRNbins/4,0.0,SNRRange,SNRNbins/4,0.0,SNRRange);
  hSN_vs_SNNReal->SetXTitle("S/N pixel with 2nd highest S/N");
  hSN_vs_SNNReal->SetYTitle("S/N seed");
  hS2N2RH = new TH2F("hS2N2RH","Seed S/N vs 2nd (chare ordered), MATCHED hits",SNRNbins/4,0.0,SNRRange,SNRNbins/4,0.0,SNRRange);
  hseedQvsS2NGood = new TH2F("hseedQvsS2NGood","Charge vs S/N seed, MATCHED hits",SNRNbins/4,0,SNRRange,ChargePixNbins/4,0,ChargePixRange);
  hseedQvsS2NGood->SetXTitle("S/N");  
  hseedQvsS2NGood->SetYTitle("Charge (electrons)");  
  hSNseedvsSNneighbour =  new TH2F("hSNseedvsSNneighbour","S/N Neighbours (wo seed) vs S/N seed for track-MATCHED hits",SNRNbins,0.,SNRRange,SNRNbins/2,0.,SNRRange/2); // JB 2013/11/07
  hSNseedvsSNneighbour->SetXTitle("S/N seed");
  hSNseedvsSNneighbour->SetYTitle("S/N neighbours");
  hQseedvsQcluster =  new TH2F("hQseedvsQcluster","Cluster charge vs seed charge for track-MATCHED hits",ChargePixNbins/4,0,ChargePixRange,ChargePixNbins/4,0,ChargePixRange); // JB 2014/05/22
  hQseedvsQcluster->SetXTitle("Charge on seed (electrons)");
  hQseedvsQcluster->SetYTitle("Charge on cluster (electrons)");
  hQseedvsQneighbour =  new TH2F("hQseedvsQneighbour","Neighbours charge (wo seed) vs seed charge for track-MATCHED hits",ChargePixNbins/4,0,ChargePixRange,ChargePixNbins/4,0,ChargePixRange); // JB 2013/11/08
  hQseedvsQneighbour->SetXTitle("Charge on seed (electrons)");
  hQseedvsQneighbour->SetYTitle("Charge on neighbours (electrons)");
  hSNseedvsQcluster =  new TH2F("hSNseedvsQcluster","Cluster charge vs seed S/N for track-MATCHED hits",SNRNbins,0.,SNRRange,ChargePixNbins/4,0,ChargePixRange); // JB 2014/05/22
  hSNseedvsQcluster->SetYTitle("cluster charge(electrons)");
  hSNseedvsQcluster->SetXTitle("S/N seed");
  
  Int_t i=0;
  for (Int_t j=0 ; j<jpixmax ; j++) {    
    sprintf(nom,"hsn_pix_%d_%d",i,j+1); 
    sprintf(titre,"S/N cluster %d-%d ",i,j+1); 
    hsn_pix_0[j] = new TH1F (nom,titre,SNRNbins,0.0,SNRRange); 
  }
  
  i = 1;
  for (Int_t j=0 ; j<jpixmax ; j++) {    
    sprintf(nom,"hsn_pix_%d_%d",i,j+1); 
    sprintf(titre,"S/N cluster %d-%d ",i,j+1); 
    hsn_pix_1[j] = new TH1F (nom,titre,SNRNbins,0.0,SNRRange); 
  }
  
  // Signal / noise seed vs Signal / noise in the pixel range [i-j] 
  i=0;
  for (Int_t j=0 ; j<jpixmax ; j++) {    
    sprintf(nom,"hsn_seed_vs_pix_%d_%d",i,j+1); 
    sprintf(titre,"S/N clusterseed vs S/N %d-%d ",i,j+1); 
    hsn_seed_vs_pix_0[j] = new TH2F(nom,titre,SNRNbins/4,0.0,SNRRange,SNRNbins/4,0.0,SNRRange);
  }
  
  i=1;
  for (Int_t j=0 ; j<jpixmax ; j++) {    
    sprintf(nom,"hsn_seed_vs_pix_%d_%d",i,j+1); 
    sprintf(titre,"S/N clusterseed vs S/N %d-%d ",i,j+1); 
    hsn_seed_vs_pix_1[j] = new TH2F(nom,titre,SNRNbins/4,0.0,SNRRange,SNRNbins/4,0.0,SNRRange);
  }
  
  //----ALL HITS: Signal/noise seed vs Signal/noise in the pixel range [i-j] 
  i=0;
  for (Int_t j=0 ; j<jpixmax ; j++) {    
    sprintf(nom,"hsnALL_seed_vs_pix_%d_%d",i,j+1); 
    sprintf(titre,"ALL S/N clusterseed vs S/N %d-%d ",i,j+1); 
    hsnALL_seed_vs_pix_0[j] = new TH2F(nom,titre,SNRNbins/4,0.0,SNRRange,SNRNbins/4,0.0,SNRRange);
  }
  i=1;
  //TH2F *hsnALL_seed_vs_pix_1[jpixmax];
  for (Int_t j=0 ; j<jpixmax ; j++) {    
    sprintf(nom,"hsnALL_seed_vs_pix_%d_%d",i,j+1); 
    sprintf(titre,"ALL S/N clusterseed vs S/N %d-%d ",i,j+1); 
    hsnALL_seed_vs_pix_1[j] = new TH2F(nom,titre,SNRNbins/4,0.0,SNRRange,SNRNbins/4,0.0,SNRRange);
  }
  

  //------------------------------------------------------------------------------
  //----- Charges, noise, S/N, pixel multiplicity plots for UNMATCHED clusters
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: unmatched hits" << endl;
  
  hqc_nc   = new TH1F("qc nc","Cluster charge for track-UNMATCHED hits",ChargeNbins,0.0,ChargeRange);
    
  hnpix_nc = new TH1F("npix_nc","nb pix in track-UNMATCHED cluster",MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5); // space replaced by _, JB 2009/09/01
  hnpix_nc->SetXTitle("# pixels");


  //------------------------------------------------------------------------------
  //----- Comparebadgoodraw() Analysis of raw frame data 
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: Comparebadgoodraw" << endl;
  
  hPedestal = new TH1F("hPedestal","Pedestal", 100, -10.,10.) ; // never filled !
  
  hraw1badone = new TH1F("hrawbadone","Frame 1 before CDS BAD",3500,0.0,3500.0);
  hraw1goodone = new TH1F("hrawgoodone","Frame 1 before CDS GOOD",3500,0.0,3500.0);
  hraw1goodone_time = new TH2F("hraw1goodone_time","raw signal before CDS GOOD vs ievt",500,0,500000,2000,0.0,4000.0);
  hraw1badone_time = new TH2F("hraw1badone_time","raw signal before CDS BAD vs ievt",500,0,500000,2000,0.0,4000.0);
  hraw1goodoneNoise_time = new TH2F("hraw1goodoneNoise_time","Noise (e-) GOOD vs ievt",500,0,500000,200,0.0,NoiseRange);
  hraw1badoneNoise_time = new TH2F("hraw1badoneNoise_time","Noise (e-) BAD vs ievt",500,0,500000,200,0.0,NoiseRange);
  
  
  hraw1PFrfr1GOOD = new TH1F("hraw1PFrfr1GOOD","Average Frame 1  (ADC) before CDS GOOD",3500,0.0,3500.0);
  hraw1PFrfr1BAD = new TH1F("hraw1PFrfr1BAD","Average Frame 1 (ADC)  before CDS BAD",3500,0.0,3500.0);
  hraw1PFrfr1GOOD_time = new TH2F("hraw1PFrfr1GOOD_time","Average Frame 1 (ADC)  before CDS GOOD vs ievt",500,0,500000,2000,0.0,4000.0);
  hraw1PFrfr1BAD_time = new TH2F("hraw1PFrfr1BAD_time","Average Frame 1  (ADC) before CDS BAD vs ievt",500,0,500000,2000,0.0,4000.0);
  hraw1PFrfr2GOOD = new TH1F("hraw1PFrfr2GOOD","Average Frame 2 (ADC)  before CDS GOOD",3500,0.0,3500.0);
  hraw1PFrfr2BAD = new TH1F("hraw1PFrfr2BAD","Average Frame 2 (ADC)  before CDS BAD",3500,0.0,3500.0);
  hraw1PFrfr2GOOD_time = new TH2F("hraw1PFrfr2GOOD_time","Average Frame 2  (ADC) before CDS GOOD vs ievt",500,0,500000,2000,0.0,4000.0);
  hraw1PFrfr2BAD_time = new TH2F("hraw1PFrfr2BAD_time","Average Frame 2  (ADC) before CDS BAD vs ievt",500,0,500000,2000,0.0,4000.0);
  hraw1NoiseGOOD_time = new TH2F("hraw1NoiseGOOD_time","Average Noise  (ADC) GOOD vs ievt",500,0,500000,200,0.0,10.0);
  hraw1NoiseBAD_time = new TH2F("hraw1NoiseBAD_time","Average Noise  (ADC) BAD vs ievt",500,0,500000,200,0.0,10.0);
  
  hraw1Noise = new TH1F("hraw1Noise","Average Noise (ADC)",200,0.0,10.0);
  hraw1Pedestal = new TH1F("hraw1Pedestal","Average Pedestal (ADC)",401,-20.0,20.0);
  hraw1CDS  = new TH1F("hraw1CDS","Average CDS (ADC)",401,-20.0,20.0);
  hraw1Signal  = new TH1F("hraw1Signal","Average Signal  (ADC) (CDS-Ped-Com)",801,-40.0,40.0);
  hraw1Noise_time = new TH2F("hraw1Noise_time","Average Noise (ADC) vs ievt",500,0,500000,200,0.0,10.0);
  hraw1Pedestal_time = new TH2F("hraw1Pedestal_time","Average Pedestal  (ADC) vs ievt",500,0,500000,401,-20.0,20.0);
  hraw1CDS_time  = new TH2F("hraw1CDS_time","Average CDS  (ADC) vs ievt",500,0,500000,401,-20.0,20.0);
  hraw1Signal_time  = new TH2F("hraw1Signal_time","Average Signal  (ADC) (CDS-Ped-Com) vs ievt",500,0,500000,401,-40.0,40.0);
  
  
  //------------------------------------------------------------------------------
  //--- Charges for calibration peak
  //------------------------------------------------------------------------------
  if( ifCalibration ) { // If calibration histograms required
    if( HistDebug) cout << "HistoBooking: calibration peak" << endl;

    hqSeedCalibration = new TH1F( "hqSeedCalibration", "Seed pixel charge for calibration", ChargeNbins,-100.0,ChargeRange);
    hqSeedCalibration->SetXTitle("ADC counts");
    
    hqNeighbourCalibration = new TH1F( "hqNeighbourCalibration", "Neighbour pixels charge sum for calibration", ChargeNbins,-100.0,ChargeRange);
    hqNeighbourCalibration->SetXTitle("ADC counts");
    
    hqSeedVsNeighbourCalibration = new TH2F( "hqSeedVsNeighbourCalibration", "Neibourgs charge sum vs Seed pixel charge for calibration",ChargeNbins,-100.0,ChargeRange, ChargeNbins,-100.0,ChargeRange);
    hqSeedVsNeighbourCalibration->SetXTitle("Seed charge(ADC)");
    hqSeedVsNeighbourCalibration->SetYTitle("Neighbours charge sum (ADC)");
    
  } // end if calibration histograms required
  
  
  //------------------------------------------------------------------------------
  //---- cluster shape study, JB 2010/04/13 based on cdritsa stuff
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: cluster shape" << endl;
  
  int Nbins_U = 7;
  double R_U[2];
  R_U[0] = -(double)Nbins_U/2;
  R_U[1] =  (double)Nbins_U/2;
  int Nbins_V = 7;
  double R_V[2];
  R_V[0] = -(double)Nbins_V/2;
  R_V[1] =  (double)Nbins_V/2;
  //hClusterMeanForm     = new TH2F("ClShape","Cluster Average Shape",7,-3,4,7,-3,4);
  hClusterMeanForm     = new TH2F("ClShape","Cluster Average Shape",Nbins_U,R_U[0],R_U[1],Nbins_V,R_V[0],R_V[1]);
  hClusterMeanForm->SetXTitle("U pixel index - seed index");
  hClusterMeanForm->SetYTitle("V pixel index - seed index");
  hClusterMeanForm->SetZTitle("average charge");

  hClusterTest1         = new TH1F("ClusTest1","Pixel row wrt seed",7,-3,4);
  hClusterTest1->SetXTitle("V pixel index - seed index");
  hClusterTest2         = new TH1F("ClusTest2","Pixel column wrt seed",7,-3,4);
  hClusterTest2->SetXTitle("U pixel index - seed index");
  
  hClusterSizeInLines   = new TH1F("ClSizeLines","Cluster size in #lines",9,-0.5,8.5);
  hClusterSizeInLines->SetXTitle("Nb of lines");
  hClusterSizeInColumns = new TH1F("ClSizeColumns","Cluster size in #columns",9,-0.5,8.5);
  hClusterSizeInColumns->SetXTitle("Nb of columns");

  hClusterSize2DInLineAndColumns = new TH2F("ClSizeLinesAndColumns",
					    "Cluster size in #lines and #columns",
					    9,-0.5,8.5,
					    9,-0.5,8.5);
  hClusterSize2DInLineAndColumns->SetXTitle("Nb of lines");
  hClusterSize2DInLineAndColumns->SetYTitle("Nb of columns");


  hClusterTypes = new TH1F("ClType","Geometrical types up to 2x2 clusters", 13, 0, 13); // JB 2014/03/31
  const char* clusterType[13] = { "", "1 pix", "2 pix row", "2 pix col", "3 pix L", "3 pix row", "3 pix col", "4 pix square", "4 pix L-row", "4 pix L-col", "4 others", "more than 4", ""};
  for ( Int_t i=1; i<=13; i++) {
      hClusterTypes->GetXaxis()->SetBinLabel( i, clusterType[i-1]);
  }

  hClusterTypesBeyond4 = new TH1F("ClTypeB4","Geometrical types larger than 2x2 clusters", 9, 0, 9); // JB 2015/05/12
  const char* clusterTypeBeyond4[9] = { "", "5 pix square+row", "5 pix square+col", "5 others", "6 pix 3c2r", "6 pix 2c3r", "6 others", "more than 6", ""};
  for ( Int_t i=1; i<=9; i++) {
    hClusterTypesBeyond4->GetXaxis()->SetBinLabel( i, clusterTypeBeyond4[i-1]);
  }

  
  Char_t name[50], title[200];
  for( Int_t i=0; i<10; i++) {
    
    sprintf( name, "multipl%d", i);
    sprintf( title, "cluster multiplicity with Charge>[%d] e", i);
    hCountPixels[i] = new TH1F(name, title ,MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5);
    hCountPixels[i]->SetXTitle("# pixels");
    hCountPixels[i]->SetFillColor(29);
    
    sprintf( name, "pxlOverThresh%d", i);
    sprintf( title, "Percentage of pixels over threshold (SNR>[%d])", i);
    hPixelsOverSNR[i] = new TH2F( name, title, 7,-3,4,7,-3,4);
    hPixelsOverSNR[i]->SetXTitle("U pixel index - seed index");;
    hPixelsOverSNR[i]->SetYTitle("V pixel index - seed index");
    hPixelsOverSNR[i]->SetZTitle("(%)");

    
    sprintf( name, "pxlOverCharge%d", i);
    sprintf( title, "Percentage of pixels over threshold (Charge>[%d])", i);
    hPixelsOverCharge[i] = new TH2F( name, title, 7,-3,4,7,-3,4);
    hPixelsOverCharge[i]->SetXTitle("U pixel index - seed index");;
    hPixelsOverCharge[i]->SetYTitle("V pixel index - seed index");
    hPixelsOverCharge[i]->SetZTitle("(%)");

    sprintf( name, "hMultVsFormFactor%d", i);
    sprintf( title, "Form factor vs cluster multiplicity with SNR>[%d]", i);
    hMultVsFormFactor[i] = new TH2F( name, title, 50,0,10, MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5);
    hMultVsFormFactor[i]->SetXTitle("# pixels");
    hMultVsFormFactor[i]->SetYTitle("# pix(Central row) / # pix(central col)");
    hMultVsFormFactor[i]->SetFillColor(29);
    
    sprintf( name, "hMultVsFormFactor1D%d", i);
    sprintf( title, "cluster multiplicity with SNR>[%d]", i);
    hMultVsFormFactor1D[i] = new TH1F( name, title, MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5);
    hMultVsFormFactor1D[i]->Sumw2(); //clm added back clu mult vs S/N 2013.08.25
    hMultVsFormFactor1D[i]->SetXTitle("# pixels");
    hMultVsFormFactor1D[i]->SetFillColor(29);
    
    sprintf( name, "Cl%d", i);
    sprintf( title, "Cluster %d", i);
    Cluster[i]     = new TH2F( name, title ,7,-3,4,7,-3,4);
    Cluster[i]->SetXTitle("U pixel index - seed index");
    Cluster[i]->SetYTitle("V pixel index - seed index");
    Cluster[i]->SetZTitle("average charge");

  }
  
  hChargeCoG_Correl     = new TH2F("hChargeCoG_Correl"  , "SUM(Q*Pos)/totQ U vs V"  ,250,0,10,250,0,10);
  hChargeCoG_Correl->SetXTitle("(#mum)");
  hChargeCoG_Correl->SetYTitle("(#mum)");
  hChargeCoG_Correl2    = new TH2F("hChargeCoG_Correl2" , "SUM(Q*Pos)mean/totQ U vs V" ,250,0,10,250,0,10);
  hChargeCoG_Correl2->SetXTitle("(#mum)");
  hChargeCoG_Correl2->SetYTitle("(#mum)");
  
  hChargeDistrInLine    = new TH1F("hChargeDistrInLine","hChargeDistrInLine",210,-10,10);
  
  hChargeDistrIn3rdLeftNeigh = new TH1F("h3rdLeftNeigh","h3rdLeftNeigh",ChargePixNbins,0.0,ChargePixRange);
  hChargeDistrIn2ndLeftNeigh = new TH1F("h2ndLeftNeigh","h2ndLeftNeigh",ChargePixNbins,0.0,ChargePixRange);
  hChargeDistrIn1stLeftNeigh = new TH1F("h1stLeftNeigh","h1stLeftNeigh",ChargePixNbins,0.0,ChargePixRange);
  hChargeDistrInSeed = new TH1F("InSeed","InSeed",ChargePixNbins,0.0,ChargePixRange);
  hChargeDistrIn1stRightNeigh = new TH1F("h1stRightNeigh","h1stRightNeigh",ChargePixNbins,0.0,ChargePixRange);
  hChargeDistrIn2ndRightNeigh = new TH1F("h2ndRightNeigh","h2ndRightNeigh",ChargePixNbins,0.0,ChargePixRange);
  hChargeDistrIn3rdRightNeigh = new TH1F("h3rdRightNeigh","h3rdRightNeigh",ChargePixNbins,0.0,ChargePixRange);
  
  
  // histograms needed for charge distribution function:
  Int_t nb=25; Int_t SpreadRange=100;
  hChargeIntegral = new TH2F("hChargeIntegral","",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
  hChargeIntSmoothed = new TH2F("hChargeSmoothed","Smoothed",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
  
  
  //------------------------------------------------------------------------------
  //----- histos to compute eta correction
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: positions" << endl;
  
  hEta2x2  = new TH1F("hEta2x2","Eta 2x2", 50,0.,1.);
  hEta2x2U  = new TH1F("hEta2x2U","Eta 2x2", 50,0.,1.);
  hEta2x2V  = new TH1F("hEta2x2V","Eta 2x2", 50,0.,1.);
  hEta2x2m = new TH1F("hEta2x2m","UCG2x2-Udigital", 60,-15.,15.);
  
    
  //------------------------------------------------------------------------------
  //----- Hit position in pixel matrix for MATCHED or SELECTED hits
  //------------------------------------------------------------------------------
  
  // Distribution (1D or 2D) of hits
  
  // Using the "standard" position
  huv    = new TH2F("huv","Hit map (V:U)",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
//  huv    = new TH2F("huv","Hit map (V:U)",100, 1620, 1720, 100, 120, 220);
  huv->SetXTitle("U position (#mum)");
  huv->SetYTitle("V position (#mum)");


  float R_uv_Rate_U[2];
  float R_uv_Rate_V[2];
  R_uv_Rate_U[0] = 0;
  R_uv_Rate_U[1] = 0;
  R_uv_Rate_V[0] = 0;
  R_uv_Rate_V[1] = 0;
  float division;
  int   intdivision;

  division    = geomUmax / PixelSizeU;
  intdivision = int(division);
  if (TMath::Abs(division - intdivision) > 1.0e-6 ) {
    if(division >= -1 && 
       division <= 0)       R_uv_Rate_U[1] = 0.0*PixelSizeU;
    else if(division >  0)  R_uv_Rate_U[1] = (intdivision + 1)*PixelSizeU;
    else if(division < -1)  R_uv_Rate_U[1] = (intdivision + 0)*PixelSizeU;
  }
  else R_uv_Rate_U[1] = geomUmax;
  division    = geomUmin / PixelSizeU;
  intdivision = int(division);
  if (TMath::Abs(division - intdivision) > 1.0e-6 ) {
    if(division >= -1 && 
       division <= 0)      R_uv_Rate_U[0] = -1.0*PixelSizeU;
    else if(division >  0) R_uv_Rate_U[0] = (intdivision + 0)*PixelSizeU;
    else if(division < -1) R_uv_Rate_U[0] = (intdivision - 1)*PixelSizeU;
  }
  else R_uv_Rate_U[0] = geomUmin;
  
  division    = geomVmax / PixelSizeV;
  intdivision = int(division);
  if (TMath::Abs(division - intdivision) > 1.0e-6 ) {
    if(division >= -1 && 
       division <= 0)      R_uv_Rate_V[1] = 0.0*PixelSizeV;
    else if(division >  0) R_uv_Rate_V[1] = (intdivision + 1)*PixelSizeV;
    else if(division < -1) R_uv_Rate_V[1] = (intdivision + 0)*PixelSizeV;
  }
  else R_uv_Rate_V[1] = geomVmax;
  division    = geomVmin / PixelSizeV;
  intdivision = int(division);
  if (TMath::Abs(division - intdivision) > 1.0e-6 ) {
    if(division >= -1 && 
       division <= 0)      R_uv_Rate_V[0] = -1.0*PixelSizeV;
    else if(division >  0) R_uv_Rate_V[0] = (intdivision + 0)*PixelSizeV;
    else if(division < -1) R_uv_Rate_V[0] = (intdivision - 1)*PixelSizeV;
  }
  else R_uv_Rate_V[0] = geomVmin;

  int NofPitchU = int((R_uv_Rate_U[1] - R_uv_Rate_U[0])/PixelSizeU);
  //cout << TMath::Abs(((R_uv_Rate_U[1] - R_uv_Rate_U[0])/NofPitchU) - PixelSizeU) << endl;
  if(TMath::Abs(((R_uv_Rate_U[1] - R_uv_Rate_U[0])/NofPitchU) - PixelSizeU) > 1.0e-3) {
    if(((R_uv_Rate_U[1] - R_uv_Rate_U[0])/NofPitchU) - PixelSizeU > 0)      NofPitchU += 1;
    else if(((R_uv_Rate_U[1] - R_uv_Rate_U[0])/NofPitchU) - PixelSizeU < 0) NofPitchU -= 1;
  }
  int NofPitchV = int((R_uv_Rate_V[1] - R_uv_Rate_V[0])/PixelSizeV);
  //cout << TMath::Abs(((R_uv_Rate_V[1] - R_uv_Rate_V[0])/NofPitchV) - PixelSizeV) << endl;
  if(TMath::Abs(((R_uv_Rate_V[1] - R_uv_Rate_V[0])/NofPitchV) - PixelSizeV) > 1.0e-3) {
    if(((R_uv_Rate_V[1] - R_uv_Rate_V[0])/NofPitchV) - PixelSizeV > 0)      NofPitchV += 1;
    else if(((R_uv_Rate_V[1] - R_uv_Rate_V[0])/NofPitchV) - PixelSizeV < 0) NofPitchV -= 1;
  }

  cout << "Bins U      = " << NofPitchU << endl;
  cout << "New Range U = (" << R_uv_Rate_U[0] << "," << R_uv_Rate_U[1] << ")" << endl;
  cout << "Width U     = " << (R_uv_Rate_U[1] - R_uv_Rate_U[0])/NofPitchU << endl;

  cout << "Bins V      = " << NofPitchV << endl;
  cout << "New Range V = (" << R_uv_Rate_V[0] << "," << R_uv_Rate_V[1] << ")" << endl;
  cout << "Width V     = " << (R_uv_Rate_V[1] - R_uv_Rate_V[0])/NofPitchV << endl;

  //geomUmin = R_uv_Rate_U[0];
  //geomUmax = R_uv_Rate_U[1];
  //geomVmin = R_uv_Rate_V[0];
  //geomVmax = R_uv_Rate_V[1];

  huv_rate = new TH2F("huv_rate","Rate map (V:U)",
		      NofPitchU,R_uv_Rate_U[0],R_uv_Rate_U[1],
		      NofPitchV,R_uv_Rate_V[0],R_uv_Rate_V[1]);
  huv_rate->SetXTitle("U position (#mum)");
  huv_rate->SetYTitle("V position (#mum)");
  huv_rate->SetZTitle("Rate (hits / event)");
  huv_rate->SetStats(false);

  int Npixels_max = 100;
  double R_pix_event[2];
  R_pix_event[0] = -0.5;
  R_pix_event[1] = Npixels_max + 0.5;
  h_pixels_event = new TH1F("h_pixels_event",
			    "Number of fired pixels per event inside geomatrix",
			    Npixels_max,R_pix_event[0],R_pix_event[1]);
  h_pixels_event->SetXTitle("(# fired pixels)/event");
  h_pixels_event->SetYTitle("Events");


  hhu    = new TH1F("hu","Hit U postion",H_Nbinhu,humin,humax);
//  hhu    = new TH1F("hu","Hit U postion",100, 1620, 1720);
  hhu->SetXTitle("U position (#mum)");
  hhv    = new TH1F("hv","Hit V position",H_Nbinhv,hvmin,hvmax);
//  hhv    = new TH1F("hv","Hit V position", 100, 120, 220);
  hhv->SetXTitle("V position (#mum)");
  
  hxy    = new TH2F("hxy","Hit map (Y:X)",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
  hxy->SetXTitle("X position (#mum)");
  hxy->SetYTitle("Y position (#mum)");
  hhx    = new TH1F("hx","Hit X postion",H_Nbinhu,humin,humax);
  hhx->SetXTitle("X position (#mum)");
  hhy    = new TH1F("hy","Hit Y position",H_Nbinhv,hvmin,hvmax);
  hhy->SetXTitle("Y position (#mum)");
  
  h2dmatchedhits   = new TH2F("h2dmatchedhits","MATCHED hit map",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
  h2dmatchedhits->SetXTitle("U digital (#mum)");
  h2dmatchedhits->SetYTitle("V digital (#mum)");

  h2DpictureMatched = new TH2F("h2DpictureMatched","2D picture of collected charges by MATCHED hits",NofPixelInRaw,0,NofPixelInRaw,NofPixelInColumn,0,NofPixelInColumn); // JB 2014/01/10
  h2DpictureMatched->SetXTitle("U pixel index");
  h2DpictureMatched->SetYTitle("V pixel index");
  h2DpictureMatched->SetZTitle("Average charge");
  
  // Using the digital position
  hhuS    = new TH1F("huS","Hit U digital position ",H_Nbinhu,humin,humax);
  hhuS->SetXTitle("U position (#mum)");
  hhvS    = new TH1F("hvS","Hit V digital position",H_Nbinhv,hvmin,hvmax);
  hhvS->SetXTitle("V position (#mum)");
  
  // Using the Eta position
  hEta2x2UL  = new TH1F("hEta2x2UL","Eta 2x2 large",H_Nbinhu,humin,humax);
  hEta2x2UL->SetXTitle("(#mum)");
  hEta2x2VL  = new TH1F("hEta2x2VL","Eta 2x2 large",H_Nbinhv,hvmin,hvmax);
  hEta2x2VL->SetXTitle("(#mum)");
  
  
  // Digital - CoG - Eta positions comparison
  
  hdCGEtaU   = new TH1F("hdCGEtaU","MATCHED hits CoG-eta3x3 U",int(PixelSize*4),-PixelSize*2,PixelSize*2);
  hdCGEtaU->SetXTitle("(#mum)");
  hdCGEtaV   = new TH1F("hdCGEtaV","MATCHED hits CoG-eta3x3 V",int(PixelSize*4),-PixelSize*2,PixelSize*2);
  hdCGEtaV->SetXTitle("(#mum)");
  hdCGDigU = new TH1F("hdCGDigU","MATCHED hits CoG-digital U",int(PixelSize*4),-PixelSize*2,PixelSize*2);
  hdCGDigU->SetXTitle("(#mum)");
  hdCGDigV = new TH1F("hdCGDigV","MATCHED hits CoG-digital V",int(PixelSize*4),-PixelSize*2,PixelSize*2);
  hdCGDigV->SetXTitle("(#mum)");
  hdCGDigUV = new TH2F("hdCGDigUV","MATCHED hits CoG-digital V vs U",int(PixelSize*4),-PixelSize*2,PixelSize*2,int(PixelSize*4),-PixelSize*2,PixelSize*2);
  hdCGDigUV->SetXTitle("(#mum)");
  hdCGDigUV->SetYTitle("(#mum)");
  hNorm = new TH2F("hNorm","MATCHED hits Eta3x3-digital ",int(PixelSize*4),-PixelSize*2,PixelSize*2,int(PixelSize*4),-PixelSize*2,PixelSize*2);
  hNorm->SetXTitle("(#mum)");
  hNorm->SetYTitle("(#mum)");
  hEta3DigU  = new TH1F("hEta3DigU","MATCHED hits Eta3x3 - digital U",(Int_t)(PixelSizeU*4),-PixelSizeU*2.0,PixelSizeU*2.0);
  hEta3DigU->SetXTitle("(#mum)");
  hEta3DigV  = new TH1F("hEta3DigV","MATCHED hits Eta3x3 - digital V",(Int_t)(PixelSizeU*4),-PixelSizeU*2.0,PixelSizeU*2.0);
  hEta3DigV->SetXTitle("(#mum)");
  hDifCorU = new TH1F("hDifCorU","MATCHED hits CoGcorrected - Eta3x3 U",(Int_t)(PixelSizeU*4),-PixelSizeU*2.0,PixelSizeU*2.0);
  hDifCorU->SetXTitle("(#mum)");
  hDifCorV = new TH1F("hDifCorV","MATCHED hits CoGcorrected - Eta3x3 V",(Int_t)(PixelSizeU*4),-PixelSizeU*2.0,PixelSizeU*2.0);
  hDifCorV->SetXTitle("(#mum)");

  //------------------------------------------------------------------------------
  //----- Binary output specific histos.
  //------------------------------------------------------------------------------
  // modified, JB 2010/06/03
  if( HistDebug) cout << "HistoBooking: binary output" << endl;
  
  hBinary_test = new TH1F("hBinary_test","hBinary_test title",100,0.0,100.0);
  hBinary_NumberOf_1_ALL = new TH1F("hBinary_NumberOf_1_ALL","Number of binary pixels in all matrix",100,-0.5,99.5);
  hBinary_NumberOf_1_goodhit = new TH1F("hBinary_NumberOf_1_goodhit","Number of binary pixels in good hits",100,-0.5,99.5);
  hBinary_NumberOf_1_submatrix = new TH1F("hBinary_NumberOf_1_submatrix","Number of binary pixels in submatrix",100,-0.5,99.5);
  hBinary_Nhitperpixel_submatrix = new TH1F("hBinary_Nhtiperpixel_submatrix","Number of hits per binary pixel",5000,0.0,5000.0);
  hBinary_NhitRateperpixel_submatrix = new TH1F("hBinary_NhitRateperpixel_submatrix","Hit rate per binary pixel",1000,0.0,1.0);
  
    
  //****************************************************
  if( ifReferenceTrack ) { // If RefTrack option
    //****************************************************

    //------------------------------------------------------------------------------
    //----- Hit - track positions correlation for GOOD hits
    //------------------------------------------------------------------------------
    
    duvall  = new TH1F("duvall","2D distance from hit to nearest track, GOOD hits",250,0.0,1000.0);
    duvall->SetXTitle("(#mum)");
    
    hAllHvvsAllTv = new TH2F("tvhv3","hv vs tv for GOOD hits", H_Nbintv,tvmin,tvmax,H_Nbinhv,hvmin,hvmax);
    
    hAllHuvsAllTv = new TH2F("tvhu3","tv hu for GOOD hits", H_Nbintv,tvmin,tvmax,H_Nbinhu,humin,humax);
    
    hAllHvvsAllTu = new TH2F("tuhv3","tu hv for GOOD hits", H_Nbintu,tumin,tumax,H_Nbinhv,hvmin,hvmax);
    
    hAllHuvsAllTu   = new TH2F("tuhu3","hu vs tu for GOOD hits", H_Nbintu,tumin,tumax,H_Nbinhu,humin,humax);
    
    
    //------------------------------------------------------------------------------
    //----- Hit - track positions correlation for MATCHED hits
    //------------------------------------------------------------------------------
    if( HistDebug) cout << "HistoBooking: correlation track-hit position" << endl;
    
    vec    = new TH2F("dudv","tv-hv(dig) vs tu-hu(dig), MATCHED hits",200,10*dumin,10*dumax,200,10*dvmin,10*dvmax);
    vec->SetXTitle("TU-HUdig (#mum)");
    vec->SetYTitle("TV-HVdig (#mum)");
    tudv   = new TH2F("tudv","tv-hv(dig) vs tu",H_Nbintu/2,tumin,tumax,200,dvmin,dvmax);
    tvdu   = new TH2F("tvdu","tu-hu(dig) vs tv",H_Nbintv/2,tvmin,tvmax,200,dumin,dumax);
    hudv   = new TH2F("hudv","tv-hv(dig) vs hu(dig)",H_Nbinhu/2,humin,humax,200,dvmin,dvmax);
    hvdu   = new TH2F("hvdu","tu-hu(dig) vs hv(dig)",H_Nbinhv/2,hvmin,hvmax,200,dumin,dumax);
    tuhu1  = new TH1F("tuhu1","tu-hu(dig)",200,dumin,dumax);
    tvhv1  = new TH1F("tvhv1","tv-hv(dig)",200,dvmin,dvmax);
    tuhu   = new TH2F("tuhu","hUdigital vs tu",H_Nbintu/2,tumin,tumax,H_Nbinhu,humin,humax);
    tvhv   = new TH2F("tvhv","hVdigital vs tv",H_Nbintu/2,tvmin,tvmax,H_Nbinhv,hvmin,hvmax);
    tuhv   = new TH2F("tuhv","tu vs hv(dig)",H_Nbinhu/2,tumin,tumax,H_Nbinhv/2,hvmin,hvmax);
    tvhu   = new TH2F("tvhu","tv vs hu(dig)",H_Nbintv/2,tumin,tumax,H_Nbinhv/2,hvmin,hvmax);
    
    
    huCG5tu   = new TH2F("huCG5tu","huCG5 tu",H_Nbintu/2,tumin,tumax,H_Nbinhu/2,humin,humax);
    hvCG5tv   = new TH2F("hvCG5tv","hvCG5 tv",H_Nbintv/2,tvmin,tvmax,H_Nbinhv/2,hvmin,hvmax);
    huCGtu   = new TH2F("huCGtu","huCG tu",H_Nbintu/2,tumin,tumax,H_Nbinhu/2,humin,humax);
    hvCGtv   = new TH2F("hvCGtv","hvCG tv",H_Nbintv/2,tvmin,tvmax,H_Nbinhv/2,hvmin,hvmax);
    
    huCGtuInPix = new TH2F("huCGtuInPix","tu-hu(dig) vs hu(cog5x5)-hu(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hvCGtvInPix = new TH2F("hvCGtvInPix","tv-hv(dig) vs hv(cog5x5)-hv(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    huCGtuInPix5 = new TH2F("huCGtuInPix5","tu-hu(dig) vs hu(cog5x5)-hu(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hvCGtvInPix5 = new TH2F("hvCGtvInPix5","tv-hv(dig) vs hv(cog5x5)-hv(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hUeta3TuInPix = new TH2F("hUeta3TuInPix","tu-hu(dig) vs hu(eta3)-hu(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hVeta3TvInPix = new TH2F("hVeta3TvInPix","tv-hv(dig) vs hv(eta3)-hv(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hUcorTuInPix = new TH2F("hUcorTuInPix","tu-hu(dig) vs hu(cgCor)-hu(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hVcorTvInPix = new TH2F("hVcorTvInPix","tv-hv(dig) vs hv(cgCor)-hv(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    huCGtuInPix4 = new TH2F("huCGtuInPix4","tu-hu(dig) vs hu(cg4x4)-hu(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    huCG2x2tuInPix = new TH2F("huCG2x2tuInPix","tu-hu(dig) vs hu(cg2x2)-hu(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hvCG2x2tvInPix = new TH2F("hvCG2x2tvInPix","tv-hv(dig) vs hv(cg2x2)-hv(dig)",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    
    hEta2x2tu1L = new TH1F("hEta2x2tu1L","UEta2x2-tuL",ResNbins,-ResRange,ResRange);
    hEta2x2tv1L = new TH1F("hEta2x2tv1L","VEta2x2-tvL",ResNbins,-ResRange,ResRange);
    hEta2x2tu2L = new TH2F("hUeta2x2TuInPixL","hu tuL",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hEta2x2tv2L = new TH2F("hVeta2x2TvInPixL","hv tvL",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hCG2x2tu1L = new TH1F("hCG2x2tu1L","UCG2x2L-tu",ResNbins,-ResRange,ResRange);
    hCG2x2tv1L = new TH1F("hCG2x2tv1L","VCG2x2L-tv",ResNbins,-ResRange,ResRange);
    
    
    hEta2x2tu2 = new TH2F("hUeta2x2TuInPix","hu tu",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hEta2x2tv2 = new TH2F("hVeta2x2TvInPix","hv tv",ResNbins/2,-ResRange,ResRange,ResNbins/2,-ResRange,ResRange);
    hTHCorr2 = new TH2F("hTHCorr2","Track-Hit",40,-20,20,40,-20,20);
    hAllHuvsAllTu2   = new TH1F("tuhu2","tu-huDig corrected",ResNbins,-ResRange,ResRange);
    hAllHvvsAllTv2   = new TH1F("tvhv2","tv-hvDig corrected",ResNbins,-ResRange,ResRange);
    
    ProfUCG = new TProfile("ProfUCG","Profile UCG-Udig vs Utrack-Udig",ResNbins/2,-ResRange,ResRange,-ResRange,ResRange);
    ProfVCG = new TProfile("ProfVCG","Profile VCG-Vdig vs Vtrack-Vdig",ResNbins/2,-ResRange,ResRange,-ResRange,ResRange);
    ProfACGn = new TProfile("ProfACGn","Profile UCG-Udig vs Vtrack-Vdig and reverse",ResNbins/2,-ResRange,ResRange,-ResRange,ResRange);
    
    
    // track to hit distance
    
    htmp5 = new TH1F("htmp5","adist",50,0,100);
    
    // to check alignement
    
    Int_t nBinsDistance = (Int_t)TrackToHitDistanceLimit*2;
    hAlignHuTu     = new TH1F("hAlignHuTu","tu-hu",nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit); //JB 2010/03/12
    hAlignHuTu->SetXTitle("U residual (#mum)");
    hAlignHvTv     = new TH1F("hAlignHvTv","tv-hv",nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit);
    hAlignHuTu->SetXTitle("V residual (#mum)");
    hAlignHuTuvsTv = new TH2F("hAlignHuTuvsTv","tu-hu vs Tv",H_Nbintv,tvmin,tvmax,nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit); //JB 2010/03/12
    hAlignHuTuvsTv->SetXTitle("U residual (#mum)");
    hAlignHuTuvsTv->SetYTitle("track V position (#mum)");
    hAlignHvTvvsTu = new TH2F("hAlignHvTvvsTu","tv-hv vs Tu",H_Nbintu,tumin,tumax,nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit); //JB 2010/03/12
    hAlignHvTvvsTu->SetXTitle("V residual (#mum)");
    hAlignHvTvvsTu->SetYTitle("track U position (#mum)");
    hAlignHuTuvsTu = new TH2F("hAlignHuTuvsTu","tu-hu vs Tu",H_Nbintu,tumin,tumax,nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit); //JB 2013/07/16
    hAlignHuTuvsTu->SetXTitle("U residual (#mum)");
    hAlignHuTuvsTu->SetYTitle("track U position (#mum)");
    hAlignHvTvvsTv = new TH2F("hAlignHvTvvsTv","tv-hv vs Tv",H_Nbintv,tvmin,tvmax,nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit); //JB 2013/07/16
    hAlignHvTvvsTv->SetXTitle("V residual (#mum)");
    hAlignHvTvvsTv->SetYTitle("track V position (#mum)");
    hAlignHuTuvsHv = new TH2F("hAlignHuTuvsHv","tu-hu vs Hv",H_Nbinhv,hvmin,hvmax,nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit); //JB 2012/11/22
    hAlignHvTvvsHu = new TH2F("hAlignHvTvvsHu","tv-hv vs Hu",H_Nbinhu,humin,humax,nBinsDistance,-TrackToHitDistanceLimit,TrackToHitDistanceLimit); //JB 2012/11/22
    
    hEta2x2vsInd   = new TH2F("hEta2x2vsInd","",50,-30,30,5,0,5);
    hChargeVsPosition = new TH2F("hChargeVsPosition","Relative charge vs dig hit position wrt diode",int(PixelSize*2),-PixelSize,PixelSize,int(PixelSize*2),-PixelSize,PixelSize);
    hChargeVsPosition->SetZTitle("q(seed)/q(3x3)");
    hChargeVsPosition->SetXTitle("trackU - seedU(dig) (#mum)");
    hChargeVsPosition->SetYTitle("trackV - seedV(dig) (#mum)");
    hChargeVsDistance = new TH1F("hChargeVsDistance","Charge vs dig hit distance to diode",int(PixelSize*2),-PixelSize,PixelSize); // clm 2013/07/16
    hChargeVsPosition->SetYTitle("q(seed)");
    hChargeVsPosition->SetXTitle("track-seed distance (#mum)");
    hAllHitsInPixel = new TH2F("hAllHitsInPixel","Hdig - T position MATCHED hits",200,10*dumin,10*dumax,200,10*dvmin,10*dvmax);
    
    
    hGOODqcel  = new TH1F("qGOODcel","GOOD Total cluster charge(3x3) MATCHED hits",125,0.0,2500.0);
    
    
    hClusterChargeProfile = new TH1F("Cluster charge","Charge profile",120,0.,60.);
    hClusterChargeNorm = new TH1F("Cluster "," ",120,0.,60.);
    
    DuvCG  = new TH1F("DuvCG","Track-Hit Distance, MATCHED hits",500,0.0,1000.0);
    DuvCG->SetXTitle("(#mum)");
    DuvCG->SetFillColor(29);
    
    hCDSvar = new TH1F("hCDSvar","",100,0,10);
    CDSVarvsTime = new TH2F("CDSvsTime","",50,0,25,50,0.,10.);
    dtime = new TH1F("timelast","Time since last trigger",100,0,50);
    
    for (Int_t i=0; i<64; i++){
      sprintf(nom1,"group%d",i+1); 
      sprintf(titre1,"Charge in seed of group %d ",i+1); 
      hChargeInSeedInGroup[i]=new TH1F (nom1,titre1,50,60.,110.);    
    }
    
    
    //------------------------------------------------------------------------------
    //----- Spatial Resolution
    //------------------------------------------------------------------------------
    if( HistDebug) cout << "HistoBooking: spatial resolution" << endl;
    
    hAllHuvsAllTu1   = new TH1F("hAhuAtu1","Tu-huDig",ResNbins,-ResRange,ResRange);
    hAllHvvsAllTv1   = new TH1F("hAhvAtv1","Tv-hvDig",ResNbins,-ResRange,ResRange);
    hAllHuvsAllTu1->SetXTitle("U residue (#mum)");
    hAllHvvsAllTv1->SetXTitle("V residue (#mum)");
    huCGtu1   = new TH1F("huCGtu1","Tu-huCG(DSF)",ResNbins,-ResRange,ResRange);
    hvCGtv1   = new TH1F("hvCGtu1","Tv-hvCG(DSF)",ResNbins,-ResRange,ResRange);
    huCGtu1->SetXTitle("U residue (#mum)");
    hvCGtv1->SetXTitle("V residue (#mum)");

    // AP, 2014/10/23
    //Defining residue for different multiplicities
    //The multiplicities considered are from 1 up to NMultPoints
    //const int NMultPoints(9);
    
    int Nbins_DistTrk = 50;
    double R_DistTrk[2];
    R_DistTrk[0] = 0.0;
    R_DistTrk[1] = 0.7*sqrt(pow(PixelSizeU,2) + pow(PixelSizeV,2));
    
    const int NMultPoints(6);
    int Ndivisions = 1*100 + 1*NMultPoints;
    huCGtu1_vs_Mult  = new TH1F*[NMultPoints];
    hvCGtv1_vs_Mult  = new TH1F*[NMultPoints];
    huvCGtuv_vs_Mult = new TH2F*[NMultPoints];
    hDist_Trck_Diode_Asso_vs_Mult = new TH1F*[NMultPoints];
    for(int imult=0;imult<NMultPoints;imult++) {
      TString HistName,HistTitle;

      HistName  = TString("huCGtu1_vs_Mult") + long(imult+1);
      if(imult+1 < NMultPoints) HistTitle = TString("Tu-huCG(DSF) for Mult. = ")    + long(imult+1);
      else                      HistTitle = TString("Tu-huCG(DSF) for Mult. #geq ") + long(imult+1);
      huCGtu1_vs_Mult[imult] = new TH1F(HistName.Data(),HistTitle.Data(),
					ResNbins,-ResRange,ResRange);
      huCGtu1_vs_Mult[imult]->SetXTitle("U residue (#mum)");

      HistName  = TString("hvCGtv1_vs_Mult") + long(imult+1);
      if(imult+1 < NMultPoints) HistTitle = TString("Tv-hvCG(DSF) for Mult. = ")    + long(imult+1);
      else                      HistTitle = TString("Tv-hvCG(DSF) for Mult. #geq ") + long(imult+1);
      hvCGtv1_vs_Mult[imult] = new TH1F(HistName.Data(),HistTitle.Data(),
					ResNbins,-ResRange,ResRange);
      hvCGtv1_vs_Mult[imult]->SetXTitle("V residue (#mum)");

      HistName  = TString("huvCGtuv_vs_Mult") + long(imult+1);
      if(imult+1 < NMultPoints) HistTitle = TString("Tu vs Tv on matrix for Mult. = ")    + long(imult+1);
      else                      HistTitle = TString("Tu vs Tv on matrix for Mult. #geq ") + long(imult+1);
      huvCGtuv_vs_Mult[imult] = new TH2F(HistName.Data(),
					 HistTitle.Data(),
					 50,0.0,2.0*PixelSizeU,
					 50,0.0,2.0*PixelSizeV);
      huvCGtuv_vs_Mult[imult]->SetXTitle("Tu position (#mum)");
      huvCGtuv_vs_Mult[imult]->GetXaxis()->CenterTitle(true);
      huvCGtuv_vs_Mult[imult]->SetYTitle("Tv position (#mum)");
      huvCGtuv_vs_Mult[imult]->GetYaxis()->CenterTitle(true);
      huvCGtuv_vs_Mult[imult]->SetStats(false);

      HistName  = TString("hDist_Trck_Diode_Asso_vs_Mult") + long(imult+1);
      if(imult+1 < NMultPoints) HistTitle = TString("Distance between associated track and closest diode for Mult. = ")    + long(imult+1);
      else                      HistTitle = TString("Distance between associated track and closest diode for Mult. #geq ") + long(imult+1);
      hDist_Trck_Diode_Asso_vs_Mult[imult] = new TH1F(HistName.Data(),
					              HistTitle.Data(),
				                      Nbins_DistTrk,R_DistTrk[0],R_DistTrk[1]);
      hDist_Trck_Diode_Asso_vs_Mult[imult]->SetXTitle("Distance(Trk - closest diode) (#mum)");
      hDist_Trck_Diode_Asso_vs_Mult[imult]->SetLineColor(1);
      hDist_Trck_Diode_Asso_vs_Mult[imult]->SetLineWidth(1);
      hDist_Trck_Diode_Asso_vs_Mult[imult]->SetMarkerColor(1);
    }
    
    hnpixc_vs_TrkDistToDiode = new TH2F("hnpixc_vs_TrkDistToDiode",
					"Probability of duplicated hits vs cluster multiplicity vs Track-distance to diode of earliest MATCHED hit",
					MaxNofPixelsInCluster+2,-0.5,MaxNofPixelsInCluster+1.5,
					Nbins_DistTrk,R_DistTrk[0],R_DistTrk[1]);
    hnpixc_vs_TrkDistToDiode->SetXTitle("# pixels/cluster");
    hnpixc_vs_TrkDistToDiode->SetYTitle("Distance(trk - closest diode) (#mum)");

    huvCGtuv = new TH2F("huvCGtuv",
			"Tu vs Tv on matrix for all associated tracks",
			50,0.0,2.0*PixelSizeU,
			50,0.0,2.0*PixelSizeV);
    huvCGtuv->SetXTitle("Tu position (#mum)");
    huvCGtuv->GetXaxis()->CenterTitle(true);
    huvCGtuv->SetYTitle("Tv position (#mum)");
    huvCGtuv->GetYaxis()->CenterTitle(true);
    huvCGtuv->SetStats(false);

    huvCGtuv_NoAsso = new TH2F("huvCGtuv_NoAsso",
			       "Tu vs Tv on matrix for non-associated tracks",
			       50,0.0,2.0*PixelSizeU,
			       50,0.0,2.0*PixelSizeV);
    huvCGtuv_NoAsso->SetXTitle("Tu position (#mum)");
    huvCGtuv_NoAsso->GetXaxis()->CenterTitle(true);
    huvCGtuv_NoAsso->SetYTitle("Tv position (#mum)");
    huvCGtuv_NoAsso->GetYaxis()->CenterTitle(true);
    huvCGtuv_NoAsso->SetStats(false);

    hDist_Trck_Diode_Asso = new TH1F("hDist_Trck_Diode_Asso",
				     "Distance between associated track and closest diode",
				     Nbins_DistTrk,R_DistTrk[0],R_DistTrk[1]);
    hDist_Trck_Diode_Asso->SetXTitle("Distance(Trk - closest diode) (#mum)");
    hDist_Trck_Diode_Asso->SetLineColor(1);
    hDist_Trck_Diode_Asso->SetLineWidth(1);
    hDist_Trck_Diode_Asso->SetMarkerColor(1);

    hDist_Trck_Diode_NoAsso = new TH1F("hDist_Trck_Diode_NoAsso",
				       "Distance between NOT associated track and closest diode",
				       Nbins_DistTrk,R_DistTrk[0],R_DistTrk[1]);
    hDist_Trck_Diode_NoAsso->SetXTitle("Distance(Trk - closest diode) (#mum)");
    hDist_Trck_Diode_NoAsso->SetLineColor(2);
    hDist_Trck_Diode_NoAsso->SetLineWidth(1);
    hDist_Trck_Diode_NoAsso->SetMarkerColor(2);

    hEffic_vs_Dist_Trck_Diode = new TH1F("hEffic_vs_Dist_Trck_Diode",
					 "#epsilon_{det} vs Distance between track and closest diode",
					 Nbins_DistTrk,R_DistTrk[0],R_DistTrk[1]);
    hEffic_vs_Dist_Trck_Diode->SetXTitle("Distance(Trk - closest diode) (#mum)");
    hEffic_vs_Dist_Trck_Diode->SetLineColor(4);
    hEffic_vs_Dist_Trck_Diode->SetLineWidth(1);
    hEffic_vs_Dist_Trck_Diode->SetMarkerColor(4);
    

    huCGwidth_vs_Mult = new TH1F("huCGwidth_vs_Mult","CG(DSF) residue width vs cluster multicity",
				 NMultPoints,
				 1.0         - 0.5,
				 NMultPoints + 0.5);
    huCGwidth_vs_Mult->SetXTitle("Cluster multiplicity");
    huCGwidth_vs_Mult->GetXaxis()->CenterTitle(true);
    huCGwidth_vs_Mult->SetYTitle("#sigma_{res} (#mum)");
    huCGwidth_vs_Mult->GetYaxis()->CenterTitle(true);
    huCGwidth_vs_Mult->SetStats(false);
    huCGwidth_vs_Mult->GetXaxis()->SetNdivisions(Ndivisions);

    hvCGwidth_vs_Mult = new TH1F("hvCGwidth_vs_Mult","CG(DSF) residue width vs cluster multicity",
				 NMultPoints,
				 1.0         - 0.5,
				 NMultPoints + 0.5);
    hvCGwidth_vs_Mult->SetXTitle("Cluster multiplicity");
    hvCGwidth_vs_Mult->GetXaxis()->CenterTitle(true);
    hvCGwidth_vs_Mult->SetYTitle("#sigma_{res} (#mum)");
    hvCGwidth_vs_Mult->GetYaxis()->CenterTitle(true);
    hvCGwidth_vs_Mult->SetStats(false);
    hvCGwidth_vs_Mult->GetXaxis()->SetNdivisions(Ndivisions);

    huCGmean_vs_Mult = new TH1F("huCGmean_vs_Mult","CG(DSF) residue mean vs cluster multicity",
				NMultPoints,
				1.0         - 0.5,
				NMultPoints + 0.5);
    huCGmean_vs_Mult->SetXTitle("Cluster multiplicity");
    huCGmean_vs_Mult->GetXaxis()->CenterTitle(true);
    huCGmean_vs_Mult->SetYTitle("Mean (#mum)");
    huCGmean_vs_Mult->GetYaxis()->CenterTitle(true);
    huCGmean_vs_Mult->SetStats(false);
    huCGmean_vs_Mult->GetXaxis()->SetNdivisions(Ndivisions);

    hvCGmean_vs_Mult = new TH1F("hvCGmean_vs_Mult","CG(DSF) residue mean vs cluster multicity",
				NMultPoints,
				1.0         - 0.5,
				NMultPoints + 0.5);
    hvCGmean_vs_Mult->SetXTitle("Cluster multiplicity");
    hvCGmean_vs_Mult->GetXaxis()->CenterTitle(true);
    hvCGmean_vs_Mult->SetYTitle("Mean (#mum)");
    huCGmean_vs_Mult->GetYaxis()->CenterTitle(true);
    hvCGmean_vs_Mult->SetStats(false);
    hvCGmean_vs_Mult->GetXaxis()->SetNdivisions(Ndivisions);

    for(int imult=0;imult<NMultPoints;imult++) {
      TString LastBinLabel;
      if(imult+1 < NMultPoints) LastBinLabel = TString("") + long(imult+1);
      else                      LastBinLabel = TString("#geq ") + long(NMultPoints);

      huCGwidth_vs_Mult->GetXaxis()->SetBinLabel(imult+1,LastBinLabel.Data());
      hvCGwidth_vs_Mult->GetXaxis()->SetBinLabel(imult+1,LastBinLabel.Data());
      huCGmean_vs_Mult->GetXaxis()->SetBinLabel(imult+1,LastBinLabel.Data());
      hvCGmean_vs_Mult->GetXaxis()->SetBinLabel(imult+1,LastBinLabel.Data());
    }

    huCGtu1_vs_TracksPerEvent  = new TH1F*[hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins()];
    hvCGtv1_vs_TracksPerEvent  = new TH1F*[hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins()];
    for(int i=0;i<hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();i++) {
      TString MyHistName,MyHistTitle;

      MyHistName  = TString("huCGtu1_vs_TracksPerEvent") + long(i+1);
      MyHistTitle = TString("Tu-huCG(DSF) for ") + long(hnTracksInGeomatrixVsTrackPerEvent->GetBinCenter(i+1)) + TString(" Tracks/event");
      huCGtu1_vs_TracksPerEvent[i] = new TH1F(MyHistName.Data(),MyHistTitle.Data(),
					      ResNbins,-ResRange,ResRange);
      huCGtu1_vs_TracksPerEvent[i]->SetXTitle("U residue (#mum)");

      MyHistName  = TString("hvCGtv1_vs_TracksPerEvent") + long(i+1);
      MyHistTitle = TString("Tv-hvCG(DSF) for ") + long(hnTracksInGeomatrixVsTrackPerEvent->GetBinCenter(i+1)) + TString(" Tracks/event");
      hvCGtv1_vs_TracksPerEvent[i] = new TH1F(MyHistName.Data(),MyHistTitle.Data(),
					      ResNbins,-ResRange,ResRange);
      hvCGtv1_vs_TracksPerEvent[i]->SetXTitle("V residue (#mum)");
    }

    huCGtu2   = new TH1F("huCGtu2","Tu-huCG",ResNbins,-ResRange,ResRange);
    hvCGtv2   = new TH1F("hvCGtu2","Tv-hvCG",ResNbins,-ResRange,ResRange);
    huCGtu2->SetXTitle("U residue (#mum)");
    hvCGtv2->SetXTitle("V residue (#mum)");
    hCG2x2tu1 = new TH1F("hCG2x2tu1","Tu-UCG2x2",ResNbins,-ResRange,ResRange);
    hCG2x2tv1 = new TH1F("hCG2x2tv1","Tv-VCG2x2",ResNbins,-ResRange,ResRange);
    hCG2x2tu1->SetXTitle("U residue (#mum)");
    hCG2x2tv1->SetXTitle("V residue (#mum)");
    hCG5URes   = new TH1F("hCG5URes","Tu-huCG5",ResNbins,-ResRange,ResRange);
    hCG5VRes   = new TH1F("hCG5VRes","Tv-hvCG5",ResNbins,-ResRange,ResRange);
    hCG5URes->SetXTitle("U residue (#mum)");
    hCG5VRes->SetXTitle("V residue (#mum)");
    hTuHuCorr = new TH1F("hTuHuCorr","Tu-Ucorr",ResNbins,-ResRange,ResRange);
    hTvHvCorr = new TH1F("hTvHvCorr","Tv-Vcorr",ResNbins,-ResRange,ResRange);
    hTuHuCorr->SetXTitle("U residue (#mum)");
    hTvHvCorr->SetXTitle("V residue (#mum)");
    hEta2x2tu1 = new TH1F("hEta2x2tu1","tu-UEta2x2",ResNbins,-ResRange,ResRange);
    hEta2x2tv1 = new TH1F("hEta2x2tv1","tv-VEta2x2",ResNbins,-ResRange,ResRange);
    hEta2x2tu1->SetXTitle("U residue (#mum)");
    hEta2x2tv1->SetXTitle("V residue (#mum)");
    hEtaURes = new TH1F("hEtaURes","Tu-UEta",ResNbins+40,-ResRange-20,ResRange+20);
    hEtaVRes = new TH1F("hEtaVRes","Tv-VEta",ResNbins+40,-ResRange-20,ResRange+20);
    hEtaURes->SetXTitle("U residue (#mum)");
    hEtaVRes->SetXTitle("V residue (#mum)");
    hEta3URes = new TH1F("hEta3URes","Tu-UEta3",ResNbins+40,-ResRange-20,ResRange+20);
    hEta3VRes = new TH1F("hEta3VRes","Tv-VEta3",ResNbins+40,-ResRange-20,ResRange+20);
    hEta3URes->SetXTitle("U residue (#mum)");
    hEta3VRes->SetXTitle("V residue (#mum)");
    hEtaU_2x2Res  = new TH1F("hEtaU_2x2Res","Tu-UEta2x2(list)",ResNbins+40,-ResRange-20,ResRange+20);
    hEtaV_2x2Res  = new TH1F("hEtaV_2x2Res","TV-VEta2x2(list)",ResNbins+40,-ResRange-20,ResRange+20);
    hEtaU_2x2Res->SetXTitle("U residue (#mum)");
    hEtaV_2x2Res->SetXTitle("V residue (#mum)");
    hEtaU_5x5Res  = new TH1F("hEtaU_5x5Res","Tu-UEta5x5(list)",ResNbins+40,-ResRange-20,ResRange+20);
    hEtaV_5x5Res  = new TH1F("hEtaV_5x5Res","Tu-UEta5x5(list)",ResNbins+40,-ResRange-20,ResRange+20);
    hEtaU_5x5Res->SetXTitle("U residue (#mum)");
    hEtaV_5x5Res->SetXTitle("V residue (#mum)");
    hAHTURes = new TH1F("hAHTURes","Tu-uAHT",ResNbins+40,-ResRange-20,ResRange+20);
    hAHTVRes = new TH1F("hAHTVRes","Tv-vAHT",ResNbins+40,-ResRange-20,ResRange+20);
    hAHTURes->SetXTitle("U residue (#mum)");
    hAHTVRes->SetXTitle("V residue (#mum)");
    
    
    //------------------------------------------------------------------------------
    //----- Hit - track positions correlation for UNMATCHED
    //------------------------------------------------------------------------------
    if( HistDebug) cout << "HistoBooking: unmatched track position" << endl;
    
    if(Mtype<5){
      tuv    = new TH2F("tuv","Track and cluster position",64,-640,640,64,-640,640);
      tuv1    = new TH2F("tuv1","Track and cluster position",64,-640,640,64,-640,640);
    }
    else {
      tuv    = new TH2F("tuv","Track and cluster position",64,-4352,4352,64,-4352,4352);
      tuv1    = new TH2F("tuv1","Track and cluster position: red-cluster, green-track ",64,-4352,4352,64,-4352,4352);
    }
    tuv->SetMarkerStyle(20);
    tuv->SetMarkerColor(3);
    tuv->SetXTitle("U (#mum)");
    tuv->SetYTitle("V (#mum)");
    tuv1->SetMarkerStyle(20);
    tuv1->SetMarkerColor(2);
    tuv1->SetXTitle("U (#mum)");
    tuv1->SetYTitle("V (#mum)");
    htu    = new TH1F("tu","UNMATCHED tracks U",H_Nbintu,tumin,tumax);
    htu->SetXTitle("TU (#mum)");
    htv    = new TH1F("tv","UNMATCHED tracks V",H_Nbintv,tvmin,tvmax);
    htv->SetXTitle("TV (#mum)");
    htuhtv  = new TH2F("htuhtv","UNMATCHED track map",int((tumax-tumin)/PixelSizeU),tumin,tumax,int((tvmax-tvmin)/PixelSizeV),tvmin,tvmax);
    htuhtv->SetXTitle("TU (#mum)");
    htuhtv->SetYTitle("TV (#mum)");
    FalseHitMap = new TH2F("FalseHitMap","UNMATCHED tracks UV when no hit",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
    huvBad    = new TH2F("huvBad","UNMATCHED tracks UV when hit too far",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
    hxyBad    = new TH2F("hxyBad","UNMATCHED tracks XY when hit too far",H_Nbinhu,humin,humax,H_Nbinhv,hvmin,hvmax);
    
    htuvInPix = new TH2F("htuvInPix","UNMATCHED tracks UV position inside pixel",20,-ResRange,ResRange,20,-ResRange,ResRange);
    
    
    
    //------------------------------------------------------------------------------
    //----- tracks properties
    //------------------------------------------------------------------------------
    if( HistDebug) cout << "HistoBooking: starting track property" << endl;
    
    //-- ALL tracks
    
    hchi2  = new TH1F("chi2","Track chi2 for ALL tracks",400,0.,200.0);
    hchi2->SetXTitle("#chi2/ndf");
    hAllTvTu  = new TH2F("AllTvTu","Map of ALL tracks",100,4*tumin,4*tumax,100,4*tvmin,4*tvmax);
    hAllTvTu->SetXTitle("Tu (#mum)");
    hAllTvTu->SetYTitle("Tv (#mum)");
    hAllTu    = new TH1F("AllTu","u of ALL tracks",100,4*tumin,4*tumax);
    hAllTu->SetXTitle("Tu (#mum)");
    hAllTv    = new TH1F("AllTv","v of ALL tracks",100,4*tvmin,4*tvmax);
    hAllTv->SetYTitle("Tv (#mum)");
    hNhitsPerTrack_all = new TH1F("hitspertrackall","# hits per ALL track",11,0,11);
    hNhitsPerTrack_all->SetXTitle("# hits");
    
    //-- GOOD tracks (mostly chi2 cut)
    
    hNTracksPerEvent = new TH1F("hNTracksPerEvent","# GOOD tracks per event",100,0,100);
    hNTracksPerEventievt = new TH1F("hNTracksPerEventievt","# GOOD tracks in telescope per event vs event number",1000,0,500000);
    hGoodChi2TvTu  = new TH2F("GoodChi2TvTu","Map of GOOD tracks", 200, -15000., 15000., 200, 15000., 15000.); //100,4*tumin,4*tumax,100,4*tvmin,4*tvmax);
    hGoodChi2TvTu->SetXTitle("Tu (#mum)");
    hGoodChi2TvTu->SetYTitle("Tv (#mum)");
    hGoodChi2Tu    = new TH1F("GoodTu","u of GOOD tracks",30,tumin,tumax);
    hGoodChi2Tv    = new TH1F("GoodTv","v of GOOD tracks",30,tvmin,tvmax);
    hGoodChi2Tx    = new TH1F("GoodTx","x of GOOD tracks",30,tumin,tumax);
    hGoodChi2Ty    = new TH1F("GoodTy","y of GOOD tracks",30,tvmin,tvmax);
    hGoodChi2AngleXZ = new TH1F("GoodAngleXZ","GOOD track angles in XZ plane",100, -10., 10.); // JB 2011/10/30
    hGoodChi2AngleYZ = new TH1F("GoodAngleYZ","GOOD track angles in YZ plane",100, -10., 10.);
    hNhitsPerTrack_good = new TH1F("hitspertrackall_good","# hits per GOOD track",11,0,11);
    hNhitsPerTrack_good->SetXTitle("# hits");

    
    //-- GOOD tracks in GEOMATRIX
    
    hTrackToClusterMinDistance  = new TH1F("hTrackToClusterMinDistance","Track-hit distance of closest GOOD cluster for GOOD track in GEOMATRIX",500,0.0,1000.0);
    hTrackToClusterMinDistance->SetXTitle("#mum"); // same param as histo DuvCG
    
    hNGoodGeomTracksPerEvent = new TH1F("NGoodGeomTracksPerEvent","# tracks in EXCLUSION GEOMATRIX per event",100,0,100);
    
    hTrackTo2ndclosestClusterDistance  = new TH1F("hTrackTo2ndclosestClusterDistance","Track-hit distance of 2nd closest GOOD cluster for GOOD track in GEOMATRIX",1000,0.0,10000.0);
    hTrackTo2ndclosestClusterDistance->SetXTitle("#mum"); // same param as histo DuvCG
    
    hMinDistance_vs_2ndDistance = new TH2F("hMinDistance_vs_2ndDistance","MinDistance_vs_2ndDistance",1000,0.,10000.0,200,0.0,400.0);
    hMinDistance_vs_2ndDistance->SetXTitle("2ndDistance #mum");
    hMinDistance_vs_2ndDistance->SetYTitle("MinDistance #mum");

    hWrongAssociationProba = new TH1F("hWrongAssociationProba","Wrong Association Probability vs distance",1000,0.0,10000.0);
    hWrongAssociationProba->SetXTitle("#mum"); // same param as histo DuvCG ; 


    //-- MATCHED tracks
    
    hchi2_c  = new TH1F("chi2 c","Track chi2 when hit MATCHED",400,0.0,200.0);
    hchi2_c->SetFillColor(29);
    hchi2_c->SetXTitle("#chi2/ndf");
    
    hdistchi2 = new TH2F("hdistchi2","Track-hit distance vs track Chi2, MATCHED",400,0.,200.0,100,0.0,200.0);
    hdistchi2->SetXTitle("#chi2/ndf");
    hdistchi2->SetYTitle("(#mum)");
    hNhitsPerTrack_matched = new TH1F("hitspertrackall_matched","# hits per MATCHED track",11,0,11);
    hNhitsPerTrack_matched->SetXTitle("# hits");

    Char_t titreref[100] ; 
    Char_t nomRef[50]; 
    for (Int_t i=0 ; i<NRefPlane; i++){
      sprintf(nomRef,"Tud_vs_Tv_%d",i+1); 
      sprintf(titreref,"Hu - Tu vs Tv, pl%d ",i+1); 
      hRef_Tud_vs_Tv[i] = new TH2F (nomRef,titreref,100,-5000.,5000.,100,-100.,100.);
      sprintf(nomRef,"Tud_vs_Tu_%d",i+1); 
      sprintf(titreref,"Hu - Tu vs Tu, pl%d ",i+1); 
      hRef_Tud_vs_Tu[i] = new TH2F (nomRef,titreref,100,-5000.,5000.,100,-100.,100.);
      sprintf(nomRef,"Tud_vs_TOX_%d",i+1); 
      sprintf(titreref,"Hu - Tu  vs TOX, pl%d ",i+1); 
      hRef_Tud_vs_TDOX[i] = new TH2F (nomRef,titreref,100,-100.,100.,100,-100.,100.);
      sprintf(nomRef,"Tud_vs_TOY_%d",i+1); 
      sprintf(titreref,"Hu - Tu  vs TOY, pl%d ",i+1); 
      hRef_Tud_vs_TDOY[i] = new TH2F (nomRef,titreref,100,-100.,100.,100,-100.,100.);
      sprintf(nomRef,"Tud_vs_Chi2_%d",i+1); 
      sprintf(titreref,"Hu - Tu  vs Chi2, pl%d ",i+1); 
      hRef_Tud_vs_Chi2[i] = new TH2F (nomRef,titreref,100,0.,4000.,100,-100.,100.);
      sprintf(nomRef,"Tud_vs_Tu1_%d",i+1); 
      sprintf(titreref,"Hu - Tu  vs Tu1, pl%d ",i+1); 
      hRef_Tud_vs_Tu1[i] = new TH2F (nomRef,titreref,100,-10000.,10000.,100,-100.,100.);
      sprintf(nomRef,"Tud_vs_Tk1_%d",i+1); 
      sprintf(titreref,"Hu - Tu  vs Tk1, pl%d ",i+1); 
      hRef_Tud_vs_Tk1[i] = new TH2F (nomRef,titreref,100,-1000.,1000.,100,-100.,100.);
    }
    
    //-- UNMATCHED tracks
    
    hchi2_nc = new TH1F("chi2 nc","Track chi2 without hit (UNMATCHED)",400,0.0,200.0);
    hchi2_nc->SetXTitle("#chi2/ndf");
    
    if( HistDebug) cout << " track property done." << endl;
    
    
    //------------------------------------------------------------------------------
    //----- efficiency
    // MG 2010/06/04
    //------------------------------------------------------------------------------
    if( HistDebug) cout << "HistoBooking: efficiency" << endl;
    
    effimap = new TH2F("effimap","Efficiency map.",H_Nbinhu/16,humin,humax,H_Nbinhv/16,hvmin,hvmax);
    effimap->SetTitle("Efficency Pixel MAP");
    effimap->SetXTitle("U position (#mum)");
    effimap->SetYTitle("V position (#mum)");
    effimap->Sumw2();
    goodtracks = new TH2F("goodtracks","Good tracks for efficiency.",H_Nbinhu/16,humin,humax,H_Nbinhv/16,hvmin,hvmax); // JB/2010/06/04
    TrkInMimo   = new TH2F("trkinmimo","GOOD tracks in GEOMATRIX for effciency",H_Nbinhu/16,humin,humax,H_Nbinhv/16,hvmin,hvmax); //MG 2010/06/11
  
    //Efficiency vs Track-hit distance,   AP 13/01/2015
    int Nbins_TrkHitDist = 76;
    double R_TrkHitDist[2];
    R_TrkHitDist[0] =  20.0;
    R_TrkHitDist[1] = 400.0;
    effi_vs_TrkHitDist = new TH1F("effi_vs_TrkHitDist","Efficiency vs Track-hit distance",
				  Nbins_TrkHitDist,R_TrkHitDist[0],R_TrkHitDist[1]);
    effi_vs_TrkHitDist->SetXTitle("Trk-Hit distance (#mum)");
    effi_vs_TrkHitDist->GetXaxis()->CenterTitle(true);
    effi_vs_TrkHitDist->SetYTitle("efficiency (%)");
    effi_vs_TrkHitDist->GetYaxis()->CenterTitle(true);
    effi_vs_TrkHitDist->SetLineColor(4);
    effi_vs_TrkHitDist->SetLineWidth(2);
    effi_vs_TrkHitDist->SetMarkerColor(4);
    effi_vs_TrkHitDist->SetMarkerStyle(20);
    effi_vs_TrkHitDist->SetStats(false);
    effi_vs_TrkHitDist->Sumw2();

    effiCorr_vs_TrkHitDist = new TH1F("effiCorr_vs_TrkHitDist","Efficiency vs Track-hit distance",
				      Nbins_TrkHitDist,R_TrkHitDist[0],R_TrkHitDist[1]);
    effiCorr_vs_TrkHitDist->SetXTitle("Trk-Hit distance (#mum)");
    effiCorr_vs_TrkHitDist->GetXaxis()->CenterTitle(true);
    effiCorr_vs_TrkHitDist->SetYTitle("efficiency (%)");
    effiCorr_vs_TrkHitDist->GetYaxis()->CenterTitle(true);
    effiCorr_vs_TrkHitDist->SetLineColor(2);
    effiCorr_vs_TrkHitDist->SetLineWidth(2);
    effiCorr_vs_TrkHitDist->SetMarkerColor(2);
    effiCorr_vs_TrkHitDist->SetMarkerStyle(20);
    effiCorr_vs_TrkHitDist->SetStats(false);
    effiCorr_vs_TrkHitDist->Sumw2();
    
    //------------------------------------------------------------------------------
    //----- Charge distribution and Pixel homogeneity depending on real track position
    //------------------------------------------------------------------------------
    if( HistDebug) cout << "HistoBooking: charge distribution" << endl;
    
    Int_t sizeReducingFactor = 4; // usefull to limit nb of bins in multi-dim histo (hence memory zise)
    
    // Charge distribution
    
    hChargeIntegral1 = new TH2F("hChInt1","Charge Integral 1",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    hChargeNorm1 = new TH2F("hChNorm1","Norm",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    hChargeIntegral2 = new TH2F("hChInt2","Charge Integral 2",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    hChargeNorm2 = new TH2F("hChNorm2","Norm",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    hChargeIntegral3 = new TH2F("hChInt3","Charge Integral 3",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    hChargeNorm3 = new TH2F("hChNorm3","Norm",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    hChargeIntegral4 = new TH2F("hChInt4","Charge Integral 4",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    hChargeNorm4 = new TH2F("hChNorm4","Norm",nb,-SpreadRange,SpreadRange,nb,-SpreadRange,SpreadRange);
    
    
    // Pixel homogeneity
    
    hHOM_tu_tv_modulo = new TH2F("hHOM_tu_tv_modulo","track position within pixel",(Int_t)PixelSizeU*4/sizeReducingFactor,-PixelSizeU*2.,PixelSizeU*2,(Int_t)PixelSizeV*4/sizeReducingFactor,-PixelSizeV*2.,PixelSizeV*2);
    
    hHOM_ResU_tu  = new TH2F("hHOM_ResU_tu","Residue U vs Residue track position",(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU,(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU);
    hHOM_ResV_tv  = new TH2F("hHOM_ResV_tv","Residue V vs Residue track position",(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV);
    ProfHOM_ResU_tu = new TProfile("ProfHOM_ResU_tu","Profile Residue U  vs Residue track position",20,-10,10,-100,100);
    ProfHOM_ResV_tv = new TProfile("ProfHOM_ResV_tv","Profile Residue V  vs Residue track position",20,-10,10,-100,100);
    
    hHOM_modUCG_modtu = new TH2F("hHOM_modUCG_modtu","mod UCG vs mod tu",(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU,(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU);
    hHOM_modVCG_modtv = new TH2F("hHOM_modVCG_modtv","mod VCG vs mod tv",(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV);
    
    hHOM_modUeta3_modtu  = new TH2F("hHOM_modUeta3_modtu","mod Eta3U vs mod tu",(Int_t)PixelSizeU*2,-PixelSizeU,PixelSizeU,(Int_t)PixelSizeU*2,-PixelSizeU,PixelSizeU);
    hHOM_modVeta3_modtv  = new TH2F("hHOM_modVeta3_modtv","mod Eta3V vs mod tv",(Int_t)PixelSizeV*2,-PixelSizeV,PixelSizeV,(Int_t)PixelSizeV*2,-PixelSizeV,PixelSizeV);
    
    hHOM_modUeta3_realtu  = new TH2F("hHOM_modUeta3_realtu","mod Eta3U vs tu",H_Nbintu/2,tumin,tumax,(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU);
    hHOM_modVeta3_realtv = new TH2F(" hHOM_modVeta3_realtv","mod Eta3V vs tv",H_Nbintv/2,tvmin,tvmax,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV);
    
    hHOM_modUCG_realtu  = new TH2F("hHOM_modUCG_realtu","mod UCG vs tu",H_Nbintu/2,tumin,tumax,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSize,PixelSize);
    hHOM_modVCG_realtv  = new TH2F("hHOM_modVCG_realtv","mod VCG vs tv",H_Nbintv/2,tvmin,tvmax,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV);
    
    hHOM_modUeta3_Eta3U  = new TH2F("hHOM_modUeta3_Eta3U","mod Eta3U vs Eta3U",H_Nbinhu/2,humin,humax,(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU);
    hHOM_modVeta3_Eta3V  = new TH2F("hHOM_modVeta3_Eta3V","mod Eta3V vs Eta3V",H_Nbinhv/2,hvmin,hvmax,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV);
    
    hHOM_modUeta3_modVeta3  = new TH2F("hHOM_modUeta3_modVeta3","mod Eta3U vs mod Eta3V",(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSize,PixelSize,(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU);
    hHOM_modUCG_modVCG  = new TH2F("hHOM_modUCG_modVCG","mod UCG vs mod VCG ",(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV);
    
    hHOM_modUeta3_modUCG  = new TH2F("hHOM_modUeta3_modUCG","mod Eta3U vs mod UCG",(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU,(Int_t)PixelSizeU*2/sizeReducingFactor,-PixelSizeU,PixelSizeU);
    hHOM_modVeta3_modVCG  = new TH2F("hHOM_modVeta3_modVCG","mod Eta3V vs mod VCG",(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV,(Int_t)PixelSizeV*2/sizeReducingFactor,-PixelSizeV,PixelSizeV);
    
    //---- charge vs diode distance:
    hHOM_Charge_diodedist_alg= new TH2F("hHOM_Charge_diodedist_alg","Charge vs diode distance to track",int(PixelSize*8)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),50,0,500); // JB 2010/03/19
    hHOM_Charge_diodedist_alg_u= new TH2F("hHOM_Charge_diodedist_alg_u","Charge vs diode distance to track in U",int(PixelSize*8)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),1000,0,10000); // CLM 2013/01/23
    hHOM_Charge_diodedist_alg_v= new TH2F("hHOM_Charge_diodedist_alg_v","Charge vs diode distance to track in V",int(PixelSize*8)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),1000,0,10000); // CLM 2013/01/23
    hHOM_Charge_diodedist    = new TH2F("hHOM_Charge_diodedist"," Seed Charge vs diode distance",int(PixelSize*8)/sizeReducingFactor,0,PixelSize*2,200,0,2000);
    hHOM_Charge2_diodedist   = new TH2F("hHOM_Charge2_diodedist"," 2 pixels Charge vs diode distance",int(PixelSize*2)/sizeReducingFactor,0,PixelSize*2,200,0,3000);
    hHOM_Charge4_diodedist   = new TH2F("hHOM_Charge4_diodedist"," 4 pixels Charge vs diode distance",int(PixelSize*2)/sizeReducingFactor,0,PixelSize*2,200,0,4000);
    hHOM_Charge9_diodedist   = new TH2F("hHOM_Charge9_diodedist"," 9 pixels Charge vs diode distance",int(PixelSize*2)/sizeReducingFactor,0,PixelSize*2,200,0,5000);
    hHOM_Charge25_diodedist   = new TH2F("hHOM_Charge25_diodedist"," 25 pixels  Charge vs diode distance",int(PixelSize*2)/sizeReducingFactor,0,PixelSize*2,200,0,6000);
    hHOM_Noise_diodedist  = new TH2F("hHOM_Noise_diodedist"," Seed pixel Noise vs diode distance",int(PixelSize*2)/sizeReducingFactor,0,PixelSize*2,NoiseNbins,0,NoiseRange);
    
    
    // start CLM 2013/01/23
    ProfhGOODCharge_Charge_DiodePosition            = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.1);
    ProfhGOODCharge_Charge_DiodePositionSeedQLT300   = new TProfile2D("ProfhGOODCharge_Charge_DiodePositionSeedQLT300","Profile All Pixel Charge vs impact position",int(PixelSize*8)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*8)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.1);
    ProfhGOODCharge_Charge_DiodePositionSeedQGT2000 = new TProfile2D("ProfhGOODCharge_Charge_DiodePositionSeedQGT2000","Profile All Pixel Charge vs impact position",int(PixelSize*8)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*8)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.1);
    
    ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    
    ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_seed = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_seed","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_seed = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_seed","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_seed = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_seed","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_seed = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_seed","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    
    ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_1stcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_1stcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_1stcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_1stcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_1stcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_1stcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_1stcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_1stcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    
    ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_2ndcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_2ndcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_2ndcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_2ndcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_2ndcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_2ndcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_2ndcrown = new TProfile2D("ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_2ndcrown","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    
    hDistVSeedOtherOldCalc = new TH2F("hDistVSeedOtherOldCalc","Seed - other pixel distance in V old calc;distance in V (um);Entries",600,-300,300,20,-10,10);
    hDistVSeedOtherNewCalc = new TH2F("hDistVSeedOtherNewCalc","Seed - other pixel distance in V new calc;distance in V (um);Entries",600,-300,300,20,-10,10);
    
    h2dCharge_Charge_DiodePosition_Track = new TH2F("h2dCharge_Charge_DiodePosition_Track","DiodeTrackPosition Track weight = 1;U;V",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4));
    h2dCharge_Charge_DiodePosition_CluSize = new TH2F("h2dCharge_Charge_DiodePosition_CluSize","DiodeTrackPosition ClusterSizel;U;V",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4));
    
    hNpixInClu = new TH1F("hNpixInClu","Number of pixels in Clu;# pixels;Entries",100,0,100);
    hNpixInClu->Sumw2();
    
    hQpixInClu = new TH1F("hQpixInClu","Pixel-by-pixel charge in Clu;# pixels;Entries",10000,0,10000);
    hQpixInClu->Sumw2();
    
    ProfhGOODCharge_Charge_DiodePositionSimpDist = new TProfile2D("ProfhGOODCharge_Charge_DiodePositionSimpDist","Profile All Pixel Charge vs impact position",int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),int(PixelSize*32)/sizeReducingFactor,-int(PixelSize*4),int(PixelSize*4),-0.01,1.);
    
    sizeReducingFactor = 5;
    hHOM_Charge_diodedist3D      = new TH3F("hHOM_Charge_diodedist3D"," Charge vs diode distance 3D",int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),2000/sizeReducingFactor,0,20000);
    hHOM_Charge2_diodedist3D     = new TH3F("hHOM_Charge2_diodedist3D","2 pixel  Charge vs diode distance 3D",int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),2000/sizeReducingFactor,0,20000);
    hHOM_Charge4_diodedist3D     = new TH3F("hHOM_Charge4_diodedist3D","4 pixel  Charge vs diode distance 3D",int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),2000/sizeReducingFactor,0,20000);
    hHOM_Charge9_diodedist3D     = new TH3F("hHOM_Charge9_diodedist3D","9 pixel Charge vs diode distance 3D",int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),2000/sizeReducingFactor,0,20000);
    hHOM_Charge25_diodedist3D    = new TH3F("hHOM_Charge25_diodedist3D","25 pixel Charge vs diode distance 3D",int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),int(PixelSize*8/sizeReducingFactor),-int(PixelSize*4),int(PixelSize*4),2000/sizeReducingFactor,0,20000);
    // end CLM 2013/01/23
    
    ProfHOM_Charge_diodedist_alg = new TProfile("ProfHOM_Charge_diodedist_alg","Charge vs diode distance to track",50,-int(PixelSize*4),int(PixelSize*4),0,2000); // JB 2010/03/11
    ProfHOM_Charge_diodedist_alg_u = new TProfile("ProfHOM_Charge_diodedist_alg_u","Charge vs diode distance to track in u",int(PixelSize*8),-int(PixelSize*4),int(PixelSize*4),0,20000); // clm 2013/01/23
    ProfHOM_Charge_diodedist_alg_v = new TProfile("ProfHOM_Charge_diodedist_alg_v","Charge vs diode distance to track in v",int(PixelSize*8),-int(PixelSize*4),int(PixelSize*4),0,20000); // clm 2013/01/23
    ProfHOM_Charge_diodedist = new TProfile("ProfHOM_Charge_diodedist"," Seed Charge vs diode distance",160,0,int(PixelSize*2),0,2000);
    ProfHOM_Charge2_diodedist = new TProfile("ProfHOM_Charge2_diodedist"," 2 pixels Charge vs diode distance",160,0,int(PixelSize*2),0,3000);
    ProfHOM_Charge4_diodedist = new TProfile("ProfHOM_Charge4_diodedist"," 4 pixels Charge vs diode distance",160,0,int(PixelSize*2),0,4000);
    ProfHOM_Charge9_diodedist = new TProfile("ProfHOM_Charge9_diodedist"," 9 pixels Charge vs diode distance",160,0,int(PixelSize*2),0,5000);
    ProfHOM_Charge25_diodedist = new TProfile("ProfHOM_Charge25_diodedist"," 25 pixels Charge vs diode distance",160,0,int(PixelSize*2),0,6000);
    
    hHOM_SNseed_diodedist  = new TH2F("hHOM_SNseed_diodedist","Seed S/N  vs diode distance",160,0,PixelSize*2,300,0,300);
    ProfHOM_SNseed_diodedist = new TProfile("ProfhHOM_SNseed_diodedist","Seed S/N  vs diode distance",160,0,PixelSize*2,0,300);
    
    hHOM_Charge_diodedist_00_10 = new TH1F("hHOM_Charge_diodedist_00_10","Seed Charge for diode dist 00-10% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_10_20 = new TH1F("hHOM_Charge_diodedist_10_20","Seed Charge for diode dist 10-20% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_20_30 = new TH1F("hHOM_Charge_diodedist_20_30","Seed Charge for diode dist 20-30% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_30_40 = new TH1F("hHOM_Charge_diodedist_30_40","Seed Charge for diode dist 30-40% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_40_50 = new TH1F("hHOM_Charge_diodedist_40_50","Seed Charge for diode dist 40-50% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_50_60 = new TH1F("hHOM_Charge_diodedist_50_60","Seed Charge for diode dist 50-60% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_60_70 = new TH1F("hHOM_Charge_diodedist_60_70","Seed Charge for diode dist 60-70% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_70_80 = new TH1F("hHOM_Charge_diodedist_70_80","Seed Charge for diode dist 70-80% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_80_90 = new TH1F("hHOM_Charge_diodedist_80_90","Seed Charge for diode dist 80-90% * pitch/sqrt(2)",200,0,2000);
    hHOM_Charge_diodedist_90_inf = new TH1F("hHOM_Charge_diodedist_90_inf","Seed Charge for diode dist 90-inf% * pitch/sqrt(2)",200,0,2000);
    
    hHOM_DU_Nevent  = new TH2F("hHOM_DU_Nevent","Residue U vs Nevent",500,0,500000,160,-PixelSize*2,PixelSize*2);
    hHOM_DV_Nevent  = new TH2F("hHOM_DV_Nevent","Residue V vs Nevent",500,0,500000,160,-PixelSize*2,PixelSize*2);
    
    hHOM_modtu_Nevent  = new TH2F("hHOM_modtu_Nevent","mod tu vs Nevent",500,0,500000,160,-PixelSize*2,PixelSize*2);
    hHOM_modtv_Nevent  = new TH2F("hHOM_modtv_Nevent","mod tv vs Nevent",500,0,500000,160,-PixelSize*2,PixelSize*2);
    
    hHOM_modUCG_Nevent  = new TH2F("hHOM_modUCG_Nevent","mod UCG vs Nevent",500,0,500000,160,-PixelSize*2,PixelSize*2);
    hHOM_modVCG_Nevent  = new TH2F("hHOM_modVCG_Nevent","mod VCG vs Nevent",500,0,500000,160,-PixelSize*2,PixelSize*2);
    
    hHOM_ResEta25_U = new TH1F("hHOM_ResEta25_U","Residue U (Eta25)",480,-ResRange-20,ResRange+20);
    hHOM_ResEta25_V = new TH1F("hHOM_ResEta25_V","Residue V (Eta25)",480,-ResRange-20,ResRange+20);
    
    
    hHOM_modUeta2x2_modVeta2x2  = new TH2F("hHOM_modUeta2x2_modVeta2x2","mod Eta2x2U vs mod Eta2x2V",160,-PixelSize,PixelSize,160,-PixelSize,PixelSize);
    hHOM_modUCG2x2_modVCG2x2  = new TH2F("hHOM_modUCG2x2_modVCG2x2","mod UCG2x2 vs mod VCG2x2 ",160,-PixelSize,PixelSize,160,-PixelSize,PixelSize);
    hHOM_modUeta5x5_modVeta5x5  = new TH2F("hHOM_modUeta5x5_modVeta5x5","mod Eta5x5U vs mod Eta5x5V",160,-PixelSize,PixelSize,160,-PixelSize,PixelSize);
    hHOM_modUCG5x5_modVCG5x5  = new TH2F("hHOM_modUCG5x5_modVCG5x5","mod UCG5x5 vs mod VCG5x5 ",160,-PixelSize,PixelSize,160,-PixelSize,PixelSize);

    //****************************************************
  } // end if RefTrack option on
  //****************************************************
  

  //------------------------------------------------------------------------------
  //----- Fake rate histos.
  //------------------------------------------------------------------------------
  if( ifFake ) { // If Fake histograms required
    if( HistDebug) cout << "HistoBooking: fake hits" << endl;
    
    hNhitperpixel = new TH1F("hNhitperpixel","Number of hits per pixel",5000,-0.5,4999.5);
    hNhitperpixel->SetXTitle("# hits");
    hNhitperpixel->SetYTitle("# pixels/total");
    // these few lines is a possible way to have "intelligent" binning for the fake rate
    // Jb 2010/07/26
    const Int_t nbins = 50;
    Double_t lowBinEdge[nbins+1];
    lowBinEdge[0]     = 1.e-9;
    lowBinEdge[nbins] = 1.e-0;
    Double_t logBinWidth = TMath::Log10(lowBinEdge[nbins]/lowBinEdge[0])/nbins;
    for( Short_t ib=1; ib<nbins; ib++) {
      lowBinEdge[ib] = lowBinEdge[ib-1] * TMath::Power(10,logBinWidth);
      //cout << "bin " << ib << " lowEdge = " << lowBinEdge[ib] << ", ";
    }
    // The two following histograms need to have the same binning
    hNhitRateperpixel = new TH1F("hNhitRateperpixel","Hit rate per pixel",1000,-0.000005,0.099995); //, nbins, lowBinEdge
    hNhitRateperpixel->SetXTitle("Fake rate per pixel");
    hNhitRateperpixel->SetYTitle("# pixels");
    
    hPixelsPerFakeRate = new TH1F( "hPixelsPerFakeRate", "Proportion of pixels above a given fake rate",1000,-0.000005,0.099995); //, nbins, lowBinEdge);
    hPixelsPerFakeRate->SetXTitle("Fake rate per pixel");
    hPixelsPerFakeRate->SetYTitle("# pixels/total");
  
  } // end If Fake histograms required

  
  //------------------------------------------------------------------------------
  //-----  MiniVectors Histograms ; NCS , 2010/01/21
  //------------------------------------------------------------------------------
  if( ifMiniVector ) { // If mimivector histograms required
  if( HistDebug) cout << "HistoBooking: minivectors" << endl;
  
    hDiffPosX   = new TH1F("hDiffPosX","Tx-Hx",121,-60.5,60.5);
    hDiffPosX->SetLineColor(2);hDiffPosX->SetLineWidth(2);
    hDiffPosX->GetXaxis()->SetTitle("Tx_{m} - Hx_{m} (#mum)");
    hDiffPosX->GetYaxis()->SetTitle("Counts");
    
    hDiffPosY   = new TH1F("hDiffPosY","Ty-Hy",121,-60.5,60.5);
    hDiffPosY->SetLineColor(2);hDiffPosY->SetLineWidth(2);
    hDiffPosY->GetXaxis()->SetTitle("Ty_{m} - Hy_{m} (#mum)");
    hDiffPosY->GetYaxis()->SetTitle("Counts");
    
    hDiffAngleX = new TH1F("hDiffAngleX","ThetaTx -ThetaHx (degrees)",100,-2.0,2.0);
    hDiffAngleX->SetLineColor(2);hDiffAngleX->SetLineWidth(2);
    hDiffAngleX->GetXaxis()->SetTitle("#thetatx - #thetahx (degrees)");
    hDiffAngleX->GetYaxis()->SetTitle("Counts");
    
    hDiffAngleY = new TH1F("hDiffAngleY","ThetaTy - ThetaHy(degrees)",100,-2.0,2.0);
    hDiffAngleY->SetLineColor(2);hDiffAngleY->SetLineWidth(2);
    hDiffAngleY->GetXaxis()->SetTitle("#thetaty - #thetahy (degrees)");
    hDiffAngleY->GetYaxis()->SetTitle("Counts");
    
    hDiffAngleX11 = new TH1F("hDiffAngleX11","ThetaTx -ThetaHx (degrees), 1 pix - 1 pix",100,-2.0,2.0);
    hDiffAngleX11->SetLineColor(2);hDiffAngleX->SetLineWidth(2);
    hDiffAngleX11->GetXaxis()->SetTitle("#thetatx - #thetahx (degrees)");
    hDiffAngleX11->GetYaxis()->SetTitle("Counts");
    hDiffAngleY11 = new TH1F("hDiffAngleY11","ThetaTy - ThetaHy(degrees), 1 pix - 1 pix",100,-2.0,2.0);
    hDiffAngleY11->SetLineColor(2);hDiffAngleY->SetLineWidth(2);
    hDiffAngleY11->GetXaxis()->SetTitle("#thetaty - #thetahy (degrees)");
    hDiffAngleY11->GetYaxis()->SetTitle("Counts");
    
    hDiffAngleX12 = new TH1F("hDiffAngleX12","ThetaTx -ThetaHx (degrees), 1 pix - 2 pix",100,-2.0,2.0);
    hDiffAngleX12->SetLineColor(2);hDiffAngleX->SetLineWidth(2);
    hDiffAngleX12->GetXaxis()->SetTitle("#thetatx - #thetahx (degrees)");
    hDiffAngleX12->GetYaxis()->SetTitle("Counts");
    hDiffAngleY12 = new TH1F("hDiffAngleY12","ThetaTy - ThetaHy(degrees), 1 pix - 2 pix",100,-2.0,2.0);
    hDiffAngleY12->SetLineColor(2);hDiffAngleY->SetLineWidth(2);
    hDiffAngleY12->GetXaxis()->SetTitle("#thetaty - #thetahy (degrees)");
    hDiffAngleY12->GetYaxis()->SetTitle("Counts");
    
    hDiffAngleX21 = new TH1F("hDiffAngleX21","ThetaTx -ThetaHx (degrees), 2 pix - 1 pix",100,-2.0,2.0);
    hDiffAngleX21->SetLineColor(2);hDiffAngleX->SetLineWidth(2);
    hDiffAngleX21->GetXaxis()->SetTitle("#thetatx - #thetahx (degrees)");
    hDiffAngleX21->GetYaxis()->SetTitle("Counts");
    hDiffAngleY21 = new TH1F("hDiffAngleY21","ThetaTy - ThetaHy(degrees), 2 pix - 1 pix",100,-2.0,2.0);
    hDiffAngleY21->SetLineColor(2);hDiffAngleY->SetLineWidth(2);
    hDiffAngleY21->GetXaxis()->SetTitle("#thetaty - #thetahy (degrees)");
    hDiffAngleY21->GetYaxis()->SetTitle("Counts");
    
    hDiffAngleX22 = new TH1F("hDiffAngleX22","ThetaTx -ThetaHx (degrees), 2 pix - 2 pix",100,-2.0,2.0);
    hDiffAngleX22->SetLineColor(2);hDiffAngleX->SetLineWidth(2);
    hDiffAngleX22->GetXaxis()->SetTitle("#thetatx - #thetahx (degrees)");
    hDiffAngleX22->GetYaxis()->SetTitle("Counts");
    hDiffAngleY22 = new TH1F("hDiffAngleY22","ThetaTy - ThetaHy(degrees), 2 pix - 2 pix",100,-2.0,2.0);
    hDiffAngleY22->SetLineColor(2);hDiffAngleY->SetLineWidth(2);
    hDiffAngleY22->GetXaxis()->SetTitle("#thetaty - #thetahy (degrees)");
    hDiffAngleY22->GetYaxis()->SetTitle("Counts");
    
    hDiffAngleXg1g1 = new TH1F("hDiffAngleXg1g1","ThetaTx -ThetaHx (degrees), >1 pix - >1 pix",100,-2.0,2.0);
    hDiffAngleXg1g1->SetLineColor(2);hDiffAngleX->SetLineWidth(2);
    hDiffAngleXg1g1->GetXaxis()->SetTitle("#thetatx - #thetahx (degrees)");
    hDiffAngleXg1g1->GetYaxis()->SetTitle("Counts");
    hDiffAngleYg1g1 = new TH1F("hDiffAngleYg1g1","ThetaTy - ThetaHy(degrees), >1 pix - >1 pix",100,-2.0,2.0);
    hDiffAngleYg1g1->SetLineColor(2);hDiffAngleY->SetLineWidth(2);
    hDiffAngleYg1g1->GetXaxis()->SetTitle("#thetaty - #thetahy (degrees)");
    hDiffAngleYg1g1->GetYaxis()->SetTitle("Counts");
    
    hxtxPL3     = new TH1F("hxtxPL3","tx-hx PL3",121,-60.5,60.5); // NCS 260110 to have the resolution of PL3 & PL4 with only Hu and not HuCG
    hxtxPL3->GetXaxis()->SetTitle("Tx_{m} - Hx_{m} (#mum)");
    hxtxPL3->GetYaxis()->SetTitle("Counts");
    hxtxPL3->SetLineColor(1);hxtxPL3->SetLineWidth(2);
    hytyPL3     = new TH1F("hytyPL3","ty-hy PL3",121,-60.5,60.5); // almost the same as huCGtu1 but same range as hDiffPosX 
    hytyPL3->SetLineColor(1);hytyPL3->SetLineWidth(2);                                                               // and there must a nearest hit to the track in both PL3 and PL4 => less entries than a plane alone
    hytyPL3->GetXaxis()->SetTitle("Ty_{m} - Hy_{m} (#mum)");
    hytyPL3->GetYaxis()->SetTitle("Counts");
    
    hxtxPL4     = new TH1F("hxtxPL4","tx-hx PL4",121,-60.5,60.5);
    hxtxPL4->GetXaxis()->SetTitle("Tx_{m} - Hx_{m} (#mum)");
    hxtxPL4->GetYaxis()->SetTitle("Counts");
    hxtxPL4->SetLineColor(4);hxtxPL4->SetLineWidth(2);
    hytyPL4     = new TH1F("hytyPL4","ty-hy PL4",121,-60.5,60.5);
    hytyPL4->GetXaxis()->SetTitle("Ty_{m} - Hy_{m} (#mum)");
    hytyPL4->GetYaxis()->SetTitle("Counts");
    hytyPL4->SetLineColor(4);hytyPL4->SetLineWidth(2);
    
    hutuPL3     = new TH1F("hutuPL3","tu-hu PL3",121,-60.5,60.5);
    hutuPL3->GetXaxis()->SetTitle("Tx_{m} - Hx_{m} (#mum)");
    hutuPL3->GetYaxis()->SetTitle("Counts");
    hutuPL3->SetLineColor(1);hutuPL3->SetLineWidth(2);
    hvtvPL3     = new TH1F("hvtvPL3","tv-hv PL3",121,-60.5,60.5);
    hvtvPL3->SetLineColor(1);hvtvPL3->SetLineWidth(2);
    hvtvPL3->GetXaxis()->SetTitle("Ty_{m} - Hy_{m} (#mum)");
    hvtvPL3->GetYaxis()->SetTitle("Counts");
    
    hutuPL4     = new TH1F("hutuPL4","tu-hu PL4",121,-60.5,60.5);
    hutuPL4->GetXaxis()->SetTitle("Tx_{m} - Hx_{m} (#mum)");
    hutuPL4->GetYaxis()->SetTitle("Counts");
    hutuPL4->SetLineColor(4);hutuPL4->SetLineWidth(2);
    hvtvPL4     = new TH1F("hvtvPL4","tv-hv PL4",121,-60.5,60.5);
    hvtvPL4->SetLineColor(4);hvtvPL4->SetLineWidth(2);
    hvtvPL4->GetXaxis()->SetTitle("Ty_{m} - Hy_{m} (#mum)");
    hvtvPL4->GetYaxis()->SetTitle("Counts");
    
    hdiffydiffx = new TH2F("hdiffydiffx", "Position differences between sides in 2D", 100, -25, 25, 100, -25, 25);
    hdiffydiffx->GetXaxis()->SetTitle("X(side2)-X(side1) (#mum)");
    hdiffydiffx->GetYaxis()->SetTitle("Y(side2)-Y(side1) (#mum)");
    hdiffydiffx11 = new TH2F("hdiffydiffx11", "Position differences between sides in 2D, 1 pix - 1 pix", 100, -25, 25, 100, -25, 25);
    hdiffydiffx11->GetXaxis()->SetTitle("X(side2)-X(side1) (#mum)");
    hdiffydiffx11->GetYaxis()->SetTitle("Y(side2)-Y(side1) (#mum)");
    hdiffydiffx12 = new TH2F("hdiffydiffx12", "Position differences between sides in 2D, 1 pix - 2 pix", 100, -25, 25, 100, -25, 25);
    hdiffydiffx12->GetXaxis()->SetTitle("X(side2)-X(side1) (#mum)");
    hdiffydiffx12->GetYaxis()->SetTitle("Y(side2)-Y(side1) (#mum)");
    hdiffydiffx21 = new TH2F("hdiffydiffx21", "Position differences between sides in 2D, 2 pix - 1 pix", 100, -25, 25, 100, -25, 25);
    hdiffydiffx21->GetXaxis()->SetTitle("X(side2)-X(side1) (#mum)");
    hdiffydiffx21->GetYaxis()->SetTitle("Y(side2)-Y(side1) (#mum)");
    hdiffydiffx22 = new TH2F("hdiffydiffx22", "Position differences between sides in 2D, 2 pix - 2 pix", 100, -25, 25, 100, -25, 25);
    hdiffydiffx22->GetXaxis()->SetTitle("X(side2)-X(side1) (#mum)");
    hdiffydiffx22->GetYaxis()->SetTitle("Y(side2)-Y(side1) (#mum)");
    hdiffydiffxg1g1 = new TH2F("hdiffydiffxg1g1", "Position differences between sides in 2D, >1 pix - >1 pix", 100, -25, 25, 100, -25, 25);
    hdiffydiffxg1g1->GetXaxis()->SetTitle("X(side2)-X(side1) (#mum)");
    hdiffydiffxg1g1->GetYaxis()->SetTitle("Y(side2)-Y(side1) (#mum)");
 
  } // end If mimivector histograms required

  
  //------------------------------------------------------------------------------ 
  // MimosaVertexFinder Histograms ; LC , 2012/09/06
  //------------------------------------------------------------------------------ 
  if( ifVertex ) { // If vertex histograms required
    if( HistDebug) cout << "HistoBooking: vertices" << endl;
    
    hVertexPosX = new TH1F("hVertexPosX","Vertex position in X",100,-50,50);
    hVertexPosX->GetXaxis()->SetTitle("X");
    
    hVertexPosY = new TH1F("hVertexPosY","Position in Y of tracks in plane z=0 (Plane of the vertex)",400,-200,200);
    hVertexPosY->GetXaxis()->SetTitle("Y");
    
    hVertexPosZ = new TH1F("hVertexPosZ","Vertex position in Z",100,-50,50);
    hVertexPosZ->GetXaxis()->SetTitle("Z");
    
    hVertexTrackDistance = new TH1F("hVertexTrackDistance","Tracks distance to the point (0,0,0)",100,0,50);
    hVertexTrackDistance->GetXaxis()->SetTitle("Distance of tracks to the point (0,0,0)");
    
    /* Difference with histo below not investigated...JB 2013/08/20
     hVertexPosXY = new TH2F("hVertexPosXY","Position in X&Y of a track in plane z=0 (Plane of the vertex)",2000,-300,300,2000,-300,300);
     hVertexPosXY->GetXaxis()->SetTitle("X position for the intersection track/plane z=0");
     hVertexPosXY->GetYaxis()->SetTitle("Y position for the intersection track/plane z=0");
     */
    
    hVertexPosXY = new TH2F("hVertexPosXY","Vertex position in X&Y",500,-50,50,500,-50,50);
    hVertexPosXY->GetXaxis()->SetTitle("Vertex position in X");
    hVertexPosXY->GetYaxis()->SetTitle("Vertex position in Y");
    
    hVertexTrackChi2 = new TH1F("hVertexTrackChi2","Track Chi2",200,0,25);
    hVertexTrackChi2->GetXaxis()->SetTitle("Track Chi2");

  } // end If vertex histograms required
  
  //------------------------------------------------------------------------------
  //----- Histograms not used !!!
  //------------------------------------------------------------------------------
    
  /*
  hChargeOrder1 =  new TH1F("hChargeOrder1"," Charge of the 1st (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder2 =  new TH1F("hChargeOrder2"," Charge of the 2nd (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder3 =  new TH1F("hChargeOrder3"," Charge of the 3rd (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder4 =  new TH1F("hChargeOrder4"," Charge of the 4th (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder5 =  new TH1F("hChargeOrder5"," Charge of the 5th (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder6 =  new TH1F("hChargeOrder6"," Charge of the 6th (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder7 =  new TH1F("hChargeOrder7"," Charge of the 7th (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder8 =  new TH1F("hChargeOrder8"," Charge of the 8th (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);
  hChargeOrder9 =  new TH1F("hChargeOrder9"," Charge of the 9th (no order) pixel (e-)",ChargePixNbins,.0,ChargePixRange);

  hGOODChargeOrder1 =  new TH1F("hGOODChargeOrder1","GOOD Ch of the 1st highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder2 =  new TH1F("hGOODChargeOrder2","GOOD Ch of the 2nd highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder3 =  new TH1F("hGOODChargeOrder3","GOOD Ch of the 3rd highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder4 =  new TH1F("hGOODChargeOrder4","GOOD Ch of the 4th highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder5 =  new TH1F("hGOODChargeOrder5","GOOD Ch of the 5th highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder6 =  new TH1F("hGOODChargeOrder6","GOOD Ch of the 6th highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder7 =  new TH1F("hGOODChargeOrder7","GOOD Ch of the 7th highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder8 =  new TH1F("hGOODChargeOrder8","GOOD Ch of the 8th highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  hGOODChargeOrder9 =  new TH1F("hGOODChargeOrder9","GOOD Ch of the 9th highest pixel (e-)",ChargeNbins,-0.05*ChargeRange,ChargeRange);
  
  hGOODChargeCor_1_2 = new TH2F("hGOODChargeCor_1_2","GOOD Ch pixel 1 vs pixel 2 (e-)",ChargeNbins/4,0.0,ChargeRange,ChargeNbins/4,0.0,ChargeRange);
  hGOODChargeCor_1_3 = new TH2F("hGOODChargeCor_1_3","GOOD Ch pixel 1 vs pixel 3 (e-)",ChargeNbins/4,0.0,ChargeRange,ChargeNbins/4,0.0,ChargeRange);
  hGOODChargeCor_1_4 = new TH2F("hGOODChargeCor_1_4","GOOD Ch pixel 1 vs pixel 4 (e-)",ChargeNbins/4,0.0,ChargeRange,ChargeNbins/4,0.0,ChargeRange);
  hGOODChargeCor_2_3 = new TH2F("hGOODChargeCor_2_3","GOOD Ch pixel 2 vs pixel 3 (e-)",ChargeNbins/4,0.0,ChargeRange,ChargeNbins/4,0.0,ChargeRange);
  
  hGOODChargeSum_4 = new TH1F("hGOODChargeSum_4","GOOD Sum of the 4 highest pixel (e-)",ChargeNbins,0.0,ChargeRange);
  
  hGOODChargeRap_1_over_2 = new TH1F("hGOODChargeRap_1_over_2","GOOD Ratio of 1st over 2nd pixel",100,0.0,10.0);
  hGOODChargeRap_1_over_3 = new TH1F("hGOODChargeRap_1_over_3","GOOD Ratio of 1st over 3rd pixel",100,0.0,10.0);
  hGOODChargeRap_1_over_4 = new TH1F("hGOODChargeRap_1_over_4","GOOD Ratio of 1st over 4th pixel",100,0.0,10.0);
  hGOODChargeRap_2_over_3 = new TH1F("hGOODChargeRap_2_over_3","GOOD Ratio of 2nd over 3rd pixel",100,0.0,10.0);
  */

  //------------------------------------------------------------------------------ 
  // MC Geneation for telescope resolution evaluation
  //------------------------------------------------------------------------------ 

  
  //------------------------------------------------------------------------------
  //----- User histograms
  // JB 2009/09/07
  //------------------------------------------------------------------------------
  if( HistDebug) cout << "HistoBooking: user" << endl;
  
  hUserHitCorrelationLine = new TH1F("hUserhitCorrelationLine", "Line correlation between hits", 300, humin, humax);
  hUserHitCorrelationLine->SetXTitle("Horizontal distance (#mum)");
  hUserHitCorrelationCol = new TH1F("hUserhitCorrelationCol", "Column correlation between hits", 300, hvmin, hvmax);
  hUserHitCorrelationCol->SetXTitle("Vertical distance (#mum)");

  
  if( ifImaging ) { // If imaging histograms required
    if(HistDebug) cout << "HistoBooking: imaging" << endl;

     h1RmsOnTheta = new TH1F("h1RmsOnTheta", "RMS according to the angle #theta", 400, 0.0, 180.0);
     h1RmsOnTheta->GetXaxis()->SetTitle("#theta (degree)");
     h1RmsOnTheta->GetYaxis()->SetTitle("RMS");
     h1RmsOnTheta->SetLineColor(kBlue);
     h1RmsOnTheta->SetLineWidth(2);
     
     Int_t NstepOnX = int((geomUmax-geomUmin)/PixelSize)/1.0;
     Int_t NstepOnY = int((geomVmax-geomVmin)/PixelSize)/1.0;
    
     h1ProjectionOnX = new TH1F("h1ProjectionOnX", "Data projection on the x'-axis of the chart reference frame", NstepOnX, geomUmin, geomUmax);
     h1ProjectionOnX->GetXaxis()->SetTitle("x' (#mum)");
     h1ProjectionOnX->GetYaxis()->SetTitle("Number of hits");
    
     h1ProjectionOnY = new TH1F("h1ProjectionOnY", "Data projection on the y'-axis of the chart reference frame", NstepOnY, geomVmin, geomVmax);
     h1ProjectionOnY->GetXaxis()->SetTitle("y' (#mum)");
     h1ProjectionOnY->GetYaxis()->SetTitle("Number of hits");

     TString namehist, titlehist;
     Int_t NstepOnXMult = int((humax-humin)/PixelSize)/1.0;
     Int_t NstepOnYMult = int((hvmax-hvmin)/PixelSize)/1.0;

     
     for (Int_t i=0; i<4; i++) {
     
       namehist = TString("h1ProjectionOnXMult") + long(i+1);
       if (i<3)  titlehist = TString("Data projection on the x'-axis of the chart reference frame with the clusters multiplicity set to ") + long(i+1);
       else      titlehist = TString("Data projection on the x'-axis of the chart reference frame with the clusters multiplicity up to ") + long(i+1);
       
       h1ProjectionOnXMult[i] = new TH1F(namehist.Data(), titlehist.Data(), NstepOnX, geomUmin, geomUmax);
       h1ProjectionOnXMult[i]->GetXaxis()->SetTitle("x' (#mum)");
       h1ProjectionOnXMult[i]->GetYaxis()->SetTitle("Number of hits");

       
       namehist = TString("h1ProjectionOnYMult") + long(i+1);
       if (i<3)  titlehist = TString("Data projection on the y'-axis of the chart reference frame with the clusters multiplicity set to ") + long(i+1);
       else      titlehist = TString("Data projection on the y'-axis of the chart reference frame with the clusters multiplicity up to ") + long(i+1);
       
       h1ProjectionOnYMult[i] = new TH1F(namehist.Data(), titlehist.Data(), NstepOnY, geomVmin, geomVmax);
       h1ProjectionOnYMult[i]->GetXaxis()->SetTitle("y' (#mum)");
       h1ProjectionOnYMult[i]->GetYaxis()->SetTitle("Number of hits");

       
       namehist = TString("h2GoodHitsMult") + long(i+1);
       if (i<3)  titlehist = TString("Good hits map of the region of interest with the clusters multiplicity set to ") + long(i+1);
       else      titlehist = TString("Good hits map of the region of interest with the clusters multiplicity up to ") + long(i+1);

       h2GoodHitsMult[i] = new TH2F(namehist.Data(), titlehist.Data(), NstepOnXMult, humin, humax, NstepOnYMult, hvmin, hvmax);
       h2GoodHitsMult[i]->GetXaxis()->SetTitle("x (#mum)");
       h2GoodHitsMult[i]->GetYaxis()->SetTitle("y (#mum)");
       h2GoodHitsMult[i]->GetZaxis()->SetTitle("Number of hits");
     
       namehist = TString("hdCGDigUVMult") + long(i+1);
       titlehist = TString("MATCHED hits CoG-digital V vs U - Multiplicity = ") + long(i+1);
       hdCGDigUVMult[i] = new TH2F(namehist.Data(), titlehist.Data(), int(PixelSize*4),-PixelSize*2,PixelSize*2,int(PixelSize*4),-PixelSize*2,PixelSize*2);
       hdCGDigUV->SetXTitle("(#mum)");
       hdCGDigUV->SetYTitle("(#mum)");
       
     }

     hdCGDigUVMult[4] = new TH2F("hdCGDigUVMult5", "MATCHED hits CoG-digital V vs U - Multiplicity = 5 ", int(PixelSize*4),-PixelSize*2,PixelSize*2,int(PixelSize*4),-PixelSize*2,PixelSize*2);
     hdCGDigUV->SetXTitle("(#mum)");
     hdCGDigUV->SetYTitle("(#mum)");
     
     hdCGDigUVMult[5] = new TH2F("hdCGDigUVMult6", "MATCHED hits CoG-digital V vs U - Multiplicity #geq 6 ", int(PixelSize*4),-PixelSize*2,PixelSize*2,int(PixelSize*4),-PixelSize*2,PixelSize*2);
     hdCGDigUV->SetXTitle("(#mum)");
     hdCGDigUV->SetYTitle("(#mum)");

     
     h1Sigma = new TH1F("h1Sigma", "Sensor spatial resolution according to clusters multiplicity", 5, 0, 1);
     h1Sigma->GetXaxis()->SetBinLabel(1, "Global");
     for (Int_t i=1; i<=3; i++) h1Sigma->GetXaxis()->SetBinLabel(i+1, Form("Multiplicity = %d", i));
     h1Sigma->GetXaxis()->SetBinLabel(5, "Multiplicity #geq 4");
     h1Sigma->GetYaxis()->SetTitle("Spatial resolution #sigma (#mum)");
    
     
     h1NumberOfHitsMult = new TH1F("h1NumberOfHitsMult", "Hits rate in the region of interest", 5, 0, 1);
     for (Int_t i=1; i<=4; i++) h1NumberOfHitsMult->GetXaxis()->SetBinLabel(i, Form("Multiplicity = %d", i));
     h1NumberOfHitsMult->GetXaxis()->SetBinLabel(5, "Multiplicity #geq 4");
     h1NumberOfHitsMult->GetYaxis()->SetTitle("N_{Hits} / N_{Hits total} (%)");
     
  } // end If imaging histograms required    
  
  //------------------------------------------------------------------------------
  //----- Done
  //------------------------------------------------------------------------------
  
  Info("MHist","All histograms booked."); 

}

//______________________________________________________________________________
//
 void MHist::Clear(const Option_t*)
{

  if(!dir) return ;

//------------------------------------------------------------------------------
//----- general control
//------------------------------------------------------------------------------

  selection=Zero(selection);

//------------------------------------------------------------------------------
//----- Charges, noise, S/N, pixel multiplicity plots for SELECTED clusters
//------------------------------------------------------------------------------

  hdummy=Zero(hdummy);
  hChargeInSeed=Zero(hChargeInSeed);
  hRealTrackNoise_time=Zero(hRealTrackNoise_time) ;
  hRealTrackNoise=Zero(hRealTrackNoise); 
  hsn=Zero(hsn);
  hSNReal=Zero(hSNReal);

  hind=Zero(hind);


  hS2N2All=Zero(hS2N2All); 
  hS2N2nd=Zero(hS2N2nd);
  hS2N2RH=Zero(hS2N2RH);
  hS2N2ndRH=Zero(hS2N2ndRH);

  hChargeIntegral1=Zero(hChargeIntegral1);
  hChargeNorm1=Zero(hChargeNorm1);
  hChargeIntegral2=Zero(hChargeIntegral2); 
  hChargeNorm2=Zero(hChargeNorm2);
  hChargeIntegral3=Zero(hChargeIntegral3);
  hChargeNorm3=Zero(hChargeNorm3);
  hChargeIntegral4=Zero(hChargeIntegral4);
  hChargeNorm4=Zero(hChargeNorm4);
  hChargeIntegral=Zero(hChargeIntegral);
  hChargeIntSmoothed=Zero(hChargeIntSmoothed);
  hQ3x34=Zero(hQ3x34);
  hQ3x35=Zero(hQ3x35);
  hQ3x345=Zero(hQ3x345);
  hQ3x327=Zero(hQ3x327);
  hQ3x345r=Zero(hQ3x345r);
  hQ3x327r=Zero(hQ3x327r);


  hraw1badone=Zero(hraw1badone);
  hraw1goodone=Zero(hraw1goodone);
  hraw1goodone_time=Zero(hraw1goodone_time) ;
  hraw1badone_time=Zero(hraw1badone_time) ;
  hraw1goodoneNoise_time=Zero(hraw1goodoneNoise_time) ;
  hraw1badoneNoise_time=Zero(hraw1badoneNoise_time) ;

  hraw1PFrfr1GOOD=Zero(hraw1PFrfr1GOOD) ;
  hraw1PFrfr1BAD=Zero(hraw1PFrfr1BAD) ;
  hraw1PFrfr1GOOD_time=Zero(hraw1PFrfr1GOOD_time) ;
  hraw1PFrfr1BAD_time=Zero(hraw1PFrfr1BAD_time) ;
  hraw1PFrfr2GOOD=Zero(hraw1PFrfr2GOOD) ;
  hraw1PFrfr2BAD=Zero(hraw1PFrfr2BAD) ;
  hraw1PFrfr2GOOD_time=Zero(hraw1PFrfr2GOOD_time) ;
  hraw1PFrfr2BAD_time=Zero(hraw1PFrfr2BAD_time) ;
  hraw1NoiseGOOD_time=Zero(hraw1NoiseGOOD_time) ;
  hraw1NoiseBAD_time=Zero(hraw1NoiseBAD_time) ;

  hraw1Noise=Zero(hraw1Noise) ;
  hraw1Pedestal=Zero(hraw1Pedestal) ;
  hraw1CDS=Zero(hraw1CDS) ;
  hraw1Signal=Zero(hraw1Signal) ;
  hraw1Noise_time=Zero(hraw1Noise_time) ;
  hraw1Pedestal_time=Zero(hraw1Pedestal_time) ;
  hraw1CDS_time=Zero(hraw1CDS_time) ;
  hraw1Signal_time=Zero(hraw1Signal_time) ;

  for (Int_t i=0 ; i<MaxNofPixelsInCluster ; i++) {
    hqcn[i]=Zero(hqcn[i]);
    hqcngeom[i]=Zero(hqcngeom[i]);
    hindivq[i]=Zero(hindivq[i]);
    hsnn[i]=Zero(hsnn[i]);
  }

  hQofPix3x3=Zero(hQofPix3x3);
  hChargeInCluster=Zero(hChargeInCluster); 

  hseedQvsS2NAll=Zero(hseedQvsS2NAll);
  hseedQvsS2NGood=Zero(hseedQvsS2NGood);
  hQseedAll=Zero(hQseedAll);
  hIndex2x2=Zero(hIndex2x2);

  hnhit=Zero(hnhit);

  cClusterCharge=Zero(cClusterCharge);

  //------------------------------------------------------------------------------
  //--- Charges for calibration peak
  //------------------------------------------------------------------------------

  hqSeedCalibration=Zero(hqSeedCalibration);
  hqNeighbourCalibration=Zero(hqNeighbourCalibration);
  hqSeedVsNeighbourCalibration=Zero(hqSeedVsNeighbourCalibration);

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //--- INITIALISATION OF ALL HISTOGRAM/CANVAS POINTERS TO NULL
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  
  cCalibration=Zero(cCalibration);
  Calib_distr1=Zero(Calib_distr1);
  Calib_distr2=Zero(Calib_distr2);
  Calib_distr3=Zero(Calib_distr3);

  vec=Zero(vec);
  tudv=Zero(tudv);
  tvdu=Zero(tvdu);
  hudv=Zero(hudv);
  hvdu=Zero(hvdu);
  tuhu1=Zero(tuhu1);
  tvhv1=Zero(tvhv1);
  tuhu=Zero(tuhu);
  tvhv=Zero(tvhv);
  tuhv=Zero(tuhv);
  tvhu=Zero(tvhu);

  hAllHvvsAllTv=Zero(hAllHvvsAllTv);
  hAllHuvsAllTv=Zero(hAllHuvsAllTv);
  hAllHvvsAllTu=Zero(hAllHvvsAllTu);
  hAllHuvsAllTu=Zero(hAllHuvsAllTu);

  huCG5tu=Zero(huCG5tu);
  hvCG5tv=Zero(hvCG5tv);
  huCGtu=Zero(huCGtu);
  hvCGtv=Zero(hvCGtv);
  huCGtuInPix=Zero(huCGtuInPix);
  hvCGtvInPix=Zero(hvCGtvInPix);
  huCGtuInPix5=Zero(huCGtuInPix5);
  hvCGtvInPix5=Zero(hvCGtvInPix5);
  htuvInPix=Zero(htuvInPix);
  ProfUCG=Zero(ProfUCG);
  ProfVCG=Zero(ProfVCG);
  ProfACGn=Zero(ProfACGn);
  hUeta3TuInPix=Zero(hUeta3TuInPix);
  hVeta3TvInPix=Zero(hVeta3TvInPix);
  hUcorTuInPix=Zero(hUcorTuInPix);
  hVcorTvInPix=Zero(hVcorTvInPix);
  huCGtuInPix4=Zero(huCGtuInPix4);
  huCG2x2tuInPix=Zero(huCG2x2tuInPix);
  hvCG2x2tvInPix=Zero(hvCG2x2tvInPix); 
  hEta2x2tu1L=Zero(hEta2x2tu1L);
  hEta2x2tv1L=Zero(hEta2x2tv1L);
  hEta2x2tu2L=Zero(hEta2x2tu2L);
  hEta2x2tv2L=Zero(hEta2x2tv2L);
  hCG2x2tu1L=Zero(hCG2x2tu1L);
  hCG2x2tv1L=Zero(hCG2x2tv1L);

  hEta2x2=Zero(hEta2x2);
  hEta2x2U=Zero(hEta2x2U);
  hEta2x2V=Zero(hEta2x2V);
  hEta2x2m=Zero(hEta2x2m);

  hEta2x2UL=Zero(hEta2x2UL);
  hEta2x2VL=Zero(hEta2x2VL);

  hCG5URes=Zero(hCG5URes);
  hCG5VRes=Zero(hCG5VRes);
  huCGtu1=Zero(huCGtu1);
  hvCGtv1=Zero(hvCGtv1);
  huCGtu2=Zero(huCGtu2);
  hvCGtv2=Zero(hvCGtv2);
  hAHTURes=Zero(hAHTURes);
  hAHTVRes=Zero(hAHTVRes);
  hEta3URes=Zero(hEta3URes);
  hEta3VRes=Zero(hEta3VRes);
  hdCGEtaU=Zero(hdCGEtaU);
  hdCGEtaV=Zero(hdCGEtaV);
  hCG2x2tu1=Zero(hCG2x2tu1);
  hCG2x2tv1=Zero(hCG2x2tv1);
  hEta2x2tu1=Zero(hEta2x2tu1);
  hEta2x2tv1=Zero(hEta2x2tv1);
  hdCGDigU=Zero(hdCGDigU);
  hdCGDigV=Zero(hdCGDigV);
  hdCGDigUV=Zero(hdCGDigUV);
  htmp5=Zero(htmp5);
  hEta2x2tu2=Zero(hEta2x2tu2);
  hEta2x2tv2=Zero(hEta2x2tv2);
  hTuHuCorr=Zero(hTuHuCorr);
  hTvHvCorr=Zero(hTvHvCorr);
  hTHCorr2=Zero(hTHCorr2);
  hEtaURes=Zero(hEtaURes);
  hEtaVRes=Zero(hEtaVRes);
  hDifCorU=Zero(hDifCorU);
  hDifCorV=Zero(hDifCorV);
  hAllHuvsAllTu1=Zero(hAllHuvsAllTu1);  
  hAllHvvsAllTv1=Zero(hAllHvvsAllTv1);
  hAllHuvsAllTu2=Zero(hAllHuvsAllTu2);
  hAllHvvsAllTv2=Zero(hAllHvvsAllTv2);
  h2dallhits=Zero(h2dallhits);
  h2dgoodhits=Zero(h2dgoodhits);
  h2dmatchedhits=Zero(h2dmatchedhits);
  h2DpictureMatched=Zero(h2DpictureMatched); // JB 2014/01/10
  hEta2x2vsInd=Zero(hEta2x2vsInd);  
  hChargeVsPosition=Zero(hChargeVsPosition); 
  hChargeVsDistance=Zero(hChargeVsDistance); // clm 2013/07/16
  hNorm=Zero(hNorm);
  hAllHitsInPixel=Zero(hAllHitsInPixel); 

  hAlignHuTu=Zero(hAlignHuTu);
  hAlignHvTv=Zero(hAlignHvTv);
  hAlignHuTuvsHv=Zero(hAlignHuTuvsHv);
  hAlignHvTvvsHu=Zero(hAlignHvTvvsHu);
  hAlignHuTuvsTv=Zero(hAlignHuTuvsTv);
  hAlignHvTvvsTu=Zero(hAlignHvTvvsTu);
  hAlignHuTuvsTu=Zero(hAlignHuTuvsTu);
  hAlignHvTvvsTv=Zero(hAlignHvTvvsTv);
  
  huv=Zero(huv);
  huvBad=Zero(huvBad);
  hxy=Zero(hxy);
  hxyBad=Zero(hxyBad);
  tuv=Zero(tuv);
  tuv1=Zero(tuv1);

  hhu=Zero(hhu);   
  hhv=Zero(hhv);
  hhuS=Zero(hhuS);
  hhvS=Zero(hhvS);  
  hAllTu=Zero(hAllTu);
  hAllTv=Zero(hAllTv);
  hGoodChi2Tu=Zero(hGoodChi2Tu); 
  hGoodChi2Tv=Zero(hGoodChi2Tv);
  hGoodChi2Tx=Zero(hGoodChi2Tx);
  hGoodChi2Ty=Zero(hGoodChi2Ty);
  hGoodChi2AngleXZ=Zero(hGoodChi2AngleXZ);
  hGoodChi2AngleYZ=Zero(hGoodChi2AngleYZ);
  htu=Zero(htu);    
  htv=Zero(htv);

  htuhtv=Zero(htuhtv);
  hqc=Zero(hqc);
  hqcel=Zero(hqcel);
  hGOODqcel=Zero(hGOODqcel);
  hsnc1=Zero(hsnc1);
  hgoodSeedPixel=Zero(hgoodSeedPixel);
  h2DgoodSeedPixel=Zero(h2DgoodSeedPixel);
  hSeedBetweenDist=Zero(hSeedBetweenDist);
  hEta3DigU=Zero(hEta3DigU);
  hEta3DigV=Zero(hEta3DigV);
  hqc_c=Zero(hqc_c);
  hqc_nc=Zero(hqc_nc);

  hoptimalsize=Zero(hoptimalsize);
  hnpix=Zero(hnpix);
  hnpix_c=Zero(hnpix_c);
  hnpixCumul_c=Zero(hnpixCumul_c);
  hnpix_nc=Zero(hnpix_nc);

  FalseHitMap=Zero(FalseHitMap); 
  hClusterChargeProfile=Zero(hClusterChargeProfile);
  hClusterChargeNorm=Zero(hClusterChargeNorm);
  /*
    etal=Zero(etal,2);
    etal1=Zero((TObject**)etal1,2);
    vecu=Zero((TObject**)vecu,25);
    vecv=Zero((TObject**)vecv,25);
    hChargeInSeedInGroup=Zero((TObject**)hChargeInSeedInGroup,64);
  */
  DuvCG=Zero(DuvCG);
  duvall=Zero(duvall);
  hTrackToClusterMinDistance=Zero(hTrackToClusterMinDistance); 
  hMinDistance_vs_2ndDistance=Zero(hMinDistance_vs_2ndDistance); 

  hCDSvar=Zero(hCDSvar);
  CDSVarvsTime=Zero(CDSVarvsTime); 
  dtime=Zero(dtime);

  //----ADC 
  hPedestal=Zero(hPedestal);
  //---ADC

  /*
    titre=Zero((TObject**)titre,50); 
    nom=Zero((TObject**)nom,50);  
  */

  hsnc=Zero(hsnc);
  /*
    hsn_pix_0=Zero((TObject**)hsn_pix_0,jpixmax);
    hsn_pix_1=Zero((TObject**)hsn_pix_1,jpixmax);

    hsn_seed_vs_pix_0=Zero((TObject**)hsn_seed_vs_pix_0,jpixmax);

    hsn_seed_vs_pix_1=Zero((TObject**)hsn_seed_vs_pix_1,jpixmax);

    hsnALL_seed_vs_pix_0=Zero((TObject**)hsnALL_seed_vs_pix_0,jpixmax);

    hsnALL_seed_vs_pix_1=Zero((TObject**)hsnALL_seed_vs_pix_1,jpixmax);
  */

  hNTracksPerEvent=Zero(hNTracksPerEvent);
  hNTracksPerEventievt=Zero(hNTracksPerEventievt);
  hNGoodGeomTracksPerEvent=Zero(hNGoodGeomTracksPerEvent);
  hAllTvTu=Zero(hAllTvTu);
  hGoodChi2TvTu=Zero(hGoodChi2TvTu);
  hchi2_c=Zero(hchi2_c);  
  hchi2_nc=Zero(hchi2_nc);
  hchi2=Zero(hchi2);

  hNhitsPerTrack_all=Zero(hNhitsPerTrack_all);
  hNhitsPerTrack_good=Zero(hNhitsPerTrack_good);
  hNhitsPerTrack_matched=Zero(hNhitsPerTrack_matched);

  /*
    hRef_Tud_vs_Tv=Zero((TObject**)hRef_Tud_vs_Tv,NRefPlane);
    hRef_Tud_vs_Tu=Zero((TObject**)hRef_Tud_vs_Tu,NRefPlane);
    hRef_Tud_vs_TDOX=Zero((TObject**)hRef_Tud_vs_TDOX,NRefPlane);
    hRef_Tud_vs_TDOY=Zero((TObject**)hRef_Tud_vs_TDOY,NRefPlane);
    hRef_Tud_vs_Chi2=Zero((TObject**)hRef_Tud_vs_Chi2,NRefPlane);
    hRef_Tud_vs_Tu1=Zero((TObject**)hRef_Tud_vs_Tu1,NRefPlane);
    hRef_Tud_vs_Tk1=Zero((TObject**)hRef_Tud_vs_Tk1,NRefPlane);
  */

  hnhitievt=Zero(hnhitievt);
  hnahitievt=Zero(hnahitievt);
  hnGOODhit=Zero(hnGOODhit);
  hnGOODhitwhentrack=Zero(hnGOODhitwhentrack);

  hAllS2N=Zero(hAllS2N);
  hallhitSN=Zero(hallhitSN);
  hallSNneighbour=Zero(hallSNneighbour); 
  hgoodSNneighbour=Zero(hgoodSNneighbour); 
  hSNneighbour=Zero(hSNneighbour);
  hSNseedvsSNneighbour=Zero(hSNseedvsSNneighbour);
  hQseedvsQcluster=Zero(hQseedvsQcluster);
  hQseedvsQneighbour=Zero(hQseedvsQneighbour);
  hSNseedvsQcluster=Zero(hSNseedvsQcluster);
  hgoodSN_vs_SNN=Zero(hgoodSN_vs_SNN);
  hallSN_vs_SNN=Zero(hallSN_vs_SNN);

  hallSN_minus_hgoodSN_vs_SNN=Zero(hallSN_minus_hgoodSN_vs_SNN);
  hdistchi2=Zero(hdistchi2);

  hSNNReal=Zero(hSNNReal);
  hSN_vs_SNNReal=Zero(hSN_vs_SNNReal);

  hChargeCor_1_2=Zero(hChargeCor_1_2); 
  hChargeCor_1_3=Zero(hChargeCor_1_3);
  hChargeCor_1_4=Zero(hChargeCor_1_4);
  hChargeCor_2_3=Zero(hChargeCor_2_3);
  hChargeSum_4=Zero(hChargeSum_4);

  hChargeRap_1_over_2=Zero(hChargeRap_1_over_2);
  hChargeRap_1_over_3=Zero(hChargeRap_1_over_3);
  hChargeRap_1_over_4=Zero(hChargeRap_1_over_4);
  hChargeRap_2_over_3=Zero(hChargeRap_2_over_3);

  hBinary_test=Zero(hBinary_test);
  hBinary_NumberOf_1_ALL=Zero(hBinary_NumberOf_1_ALL);
  hBinary_NumberOf_1_goodhit=Zero(hBinary_NumberOf_1_goodhit);
  hBinary_NumberOf_1_submatrix=Zero(hBinary_NumberOf_1_submatrix);
  hBinary_Nhitperpixel_submatrix =Zero(hBinary_Nhitperpixel_submatrix);
  hBinary_NhitRateperpixel_submatrix =Zero(hBinary_NhitRateperpixel_submatrix);

  hNhitperpixel =Zero(hNhitperpixel);
  hNhitRateperpixel =Zero(hNhitRateperpixel);
  hPixelsPerFakeRate =Zero(hPixelsPerFakeRate);

  hHOM_tu_tv_modulo =Zero(hHOM_tu_tv_modulo);
  hHOM_ResU_tu =Zero(hHOM_ResU_tu);
  hHOM_ResV_tv =Zero(hHOM_ResV_tv);
  ProfHOM_ResU_tu =Zero(ProfHOM_ResU_tu);
  ProfHOM_ResV_tv =Zero(ProfHOM_ResV_tv);
  hHOM_Charge_diodedist_alg =Zero(hHOM_Charge_diodedist_alg); // JB 2010/03/19
  hHOM_Charge_diodedist_alg_u =Zero(hHOM_Charge_diodedist_alg_u); // clm 2013/01/23
  hHOM_Charge_diodedist_alg_v =Zero(hHOM_Charge_diodedist_alg_v); // clm 2013/01/23
  hHOM_Charge_diodedist =Zero(hHOM_Charge_diodedist);
  hHOM_Charge2_diodedist =Zero(hHOM_Charge2_diodedist);
  hHOM_Charge4_diodedist =Zero(hHOM_Charge4_diodedist);
  hHOM_Charge9_diodedist =Zero(hHOM_Charge9_diodedist);
  hHOM_Charge25_diodedist =Zero(hHOM_Charge25_diodedist);
  hHOM_Noise_diodedist  =Zero(hHOM_Noise_diodedist);

  // start CLM 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition = Zero(ProfhGOODCharge_Charge_DiodePosition);
  ProfhGOODCharge_Charge_DiodePositionSeedQLT300 = Zero(ProfhGOODCharge_Charge_DiodePositionSeedQLT300);
  ProfhGOODCharge_Charge_DiodePositionSeedQGT2000 = Zero(ProfhGOODCharge_Charge_DiodePositionSeedQGT2000);
  
  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow);
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow);
  
  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_seed = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_seed);
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_seed = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_seed);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_seed = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_seed);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_seed = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_seed);
  
  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_1stcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_1stcrown);
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_1stcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_1stcrown);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_1stcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_1stcrown);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_1stcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_1stcrown);
  
  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_2ndcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_2ndcrown);
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_2ndcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_2ndcrown);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_2ndcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_2ndcrown);
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_2ndcrown = Zero(ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_2ndcrown);
  
  hDistVSeedOtherOldCalc = Zero(hDistVSeedOtherOldCalc);
  hDistVSeedOtherNewCalc = Zero(hDistVSeedOtherNewCalc);  
  
  h2dCharge_Charge_DiodePosition_Track = Zero(h2dCharge_Charge_DiodePosition_Track);
  h2dCharge_Charge_DiodePosition_CluSize = Zero(h2dCharge_Charge_DiodePosition_CluSize);
  
  hNpixInClu = Zero(hNpixInClu);
  hQpixInClu = Zero(hQpixInClu);

  ProfhGOODCharge_Charge_DiodePositionSimpDist = Zero(ProfhGOODCharge_Charge_DiodePositionSimpDist);
  
   hHOM_Charge_diodedist3D = Zero( hHOM_Charge_diodedist3D );
   hHOM_Charge2_diodedist3D = Zero( hHOM_Charge2_diodedist3D );
   hHOM_Charge4_diodedist3D = Zero( hHOM_Charge4_diodedist3D );
   hHOM_Charge9_diodedist3D = Zero(  hHOM_Charge9_diodedist3D);
   hHOM_Charge25_diodedist3D = Zero( hHOM_Charge25_diodedist3D );
  // end CLM 2013/01/23

  ProfHOM_Charge_diodedist_alg = Zero(ProfHOM_Charge_diodedist_alg); // JB 2010/03/11
  ProfHOM_Charge_diodedist_alg_u = Zero(ProfHOM_Charge_diodedist_alg_u); // clm 2013/01/23
  ProfHOM_Charge_diodedist_alg_v = Zero(ProfHOM_Charge_diodedist_alg_v); // clm 2013/01/11
  ProfHOM_Charge_diodedist = Zero(ProfHOM_Charge_diodedist);
  ProfHOM_Charge2_diodedist = Zero(ProfHOM_Charge2_diodedist);
  ProfHOM_Charge4_diodedist = Zero(ProfHOM_Charge4_diodedist);
  ProfHOM_Charge9_diodedist = Zero(ProfHOM_Charge9_diodedist);
  ProfHOM_Charge25_diodedist = Zero(ProfHOM_Charge25_diodedist);

  hHOM_SNseed_diodedist = Zero(hHOM_SNseed_diodedist);
  ProfHOM_SNseed_diodedist = Zero(ProfHOM_SNseed_diodedist);
  hHOM_Charge_diodedist_00_10 = Zero(hHOM_Charge_diodedist_00_10);
  hHOM_Charge_diodedist_10_20 = Zero(hHOM_Charge_diodedist_10_20);
  hHOM_Charge_diodedist_20_30 = Zero(hHOM_Charge_diodedist_20_30);
  hHOM_Charge_diodedist_30_40 = Zero(hHOM_Charge_diodedist_30_40);
  hHOM_Charge_diodedist_40_50 = Zero(hHOM_Charge_diodedist_40_50);
  hHOM_Charge_diodedist_50_60 = Zero(hHOM_Charge_diodedist_50_60);
  hHOM_Charge_diodedist_60_70 = Zero(hHOM_Charge_diodedist_60_70);
  hHOM_Charge_diodedist_70_80 = Zero(hHOM_Charge_diodedist_70_80);
  hHOM_Charge_diodedist_80_90 = Zero(hHOM_Charge_diodedist_80_90);
  hHOM_Charge_diodedist_90_inf = Zero(hHOM_Charge_diodedist_90_inf);

  hHOM_modUCG_modtu    = Zero(hHOM_modUCG_modtu);
  hHOM_modVCG_modtv    = Zero(hHOM_modVCG_modtv); 
  hHOM_modUeta3_modtu  = Zero(hHOM_modUeta3_modtu);
  hHOM_modVeta3_modtv  = Zero(hHOM_modVeta3_modtv);
  hHOM_modUeta3_realtu = Zero(hHOM_modUeta3_realtu);
  hHOM_modVeta3_realtv = Zero(hHOM_modVeta3_realtv); 
  hHOM_modUCG_realtu  = Zero(hHOM_modUCG_realtu);
  hHOM_modVCG_realtv  = Zero(hHOM_modVCG_realtv);
  hHOM_modUeta3_Eta3U    = Zero(hHOM_modUeta3_Eta3U);
  hHOM_modVeta3_Eta3V    = Zero(hHOM_modVeta3_Eta3V);
  hHOM_modUeta3_modVeta3 = Zero(hHOM_modUeta3_modVeta3);
  hHOM_modUCG_modVCG  = Zero(hHOM_modUCG_modVCG );
  hHOM_modUeta3_modUCG= Zero(hHOM_modUeta3_modUCG);
  hHOM_modVeta3_modVCG= Zero(hHOM_modVeta3_modVCG);

  hHOM_DU_Nevent  =  Zero(hHOM_DU_Nevent);
  hHOM_DV_Nevent  =  Zero(hHOM_DV_Nevent);

  hHOM_modtu_Nevent = Zero(hHOM_modtu_Nevent);
  hHOM_modtv_Nevent = Zero(hHOM_modtv_Nevent);

  hHOM_modUCG_Nevent = Zero(hHOM_modUCG_Nevent);
  hHOM_modVCG_Nevent = Zero(hHOM_modVCG_Nevent);

  hHOM_ResEta25_U = Zero(hHOM_ResEta25_U);
  hHOM_ResEta25_V = Zero(hHOM_ResEta25_V);

  hEtaU_2x2Res = Zero(hEtaU_2x2Res);
  hEtaV_2x2Res = Zero(hEtaV_2x2Res);
  hEtaU_5x5Res = Zero(hEtaU_5x5Res);
  hEtaV_5x5Res = Zero(hEtaV_5x5Res);

  hHOM_modUeta2x2_modVeta2x2 = Zero(hHOM_modUeta2x2_modVeta2x2);
  hHOM_modUCG2x2_modVCG2x2 = Zero(hHOM_modUCG2x2_modVCG2x2);
  hHOM_modUeta5x5_modVeta5x5 = Zero(hHOM_modUeta5x5_modVeta5x5);
  hHOM_modUCG5x5_modVCG5x5 = Zero(hHOM_modUCG5x5_modVCG5x5);

  // Histos for cluster shape study, JB 2010/04/13				
  hClusterMeanForm = Zero(hClusterMeanForm);
  for( Int_t i=0; i<10; i++) {
    hCountPixels[i] = Zero(hCountPixels[i]);
    hPixelsOverSNR[i] = Zero(hPixelsOverSNR[i]);
    hPixelsOverCharge[i] = Zero(hPixelsOverCharge[i]);
    Cluster[i] = Zero(Cluster[i]);
    hMultVsFormFactor[i] = Zero(hMultVsFormFactor[i]);
    hMultVsFormFactor1D[i] = Zero(hMultVsFormFactor1D[i]); //clm 2013.08.25
  }
  hChargeCoG_Correl = Zero(hChargeCoG_Correl);
  hChargeCoG_Correl2 = Zero(hChargeCoG_Correl2);
  h_SNRratioL = Zero(h_SNRratioL);
  hClusterTest1 = Zero(hClusterTest1);
  hClusterTest2 = Zero(hClusterTest2);
  hClusterSizeInLines = Zero(hClusterSizeInLines);
  hClusterSizeInColumns = Zero(hClusterSizeInColumns);
  hChargeDistrInSeed = Zero(hChargeDistrInSeed);
  hChargeDistrInLine = Zero(hChargeDistrInLine);
  hChargeDistrIn3rdLeftNeigh = Zero( hChargeDistrIn3rdLeftNeigh);
  hChargeDistrIn2ndLeftNeigh = Zero( hChargeDistrIn2ndLeftNeigh);
  hChargeDistrIn1stLeftNeigh = Zero( hChargeDistrIn1stLeftNeigh);
  hChargeDistrIn3rdRightNeigh = Zero(hChargeDistrIn3rdRightNeigh);
  hChargeDistrIn2ndRightNeigh = Zero(hChargeDistrIn2ndRightNeigh);
  hChargeDistrIn1stRightNeigh = Zero(hChargeDistrIn1stRightNeigh);
  hClusterTypes = Zero(hClusterTypes);
  hClusterTypesBeyond4 = Zero(hClusterTypesBeyond4);
  
  // Histos not used
  /*
   hChargeOrder1=Zero(hChargeOrder1);
   hChargeOrder2=Zero(hChargeOrder2);
   hChargeOrder3=Zero(hChargeOrder3);
   hChargeOrder4=Zero(hChargeOrder4);
   hChargeOrder5=Zero(hChargeOrder5);
   hChargeOrder6=Zero(hChargeOrder6);
   hChargeOrder7=Zero(hChargeOrder7);
   hChargeOrder8=Zero(hChargeOrder8);
   hChargeOrder9=Zero(hChargeOrder9);
   
   hGOODChargeOrder1=Zero(hGOODChargeOrder1);
   hGOODChargeOrder2=Zero(hGOODChargeOrder2);
   hGOODChargeOrder3=Zero(hGOODChargeOrder3);
   hGOODChargeOrder4=Zero(hGOODChargeOrder4);
   hGOODChargeOrder5=Zero(hGOODChargeOrder5);
   hGOODChargeOrder6=Zero(hGOODChargeOrder6);
   hGOODChargeOrder7=Zero(hGOODChargeOrder7);
   hGOODChargeOrder8=Zero(hGOODChargeOrder8);
   hGOODChargeOrder9=Zero(hGOODChargeOrder9);
   hGOODChargeCor_1_2=Zero(hGOODChargeCor_1_2);
   hGOODChargeCor_1_3=Zero(hGOODChargeCor_1_3);
   hGOODChargeCor_1_4=Zero(hGOODChargeCor_1_4);
   hGOODChargeCor_2_3=Zero(hGOODChargeCor_2_3);
   hGOODChargeSum_4=Zero(hGOODChargeSum_4);
   hGOODChargeRap_1_over_2=Zero(hGOODChargeRap_1_over_2); 
   hGOODChargeRap_1_over_3=Zero(hGOODChargeRap_1_over_3);
   hGOODChargeRap_1_over_4=Zero(hGOODChargeRap_1_over_4);
   hGOODChargeRap_2_over_3=Zero(hGOODChargeRap_2_over_3);
   */

  effimap=Zero(effimap);
  goodtracks=Zero(goodtracks);
  TrkInMimo=Zero(TrkInMimo);
  
  
  MainCanvas=Zero(MainCanvas);
  cres=Zero(cres);
  c2=Zero(c2);
  casym=Zero(casym);
  ceffi=Zero(ceffi);
  ccomp=Zero(ccomp);
  ccomp2=Zero(ccomp2);
  ccomp3=Zero(ccomp3);
  c3=Zero(c3);
  c4=Zero(c4);
  c4_2=Zero(c4_2);
  c5=Zero(c5);
  c6=Zero(c6);
  c7=Zero(c7);
  for (int i =0; i<numcanvasSN; i++) {cSN[i]=Zero(cSN[i]);}
  for (int i =0; i<numcanvas; i++) {cRef[i]=Zero(cRef[i]);}
  for (int i =0; i<numcanvasOptimize; i++) {cOptimize[i]=Zero(cOptimize[i]);}
  cM8=Zero(cM8);
  cfake=Zero(cfake);

  PixHom=Zero(PixHom);
  PixHom2=Zero(PixHom2);
  PixHom3=Zero(PixHom3);
  PixHom4=Zero(PixHom4);
  PixHom5=Zero(PixHom5);
  PixHom6=Zero(PixHom6);

  greff=Zero(greff);
  grnum=Zero(grnum);
  grevt=Zero(grevt);

  //------------------------------------------------------------------------------ 
  // MiniVectors Histograms ; NCS , 2010/01/21
  //  modified JB 2011/11/01
  //------------------------------------------------------------------------------ 
  hDiffPosX=Zero(hDiffPosX);
  hDiffPosY=Zero(hDiffPosY);
  hDiffAngleX=Zero(hDiffAngleX);
  hDiffAngleY=Zero(hDiffAngleY);
  hDiffAngleX11=Zero(hDiffAngleX11);
  hDiffAngleY11=Zero(hDiffAngleY11);
  hDiffAngleX12=Zero(hDiffAngleX12);
  hDiffAngleY12=Zero(hDiffAngleY12);
  hDiffAngleX21=Zero(hDiffAngleX21);
  hDiffAngleY21=Zero(hDiffAngleY21);
  hDiffAngleX22=Zero(hDiffAngleX22);
  hDiffAngleY22=Zero(hDiffAngleY22);
  hDiffAngleXg1g1=Zero(hDiffAngleXg1g1);
  hDiffAngleYg1g1=Zero(hDiffAngleYg1g1);
  hxtxPL3=Zero(hxtxPL3);
  hytyPL3=Zero(hytyPL3);
  hxtxPL4=Zero(hxtxPL4);
  hytyPL4=Zero(hytyPL4);
  hutuPL3=Zero(hutuPL3);
  hvtvPL3=Zero(hvtvPL3);
  hutuPL4=Zero(hutuPL4);
  hvtvPL4=Zero(hvtvPL4);
  hdiffydiffx=Zero(hdiffydiffx);
  hdiffydiffx11=Zero(hdiffydiffx11);
  hdiffydiffx12=Zero(hdiffydiffx12);
  hdiffydiffx21=Zero(hdiffydiffx21);
  hdiffydiffx22=Zero(hdiffydiffx22);
  hdiffydiffxg1g1=Zero(hdiffydiffxg1g1);


  //------------------------------------------------------------------------------ 
  // MimosaVertexFinder histograms
  //------------------------------------------------------------------------------ 


  hVertexPosX = Zero(hVertexPosX); 
  hVertexPosY = Zero(hVertexPosY);
  hVertexPosZ = Zero(hVertexPosZ);
  hVertexTrackDistance = Zero(hVertexTrackDistance);
  hVertexPosXY = Zero(hVertexPosXY);
  hVertexTrackChi2 = Zero(hVertexTrackChi2);
  cVertexFinder = Zero(cVertexFinder);


  //------------------------------------------------------------------------------ 
  // User histograms
  //------------------------------------------------------------------------------ 

  cUser=Zero(cUser);
  hUserHitCorrelationLine=Zero(hUserHitCorrelationLine);
  hUserHitCorrelationCol=Zero(hUserHitCorrelationCol);

  saved->cd();
  dir->Close();
  if(dir) { delete dir; dir = 0; }
 
  if(bar2) {bar2->Hide(); delete bar2 ; bar2=0;}

}

