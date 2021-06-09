#ifndef _DBeaster_included_
#define _DBeaster_included_

// #include <THStack.h>
// #include <TH3.h>
#include "TLegend.h"
#include "TObject.h"

//TAF Include
#include "DTracker.h"
#include "DLadder.h"
#include "DPlane.h"
#include "DAlign.h"
#include "DHit.h"
#include "DR3.h"
#include "DHelixFitter.h"


using namespace std;
class DHelixFitter;

class DBeaster: public TObject {
private:
public:
  DBeaster (DTracker *Tracker);
  virtual ~DBeaster ();

  DLadder          *aLadder;
  DPlane           *aPlane;
  DHit             *aHit;
  DTracker         *tTracker;
  DEventMC         *MCInfoHolder;
  DSession         *fSession;
  DPrecAlign       *Dprec;
  DHelixFitter     *fHelixFitter;

  struct ABeastHit {
    //Hit Identification
    int       LadderId;
    int       ModuleId;
    int       SensorId;
    int       HitId;
    int       McHitId ;
    //Position in Ladder Ref Frame
    DR3       hitPositionLadderUVW;
    //Position in Lab Ref Frame
    DR3       hitPositionLabXYZ;
    // Analysis Flag

      //Spatial Clustering
      int       RecoId_SC;
      bool      IsRec_SC;
      //Superposition
      int       RecoId_SP;
      bool      IsRec_SP;
      double    RpairedSP;
      bool      IsFill_SC;
      //Alignment
      int       RecoId_AL;
      bool      IsRec_AL;
      double    VpairedAL;

  };
  //List Of Hits on Module 1 & 2
  std::vector<ABeastHit>  ListOfBeastHitsOnM1;
  std::vector<ABeastHit>  ListOfBeastHitsOnM2;
  struct ABeastRecoPart {
    int       ClassType;
    std::vector<ABeastHit> HitList;
    std::vector<int> HitScIdList;
    std::vector<int> HitSpIdList;

    bool             IsRec_SPreco;
    bool             IsRec_ALreco;
  };
  std::vector<ABeastRecoPart>     ListOfRecoPartsM1_SC;
  std::vector<ABeastRecoPart>     ListOfRecoPartsM2_SC;
  std::vector<ABeastRecoPart>     ListOfRecoPartsM1_AL;
  std::vector<ABeastRecoPart>     ListOfRecoPartsM2_AL;
  std::vector<ABeastRecoPart>     ListOfRecoParts_SP;
  std::vector<ABeastRecoPart>     ListOfBeastRecoParts_L1;
  std::vector<ABeastRecoPart>     ListOfBeastRecoParts_L2;
  std::vector<SimParticle_t>      ListOfTsIpCandidat;
  std::vector<SimParticle_t>      ListOfTsCandidat;
  std::vector<int>                ListOfTslCandidat;
  std::vector<int>                ListOfSimuPartClass;
  std::vector<int>                ListOfSimuPartIdL1;
  std::vector<int>                ListOfSimuPartIdL2;
  std::vector<ABeastRecoPart>     ListOfRecoTS;
  std::vector<ABeastRecoPart>     ListOfFakeRecoTs;

  //********* Particle patterns********* //
  // OS  --> One Side                   //
  // TS  --> Two Sides                 //
  // OSL --> One Side Looper          //
  // TSL --> Two Sides Looper        //
  // ******************************** //
    int NbPart;
    int NbHit;
    int NbPolyLine;
    int NbHitsMod1;
    int NbHitsMod2;
    int NbHitsL1;
    int NbHitsL2;
    int LadderNumber;
    int PlaneIdx;
    int SensorMin;
    int SensorMax;
  // Boolean for Particle classification
    bool Ladder1;
    bool Ladder2;
    bool Ladder;
    bool Module1;
    bool Module2;
    bool MoreThanTwoHits;
    int  NbHitsTest;

  //********* Hit Parameters *********//
    double PhiLoc;
    double ThetaLoc;
    double Rsphere;
    double Utest;
    double Vtest;
    double Xsphere;
    double Ysphere;
    double Zsphere;
    double DeltaV;
    double DeltaU;
    double DeltaVtest;

  //Lab reference frame
    double X1;
    double X2;
    double X;
    double Y1;
    double Y2;
    double Y;
    double dR;
  //Ladder reference frame
    double U1;
    double U2;
    double dU;
    double V1;
    double V2;
    double dV;
  //Angular projection
    double AP1;
    double AP2;
    double dAP;
  //Momentum
    double Pin;
    double dP;
    double Pt;
    double Ptout;
    double dPt;
    double Pt0;

    double Rvertex;

  //   int TwoSidesLoopersIdx;
  //   int OneSidesLoopersIdx;
  //   int TwoSidesHitsIdx;

    Int_t TwoSidesLoopersFrames;
    Int_t TwoSidesLoopersParts;

    Int_t OneSidesLoopersFrames;
    Int_t OneSidesLoopersParts;

    Int_t TwoSidesHitsFrames;
    Int_t TwoSidesHitsParts;

    std::vector<int> L1M1_1part;
    std::vector<int> L1M2_1part;
    std::vector<int> L2M1_1part;
    std::vector<int> L2M2_1part;

    std::vector<int> L1M1_2part;
    std::vector<int> L1M2_2part;
    std::vector<int> L2M1_2part;
    std::vector<int> L2M2_2part;

    std::vector<int> L1M1_3part;
    std::vector<int> L1M2_3part;
    std::vector<int> L2M1_3part;
    std::vector<int> L2M2_3part;



