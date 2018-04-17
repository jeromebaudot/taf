// @(#)maf/dtools:$Name:  $:$Id: DParticle.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author   :  Dirk Meier   98/02/19

  /////////////////////////////////////////////////////////////
  //                                                         //
  // Class Description of Particle                           //
  //                                                         //
  //                                                         //
  /////////////////////////////////////////////////////////////

//*-- Modified :  
//*-- Copyright:  RD42
//*-- Author   :  Dirk Meier   98/02/19
//*KEEP,CopyRight.
/************************************************************************
 * Copyright(c) 1998, RD42@CERN, DiamondTracking.
 ***********************************************************************/
//*KEND.

//*KEEP,DParticle.
#include "DParticle.h"

//*KEEP,DR3.
#include "DR3.h"

//*KEND.

ClassImp(DParticle)               // Description of a Particle

//______________________________________________________________________________
//  
DParticle::DParticle()
{ 
  fMomentum  = new DR3();
  fPosition  = new DR3();
}

//______________________________________________________________________________
//  
DParticle::~DParticle()
{ 
}

//______________________________________________________________________________
//  
void DParticle::Vacuum(){
  fMomentum->Zero();
  fPosition->Zero();
  fCharge = 0.;
}
