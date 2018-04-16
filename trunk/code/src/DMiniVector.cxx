//  Author   :  L. Cousin 2014/12/19
//  Save, recompute and project mini-vectors

#include<iostream>
#include "DMiniVector.h"

//______________________________________________________________________________
//
MiniVector::MiniVector(DPrecAlign* ladderDPrecAlign, DR3& hitLeftPosition, DR3& hitRightPosition, DPrecAlign* alignPlaneLeft, DPrecAlign* alignPlaneRight, DR3& centerPosition, DR3& direction, DR3 planeFrameLeft, DR3 planeFrameRight )
{
  // LC 2014/12/19.
  // Compute the MiniVector in Ladder frame.
  _ladderDPrecAlign = ladderDPrecAlign;
  _planesDPrecAlign.first  = alignPlaneLeft;
  _planesDPrecAlign.second = alignPlaneRight;
 
  // from tracker frame to plane/ladder frame. 
  DR3 leftPos   = alignPlaneLeft->TransformHitToPlane(hitLeftPosition);
  DR3 rightPos  = alignPlaneRight->TransformHitToPlane(hitRightPosition);
 
  //DR3 centerPos2 = (hitLeftPosition+hitRightPosition)/2.;
  DR3 centerPos = _ladderDPrecAlign->TransformHitToPlane(centerPosition);

  _planeFrameLeft  = planeFrameLeft;
  _planeFrameRight = planeFrameRight;

/*
  centerPosition.Print();
  centerPos2.Print();
  DR3 centerLadder = (leftPos+rightPos)/2.;
  centerLadder.Print();
  centerPos.Print();
*/

  for(Int_t i=0; i<3 ; ++i) {
  
    _hitLeftPosition.push_back( leftPos(i) );
    _hitRightPosition.push_back( rightPos(i) );
    _miniVectorCenterPosition.push_back( centerPos(i) );
    _miniVectorDirection.push_back( 0. );
    _miniVectorOrigin.push_back( 0. );
  }
 
  GetMiniVectorDirection();

  //std::cout<<" diff direction 0 = "<<_miniVectorDirection[0] - direction(0)<<std::endl;
  //std::cout<<" diff direction 1 = "<<_miniVectorDirection[1] - direction(1)<<std::endl;
  //std::cout<<" diff direction 2 = "<<_miniVectorDirection[2] - direction(2)<<std::endl;

  monteCarloMiniVector = 0;

}

//______________________________________________________________________________
//
void MiniVector::SetMonteCarloMiniVector(DR3& hitLeftPositionMC, DR3& hitRightPositionMC, DR3& centerPositionMC, DR3& directionMC, DR3 planeFrameLeftMC, DR3 planeFrameRightMC )
{
  // LC 2014/12/19.
  // Compute the MiniVector in Ladder frame.
 
  // from tracker frame to plane/ladder frame. 
  DR3 leftPosMC   = _planesDPrecAlign.first->TransformHitToPlane(hitLeftPositionMC);
  DR3 rightPosMC  = _planesDPrecAlign.second->TransformHitToPlane(hitRightPositionMC);
 
  //DR3 centerPos2 = (hitLeftPosition+hitRightPosition)/2.;
  DR3 centerPosMC = _ladderDPrecAlign->TransformHitToPlane(centerPositionMC);

  _planeFrameLeftMC  = planeFrameLeftMC;
  _planeFrameRightMC = planeFrameRightMC;

/*
  centerPosition.Print();
  centerPos2.Print();
  DR3 centerLadder = (leftPos+rightPos)/2.;
  centerLadder.Print();
  centerPos.Print();
*/

  for(Int_t i=0; i<3 ; ++i) {
  
    _hitLeftPositionMC.push_back( leftPosMC(i) );
    _hitRightPositionMC.push_back( rightPosMC(i) );
    _miniVectorCenterPositionMC.push_back( centerPosMC(i) );
    _miniVectorDirectionMC.push_back( 0. );
    _miniVectorOriginMC.push_back( 0. );
  }

  std::cout<<_miniVectorCenterPositionMC[0]<<std::endl;
 
  GetMiniVectorDirectionMC();

  //std::cout<<" diff direction 0 = "<<_miniVectorDirection[0] - direction(0)<<std::endl;
  //std::cout<<" diff direction 1 = "<<_miniVectorDirection[1] - direction(1)<<std::endl;
  //std::cout<<" diff direction 2 = "<<_miniVectorDirection[2] - direction(2)<<std::endl;

  monteCarloMiniVector = 1;

}



