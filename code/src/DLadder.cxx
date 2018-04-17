#include "DLadder.h"
#include "DSetup.h"
#include "DSession.h"
#include "DAcq.h"
#include "DTracker.h"
#include "DHit.h"
//#include "DHitMonteCarlo.h"
#include "DPlane.h"
#include "DLine.h"
#include "DTrack.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TVector3.h"
#include "TRotation.h"

// Created b y LC
// Last Modified: BH 2013/08/20, warning corrections from clang
// Last Modified: JB 2014/02/17, DLadder, UpdateConfig, PlaneToLadder
// Last Modified: LC 2014/12/05, UpdateAlignment() + AlignLadder 
// Last Modified: LC 2014/12/08, MonteCarlo Plots, UpdateAlignment(DR3)
// Last Modified: LC 2014/12/08, UpdateAlignment() (For ladder : Plume, Simple, Salat)
// Last Modified: LC 2014/12/08, MakeMiniVectors(), MakeTrack()
// Last Modified: LC 2014/12/18, Big code cleaning. Now custom by default.
// Last Modified: LC 2014/12/20, New MakeMiniVector() method.
// Last Modified: LC 2014/12/20, Now we use MiniVector Objects.

/////////////////////////////////////////////////////////////
//                                                         //
//  Class that handles a group of planes (or sensors).
//  
//  The Ladder class supersedes only the positions/tilts
//    of its associated planes.
//
//  OUTDATED : There are some predefined ladder types:
//    - Simple : ?
//    - Plume  : 6 MIMOSA-26 sensors in a raw, 
//               with same orientation
//    - Salat  : 4 MIMOSA-28 sensors arranged in a specific way
//               see ...
//
//  For predefined types, the inter
//
//  You may define your own geometry with the "Custom" type
//  
//  UPTODATE : "Custom" / "custom" by default. (2014/12/18)
//  UPTODATE : Using DprecAlign mecanism. 
//             Local transformations are removed.
//
//  The class was created by Loic Cousin.
//                                                         //
//                                                         //   
/////////////////////////////////////////////////////////////



//______________________________________________________________________________
//  
DLadder::DLadder() 
{
  fDebugLadder = 0;
}

