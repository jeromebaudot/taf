// @(#)maf/dtools:$Name:  $:$Id:DTracker.h  v.1 2005/10/02 18:03:46 sha Exp $
//  Author   :   Dirk Meier   97/12/10

#ifndef _DTracker_included_
#define _DTracker_included_


  ////////////////////////////////////////////////////////////
  // Class Description of DTracker                          //
  //                                                        //
  ////////////////////////////////////////////////////////////


#include "TObject.h"
#include "TBRIK.h"
#include "TNode.h"
#include "TObjArray.h"
#include "Riostream.h"
#include "DLadder.h"
#include "TList.h"
#include "MKalmanFilter.h"
#include "MLeastChiSquare.h"
#include "DEventMC.h"

#include "DBeaster.h"


using namespace std;

class DSetup;
class DPlane;
class DTrack;
class DHit;
class DAcq;
class DLadder;
class DEventMC;
class DBeaster; //DC 2017/03/08
void FCNAlignVertex(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag); //LC. 2012/12/13.

class DTracker : public TObject {

 private:
  DAcq            *fAcq;               // pointer to acquisition
  DSetup          *fc;                 // pointer to the configuration

  Int_t            fTracksMaximum;     // maximum number of tracks allowed
  Int_t            fTracksFinder;       // select track finding method
  Int_t            fTracksN;           // number of tracks found
  DTrack         **fTrack;             //! pointer to track list
  DTrack          *fTrackVoid;         // this is an "empty" track

  Int_t            fVertexMaximum;     // maximum number of vertex allowed, JB 2013/06/11

  DTrack         **fSubTrack;          // pointer to subtrack list, JB 2014/12/15
  Int_t            fSubTrackPlanesN;   // number of planes in subtrack, JB 2014/12/15
  Int_t           *fSubTrackPlaneIds;  // array of ids for planes in subtrack, JB 2014/12/15

  Int_t            fTrackFittingAlg;   // Track fitting algorithm, QL 2016/06/06
  //Bool_t	   fKalEnabled;	       // Enable Kalman filter&smoother or not.
  //DTrack	  *fKalTrack;	       // pointer to tracks fitted by Kal.

  Int_t            fHitsMaximum;       // maximum number of hits allowed
  Int_t            fHits;              // number of hits found
  DHit           **fHitList;           //! pointer to hit list

  Int_t            fPlanesStatus ;     // init status of the planes
  Int_t            fAlignmentStatus;   // status of the alignement

  Int_t            fPlanesN;           // number of planes
  TObjArray       *fPlaneArray;	      // pointer to array of planes
  TObjArray       *fLadderArray;       // pointer to array of Ladders
  Int_t            fNumberOfLadders;   // LC 2013/01/15.

  TString          fTrackerName;       // name of this tracker
  TNode           *fTrackerNode;       // pointer to the node of the tracker
  TBRIK           *fTrackerGeometry;   // pointer to the geometry

  void             find_tracks();      // original method to find tracks


  void             find_tracks_1_opt();// method to find tracks like find_tracks() but with more options VR 2014/07/14
  Int_t            fTrackingPlaneOrderType; // the planes ordering type for finding tracks VR 2014/07/14

  void             find_tracks_2_ivi();      // method to tracks adapted for IVI VR 2014/07/14
  // see DSetup.h or DSetup.cxx for above member relative to find_tracks_2_ivi()

  void             find_tracks_Beast(); // method to tracks adapted to Beast Experiment DC 2017/02

  Int_t            fTrackingPass;
  Int_t*     	   fPreTrackHitsNbMinimum;
  Int_t*     	   fPreTrackHitsTypeUsed;
  Float_t*   	   fPreTrackHitsMaxDist;
  Int_t*     	   fExtTrackHitsNbMinimum;
  Int_t*     	   fExtTrackHitsTypeUsed;
  Float_t*   	   fExtTrackHitsMaxDist;
  Int_t*     	   fFullTrackHitsNbMinimum;
  Int_t            fTrackingOrderLines;
  Int_t**          fTrackingOrderPreTrack;
  Int_t**          fTrackingOrderExtTrack;

  Int_t            fKeepUnTrackedHitsBetw2evts; //explicit // VR 2014.08.28
  Double_t         fSearchMoreHitDistance;   // Max distance to associate a hit to a pre-track, VR 2014/06/29

