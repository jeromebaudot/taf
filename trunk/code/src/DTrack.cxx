// @(#)maf/dtools:$Name:  $:$Id:DTrack.cxx  v.1 2005/10/02 18:03:46 sha Exp $
//*-- Copyright:  RD42
// Author   :  Dirk Meier   98/01/07
// Last modified : JB 2009/05/25
// Last Modified: JB 2009/07/20
// Last Modified: JB 2009/09/11 makechi2
// Last Modified: JB 2009/09/14 Analyse
// Last Modified: JB 2010/11/25 Analyse (new fit algorithm)
// Last Modified: JB 2011/07/25 destructor
// Last Modified: JB 2012/05/07 new mechanism to store hits
// Last Modified: JB 2012/08/20 debug is now set by DTracker
// Last Modified: JB 2012/08/27 Analyse, strip-telescope tracking
// Last Modified: VR 2014/06/29 Add GetDistTr2Hit() and fDistTr2Hit, computed in makeChiSquare()
// Last Modified: VR 2014/11/07 Add fHitsMemorized and accessor
// Last Modified: LC 2014/12/08 Add MonteCarlo Tracks from DHitMonteCarlo
// Last Modified: LC 2014/12/15 Remove MC tracks. and hitMC collection.

  //////////////////////////////////////////////////////////////////
  // Class Description of DTrack                                  //
  //                                                              //
  // A particle track from e.g. accelerator passing the tracker   //
  // The track is measured by the tracker with its silicon        //
  // reference planes                                             //
  // The track is e.g. a straight line                            //
  // The line is found by a fit to the hits in the silicon planes // 
  //                                                              //
  //////////////////////////////////////////////////////////////////


//*KEEP,CopyRight.
/************************************************************************
 * Copyright(c) 1998, RD42@CERN, DiamondTracking
 ***********************************************************************/
//*KEND.

//*KEEP,DTrack.
#include "DTrack.h"
#include "DTracker.h"
//*KEEP,DLine.
#include "DLine.h"
//*KEEP,DHit.
#include "DHit.h"
//#include "DHitMonteCarlo.h"
//*KEEP,DR3.
#include "DR3.h"
//*KEEP,DPlane.
#include "DPlane.h"
//*KEEP,DPrecAlign.
#include "DPrecAlign.h"
//*KEEP,DParticle.
#include "DParticle.h"
//*KEND.
#include "DTrackFitter.h"
#include "MKalmanFilter.h"

ClassImp(DTrack) // Description of a Track

//extern  int fopen_lockfile(); // JB, removed for inline compilation
//extern  FILE * VG_Lockfile;
//______________________________________________________________________________
//  
DTrack::DTrack( Int_t maxNHits)
{
  fDebugTrack     = 0;
  fTrackNumber    = -1; // CD, Nov 2007, JB 2009/07/20
  fLineTrajectory = new DLine();
  fParticle       = new DParticle();
  fTangent        = new DR3();
  fMaxNHits       = maxNHits; // JB 2012/05/07
  fHitList        = new DHit*[fMaxNHits];   // these are pointers to hits to make one track, JB 2012/05/07
  //fHitListMC      = new DHitMonteCarlo*[fMaxNHits];
  fValid          = kFALSE;
  fShareHit       = 0; // CD, Nov 2007
  fMaxHitsPerPlane= 0; // JB 2009/05/25
 //if(0)  fopen_lockfile();
  SetVertex(0.,0.,0.);
}


//______________________________________________________________________________
//
DTrack::DTrack() // /!\ 2 hits only /!\ Default constructor. To put DTrack objects in a container :)
{
  fDebugTrack     = 0;
  fTrackNumber    = -1; // CD, Nov 2007, JB 2009/07/20
  fLineTrajectory = new DLine();
  fParticle       = new DParticle();
  fTangent        = new DR3();
  fMaxNHits       = 3; // JB 2012/05/07
  fHitList        = new DHit*[fMaxNHits];   // 2 hits minimum to make the track.
  fValid          = kFALSE;
  fShareHit       = 0; // CD, Nov 2007
  fMaxHitsPerPlane= 0; // JB 2009/05/25
 //if(0)  fopen_lockfile();
  SetVertex(0.,0.,0.);
  fHitsMemorized = 0;
}


//______________________________________________________________________________
//  
DTrack::DTrack(const DTrack& aTrack)
{
  fDebugTrack     = aTrack.GetDebug(); // JB 2012/09/20
  fTrackNumber    = aTrack.GetNumber(); // CD, Nov 2007, JB 2009/07/20
  fLineTrajectory = new DLine(aTrack.GetLinearFit());
  fParticle       = new DParticle(aTrack.GetParticle());
  fTangent        = new DR3(aTrack.GetTangent());
  // New mechanism to store hits
   // JB 2012/05/07
  fMaxNHits       = aTrack.GetMaxNHits();
  fHitList        = new DHit*[fMaxNHits];
  //for( Int_t iHit=0; iHit< aTrack.GetHitsNumber(); iHit++ ) {
  //  fHitList[iHit] = (DHit*)(aTrack.GetHit( iHit));
  //}
  fValid          = kFALSE;
  fShareHit       = aTrack.GetShareHit(); // CD, Nov 2007
  fMaxHitsPerPlane= aTrack.GetMaxHitsPerPlane(); // JB, 2009/05/25
  //if(0) fopen_lockfile();
  //SetVertex(0.,0.,0.);
  fHitsMemorized = aTrack.GetHitsMemorizedNumber();
}

