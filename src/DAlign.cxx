// @(#)maf/dtools:$Name:  $:$Id: DAlign.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author: Dirk Meier   97/01/20 
// Last modified: JB 2008/10/24
// Last modified: JB 2009/05/13
// Last modified: JB 2009/05/25
// Last Modified: RDM 010809
// Last Modified: JB 2009/09/08 StoreU,V,2D
// Last Modified: JB 2010/12/14 AccumulateU/V/2D, ShowStoredResolution
// Last Modified: JB 2011/04/05 Modified
// Last Modified: SS 2011/09/15 Introduced more monitoring plots UU & VV
// Last Modified: JB 2013/06/10 different bounds in U and V introduced
// Last Modified: JB 2013/06/21 new 2D plots DV vs DU
// Last Modified: JB 2013/08/22 Display()
// Last Modified: BB 2014/04/08 new plots : profile DU VS U, DU VS V, DV VS U, DV VS V and 2D DUUV DVUV
//
  ////////////////////////////////////////////////////////////
  //                                                        //
  // Class  DAlign                                          //
  //                                                        //
  // Used to determine the U, V shifts and W rotation
  //  of a plane, from an chi2 fit.
  // The analytical solution of the chi2 fit is implemented.
  // But note that two independant fits are executed in 
  //  the two directions (U and V).
  //
  // These methods are used by the AlignTracker method 
  //  of the MAlign class.
  // An object DAlign is assigned to each DPlane object.
  //                                                        //
  ////////////////////////////////////////////////////////////


//*-- Modified :  IG
//*-- Author   :  Dirk Meier   97/01/20

#include "Riostream.h"
#include <math.h>

#include "DAlign.h"
#include "DHit.h"
#include "DTrack.h"
#include "DR3.h"
#include "DGlobalTools.h"

ClassImp(DAlign) // Description of DAlign 

//______________________________________________________________________________
//  
DAlign::DAlign()
{
  SetDebug( 0 );
  fPlaneNumber = 0;
}

//______________________________________________________________________________
//  
DAlign::DAlign( Int_t aPlaneNumber, Int_t aDebugLevel)
{

  // Default DAcqModule ctor.

  SetDebug( aDebugLevel );
  //if( aPlaneNumber==3) fDebugAlign=1;
  if(fDebugAlign) cout << endl << " -*-*- DAlign User Constructor -*-*- " << endl;

  fPlaneNumber      = aPlaneNumber;
  fEnoughU          = kFALSE;
  fEnoughV          = kFALSE;
  fEnough2D         = kFALSE;
  fAlignResolutionU = 0.0;
  fAlignMeanU       = 0.0;
  fAlignResolutionV = 0.0;
  fAlignMeanV       = 0.0;
  fAlignSum_du      = 0.0;
  fAlignSum_dudu    = 0.0;
  fAlignSum_duvt    = 0.0;
  fAlignSum_vt      = 0.0;
  fAlignSum_vtvt    = 0.0;
  fAlignCountu      = 0;
  fAlignSum_dv      = 0.0;
  fAlignSum_dvdv    = 0.0;
  fAlignSum_dvut    = 0.0;
  fAlignSum_ut      = 0.0;
  fAlignSum_utut    = 0.0;
  fAlignCountv      = 0;
  fAlignSum_uvt2    = 0.;
  fAlignSum_dvuduv  = 0.0;
  fAlignCount2D     = 0;
  fAlignTiltW       = 0.0;
  fAlignOffsetU     = 0.0;
  fAlignOffsetV     = 0.0; // JB
  fAlignTiltWError  = 0.0;
  fAlignOffsetUError= 0.0;
  fAlignOffsetVError= 0.0; // JB
  fAlignCorrelation = 0.0;
  fNumberOfEvents   = 300; // preliminary number (100->400 YG), JB 2008/10/20
  fBound            = 5000.; // preliminary max distance track-hit
  fBoundU           = 5000.; // preliminary max distance track-hit, JB 2013/06/10
  fBoundV           = 5000.; // preliminary max distance track-hit, JB 2013/06/10

  fAlignCanvas = NULL;
  fpfxAlignCanvas = NULL;
  fAlign3DCanvas = NULL;
  fhAlignDU = NULL;
  fhAlignDV = NULL;
  fhAlignDVDU = NULL;
  fhAlignPosUV = NULL;
  fhAlignPosVU = NULL;
  fhAlignPosUU = NULL;
  fhAlignPosVV = NULL;
  fhAlignUU = NULL;
  fhAlignVV = NULL;
  fhAlignPosUUV = NULL;
  fhAlignPosVUV = NULL;
  fpfxAlignPosUV = NULL;
  fpfxAlignPosVU = NULL;
  fpfxAlignPosUU = NULL;
  fpfxAlignPosVV = NULL;

  if(fDebugAlign) {
    cout << "  Plane " << fPlaneNumber;
    cout << "  Miminal number of events to accumulate " << fNumberOfEvents << endl;
    cout << "  Maxinal distance between track and hit " << fBound << ", in U " << fBoundU << ", in V " << fBoundV << endl;
  }

  if(fDebugAlign) cout << endl << " -*-*- DAlign User Constructor DONE -*-*- " << endl;

}

//______________________________________________________________________________
//  
DAlign::~DAlign()
{ 
  // Default DAlign destructor.

  if (fAlignCanvas) {delete fAlignCanvas; fAlignCanvas=0;}
  if (fpfxAlignCanvas) {delete fpfxAlignCanvas; fpfxAlignCanvas=0;} // BB 060314
  if (fhAlignPosUV) {delete fhAlignPosUV; fhAlignPosUV=0;}
  if (fpfxAlignPosUV) {delete fpfxAlignPosUV; fpfxAlignPosUV=0;} // BB 060314
  if (fpfxAlignPosVU) {delete fpfxAlignPosVU; fpfxAlignPosVU=0;} // BB 060314
  if (fpfxAlignPosUU) {delete fpfxAlignPosUU; fpfxAlignPosUU=0;} // BB 060314
  if (fpfxAlignPosVV) {delete fpfxAlignPosVV; fpfxAlignPosVV=0;} // BB 060314
  if (fhAlignPosUUV)  {delete fhAlignPosUUV; fhAlignPosUUV=0;}   // BB 070414
  if (fhAlignPosVUV)  {delete fhAlignPosVUV; fhAlignPosVUV=0;}   // BB 070414
  if (fhAlignPosVU) {delete fhAlignPosVU; fhAlignPosVU=0;}
  if (fhAlignPosUU) {delete fhAlignPosUU; fhAlignPosUU=0;}
  if (fhAlignPosVV) {delete fhAlignPosVV; fhAlignPosVV=0;}
  if (fhAlignDU) {delete fhAlignDU; fhAlignDU=0;}
  if (fhAlignDV) {delete fhAlignDV; fhAlignDV=0;}
  if (fhAlignDVDU) {delete fhAlignDVDU; fhAlignDVDU=0;}
  if (fhAlignUU) {delete fhAlignUU; fhAlignUU=0;} // JB 2014/08/26
  if (fhAlignVV) {delete fhAlignVV; fhAlignVV=0;}
}

//______________________________________________________________________________
//  
void DAlign::SetBounding(Double_t aBound)  
{ 

  // Change boundings in U and V to accept a track to hit distance
  // to the same value
  // and update histograms axis
  //
  // JB, 2008/10/14
  // Updated, JB 2009/05/14
  // Updated, JB 2013/06/10 to use SetBoundings

  SetBoundings( aBound, aBound);

}

