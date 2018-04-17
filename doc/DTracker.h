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

class DSetup;
class DPlane;
class DTrack;
class DHit;
class DAcq;
class DLadder;

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
    
  void             find_tracks_2_ivi();// method to tracks adapted for IVI VR 2014/07/14
  Int_t            fTrackingOrderLines;      // Number of Lines to define planes tracking order
  Int_t**          fTrackingOrderPreTrack;   // Planes'order to build pre-tracks
  Int_t**          fTrackingOrderFulTrack;   // Planes'order to build full-tracks
  Int_t            fKeepUnTrackedHitsBetw2evts; //explicit // VR 2014.08.28
  Double_t         fSearchMoreHitDistance; // Max distance to associate a hit to a pre-track, VR 2014/06/29
  
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

 public:
  DTracker();                                 
  DTracker(DSetup& c, DAcq& aAcq);                                 
  ~DTracker();
     
  Int_t            Update();
  Int_t            GetAlignmentStatus()                       { return fAlignmentStatus; }
  void             SetAlignmentStatus(Int_t aStatusValue);
  void             SetRequiredHits(Int_t aNumber);
  Int_t            GetRequiredHits()                          { return fRequiredHits; } // JB 2011/03/14
  void             Align(DTrack *aTrack);                     // align planes in telescope with respect to track

  DAcq            *GetAcq()                                   { return  fAcq;          }
  DSetup          *GetSetup()                                 { return  fc;            }
  DTrack          *GetTrack(Int_t aTN);
  DTrack          *GetPrincipalTrack()                        { return  fTrack[0];     }
  DTrack		      *nearest_track( DHit *aHit); // JB, 2009/07/17
  DHit			   *nearest_hit( DTrack *aTrack, Int_t aPlaneNumber, Bool_t &hitAssociated); // JB 2009/07/17, JB 2014/08/29
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

  void  SetDebug(Int_t aDebug);
  Int_t GetDebug()    { return fDebugTracker;}

  void             PrintStatistics(ostream &stream=cout); // JB 2009/09/09 // SS 2011/12/14

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