//______________________________________________________________________________
//  
DLadder::DLadder( DTracker& aTracker, TString ladderName, Int_t aLadderID, Int_t aStatus, Double_t sensorLengthX, Double_t sensorSeparator, Double_t distanceFromFoam, Double_t rotationX, Double_t rotationY, Double_t rotationZ ) 
{

  //
  //
  // LC 2013-2014 
  // Modified: JB 2014/01/10, introduced "Custom" type
  // Modified: LC 2014/12/xx, Use custom type as default.
 
  
  fTracker = &aTracker;
  fAcq     = fTracker->GetAcq();        // set pointer to Data Acquisition
  fc       = fTracker->GetSetup();      // set pointer to Configuration
  fSession = fc->GetSession();          // set pointer to Session
  fDebugLadder = fc->GetDebug();

  ladderID = aLadderID;
  Status = aStatus;
  ladderType = ladderName;

  separatorLengthU = sensorSeparator;

  fIfAssociated = 0;

  //-+-+-+   Position of the Ladder

  // initial tilt about the axis perpendicular to the plane (w-axis)
  fTilt = DR3(fc->GetLadderPar(ladderID-1).Tilt);
  if (fDebugLadder)   {printf("  DLadder %2d,  Intitial Tilt around z, y, x axis [rad]  : ",ladderID);  fTilt.Print();}

  // initial position in xyz coordinates
  fPosition = DR3(fc->GetLadderPar(ladderID-1).Position);
  if (fDebugLadder) {printf("  DLadder %2d,  Initial Position(in xyz) [um]: ", ladderID); fPosition.Print();}

  // Initial alignment parameters
  Int_t DPrecAlignMethod = fc->GetTrackerPar().DPrecAlignMethod;   // LC 2015/01/31 
  fPrecAlign = new DPrecAlign(DPrecAlignMethod);                   // LC 2015/01/31

  fPrecAlign->SetTranslation( fPosition ); // BEWARE, translation to be set first, always!
  fPrecAlign->SetRotations( fTilt );       // because SetRot method compute the plane
  if (fDebugLadder) fPrecAlign->PrintAll();


  std::cout<<"//*******************************************************//"<<std::endl;
  std::cout<<"// Ladder "<<ladderID<<" Position : X = "<<fPosition(0)<<" Y = "<<fPosition(1)<<" Z = "<<fPosition(2)<<" //"<<std::endl;
  std::cout<<"// Ladder "<<ladderID<<" Rotation : X = "<<fTilt(2)<<"  Y = "<<fTilt(1)<<"  Z = "<<fTilt(0)<<" //"<<std::endl;
  std::cout<<"//*******************************************************//"<<std::endl;
 
  if(ladderName=="Custom" || ladderName=="custom") {
     
    for ( Int_t iplane=1; iplane<=fc->GetLadderPar(ladderID-1).Planes; iplane++) { // loop on planes associated to ladder
            
      planeRelativePosition[iplane](0) = fc->GetLadderPar(ladderID-1).PlaneShift[iplane-1](0);
      planeRelativePosition[iplane](1) = fc->GetLadderPar(ladderID-1).PlaneShift[iplane-1](1);
      planeRelativePosition[iplane](2) = fc->GetLadderPar(ladderID-1).PlaneShift[iplane-1](2);

      planeRelativeRotation[iplane](2) = fc->GetLadderPar(ladderID-1).PlaneTilt[iplane-1](0);
      planeRelativeRotation[iplane](1) = fc->GetLadderPar(ladderID-1).PlaneTilt[iplane-1](1);
      planeRelativeRotation[iplane](0) = fc->GetLadderPar(ladderID-1).PlaneTilt[iplane-1](2);

      planeRelativePrecAlign[iplane] = new DPrecAlign(DPrecAlignMethod);  // LC 2015/01/31
      
      planeRelativePrecAlign[iplane]->SetTranslation( planeRelativePosition[iplane]);
      planeRelativePrecAlign[iplane]->SetRotations( planeRelativeRotation[iplane]);
    
    } // end loop on planes
    
  } // end if Custom type

  TString hDU_Name = "fhDX" + TString(ladderID);
  fhDU = new TH1F(hDU_Name,"Residu in X",500,-100.,+100.);
  
  TString hDV_Name = "fhDY" + TString(ladderID);
  fhDV = new TH1F(hDV_Name,"Residu in Y",500,-100.,+100.);
  
  TString hDW_Name = "fhDZ" + TString(ladderID);
  fhDW = new TH1F(hDW_Name,"Residu in Z",1000,-100.,+100.); 

  TString hDU_U_Name = "fhDX_X" + TString(ladderID);
  fhDU_U = new TH2F(hDU_U_Name,"Residu in X vs X",500,-10000.,+10000.,200,-1000.,1000.);
  
  TString hDV_V_Name = "fhDY_Y" + TString(ladderID);
  fhDV_V = new TH2F(hDV_V_Name,"Residu in Y vs Y",500,-5000.,+5000.,200,-1000.,1000.);

  TString hDU_V_Name = "fhDX_Y" + TString(ladderID);
  fhDU_V = new TH2F(hDU_V_Name,"Residu in X vs Y",500,-5000.,+5000.,200,-1000.,1000.);

  TString hDV_U_Name = "fhDY_X" + TString(ladderID);
  fhDV_U = new TH2F(hDV_U_Name,"Residu in Y vs X",500,-10000.,+10000.,200,-1000.,1000.);

  TString hDW_U_Name = "fhDZ_X" + TString(ladderID);
  fhDW_U = new TH2F(hDW_U_Name,"Residu in Z vs X",500,-10000.,+10000.,200,-1000.,1000.);

  TString hDW_V_Name = "fhDZ_Y" + TString(ladderID);
  fhDW_V = new TH2F(hDW_V_Name,"Residu in Z vs Y",500,-10000.,+10000.,200,-1000.,1000.);

  // TString hDU_W_Name = "fhDX_Z" + TString(ladderID);
  // fhDU_W = new TH2F(hDU_W_Name,"Residu in X vs Z",500,-10000.,+10000.,200,-1000.,1000.);

  // TString hDV_W_Name = "fhDY_Z" + TString(ladderID);
  // fhDV_W = new TH2F(hDV_W_Name,"Residu in Y vs Z",500,-10000.,+10000.,200,-1000.,1000.);

  // TString hDW_W_Name = "fhDZ_Z" + TString(ladderID);
  // fhDW_W = new TH2F(hDW_W_Name,"Residu in Z vs Z",500,-10000.,+10000.,200,-1000.,1000.);

  TString hSlopeX_Name = "fhSlopeX" + TString(ladderID);
  fhSlopeX = new TH1F(hSlopeX_Name,"SlopeX",2000,-1.,+1.);
  
  TString hSlopeY_Name = "fhSlopeY" + TString(ladderID);
  fhSlopeY = new TH1F(hSlopeY_Name,"SlopeY",2000,-1.,+1.);

  TString hDiffSlopeX_Name = "fhDiffSlopeX" + TString(ladderID);
  fhDiffSlopeX = new TH1F(hDiffSlopeX_Name,"Error Slope X",200,-1.,+1.);

  TString hResMC_U_Name = "fResMC_U" + TString(ladderID);
  fResMC_U = new TH1F(hResMC_U_Name,"Residu MC in U",4000,-100.,+100.);

  TString hResMC_V_Name = "fResMC_V" + TString(ladderID);
  fResMC_V = new TH1F(hResMC_V_Name,"Residu MC in V",4000,-100.,+100.);

  TString hResMC_W_Name = "fResMC_W" + TString(ladderID);
  fResMC_W = new TH1F(hResMC_W_Name,"Residu MC in W",4000,-100.,+100.);

}