//______________________________________________________________________________
//  
void DAlign::SetBoundings(Double_t aBoundU, Double_t aBoundV)  
{ 
  
  // Change boundings in U and V to accept a track to hit distance
  // and update histograms axis
  //
  // JB, 2013/06/10 from a copy of the old SetBounding method.
  // Modified JB 2013/08/31 update of fBound as well
  
  fBoundU = aBoundU; 
  fBoundV = aBoundV; 
  fBound = sqrt( fBoundU*fBoundU+fBoundV*fBoundV); // JB 2013/08/31
  cout <<"Bound set to "<<fBoundU<<" in U and to "<<fBoundV<<" in V and "<<fBound<<" in 2D, for plane "<<fPlaneNumber<<endl;
  
  if (fhAlignPosUV) { fhAlignPosUV->SetAxisRange( -fBoundU, fBoundU, "Y"); }
  if (fpfxAlignPosUV) { fpfxAlignPosUV->SetAxisRange( -fBoundU, fBoundU, "Y"); } // BB 06/03/14
  if (fpfxAlignPosVU) { fpfxAlignPosVU->SetAxisRange( -fBoundV, fBoundV, "Y"); } // BB 06/03/14
  if (fpfxAlignPosUU) { fpfxAlignPosUU->SetAxisRange( -fBoundU, fBoundU, "Y"); } // BB 06/03/14
  if (fpfxAlignPosVV) { fpfxAlignPosVV->SetAxisRange( -fBoundV, fBoundV, "Y"); } // BB 06/03/14
  if (fhAlignPosUUV) { fhAlignPosUUV->SetAxisRange( -fBoundU, fBoundU, "Z"); }   // BB 08/04/14
  if (fhAlignPosVUV) { fhAlignPosVUV->SetAxisRange( -fBoundV, fBoundV, "Z"); }   // BB 08/04/14
  if (fhAlignPosVU) { fhAlignPosVU->SetAxisRange( -fBoundV, fBoundV, "Y"); }
  if (fhAlignPosUU) { fhAlignPosUU->SetAxisRange( -fBoundU, fBoundU, "Y"); }
  if (fhAlignPosVV) { fhAlignPosVV->SetAxisRange( -fBoundV, fBoundV, "Y"); }
  if (fhAlignDU) { fhAlignDU->SetAxisRange( -fBoundU, fBoundU, "X"); }
  if (fhAlignDV) { fhAlignDV->SetAxisRange( -fBoundV, fBoundV, "X"); }
  if (fhAlignDVDU) { 
    fhAlignDVDU->SetAxisRange( -fBoundU, fBoundU, "X"); 
    fhAlignDVDU->SetAxisRange( -fBoundV, fBoundV, "Y"); 
  }
}

//______________________________________________________________________________
//  
void DAlign::SetGeoLimits(Double_t umin, Double_t umax, Double_t vmin, Double_t vmax)
{
  // Define geometrical limits of hits to be considered for alignment.
  // Those limits are given in the plane frame.
  //
  // JB 2013/06/10

  fLimitsU[0] = umin;
  fLimitsU[1] = umax;
  fLimitsV[0] = vmin;
  fLimitsV[1] = vmax;
  
}

//______________________________________________________________________________
//  
void DAlign::CreateDisplay( Double_t Umin, Double_t Umax, Double_t Vmin, Double_t Vmax)
{
  // Create the canvas and histos to monitor alignement
  // JB, 2008/10/24
  // Modified: RDM 010809
  // Modified BB 050314 to study tilt
  // Modified JB 2014/08/26 to add simple correlation plots

  Char_t name[50], title[100];
  sprintf( name, "cAlignP%d", fPlaneNumber);
  sprintf( title, "Align monitor plane %d", fPlaneNumber);
  fAlignCanvas = new TCanvas( name, title, fPlaneNumber*50, fPlaneNumber*50, 750, 600);
  fAlignCanvas->Divide(2,4);

  sprintf( name, "hAlignPosUV%d", fPlaneNumber);
  sprintf( title, "align monitor - #Delta U vs V - Plane %d", fPlaneNumber);
  fhAlignPosUV = new TH2F( name, title, 100, Vmin, Vmax, 100,-fBoundU, fBoundU);  //RDM010809
  fhAlignPosUV->SetXTitle("V (#mum)");
  fhAlignPosUV->SetYTitle("#Delta U (#mum)");

  sprintf( name, "hAlignDU%d", fPlaneNumber);
  sprintf( title, "align monitor - #Delta U - Plane %d", fPlaneNumber);
  fhAlignDU = new TH1F( name, title, 100, -fBoundU, fBoundU);
  fhAlignDU->SetXTitle("#Delta U (#mum)");

  sprintf( name, "hAlignPosVU%d", fPlaneNumber);
  sprintf( title, "align monitor - #Delta V vs U - Plane %d", fPlaneNumber);
  fhAlignPosVU = new TH2F( name, title, 100, Umin, Umax, 100,-fBoundV, fBoundV); //RDM010809
  fhAlignPosVU->SetXTitle("U (#mum)");
  fhAlignPosVU->SetYTitle("#Delta V (#mum)");

  sprintf( name, "hAlignDV%d", fPlaneNumber);
  sprintf( title, "align monitor - #Delta V - Plane %d", fPlaneNumber);
  fhAlignDV = new TH1F( name, title, 100, -fBoundV, fBoundV);
  fhAlignDV->SetXTitle("#Delta V (#mum)");
  
  sprintf( name, "hAlignDVDU%d", fPlaneNumber);
  sprintf( title, "align monitor - #Delta V vs #Delta U - Plane %d", fPlaneNumber);
  fhAlignDVDU = new TH2F( name, title, 100, -fBoundU, fBoundU, 100, -fBoundV, fBoundV);
  fhAlignDVDU->SetXTitle("#Delta U (#mum)");
  fhAlignDVDU->SetYTitle("#Delta V (#mum)");
  
  sprintf( name, "hAlignPosUU%d", fPlaneNumber);
  sprintf( title, "align monitor - #Delta U vs U - Plane %d", fPlaneNumber);
  fhAlignPosUU = new TH2F( name, title, 100, Umin, Umax, 100,-fBoundU, fBoundU);  //RDM010809
  fhAlignPosUU->SetXTitle("U (#mum)");
  fhAlignPosUU->SetYTitle("#Delta U (#mum)");

  sprintf( name, "hAlignPosVV%d", fPlaneNumber);
  sprintf( title, "align monitor - #Delta V vs V - Plane %d", fPlaneNumber);
  fhAlignPosVV = new TH2F( name, title, 100, Vmin, Vmax, 100,-fBoundV, fBoundV); //RDM010809
  fhAlignPosVV->SetXTitle("V (#mum)");
  fhAlignPosVV->SetYTitle("#Delta V (#mum)");
  
  sprintf( name, "hAlignUU%d", fPlaneNumber);
  sprintf( title, "align monitor - Uhit vs Utrack - Plane %d", fPlaneNumber);
  fhAlignUU = new TH2F( name, title, 100, Umin, Umax, 100, Umin, Umax); //JB 2014/08/26
  fhAlignUU->SetXTitle("U track (#mum)");
  fhAlignUU->SetYTitle("U hit (#mum)");
  
  sprintf( name, "hAlignVV%d", fPlaneNumber);
  sprintf( title, "align monitor - Vhit vs Vtrack - Plane %d", fPlaneNumber);
  fhAlignVV = new TH2F( name, title, 100, Vmin, Vmax, 100, Vmin, Vmax); //JB 2014/08/26
  fhAlignVV->SetXTitle("V track (#mum)");
  fhAlignVV->SetYTitle("V hit (#mum)");
}