//______________________________________________________________________________
// 
DTrack::DTrack(DTrack* aTrack)
{
  
  fLineTrajectory = new DLine(aTrack->GetLinearFit());
  fParticle       = new DParticle(aTrack->GetParticle());
  fTangent        = new DR3(aTrack->GetTangent());

  fMaxNHits       = aTrack->GetMaxNHits();
  fHitList        = new DHit*[fMaxNHits];

  //std::cout<<"Constructor by copy, DTRACK !!!!!!!!!!!!!!!!!"<<std::endl;

  for( Int_t iHit=0; iHit<aTrack->GetHitsNumber(); ++iHit ) {
    fHitList[iHit] = (DHit*)(aTrack->GetHit(iHit));
  }

  fValid          = kFALSE;
}
//______________________________________________________________________________
//
DTrack::DTrack(DR3 Origin, DR3 Slope)
{

  //Bluilding a DTrack object from the original and slope vectors

  fDebugTrack     = 0;
  fTrackNumber    = -1;
  fLineTrajectory = new DLine();
  fParticle       = new DParticle();
  fTangent        = new DR3();
  fMaxNHits       = 2;
  fHitList        = new DHit*[fMaxNHits];
  fValid          = kFALSE;
  fShareHit       = 0;
  fMaxHitsPerPlane= 0;
  SetVertex(0.,0.,0.);
  fHitsMemorized = 0;
  
  DR3           tLineFitOrigin;  // origin in the tracker system
  DR3           tLineFitSlope;   // slope along z-axis in tracker system
  Float_t       tDx=0., tDy=0., tDz=1.;
  DR3           tLineFitDirection; // calculated from slope in z direction
  Float_t       tLength = 0.;

  tLineFitOrigin(0) = Origin(0);
  tLineFitOrigin(1) = Origin(1);
  tLineFitOrigin(2) = Origin(2);
  tLineFitSlope(0)  = Slope(0);
  tLineFitSlope(1)  = Slope(1);
  tLineFitSlope(2)  = Slope(2);

  tDx = tLineFitSlope(0);
  tDy = tLineFitSlope(1);
  tDz = 1/sqrt(tDx * tDx + tDy * tDy + 1);

  tLineFitDirection = (tLineFitSlope * tDz);
  fLineTrajectory->SetValue(tLineFitOrigin,tLineFitDirection,tLineFitSlope,tLength);

}
//______________________________________________________________________________
//  
void DTrack::copy(const DTrack& aTrack)
{
  *fLineTrajectory = aTrack.GetLinearFit();
  *fParticle       = aTrack.GetParticle();
  *fTangent        = aTrack.GetTangent();
  fHitsMemorized   = aTrack.GetHitsMemorizedNumber();
}

//______________________________________________________________________________
//  
DTrack::~DTrack()
{
  delete fLineTrajectory;
  //delete fHitList; no needed, JB 2011/07/25
  delete fParticle;
  delete fTangent;
  delete fHitList; // JB 2012/05/07
}

//____________________________________________________________________________
//  
void DTrack::Reset()
{
  fLineTrajectory->Zero();
  fParticle->Vacuum();
  fTangent->Zero();
  fValid          = kFALSE;  
  fDeltaOrigineX   = 0.0 ;
  fDeltaOrigineY   = 0.0 ;
  fHitsMemorized = 0;
}