DLadder::~DLadder()
{
   delete fhDU;
   delete fhDV;
   delete fhDU_U;
   delete fhDV_V;
   delete fhDU_V;
   delete fhDV_U;
   delete fhDW_U;
   delete fhDW_V;
   // delete fhDU_W;
   // delete fhDV_W;
   // delete fhDW_W;
   delete fhSlopeX;
   delete fhSlopeY; 
   delete fhDiffSlopeX;
   delete myProfileX;
   delete myProfileY;
   delete fResMC_U;
   delete fResMC_V;
   delete fResMC_W;

   delete fPrecAlign;
}


//______________________________________________________________________________
//  
void DLadder::UpdateAlignment()
{
  
  // Update the positions (tilts and translations) of all planes within the ladder.
  //
  // LC
  // Modified: JB 2014/02/17 Use ConvoluteAlignment for Custom case
  // Modified: LC 2014/12/05 Custom Case + remove old code (new code for PLUME ladders arrive soon ...)
  // Modified: LC End of December 2014 : Cleaning code + Custom by default
  
  if (fDebugLadder) {
    printf("  DLadder %2d,  updating alignment for %d planes.\n",ladderID,GetNumberOfPlanes());
  }

  fTilt(0) = (Double_t)fPrecAlign->GetRotations()[2];
  fTilt(1) = (Double_t)fPrecAlign->GetRotations()[1];
  fTilt(2) = (Double_t)fPrecAlign->GetRotations()[0];
  
  fPosition(0) = (Double_t)fPrecAlign->GetTranslation()[0];
  fPosition(1) = (Double_t)fPrecAlign->GetTranslation()[1];
  fPosition(2) = (Double_t)fPrecAlign->GetTranslation()[2];
 
  if(ladderType=="Custom" || ladderType=="custom") {  // for Custom case, JB 2014/02/17    
   
    Int_t j = GetFirstPlane();
    for(int i=1 ; i<=GetNumberOfPlanes() ; ++i) {
      // Set the alignment relative to the ladder
      planeList[j]->GetPrecAlignment()->CopyAlignment( planeRelativePrecAlign[i] );
      
      if( fDebugLadder ) {
        cout << " DLadder::UpdateAlignement: Plane alignement before convolution" << endl;
        planeList[i]->GetPrecAlignment()->PrintAll();                                        
      }
      
      // Convolute the relative alignment with the ladder alignment
      planeList[j]->GetPrecAlignment()->ConvoluteAlignment( fPrecAlign );
      //planeList[j]->GetPrecAlignment()->PrintRotationMatrix();
      //planeList[j]->GetPrecAlignment()->PrintTorMatrix();
      //planeList[j]->GetPrecAlignment()->PrintRotTimeInv();

      planeList[j]->UpdatePositions();      
      ++j;

    } // end for
  } // end if custom
  else std::cout<<" No Custom ladder found."<<std::endl;
}

//______________________________________________________________________________
//  
void DLadder::GetRelativePosition( Int_t aPlaneNumber, DR3* aTilt, DR3* aShift)
{
  
  // Set the provided tilts and translations pointers to the relative positions 
  // of the given planes with respect to the ladder.
  // Note aPlaneNumber is the relative number within the ladder (from 1)!
  //
  // JB 2014/02/10
  
  if( aPlaneNumber<0 && aPlaneNumber<GetNumberOfPlanes() ) {
    std::cout << "WARNING there are only " << GetNumberOfPlanes() << " planes in ladder " << ladderID << "(" << GetLadderType().Data() << ")!" << std::endl; 
    return;
  }
  
  if(ladderType=="Custom" || ladderType=="custom") {
    
    aTilt->SetValue( (Double_t*)(planeRelativePrecAlign[aPlaneNumber]->GetRotations()) );
    aShift->SetValue( (Double_t*)(planeRelativePrecAlign[aPlaneNumber]->GetTranslation()) );
    
  } 
  
  printf( " Relative position of plane %d (absolute #=%d) wrt ladder %d:\n", aPlaneNumber, planeList[aPlaneNumber]->GetPlaneNumber(), ladderID);
  printf( "   Tilt: /z=%.2f, /y=%.2f, /x=%.2f\n", (*aTilt)(0), (*aTilt)(1), (*aTilt)(2));
  printf( "   Shift: z=%.2f, y=%.2f, x=%.2f\n", (*aShift)(0), (*aShift)(1), (*aShift)(2));

}

//______________________________________________________________________________
//  
DR3 DLadder::PlaneToLadder(DR3& aPlaneCoordinate, Int_t aPlaneNumber){
  
  // converts a point given in the Plane coordinate frame UVW
  // to the ladder coordinate frame, using the relative alignment.
  //
  // JB 2014/02/17
  
  return planeRelativePrecAlign[aPlaneNumber]->TransformHitToTracker( aPlaneCoordinate);
  
}