//______________________________________________________________________________
//  
void DAlign::CreateDisplayDeformation( Double_t Umin, Double_t Umax, Double_t Vmin, Double_t Vmax){

  Char_t name[50], title[100];

  sprintf( name, "cPfxAlignP%d", fPlaneNumber);                                                 // BB 060314 
  sprintf( title, "Profile of align monit plane %d", fPlaneNumber);
  fpfxAlignCanvas = new TCanvas( name, title, fPlaneNumber*50, fPlaneNumber*50, 750, 450);
  fpfxAlignCanvas->Divide(2,2);

  sprintf( name, "cAlign3DP%d", fPlaneNumber);                                                  // BB 070414
  sprintf( title, "3D Align plane %d", fPlaneNumber);
  fAlign3DCanvas = new TCanvas( name, title, fPlaneNumber*50, fPlaneNumber*50, 750, 450);
  fAlign3DCanvas->Divide (1,2);

  sprintf( name, "pfxAlignPosUV%d", fPlaneNumber);                                              // BB 060314
  sprintf( title, "profile of align monitor - U dim vs V - Plane %d", fPlaneNumber);            // BB 060314
  fpfxAlignPosUV = new TProfile( name, title, 100, Vmin, Vmax);                                 // BB 060314   
  fpfxAlignPosUV->SetXTitle("V (#mum)");                                                        // BB 060314
  fpfxAlignPosUV->SetYTitle("#Delta U (#mum)");                                                 // BB 060314

  sprintf( name, "pfxAlignPosVU%d", fPlaneNumber);                                              // BB 060314
  sprintf( title, "profile of align monitor - V dim vs U - Plane %d", fPlaneNumber);            // BB 060314
  fpfxAlignPosVU = new TProfile( name, title, 100, Umin, Umax);                                 // BB 060314   
  fpfxAlignPosVU->SetXTitle("U (#mum)");                                                        // BB 060314
  fpfxAlignPosVU->SetYTitle("#Delta V (#mum)");                                                 // BB 060314

  sprintf( name, "pfxAlignPosUU%d", fPlaneNumber);                                              // BB 060314
  sprintf( title, "profile of align monitor - U dim vs U - Plane %d", fPlaneNumber);            // BB 060314
  fpfxAlignPosUU = new TProfile( name, title, 100, Umin, Umax);                                 // BB 060314   
  fpfxAlignPosUU->SetXTitle("U (#mum)");                                                        // BB 060314
  fpfxAlignPosUU->SetYTitle("#Delta U (#mum)");                                                 // BB 060314

  sprintf( name, "pfxAlignPosVV%d", fPlaneNumber);                                              // BB 060314
  sprintf( title, "profile of align monitor - V dim vs V - Plane %d", fPlaneNumber);            // BB 060314
  fpfxAlignPosVV = new TProfile( name, title, 100, Vmin, Vmax);                                 // BB 060314   
  fpfxAlignPosVV->SetXTitle("V (#mum)");                                                        // BB 060314
  fpfxAlignPosVV->SetYTitle("#Delta V (#mum)");                                                 // BB 060314

  sprintf( name, "hAlignPosUUV%d",fPlaneNumber);                                                // BB 070414
  sprintf( title, "align monitor - U dim vs U and V - Plane %d", fPlaneNumber);                 // BB 070414
  fhAlignPosUUV = new TH2F( name, title, 10, Umin, Umax, 10, Vmin, Vmax);                     // BB 070414
  fhAlignPosUUV->SetXTitle("U (#mum)");                                                         // BB 070414
  fhAlignPosUUV->SetYTitle("V (#mum)");                                                         // BB 070414
  fhAlignPosUUV->SetZTitle("#Delta U (#mum)");                                                  // BB 080414
  
  sprintf( name, "hAlignPosVUV%d",fPlaneNumber);                                                // BB 070414
  sprintf( title, "align monitor - V dim vs U and V - Plane %d", fPlaneNumber);                 // BB 070414
  fhAlignPosVUV = new TH2F( name, title, 10, Umin, Umax, 10, Vmin, Vmax);                     // BB 070414
  fhAlignPosVUV->SetXTitle("U (#mum)");                                                         // BB 070414
  fhAlignPosVUV->SetYTitle("V (#mum)");                                                         // BB 070414
  fhAlignPosVUV->SetZTitle("#Delta V (#mum)");                                                  // BB 080414

}

//______________________________________________________________________________
//  
void DAlign::StoreU(DR3& aTrackPos, Double_t aDistanceU){

  // Simply store distances track-hit without trying to minimize
  // only for later Display() 
  //
  // JB, 2009/09/08
  
  if( fDebugAlign>1) printf("   DAlign: plane %d Store in U with distance %f <?> %.1f\n", fPlaneNumber, aDistanceU, fBoundU);

  // Accumulation of hits
  if ( fabs(aDistanceU)<=fBoundU && fhAlignPosUV && fhAlignPosUU && fhAlignUU && fhAlignDU) { 
    fAlignCountu++;
    fhAlignPosUV->Fill( aTrackPos(1), aDistanceU);
    fhAlignPosUU->Fill( aTrackPos(0), aDistanceU);
    fhAlignUU->Fill( aTrackPos(0), aDistanceU+aTrackPos(0)); // JB 2014/08/26
    fhAlignDU->Fill( aDistanceU);
//       if( fAlignCountu%50==0 ) { 
// 	fAlignCanvas->cd(1); 
// 	fhAlignPosU->Draw(); 
// 	fAlignCanvas->Update(); 
//       }
  }
  
  // Accumulation of hits for profile of residual as a function of the position of the hit on the plane
  if ( fabs(aDistanceU)<=fBoundU && fpfxAlignPosUV && fpfxAlignPosUU && fhAlignPosUUV){ // BB 2014/05/20 

    fAlignCountu++;
    fpfxAlignPosUV->Fill( aTrackPos(1), aDistanceU);    // BB 060314
    fpfxAlignPosUU->Fill( aTrackPos(0), aDistanceU);    // BB 060314
    fhAlignPosUUV->Fill(aTrackPos(0),aTrackPos(1), aDistanceU); //BB 070414
  }

}

//______________________________________________________________________________
//  
void DAlign::StoreV(DR3& aTrackPos, Double_t aDistanceV){

  // Simply store distances track-hit without trying to minimize
  // only for later Display() 
  //
  // JB, 2009/09/08
  
  if( fDebugAlign>1) printf("   DAlign: plane %d Store in V with distance %f <?> %.1f\n", fPlaneNumber, aDistanceV, fBoundV);

  // Accumulation of hits
  if ( fabs(aDistanceV)<=fBoundV && fhAlignPosVU && fhAlignPosVV && fhAlignVV && fhAlignDV) {
    fAlignCountv++;
    fhAlignPosVU->Fill( aTrackPos(0), aDistanceV);
    fhAlignPosVV->Fill( aTrackPos(1), aDistanceV);
    fhAlignVV->Fill( aTrackPos(1), aDistanceV+aTrackPos(1)); // JB 2014/08/26
    fhAlignDV->Fill( aDistanceV);
//       if( fAlignCountv%50==0 ) { 
// 	fAlignCanvas->cd(2); 
// 	fhAlignPosV->Draw(); 
// 	fAlignCanvas->Update(); 
//       }
  }
  
  // Accumulation of hits for profile of residual as a function of the position of the hit on the plane
  if ( fabs(aDistanceV)<=fBoundV && fpfxAlignPosVU && fpfxAlignPosVV && fhAlignPosVUV){ // BB 2014/05/20

    fAlignCountv++;
    fpfxAlignPosVU->Fill( aTrackPos(0), aDistanceV);    // BB 060314
    fpfxAlignPosVV->Fill( aTrackPos(1), aDistanceV);    // BB 060314
    fhAlignPosVUV->Fill(aTrackPos(0),aTrackPos(1), aDistanceV); //BB 070414
  }

}

//______________________________________________________________________________
//  
void DAlign::Store2D(DR3& aTrackPos, Double_t aDistanceU, Double_t aDistanceV){

  // Simply store distances track-hit without trying to minimize
  // only for later Display() 
  //
  // JB, 2009/05/13
  // Modified: JB 2009/09/08 to count stored events
  // Modified: BB 2014/03/06 for new histos
  
  if( fDebugAlign>1) printf("   DAlign: plane %d Store in U and V with distance (%f, %f) <?> (%.1f, %.1f)\n", fPlaneNumber, aDistanceU, aDistanceV, fBoundU, fBoundV);

  // Accumulation of hits
  if ( fabs(aDistanceU)<=fBoundU && fhAlignPosUV
       && fabs(aDistanceV)<=fBoundV && fhAlignPosVU && fhAlignPosUU && fhAlignPosVV && fhAlignDVDU && fhAlignDU && fhAlignDV) { // BB 2015/07/28 
    fAlignCount2D++; // JB 2009/08/09
    fhAlignPosUV->Fill( aTrackPos(1), aDistanceU);
    fhAlignDU->Fill( aDistanceU);
    fhAlignPosVU->Fill( aTrackPos(0), aDistanceV);
    fhAlignDV->Fill( aDistanceV);
    fhAlignPosUU->Fill( aTrackPos(0), aDistanceU);
    fhAlignUU->Fill( aTrackPos(0), aDistanceV+aTrackPos(0)); // JB 2014/08/26
    fhAlignPosVV->Fill( aTrackPos(1), aDistanceV);
    fhAlignVV->Fill( aTrackPos(1), aDistanceV+aTrackPos(1)); // JB 2014/08/26
    fhAlignDVDU->Fill( aDistanceU, aDistanceV);
    
//       if( fAlignCount2D%50==0 ) { 
// 	fAlignCanvas->cd(1); 
// 	fhAlignPosU->Draw(); 
// 	fAlignCanvas->cd(2); 
// 	fhAlignPosV->Draw(); 
// 	fAlignCanvas->Update(); 
//       }
  }
  
  // Accumulation of hits for profile of residual as a function of hit position on the plane
 
  if ( fabs(aDistanceU)<=fBoundU && fpfxAlignPosUV && fabs(aDistanceV)<=fBoundV && fpfxAlignPosVU && fpfxAlignPosUU && fpfxAlignPosVV && fhAlignPosUUV && fhAlignPosVUV){ // BB 2014/05/20

    fAlignCount2D++;
    fpfxAlignPosUV->Fill( aTrackPos(1), aDistanceU);            // BB 06/03/14
    fpfxAlignPosVU->Fill( aTrackPos(0), aDistanceV);            // BB 06/03/14
    fpfxAlignPosUU->Fill( aTrackPos(0), aDistanceU);            // BB 06/03/14
    fpfxAlignPosVV->Fill( aTrackPos(1), aDistanceV);            // BB 06/03/14
    fhAlignPosUUV->Fill(aTrackPos(0),aTrackPos(1), aDistanceU); // BB 07/04/14
    fhAlignPosVUV->Fill(aTrackPos(0),aTrackPos(1), aDistanceV); // BB 07/04/14
  }

}