//_____________________________________________________________________________
//  
Bool_t DTrack::Analyze( Int_t aNumber, DHit** aHitList, Int_t nHits, Float_t resolution)
{

  // Fits the track in a linear approximation from the collection of hits.
  // The fit depends on the number of hits (fHits) in the collection:
  // If fHits>=3 : least square method,
  // If fHits==2 : line drawn from two points,
  // If fHits==1 : choice by changing the comments (see in code),
  //   choice 1: the track is perpendicular to the plane (slope=0, origin=hit),
  //   choice 2: the track has a fixed slope and origin=hit,
  //   choice 3: use a specified vertex as a second point.
  //  -> default is choice 1.
  //
  // Returns kTRUE if the track is good enough, kFALSE otherwise.
  //
  // Specific cases with 1 or 2 hits added by JB, 2007 June
  // Fit with Pixel planes added by JB, 2007 August
  // track number added, JB 2009/07/17
  // Fit in tracker frame since 2010/11/25
  //
  // Modified JB 2009/09/14, to handle AnalysisMode>=2	
  // Last Modified JB, 2010/11/25 Use new DPrecAlign mechanism for Plane position
  // Last Modified JB, 2012/05/07 Fix bug on fHitList which was properly stored
  // Last Modified JB, 2012/08/28 Allow strip-telescope track fitting again

  fTrackNumber = aNumber; // JB 2009/07/17
  fHits    = nHits;
  fValid   = kFALSE;

  DPrecAlign *hitAlignment;
  DR3 aPosition;

  // The following line was not insuring the permanent copy of the pointers
  //  and hence was pointing to the wrong hits after the next track treatment.
  //fHitList = aHitList;
  // The following lines manage properly the hit pointers 
  //  which are now permanently attached to this track.
  // JB 2012/05/07
  for( Int_t iHit=0; iHit<nHits; iHit++ ) {
    fHitList[iHit] = aHitList[iHit];
  }

  //std::cout<<"nHits = "<<nHits<<std::endl;

  if( fHitList[0] ) {
    fDebugTrack = fHitList[0]->GetDebug();
  }
  //fDebugTrack = 1;

  if(fDebugTrack) printf("DTrack::Analyse track number %d\n", fTrackNumber);

  DR3           tLineFitOrigin;  // origin in the tracker system
  DR3           tLineFitSlope;   // slope along z-axis in tracker system
  Float_t       tDx=0., tDy=0., tDz=1.;
  DR3           tLineFitDirection; // calculated from slope in z direction
  Float_t       tLength = 0.;

  // init
  tLineFitOrigin.SetValue(0.,0.,0.);
  tLineFitSlope.SetValue(0.,0.,1.);
  tLineFitDirection.SetValue(0.,0.,0.);
  
  fHitsMemorized = 0;
  for( Int_t iHit=0; iHit<nHits; iHit++) if(aHitList[iHit]->GetIsFromPreviousEvent())fHitsMemorized++; 


  //============
  // Case with only one hit, assume the track slope is null
  //============
  // The track origin is taken from the hit position in the plane
  if( fHits==1) {

    DR3 bPosition( *(fHitList[0]->GetPosition()) );
    if(fDebugTrack) printf("DTrack::Analyse track with %d hit taking origin uvw(%.1f, %.1f, %.1f) from it and slope fixed\n", fHits, bPosition(0), bPosition(1), bPosition(2) );

    hitAlignment = fHitList[0]->GetPlane()->GetPrecAlignment();
    aPosition = hitAlignment->TransformHitToTracker( *(fHitList[0]->GetPosition()) );

    //============
    // CHOICE 1, the track has a no slope

    tLineFitOrigin(0) = aPosition(0);
    tLineFitOrigin(1) = aPosition(1);
    tLineFitSlope(0)  = 0.; //-aPosition(0)/fHitList[0]->GetPlane()->GetPosition()(2); // 0. 
    tLineFitSlope(1)  = 0.; //-aPosition(1)/fHitList[0]->GetPlane()->GetPosition()(2); // 0.

    // end choice 1

    // CHOICE 2, tracks with constant slope.
/*
    const Double_t thetaX = 0.;
    const Double_t thetaY = 10.; // in deg.
    const Double_t thetaZ = 0.;
    //const Double_t z0 = 0.;

    tLineFitOrigin(0) = aPosition(0);
    tLineFitOrigin(1) = aPosition(1);
    tLineFitOrigin(2) = aPosition(2);
    // Slope on Y only :
    tLineFitSlope(0)  = tan(thetaY*TMath::Pi()/180.0);
    tLineFitSlope(1)  = 0.; 
*/
    // end choice 2
    
    // CHOICE 3, build the track with a second point (the vertex)
/*
    Double_t x0 = fVertexX;  // the vertex position
    Double_t y0 = fVertexY;
    Double_t z0 = fVertexZ;
   
    Double_t x1 = aPosition(0);
    Double_t y1 = aPosition(1);
    Double_t z1 = aPosition(2);

    // std::cout<<"Set vertex coordinate : X = "<<x0<<" Y = "<<y0<<" Z= "<<z0<<std::endl; 

//    DR3 tilt = fHitList[0]->GetPlane()->GetTilt();
//    std::cout<<" tilts = "<<tilt(0)<<"  "<<tilt(1)<<"   "<<tilt(2)<<std::endl;
//    Double_t zPrime = z1 - x1 *tan(40*TMath::Pi()/180.0);
//    Double_t xPrime = x1 - ((x1*x1)/z1)  * tan(40*TMath::Pi()/180.0);
//    
//    x1 = xPrime;
//    z1 = zPrime;

    if( z1-z0 != 0. ) {
      tLineFitOrigin(0) = (z1*x0-z0*x1)/(z1-z0);
      tLineFitOrigin(1) = (z1*y0-z0*y1)/(z1-z0);    
      tLineFitSlope(0)  = (x1-x0)/(z1-z0);
      tLineFitSlope(1)  = (y1-y0)/(z1-z0);
    }
    else {
      tLineFitOrigin(0) = x1;
      tLineFitOrigin(1) = y1;    
      tLineFitSlope(0)  = 0.;
      tLineFitSlope(1)  = 0.;      
    }
*/
    // end choice 3

    // END CHOICE
    //============
    
    if( fDebugTrack) printf( "DTrack::Analyze hit[pl %d]=xyz(%.2f, %.2f, %.2f)\n",fHitList[0]->GetPlane()->GetPlaneNumber() , aPosition(0), aPosition(1), aPosition(2));

  }


  //============
  // Case with only two 2D hits, no fit
  //============
  // The track slope and origin are simply taken from the difference of the two hit positions
  else if( fHits==2) {

    if(fDebugTrack) printf("DTrack::Analyse track with %d hits taking origin and slope from a simple difference\n", fHits);

//     DR3 aPosition; 
//     Double_t u0 = fHitList[0]->GetPositionUhit() + fHitList[0]->GetPlane()->GetPositionU();
//     Double_t v0 = fHitList[0]->GetPositionVhit() + fHitList[0]->GetPlane()->GetPositionV();
//     // Shifting and rotating hit 0 was added by JB, 2008/10/21
//     aPosition.SetValue( u0, v0 ,0.);
//     aPosition = fHitList[0]->GetPlane()->PlaneToTracker( aPosition);
//     Double_t x0 = aPosition(0);
//     Double_t y0 = aPosition(1);
//     Double_t u1 = fHitList[1]->GetPositionUhit() + fHitList[1]->GetPlane()->GetPositionU();
//     Double_t v1 = fHitList[1]->GetPositionVhit() + fHitList[1]->GetPlane()->GetPositionV();
//     aPosition.SetValue( u1, v1 ,0.);
//     aPosition = fHitList[1]->GetPlane()->PlaneToTracker( aPosition);
//     Double_t x1 = aPosition(0);
//     Double_t y1 = aPosition(1);
//     Double_t z0 = fHitList[0]->GetPlane()->GetPosition()(2);
//     Double_t z1 = fHitList[1]->GetPlane()->GetPosition()(2);

    hitAlignment = fHitList[0]->GetPlane()->GetPrecAlignment();
    aPosition = hitAlignment->TransformHitToTracker( *(fHitList[0]->GetPosition()) );
    Double_t x0 = aPosition(0);
    Double_t y0 = aPosition(1);
    Double_t z0 = aPosition(2);
    hitAlignment = fHitList[1]->GetPlane()->GetPrecAlignment();
    aPosition = hitAlignment->TransformHitToTracker( *(fHitList[1]->GetPosition()) );
    Double_t x1 = aPosition(0);
    Double_t y1 = aPosition(1);
    Double_t z1 = aPosition(2);
    
    if( fDebugTrack) printf( "DTrack::Analyze hit[pl %d]=(%.2f, %.2f, %.2f) hit[pl %d]=(%.2f, %.2f, %.2f)\n",fHitList[0]->GetPlane()->GetPlaneNumber() , x0, y0, z0, fHitList[1]->GetPlane()->GetPlaneNumber(), x1, y1, z1);

    if( z1-z0 != 0.) {
      tLineFitOrigin(0) = (z1*x0-z0*x1)/(z1-z0);
      tLineFitOrigin(1) = (z1*y0-z0*y1)/(z1-z0);
      tLineFitSlope(0)  = (x1-x0)/(z1-z0);
      tLineFitSlope(1)  = (y1-y0)/(z1-z0);
    }
    else {
      printf("DTrack WARNING two hits at same z = %.3f = %.3f, cannot fit track !\n", z0, z1);
    }

  }

  //============
  // Case with only two 1D hits (from STRIPs), no fit
  //============
  // The track origin is taken from the hit position in the plane
  // JB 2013/08/25
  else if( fHits==2 && fHitList[0]->GetPlane()->GetAnalysisMode() == 1 ) {
    
    if(fDebugTrack) printf("DTrack::Analyse track with %d hits taking origin and slope from a simple difference\n", fHits);
    
    Double_t x=1.e12, y=1.e12;
    Bool_t foundX = kFALSE;
    Bool_t foundY = kFALSE;

    for (Int_t ht = 1; ht <= fHits; ht++){ // loop on hit list

      hitAlignment = fHitList[ht-1]->GetPlane()->GetPrecAlignment();
      hitAlignment->TransformHitToTracker( *(fHitList[ht-1]->GetPosition()) );
      aPosition = hitAlignment->TransformHitToTracker( *(fHitList[ht-1]->GetPosition()) );
      
      if( fabs(cos(hitAlignment->GetRotations()[0])) < 0.1 ) { // vertical position
        y = aPosition(1);
        foundY = kTRUE;
      }
      else {
        x = aPosition(0);
        foundY = kTRUE;        
      }

    } // end loop on hit list
    
      if( foundX && foundX ) {
        tLineFitOrigin(0) = x;
        tLineFitOrigin(1) = y;
        tLineFitSlope(0)  = 0.;
        tLineFitSlope(1)  = 0.;
      }
      else {
        Warning("DTrack::Analyze","Did not found both X(%s) and Y(%s) -> no track!", (foundX?"yes":"no"), (foundY?"yes":"no"));
      }
        
    if( fDebugTrack) printf( "DTrack::Analyze 2 1D hits from planes %d and %d at x,y =(%.2f, %.2f)\n", fHitList[0]->GetPlane()->GetPlaneNumber(), fHitList[1]->GetPlane()->GetPlaneNumber(), x, y);
    
  }

  //============
  // Least square fit when more than two hits
  // 
  // afit is the 4 elements vector containing the line parameters in 3D
  //      (x, y) = (afit[0]+z*afit[1], afit[2]+zafit[3])
  // 
  // first compute the 4x4 matrix(2D fit) depending on the case Strip or Pixels
  // then the chi2 minimization is equivalent to a matrix inversion
  // 
  // This part was changed by JB 2010/11/25 to implement a least square fit
  // algo in the tracker frame, not anymore from the position in the plane frame
  //============
  else { // fHits>2

    if(fDebugTrack) printf("DTrack::Analyse track with %d hits fitting\n", fHits);

    Double_t      fact[4];
    Double_t      varm[4][4];
    Double_t      covm[4][4];
    Double_t      afit[4];
    Double_t      uvec[4];

    //-- Case for STRIPS
    // still fit in the plane frame
    // taken from John's code. See also section 4.1.6 of Dirk Meier's thesis, page 127 to 130
    // Sign of fact[2] and fact[3] was set to -sin() to match new alignement,
    //  changed by JB 2012/08/27
    if( fHitList[0]->GetPlane()->GetAnalysisMode() == 1 ) { // if strips
      vzero(&afit[0],4);
      vzero(&varm[0][0],4*4);
      vzero(&covm[0][0],4*4);
      vzero(&uvec[0],4);
      vzero(&fact[0],4);
      
      for (Int_t ht = 1; ht <= fHits; ht++){ // loop on hit list        
        fact[0] = cos( fHitList[ht-1]->GetPlane()->GetTilt()(0) ); 
        fact[1] = cos( fHitList[ht-1]->GetPlane()->GetTilt()(0) ) * fHitList[ht-1]->GetPlane()->GetPosition()(2); 
        fact[2] = -sin( fHitList[ht-1]->GetPlane()->GetTilt()(0) ); 
        fact[3] = -sin( fHitList[ht-1]->GetPlane()->GetTilt()(0) ) * fHitList[ht-1]->GetPlane()->GetPosition()(2); 
	
        for (Int_t j = 0; j < 4; j++) {
          uvec[j] += ( fact[j] * ( fHitList[ht-1]->GetPositionUhit() + fHitList[ht-1]->GetPlane()->GetPositionU() )/(resolution * resolution) );
          for (Int_t k = 0; k < 4; k++) {
            varm[j][k] += ( fact[j] * fact[k] / (resolution * resolution) );
          }
        }
        
        if( fDebugTrack) printf( "DTrack::Analyze for STRIPS hit[pl %d] positionU = %.2f\n",fHitList[ht-1]->GetPlane()->GetPlaneNumber(), fHitList[ht-1]->GetPositionUhit() );
        
        
        hitAlignment = fHitList[ht-1]->GetPlane()->GetPrecAlignment();
        aPosition = hitAlignment->TransformHitToTracker( *(fHitList[ht-1]->GetPosition()) );
        
        if( fDebugTrack) printf( "DTrack::Analyze for STRIPS hit %d plane %d tilt = %.1f %.1f %.1f", ht, fHitList[ht-1]->GetPlane()->GetPlaneNumber(), hitAlignment->GetRotations()[0], hitAlignment->GetRotations()[1], hitAlignment->GetRotations()[2] );
        
        if( fabs(cos(hitAlignment->GetRotations()[0])) < 0.1 ) { // vertical position
          if( fDebugTrack) printf( ", vertical position = %.1f\n", aPosition(1));
          uvec[2] += aPosition(1)/(resolution * resolution);
          uvec[3] += aPosition(1)*aPosition(2)/(resolution * resolution);
          
          varm[2][0] = 0.;
          varm[2][1] = 0.;
          varm[2][2] += 1./(resolution * resolution);
          varm[2][3] += aPosition(2)/(resolution * resolution);
          varm[3][0] = 0.;
          varm[3][1] = 0.;
          varm[3][2] += aPosition(2)/(resolution * resolution);
          varm[3][3] += aPosition(2)*aPosition(2)/(resolution * resolution);
        }
        else { // horizontal position
          if( fDebugTrack) printf( ", horizontal position = %.1f\n", aPosition(0));
          uvec[0] += aPosition(0)/(resolution * resolution);
          uvec[1] += aPosition(0)*aPosition(2)/(resolution * resolution);
          
          varm[0][0] += 1./(resolution * resolution);
          varm[0][1] += aPosition(2)/(resolution * resolution);
          varm[0][2] = 0.;
          varm[0][3] = 0.;
          varm[1][0] += aPosition(2)/(resolution * resolution);
          varm[1][1] += aPosition(2)*aPosition(2)/(resolution * resolution);
          varm[1][2] = 0.;
          varm[1][3] = 0.;
        }
      } //end loop on hit list
      
    } // end if strips
    
    //-- Case for PIXELS (Added by JB, 2007 August)
    // fit in the telescope frame since 2010/11/25
    else if ( fHitList[0]->GetPlane()->GetAnalysisMode() >= 2 ){ // if pixels, JB 2009/09/14 for >=2
      vzero(&afit[0],4);
      vzero(&varm[0][0],4*4);
      vzero(&covm[0][0],4*4);
      vzero(&uvec[0],4);
      
      for (Int_t ht = 1; ht <= fHits; ht++){ // loop on hits
	
        hitAlignment = fHitList[ht-1]->GetPlane()->GetPrecAlignment();
        aPosition = hitAlignment->TransformHitToTracker( *(fHitList[ht-1]->GetPosition()) );
	double resolutionU = fHitList[ht-1]->GetResolutionUhit();
	double resolutionV = fHitList[ht-1]->GetResolutionVhit();

        /*
        if(fHitList[0]->GetPlane()->GetAnalysisMode() == 1) { // if strips
          if( fabs(cos(hitAlignment->GetRotations()[0])) < 0.1 ) { // vertical position
            uvec[2] += aPosition(1)/(resolution * resolution);
            uvec[3] += aPosition(1)*aPosition(2)/(resolution * resolution);

            varm[2][0] = 0.;
            varm[2][1] = 0.;
            varm[2][2] += 1./(resolution * resolution);
            varm[2][3] += aPosition(2)/(resolution * resolution);
            varm[3][0] = 0.;
            varm[3][1] = 0.;
            varm[3][2] += aPosition(2)/(resolution * resolution);
            varm[3][3] += aPosition(2)*aPosition(2)/(resolution * resolution);
          }
          else {
            uvec[0] += aPosition(0)/(resolution * resolution);
            uvec[1] += aPosition(0)*aPosition(2)/(resolution * resolution);

            varm[0][0] += 1./(resolution * resolution);
            varm[0][1] += aPosition(2)/(resolution * resolution);
            varm[0][2] = 0.;
            varm[0][3] = 0.;
            varm[1][0] += aPosition(2)/(resolution * resolution);
            varm[1][1] += aPosition(2)*aPosition(2)/(resolution * resolution);
            varm[1][2] = 0.;
            varm[1][3] = 0.;
          }
        }
        else {*/

	/*
        //Previous way of doing tracking using only one global resolution for U and V
        uvec[0] += aPosition(0)/(resolution * resolution);
        uvec[1] += aPosition(0)*aPosition(2)/(resolution * resolution);
        uvec[2] += aPosition(1)/(resolution * resolution);
        uvec[3] += aPosition(1)*aPosition(2)/(resolution * resolution);
        
        varm[0][0] += 1./(resolution * resolution);
        varm[0][1] += aPosition(2)/(resolution * resolution);
        varm[0][2] = 0.;
        varm[0][3] = 0.;
        varm[1][0] += aPosition(2)/(resolution * resolution);
        varm[1][1] += aPosition(2)*aPosition(2)/(resolution * resolution);
        varm[1][2] = 0.;
        varm[1][3] = 0.;
        varm[2][0] = 0.;
        varm[2][1] = 0.;
        varm[2][2] += 1./(resolution * resolution);
        varm[2][3] += aPosition(2)/(resolution * resolution);
        varm[3][0] = 0.;
        varm[3][1] = 0.;
        varm[3][2] += aPosition(2)/(resolution * resolution);
        varm[3][3] += aPosition(2)*aPosition(2)/(resolution * resolution);
	*/
	
        //New way of doing tracking using a resolution on U and V for each hit
        uvec[0] += aPosition(0)/(resolutionU * resolutionU);
        uvec[1] += aPosition(0)*aPosition(2)/(resolutionU * resolutionU);
        uvec[2] += aPosition(1)/(resolutionV * resolutionV);
        uvec[3] += aPosition(1)*aPosition(2)/(resolutionV * resolutionV);
        
        varm[0][0] += 1./(resolutionU * resolutionU);
        varm[0][1] += aPosition(2)/(resolutionU * resolutionU);
        varm[0][2] = 0.;
        varm[0][3] = 0.;
        varm[1][0] += aPosition(2)/(resolutionU * resolutionU);
        varm[1][1] += aPosition(2)*aPosition(2)/(resolutionU * resolutionU);
        varm[1][2] = 0.;
        varm[1][3] = 0.;
        varm[2][0] = 0.;
        varm[2][1] = 0.;
        varm[2][2] += 1./(resolutionV * resolutionV);
        varm[2][3] += aPosition(2)/(resolutionV * resolutionV);
        varm[3][0] = 0.;
        varm[3][1] = 0.;
        varm[3][2] += aPosition(2)/(resolutionV * resolutionV);
        varm[3][3] += aPosition(2)*aPosition(2)/(resolutionV * resolutionV);

        //}

      } // en loop on hits

    } // end if pixels
    
    //-- Unknown case
    else{ // unknown mode
      printf("WARNING from DTrack::Analyze, unkown AnalysisMode %d for plane %d, STOP!\n", fHitList[0]->GetPlane()->GetAnalysisMode(), fHitList[0]->GetPlane()->GetPlaneNumber());
      return fValid;
    }
    
    // invert matrix
    invert(4,&varm[0][0],&covm[0][0]);
      
    // multiply matrix by measurements vector
    
    for (Int_t j = 0; j < 4; j++) {
      for (Int_t k = 0; k < 4; k++) {
        afit[j] += covm[j][k] * uvec[k];
      }
    }
/*   
    // Test New Track Fitting :
    DR3 initialTrackDirections(afit[1], afit[3], 1.);
    DR3 initialTrackOrigin(afit[0], afit[2], 0.);
   
    std::cout<<" hit Number = "<<fHits<<std::endl;
    initialTrackDirections.Print();
    initialTrackOrigin.Print();

    DTrackFitter* trackFit = new DTrackFitter( 4, fHits, 2, 1, fHitList, initialTrackDirections, initialTrackOrigin);
    trackFit->ProcessHits();

    DR3 fittedTrackDirections = trackFit->GetTrackDirections();
    DR3 fittedTrackOrigin    = trackFit->GetTrackOrigin();
   
    delete trackFit; 
    
    std::cout<<"Track Direction : Avant/Apres :"<<std::endl;
    initialTrackDirections.Print();
    fittedTrackDirections.Print();

    std::cout<<"Track Origin : Avant/Apres : "<<std::endl;
    initialTrackOrigin.Print();
    fittedTrackOrigin.Print();
*/   
    // End test
    
    tLineFitOrigin(0) = /*fittedTrackOrigin(0);*/ afit[0];
    tLineFitOrigin(1) = /*fittedTrackOrigin(1);*/ afit[2];
    tLineFitOrigin(2) = 0.0;
    tLineFitSlope(0)  = /*fittedTrackDirections(0);*/ afit[1];
    tLineFitSlope(1)  = /*fittedTrackDirections(1);*/ afit[3];
    tLineFitSlope(2)  = 1.0;
    
    fDeltaOrigineX = sqrt(covm[0][0]);
    fDeltaOrigineY = sqrt(covm[2][2]);
    
  } // end case with more than 2 hits

  tDx = tLineFitSlope(0);
  tDy = tLineFitSlope(1);
  tDz = 1/sqrt(tDx * tDx + tDy * tDy + 1);
  tLineFitDirection = (tLineFitSlope * tDz);
  fLineTrajectory->SetValue(tLineFitOrigin,tLineFitDirection,tLineFitSlope,tLength);

  makeChiSquare(resolution);   

  if(fDebugTrack) printf("DTrack::Analyse track with %d hits is x=%.4f z + %.4f and y=%.4f z + %.4f with chi2=%f\n", fHits, tLineFitSlope(0), tLineFitOrigin(0), tLineFitSlope(1), tLineFitOrigin(1), fChiSquare);


  //============
  // put here to analyse the  quality of the tracks. YG
  //============
  fValid = kTRUE;

  for( Int_t iHit=0; iHit<fHits; iHit++ ) {
    if(  fMaxHitsPerPlane && fHitList[iHit]->GetPlane()->GetHitsN() > fMaxHitsPerPlane ) { // JB, 2009/05/25
      fValid = kFALSE;
      if(fDebugTrack) printf("DTrack::Analyse track rejected because %d > %d hits in plane %d\n", fHitList[iHit]->GetPlane()->GetHitsN(), fMaxHitsPerPlane, fHitList[iHit]->GetPlane()->GetPlaneNumber());
    }
  }

  // Requires the track is almost horizontal, JB Sept 2007
  Double_t maxSlope = 10.0; // 0.105;
  if( TMath::Abs(tDx)>maxSlope || TMath::Abs(tDy)>maxSlope ) {
    fValid = kFALSE;
    if(fDebugTrack) printf("DTrack::Analyse track rejected because slope (%f, %f) > %f\n", tDx, tDy, maxSlope);
  }

  /*
  if( fValid ) {
    // Refit track with just two 2D points to mitigate multiple scattering
    // Special trick for DESY runs
    Int_t listOfPlanes[4] = { 3, 4, 5, 6}; // for strip telescope
    ReFit( 4, listOfPlanes);
    // end of refit
  }
   */
  
  return fValid;
}

