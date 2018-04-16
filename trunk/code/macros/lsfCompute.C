#define _FILE_OFFSET_BITS 64
#include "TMath.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TFile.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
#include <iostream>
#include "TTree.h"
#include "DEvent.h"

void lsfCompute(){

  // Macro created by Mario Bachaalany 2010
  // Modified by JB 2010/10/14 to fit withTAF Ttree

  Short_t debug = 3; // 0 is no message
  Int_t nEvents = 00000; // if 0, read all

  gStyle->SetOptFit(1);

  int dimension = 0;
  double derive = 0.;
  double a = 0.;
  double b = 0.;
  double mu = 0.;
  double sigma = 0.;



 TFile *f1 = TFile::Open("datDSF/run5035_01.root","read");

 TTree *t1 = (TTree*) f1->Get("T");

 DEvent *Evt = new DEvent() ;
 TBranch *branch = t1->GetBranch("fEvent");
 branch->SetAddress(&Evt);
 
 nEvents = (nEvents==0)?t1->GetEntries():TMath::Min(nEvents,(Int_t)t1->GetEntries());
 cout << "Entries in the tree: " << t1->GetEntries();
 cout << ", will read " << nEvents << endl;

 //========
 // Finding the rotation angle

 int sigma_min = 100000;
 long int nHitsSelected = 0;
 double thetamin = 23.00;
 double amin = cos(thetamin*TMath::DegToRad());
 double bmin = sin(thetamin*TMath::DegToRad());
 double cluster_x_proj;
 double cluster_y_proj;
 /*
 cout << "Finding the rotation angle minimizing the std deviation..." << endl;
 
 for ( int theta = -100 ; theta < +100 ; theta += 10){ // steps in degrees*10

   a = cos(theta/10.*TMath::DegToRad());
   b = sin(theta/10.*TMath::DegToRad());
   if(debug>1) cout << " testing  cos(theta) = " << a << ", sin(theta) = " << b << ", theta = " << theta/10. << " deg." << endl;

   mu = sigma = 0.;
   
   for ( int loop1=0 ; loop1 < nEvents ; loop1++){ // loop on Tree entries
     
     t1->GetEntry(loop1);
     TClonesArray *Hits   = Evt->GetAuthenticHits(); //hits (all planes)
     Int_t NbOfHits = Hits->GetLast()+1;

     if(debug>2) { if(loop1%1000==0) cout << "event " << loop1 << endl; }

     for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits
       DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);
       
       //-------------------
       // cut for hits
       //-------------------
       if(    ahit->Hpk==4
	   && ahit->Hq0/ahit->Hn0 > 1.
	   //&& ahit->HNNS > 10
	   && ahit->Hu > 0.
	 ) {

	 //cout << "   hit " << iHit;

	 nHitsSelected++;
	 cluster_x_proj = a*ahit->Hu + b*ahit->Hv;
	 cluster_y_proj = -b*ahit->Hu + a*ahit->Hv;
	 mu += cluster_y_proj;
	 sigma += cluster_y_proj * cluster_y_proj;
       }

     } //end loop on hits
     //cout << endl;
     
   } //end loop on tree entries
   
   mu /= nHitsSelected;
   sigma = sqrt( sigma/nHitsSelected - mu*mu );
   if(debug>1) cout << "   -> mu = " << mu << ", sigma = " << sigma << endl;
   
   if(sigma < sigma_min ){
     sigma_min = sigma;
     amin = a;
     bmin = b;
     thetamin = (double)theta/10.;
     if(debug>1) cout<<"New best rotation, (mu, sigma, cos(theta), sin(theta), theta): "<<mu<<"      "<<sigma_min<<"        "<<amin<<"      "<<bmin<<"    "<<thetamin<<endl;
   }
   mu = 0;
   sigma = 0;
 }
 */ 
 cout << " # hits selected " << nHitsSelected << endl;
 cout << " ==> cos(theta) = " << amin << ", sin(theta) = " << bmin << ", theta = " << thetamin << " deg." << endl;

 //========
 // Filling the histos with the rotated positions

 TH1F* rotation  = new TH1F("rotation","rotation",500, -5000,5000);
 TH2F* rotation2d  = new TH2F("rotation2d","rotation2d",2000, -5000, 5000, 2000,-5000,5000);
 TH1F* rotationbincontent = new TH1F("rotationbincontent","rotationbincontent",200,-100,100);

 cout << "Filling the histograms (2D and rotated projection)." << endl;
 
 float minX=1e6., maxX=-1.e6, minY=1.e6, maxY=-1.e6;
 nHitsSelected = 0;

 for ( int loop2 = 0 ; loop2 < nEvents ; loop2++){ // loop on tree entries

   t1->GetEntry(loop2);
   TClonesArray *Hits   = Evt->GetAuthenticHits(); //hits (all planes)
   Int_t NbOfHits = Hits->GetLast()+1;
   if(debug>2) { if( loop2%10000==0) cout << "event " << loop2 << endl; }

   for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits
     DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);

     cluster_x_proj = amin*ahit->Hu + bmin*ahit->Hv;
     cluster_y_proj = -bmin*ahit->Hu + amin*ahit->Hv;

     if( cluster_x_proj<minX ) minX = cluster_x_proj;
     if( cluster_x_proj>maxX ) maxX = cluster_x_proj;
     if( cluster_y_proj<minY ) minY = cluster_y_proj;
     if( cluster_y_proj>maxY ) maxY = cluster_y_proj;

     //-------------------
     // cut for hits
     //-------------------
     if(    ahit->Hpk==2
	 && ahit->Hq0/ahit->Hn0 > 2.
	 && ahit->Hn0 < 90.
	 && ahit->HSNneighbour >3
	 //&& ahit->HNNS > 10
	 //&& ahit->Hu < 0. 
	 //&& ahit->Hv < -3000.
	 //&& -8000 <= cluster_x_proj && cluster_x_proj <=  -500
	 && -2000 <= cluster_x_proj && cluster_x_proj <=  4000
	 &&  -500 <= cluster_y_proj && cluster_y_proj <=  500
       ) { // hit selection

       nHitsSelected++;
       rotation->Fill(ahit->Hv);
       rotation2d->Fill(cluster_x_proj,cluster_y_proj);

     } //end hit selection

   } //end loop on hits

 } // end loop on tree entries

 cout << " # hits selected " << nHitsSelected << endl;
 cout << " Range in x [ " << minX << ", " << maxX << "], in Y [ " << minY << ", " << maxY << "]";
 cout << " Hits selected in 2D: " << rotation2d->GetEntries() << ", in 1D projection: " << rotation->GetEntries() << endl;

 for( Int_t ibinx=1; ibinx<= rotation2d->GetNbinsX(); ibinx++){
   for( Int_t ibiny=1; ibiny<= rotation2d->GetNbinsY(); ibiny++){
     if (rotation2d->GetBinContent( ibinx, ibiny) != 0 ) rotationbincontent->Fill(rotation2d->GetBinContent( ibinx, ibiny));	
     //if( rotation2d->GetBinContent( ibinx, ibiny)>3) rotation2d->SetBinContent( ibinx, ibiny, 0);
   }
 }


 TH1D* rotation2dproj;
 //rotation2dproj->Sumw2();
 rotation2dproj = (TH1D*)rotation2d->ProjectionY("rotation2dproj", 1, rotation2d->GetNbinsX(),"");
 rotation2dproj->SetTitle("rotation2dproj");
 //rotation2dproj->Rebin(2,"",0);

 //========
 // Differentiating the projection

 
 TH1F* rotation2dprojLargeBin = rotation2dproj->Clone();
 rotation2dprojLargeBin->Rebin(8,"",0);
 TH1F* rotation2dprojderive = rotation2dprojLargeBin->Clone();
 rotation2dprojderive->SetName("rotation2dprojderive");
 rotation2dprojderive->SetTitle("rotation2dprojderive");

 for( Int_t ibinx=1; ibinx<=rotation2dprojLargeBin->GetNbinsX(); ibinx++){
   derive = (rotation2dprojLargeBin->GetBinContent(ibinx+1) - rotation2dprojLargeBin->GetBinContent(ibinx-1))/(rotation2dprojLargeBin->GetBinCenter(ibinx+1) - rotation2dprojLargeBin->GetBinCenter(ibinx-1));
   //derive = (rotation2dprojLargeBin->GetBinContent(ibinx) - rotation2dprojLargeBin->GetBinContent(ibinx-1))/(rotation2dprojLargeBin->GetBinCenter(ibinx) - rotation2dprojLargeBin->GetBinCenter(ibinx-1));
   rotation2dprojderive->SetBinContent( ibinx, -derive);
   rotation2dprojderive->SetBinError(ibinx, sqrt( rotation2dprojLargeBin->GetBinContent(ibinx+1) + rotation2dprojLargeBin->GetBinContent(ibinx-1) ) / (rotation2dprojLargeBin->GetBinCenter(ibinx+1) - rotation2dprojLargeBin->GetBinCenter(ibinx-1)) );
   //rotation2dprojderive->SetBinError(ibinx,sqrt(rotation2dprojLargeBin->GetBinContent(ibinx) + rotation2dprojLargeBin->GetBinContent(ibinx-1))/(rotation2dprojLargeBin->GetBinCenter(ibinx) - rotation2dprojLargeBin->GetBinCenter(ibinx-1)));
 }
 rotation2dprojderive->SetMinimum(0);
  
 // If step is from 0->1, use 1+erf and fill with +derivative
 // If step is from 1->0, use 1-erf and fill with -derivative
  TF1* fprofilegaus = new TF1( "fprofilegaus", "[0]*(1-TMath::Erf((x+[1])/sqrt(2)/[2]))/2.+[3]", -5000, 5000);
  fprofilegaus->SetLineWidth(1);
  fprofilegaus->SetLineColor(3);
  fprofilegaus->SetParameters( 200., -2000., 100., 20.);
  fprofilegaus->SetParNames( "norm", "#mu", "#sigma", "cst level");
  //fprofilegaus->FixParameter(3,0.);
  fprofilegaus->SetRange( 1700., 2500.);

  TF1* gauss = new TF1( "gauss", "gaus", 1800., 2400.);
  gauss->SetLineColor(2);


 //========
 // Display

  TCanvas* canvas = new TCanvas ("Cluster","Cluster spec",1000,800);
  canvas->Divide (2,3);

  canvas->cd(1);
  rotation->Draw();

  canvas->cd(2);
  rotation2d->Draw();

  canvas->cd(3);
  rotation2dproj->Draw("e");
  rotation2dproj->Fit("fprofilegaus","R");
  rotation2dproj->Draw("esame");

  canvas->cd(4);
  rotation2dprojderive->Draw();
  rotation2dprojderive->Fit("gauss","R");

  canvas->cd(5);
  rotationbincontent->Draw();
  //gPad->SetLogy(1);


 //========
 // Printout
 cout << "Rotation angle " << thetamin << "deg" << endl;
 cout << "Bin width = " << rotation2dproj->GetBinWidth(1) << " um" << endl;
 printf( "LSF std. dev. from derivative = %.2f +/- %.2f\n", gauss->GetParameter(2), gauss->GetParError(2) );
 printf( "LSF std. dev. from erf = %.2f +/- %.2f\n", fprofilegaus->GetParameter(2), fprofilegaus->GetParError(2));

}