//______________________________________________________________________________
//  
void DAlign::Display(){

  // Display stored distances track-hit
  //
  // JB, 2009/05/13
  // Modified, JB 2012/08/23 condition on non nul-entries added
  // Modified, JB 2012/08/22 fit added if Integral large enough
  
  //cout<<fSession->GetPlaneNumber()<<endl;

  if ( fhAlignPosUV && fhAlignPosUV->GetEntries()>0) { 
    fAlignCanvas->cd(1); 
    fhAlignPosUV->Draw(); 
  }
  if ( fhAlignDU && fhAlignDU->Integral()>0) { 
    fAlignCanvas->cd(5); 
    fhAlignDU->Draw();
    fhAlignDU->Fit("gaus");
    //gPad->SaveAs(fileNameResDU);
  }
  if ( fhAlignPosVU && fhAlignPosVU->GetEntries()>0) {
    fAlignCanvas->cd(2); 
    fhAlignPosVU->Draw(); 
  }
  if ( fhAlignDVDU && fhAlignDVDU->GetEntries()>0) { 
    fAlignCanvas->cd(2); 
    fhAlignDVDU->Draw();
  }
  if ( fhAlignDV && fhAlignDV->Integral()>0) { 
    fAlignCanvas->cd(6); 
    fhAlignDV->Draw();
    fhAlignDV->Fit("gaus");
    //gPad->SaveAs(fileNameResDV);
  }
  if ( fhAlignPosUU && fhAlignPosUU->GetEntries()>0) { 
    fAlignCanvas->cd(3); 
    fhAlignPosUU->Draw(); 
  }
  if ( fhAlignPosVV && fhAlignPosVV->GetEntries()>0) { 
    fAlignCanvas->cd(4); 
    fhAlignPosVV->Draw(); 
  }
  if ( fhAlignUU && fhAlignUU->GetEntries()>0) { // JB 2014/08/26
    fAlignCanvas->cd(7); 
    fhAlignUU->Draw(); 
  }
  if ( fhAlignVV && fhAlignVV->GetEntries()>0) { // JB 2014/08/26
    fAlignCanvas->cd(8); 
    fhAlignVV->Draw(); 
  }
  fAlignCanvas->Update(); 
}

//______________________________________________________________________________
//  
void DAlign::DisplayDeformation(){

 // Display profile of stored distance track-hit
 // BB 2014/05/20
 
  if ( fpfxAlignPosUV && fpfxAlignPosUV->GetEntries()>0){
    fpfxAlignCanvas->cd(1);
    fpfxAlignPosUV->Draw();
  }
  fpfxAlignCanvas->Update();
  if ( fpfxAlignPosVU && fpfxAlignPosVU->GetEntries()>0){
    fpfxAlignCanvas->cd(2);
    fpfxAlignPosVU->Draw();
  }
  if ( fpfxAlignPosUU && fpfxAlignPosUU->GetEntries()>0){
    fpfxAlignCanvas->cd(3);
    fpfxAlignPosUU->Draw();
  }
  if ( fpfxAlignPosVV && fpfxAlignPosVV->GetEntries()>0){
    fpfxAlignCanvas->cd(4);
    fpfxAlignPosVV->Draw();
  }
  fpfxAlignCanvas->Update();

  if ( fhAlignPosUUV && fhAlignPosUUV->GetEntries()>0){ // BB 070414
    fAlign3DCanvas->cd(1);
    fhAlignPosUUV->Draw("surf1");
  }
  if ( fhAlignPosVUV && fhAlignPosVUV->GetEntries()>0){ // BB 080414
    fAlign3DCanvas->cd(2);
    fhAlignPosVUV->Draw("surf1");
  }
  fAlign3DCanvas->Update();

}

//______________________________________________________________________________
//  
void DAlign::DisplayDeformationFitted(){
  // Display profile of stored distance track-hit with a Legendre polynomials fit
  // BB 2014/05/20
  // Modified JB 2015/10/31 to update Legendre polynome coefficients
  
  TF1 *fit = NULL;
  
//  Double_t *coeffU = new Double_t[8];
//  Double_t *coeffV = new Double_t[8];
//  if( f1legendre7U != NULL) coeffU = (Double_t*)f1legendre7U->GetParameters();
//  if( f1legendre7V != NULL) coeffV = (Double_t*)f1legendre7V->GetParameters();
  Double_t coeffU[8], coeffV[8];
  printf( "\n Current coeff for DeltaU vs U:");
  for (Int_t ipar=0; ipar<8; ipar++) {
    if( f1legendre7U != NULL) coeffU[ipar] = f1legendre7U->GetParameter( ipar);
    else coeffU[ipar] = 0.0;
    printf(" %.1f", coeffU[ipar]);
  }
  printf("\n");
  printf( " Current coeff for DeltaU vs V:");
  for (Int_t ipar=0; ipar<8; ipar++) {
    if( f1legendre7V != NULL) coeffV[ipar] = f1legendre7V->GetParameter( ipar);
    else coeffV[ipar] = 0.0;
    printf(" %.1f", coeffV[ipar]);
  }
  printf("\n");
  
  
  
  if ( fpfxAlignPosUV && fpfxAlignPosUV->GetEntries()>0){
    fpfxAlignCanvas->cd(1);
    fpfxAlignPosUV->Fit("f1legendre7V");
    fpfxAlignPosUV->Draw();
    fit = fpfxAlignPosUV->GetFunction("f1legendre7V");  
    if( fit != NULL){
      printf( "\n -> NEW fitted coeff for DeltaU vs V:\n coeffLegendreV");
      for (Int_t ipar=1; ipar<8; ipar++) {
        printf(": %.1f", coeffV[ipar]+fit->GetParameter( ipar));
      }
      printf("\n\n");
    }
  }
  fpfxAlignCanvas->Update();
  fit=NULL;
  if ( fpfxAlignPosVU && fpfxAlignPosVU->GetEntries()>0){
    fpfxAlignCanvas->cd(2);
    fpfxAlignPosVU->Fit("f1legendre7U");
    fpfxAlignPosVU->Draw();
//    fit = fpfxAlignPosVU->GetFunction("f1legendre7U");  
//    if( fit != NULL){
//      printf( "\n -> NEW fitted coeff for DeltaV vs U:");
//      for (Int_t ipar=0; ipar<8; ipar++) {
//        printf(" %.1f", fit->GetParameter( ipar));
//      }
//      printf("\n\n");
//    }
  }
  fit=NULL;
  if ( fpfxAlignPosUU && fpfxAlignPosUU->GetEntries()>0){
    fpfxAlignCanvas->cd(3);
    fpfxAlignPosUU->Fit("f1legendre7U");
    fpfxAlignPosUU->Draw();
    fit = fpfxAlignPosUU->GetFunction("f1legendre7U");  
    if( fit != NULL){
      printf( "\n -> NEW fitted coeff for DeltaU vs U:\n coeffLegendreU");
      for (Int_t ipar=1; ipar<8; ipar++) {
        printf(": %.1f", coeffU[ipar]+fit->GetParameter( ipar));
      }
      printf("\n\n");
    }
  }
  fit=NULL;
  if ( fpfxAlignPosVV && fpfxAlignPosVV->GetEntries()>0){
    fpfxAlignCanvas->cd(4);
    fpfxAlignPosVV->Fit("f1legendre7V");
    fpfxAlignPosVV->Draw();
//    fit = fpfxAlignPosVV->GetFunction("f1legendre7V");  
//    if( fit != NULL){
//      printf( "\n -> NEW fitted coeff for DeltaV vs V:");
//      for (Int_t ipar=0; ipar<8; ipar++) {
//        printf(" %.1f", fit->GetParameter( ipar));
//      }
//      printf("\n\n");
//    }
  }
  fpfxAlignCanvas->Update();

}    