// __________________________________________________________________________
//
void DTrack::makeChiSquare(Float_t dhs){
  // Computes the chi2 of the fit track using dhs as the error
  //
  // Computation in V dim added by JB, August 2007.
  // Test for pixels changed to analysisMode>=2 changed by JB 2009/09/11
  // Computation structure (not value) slightly changed for better clarity, JB 2013/08/27
  // Compute fDistTr2Hit, by VR 2014/06/29

  Float_t tdU, tdV;
  Float_t tUtrack, tVtrack;
  Double_t reducedU, reducedV, reduced;
  Int_t   ndfU, ndfV, ndf;
  Int_t hitDim;
  DPlane *aPlane;
  fDistTr2Hit=0.0;

  // Initialization
  fChiSquare = fChiSquareU = fChiSquareV = 0.0;
  if(fHitList[0]->GetPlane()->GetAnalysisMode() == 1) { // if strips
    ndfU = -4; // indeed, two parameters are fit per dimemsions which both use U in plane frame
    ndfV = 0;
    hitDim = 1;
    if( fHits<=4 ) return; // return 0. for chisquare if there is less than 2 hits
  } //end if strips
  else { // if pixels
    ndfU = ndfV = -2; // indeed, two parameters are fit per dimemsions (4 in all)
    hitDim = 2;
    if( fHits<=2 ) return; // return 0. for chisquare if there is less than 2 hits    
  } // end if pixels

  
  
  // Loop on hits
  for (Int_t ht = 0; ht < fHits; ht++){
    aPlane = fHitList[ht]->GetPlane();
    tUtrack = Intersection(aPlane)(0);
    //tdU     = (fHitList[ht]->GetPositionUhit() - tUtrack) / dhs;
    tdU     = (fHitList[ht]->GetPositionUhit() - tUtrack) / fHitList[ht]->GetResolutionUhit();
    ndfU++;
    fChiSquareU += tdU * tdU;
    if(fDebugTrack) printf( "   makeChiSquare: Delta_U: hit %d, plane %d, hu=%.1f, tu=%.1f, deltaU=%.1f, chi2U=%.1f\n", ht, aPlane->GetPlaneNumber(), fHitList[ht]->GetPositionUhit(), tUtrack, tdU, fChiSquareU);

    // only for pixel detector
    if( aPlane->GetAnalysisMode() >= 2 ) { // >=2 JB 2009/09/11
      tVtrack = Intersection(aPlane)(1);
      //tdV     = (fHitList[ht]->GetPositionVhit() - tVtrack) / dhs;
      tdV     = (fHitList[ht]->GetPositionVhit() - tVtrack) / fHitList[ht]->GetResolutionVhit();
      ndfV++;
      fChiSquareV += tdV * tdV;
      fDistTr2Hit += sqrt(tdU*tdU+tdV*tdV);
      if(fDebugTrack) printf( "   makeChiSquare: Delta_V: plane %d, hv=%.1f, tv=%.1f, deltaV=%.1f, chi2V=%.1f\n", aPlane->GetPlaneNumber(), fHitList[ht]->GetPositionVhit(), tVtrack, tdV, fChiSquareV);
    }
  } // end loop on hits
  fDistTr2Hit = fDistTr2Hit/fHits;
  
  // Final computation
  // ndfV and fChiSquareV = 0 if only strips
  //reducedU = fChiSquareU / ndfU / (dhs*dhs);
  reducedU = fChiSquareU / ndfU;
  reducedV = 0;
  //if( ndfV>0 ) reducedV = fChiSquareV / ndfV / (dhs*dhs);
  if( ndfV>0 ) reducedV = fChiSquareV / ndfV;
  ndf = ndfU + ndfV;
  fChiSquare = fChiSquareU + fChiSquareV;
  //reduced = fChiSquare / ndf / (dhs*dhs);  
  reduced = fChiSquare / ndf;
  
  if(fDebugTrack) printf("DTrack::MakeChiSquare with %d %dD-hits, Sum(DU^2)=%.1f / ndfU=%d ==> %.1f, Sum(DV^2)=%.1f / ndfV=%d ==> %.1f, Sum(DU^2+DV^2)=%f / ndf=%d ==> %.1f, resol=%f\n", fHits, hitDim, fChiSquareU, ndfU, reducedU, fChiSquareV, ndfV, reducedV, fChiSquare, ndf, reduced, dhs);

  fChiSquareU = reducedU;
  fChiSquareV = reducedV;
  fChiSquare  = reduced;

}