//______________________________________________________________________________
//
void MiniVector::SetHitLeft(DR3& hitLeftPosition)
{
  DR3 leftPos =  (_planesDPrecAlign.first)->TransformHitToPlane(hitLeftPosition);
  
  for(Int_t i=0; i<3 ; ++i) {
    _hitLeftPosition.push_back( leftPos(i) );
  }
}
//______________________________________________________________________________
//
void MiniVector::SetHitRight(DR3& hitRightPosition)
{
  DR3 rightPos = ( _planesDPrecAlign.second)->TransformHitToPlane(hitRightPosition);
  
  for(Int_t i=0; i<3 ; ++i) {
    _hitRightPosition.push_back( rightPos(i) );
  }
}
//______________________________________________________________________________
//
void MiniVector::SetCenter(DR3& centerPosition)
{
  DR3 centerPos = _ladderDPrecAlign->TransformHitToPlane(centerPosition);
  
  for(Int_t i=0; i<3 ; ++i) {
    _miniVectorCenterPosition.push_back( centerPos(i) );
  }
}

//______________________________________________________________________________
//
void MiniVector::SetDirection(DR3& direction)
{
  for(Int_t i=0; i<3 ; ++i) {
    _miniVectorDirection.push_back( direction(i) );
  }
}

//______________________________________________________________________________
//
void MiniVector::SetHitLeftMC(DR3& hitLeftPositionMC)
{
  DR3 leftPos =  (_planesDPrecAlign.first)->TransformHitToPlane(hitLeftPositionMC);
  
  for(Int_t i=0; i<3 ; ++i) {
    _hitLeftPositionMC.push_back( leftPos(i) );
  }

}

//______________________________________________________________________________
//
void MiniVector::SetHitRightMC(DR3& hitRightPositionMC)
{
  DR3 rightPos = ( _planesDPrecAlign.second)->TransformHitToPlane(hitRightPositionMC);
  
  for(Int_t i=0; i<3 ; ++i) {
    _hitRightPositionMC.push_back( rightPos(i) );
  }

}

//______________________________________________________________________________
//
void MiniVector::SetCenterMC(DR3& centerPositionMC)
{
  DR3 centerPos = _ladderDPrecAlign->TransformHitToPlane(centerPositionMC);
  
  for(Int_t i=0; i<3 ; ++i) {
    _miniVectorCenterPositionMC.push_back( centerPos(i) );
  }

}

//______________________________________________________________________________
//
void MiniVector::SetDirectionMC(DR3& directionMC)
{
  for(Int_t i=0; i<3 ; ++i) {
    _miniVectorDirectionMC.push_back( directionMC(i) );
  }

}

//______________________________________________________________________________
//
void MiniVector::PrintMiniVector()
{
  std::cout<<"//***************************//"<<std::endl;
  if( _hitRightPosition.size() != 0 ) std::cout<<"// hitRight=("<< _hitRightPosition[0]<<","<<_hitRightPosition[1]<<","<<_hitRightPosition[2]<<")      //"<<std::endl;
  else std::cout<<"// No mini-vector found.   //"<<std::endl;
  if( _hitLeftPosition.size() != 0 ) std::cout<<"//  hitLeft =("<< _hitLeftPosition[0]<<","<<_hitLeftPosition[1]<<","<<_hitLeftPosition[2]<<")      //"<<std::endl;
  if( _miniVectorCenterPosition.size() != 0 ) std::cout<<"// center  =("<< _miniVectorCenterPosition[0]<<","<<_miniVectorCenterPosition[1]<<","<<_miniVectorCenterPosition[2]<<")      //"<<std::endl;
  if( _miniVectorDirection.size() != 0 ) std::cout<<"// direction=("<< _miniVectorDirection[0]<<","<<_miniVectorDirection[1]<<","<<_miniVectorDirection[2]<<")      //"<<     std::endl;
  std::cout<<"//***************************//"<<std::endl;
}


//______________________________________________________________________________
//
DR3 MiniVector::GetHitLeftPlanePosition()
{
  return DR3( _hitLeftPosition[0], _hitLeftPosition[1], _hitLeftPosition[2] );
}