//______________________________________________________________________________
//  
void DLadder::AddPlane(DPlane* aPlane, Int_t planeID)
{
  std::cout<<"//*******************************************************//"<<std::endl;
  std::cout<<"// Ladder "<<ladderID<< ": adding plane "<<planeID<<" with tracker index "<<aPlane->GetPlaneNumber()<<" //"<<std::endl;
  planeList[aPlane->GetPlaneNumber()] = aPlane;

  //planeRotation[planeID] = DR3( aPlane->GetPrecAlignment()->GetRotations()[2],
  //                             aPlane->GetPrecAlignment()->GetRotations()[1],
  //                              aPlane->GetPrecAlignment()->GetRotations()[0] );
  
  std::cout<<"// Translations : X = "<<aPlane->GetPrecAlignment()->GetTranslation()[0]
           <<" Y = "<<aPlane->GetPrecAlignment()->GetTranslation()[1]<<" Z = "
           <<aPlane->GetPrecAlignment()->GetTranslation()[2]<<" //"<<std::endl;
        
  std::cout<<"// Rotations    : X = "<<aPlane->GetPrecAlignment()->GetRotations()[2]
           <<" Y = "<<aPlane->GetPrecAlignment()->GetRotations()[1]<<" Z = "
           <<aPlane->GetPrecAlignment()->GetRotations()[0]<<" //"<<std::endl;

  std::cout<<"//*******************************************************//"<<std::endl;

}