    DR3 *hitPositionPlane;
    DR3 hitPositionLadder;
    DR3 hitPositionLab;
  //********* Histogram Definition *********//

    // TH1F *h_hitL1;
    //
    // TH1F *h_SimuPart;
    // TH1F *h_RecoPart;
    //
    // TH1F *h_RecoPart_aL1;
    // TH1F *h_RecoPart_bL1;
    // TH1F *h_RecoPart_cL1;
    // TH1F *h_RecoPart_dL1;
    //
    // TH1F *h_RecoPart_aL2;
    // TH1F *h_RecoPart_bL2;
    // TH1F *h_RecoPart_cL2;
    // TH1F *h_RecoPart_dL2;
    //
    // TH1F *h_SimuPart_aL1;
    // TH1F *h_SimuPart_bL1;
    // TH1F *h_SimuPart_cL1;
    // TH1F *h_SimuPart_dL1;
    //
    // TH1F *h_SimuPart_aL2;
    // TH1F *h_SimuPart_bL2;
    // TH1F *h_SimuPart_cL2;
    // TH1F *h_SimuPart_dL2;
    //
    // TH1F *h_SimuOS;
    // TH1F *h_SimuTS;
    // TH1F *h_SimuOSL;
    // TH1F *h_SimuTSL;
    //
    // TH1F *h_SimuOS_Good;
    // TH1F *h_SimuTS_Good;
    // TH1F *h_SimuOSL_Good;
    // TH1F *h_SimuTSL_Good;
    //
    // TH1F *h_SimuOS_Bad;
    // TH1F *h_SimuTS_Bad;
    // TH1F *h_SimuOSL_Bad;
    // TH1F *h_SimuTSL_Bad;
    //
    // THStack *hs_PartnerOS;
    // THStack *hs_PartnerTS;
    // THStack *hs_PartnerOSL;
    // THStack *hs_PartnerTSL;
    //
    // TH1F *h_MomentumOS;
    // TH1F *h_MomentumTS;
    // TH1F *h_MomentumOSL;
    // TH1F *h_MomentumTSL;
    //
    // TH1F *h_AngularDistributionTsL1;
    // TH1F *h_AngularDistributionTsL2;
    // TH1F *h_AngularDistributionTslL1;
    // TH1F *h_AngularDistributionTslL2;
    //
    // TH1F *h_NbTslL1;
    // TH1F *h_NbTslL2;
    //
    // TH2F *h_FractionLossL1;
    // TH2F *h_FractionLossL2;
    //
    // THStack *hs_DCH_CatHitStack;
    //
    //
    // TH1F *h_D3hit1Part;
    // TH1F *h_D3hit2Part;
    // TH1F *h_D3hit3Part;
    //
    // TH3F *h_VrtxPosXYZ;
    // TH2F *h_VrtxPosXZ;
    // TH2F *h_VrtxPosXY;
    // TH2F *h_VrtxPosRZ;
    //
    // TH2F *h_VrtxPosXY_OS;
    // TH2F *h_VrtxPosXY_TS;
    // TH2F *h_VrtxPosXY_OSL;
    // TH2F *h_VrtxPosXY_TSL;
    //
    // TH2F *h_VrtxPosXY2;
    //
    // TH1F *h_ER_P_OSL;
    // TH1F *h_ER_Pt_OSL;
    //
    // TH1F *h_ER_P_TSL;
    // TH1F *h_ER_Pt_TSL;
    //
    // TH1F *h_ER_P_TS;
    // TH1F *h_ER_Pt_TS;
    //TLine *l_projection;

    // TPolyMarker *m_Test;
    // TPolyMarker *m_Test2;
    //
    // TPolyLine *l_Test;
    // TPolyLine *l_Test2;
    //TView3D *view;

    TVector3 Vhit;
    TVector2 VDhit;
    double MagVDhit;
    DHelixFitter    *GetHelixFitter()                           { return fHelixFitter;   }

    // vector<ABeastHit> GetBeastHitsOnM1() const {return ListOfBeastHitsOnM1;}
    // vector<ABeastHit> GetBeastHitsOnM2() const {return ListOfBeastHitsOnM2;}
    // vector<ABeastRecoPart> GetListOfBeastRecoPart() {return ListOfBeastRecoParts;}
    virtual DR3  GetHitPositionOnLadderRefFrame(int PlaneId,int HitId);
    virtual DR3  GetHitPositionOnLabRefFrame(int PlaneId,int HitId);
    virtual void SpatialClustering(double Seuil);
    virtual void Superposition(double Seuil);
    virtual void Allignement(double Seuil);
    virtual void FillListOfRecoPartsSC();
    virtual void FillListOfRecoPartsSP();
    virtual void FillListOfBeastRecoPart(int ladderID);
    virtual void SimuCategorieClassification();
    virtual void SimuCategorieClassificationNew();
    virtual void RecoCategorieClassification(int ladderId,int iHitSeuil);
    virtual void Fill_Modules_HitList(int LadderId);
    virtual void Particle_Reconstruction(int LadderId);
    virtual void Clear_List();
    // virtual void PurityTest();
    // virtual void PurityTest_Partner();
    // virtual void Categorie_Momentum();
    // virtual void Angular_DistributionSimuTS();
    // virtual void Angular_DistributionSimuTSL();
    virtual void FillTslCandidat();
    virtual void FillListOfRecoPartsSPnew();
    virtual void HelixFitBuildUpStudy(int iLadder);
    virtual void BuildMcTrueTsFromIpList(int iLadder);
    virtual void BuildMcTrueTsList(int iLadder);
    virtual void BuildFakeRecoTsList(int iLadder);

ClassDef(DBeaster,1)
};
#endif
