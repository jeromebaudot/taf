// @(#)maf/dtools:$Name:  $:$Id: DEvent.h,v.2 2005/10/02 18:03:46 sha Exp $
// Author: Dirk Meier   98/02/18


#ifndef _DEvent_included_
#define _DEvent_included_


  ////////////////////////////////////////////////////////////////
  // Class Description of DEvent                                //
  //                                                            //
  // Trigger event contains values from detector planes         //
  // This event is later written to a data summary ".root" file //
  //                                                            //
  // Datamembers are public for historical resons               //
  ////////////////////////////////////////////////////////////////

#include <math.h>
#include "Riostream.h"

// ROOT classes
#include "TObject.h"
#include "TClonesArray.h"
#include "TArrayF.h"
#include "TH1.h"

class DSetup;
class DPlane;          // used to fill data into event plane
class DTrack;
class DHit;
class DAcq;


class DEventHeader  {

  // data which is characteristic to the event
  // convention: variables start with `E'

 public:
  DEventHeader(){;}
  virtual         ~DEventHeader(){;}
  void           Set(const Int_t aNEvent, 
		     const Int_t aNRun, 
		     const Int_t aDate, 
		     const Int_t aTime, 
		     const Int_t aType,
		    std::vector<int> *aListOfTriggers=NULL,
		    std::vector<int> *aListOfFrames=NULL,
		    std::vector<int> *aListOfTimestamps=NULL);
  void           SetTimeSinceLastEvent(const Int_t aDeltaTime);
  Int_t          GetEventNumber()     const { return Ek;    }
  Int_t          GetRunNumber()       const { return Erunk; }
  Int_t          GetDate()            const { return Edate; }
  Int_t          GetTime()            const { return Etime; }
  Int_t          GetNumberOfTriggers()        { return ENumberOfTriggers;}
  Int_t          GetNumberOfFrames()          { return ENumberOfFrames;}
  Int_t          GetNumberOfTimestamps()      { return ENumberOfTimestamps;}
  Int_t          GetTriggerAt( int index)     { return EListOfTriggers[index]; }
  Int_t          GetFrameAt( int index)       { return EListOfFrames[index]; }
  Int_t          GetTimestampAt( int index)   { return EListOfTimestamps[index]; }
  void           Print();

  Int_t          Ek;              // Number of the Event
  Int_t          Erunk;           // Number of the Run
  Int_t          Edate;           // Date YYMMDD
  Int_t          Etime;           // Time HHMMSS
  Int_t          EtimeSinceLastEvent ;
  Int_t          Erelative;       // time in seconds since start of month, 
                                  // causes problems when month wraps.
  Int_t          Etype;           // type of the event 
                                  // (may be significant when random trigger comes)
  Int_t          ENumberOfTriggers;
  Int_t          EListOfTriggers[50];    // list of triggers JB 2010/07/07
  Int_t          ENumberOfFrames;
  Int_t          EListOfFrames[50];      // list of frames JB 2010/07/07
  Int_t          ENumberOfTimestamps;
  Int_t          EListOfTimestamps[50];  // list of timestamps JB 2010/07/07

  ClassDef(DEventHeader,2)        // Describes Event Header
    };

//__________________________________________________________________________________
//  

class DEvent : public TObject {
 public:
  DEvent();                                 
  DEvent(DSetup& fc);
  virtual         ~DEvent();
  void             Clear(const Option_t * /*option*/ = "");
  void             SetHeader(const Int_t aNEvent, 
			     const Int_t aNRun, 
			     const Int_t aDate, 
			     const Int_t aTime, 
			     const Int_t aType,
			    std::vector<int> *aListOfTriggers=NULL,
			    std::vector<int> *aListOfFrames=NULL,
			    std::vector<int> *aListOfTimestamps=NULL);
  void             SetTimeInterval(const Int_t aDeltaTime);
  void             AddAuthenticPlane(DPlane& aPlane, Int_t aNEvent);
  void             AddTransparentPlane(DPlane& aPlane, DTrack& aTrack, DHit& aHit);
  void             AddAuthenticHit(DHit& aHit, Int_t aNEvent, DTrack& aTrack);
  DEventHeader&    GetHeader()               { return fHeader;   }
  TClonesArray    *GetAuthenticHits()        { return fAHits;    }
  TClonesArray    *GetAuthenticPlanes()      { return fAPlanes;  }
  TClonesArray    *GetTransparentPlanes()    { return fT1Planes; }
  void            SetTDC(Float_t aTDCValue) { fTDC = aTDCValue; }
  void            SetFrame(const Int_t aFrameNb, 
		           const Int_t aLineNb);
  Int_t          GetFrameNb()              { return fFrameNb; }
  Int_t          GetLineNb()               { return fLineNb; }