//______________________________________________________________________________
//  
void DLadder::MakeMiniVectors()
{
 // LC 2012.
 // Mini-vectors reconstuction. 
 // Only for PLUME ladders (or double sided ladders)
 // Each hit on the first face is associated to the closest hit in the second face. 
 // LC 2014/12/17 : Cleaning code.
 // LC 2014/12/19 : Standalone method MakeMiniVectors(planeNumber) --> MakeMiniVectors().
 // LC 2014/12/19 : Make all MVs per event and save them in _miniVectors container.
 // LC 2014/12/19 : New class MiniVector. Mini-vectors are now saved in DLadder (See : _miniVectors)
 // LC 2015/03/07 : Fill MonteCarlo MiniVector objects.

  ResetMiniVectors();

  DPlane* myPlaneLeft = NULL;
   
  for( Int_t planeNumber=GetFirstPlane() ; planeNumber<(GetFirstPlane()+GetNumberOfPlanes()/2) ; ++planeNumber ) {  
    
    Double_t distance = 0;

    myPlaneLeft = planeList[planeNumber];
    DPrecAlign* myPrecAlignLeft  = myPlaneLeft->GetPrecAlignment();

    if( myPlaneLeft->GetHitsN()==0 ) continue; 

    for(Int_t iHitLeft=1 ; iHitLeft<=myPlaneLeft->GetHitsN() ; ++iHitLeft) {

      DHit* myMiniVectorHitLeft = myPlaneLeft->GetHit(iHitLeft);

      DR3 positionLeft = myPrecAlignLeft->TransformHitToTracker( DR3(myMiniVectorHitLeft->GetPositionUhitCG(), myMiniVectorHitLeft->GetPositionVhitCG(),  0. /*myMiniVectorHitLeft->GetPositionWhitCG()*/) );
      //myPrecAlignLeft->PrintAll();
      //positionLeft.Print();
      //myMiniVectorHitLeft->GetMonteCarloPosition().Print();
      //DR3(myMiniVectorHitLeft->GetPositionUhitCG(), myMiniVectorHitLeft->GetPositionVhitCG(),  0.).Print();
      //positionLeft.Print();

      std::map<Double_t, DR3> rightPositions;
      std::map<Double_t, std::pair<DHit*, Int_t> > mapHitsRight;

      for(Int_t i=(GetFirstPlane()+GetNumberOfPlanes()/2) ; i<(GetFirstPlane()+GetNumberOfPlanes()) ; ++i) {
        
        DPlane* myPlaneRight = planeList[i];
        
        if( myPlaneRight->GetHitsN()==0 ) continue; 
 
        for( Int_t iHitRight=1 ; iHitRight<=myPlaneRight->GetHitsN() ; ++iHitRight) {
       
          DPrecAlign* myPrecAlignRight =  myPlaneRight->GetPrecAlignment();
          
          DHit* myHitRight = myPlaneRight->GetHit(iHitRight);
          
          DR3 positionRight = myPrecAlignRight->TransformHitToTracker( DR3(myHitRight->GetPositionUhitCG(), myHitRight->GetPositionVhitCG(), 0./*myHitRight->GetPositionWhitCG()*/) );
          //DR3(myHitRight->GetPositionUhitCG(), myHitRight->GetPositionVhitCG(), 0./*myHitRight->GetPositionWhitCG()*/).Print();

          // Compute mini-vector length
          distance = sqrt( (positionRight(2)-positionLeft(2))*(positionRight(2)-positionLeft(2)) + (positionRight(1)-positionLeft(1))*(positionRight(1)-positionLeft(1)) + (positionRight(0)-positionLeft(0))*(positionRight(0)-positionLeft(0)) );
       
          // Filling in a map to order per distance (first element = closest) 
          std::pair<DHit*,Int_t> paireHitRight(myHitRight,i);
          mapHitsRight[distance] = paireHitRight;
          rightPositions[distance] = positionRight;
        } // end for hits right
    
      } // end loop on right planes

      DR3 positionRight(0.,0.,0.); 
      DHit* myMiniVectorHitRight=NULL;
      Int_t planeRightIndex;

      DR3 planeFrameLeftPosition(0.,0.,0.);
      DR3 planeFrameRightPosition(0.,0.,0.);

      DR3 trackerFrameLeftPositionMC(0., 0., 0.);
      DR3 trackerFrameRightPositionMC(0., 0., 0.);
      DR3 planeFrameLeftPositionMC(0., 0., 0.);
      DR3 planeFrameRightPositionMC(0., 0., 0.);

      if(mapHitsRight.size()!=0) {
        myMiniVectorHitRight = mapHitsRight.begin()->second.first;
        positionRight = rightPositions.begin()->second;
        planeRightIndex = mapHitsRight.begin()->second.second;
        myMiniVectorHitRight = mapHitsRight.begin()->second.first;

        planeFrameLeftPosition =  DR3(myMiniVectorHitLeft->GetPositionUhitCG(), myMiniVectorHitLeft->GetPositionVhitCG(),  0. );
        planeFrameRightPosition = DR3(myMiniVectorHitRight->GetPositionUhitCG(), myMiniVectorHitRight->GetPositionVhitCG(), 0. );
    
        if( myMiniVectorHitRight->IfMonteCarlo() == 1 && myMiniVectorHitLeft->IfMonteCarlo() == 1 ) {
          trackerFrameLeftPositionMC = myMiniVectorHitLeft->GetMonteCarloPosition();
          trackerFrameRightPositionMC = myMiniVectorHitRight->GetMonteCarloPosition();
          planeFrameLeftPositionMC  = planeList[planeNumber]->GetPrecAlignment()->TransformHitToPlane(trackerFrameLeftPositionMC); 
          planeFrameRightPositionMC = planeList[planeRightIndex]->GetPrecAlignment()->TransformHitToPlane(trackerFrameRightPositionMC);
       
        }

      }

      if(positionRight(2) != positionLeft(2) && mapHitsRight.size()!=0 ) {
  
        DR3 center(0.,0.,0.);
   
        center(0) = ( positionRight(0)+positionLeft(0) )/2.;
        center(1) = ( positionRight(1)+positionLeft(1) )/2.;
        center(2) = ( positionRight(2)+positionLeft(2) )/2.; 

        Int_t pixelCut = 3;

        Int_t colLeft = planeList[planeNumber]->GetStripsNu();
        Int_t rowLeft = planeList[planeNumber]->GetStripsNv();
        DR3 pitchLeft = planeList[planeNumber]->GetStripPitch();

        Int_t colRight = planeList[planeRightIndex]->GetStripsNu();
        Int_t rowRight = planeList[planeRightIndex]->GetStripsNv();
        DR3 pitchRight = planeList[planeRightIndex]->GetStripPitch();

        Double_t limitLeftU = colLeft*pitchLeft(0)/2. - pixelCut*pitchLeft(0);
        Double_t limitLeftV = rowLeft*pitchLeft(1)/2. - pixelCut*pitchLeft(1);

        Double_t limitRightU = colRight*pitchRight(0)/2. - pixelCut*pitchRight(0);
        Double_t limitRightV = rowRight*pitchRight(1)/2. - pixelCut*pitchRight(1);

        DR3 direction = ComputeDirection(positionLeft, positionRight);
        DR3 directionMC = ComputeDirection(trackerFrameLeftPositionMC, trackerFrameRightPositionMC);
        DR3 centerMC = (trackerFrameLeftPositionMC+trackerFrameRightPositionMC)/2.;
       
        centerMC.Print();
       /*
        positionLeft.Print();
        positionRight.Print();
       */

        // Cut for keep cluster of 5*5
        if( TMath::Abs(planeFrameLeftPosition(0))<=limitLeftU && TMath::Abs(planeFrameRightPosition(0))<=limitRightU && TMath::Abs(planeFrameLeftPosition(1))<=limitLeftV && TMath::Abs(planeFrameRightPosition(1))<=limitRightV ) {
          
          MiniVector* myNewMiniVector = new MiniVector(fPrecAlign, positionLeft, positionRight, planeList[planeNumber]->GetPrecAlignment(), planeList[planeRightIndex]->GetPrecAlignment(), center, direction, planeFrameLeftPosition, planeFrameRightPosition);
  
          if( myMiniVectorHitRight->IfMonteCarlo() == 1 && myMiniVectorHitLeft->IfMonteCarlo() == 1 ) {    
            myNewMiniVector->SetMonteCarloMiniVector( trackerFrameLeftPositionMC, trackerFrameRightPositionMC, centerMC, directionMC, planeFrameLeftPositionMC, planeFrameRightPositionMC );
          }

          AddMiniVector(myNewMiniVector);
        }
        
        //std::cout<<" Number of reconstructed mini-vectors = "<<GetMiniVectorsSize(); 
    
      } // end if

    }  // end loop on left hits (for the planeNumber plane left )

  } // end loop on left planes

}