//_____________________________________________________________________________
//  
void DTrack::vzero(Double_t *array, Int_t N)
{
  for (Int_t j = 0; j < N; j++)
    array[j] = 0.0;
}

//_____________________________________________________________________________
//  
void DTrack::invert(Int_t n, Double_t *a, Double_t *b)
{
  // taken from John's tta code
  Double_t q[15][15];
  Double_t e;
  Int_t    i,j,k;

  vzero(&q[0][0],15*15);
  
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      b[i*n+j] = a[i*n+j];

  for (i = 0; i < n; i++)
    if (b[i*n+i] != 0.0) q[i][i] = 1.;
  
  for (k = 0; k < n; k++) {
    if (b[k*n+k] != 0.0 ) {
      for (i = 0; i < n; i++) {
	if (i != k) {
	  e = b[i*n+k];
	  for (j = 0; j < n; j++) {
	    b[i*n+j] -= e * b[k*n+j] / b[k*n+k];
	    if (j == k ) b[i*n+j] = 0.0;
	    q[i][j] -= e * q[k][j] / b[k*n+k];
	  }
	}
      }
    }
  }

  for (i = 0; i < n; i++) {
    e = b[i*n+i];
    if (e == 0.0) e = 1.;
    for (j = 0; j < n; j++){
      b[i*n+j] = q[i][j]/e;
    }
  }

}