//______________________________________________________________________________
//
DR3 MiniVector::GetHitRightPlanePosition()
{
  return DR3( _hitRightPosition[0], _hitRightPosition[1], _hitRightPosition[2] );
}

//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorLadderCenter() 
{
  return DR3( _miniVectorCenterPosition[0], _miniVectorCenterPosition[1], _miniVectorCenterPosition[2] );
}

//______________________________________________________________________________
//
DR3 MiniVector::GetHitLeftTrackerPosition()
{
  // Recompute the hit position wrt the ladder alignment parameters.
  
  DR3 hitLeftLadderFrame = DR3( _hitLeftPosition[0], _hitLeftPosition[1], _hitLeftPosition[2] );
  DR3 hitLeftTrackerFrame = (_planesDPrecAlign.first)->TransformHitToTracker( hitLeftLadderFrame );

  return hitLeftTrackerFrame;
}

//______________________________________________________________________________
//
DR3 MiniVector::GetHitRightTrackerPosition()
{
  // Recompute the hit position wrt the ladder alignment parameters.
  DR3 hitRightLadderFrame = DR3( _hitRightPosition[0], _hitRightPosition[1], _hitRightPosition[2] );
  DR3 hitRightTrackerFrame = (_planesDPrecAlign.second)->TransformHitToTracker( hitRightLadderFrame );

  return hitRightTrackerFrame; 
}

//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorTrackerCenter()
{
  // Recompute the hit position wrt the ladder alignment parameters.
  
  DR3 leftTracker = GetHitLeftTrackerPosition();
  DR3 rightTracker = GetHitRightTrackerPosition();
  DR3 centerTracker = (rightTracker + leftTracker)/2.;


//  std::cout<<"cnterTracker/DPrecAlign Left/ DPrecAlignRight"<<std::endl;
//  centerTracker.Print();
/*
  (_planesDPrecAlign.first)->PrintAll();
  (_planesDPrecAlign.first)->PrintTorMatrix();
  (_planesDPrecAlign.second)->PrintAll();  
  (_planesDPrecAlign.second)->PrintTorMatrix();
*/
  DR3 centerLadderFrame = DR3( _miniVectorCenterPosition[0], _miniVectorCenterPosition[1], _miniVectorCenterPosition[2] );
  DR3 centerTrackerFrame = _ladderDPrecAlign->TransformHitToTracker( centerLadderFrame );
/*
  std::cout<<"cnterTracker = "<<std::endl;
  centerTrackerFrame.Print();
  centerTracker.Print();
*/
  return centerTracker/*Frame*/;
}

//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorOrigin()
{
  return DR3(_miniVectorOrigin[0], _miniVectorOrigin[1], _miniVectorOrigin[2] );
}


//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorDirection()
{
  // Recompute the mini-vector direction wrt the ladder alignment parameters.
  DR3 hitLeft  = GetHitLeftTrackerPosition();
  DR3 hitRight = GetHitRightTrackerPosition(); 

  Double_t x0 = hitLeft(0);
  Double_t y0 = hitLeft(1);
  Double_t z0 = hitLeft(2);

  Double_t x1 = hitRight(0);
  Double_t y1 = hitRight(1);
  Double_t z1 = hitRight(2);

 //-----------------------------//
 //       x = az + b            //
 //       y = cz + d            //
 //-----------------------------//
  
  DR3 tLineFitSlope(0.,0.,1.);   // Slopes in the tracker frame
  DR3 tLineFitOrigin(0.,0.,0.);

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

  for(Int_t i=0; i<3 ; ++i) {
    _miniVectorDirection[i] = tLineFitSlope(i);
    _miniVectorOrigin[i]    = tLineFitOrigin(i);
  }

  return tLineFitSlope;
}

//______________________________________________________________________________
//    
DR3 MiniVector::CalculateMiniVectorIntersectionTracker(DPrecAlign* myDPrecAlign)
{
  // LC 2014/12/20 Similar to Calcultate Intersection but for MiniVector objects
  
  DR3 Direction = GetMiniVectorDirection();
  DR3 Origin = GetMiniVectorOrigin(); // After GetMVDirection.

  Double_t tx=Origin(0); 
  Double_t ty=Origin(1); 
  Double_t tz=Origin(2);
  Double_t tdx=Direction(0);
  Double_t tdy=Direction(1);

  DR3 _coeffABC = myDPrecAlign->GetCoeffs();
  
  //_coeffABC.Print();
   
  Double_t t=-(_coeffABC(0)*tx+_coeffABC(1)*ty+tz+_coeffABC(2))/(_coeffABC(0)*tdx+_coeffABC(1)*tdy+1.);
  
  Double_t _x = tx+t*tdx;
  Double_t _y = ty+t*tdy;
  Double_t _z = tz+t*1.;

  return DR3(_x, _y, _z);
}