DR3 DLadder::ComputeDirection(DR3& positionLeft, DR3& positionRight)
{
   
  Double_t x0 = positionLeft(0);
  Double_t y0 = positionLeft(1);
  Double_t z0 = positionLeft(2);

  Double_t x1 = positionRight(0);
  Double_t y1 = positionRight(1);
  Double_t z1 = positionRight(2);

 //-----------------------------//
 //       x = az + b            //
 //       y = cz + d            //
 //-----------------------------//
  
  DR3 tLineFitSlope(0.,0.,1.);   // Slopes in the tracker system

  if( z1-z0 != 0. ) {
    tLineFitSlope(0)  = (x1-x0)/(z1-z0);
    tLineFitSlope(1)  = (y1-y0)/(z1-z0);
  }
  else {
    tLineFitSlope(0)  = 0.;
    tLineFitSlope(1)  = 0.;      
  }

  return tLineFitSlope;
}

//______________________________________________________________________________
//  
void DLadder::AlignLadder( std::vector<Double_t>& DataTemp, const Int_t ladderNumber, const Int_t planeNumber )
{

   //  LC 2013/01/09 : New method to fit with Minuit.
   //  This method add data before the fit with Minuit.
   //  Modified: LC 2014/12/05 : Custom ladders + clean old code
     // In DPlane we define :
     /*
     myData[0] -->  GetHit(k)->GetPositionUhitCG()
     myData[1] -->  GetHit(k)->GetPositionVhitCG()
     myData[2] -->  aTrack->GetLinearFit().GetOrigin()(0)
     myData[3] -->  aTrack->GetLinearFit().GetOrigin()(1)
     myData[4] -->  aTrack->GetLinearFit().GetOrigin()(2)
     myData[5] -->  aTrack->GetLinearFit().GetSlopeZ()(0)
     myData[6] -->  aTrack->GetLinearFit().GetSlopeZ()(1)
     myData[7] -->  tDistanceU
     myData[8] -->  tDistanceV
     myData[9] -->  tDist2D
     */

    Int_t size = DataTemp.size();
    Int_t longueur = 15;
    for(Int_t i=0; i<size/longueur ; ++i) {
   
      DPrecAlign* planeDPrecAlign = NULL;
      if(GetLadderType()=="Custom" || GetLadderType()=="custom") planeDPrecAlign = planeList[planeNumber]->GetPrecAlignment();
    
      planeDPrecAlign->NewData(DataTemp[i*longueur+0],  DataTemp[i*longueur+1],
			       DataTemp[i*longueur+13], DataTemp[i*longueur+14],
			       DataTemp[i*longueur+2],  DataTemp[i*longueur+3], DataTemp[i*longueur+4], DataTemp[i*longueur+5], DataTemp[i*longueur+6]);
      
    }
}


//______________________________________________________________________________
// 
/*
void DLadder::FillResidus( std::vector<Double_t>& Data, const Int_t planeNumber, DR3& dxdydz )
{
  fhDU->Fill(Data[7]); // residu DU. 10 -> CoG U 2x2 // 7 -> DU CoG
  fhDV->Fill(Data[8]); // residu DV. 11 -> CoG V 2x2 // 8 -> DV CoG
  fhDW->Fill(Data[12]); // 12 -> DW CoG

  fhDU_U->Fill(Data[0],Data[7]);  // DU vs U 
  fhDV_V->Fill(Data[1],Data[8]);  // DV vs V

  fhDU_V->Fill(Data[0],Data[8]);  // DU vs V
  fhDV_U->Fill(Data[1],Data[7]);  // DU vs U

  fhSlopeX->Fill(Data[5]);        // SlopeX
  fhSlopeY->Fill(Data[6]);        // SlopeY

  Double_t angleMC = 2.0;
  Double_t angle = 180./TMath::Pi()*TMath::ATan( TMath::Sqrt(dxdydz(0)*dxdydz(0)+dxdydz(1)*dxdydz(1)) / dxdydz(2) );
  fhDiffSlopeX->Fill( (angle-angleMC)/angleMC );

}
*/
//______________________________________________________________________________
//  
void DLadder::FillResidus(std::vector<Double_t>& Data, const Int_t planeNumber)
{
  fhDU->Fill(Data[10]); // residu DX. 10 -> CoG U 2x2 // 7 -> DU CoG
  fhDV->Fill(Data[11]); // residu DY. 11 -> CoG V 2x2 // 8 -> DV CoG
  fhDW->Fill(Data[12]); // 12 -> DZ CoG

  fhDU_U->Fill(Data[7],Data[10]);  // DX vs X 
  fhDV_V->Fill(Data[8],Data[11]);  // DY vs Y

  fhDU_V->Fill(Data[8],Data[10]);  // DX vs Y
  fhDV_U->Fill(Data[7],Data[11]);  // DY vs X

  fhDW_U->Fill(Data[7],Data[12]);  // DZ vs X
  fhDW_V->Fill(Data[8],Data[12]);  // DZ vs Y

  // fhDW_W->Fill(Data[9],Data[12]);  // DZ vs Z
  // fhDU_W->Fill(Data[9],Data[10]);  // DX vs Z
  // fhDV_W->Fill(Data[9],Data[11]);  // DY vs Z

  //  fhSlopeX->Fill(Data[5]);        // SlopeX
  //  fhSlopeY->Fill(Data[6]);        // SlopeY

  //Double_t angleMC = 10.0;
  //  fhDiffSlopeX->Fill( 0. );

}