//______________________________________________________________________________
//  
void DAlign::Display(Int_t fRunNumber){

  // Display stored distances track-hit
  //
  // JB, 2009/05/13
  // Modified, JB 2012/08/23 condition on non nul-entries added
  //cout<<fSession->GetPlaneNumber()<<endl;
  //
  // Modified LC 2012/09/20 : save histo in resultis_ana_Mxx dir.

  TString fileNameResDU = "residu_DU_plane"; 
  fileNameResDU += fPlaneNumber;
  fileNameResDU += "_";
  fileNameResDU += fRunNumber;
  fileNameResDU += ".root";
 
  TString fileNameResDV = "residu_DV_plane";
  fileNameResDV += fPlaneNumber;
  fileNameResDV += "_";
  fileNameResDV += fRunNumber;
  fileNameResDV += ".root";


  if ( fhAlignPosUV && fhAlignPosUV->GetEntries()>0) { 
    fAlignCanvas->cd(1); 
    fhAlignPosUV->Draw(); 
  }
  if ( fhAlignDU && fhAlignDU->GetEntries()>0) { 
    fAlignCanvas->cd(5); 
    fhAlignDU->Draw();
    fhAlignDU->Fit("gaus");
    fhAlignDU->SaveAs(fileNameResDU);
  }
  if ( fhAlignPosVU && fhAlignPosVU->GetEntries()>0) {
    fAlignCanvas->cd(2); 
    fhAlignPosVU->Draw(); 
  }
  if ( fhAlignDVDU && fhAlignDVDU->GetEntries()>0) { 
    fAlignCanvas->cd(2); 
    fhAlignDVDU->Draw();
  }
  if ( fhAlignDV && fhAlignDV->GetEntries()>0) { 
    fAlignCanvas->cd(6); 
    fhAlignDV->Draw();
    fhAlignDV->Fit("gaus");
    fhAlignDV->SaveAs(fileNameResDV);
  }
  if ( fhAlignPosUU && fhAlignPosUU->GetEntries()>0) { 
    fAlignCanvas->cd(3); 
    fhAlignPosUU->Draw(); 
  }
  if ( fhAlignPosVV && fhAlignPosVV->GetEntries()>0) { 
    fAlignCanvas->cd(4); 
    fhAlignPosVV->Draw(); 
  }
  if ( fhAlignUU && fhAlignUU->GetEntries()>0) { // JB 2014/08/26
    fAlignCanvas->cd(7); 
    fhAlignUU->Draw(); 
  }
  if ( fhAlignVV && fhAlignVV->GetEntries()>0) { // JB 2014/08/26
    fAlignCanvas->cd(8); 
    fhAlignVV->Draw(); 
  }
  fAlignCanvas->Update(); 
}

//______________________________________________________________________________
//  
void DAlign::DisplayDeformation(Int_t fRunNumber){

  // Display profile of stored distances track-hit
  // BB 2014/05/20
  //
  // JB, 2009/05/13
  // Modified, JB 2012/08/23 condition on non nul-entries added
  //cout<<fSession->GetPlaneNumber()<<endl;
  //
  // Modified LC 2012/09/20 : save histo in resultis_ana_Mxx dir.

  if (fpfxAlignPosUV && fpfxAlignPosUV->GetEntries()>0) {                       // BB 060314
    fpfxAlignCanvas->cd(1);
    fpfxAlignPosUV->Draw();
  }
  if (fpfxAlignPosVU && fpfxAlignPosVU->GetEntries()>0) {                       // BB 060314
    fpfxAlignCanvas->cd(2);
    fpfxAlignPosVU->Draw();
  }

  if (fpfxAlignPosUU && fpfxAlignPosUU->GetEntries()>0) {                       // BB 060314
    fpfxAlignCanvas->cd(3);
    fpfxAlignPosUU->Draw();
  }
  if (fpfxAlignPosVV && fpfxAlignPosVV->GetEntries()>0) {                       // BB 060314
    fpfxAlignCanvas->cd(4);
    fpfxAlignPosVV->Draw();
  }
  fpfxAlignCanvas->Update();

  if ( fhAlignPosUUV && fhAlignPosUUV->GetEntries()>0){                         // BB 070414
    fAlign3DCanvas->cd(1);
    fhAlignPosUUV->Draw("surf1");
  }
  if ( fhAlignPosVUV && fhAlignPosVUV->GetEntries()>0){                         // BB 070414
    fAlign3DCanvas->cd(2);
    fhAlignPosUUV->Draw("surf1");
  }
  fAlign3DCanvas->Update();

}

//______________________________________________________________________________
//  
void DAlign::DisplayDeformationFitted(Int_t fRunNumber){
 // Display profile of stored distance track-hit with a Legendre polynomials fit
 // BB 2014/05/20
  // Modified JB 2015/10/31 to update Legendre polynome coefficients
  
  TF1 *fit = NULL;
  
  Double_t *coeffU, *coeffV;
  coeffU = (Double_t*)f1legendre7U->GetParameters();
  coeffV = (Double_t*)f1legendre7V->GetParameters();
  printf( "\n Current coeff for DeltaU vs U:");
  for (Int_t ipar=0; ipar<8; ipar++) {
    printf(" %.1f", coeffU[ipar]);
  }
  printf("\n");
  printf( " Current coeff for DeltaU vs V:");
  for (Int_t ipar=0; ipar<8; ipar++) {
    printf(" %.1f", coeffV[ipar]);
  }
  printf("\n");
  
  
 
  if ( fpfxAlignPosUV && fpfxAlignPosUV->GetEntries()>0){
    fpfxAlignCanvas->cd(1);
    fpfxAlignPosUV->Fit("f1legendre7V");
    fpfxAlignPosUV->Draw();
    fit = fpfxAlignPosUV->GetFunction("f1legendre7V");  
    if( fit != NULL){
      printf( "\n -> NEW fitted coeff for DeltaU vs V:\n coeffLegendreV");
      for (Int_t ipar=1; ipar<8; ipar++) {
        printf(": %.1f", coeffV[ipar]+fit->GetParameter( ipar));
      }
      printf("\n\n");
    }
  }
  fpfxAlignCanvas->Update();
  if ( fpfxAlignPosVU && fpfxAlignPosVU->GetEntries()>0){
    fpfxAlignCanvas->cd(2);
    fpfxAlignPosVU->Fit("f1legendre7U");
    fpfxAlignPosVU->Draw();
  }
  fit=NULL;
  if ( fpfxAlignPosUU && fpfxAlignPosUU->GetEntries()>0){
    fpfxAlignCanvas->cd(3);
    fpfxAlignPosUU->Fit("f1legendre7U");
    fpfxAlignPosUU->Draw();
    fit = fpfxAlignPosUU->GetFunction("f1legendre7U");  
    if( fit != NULL){
      printf( "\n -> NEW fitted coeff for DeltaU vs U:\n coeffLegendreU");
      for (Int_t ipar=1; ipar<8; ipar++) {
        printf(": %.1f", coeffU[ipar]+fit->GetParameter( ipar));
      }
      printf("\n\n");
    }
  }
  if ( fpfxAlignPosVV && fpfxAlignPosVV->GetEntries()>0){
    fpfxAlignCanvas->cd(4);
    fpfxAlignPosVV->Fit("f1legendre7V");
    fpfxAlignPosVV->Draw();
  }
  fpfxAlignCanvas->Update();


}    
//______________________________________________________________________________
//  
void DAlign::SaveDisplay( const Char_t *fileName){

  // Save all canvas and histos in a file
  //
  // JB 2014/04/06
  
  TFile *outputFile=new TFile(fileName,"UPDATE");

  // Canvas
  fAlignCanvas->Write();
  //fAlign3DCanvas->Write();  // COMMENT BB 2015/07/28

  
  // histos
  fhAlignDU->Write();
  fhAlignDV->Write();
  fhAlignDVDU->Write();
  fhAlignPosUV->Write();
  fhAlignPosVU->Write();
  fhAlignPosUU->Write();
  fhAlignPosVV->Write();
  fhAlignUU->Write(); // JB 2014/08/26
  fhAlignVV->Write();
  //fhAlignPosUUV->Write(); 
  //fhAlignPosVUV->Write();
  
  outputFile->Close();
  
}  
 
