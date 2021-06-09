#ifndef _DStrip_included_
#define _DStrip_included_

#include <math.h>

// ROOT classes
#include "TObject.h"
#include "TH1.h"
#include "DR3.h"
//#include "DPixel.h"  //YV 04/06/09

class DSetup;
class DPlane;
class TH1F;

//YV 04/06/09 make DStrip class inherit from DPixel
class DStrip : public TObject {
  //class DStrip : public DPixel {


 public:
  DStrip();                                 
  DStrip(DPlane& aPlane, const Int_t aStripIndex, DR3& aPosition, DR3& aSize);
  ~DStrip();
  
  Int_t              Introduce(     DStrip   &aStrip);
  Float_t            Distance(      DStrip   &aStrip);
  Float_t            Distance(const DR3&      aPosition);
  Float_t            DistanceU(DStrip&        aStrip);
  Float_t            DistanceU(const DR3&     aPosition);
  Float_t            DistanceV(DStrip&        aStrip);
  Float_t            DistanceV(const DR3&     aPosition);

  void               SumValue();         // sums the value and increments a counter
  void               SumSquareValue();   // sums the square of the value and increments counter
  void               InitPedestal();
  void               InitNoiseAndPedestal(); 
  void               SetNoise(Float_t tNV );       // set noise  to  the value
  void               SetPedestal(Float_t tPV);     // set pedestal to the value
  void               InitNoise();
  void               SetCommonMode(Float_t tCM)     { fCommonMode = tCM;      }
  void               SetRawValue(Float_t tRV);     // sets raw value and updates;
  //---addition 10/1/01
  void               SetRawFrame1Value(Float_t tRF1);     // sets raw value and updates;
  void               SetRawFrame2Value(Float_t tRF2);     // sets raw value and updates;
  Float_t            GetRawFrame1Value()                  { return  fRawFrame1Value;      }
  Float_t            GetRawFrame2Value()                  { return  fRawFrame2Value;      }

  void               Update();
  void               UpdatePedestalAndNoise();
  void               UpdatePedestal(Float_t tRV);
  void               UpdateSignal();
  void               UpdateSignal(Float_t tRV);
  void               UpdateNoise(Float_t tSV);

  Int_t              GetStripIndex()                { return  fStripIndex;    }
  Int_t              GetPixelIndex()                { return  fPixelIndex;    } // JB 2012/08/18
  void               SetPixelIndex( Int_t anIndex)  { fPixelIndex = anIndex;  } // JB 2012/08/18
  Int_t              GetPlaneNumber()               { return  fPlaneNumber;   }
  Float_t            GetRawValue()                  { return  fRawValue;      }
  Float_t            GetPulseHeight()               { return  fPulseHeight;   }
  Float_t            GetPulseHeightToNoise();
  Float_t            GetNoise()                     { return  fNoise;         }
  Float_t            GetPedestal()                  { return  fPedestal;      }
  Float_t            GetCommonMode()                { return  fCommonMode;    } 
  Int_t              GetPedestalCounts()            { return  fSumCount;      } // JB 2010/10/16
  Int_t              GetNoiseCounts()               { return  fSumSquareCount;} // JB 2010/10/16
  /*
  //YV 04/06/09 change floats to double
  Double_t            GetRawValue()                  { return  fRawValue;      }
  Double_t            GetPulseHeight()               { return  fPulseHeight;   }
  Double_t            GetPulseHeightToNoise();
  Double_t            GetNoise()                     { return  fNoise;         }
  Double_t            GetPedestal()                  { return  fPedestal;      }
  Double_t            GetCommonMode()                { return  fCommonMode;    } 
  */


  DR3&               GetPosition()                  { return  fPosition;      }
  DR3&               GetSize()                      { return  fSize;          }
  DPlane            *GetPlane()                     { return  fPlane;         }
  //---abesson september 2005
  DSetup            *GetSetup()                     { return   fc;            }
  DStrip            *GetNeighbour(Int_t aNI);
  Int_t              GetNeighbourCount()            { return  fNeighbourCount;}
  Bool_t             Found()                        { return  fFound;         }
  void               SetFound(Bool_t b)             { fFound = b;             }
  Bool_t               KillNoise();

 private:
  Int_t              fDebugStrip;
  Float_t            fRawValue;                     // the rawvalue
  //Double_t            fRawValue;                     // the rawvalue YV 04/06/09
  Float_t            fRawFrame1Value;                     // the rawvalue w/o CDS (1st frame)
  Float_t            fRawFrame2Value;                     // the rawvalue w/o CDS (2nd frame)
  Float_t            fSumValue;                     // sum of  values for pedestal/noise initialization
  Float_t            fSumSquareValue;               // sum of squares of raw values for noise initialization
  Float_t            fInitialPedestal;              // initial pedestal value
  Float_t            fInitialTMS;                   // initial variance (the mean square)
  Int_t              fSumCount;                     // counter for raw signal summation
  Int_t              fSumSquareCount;               // counter for squared raw signal summation

  Int_t              fPlaneNumber;                  // number of the plane
  DPlane            *fPlane;                        // pointer to the plane 
  DSetup            *fc;                            // pointer to the configuration
  DR3                fPosition;                     // position in uvw coordinates in the plane
  DR3                fSize;                         // size in uvw directions

  Int_t              fStripIndex;                   // number of this strip 
  Int_t              fPixelIndex;                   // corresponding index in the pixel List, JB 2012/08/18 
  Float_t            fPulseHeight;                  // pulseheight on strip
  Float_t            fNoise;                        // noise on strip
  Float_t            fCommonMode;                   // the common mode value 
  Float_t            fPedestal;
  /*
  //YV 04/06/09
  Double_t            fPulseHeight;                  // pulseheight on strip
  Double_t            fNoise;                        // noise on strip
  Double_t            fCommonMode;                   // the common mode value 
  Double_t            fPedestal;
  */
  Int_t              fWeightPedestal;               // a weight (e.g. 100) for pedestal follower
  Int_t              fWeightNoise;                  // a weight (e.g. 100) for pedestal follower

  //YV ends

  Int_t              fCallCount;                    // Number of events in long term noise calc.
  Int_t              fCallCountMaximum;             // how many events to include

  Bool_t             fFound;                        // flag, that strip is found in hit
  Bool_t             fUsePulse;                     // flag, that strip can be used in analysis
  
  DStrip**           fNeighbourList;                 //! array of strips in the neighbourhood
  Int_t              fNeighbourCount;
  Int_t              fNeighbourCountMaximum;         // max. # of neighbours in bound
  Float_t            fBound;                         // circle in mm around seed center

  Float_t            aSignalSupressedValue(Float_t* data, const Int_t entries);
  Int_t              extremumIndex(const Float_t* data, const Int_t N);
  Int_t              fCacheSize;                     // the size for the noise and pedestal buffer = 5;
  Float_t            fNoiseCache[50];                 // FIFO buffer for noise measurement
  Int_t              fNoiseCacheIndex;               // counter 
  Float_t            fPedestalCache[50];              // FIFO buffer for noise measurement
  Int_t              fPedestalCacheIndex;            // counter 
  Float_t            median(Float_t* data, const Int_t N);
  
  ClassDef(DStrip,3)                                 // Strip or Pixel of a Detector Plane

};

#endif










