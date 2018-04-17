// Author   : LC, 2012/10/18

  ////////////////////////////////////////////////////////////
  // Class Description of DMonteCarlo                       //
  //                                                        //
  //                                                        //
  ////////////////////////////////////////////////////////////


#include "DMonteCarlo.h"
#include "DR3.h"

ClassImp(DMonteCarlo) // Description of Single Detector DMonteCarlo 
//______________________________________________________________________________
//  
DMonteCarlo::DMonteCarlo()
{ 
  // DMonteCarlo default constructor
}

//______________________________________________________________________________
//  

DMonteCarlo::~DMonteCarlo()
{
  // DMonteCarlo default destructor

}

//______________________________________________________________________________
//  
DMonteCarlo::DMonteCarlo( Int_t aPlaneNumber, Double_t aMC_X, Double_t aMC_Y, Double_t aMC_Z, Double_t aValue)
{
  // constructor of a MonteCarlo point with his parameter

  fDebugMonteCarlo     = 0;

  fPlaneNumber    = aPlaneNumber;
  fMonteCarloX    = aMC_X;
  fMonteCarloY    = aMC_Y;
  fMonteCarloZ    = aMC_Z;
  fRawValue       = aValue;

  fFound          = kFALSE;

  if(fDebugMonteCarlo>1) printf("DMonteCarlo : Plane %d with value %f built\n", fPlaneNumber, fRawValue);

}
