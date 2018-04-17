#ifndef _DPixel_included_
#define _DPixel_included_

#include <math.h>

// ROOT classes
#include "TObject.h"
#include "DR3.h"

class DSetup;
class DPlane;
class TH1F;
class DPixel : public TObject {

 public:
  DPixel();                                 
  DPixel( Int_t aNumber, const Int_t aIndex, Double_t aValue, Int_t aTime=0);
  DPixel( Int_t aNumber, Int_t aLine, Int_t aColumn, Double_t aValue, Int_t aTime=0); // JB 2009/08/17
  //DPixel( Int_t aNumber, const Int_t aIndex, Double_t aValue, Int_t aLine, Int_t aColumn, DR3 aPosition, DR3 aSize);
  ~DPixel();
  
  Double_t           Distance( DPixel   &aPixel);
  Double_t           Distance( const DR3&      aPosition);
  Double_t           DistanceU( DPixel&        aPixel);
  Double_t           DistanceU( const DR3&     aPosition);
  Double_t           DistanceV( DPixel&        aPixel);
  Double_t           DistanceV( const DR3&     aPosition);

  void               SetPlanenumber(Int_t aNumber) { fPlaneNumber = aNumber; }
  void               SetRawValue( Double_t aRV)    { fRawValue = aRV; }
  void               SetPulseHeight( Double_t aPH) { fPulseHeight = aPH; }
  void               SetPedestal( Double_t aPed)   { fPedestal = aPed; }
  void               SetNoise( Double_t aNoise)    { fNoise = aNoise; }
  void               SetPixelLine( Int_t aLin)     { fPixelLine = aLin; }
  void               SetPixelColumn( Int_t aCol)   { fPixelColumn = aCol; }
  void               SetPosition( DR3 aPosition)   { fPosition = aPosition; }
  void               SetSize( DR3 aSize)           { fSize = aSize; }
  void               SetFound(Bool_t b)            { fFound = b; }

  void               SetPixelIndex(Int_t px)       { fPixelIndex = px;} //RDM060809

  Int_t              GetPixelIndex()               { return  fPixelIndex;}
  Int_t              GetPixelLine()                { return  fPixelLine;}
  Int_t              GetPixelColumn()              { return  fPixelColumn;}

  Double_t           GetRawValue()                 { return  fRawValue;      }
  Double_t           GetPulseHeight()              { return  fPulseHeight;   }
  Double_t           GetNoise()                    { return  fNoise;         }
  Double_t           GetPedestal()                 { return  fPedestal;      }
  Double_t           GetCommonMode()               { return  fCommonMode;    } 
  Double_t           GetPulseHeightToNoise();

  Int_t              GetTimestamp()                { return  fTimestamp;     }
  
  DR3&               GetPosition()                 { return  fPosition;      }
  DR3&               GetSize()                     { return  fSize;          }
  Bool_t             Found()                       { return  fFound;         }

  Int_t              GetPlaneNumber()              { return  fPlaneNumber;   }

 private:

  Int_t              fDebugPixel;
  Int_t              fPlaneNumber;                  // number of the plane
  DR3                fPosition;                     // position in uvw coordinates in the plane
  DR3                fSize;                         // size in uvw directions

  Int_t              fPixelIndex;                   // index of the pixel
  Int_t              fPixelLine;                    // line in the matrix
  Int_t              fPixelColumn;                  // column in the matrix
  Double_t           fRawValue;                     // the rawvalue
  Double_t           fPulseHeight;                  // pulseheight on strip
  Double_t           fNoise;                        // noise on strip
  Double_t           fCommonMode;                   // the common mode value 
  Double_t           fPedestal;                     // pedestal of strip
  Int_t              fTimestamp;                    // timestamp associated, 2013/06/22

  Bool_t             fFound;                        // flag, that strip is found in hit
  
  ClassDef(DPixel,1)                                 // Pixel or Pixel of a Detector Plane

};

#endif