  DEventHeader     fHeader;       // Header of the Event
  Int_t            fEvt;
  Short_t          fAHitsN;       // number of cluster hits in data array
  TClonesArray    *fAHits;        // pointer to hits 
  Short_t          fT1PlanesN;    // number of planes in transparent data array
  TClonesArray    *fT1Planes;     // transparent data of detector planes

  Short_t          fAPlanesN;     // number of authentic planes in array
  TClonesArray    *fAPlanes;      // pointer to authentic planes

  Float_t          fTDC;          // TDC value for SCT analysis

  Int_t          fFrameNb;        // frame number from CMOS sensor
  Int_t          fLineNb;         // line number from CMOS sensor

  ClassDef(DEvent,3)              // Describes Event
    };

//____________________________________________________________________
//  

class DAuthenticHit : public TObject {

  // data which is characteristic to a hit
  // convention: variables start with `H'

 public:
  DAuthenticHit(){;}
  DAuthenticHit(DHit& aHit, Int_t aNEvent, DTrack& aTrack);
  virtual       ~DAuthenticHit(){;}

  Int_t          Hevt;           // event number
  Short_t        Hhk;            // authentic hit number in the plane 
  Short_t        HhN;            // number of authentic hits in the plane 
  Short_t        Hpk;            // number of the plane (Hit Plane Number)
  Float_t        Hsu;            // u position of hit Seed Strip
  Float_t        Hsv;            // v position of hit Seed Strip
  Float_t        Hu;             // position of hit measured with charge fraction method (2-strip 
  Float_t        Hv;
  Float_t        HuCG;           // position of hit measured with center of gravity (3x3) method
  Float_t        HvCG;           // position of hit measured with center of gravity (3x3) method
  Float_t        HuEta;          // position of hit measured with eta method
  Float_t        HvEta;          // position of hit measured with eta method


  Float_t        Htv;            // v position of track next to this hit
  Float_t        Htu;            // u position of track next to this hit
  Short_t        HtN;            // number of tracks found passing the plane
  Short_t        Htk;            // nearest track number, JB 2009/06/26
  Short_t        HtHn;           // number of hits in the track, JB 2009/09/08
  Float_t        HtChi2;         // chiSquare/ndf of track fit (2D), track next to this hit
  Float_t        HtChi2u;        // chiSquare/ndf of track fit in u dim, track next to this hit
  Float_t        HtChi2v;        // chiSquare/ndf of track fit in v dim, track next to this hit

  Float_t        Hqc;            // cluster pulse sum
  Short_t        HsN;            // strips in cluster
  Float_t        Hsn;            // seed strip noise 
  //----ab
  Float_t        HsnPulse;        // seed strip pulse
  Float_t        HsnIndex;        // seed strip index

  Float_t        HSNc;           // cluster S/N  (for S/N of neighbours see HSNneighbour)
  Float_t        Hnc;            // cluster noise average
  Float_t        HSNc1;          // Hqc/Hsn
  Int_t          Hsk;            // seed strip index
 
  Int_t          HNNS;           //Number of neighbours of the seed ;

  Float_t        Hq0;            // pulseheight on seed strip, (neighbour 0)
  Float_t        Hq1;            // second highest pulseheight
  Float_t        Hq2;            // third highest pulseheight
  Float_t        Hq3;
  Float_t        Hq4;
  Float_t        Hq5;
  Float_t        Hq6;
  Float_t        Hq7;
  Float_t        Hq8;
  Float_t        HqM[100];     // all  pixels in MIMOSA cluster
  
  Int_t         Hk0;             // strip index of strip with charge Hq0
  Int_t         Hk1;             // strip index of strip with charge Hq1
  Int_t         Hk2;             // with Hq2
  Int_t         Hk3;             // with Hq3
  Int_t         Hk4;             // with Hq4
  Int_t         Hk5;             // with Hq5
  Int_t         Hk6;             // with Hq6
  Int_t         Hk7;
  Int_t         Hk8;
  Int_t         HkM[400];         // needs to be large enough

  Float_t        Hn0;            // noise on seed strip, (neighbour 0)
  Float_t        Hn1;            // second pixel noise
  Float_t        Hn2;            // third pixel noise
  Float_t        Hn3;
  Float_t        Hn4;
  Float_t        Hn5;
  Float_t        Hn6;
  Float_t        Hn7;
  Float_t        Hn8;
  Float_t        HnM[400];     // all  pixels in MIMOSA cluster


  Float_t        HqL;             // charge on left most strip of two
  Float_t        HqR;             // charge on right most strip of two
  Float_t        HqRoS;           // charge right of seed
  Float_t        HqLoS;           // charge left of seed

  Int_t         HkL;             // index of strip left of seed, `HkR' = HkL+1 would be redundant
  Float_t        HuL;             // postion u [micron] left of two highest

  Float_t       HSNneighbour;    // S / N of neighbours.

  ClassDef(DAuthenticHit,4)  
    };

//__________________________________________________________________________
//  

class DAuthenticPlane : public TObject {