void DLadder::FillResidusMC(std::vector<Double_t>& Res, const Int_t planeNumber)
{
  fResMC_U->Fill(Res[0]); // Residus MC in U.
  fResMC_V->Fill(Res[1]); // Residus MC in V.
  fResMC_W->Fill(Res[2]); // Residus MC in W.
}

//______________________________________________________________________________
//  
void DLadder::PrepareDisplay()
{
  TString name = "Residus ladder ";
  name += ladderID;
  TString canvasName = "Canvas";
  canvasName += ladderID;

  fCanvas = new TCanvas(canvasName, name, 800, 600);
  fCanvas->Divide(3,4);
/*
  TString nameMC = "Residus MC ladder ";
  nameMC += ladderID;
  TString canvasNameMC = "CanvasMC";
  canvasNameMC += ladderID;

  fCanvasMC = new TCanvas(canvasNameMC, nameMC, 800, 600);
  fCanvasMC->Divide(1,3);
*/
}

//______________________________________________________________________________
//  
void DLadder::Display(Int_t runNumber)
{
  std::cout<<"Display"<<std::endl;

  TString fileNameResDU = "residu_DX_Ladder_"; 
  fileNameResDU += ladderID;
  fileNameResDU += "_";
  fileNameResDU += runNumber;
  fileNameResDU += ".root";
 
  TString fileNameResDV = "residu_DY_Ladder_";
  fileNameResDV += ladderID;
  fileNameResDV += "_";
  fileNameResDV += runNumber;
  fileNameResDV += ".root";

  TString fileNameResDW = "residu_DZ_Ladder_";
  fileNameResDW += ladderID;
  fileNameResDW += "_";
  fileNameResDW += runNumber;
  fileNameResDW += ".root";

  TString fileNameDiffSlope = "DiffSlope_Ladder_";
  fileNameDiffSlope += ladderID;
  fileNameDiffSlope += "_";
  fileNameDiffSlope += runNumber;
  fileNameDiffSlope += ".root";

  if ( fhDU && fhDU->GetEntries()>0) { 
    fCanvas->cd(1); 
    fhDU->Draw();
    fhDU->Fit("gaus");
    gStyle->SetOptFit(111);
    fhDU->SaveAs(fileNameResDU);
    //fhDU->SaveAs(fileNameResDU);
    fCanvas->cd(4);
    fhDU_U->SetMarkerColor(kRed);
    fhDU_U->Draw();
    
    fCanvas->cd(7);
    fhDU_V->SetMarkerColor(kGreen);
    fhDU_V->Draw();

    // fCanvas->cd(10);
    // fhDU_W->SetMarkerColor(kBlue);
    // fhDU_W->Draw();
  }

  if ( fhDV && fhDV->GetEntries()>0) {
    fCanvas->cd(2); 
    fhDV->Draw();
    fhDV->Fit("gaus");
    gStyle->SetOptFit(111);
    fhDV->SaveAs(fileNameResDV);
    //fhDV->SaveAs(fileNameResDV);
    fCanvas->cd(5);
    fhDV_U->SetMarkerColor(kRed);
    fhDV_U->Draw();

    fCanvas->cd(8);
    fhDV_V->SetMarkerColor(kGreen);
    fhDV_V->Draw();

    // fCanvas->cd(11);
    // fhDV_W->SetMarkerColor(kBlue);
    // fhDV_W->Draw();
  }

  if ( fhDW && fhDW->GetEntries()>0) { 
    fCanvas->cd(3); 
    fhDW->Draw();
    fhDW->Fit("gaus");
    gStyle->SetOptFit(111);
    fhDW->SaveAs(fileNameResDW);

    fCanvas->cd(6);
    fhDW_U->SetMarkerColor(kRed);
    fhDW_U->Draw();

    fCanvas->cd(9);
    fhDW_V->SetMarkerColor(kGreen);
    fhDW_V->Draw();

    // fCanvas->cd(12);
    // fhDW_W->SetMarkerColor(kBlue);
    // fhDW_W->Draw();
  }
  // if( fhSlopeX && fhSlopeX->GetEntries()>0 ) {
  //   fCanvas->cd(8);
  //   fhSlopeX->Draw();
  // }

  // if( fhSlopeY && fhSlopeY->GetEntries()>0 ) {
  //   fCanvas->cd(9);
  //   fhSlopeY->Draw();
  // }

  // if( fhSlopeX && fhSlopeX->GetEntries()>0 ) {
  //   fCanvas->cd(10);
  //   gPad->SetLogy();
  //   fhDiffSlopeX->Draw();
  //   fhDiffSlopeX->SaveAs(fileNameDiffSlope);
  // }
  
  if( fhDV && fhDV->GetEntries()>0 ) {
    fCanvas->cd(10);
    myProfileX = fhDV_V->ProfileX("myProfileX", -5000, 5000);
    myProfileX->Draw();
    fCanvas->cd(11);
    myProfileY = fhDV_U->ProfileX("myProfileY");
    myProfileY->Draw();
  }
/* 
  TString fileNameResMC_DU = "residuMC_DU_Ladder_"; 
  fileNameResMC_DU += ladderID;
  fileNameResMC_DU += "_";
  fileNameResMC_DU += runNumber;
  fileNameResMC_DU += ".root";
 
  TString fileNameResMC_DV = "residuMC_DV_Ladder_";
  fileNameResMC_DV += ladderID;
  fileNameResMC_DV += "_";
  fileNameResMC_DV += runNumber;
  fileNameResMC_DV += ".root";

  TString fileNameResMC_DW = "residuMC_DW_Ladder_";
  fileNameResMC_DW += ladderID;
  fileNameResMC_DW += "_";
  fileNameResMC_DW += runNumber;
  fileNameResMC_DW += ".root"; 
  
  if( fResMC_U && fResMC_U->GetEntries()>0 ) {
    fCanvasMC->cd(1);
    fResMC_U->Draw();
    fResMC_U->Fit("gaus");
    gStyle->SetOptFit(111);
    fResMC_U->SaveAs(fileNameResMC_DU);

  }
 
  if( fResMC_V && fResMC_V->GetEntries()>0 ) {
    fCanvasMC->cd(2);
    fResMC_V->Draw();
    fResMC_V->Fit("gaus");
    gStyle->SetOptFit(111);
    fResMC_V->SaveAs(fileNameResMC_DV);
  }

  if( fResMC_W && fResMC_W->GetEntries()>0 ) {
    fCanvasMC->cd(3);
    fResMC_W->Draw();
    fResMC_W->Fit("gaus");
    gStyle->SetOptFit(111);
    fResMC_W->SaveAs(fileNameResMC_DW);
  }
*/
}