//______________________________________________________________________________
// 
void DAlign::SaveDisplayDeformation( const Char_t *fileName){
  // Save all canvas and histos in a file
  //
  // JB 2014/04/06, modified BB 2014/05/20

  TFile *outputFileDeformation=new TFile(fileName,"UPDATE");

  // Canvas
  fpfxAlignCanvas->Write();
  //fAlign3DCanvas->Write();

  // histos
  fpfxAlignPosUV->Write();
  fpfxAlignPosVU->Write();
  fpfxAlignPosUU->Write();
  fpfxAlignPosVV->Write();

  outputFileDeformation->Close();

}

//______________________________________________________________________________
//  
void DAlign::AccumulateU(DR3& aTrackPos, Double_t aDistanceU){

  // First updates event by event measured and predicted positions to compute a chi square.
  // Once the number of events is enough (hard coded to 200, see below),
  // does a line fit to the measurements of (u_hit - u_track) versus (v_track),
  // u_track and v_track are the intersections of the track with this plane,
  // u_hit=aDistanceU is the measured position of the hit in this plane.
  //
  // Only for U direction (see equivalent function for V dim), JB, 2007 June
  //
  // Last Modified: JB 2010/12/14, taking into account potential rotation in Y is now unecessary due to the proper management of the plane rotations

  Double_t       tDenominator;

  if(fDebugAlign>1) printf("   DAlign: plane %d Accumulating in U with distance %.1f <?> %.1f\n", fPlaneNumber, aDistanceU, fBoundU);
  if(fDebugAlign && fAlignCountu%50==0 && fAlignCountu) cout << " plane " << fPlaneNumber << " has accumulated " << fAlignCountu << " distances in the U direction." << endl;

  // Accumaltion of hits
  if (fabs(aDistanceU) <= fBoundU) {
    if (fhAlignPosUV) {
      fhAlignPosUV->Fill( aTrackPos(1), aDistanceU);
      if( fAlignCountu%50==0 ) { 
	fAlignCanvas->cd(1); 
	fhAlignPosUV->Draw(); 
	fAlignCanvas->Update(); 
      }
    }   
   if (fpfxAlignPosUV) {                                // BB 060314
      fpfxAlignPosUV->Fill( aTrackPos(1), aDistanceU);
      if( fAlignCountu%50==0 ) {
      fpfxAlignCanvas->cd(1);
      fpfxAlignPosUV->Draw();
      fpfxAlignCanvas->Update();
      }
    }
   
   if (fhAlignPosUUV) {
      fhAlignPosUUV->Fill( aTrackPos(0), aTrackPos(1), aDistanceU);    // BB 080414
      if ( fAlignCountu%50==0) {
        fAlign3DCanvas->cd(1);
        fhAlignPosUUV->Draw("colz");
        fAlign3DCanvas->Update();
      }
   }
    fAlignSum_du   += aDistanceU;                    // summation of u_hit - u_track
    fAlignSum_dudu += (aDistanceU * aDistanceU);     // summation for Resolution estimate
    fAlignSum_duvt += (aDistanceU * aTrackPos(1));   // summation of (u_hit - u_track) * v_track
    fAlignSum_vt   += aTrackPos(1); 
    fAlignSum_vtvt += (aTrackPos(1) * aTrackPos(1));
    fAlignCountu++;
    if (fDebugAlign) printf("   DAlign::AccumulateU plane %d, dist=%.1f / %.1f, du=%.1f, dudu=%.1f, vt=%.1f, countU=%d/%d\n", fPlaneNumber, fabs(aDistanceU), fBoundU, fAlignSum_du, fAlignSum_dudu, fAlignSum_vt, fAlignCountu, fNumberOfEvents);
  }

  // Compute parameters according to the linear chisquare fit
  if (fAlignCountu > fNumberOfEvents) {
    
    tDenominator = fAlignCountu * fAlignSum_vtvt - fAlignSum_vt * fAlignSum_vt;
    
    fAlignOffsetU= (fAlignSum_du * fAlignSum_vtvt - fAlignSum_duvt * fAlignSum_vt) / tDenominator;
    fAlignTiltW  = (fAlignCountu  * fAlignSum_duvt - fAlignSum_du   * fAlignSum_vt) / tDenominator;

    fAlignOffsetUError = sqrt( fAlignSum_vtvt / tDenominator );
    fAlignTiltWError   = sqrt( fAlignCountu / tDenominator );
    fAlignCorrelation  = fAlignSum_vt / ( tDenominator * fAlignOffsetUError * fAlignTiltWError );

    fAlignOffset(0)    = fAlignOffsetU;

    fAlignMeanU        = fAlignSum_du/fAlignCountu;
    fAlignResolutionU  = sqrt(fAlignSum_dudu/fAlignCountu - fAlignMeanU * fAlignMeanU);
    
    fEnoughU = kTRUE;
    if (fDebugAlign) printf("DAlign::AccumulateU Pl %d Uoffset=%.1f +/- %.1f, Uresidu=%.1f +/- %.1f\n", fPlaneNumber, fAlignOffsetU, fAlignOffsetUError, fAlignMeanU, fAlignResolutionU);
    if (fDebugAlign && fhAlignPosUV) printf("     histoU mean=%.1f rms=%.1f\n", fhAlignPosUV->GetMean(2), fhAlignPosUV->GetRMS(2));
  }
  else
    fEnoughU = kFALSE;
}

//______________________________________________________________________________
//  
void DAlign::AccumulateV(DR3& aTrackPos, Double_t aDistanceV){

  // First updates event by event measured and predicted positions to compute a chi square.
  // Once the number of events is enough (hard coded to 200, see below),
  // does a line fit to the measurements of (v_hit - v_track) versus (u_track),
  // u_track and v_track are the intersections of the track with this plane,
  // v_hit=aDistanceV is the measured position of the hit in this plane
  //
  // Only for V direction (See equivalent function for U dim, JB, 2007 June
  //
  // Inversion of tilt angle when plane is X-ratated by 180deg
  // Last Modified: JB 2010/12/14, taking into account potential rotation in X is now unecessary due to the proper management of the plane rotations

  Double_t       tDenominator;
  
  if( fDebugAlign>1) printf("   DAlign: plane %d Accumulating in V with distance %.1f <?> %.1f\n", fPlaneNumber, aDistanceV, fBoundV);
  if(fDebugAlign && fAlignCountv && fAlignCountv%50==0 ) cout << " plane " << fPlaneNumber << " has accumulated " << fAlignCountv << " distances in the V direction." << endl;

  // Accumaltion of hits
  if (fabs(aDistanceV) <= fBoundV) {
    if (fhAlignPosVU) {
      fhAlignPosVU->Fill( aTrackPos(0), aDistanceV);
      if( fAlignCountv%50==0 ) { 
	fAlignCanvas->cd(2); 
	fhAlignPosVU->Draw(); 
	fAlignCanvas->Update(); 
      }
    }
    if (fpfxAlignPosVU) {                               // BB 060314
      fpfxAlignPosVU->Fill( aTrackPos(0), aDistanceV);
      if( fAlignCountv%50==0 ) {
        fpfxAlignCanvas->cd(2);
        fpfxAlignPosVU->Draw();
        fpfxAlignCanvas->Update();
      }
    }

   if (fhAlignPosVUV) {
      fhAlignPosVUV->Fill( aTrackPos(0), aTrackPos(1), aDistanceV);    // BB 080414
      if ( fAlignCountv%50==0) {
        fAlign3DCanvas->cd(1);
        fhAlignPosVUV->Draw("colz");
        fAlign3DCanvas->Update();
      }
    }
    fAlignSum_dv   += aDistanceV;                    // summation of v_hit - v_track
    fAlignSum_dvdv += aDistanceV * aDistanceV;       // summation for Resolution estimate
    fAlignSum_dvut += aDistanceV * aTrackPos(0);     // summation of (v_hit - v_track) * u_track
    fAlignSum_ut   += aTrackPos(0); 
    fAlignSum_utut += aTrackPos(0) * aTrackPos(0);
    fAlignCountv++;
    if (fDebugAlign) printf("   DAlign::AccumulateV plane %d, dist=%.1f / %.1f, dv=%.1f, dvdv=%.1f, ut=%.1f, countV=%d/%d\n", fPlaneNumber, fabs(aDistanceV), fBoundV, fAlignSum_dv, fAlignSum_dvdv, fAlignSum_ut, fAlignCountv, fNumberOfEvents);
  }

  // Compute parameters according to the linear chisquare fit
  if (fAlignCountv > fNumberOfEvents) {
    
    tDenominator = fAlignCountv * fAlignSum_utut - fAlignSum_ut * fAlignSum_ut;
    
    fAlignOffsetV= (fAlignSum_dv * fAlignSum_utut - fAlignSum_dvut * fAlignSum_ut) / tDenominator;
    
    fAlignOffsetVError = sqrt( fAlignSum_utut / tDenominator );

    fAlignOffset(1)    = fAlignOffsetV;

    fAlignMeanV        = fAlignSum_dv/fAlignCountv;
    fAlignResolutionV  = sqrt(fAlignSum_dvdv/fAlignCountv - fAlignMeanV * fAlignMeanV);
    
    fEnoughV = kTRUE;
    if (fDebugAlign) printf("   DAlign::AccumulateV plane %d Voffset=%.1f +/- %.1f, Vresidu=%.1f +/- %.1f\n", fPlaneNumber, fAlignOffsetV, fAlignOffsetVError, fAlignMeanV, fAlignResolutionV);
    if (fDebugAlign && fhAlignPosVU) printf("     histoV mean=%.1f rms=%.1f\n", fhAlignPosVU->GetMean(2), fhAlignPosVU->GetRMS(2));
  }
  else
    fEnoughV = kFALSE;
}