  // data which is characteristic to a plane
  // convention: variables start with `P'

 public:
  DAuthenticPlane(){;}
  DAuthenticPlane(DPlane& aPlane, Int_t aNEvent);
  virtual       ~DAuthenticPlane(){;}
  
 public:
  Int_t          Pevt;           // event number
  Short_t        Ppk;            // Authentic Plane Number
  Short_t        PhN;            // number of hits in this plane
  Short_t        PtN;            // number of tracks crossing this plane
  Float_t        Pt;             // authentic plane threshold on single strip [ADC]
  Short_t        PotN;           // authentic number of strips with pulseheight over threshold
  Float_t        PotQ;           // charge sum on strips over threshold

  // the following is usefull for noise estimation on channels/strips
  Int_t          Psk;            // index of MCstrip
  Float_t        Psq;            // pulseheight on MCstrip
  Float_t        Ppq;            // pedestal value on MCstrip 
  Float_t        Pnq;            // noise value on MCstrip 

  Float_t        PFq;             //Charge on a fixed pixel
  Float_t        PFn;             //Noise  on a fixed pixel
  Float_t        PFp;             //Pedestal -  --  -
  Float_t        PFr;             //Raw value - --  -
  Float_t        PFrfr1;          //Raw value frame 1 - --  -
  Float_t        PFrfr2;          //Raw value frame 2 - --  -

  Float_t        Pcom1;          // common mode correction on up to 2 regions 
  Float_t        Pcom2;          // (e.g. for three readout chips)
  Float_t        Pcom3;
  Float_t        Pcom4;
  Float_t        Pcom5;
  Float_t        Pcom6;
  Float_t        PqL1;           // charge on MCstrip -1
  Float_t        PCDSvar;

  ClassDef(DAuthenticPlane,1)  
    };

//_________________________________________________________
//  

class DTransparentPlane : public TObject {

  // data from plane in region of track intersection
  // convention: variables start with `T'  except
  // those of DoubleSided `DS'

 public:
  DTransparentPlane(){;}
  DTransparentPlane(DPlane& aPlane, DTrack& aTrack);
  DTransparentPlane(DPlane& aPlane, DTrack& aTrack, DHit& aHit);
  virtual       ~DTransparentPlane() { 
    Tpk = 0;
    Ttk = 0;
    Tu  = 0.;
    Tv  = 1.;
    Tud = 0.;
    Tvd = 0.;
    ThN = 0 ;
    Tpt = 0.;
    TotN= 0;
    TotQ= 0.;
  }
  
 
  Short_t        Tpk;             // number of the plane ( Track Plane Number)
  Short_t        TtN;             // number of tracks in this plane
  Short_t        Ttk;             // number of the track normally 1
  Short_t        TtHn;            // number of hits in the track, JB 2009/09/08
 
  Float_t        Tu;              // track position perpendicular to strip, u-direction
  Float_t        Tv;              // track position along strip, v-direction

  Float_t        TDOX ;           //Incertitude sur la position de l origine de la trace
  Float_t        TDOY ;           //Incertitude sur la position de l origine de la trace

  Float_t        Tud;             // (principal)Hu - Tu
  Float_t        Tvd;             // (principal)Hv - Tv
  Short_t        ThN;             // number of hits found in the plane 
  Short_t        Thk;             // // authentic hit number in the plane 
  Int_t          TsN;             // number of strips 
  Float_t        Tpt;             // transparent plane threshold on single strip [ADC]
  Short_t        TotN;            // number of strips in neighbourhood to the track over thresh.
  Float_t        TotQ;            // transparent charge sum on strips 
                                  // in neighbourhood of the track over threshold
  Float_t        Tqc;             // Principal hit cluster pulse sum
  Float_t        Tq0;             // Principal hit seed pulseheight
  Float_t        Tn0;             // Principal hit seed noise

  Float_t        Tq1;             // transparent charge on nearest track, 
  Float_t        Tq2;             // transparent charge on 2nd next nearest
  Float_t        Tq3;             // and so on
  Float_t        Tq4;
  Float_t        Tq5;
  Float_t        Tq6;
  Float_t        Tq7;
  Float_t        Tq8;
  Float_t        Tq9;

  Float_t        Tu1;              // absolute u postion [micron ] of strip next to track
  Short_t        Tk1;              // index of strip next to track 

  Float_t        Tchi2;            // ChiSquare/ndf of the track fit, 2 dim
  Float_t        Tchi2u;           // ChiSquare/ndf of the track fit, U dim
  Float_t        Tchi2v;           // ChiSquare/ndf of the track fit, V dim

  // here one would prefer Tdu, Tdv
  Float_t        Tx,  Ty,  Tz;   // track coordinates
  Float_t        Tdx, Tdy;       // track directions
  Float_t        Tdu, Tdv;       // the slope in plane coordinates

  
  ClassDef(DTransparentPlane,1)  
    };

#endif