  void             find_tracks_withStrips(); // finds the track in the strip-tracker
  void             find_tracks_and_vertex();
  void             find_vertex();      // finds the vertex for the tracks in the event. LC 2012/12/13.

  Int_t            fTrackSeedDevs;     // amount of track seed planes (e.g. 2 Silicon planes)
  Int_t            fFixRefDevs;        // amount of fixed reference planes (e.g. 4 Silicon planes)
  Int_t            fVarRefDevs;        // amount of variable reference planes (e.g. 4 other Silicon planes)
  Int_t            fTestDevs;          // amount of DUT planes
  Int_t           *fListTrackSeedDevs; //! list with numbers of fixed reference planes
  Int_t           *fListFixRefDevs;    //! list with numbers of fixed reference planes
  Int_t           *fListVarRefDevs;    //! list with numbers of variable reference planes
  Int_t           *fListTestDevs;      //! list with numbers of det. under test


  Int_t            fRequiredHits;      //! number of hits required to make a track, Jb 2009/05/21
  Double_t         fSearchHitDistance; //! Max distance to associate a track and a hit, JB 2009/05/21
  Int_t            fUseSlopeInExtrapolation; // JB 2013/06/21
  Int_t            fUseVertexConstraint; // JB 2013/06/21

  Int_t           *fTrackCount;        // Counter of tracks with # hits per track, JB 2009/09/08
  Int_t           *fTrackCountPerPlane; // Counter of tracks per plane, JB 2014/08/29

  Int_t            fDebugTracker;                 // debug flag

  static DTracker* fgInstance;

  DEventMC*        MCInfoHolder;        // AP 2016/07/27   Object with all the MC information. i.e. the full list of particles, hits and pixels (both from physics and noise)

  DBeaster*        fBeaster;


 public:
  DTracker();
  DTracker(DSetup& c, DAcq& aAcq);
  ~DTracker();

  enum             {kSimpleChi2, kKalman, kChi2MS}; // QL 2016/06/06
  Int_t            Update();
  Int_t            UpdateMC();
  Int_t            GetAlignmentStatus()                       { return fAlignmentStatus; }
  void             SetAlignmentStatus(Int_t aStatusValue);
  void             SetRequiredHits(Int_t aNumber);
  Int_t            GetRequiredHits()                          { return fRequiredHits; } // JB 2011/03/14
  void             Align(DTrack *aTrack);                     // align planes in telescope with respect to track

  DAcq            *GetAcq()                                   { return  fAcq;          }
  DBeaster        *GetBeaster()                               { return fBeaster;       }
  DSetup          *GetSetup()                                 { return  fc;            }
  DTrack          *GetTrack(Int_t aTN);
  DTrack          *GetSubTrack(Int_t aTN); // JB 2014/12/15
  DTrack          *GetPrincipalTrack()                        { return  fTrack[0];     }
  DTrack		      *nearest_track( DHit *aHit); // JB, 2009/07/17
  DHit			      *nearest_hit( DTrack *aTrack, Int_t aPlaneNumber, Bool_t &hitAssociated); // JB 2009/07/17, JB 2014/08/29
  DPlane          *GetPlane(Int_t aPk);
  DLadder         *GetLadder(Int_t aLadder);
  Int_t            GetNumberOfLadders()                       { return  fNumberOfLadders;}
  TString          GetTrackerName()                           { return  fTrackerName;  }
  Int_t            GetPlanesN()                               { return  fPlanesN;      }
  Int_t            GetTracksN()                               { return  fTracksN;      }
  Int_t            GetTracksMaximum()                         { return  fTracksMaximum;}
  Int_t            GetNTrackSeed()                            { return  fTrackSeedDevs; }
  Int_t            GetNFixRef()                               { return  fFixRefDevs; }
  Int_t            GetNVarRef()                               { return  fVarRefDevs; }
  Int_t           *GetListTrackSeed()                         { return  fListTrackSeedDevs; }
  Int_t           *GetListFixRef()                            { return  fListFixRefDevs; }
  Int_t           *GetListVarRef()                            { return  fListVarRefDevs; }
  Int_t            GetNDUT()                                  { return  fTestDevs; }
  Int_t           *GetListDUT()                               { return  fListTestDevs; }
  Int_t            GetTrackCount( Int_t i)                    { return  fTrackCount[i]; }
  Int_t            GetTrackCountPerPlane( Int_t iPlane)       { if( iPlane<fPlanesN ) return  fTrackCountPerPlane[iPlane]; else return -1; }