//______________________________________________________________________________
//    
DR3 MiniVector::CalculateMiniVectorIntersectionLadder(DPrecAlign* myDPrecAlign)
{
  // LC 2014/12/20 Similar to Calcultate Intersection but for MiniVector objects
  
  DR3 Direction = GetMiniVectorDirection();
  DR3 Origin = GetMiniVectorOrigin(); // GetMiniVectorTrackerCenter();

  //std::cout<<"origin and direction"<<std::endl; 
  //Direction.Print();
  //Origin.Print();
  
  Double_t tx=Origin(0); 
  Double_t ty=Origin(1); 
  Double_t tz=Origin(2);
  Double_t tdx=Direction(0);
  Double_t tdy=Direction(1);

  DR3 _coeffABC = myDPrecAlign->GetCoeffs();
  
  Double_t t=-(_coeffABC(0)*tx+_coeffABC(1)*ty+tz+_coeffABC(2))/(_coeffABC(0)*tdx+_coeffABC(1)*tdy+1.);
  
  Double_t _x = tx+t*tdx;
  Double_t _y = ty+t*tdy;
  Double_t _z = tz+t*1.;

/*  
  std::cout<<"pos tracker"<<std::endl;
  DR3(_x, _y, _z).Print();
*/
  return myDPrecAlign->TransformHitToPlane( DR3(_x, _y, _z) );

}

//______________________________________________________________________________
//
DR3 MiniVector::GetHitLeftPlanePositionMC()
{
  return DR3( _hitLeftPositionMC[0], _hitLeftPositionMC[1], _hitLeftPositionMC[2] );
}

//______________________________________________________________________________
//
DR3 MiniVector::GetHitRightPlanePositionMC()
{
  return DR3( _hitRightPositionMC[0], _hitRightPositionMC[1], _hitRightPositionMC[2] );
}

//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorLadderCenterMC() 
{
  return DR3( _miniVectorCenterPositionMC[0], _miniVectorCenterPositionMC[1], _miniVectorCenterPositionMC[2] );
}

//______________________________________________________________________________
//
DR3 MiniVector::GetHitLeftTrackerPositionMC()
{
  // Recompute the hit position wrt the ladder alignment parameters.
  
  DR3 hitLeftLadderFrameMC = DR3( _hitLeftPositionMC[0], _hitLeftPositionMC[1], _hitLeftPositionMC[2] );
  DR3 hitLeftTrackerFrameMC = (_planesDPrecAlign.first)->TransformHitToTracker( hitLeftLadderFrameMC );

  return hitLeftTrackerFrameMC;
}

//______________________________________________________________________________
//
DR3 MiniVector::GetHitRightTrackerPositionMC()
{
  // Recompute the hit position wrt the ladder alignment parameters.
  DR3 hitRightLadderFrameMC = DR3( _hitRightPositionMC[0], _hitRightPositionMC[1], _hitRightPositionMC[2] );
  DR3 hitRightTrackerFrameMC = (_planesDPrecAlign.second)->TransformHitToTracker( hitRightLadderFrameMC );

  return hitRightTrackerFrameMC; 
}

//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorTrackerCenterMC()
{
  // Recompute the hit position wrt the ladder alignment parameters.
  
  DR3 leftTrackerMC = GetHitLeftTrackerPositionMC();
  DR3 rightTrackerMC = GetHitRightTrackerPositionMC();
  DR3 centerTrackerMC = (rightTrackerMC + leftTrackerMC)/2.;


//  std::cout<<"cnterTracker/DPrecAlign Left/ DPrecAlignRight"<<std::endl;
//  centerTracker.Print();
/*
  (_planesDPrecAlign.first)->PrintAll();
  (_planesDPrecAlign.first)->PrintTorMatrix();
  (_planesDPrecAlign.second)->PrintAll();  
  (_planesDPrecAlign.second)->PrintTorMatrix();
*/
  DR3 centerLadderFrameMC = DR3( _miniVectorCenterPositionMC[0], _miniVectorCenterPositionMC[1], _miniVectorCenterPositionMC[2] );
  DR3 centerTrackerFrameMC = _ladderDPrecAlign->TransformHitToTracker( centerLadderFrameMC );
/*
  std::cout<<"cnterTracker = "<<std::endl;
  centerTrackerFrame.Print();
  centerTracker.Print();
*/
  return centerTrackerMC/*Frame*/;
}

