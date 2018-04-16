// @(#)maf/dtools:$Name:  $:$Id: DLine.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author :  Dirk Meier     98/01/09

#ifndef _DLine_included_
#define _DLine_included_



  //////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                                          //
  // Class Description of DLine                                                               // 
  //                                                                                          //
  // a Line object is defined by its                                                          //
  //                  origin     = (x_0,y_0,z_0),                                             //
  //                  direction  = (dx,dy,dz),                                                //
  // and              length.                                                                 //
  // Points on the line at r_i are given as a function                                        //
  // of the parameter beta. beta has no dimension.                                            //
  // r_i(beta) = origin_i + beta * direction_i                                                //
  // If one wants the pair (x,y) as a function of z along (0,0,dz) then beta = z/dz           //
  //    and r_1(beta) = x_0 + z * dx/dz                                                       //
  //        r_2(beta) = y_0 + z * dy/dz                                                       //
  //        r_3(beta) = z_0 + z * 1                                                           //  
  // In case one needs pair (x,y,z) as a function of l along (dx,dy,dz) then beta' = l/dl     //
  //    and r_1(beta') = x_0 + l * dx/dl                                                      //
  //        r_2(beta') = y_0 + l * dy/dl                                                      //
  //        r_3(beta') = z_0 + l * dz/dl                                                      //
  // with the relation beta^2 = beta'^2 * (1-x^2/l^2-y^2/l^2) / (1-dx^2/dl^2-dy^2/dl^2)       //
  //                                                                                          //
  //////////////////////////////////////////////////////////////////////////////////////////////


#include <math.h>

// ROOT classes
#include "TObject.h"


class DR3;

class DLine : public TObject {

private:
  DR3               *fOrigin;       // origin x0,y0,z0 
  DR3               *fDirection;    // direction dx,dy,dz in [mm]
  DR3               *fSlope;        // the slope (dx/dz, dy/dz, 1) this is redundant					 
  Float_t            fLength;


public:
  DLine();                       
  DLine(DR3 &aOrigin, DR3 &aSlope, Float_t aLength);
  ~DLine();
  void               Zero();
  Float_t            Distance(DR3 &p);
  DR3&               GetOrigin()             const { return *fOrigin; }
  DR3&               GetDirection()          const { return *fDirection; } 
  DR3&               GetSlopeZ()             const { return *fSlope; } 
  Float_t            GetLength()             const { return fLength; }
  DR3                GetPoint(Float_t beta); // get point on line at beta, parameter along the line
  DR3                GetIntersectZ(Float_t aZvalue);

  void               SetValue(const DR3& aOrigin, 
			      const DR3& aDirection, 
			      const DR3& aSlope,
			      const Float_t aLength);

  ClassDef(DLine,1)   // Describes DLine

};

#endif