//______________________________________________________________________________
//  
void DAlign::Accumulate2D(DR3& aTrackPos, Double_t aDistanceU, Double_t aDistanceV){

  // method to correlate alignment in U and V
  //
  // First updates event by event measured and predicted positions to compute a chi square:
  //  chi2 = M-matrix x Align-vector - Shift-vector,
  //  where Shift-vector = {sum(uhit-utrack), sum(vhit-vtrack), sum[vtrack(uhit-utrack)-utrack(vhit-vtrack)]}
  //  and   Align-vector = { Uoffser, Voffset, TiltW}.
  // Once the number of events is enough (fNumberOfEvents),
  // does a line fit or simulatenous chi2 minimization to determine inverse-M.
  // So, Align-vector = inverse-M x Shift-vector.
  //
  // JB, 2008/10/27
  // Last Modified: JB 2010/12/14, taking into account potential rotation in X or Y is now unecessary due to the proper management of the plane rotations

  
  if( fDebugAlign>1) printf("   DAlign: plane %d Accumulating in U and V with distance (%f, %f) <?> (%.1f, %.1f)\n", fPlaneNumber, aDistanceU, aDistanceV, fBoundU, fBoundV);

  // Accumaltion of hits
//   Double_t aDistanceU = aHitPos(0)-aTrackPos(0);
//   Double_t aDistanceV = aHitPos(1)-aTrackPos(1);
  if ( fabs(aDistanceU)<=fBoundU && fabs(aDistanceV)<=fBoundV ) {

//     if (fhAlignPosU) {
//       fhAlignPosU->Fill( aTrackPos(1), aDistanceU);
//       if( fAlignCountu%50==0 ) { 
// 	fAlignCanvas->cd(1); 
// 	fhAlignPosU->Draw(); 
// 	fAlignCanvas->Update(); 
//       }
//     }
//     if (fhAlignPosV) {
//       fhAlignPosV->Fill( aTrackPos(0), aDistanceV);
//       if( fAlignCountv%50==0 ) { 
// 	fAlignCanvas->cd(2); 
// 	fhAlignPosV->Draw(); 
// 	fAlignCanvas->Update(); 
//       }
//     }

    fAlignSum_du     += aDistanceU;                    // summation of u_hit - u_track
    fAlignSum_dv     += aDistanceV;                    // summation of v_hit - v_track
    fAlignSum_ut     += aTrackPos(0); 
    fAlignSum_vt     += aTrackPos(1); 
    fAlignSum_utut += aTrackPos(0) * aTrackPos(0);
    fAlignSum_vtvt += (aTrackPos(1) * aTrackPos(1));
    fAlignSum_uvt2   += aTrackPos(0) * aTrackPos(0) + aTrackPos(1) * aTrackPos(1);
    fAlignSum_dvuduv += aDistanceU * aTrackPos(1) - aDistanceV * aTrackPos(0);
    fAlignSum_dudu   += aDistanceU * aDistanceU;       // summation for Resolution estimate
    fAlignSum_dvdv   += aDistanceV * aDistanceV;       // summation for Resolution estimate
    fAlignSum_duvt += (aDistanceU * aTrackPos(1));   // summation of (u_hit - u_track) * v_track
    fAlignSum_dvut += aDistanceV * aTrackPos(0);     // summation of (v_hit - v_track) * u_track
    fAlignCount2D++;

    if (fDebugAlign) printf("   DAlign::Accumulate2D plane %d, dist=(%.1f, %.1f) <?> (%.1f, %.1f), track=(%.1f, %.1f), Sum=(%.1f, %.1f), count2D=%d/%d\n", fPlaneNumber, aDistanceU, aDistanceV, fBoundU, fBoundV, aTrackPos(0), aTrackPos(1), fAlignSum_ut, fAlignSum_vt, fAlignCount2D, fNumberOfEvents);
  }

  // Compute parameters according to the linear chisquare fit
  if (fAlignCount2D > fNumberOfEvents) {

    /*
    Double_t tDenominator = fAlignCount2D * fAlignSum_uvt2 - fAlignSum_ut * fAlignSum_ut - fAlignSum_vt * fAlignSum_vt;
    
    Double_t invM[3][3];
    invM[0][0] =  fAlignSum_uvt2-fAlignSum_ut*fAlignSum_ut/fAlignCount2D;
    invM[0][1] = -fAlignSum_vt*fAlignSum_ut/fAlignCount2D;
    invM[0][2] =  fAlignSum_vt;
    invM[1][0] = -fAlignSum_vt*fAlignSum_ut/fAlignCount2D;
    invM[1][1] =  fAlignSum_uvt2-fAlignSum_vt*fAlignSum_vt/fAlignCount2D;
    invM[1][2] = -fAlignSum_ut;  
    invM[2][0] = -fAlignSum_vt;
    invM[2][1] =  fAlignSum_ut;
    invM[2][2] = -fAlignCount2D;  

    Double_t vec[3] = { fAlignSum_du, fAlignSum_dv, fAlignSum_dvuduv};
    Double_t sol[3] = {0., 0., 0.};
    // sol = invM * vec;
    for( Int_t i=0; i<3; i++) {
      for( Int_t j=0; j<3; j++) {
	sol[i] += invM[i][j]*vec[j];
      }
      sol[i] /= tDenominator;
    }

    fAlignOffsetU = sol[0];
    fAlignOffsetV = sol[1];
    fAlignTiltW   = sol[2];

    fAlignOffsetUError = 0.; // don't know yet how to compute
    fAlignOffsetVError = 0.;
    fAlignTiltWError   = 0.;
    */

    Double_t tDenominator = fAlignCount2D * fAlignSum_vtvt - fAlignSum_vt * fAlignSum_vt;
    fAlignOffsetU= (fAlignSum_du * fAlignSum_vtvt - fAlignSum_duvt * fAlignSum_vt) / tDenominator;
    fAlignTiltW  = (fAlignCount2D  * fAlignSum_duvt - fAlignSum_du   * fAlignSum_vt) / tDenominator;
    fAlignOffsetUError = sqrt( fAlignSum_vtvt / tDenominator );
    fAlignTiltWError   = sqrt( fAlignCount2D / tDenominator );
    fAlignCorrelation  = fAlignSum_vt / ( tDenominator * fAlignOffsetUError * fAlignTiltWError );
    tDenominator = fAlignCount2D * fAlignSum_utut - fAlignSum_ut * fAlignSum_ut;
    fAlignOffsetV= (fAlignSum_dv * fAlignSum_utut - fAlignSum_dvut * fAlignSum_ut) / tDenominator;
    fAlignOffsetVError = sqrt( fAlignSum_utut / tDenominator );

    fAlignOffset(0)    = fAlignOffsetU;
    fAlignOffset(1)    = fAlignOffsetV;

    fAlignMeanU        = fAlignSum_du/fAlignCount2D;
    fAlignResolutionU  = sqrt(fAlignSum_dudu/fAlignCount2D - fAlignMeanU * fAlignMeanU);
    fAlignMeanV        = fAlignSum_dv/fAlignCount2D;
    fAlignResolutionV  = sqrt(fAlignSum_dvdv/fAlignCount2D - fAlignMeanV * fAlignMeanV);
    
    fEnough2D = kTRUE;
    if (fDebugAlign) printf("   DAlign::Accumulate2D plane %d uoffset=%.1f +/- %.1f Voffset=%.1f +/- %.1f, Uresidu=%.1f +/- %.1f, Vresidu=%.1f +/- %.1f\n", fPlaneNumber, fAlignOffsetU, fAlignOffsetUError, fAlignOffsetV, fAlignOffsetVError, fAlignMeanU, fAlignResolutionU, fAlignMeanV, fAlignResolutionV);
//     if (fDebugAlign && fhAlignPosU) printf("     histoU mean=%.1f rms=%.1f\n", fhAlignPosU->GetMean(2), fhAlignPosU->GetRMS(2));
//     if (fDebugAlign && fhAlignPosV) printf("     histoV mean=%.1f rms=%.1f\n", fhAlignPosV->GetMean(2), fhAlignPosV->GetRMS(2));
  }
  else
    fEnough2D = kFALSE;
}

