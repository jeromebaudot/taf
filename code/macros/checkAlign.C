//
// This macro check the aligment of the planes in the tracker.
// It plots the track-hit residual distribution of each plane.
// It reads the TTree contained in the DSF File. 
//
// Inputs are specified in the "Parameters" section.
//
// Valérian REITHINGER
// valerian.reithinger@ipnl.in2p3.fr
// version 2013.05.28

void checkAlign(void)
{
  /////////////////////////////////////////////////////
  //	Parameters
  /////////////////////////////////////////////////////
	TFile *f=new TFile("DSF.root");

  //Int_t Xmax = 10030  ;
	Int_t Xmax =	11000;
	Int_t Xmin = -1000;
	Int_t XbinNb = 100;
	
	Int_t nbPlanes = 8;
	Bool_t SaveInPdf = 1;
	Bool_t SaveInRoot = 1;
  /////////////////////////////////////////////////////
  //	Cuts
  /////////////////////////////////////////////////////



  /////////////////////////////////////////////////////
  //	Graphical Objects Declartation & Creation
  /////////////////////////////////////////////////////
    TCanvas *canvasU;
    TCanvas *canvasV;
    TH1F    **histosU;
		TH1F    **histosV;
  	gStyle->SetOptStat("nemroui");
  

  /////////////////////////////////////////////////////
  //	Initialisation
  /////////////////////////////////////////////////////
    // Names
    Char_t nameFileTreeVertices[200];
    Char_t name[100];
    Char_t title[100];
    Char_t drawCommand[100];
    Char_t cutString[100];
    Char_t cutName[100];
    Char_t filename[100];


  
  if(1)
  {
    // Canvas
    sprintf(name,"canvasDistancesUTracks2Hit");
    sprintf(title,"Tracks-Hits distances along U");
    canvasU = new TCanvas(name,title);
		canvasU->Divide((Int_t)ceil(nbPlanes/2.), (nbPlanes>1)?2:1);
		
		histosU = new TH1F*[nbPlanes];
		for (int i=1; i<=nbPlanes; i++)
		{
			canvasU->cd(i);
		  // Histo
		  sprintf(name,"histoDistancesTracks2Hit_U_plane%d",i);
		  sprintf(title,"plane %d",i);
		  histosU[i-1]  = new TH1F(name,title,XbinNb,Xmin,Xmax);
		  histosU[i-1]-> SetXTitle("U PosHit-PosTck [um]");
		  histosU[i-1]-> SetYTitle("counts []");
		  histosU[i-1]-> SetLineColor(1);
		  // Fill
		  sprintf(drawCommand,"Tud>>%s",name);
		  sprintf(cutString,"Tpk==%d",i);//&& TtHn==4
		  cout << drawCommand << " " << cutString << endl;
		  T->Draw(drawCommand,cutString);		  
		}
		if (SaveInPdf)  canvasU->Print("canvasDistancesUTracks2Hit.pdf");
		if (SaveInRoot) canvasU->Print("canvasDistancesUTracks2Hit.root");
		
		// Canvas
    sprintf(name,"canvasDistancesVTracks2Hit");
    sprintf(title,"Tracks-Hits distances along V");
    canvasV = new TCanvas(name,title);
		canvasV->Divide((Int_t)ceil(nbPlanes/2.), (nbPlanes>1)?2:1);
		
		histosV = new TH1F*[nbPlanes];
		for (int i=1; i<=nbPlanes; i++)
		{
			canvasV->cd(i);
			sprintf(name,"histoDistancesTracks2Hit_V_plane%d",i);
		  sprintf(title,"plane %d",i);
		  histosV[i-1]  = new TH1F(name,title,XbinNb,Xmin,Xmax);
		  histosV[i-1]-> SetXTitle("PosHit-PosTck [um]");
		  histosV[i-1]-> SetYTitle("counts []");
		  histosV[i-1]-> SetLineColor(2);
		  // Fill
		  sprintf(drawCommand,"Tvd>>%s",name);
		  sprintf(cutString,"Tpk==%d",i);//&& TtHn==4
		  cout << drawCommand << " " << cutString << endl;
		  T->Draw(drawCommand,cutString,"same");
		}
		if (SaveInPdf)  canvasV->Print("canvasDistancesVTracks2Hit.pdf");
		if (SaveInRoot) canvasV->Print("canvasDistancesVTracks2Hit.root");
	} 
}


/*
  Short_t        Tpk;             // number of the plane ( Track Plane Number)
  Short_t        TtN;             // number of tracks in this plane
  Short_t        Ttk;             // number of the track normally 1
  Short_t        TtHn;            // number of hits in the track, JB 2009/09/08
 
  Float_t        Tu;              // track position perpendicular to strip, u-direction
  Float_t        Tv;              // track position along strip, v-direction

  Float_t        TDOX ;           //Incertitude sur la position de l origine de la trace
  Float_t        TDOY ;           //Incertitude sur la position de l origine de la trace

  Float_t        Tud;             // (principal)Hu - Tu
  Float_t        Tvd;             // (principal)Hv - Tv
  Short_t        ThN;             // number of hits found in the plane 
  Short_t        Thk;             // // authentic hit number in the plane 
  Int_t          TsN;             // number of strips 
  Float_t        Tpt;             // transparent plane threshold on single strip [ADC]
  Short_t        TotN;            // number of strips in neighbourhood to the track over thresh.
  Float_t        TotQ;            // transparent charge sum on strips 
                                  // in neighbourhood of the track over threshold
  Float_t        Tqc;             // Principal hit cluster pulse sum
  Float_t        Tq0;             // Principal hit seed pulseheight
  Float_t        Tn0;             // Principal hit seed noise

  Float_t        Tq1;             // transparent charge on nearest track, 
  Float_t        Tq2;             // transparent charge on 2nd next nearest
  Float_t        Tq3;             // and so on
  Float_t        Tq4;
  Float_t        Tq5;
  Float_t        Tq6;
  Float_t        Tq7;
  Float_t        Tq8;
  Float_t        Tq9;

  Float_t        Tu1;              // absolute u postion [micron ] of strip next to track
  Short_t        Tk1;              // index of strip next to track 

  Float_t        Tchi2;            // ChiSquare/ndf of the track fit, 2 dim
  Float_t        Tchi2u;           // ChiSquare/ndf of the track fit, U dim
  Float_t        Tchi2v;           // ChiSquare/ndf of the track fit, V dim

  // here one would prefer Tdu, Tdv
  Float_t        Tx,  Ty,  Tz;   // track coordinates
  Float_t        Tdx, Tdy;       // track directions
  Float_t        Tdu, Tdv;       // the slope in plane coordinates
  
  */
  
  