//______________________________________________________________________________
//  
DR3 DTrack::Intersection(DPlane *aPlane)
{

  // calculates the Intersection of the Track with the plane in
  // the coordinate system of the plane. This gives the intersection (u_track,v_track,0)
  //
  // JB 2010/11/25

  return aPlane->Intersection( this);//GetPrecAlignment()->TransformTrackToPlane( GetLinearFit().GetOrigin()(0), GetLinearFit().GetOrigin()(1), GetLinearFit().GetOrigin()(2), GetLinearFit().GetSlopeZ()(0), GetLinearFit().GetSlopeZ()(1));

}

void DTrack::SetVertex(Float_t vertexX, Float_t vertexY, Float_t vertexZ)
{
  fVertexX = vertexX;
  fVertexY = vertexY;
  fVertexZ = vertexZ;
}

//______________________________________________________________________________
//  
/*
DR3 DTrack::Intersection(DPlane *aPlane)
{

  // functions deprecated USE NEW DPRECALIGN MECHANISM (JB 2010/11/25)

  // calculates the Intersection of the Track with the plane in
  // the coordinate system of the plane. This gives the intersection (u_track,v_track,0)

  DR3 result(GetLinearFit().GetOrigin());  // track origin in xyz tracker coordinates
  result   += GetLinearFit().GetSlopeZ() * aPlane->GetPosition()(2); // intersection in xyz frame at z_plane 
  result  =  aPlane->Rotate_TrackerToPlane( result );  // now rotate to the uvd frame
  DR3 offset(aPlane->GetPositionU(),aPlane->GetPositionV(),0.); // this is the offset of the plane
  result -=  offset;
  return  result;

}
*/