//______________________________________________________________________________
//
void DLadder::AddMiniVector(MiniVector* myNewMiniVector)
{

  _miniVectors.push_back(myNewMiniVector);

}

//______________________________________________________________________________
//
void DLadder::RemoveLastMiniVector()
{ 

  std::vector<MiniVector*>::iterator iter = _miniVectors.end();
  delete *iter;
  _miniVectors.pop_back();

}

//______________________________________________________________________________
//
void DLadder::ResetMiniVectors()
{
  std::vector<MiniVector*>::iterator iter = _miniVectors.begin();
  for( ; iter!=_miniVectors.end() ; ++iter ) delete *iter;
  _miniVectors.clear();
}

//______________________________________________________________________________
//
void DLadder::ResetAssociatedMiniVectors()
{
  std::vector<MiniVector*>::iterator iter = _associatedMiniVectors.begin();
  for( ; iter!=_associatedMiniVectors.end() ; ++iter ) delete *iter;
  _associatedMiniVectors.clear();
}

//______________________________________________________________________________
//
void DLadder::ResetMiniVectorsList(std::vector<Int_t> listOfDelete )
{
  std::vector<Int_t>::iterator it = listOfDelete.begin();
  for( ; it!=listOfDelete.end() ; ++it) {
    delete _associatedMiniVectors[*it];
    _associatedMiniVectors.erase(_associatedMiniVectors.begin()+*it);
  }
}

//______________________________________________________________________________
//
void DLadder::AddAssociatedMiniVector(MiniVector* myAssociatedMV)
{ 
  MiniVector* associatedMiniVector = new MiniVector();
  *associatedMiniVector = *myAssociatedMV;
  _associatedMiniVectors.push_back(associatedMiniVector);  
}