//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorOriginMC()
{
  return DR3(_miniVectorOriginMC[0], _miniVectorOriginMC[1], _miniVectorOriginMC[2] );
}


//______________________________________________________________________________
//
DR3 MiniVector::GetMiniVectorDirectionMC()
{
  // Recompute the mini-vector direction wrt the ladder alignment parameters.
  DR3 hitLeftMC  = GetHitLeftTrackerPositionMC();
  DR3 hitRightMC = GetHitRightTrackerPositionMC(); 

  Double_t x0 = hitLeftMC(0);
  Double_t y0 = hitLeftMC(1);
  Double_t z0 = hitLeftMC(2);

  Double_t x1 = hitRightMC(0);
  Double_t y1 = hitRightMC(1);
  Double_t z1 = hitRightMC(2);

 //-----------------------------//
 //       x = az + b            //
 //       y = cz + d            //
 //-----------------------------//
  
  DR3 tLineFitSlope(0.,0.,1.);   // Slopes in the tracker frame
  DR3 tLineFitOrigin(0.,0.,0.);

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

  for(Int_t i=0; i<3 ; ++i) {
    _miniVectorDirectionMC[i] = tLineFitSlope(i);
    _miniVectorOriginMC[i]    = tLineFitOrigin(i);
  }

  return tLineFitSlope;
}

//______________________________________________________________________________
//    
DR3 MiniVector::CalculateMiniVectorIntersectionTrackerMC(DPrecAlign* myDPrecAlign)
{
  // LC 2014/12/20 Similar to Calcultate Intersection but for MiniVector objects
  
  DR3 DirectionMC = GetMiniVectorDirectionMC();
  DR3 OriginMC = GetMiniVectorOriginMC(); // After GetMVDirection.

  Double_t tx=OriginMC(0); 
  Double_t ty=OriginMC(1); 
  Double_t tz=OriginMC(2);
  Double_t tdx=DirectionMC(0);
  Double_t tdy=DirectionMC(1);

  DR3 _coeffABC = myDPrecAlign->GetCoeffs();
  
  //_coeffABC.Print();
   
  Double_t t=-(_coeffABC(0)*tx+_coeffABC(1)*ty+tz+_coeffABC(2))/(_coeffABC(0)*tdx+_coeffABC(1)*tdy+1.);
  
  Double_t _x = tx+t*tdx;
  Double_t _y = ty+t*tdy;
  Double_t _z = tz+t*1.;

  return DR3(_x, _y, _z);
}

//______________________________________________________________________________
//    
DR3 MiniVector::CalculateMiniVectorIntersectionLadderMC(DPrecAlign* myDPrecAlign)
{
  // LC 2014/12/20 Similar to Calcultate Intersection but for MiniVector objects
  
  DR3 DirectionMC = GetMiniVectorDirectionMC();
  DR3 OriginMC = GetMiniVectorOriginMC(); // GetMiniVectorTrackerCenter();

  //std::cout<<"origin and direction"<<std::endl; 
  //Direction.Print();
  //Origin.Print();
  
  Double_t tx=OriginMC(0); 
  Double_t ty=OriginMC(1); 
  Double_t tz=OriginMC(2);
  Double_t tdx=DirectionMC(0);
  Double_t tdy=DirectionMC(1);

  DR3 _coeffABC = myDPrecAlign->GetCoeffs();
  
  Double_t t=-(_coeffABC(0)*tx+_coeffABC(1)*ty+tz+_coeffABC(2))/(_coeffABC(0)*tdx+_coeffABC(1)*tdy+1.);
  
  Double_t _x = tx+t*tdx;
  Double_t _y = ty+t*tdy;
  Double_t _z = tz+t*1.;

/*  
  std::cout<<"pos tracker"<<std::endl;
  DR3(_x, _y, _z).Print();
*/
  return myDPrecAlign->TransformHitToPlane( DR3(_x, _y, _z) );

}
