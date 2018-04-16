// @(#)maf/dtools:$Name:  $:$Id: DParticle.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author   :  Dirk Meier   98/02/19

#ifndef _DParticle_included_
#define _DParticle_included_

  /////////////////////////////////////////////////////////////
  //                                                         //
  // Class Description of Particle                           //
  //                                                         //
  //                                                         //
  /////////////////////////////////////////////////////////////

#include <math.h>
#include "Riostream.h"

// ROOT classes
#include "TObject.h"
#include "TClonesArray.h"

class DHit;
class DLine;
class DR3;
class DPlane;
class DTrack;

class DParticle : public TObject {
 private:
  DR3             *fMomentum;    // this should become four-momentum vector....
  DR3             *fPosition;
  Float_t          fCharge;
  Float_t          fPDGCode;
public:
  DParticle();                                 
  virtual         ~DParticle();
  void             Vacuum();
  DR3&             GetMomentum(){ return *fMomentum; }
  DR3&             GetPosition(){ return *fPosition; }
  Float_t          GetCharge(){ return fCharge; }
  Float_t          GetPDGCode(){return fPDGCode; }
  ClassDef(DParticle,1)   // Describes Particle

};

#endif