//______________________________________________________________________________
//  
void DAlign::Modified()
{
  // reset the summation variables
  // and update the max distance track-hit
  //
  // Modified: JB 2011/04/05 re-init of Enough flags added
  
  printf(" Alignement Accumulation restarts .... \n\n");
  
  fAlignSum_du      = 0.0;
  fAlignSum_dudu    = 0.0;
  fAlignSum_duvt    = 0.0;  
  fAlignSum_vt      = 0.0;
  fAlignSum_vtvt    = 0.0; 
  fAlignCountu      = 0;
  fEnoughU         = kFALSE; // JB 2011/04/05
  fAlignSum_dv      = 0.0; // JB
  fAlignSum_dvdv    = 0.0;
  fAlignSum_dvut    = 0.0;  
  fAlignSum_ut      = 0.0;
  fAlignSum_utut    = 0.0; 
  fAlignCountv      = 0;
  fEnoughV         = kFALSE; // JB 2011/04/05
  fAlignSum_dvuduv  = 0; // JB 2009/05/18
  fAlignSum_uvt2    = 0; // JB 2009/05/18
  fAlignCount2D     = 0; // JB 2009/05/13
  fEnough2D         = kFALSE; // JB 2011/04/05
  Double_t tBoundU, tBoundV;
  // Introduction of bounds in both direction, JB 2013/06/10
  // tBound /= 2.; // replaced by the following 2 lines, JB, 2007 August
  // minimal value, changed from 150 to 50 by JB, 2009/07/19
  Double_t minimalBound = 50.;
  tBoundU = TMath::Max( minimalBound, 1.5*fabs(fAlignResolutionU));
  tBoundV = TMath::Max( minimalBound, 1.5*fabs(fAlignResolutionV));
  if( tBoundU < fBoundU ) {
    SetBoundings(tBoundU, fBoundV);
  } 
  if( tBoundV < fBoundV ) {
    SetBoundings(fBoundU, tBoundV);
  } 
//   if(fhAlignPosU) fhAlignPosU->Reset();
//   if(fhAlignPosV) fhAlignPosV->Reset();
}

//______________________________________________________________________________
//  

void DAlign::ShowCorrection()
{
  printf(" Found Deviation in U Position [um] : %f\n",fAlignOffsetU);
  printf(" Found Deviation in V Position [um] : %f\n",fAlignOffsetV);
  printf(" Found Deviation in W-Tilt     [deg]: %f\n",fAlignTiltW*180/M_PI);
  printf(" -> Estimated Resolution U %f um at Residual Mean %f um\n",fAlignResolutionU,fAlignMeanU);
  printf(" -> Estimated Resolution V %f um at Residual Mean %f um\n",fAlignResolutionV,fAlignMeanV);
  printf(" Estimated Offest-U Error      [um] : %f\n",fAlignOffsetUError);
  printf(" Estimated Offest-V Error      [um] : %f\n",fAlignOffsetVError);
  printf(" Estimated Tilt-W Error        [deg]: %f\n",fAlignTiltWError*180/M_PI);  
  printf(" Correlation between du and vt      : %f\n",fAlignCorrelation);  
}

//______________________________________________________________________________
//  

void DAlign::ShowStoredResolution()
{
  // JB 2010/12/14
  printf(" -> Estimated Resolution U %f um at Residual Mean %f um\n",fhAlignDU->GetRMS(),fhAlignDU->GetMean());
  printf(" -> Estimated Resolution V %f um at Residual Mean %f um\n",fhAlignDV->GetRMS(),fhAlignDV->GetMean());
}

//______________________________________________________________________________
//  
void DAlign::FitDeformation(Double_t (*CoeffLegendreU), Double_t (*CoeffLegendreV), Float_t minU, Float_t maxU, Float_t minV, Float_t maxV){

  // Define a Legendre polynomial of the order of 7th
  //
  // BB 2014/05/20
  // Modified JB 2015/10/31 range for fit function implemented
    
    cout << " Coefficients for U | Coefficients for V coordinates written in config file : " << endl;
    cout << "   - CoeffU(0) : " << CoeffLegendreU[0]  << " | CoeffV(0) : " << CoeffLegendreV[0] << endl;
    cout << "   - CoeffU(1) : " << CoeffLegendreU[1]  << " | CoeffV(1) : " << CoeffLegendreV[1] << endl;
    cout << "   - CoeffU(2) : " << CoeffLegendreU[2]  << " | CoeffV(2) : " << CoeffLegendreV[2] << endl;
    cout << "   - CoeffU(3) : " << CoeffLegendreU[3]  << " | CoeffV(3) : " << CoeffLegendreV[3] << endl;
    cout << "   - CoeffU(4) : " << CoeffLegendreU[4]  << " | CoeffV(4) : " << CoeffLegendreV[4] << endl;
    cout << "   - CoeffU(5) : " << CoeffLegendreU[5]  << " | CoeffV(5) : " << CoeffLegendreV[5] << endl;
    cout << "   - CoeffU(6) : " << CoeffLegendreU[6]  << " | CoeffV(6) : " << CoeffLegendreV[6] << endl;
    cout << "   - CoeffU(7) : " << CoeffLegendreU[7]  << " | CoeffV(7) : " << CoeffLegendreV[7] << endl;

  // If no specified range, use sensor size (coeff[0])
  if( fabs(minU-maxU)<1. ) {
    minU = -CoeffLegendreU[0];
    maxU = CoeffLegendreU[0];
  }
  if( fabs(minV-maxV)<1. ) {
    minV = -CoeffLegendreV[0];
    maxV = CoeffLegendreV[0];
  }
  
  f1legendre7U = new TF1("f1legendre7U","[1] + [2]*x/[0] + [3]*(3*x*x/[0]/[0]-1)/8 + [4]*x/[0]*(5*x*x/[0]/[0]-3)/2 + [5]*(35*x*x*x*x/[0]/[0]/[0]/[0]-30*x*x/[0]/[0]+3)/8 + [6]*x/[0]*(63*x*x*x*x/[0]/[0]/[0]/[0]-70*x*x*x/[0]/[0]/[0]+15)/8 + [7]*(231*x*x*x*x*x*x/[0]/[0]/[0]/[0]/[0]/[0]-315*x*x*x*x/[0]/[0]/[0]/[0]+105*x*x/[0]/[0]-5)/16", minU, maxU);
  f1legendre7U->SetParameters( CoeffLegendreU[0], CoeffLegendreU[1], CoeffLegendreU[2], CoeffLegendreU[3], CoeffLegendreU[4], CoeffLegendreU[5], CoeffLegendreU[6], CoeffLegendreU[7]);
  f1legendre7U->FixParameter( 0, CoeffLegendreU[0]);
  
  f1legendre7V = new TF1("f1legendre7V","[1] + [2]*x/[0] + [3]*(3*x*x/[0]/[0]-1)/8 + [4]*x/[0]*(5*x*x/[0]/[0]-3)/2 + [5]*(35*x*x*x*x/[0]/[0]/[0]/[0]-30*x*x/[0]/[0]+3)/8 + [6]*x/[0]*(63*x*x*x*x/[0]/[0]/[0]/[0]-70*x*x*x/[0]/[0]/[0]+15)/8 + [7]*(231*x*x*x*x*x*x/[0]/[0]/[0]/[0]/[0]/[0]-315*x*x*x*x/[0]/[0]/[0]/[0]+105*x*x/[0]/[0]-5)/16", minV, maxV);
  f1legendre7V->SetParameters( CoeffLegendreV[0], CoeffLegendreV[1], CoeffLegendreV[2], CoeffLegendreV[3], CoeffLegendreV[4], CoeffLegendreV[5], CoeffLegendreV[6], CoeffLegendreV[7]);
  f1legendre7V->FixParameter( 0, CoeffLegendreV[0]);
}