  TNode           *GetNode()                                  { return  fTrackerNode;  }

  Int_t            GetPlanesStatus()                          {return fPlanesStatus;}

  void             SetPlanesForSubTrack(Int_t nPlanes, Int_t *planeIds);// JB 2015/08/21
  Int_t            GetNPlanesForSubTrack() { return fSubTrackPlanesN; } // JB 2014/12/15
  Int_t           *GetPlanesForSubTrack() {return fSubTrackPlaneIds; } // JB 2014/12/15
  Int_t            GetPlaneIdForSubTrack( Int_t aNb) { return (aNb<fSubTrackPlanesN)?fSubTrackPlaneIds[aNb]:0; } // JB 2014/12/15
  void             MakeSubTrack( DTrack *aTrack, DHit** aHitList, Int_t nHits); // JB 2014/12/15
  void             MakeKalTrack( DTrack *aTrack, DHit** aHitList, Int_t nHits); // QL 2016/05/26
  void             MakeLeastChi2Track( DTrack *aTrack, DHit** aHitList, Int_t nHits); // QL 2016/06/08

  void  SetDebug(Int_t aDebug);
  Int_t GetDebug()    { return fDebugTracker;}

  DEventMC*        GetMCInfoHolder()            { return MCInfoHolder;  }     // AP 2016/04/21 : Function to get the MCInfoHolder

  void             PrintStatistics(ostream &stream=cout); // JB 2009/09/09 // SS 2011/12/14

  void             MCTracksTruthMatching(void);           //AP 2016/08/08: Function to perform the truth matching of the reconstructed tracks

/*
  static DTracker*& Instance() {
  :tabprev  if (!fgInstance)
    {
      cout << "DTracker::Instance: No current DTracker session detected! Creating a new one..." << endl;
      fgInstance = new DTracker();
    }
    return fgInstance;
  }
*/
  void             fcn(Int_t &npar, Double_t *gin, Double_t &fValue, Double_t *x, Int_t iflag);
  static DTracker*& Instance()  { return fgInstance; }

  ClassDef(DTracker,1)                 // Describes DTracker

};

/*
class NodeData
{
  private:

    Int_t     nodeNumber;
    Double_t  Chi2;
    Int_t     hitNumber;

  public:

    NodeData(Int_t node, Int_t hNumber, Double_t chi2)  { nodeNumber = node; hitNumber = hNumber; Chi2 = chi2; };
    Int_t     GetNodeNumber()  { return nodeNumber; };
    Double_t  GetChi2()        { return Chi2; };
    Int_t     GetHitNumber()   { return hitNumber; };

    void      SetNodeNumber(Int_t node)    { nodeNumber = node; };
    void      SetChi2(Double_t chi2)       { Chi2 = chi2; };
    void      SetHitNunber(Int_t hNumber)  { hitNumber = hNumber; };
};

class TreeNode
{

  private:

    //Int_t numberOfNodes;
    NodeData* data;
    std::map<Int_t, TreeNode*> children;
    std::map<Double_t, Int_t>  classment;

  public:

    TreeNode()                           {};
    //Int_t      GetNumberOfNodes()        { return numberOfNodes; };
    NodeData*  GetData()                 { return data; };
    TreeNode*  GetNode(Int_t hitNumber)  { std::map<Int_t, TreeNode*>::iterator iter = children.find(hitNumber); return iter->second; };
    Int_t GetHitnumberWithChi2Min()      { return classment[0]; };

    void       SetData(NodeData* myData) { data = myData; };
    void       AddChildren(Int_t hitNumber, NodeData* myData) { TreeNode* T = new TreeNode(); T->SetData(myData); children.insert(std::pair<Int_t, TreeNode*>(hitNumber,T));
    classment.insert(std::pair<Double_t,Int_t>(myData->GetChi2(), myData->GetHitNumber()) );  };

};
*/
#endif