//_____________________________________________________________________________
//  
Bool_t DTrack::ReFit( Int_t nPlanes, Int_t *ListOfPlanes)
{
  
  // Refit the track parameter with the subset of specified planes.
  // Not that the initial chi2 is not changed, 
  //  so that a quality cut can still be applied.
  //
  // JB 2013/08/24
  
  fDebugTrack=2;
  
  if(fDebugTrack) Info("DTrack::Refit", "Track %d will be refitted from %d points to %d points!", fTrackNumber, fHits, nPlanes);
  
  Int_t nFoundPlanes = 0;
  
  // Test there is enough planes
  if( fHits<nPlanes ) {
    Warning("DTrack::Refit","Not enough hits (%d) in track to refit with %d planes", fHits, nPlanes);
    return kFALSE;
  }
  
  // Find hits matching the required planes
  DHit *aHit;
  DHit **newHitList = new DHit*[nPlanes];
  for (Int_t ht=0; ht < fHits; ht++){ // loop on hit list
    if(fDebugTrack) printf("  Refit consider hit %d with %d planes already found.\n", ht, nFoundPlanes);
    aHit = fHitList[ht];
    for (Int_t iPlane=0; iPlane<nPlanes; iPlane++) {
      if( aHit->GetPlane()->GetPlaneNumber() == ListOfPlanes[iPlane] ) {
        newHitList[ht] = aHit;
        nFoundPlanes++;
        break;
      }
    }
  } // end on hit list
  
  // If all required hits found, try to refit
  // If successful, replace the current track fit with the new one
  Bool_t fitSuccess = kFALSE;
  if (nFoundPlanes == nPlanes) {
    DTrack *newTrack = new DTrack();
    if(fDebugTrack) printf("  Refiting...\n");
    fitSuccess = newTrack->Analyze( fTrackNumber, newHitList, nPlanes, 4.);
    if( fitSuccess ) {
      if(fDebugTrack>1) {  
        printf( " --> Refit successfull: \n");
        printf("    old track with %d hits was x=%.4f z + %.4f and y=%.4f z + %.4f with chi2=%f\n", fHits, fLineTrajectory->GetSlopeZ()(0), fLineTrajectory->GetOrigin()(0), fLineTrajectory->GetSlopeZ()(1), fLineTrajectory->GetOrigin()(1), fChiSquare);
      }
      DLine *aLinearFit = &(newTrack->GetLinearFit());
      SetLinearFit( aLinearFit );      
      if(fDebugTrack>1) printf("    new track with %d hits was x=%.4f z + %.4f and y=%.4f z + %.4f with chi2=%f\n", fHits, fLineTrajectory->GetSlopeZ()(0), fLineTrajectory->GetOrigin()(0), fLineTrajectory->GetSlopeZ()(1), fLineTrajectory->GetOrigin()(1), fChiSquare);

      }
    delete newTrack;
  }
  else {
    Warning("DTrack::Refit", "Refit fails because only %d requested planes found among the %d hits\n", nPlanes, fHits);
  }
  
  fDebugTrack=0;

  //delete[] newHitList;
  return fitSuccess;
  
}
//_____________________________________________________________________________
//
void  DTrack::SetLinearFit2(DLine* aLine)
{

  //cout << "Dtrack::SetLinearFit2::  Origin    = (" << aLine->GetOrigin()(0)    << "," << aLine->GetOrigin()(1)    << "," << aLine->GetOrigin()(2) << ")" << endl;
  //cout << "Dtrack::SetLinearFit2::  Direction = (" << aLine->GetDirection()(0) << "," << aLine->GetDirection()(1) << "," << aLine->GetDirection()(2) << ")" << endl;
  //cout << "Dtrack::SetLinearFit2::  SlopeZ    = (" << aLine->GetSlopeZ()(0)    << "," << aLine->GetSlopeZ()(1)    << "," << aLine->GetSlopeZ()(2) << ")" << endl;
  //cout << "Dtrack::SetLinearFit2::  Length    =  " << aLine->GetLength() << endl;

  fLineTrajectory->SetValue(aLine->GetOrigin(),
			    aLine->GetDirection(),
			    aLine->GetSlopeZ(),
			    aLine->GetLength());

  return;

}
