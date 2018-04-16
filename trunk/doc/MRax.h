#ifndef _MRax_included_
#define _MRax_included_

//////////////////
// INCLUDES
//////////////////

// ROOT
#include "Riostream.h"
#include "TArrow.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TControlBar.h"
#include "TGaxis.h"
#include "TF1.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TLine.h"
#include "TObject.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TPolyMarker.h"
#include "TStyle.h"
#include "TText.h"
#include "TVector.h"
#include "TMaterial.h"
#include "TTUBE.h"

// ROOT GUI
#include "TGDockableFrame.h"
#include "TGMenu.h"
#include "TGMdiDecorFrame.h"
#include "TG3DLine.h"
#include "TGMdiFrame.h"
#include "TGMdiMainFrame.h"
#include "TGMdiMenu.h"
#include "TGListBox.h"
#include "TGNumberEntry.h"
#include "TGScrollBar.h"
#include "TGComboBox.h"
#include "TGuiBldHintsEditor.h"
#include "TRootBrowser.h"
#include "TGuiBldNameFrame.h"
#include "TGFrame.h"
#include "TGFileDialog.h"
#include "TGShutter.h"
#include "TGButtonGroup.h"
#include "TGCommandPlugin.h"
#include "TGCanvas.h"
#include "TGFSContainer.h"
#include "TGuiBldEditor.h"
#include "TGColorSelect.h"
#include "TGTextEdit.h"
#include "TGButton.h"
#include "TGFSComboBox.h"
#include "TGLabel.h"
#include "TGView.h"
#include "TGMsgBox.h"
#include "TRootGuiBuilder.h"
#include "TGFileBrowser.h"
#include "TGTab.h"
#include "TGListView.h"
#include "TGSplitter.h"
#include "TGTextEditor.h"
#include "TRootCanvas.h"
#include "TGStatusBar.h"
#include "TGListTree.h"
#include "TGuiBldGeometryFrame.h"
#include "TGToolTip.h"
#include "TGToolBar.h"
#include "TGuiBldDragManager.h"
#include "TGHtmlBrowser.h"
#include "Riostream.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"
#include "TGeoMaterial.h"


// TAF
#include "DAcq.h"
#include "DCut.h"
#include "DGlobalTools.h"
#include "DHit.h"
#include "DPixel.h"
#include "DLine.h"
#include "DPlane.h"
#include "DR3.h"
#include "DSession.h"
#include "DStrip.h"
#include "DTrack.h"
#include "DTracker.h"
#include "MAnalysis.h"

// C++
#include <cmath>
#include <iostream>
#include <vector>


class MRax : public TObject
{
//______________________________________________________________________________________________________
//
//	MRAX configuration
//______________________________________________________________________________________________________
  //****************************************************************
  //		General configuration
  //****************************************************************
private:
  Int_t ConfigureGeneral(void);
  
  //****************************************************************
  //		Displays configuration
  //****************************************************************
private:
  Int_t ConfigureDisplays(void);
  
private:
  Int_t configDisplaysPadDivideX; 
  Int_t configDisplaysPadDivideY;
  vector<Int_t>  configPlanesDrawOrder_default;
  vector<Char_t> configPlanesLeftRight_default;
  vector<Int_t>  configPlanesDrawOrder_custom1;
  vector<Char_t> configPlanesLeftRight_custom1;
  Int_t configOverlapLineRowNumber;


//______________________________________________________________________________________________________
//
//	Display functions : pixels, hits, tracks, vertices
//______________________________________________________________________________________________________
  //****************************************************************
  //		Pixels in Cmos Frame [Pixels]
  //****************************************************************
public:
  Int_t DisplayPixelsInCmosFrameInPixel_Init(void);
  Int_t DisplayPixelsInCmosFrameInPixel_Fill(void);
  Int_t DisplayPixelsInCmosFrameInPixel_Show(Bool_t WithHits=1, Bool_t WithOverlapLine=1,Bool_t WithZbar=1, Int_t minPixOccu=0);
  Int_t DisplayPixelsInCmosFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayPixelsInCmosFrameInPixel_SaveHistos(void);
  Int_t DisplayPixelsInCmosFrameInPixel_Reset(void);
  Int_t DisplayPixelsInCmosFrameInPixel_Kill(void);

private:
  Bool_t	flagDisplayPixelsInCmosFrameInPixel_Init;
  Int_t		*DisplayPixelsInCmosFrameInPixel_PlanesDrawOrder;
  TCanvas	*canvasDisplayPixelsInCmosFrameInPixel;
  TPaveLabel	*labelDisplayPixelsInCmosFrameInPixel;
  TPad		*padDisplayPixelsInCmosFrameInPixel;
  TH2I		**histoDisplayPixelsInCmosFrameInPixel_RAW;
  TH2I		**histoDisplayPixelsInCmosFrameInPixel_HIT;
  TLine		**lineDisplayPixelsInCmosFrameInPixel;
  Int_t         eventsProcessedDisplayPixelsInCmosFrameInPixel;

  //****************************************************************
  //            Noisy Pixels
  //****************************************************************
public:
  Int_t DisplayNoiseStudy_Init(void);
  Int_t DisplayNoiseStudy_Show(Float_t relOccur_min=0.01, Float_t relOccur_max=1, UInt_t relOccur_steps=20, Bool_t xAxisLog0_Lin1=0);
  Int_t DisplayNoiseStudy_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayNoiseStudy_SaveHistos();
  Int_t DisplayNoiseStudy_Reset(void);
  Int_t DisplayNoiseStudy_Kill(void);

private:
  Bool_t        flagDisplayNoiseStudy_Init;
  Int_t         *DisplayNoiseStudy_PlanesDrawOrder;
  TCanvas       *canvasDisplayNoiseStudy;
  TPaveLabel    *labelDisplayNoiseStudy;
  TPad          *padDisplayNoiseStudy;
  TGraph        **graphDisplayNoiseStudy;
  
  //****************************************************************
  //		Pixels in FIRST board Frame [Pixels]
  //****************************************************************
public:
  Int_t DisplayPixelsInFirstBoardFrameInPixel_Init(void);
  Int_t DisplayPixelsInFirstBoardFrameInPixel_Fill(void);
  Int_t DisplayPixelsInFirstBoardFrameInPixel_Show(Bool_t WithHits=1, Bool_t WithOverlapLine=1,Bool_t WithZbar=1, Int_t minPixOccu=0);
  Int_t DisplayPixelsInFirstBoardFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayPixelsInFirstBoardFrameInPixel_Reset(void);
  Int_t DisplayPixelsInFirstBoardFrameInPixel_Kill(void);

private:
  Bool_t	flagDisplayPixelsInFirstBoardFrameInPixel_Init;
  Int_t		*DisplayPixelsInFirstBoardFrameInPixel_PlanesDrawOrder;
  Char_t	*DisplayPixelsInFirstBoardFrameInPixel_PlanePosition;
  TCanvas	*canvasDisplayPixelsInFirstBoardFrameInPixel;
  TPaveLabel	*labelDisplayPixelsInFirstBoardFrameInPixel;
  TPad		*padDisplayPixelsInFirstBoardFrameInPixel;
  TH2I		**histoDisplayPixelsInFirstBoardFrameInPixel_RAW;
  TH2I		**histoDisplayPixelsInFirstBoardFrameInPixel_HIT;
  TLine		**lineDisplayPixelsInFirstBoardFrameInPixel;
  TGaxis	**axisDisplayPixelsInFirstBoardFrameInPixel;
  TF1 		*faxisDisplayPixelsInFirstBoardFrameInPixel_XLeft;
  TF1 		*faxisDisplayPixelsInFirstBoardFrameInPixel_YLeft;
  TF1 		*faxisDisplayPixelsInFirstBoardFrameInPixel_XRight;
  TF1 		*faxisDisplayPixelsInFirstBoardFrameInPixel_YRight;
  Int_t         eventsProcessedDisplayPixelsInFirstBoardFrameInPixel;
      
  //****************************************************************
  //		Hits in Cmos Frame [Pixels]
  //****************************************************************
public:
  Int_t DisplayHitsInCmosFrameInPixel_Init(void);
  Int_t DisplayHitsInCmosFrameInPixel_Fill(void);
  Int_t DisplayHitsInCmosFrameInPixel_Show(Bool_t ShowAsso2TrackHits=1, Bool_t WithOverlapLine=1,Bool_t HitShowedAsPix=0, Int_t minHitOccu=0);
  Int_t DisplayHitsInCmosFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayHitsInCmosFrameInPixel_Reset(void);
  Int_t DisplayHitsInCmosFrameInPixel_Kill(void);

private:
  Bool_t	flagDisplayHitsInCmosFrameInPixel_Init;
  Int_t		*DisplayHitsInCmosFrameInPixel_PlanesDrawOrder;
  TCanvas	*canvasDisplayHitsInCmosFrameInPixel;
  TPaveLabel	*labelDisplayHitsInCmosFrameInPixel;
  TPad		*padDisplayHitsInCmosFrameInPixel;
  TH2I		**histoDisplayHitsInCmosFrameInPixel_unTrack, **histoDisplayHitsInCmosFrameInPixel_Track;
  TLine		**lineDisplayHitsInCmosFrameInPixel;
  Int_t         eventsProcessedDisplayHitsInCmosFrameInPixel;

  //****************************************************************
  //		Hits in FIRST board Frame [Pixels]
  //****************************************************************
public:
  Int_t DisplayHitsInFirstBoardFrameInPixel_Init(void);
  Int_t DisplayHitsInFirstBoardFrameInPixel_Fill(void);
  Int_t DisplayHitsInFirstBoardFrameInPixel_Show(Bool_t ShowAsso2TrackHits=1, Bool_t WithOverlapLine=1,Bool_t HitShowedAsPix=0, Int_t minHitOccu=0);
  Int_t DisplayHitsInFirstBoardFrameInPixel_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayHitsInFirstBoardFrameInPixel_Reset(void);
  Int_t DisplayHitsInFirstBoardFrameInPixel_Kill(void);

private:
  Bool_t	flagDisplayHitsInFirstBoardFrameInPixel_Init;
  Int_t		*DisplayHitsInFirstBoardFrameInPixel_PlanesDrawOrder;
  Char_t	*DisplayHitsInFirstBoardFrameInPixel_PlanePosition;
  TCanvas	*canvasDisplayHitsInFirstBoardFrameInPixel;
  TPaveLabel	*labelDisplayHitsInFirstBoardFrameInPixel;
  TPad		*padDisplayHitsInFirstBoardFrameInPixel;
  TH2I		**histoDisplayHitsInFirstBoardFrameInPixel_unTrack, **histoDisplayHitsInFirstBoardFrameInPixel_Track;
  TLine		**lineDisplayHitsInFirstBoardFrameInPixel;
  TGaxis	**axisDisplayHitsInFirstBoardFrameInPixel;
  TF1 		*faxisDisplayHitsInFirstBoardFrameInPixel_XLeft;
  TF1 		*faxisDisplayHitsInFirstBoardFrameInPixel_YLeft;
  TF1 		*faxisDisplayHitsInFirstBoardFrameInPixel_XRight;
  TF1 		*faxisDisplayHitsInFirstBoardFrameInPixel_YRight;
  Int_t         eventsProcessedDisplayHitsInFirstBoardFrameInPixel;

  //****************************************************************
  //		Hits in Tracker Frame [um]
  //****************************************************************
public:
  Int_t DisplayHitsInTrackerFrameInUm_Init(void);
  Int_t DisplayHitsInTrackerFrameInUm_Fill(void);
  Int_t DisplayHitsInTrackerFrameInUm_Show(Int_t TrackDisplayMode=1 /*0:0; 1:circle; 2:text*/);
  Int_t DisplayHitsInTrackerFrameInUm_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayHitsInTrackerFrameInUm_Reset(void);
  Int_t DisplayHitsInTrackerFrameInUm_Kill(void);

private:
  Bool_t	flagDisplayHitsInTrackerFrameInUm_Init;
  Int_t		*DisplayHitsInTrackerFrameInUm_PlanesDrawOrder;
  TCanvas	*canvasDisplayHitsInTrackerFrameInUm;
  TPaveLabel	*labelDisplayHitsInTrackerFrameInUm;
  TPad		*padDisplayHitsInTrackerFrameInUm;
  TH2I		**histoDisplayHitsInTrackerFrameInUm_HIT;
  TH2I		**histoDisplayHitsInTrackerFrameInUm_HITasso2track;
  TH2I		**histoDisplayHitsInTrackerFrameInUm_HITintersecTrackPlane;
  Int_t         eventsProcessedDisplayHitsInTrackerFrameInUm;

  //****************************************************************
  //		Hits in Tracker Frame Overlay view [um]
  //****************************************************************
public:
  Int_t DisplayHitsInTrackerFrameInUmOverlayed_Init(Bool_t alignMode=1);
  Int_t DisplayHitsInTrackerFrameInUmOverlayed_Fill(void);
  Int_t DisplayHitsInTrackerFrameInUmOverlayed_Show(Bool_t *planes2display=NULL);
  Int_t DisplayHitsInTrackerFrameInUmOverlayed_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayHitsInTrackerFrameInUmOverlayed_Reset(void);
  Int_t DisplayHitsInTrackerFrameInUmOverlayed_Kill(void);

private:
  Bool_t	flagDisplayHitsInTrackerFrameInUmOverlayed_Init;
  TCanvas	*canvasDisplayHitsInTrackerFrameInUmOverlayed;
  TPaveLabel	*labelDisplayHitsInTrackerFrameInUmOverlayed;
  TPad		*padDisplayHitsInTrackerFrameInUmOverlayed;
  TH2I		**histoDisplayHitsInTrackerFrameInUmOverlayed;
  Bool_t	*flagsDisplayHitsInTrackerFrameInUmOverlayed_plan2disp;
  Int_t         eventsProcessedDisplayHitsInTrackerFrameInUmOverlayed;

  //****************************************************************
  //		Tracks in 3D (obviously in Tracker Frame [um])
  //****************************************************************
public:
  Int_t DisplayTracksIn3D_Init(void);
  Int_t DisplayTracksIn3D_Show(Bool_t DrawTracks=1, Bool_t DrawSensors=1, Bool_t DrawTrackedHits=1, Bool_t DrawAllHits=1);
  Int_t DisplayTracksIn3D_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayTracksIn3D_Reset(void);
  Int_t DisplayTracksIn3D_Kill(void);

private:
  Bool_t 		flagDisplayTracksIn3D_Init;
  TCanvas		*canvasDisplayTracksIn3D;
  TPaveLabel		*labelDisplayTracksIn3D;
  TPad			*padDisplayTracksIn3D;
  TH3I			*histoDisplayTracksIn3D;
  Double_t 		DisplayTracksIn3DViewLimitZmin,DisplayTracksIn3DViewLimitZmax;
  Double_t 		DisplayTracksIn3DViewLimitXmin,DisplayTracksIn3DViewLimitXmax;
  Double_t 		DisplayTracksIn3DViewLimitYmin,DisplayTracksIn3DViewLimitYmax;

  //****************************************************************
  //		Vertices in 3D (obviously in Beam Frame [um])
  //****************************************************************
public:
  Int_t DisplayVerticesIn3D_Init(void);
  Int_t DisplayVerticesIn3D_Show(Bool_t DrawSensors=1, Bool_t DrawTracks=1, Bool_t DrawTrackerDir=1, Bool_t DrawTarget=1, Bool_t DrawBeam=1, Bool_t DrawVerticesPoints=1, Bool_t DrawVerticesPointsSmall=1, Bool_t DrawAcceptance=1);
  Int_t DisplayVerticesIn3D_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayVerticesIn3D_Reset(void);
  Int_t DisplayVerticesIn3D_Kill(void);

private:
  Bool_t 		flagDisplayVerticesIn3D_Init;
  TCanvas		*canvasDisplayVerticesIn3D;
  TPaveLabel		*labelDisplayVerticesIn3D;
  TPad			*padDisplayVerticesIn3D;
  TH3I			*histoDisplayVerticesIn3D;
  // Draw objects
  TPolyLine3D 		*linesTraget[12]; // 3 times 4 lines to draw a cube
  TPolyLine3D 		*lineBeamBeforeTraget, *lineBeam;
  TPolyLine3D 		**lineBoxPlane;
  TPolyLine3D 		*lineTrackDir;
  TPolyLine3D 		*lineTrack;
  // Geometry
  Int_t			id_i, id_j, id_k;
  Double_t 		i[2],j[2],k[2];
  Double_t 		distDisplayedTracks;
  DPrecAlign		*Beam2Tracker;
  Double_t 		DispVer3DViewLimitImin,DispVer3DViewLimitImax;
  Double_t 		DispVer3DViewLimitJmin,DispVer3DViewLimitJmax;
  Double_t 		DispVer3DViewLimitKmin,DispVer3DViewLimitKmax;
  Double_t		DispVer3D_TargetDim[3];
  Double_t		DispVer3D_TargetOrigin[3];
  Double_t		aPosition[3];
  DR3			DispVer3D_TrackerTilt, DispVer3D_TrackerTranslation;
  DR3 			pointsBeamDirBeamFrame[2],pointsTrackTrackFrame[2], pointsTrackBeamFrame[2], Intersection;

//______________________________________________________________________________________________________
//
//	Display statistics
//______________________________________________________________________________________________________
  //****************************************************************
  //            CMOS overflow lines
  //****************************************************************
public:
  Int_t DisplayCmosOverFlow_Init(void);
  Int_t DisplayCmosOverFlow_Fill(void);
  Int_t DisplayCmosOverFlow_Show();
  Int_t DisplayCmosOverFlow_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayCmosOverFlow_Reset(void);
  Int_t DisplayCmosOverFlow_Kill(void);

private:
  Bool_t        flagDisplayCmosOverFlow_Init;
  Int_t         *DisplayCmosOverFlow_PlanesDrawOrder;
  TCanvas       *canvasDisplayCmosOverFlow;
  TPaveLabel    *labelDisplayCmosOverFlow;
  TPad          *padDisplayCmosOverFlow;
  TH1I          **histoDisplayCmosOverFlowRows,**histoDisplayCmosOverFlowRowsNb;
  Int_t         eventsProcessedDisplayCmosOverFlow;
  
  //****************************************************************
  //            Clusters size (a hit is the center of a cluster)
  //****************************************************************
public:
  Int_t DisplayClustersSize_Init(Int_t nbBinX = 30);
  Int_t DisplayClustersSize_Fill(void);
  Int_t DisplayClustersSize_Show(Bool_t All=1,Bool_t Tracked=1, Bool_t Untracked=1);
  Int_t DisplayClustersSize_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayClustersSize_SaveHistos();
  Int_t DisplayClustersSize_Reset(void);
  Int_t DisplayClustersSize_Kill(void);

private:
  Bool_t        flagDisplayClustersSize_Init;
  Int_t         *DisplayClustersSize_PlanesDrawOrder;
  TCanvas       *canvasDisplayClustersSize;
  TPaveLabel    *labelDisplayClustersSize;
  TPad          *padDisplayClustersSize;
  TH1I          **histoDisplayClustersSizeAll,**histoDisplayClustersSizeTracked, **histoDisplayClustersSizeUnTracked;
  Int_t         eventsProcessedDisplayClustersSize;
  
  //****************************************************************
  //		Hits By EVent
  //****************************************************************
public:
  Int_t DisplayHitsByEvent_Init(Int_t nbBinX = 100);
  Int_t DisplayHitsByEvent_Fill(void);
  Int_t DisplayHitsByEvent_Show(void);
  Int_t DisplayHitsByEvent_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayHitsByEvent_Reset(void);
  Int_t DisplayHitsByEvent_Kill(void);

private:
  Bool_t	flagDisplayHitsByEvent_Init;
  Int_t		*DisplayHitsByEvent_PlanesDrawOrder;
  TCanvas	*canvasDisplayHitsByEvent;
  TPaveLabel	*labelDisplayHitsByEvent;
  TPad		*padDisplayHitsByEvent;
  TH1I		**histoDisplayHitsByEvent;
  Int_t         eventsProcessedDisplayHitsByEvent;

  //****************************************************************
  //		Association of hits to tracks
  //****************************************************************
public:
  Int_t DisplayHitsTracksAssociation_Init(Int_t nbBinX = 30);
  Int_t DisplayHitsTracksAssociation_Fill(void);
  Int_t DisplayHitsTracksAssociation_Show(void);
  Int_t DisplayHitsTracksAssociation_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayHitsTracksAssociation_SaveHistos(void);
  Int_t DisplayHitsTracksAssociation_Reset(void);
  Int_t DisplayHitsTracksAssociation_Kill(void);

private:
  Bool_t	flagDisplayHitsTracksAssociation_Init;
  Bool_t        flagDisplayTracksAndTriggersProperties_withTrigger;
  Int_t		*DisplayHitsTracksAssociation_PlanesDrawOrder;
  TCanvas	*canvasDisplayHitsTracksAssociation;
  TPaveLabel	*labelDisplayHitsTracksAssociation;
  TPad		*padDisplayHitsTracksAssociation;
  TH1I		**histoDisplayHitsTracksAssociation;
  Int_t         eventsProcessedDisplayHitsTracksAssociation;

  //****************************************************************
  //		Tracks and Triggers Properties
  //****************************************************************
public:
  Int_t DisplayTracksAndTriggersProperties_Init(Bool_t withTriggers=0, Int_t Chi2Max=10000,Int_t SlopeMax=30);
  Int_t DisplayTracksAndTriggersProperties_Fill(void);
  Int_t DisplayTracksAndTriggersProperties_Show(void);
  Int_t DisplayTracksAndTriggersProperties_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayTracksAndTriggersProperties_SaveHistos(void);
  Int_t DisplayTracksAndTriggersProperties_Reset(void);
  Int_t DisplayTracksAndTriggersProperties_Kill(void);

private:
  Bool_t 		flagDisplayTracksAndTriggersProperties_Init;
  TCanvas		*canvasDisplayTracksAndTriggersProperties;
  TPaveLabel		*labelDisplayTracksAndTriggersProperties;
  TPad			*padDisplayTracksAndTriggersProperties;
  TH1D			*histoDisplayTracksAndTriggersProperties_Chi2;
  TH1D			*histoDisplayTracksAndTriggersProperties_DistTr2Hit;
  TH1D			*histoDisplayTracksAndTriggersProperties_SlopesX;
  TH1D			*histoDisplayTracksAndTriggersProperties_SlopesY;
  TH1I			*histoDisplayTracksAndTriggersProperties_HitsByTrack;
  TH1I			*histoDisplayTracksAndTriggersProperties_TracksByEvent;
  TH1I			*histoDisplayTracksAndTriggersProperties_TriggersByEvent;
  TH1I			*histoDisplayTracksAndTriggersProperties_TriggersPosition;
  Int_t                 eventsProcessedDisplayTracksAndTriggersProperties;

  //****************************************************************
  //		Display Chronogram : Hits Tracks Triggers numbers vs time/events
  //****************************************************************
public:
  Int_t DisplayChronogram_Init(Bool_t withTriggers=0);
  Int_t DisplayChronogram_Fill(void);
  Int_t DisplayChronogram_Show(void);
  Int_t DisplayChronogram_Save(vector<TString> &vectorStringFilesExtensions);
  Int_t DisplayChronogram_Reset(void);
  Int_t DisplayChronogram_Kill(void);

private:
  Bool_t 		flagDisplayChronogram_Init;
  Bool_t                flagDisplayChronogramWithTriggers;
  TCanvas		*canvasDisplayChronogram;
  TPaveLabel		*labelDisplayChronogram;
  TPad			*padDisplayChronogram;
  TGraph		*graphDisplayChronogram_TracksByEvent, **graphDisplayChronogram_HitsByEvent, **graphDisplayChronogram_OFlinesNbByEvent, *graphDisplayChronogram_TriggersByEvent;
  vector<Int_t>		vectorStatsAlongEvents_eventNb;
  vector< vector<Int_t> > vectorStatsAlongEvents_HitsNb;
  vector< vector<Int_t> > vectorStatsAlongEvents_OFlinesNb;
  vector<Int_t>		vectorStatsAlongEvents_nbOfTracks;
  vector<Int_t>		vectorStatsAlongEvents_nbOfTriggers;
  TLegend               *legendDisplayChronogram_HitsByEvent;
  TLegend               *legendDisplayChronogram_OFlinesNbByEvent;
  Int_t                 eventsProcessedDisplayChronogram;
  Int_t                 maxOFlinesDisplayChronogram;

//______________________________________________________________________________________________________
//
//	Store and compute functions
//______________________________________________________________________________________________________
  //****************************************************************
  //		Store tracks in memory / save on HDD if asked
  //****************************************************************
public :
    Int_t StoreTracks_Init(void);
    Int_t StoreTracks_Fill(Bool_t FillHitsVector=1);
    Int_t StoreTracks_Reset(void);
    Int_t StoreTracks_Kill(void);
    Int_t StoreTracks_SaveTracks(TString &filenameSaveTracks);

private :
    Bool_t flagStoreTracks_Init;

    struct hitsStruct
    {
      vector<DR3>	HitsPositions;
      vector<Int_t>	HitsPlanes;
    };
    hitsStruct		aHitsStruct;
    vector<hitsStruct>	hitsVector;

    struct trackStruct
    {
      Int_t		TracksNbInEvent;
      DR3 		TrackOrigin;
      DR3 		TrackSlopeZ;
      Double_t		TrackChi2;
      vector<DR3>	HitsPositions;
      vector<Int_t>	HitsPlanes;
    };
    trackStruct		aTrackStruct;
    vector<trackStruct>	tracksVector;

    struct trackStructExport
    {
      Int_t		TracksNbInEvent;
      Double_t 		TrackOrigin[3];
      Double_t 		TrackSlopeZ[3];
      Double_t		TrackChi2;
      Int_t		HitsNbInTrack;
    };
    trackStructExport	aTrackStructExport;
    
    Int_t       eventsProcessedStoreTracks;

  //****************************************************************
  //		Compute Vertices
  //****************************************************************
public :
    Int_t ComputeVertices_Init(void);
    Int_t ComputeVertices_Do(void);
    Int_t ComputeVertices_Reset(void);
    Int_t ComputeVertices_Kill(void);
    Int_t ComputeVertices_SaveVertices(TString &filenameSaveVertices);

private :
    Bool_t flagComputeVertices_Init;
    Bool_t flagComputeVertices_Done;

    Char_t ComputeVertices_experimentGeometry[1000];
    Char_t ComputeVertices_IntersectionMethod[1000];

    struct vertexStruct
    {
      Int_t		TracksNbInEvent;
      DR3 		TrackOriginTrFr;
      DR3 		TrackSlopeZTrFr;
      Double_t		TrackChi2;
      Int_t		HitsNbInTrack;
      DR3		VertexPositionBeamFr;
      Double_t		VertexSegmentSize;
    };
    vertexStruct	aVertexStruct;
    vector<vertexStruct>verticesVector;

    struct vertexStructExport
    {
      Int_t		TracksNbInEvent;
      Double_t 		TrackOriginTrFr[3];
      Double_t 		TrackSlopeZTrFr[3];
      Double_t		TrackChi2;
      Int_t		HitsNbInTrack;
      Double_t		VertexPositionBeamFr[3];
      Double_t		VertexSegmentSize;
    };
    vertexStructExport	aVertexStructExport;

 //****************************************************************
  //		Misc
  //****************************************************************
public:
  void ComputeOverlap(void);
  Int_t GenerateVetoPixelsCode(UInt_t absoluteThreshold = 100);



//______________________________________________________________________________________________________
//
//	PRINT Statistics & Numbers
//______________________________________________________________________________________________________

  //****************************************************************
  //		Print Cumulated Statistics Along events
  //****************************************************************
public:
  void PrintCumulatedStatistics_Reset(void);
  void PrintCumulatedStatistics_Fill(void);
  void PrintCumulatedStatistics_Print(void);

private :
  struct structStatsCumulatedAlongEvents
  {
    Int_t		eventsAnalysed;
    Int_t		triggersTotal;
    Int_t		triggersEventsWith;
    Int_t		triggersEventsWithout;
    Int_t		tracksTotal;
    Int_t		tracksEventsWith;
    Int_t		tracksEventsWithout;
    Int_t		hitsTotalAllPlanes;
    Int_t 		hitsTotalByPlane[20];
  };
  structStatsCumulatedAlongEvents astructStatsCumulatedAlongEvents ;

  //****************************************************************
  //		Print Event Informations
  //****************************************************************
public :
    void  PrintEventInfos(void);
    void  PrintPXIeStats(void);
    Int_t PrintCurrentEventNumber(void);
    Int_t GetCurrentEventNumber(void);
    Int_t PrintLogFile(void);

//______________________________________________________________________________________________________
//
//            Events navigation and processing
//______________________________________________________________________________________________________
//
  //****************************************************************
  //    Events navigation (without processing !)
  //****************************************************************
public:
  Int_t GoToNextEvent(Bool_t updateTracker=1);
  Int_t GoToAnEvent(Int_t EventNbToGo, Bool_t updateTracker=1);
  Int_t GoToNextEventWithHits(Int_t hitsNbMinimumToStop=1);
  Int_t GoToNextEventWithTracks(Int_t tracksNbMinimumToStop=1);
  Int_t SkipEvents(Int_t nbEventsToSkip=1, Bool_t updateTracker=1);

//****************************************************************
//    Events processing 
//****************************************************************
public:
  Int_t ProcessEvents(Int_t aNbOfEvents=1,Bool_t resetBefore=1, Bool_t showDisplays=0);
  Int_t ProcessAllEvents(Int_t refreshDisplaysEveryXevents=0, Bool_t resetBefore=1);

 //****************************************************************
  //	Global Actions
  //****************************************************************
public:
  Int_t FillAllDisplays(void);
  Int_t ShowAllDisplays(void);
  Int_t SaveAllDisplays(const Char_t *fileExtension1=NULL, const Char_t *fileExtension2=NULL, const Char_t *fileExtension3=NULL, const Char_t *fileExtension4=NULL, const Char_t *fileExtension5=NULL, const Char_t *fileExtension6=NULL);
  Int_t SaveAllDisplays(vector<TString> &aVectorStringFilesExtensions /*= vector<TString>()*/);
  Int_t SaveAllHistograms(void);
  Int_t SaveResults(void);
  Int_t ResetAllDisplays(void);
  Int_t KillAllDisplays(void);


//______________________________________________________________________________________________________
//
//	      GUI
//______________________________________________________________________________________________________

  //****************************************************************
  //		GUI fuctions <-> button
  //****************************************************************
  // Analysis control tab
  //	Analysis control group frame
  //		Process Events zone
  //****************************************************************
public: //GuiButton MUST be public
  Int_t GuiButtonProcess1event(void);
  Int_t GuiButtonProcess10events(void);
  Int_t GuiButtonProcess100events(void);
  Int_t GuiButtonProcessXevents(void);
  Int_t GuiButtonProcessAllEvents(void);

  //****************************************************************
  //		Go to event with ... zone
  //****************************************************************
public: //GuiButton MUST be public
  Int_t  GuiButtonGoToEventWithNtracks(void);
  Int_t GuiButtonGoToEventWithNhits(void);
  Int_t GuiButtonGoToEventNb(void);

  //****************************************************************
  //		Actions zone
  //****************************************************************
public: //GuiButton MUST be public
  Int_t GuiButtonToggleVerbosity(void);
  Int_t GuiButtonSetTafDebugLevel(void);
  Int_t GuiButtonResetDisplays(void);
  Int_t GuiButtonSaveDisplays(void);
  Int_t GuiButtonSaveHistograms(void);
  Int_t GuiButtonSaveResults(void);
  Int_t GuiButtonQuitTaf(void);

  //****************************************************************
  //		GUI SubFunctions
  //****************************************************************
public:
  Int_t GuiProcessEvents(Int_t nbEventsToProcess=1, Int_t refreshDisplaysEveryXevents=0/*see GuiProcessEvents()*/);
  Int_t GuiInitDisplays(void);
  Int_t GuiShowDisplays(void);
  Int_t GuiResetDisplays(void);
  Int_t GuiReProcessLastEvent(void);
  Int_t GuiActualizeEventNumber(void);
  Int_t GuiActualizeTextsDislays(void);

  //****************************************************************
  // Allignment tab
  //****************************************************************
public: //GuiButton MUST be public
  Int_t GuiButtonAlignResetValues(void);
  Int_t GuiButtonAlignDoTranslateTurn(void);
  Int_t GuiButtonAlignDisplayPosTilt(void);
  Int_t GuiButtonAlignUpdatePosTilt(void);
  Int_t GuiButtonAlignUpdateCfgFile(void);
  Int_t GuiButtonRefreshDisplays(void);

  //****************************************************************
  //            Noise tab 
  //****************************************************************
  Int_t GuiButtonGenerateVetoPixelsCode(void);
    
//
  //****************************************************************
  //            GUI windows : Lauch, Display, Connect
  //****************************************************************
public :
  Int_t GuiLauch(void);
private :
  Int_t GuiCreate(void);
  Int_t GuiConnectButtons(void);

  //****************************************************************
  //            GUI Create
  //****************************************************************

private :
    // Display Event /
    //          / Views selection /
    //                            / Raw data /
    TGNumberEntry *fNumberEntry_RawMinPixOccu;
    //                                       / CMOS native frame
    TGCheckButton *fTextButtonDisRawDatCmoNatGeoInPix, *fTextButtonDisRawDatCmoNatGeoInPix_Cumul, *fTextButtonDisRawDatCmoNatGeoInPix_Hit, *fTextButtonDisRawDatCmoNatGeoInPix_OlLine, *fTextButtonDisRawDatCmoNatGeoInPix_ColBar;
    //                                       / Tracker Frame
    TGCheckButton *fTextButtonDisRawDatTrackerFraInPix, *fTextButtonDisRawDatTrackerFraInPix_Cumul, *fTextButtonDisRawDatTrackerFraInPix_Hit, *fTextButtonDisRawDatTrackerFraInPix_OlLine, *fTextButtonDisRawDatTrackerFraInPix_ColBar;
    //                            / Hits in Pix   /
    TGNumberEntry *fNumberEntry_HitMinOccu;
    //                                       / CMOS native frame
    TGCheckButton *fTextButtonDisHitsCmoNatGeoInPix, *fTextButtonDisHitsCmoNatGeoInPix_Cumul, *fTextButtonDisHitsCmoNatGeoInPix_HitShowedAsPix, *fTextButtonDisHitsCmoNatGeoInPix_OlLine, *fTextButtonDisHitsCmoNatGeoInPix_AssoTrack;
    //                                       / Tracker Frame
    TGCheckButton *fTextButtonDisHitsInTrackerFrameInPix, *fTextButtonDisHitsInTrackerFrameInPix_Cumul, *fTextButtonDisHitsInTrackerFrameInPix_HitShowedAsPix, *fTextButtonDisHitsInTrackerFrameInPix_OlLine, *fTextButtonDisHitsInTrackerFrameInPix_AssoTrack;
    //                          / Hits in Um    /
    //                                       / Tracker Frame
    TGCheckButton *fTextButtonDisHitsInTrackerFrameInUm, *fTextButtonDisHitsInTrackerFrameInUm_Cumul;
    TGButtonGroup *fGButtonGroupHitTrackInter;
    TGRadioButton *fGRadioButton_HitTrackInter_none,*fGRadioButton_HitTrackInter_cross;
    //                                      / Overlay view
    TGCheckButton *fTextButtonDisHitsInTrackerFrameInUmOv, *fTextButtonDisHitsInTrackerFrameInUmOv_Cumul;
    TGListBox     *fListBoxOverlayHitsInTrackerFrameInUm;
    TList         *fListOverlayHitsInTrackerFrameInUm;
    //                          / Tracks
    //                                  / 3D View
    TGCheckButton *fTextButtonDisTracks3D, *fTextButtonDisTracks3D_Cumul,  *fTextButtonDisTracks3D_DrawSensor,*fTextButtonDisTracks3D_DrawAllHits, *fTextButtonDisTracks3D_DrawTrackedHits, *fTextButtonDisTracks3D_DrawTracks;
    //                          / Vertices
    TGCheckButton *fTextButtonDisVertices, *fTextButtonDisVertices_Cumul, *fTextButtonDisVertices_DrawSensor, *fTextButtonDisVertices_DrawTracks, *fTextButtonDisVertices_DrawTrackDir, *fTextButtonDisVertices_DrawTarget;
    TGCheckButton *fTextButtonDisVertices_DrawBeam, *fTextButtonDisVertices_DrawVerticesPoints, *fTextButtonDisVertices_DrawVerticesPointsSmall, *fTextButtonDisVertices_SaveTree,*fTextButtonDisVertices_DrawAcceptance;
    //                          / Statistics
    //                                  / CMOS
    TGCheckButton *fTextButtonDisCMOSoverflowLines, *fTextButtonDisCMOSoverflowLines_Cumul;
    //                                  / Clusters
    TGCheckButton *fTextButtonDisHitsSize, *fTextButtonDisHitsSize_Cumul,*fTextButtonDisHitsSizeAll,*fTextButtonDisHitsSizeTracked,*fTextButtonDisHitsSizeUnTracked,*fTextButtonDisHitsByEvent, *fTextButtonDisHitsByEvent_Cumul, *fTextButtonDisHitsAssociation, *fTextButtonDisHitsAssociation_Cumul;
    TGNumberEntry *fNumberEntry_HitsByEventBinMax;
    //                                  / Tracks +Triggers
    TGCheckButton *fTextButtonDisStats, *fTextButtonDisStats_Cumul, *fTextButtonDisStats_withTriggers;
    TGNumberEntry *fNumberEntry_TracksHistoChi2Max, *fNumberEntry_TracksHistoSlopeMax, *fNumberEntry_ClustersBinMax;
    //                                  / Along Events
    TGCheckButton *fTextButtonDisStatsAlongEvents, *fTextButtonDisStatsAlongEvents_Cumul,  *fTextButtonDisStatsAlongEvents_withTriggers;
    //
    // Noise
    TGCheckButton *fTextButtonNoiseDisRawDatCmoNatGeoInPix, *fTextButtonNoiseDisRawDatCmoNatGeoInPix_Hit, *fTextButtonNoiseDisRawDatCmoNatGeoInPix_ColBar;
    TGNumberEntry *fNumberEntry_NoiseRawMinPixOccu;
    TGLabel *fLabelNoiseMinPixOccuRelValue, *fLabelNoiseNbOfEvents;
    TGCheckButton *fTextButtonDisNoiseStudy;
    TGTextButton *fTextButtonGenerateVetoPixelsCode;
    TGNumberEntry  *fNumberEntry_NoiseThresholdsMin, *fNumberEntry_NoiseThresholdsMax, *fNumberEntry_NoiseThresholdsNb;
    // Alignment
    //          / Display
    TList         *fListAlignPlanesToDisp, *fListAlignPlanesToMove;
    TGListBox     *fListBoxAlignPlanesToDisp, *fListBoxAlignPlanesToMove;
    // Translate & Turn
    TGNumberEntry *fNumberEntry_AlignTranslatX, *fNumberEntry_AlignTranslatY, *fNumberEntry_AlignTranslatZ;
    TGNumberEntry *fNumberEntry_AlignTurnX, *fNumberEntry_AlignTurnY, *fNumberEntry_AlignTurnZ;
    // Positions & Tilts
    TGNumberEntry *fNumberEntry_AlignPosX, *fNumberEntry_AlignPosY, *fNumberEntry_AlignPosZ;
    TGNumberEntry *fNumberEntry_AlignTiltX, *fNumberEntry_AlignTiltY, *fNumberEntry_AlignTiltZ;
    //          / Control
    TGCheckButton *fTextButtonAlignViewOverHits, *fTextButtonAlignViewOverHits_Cumul;
    TGTextButton *fTextButtonAlignResetValues, *fTextButtonAlignDoTranslatTurn, *fTextButtonAlignDisplayActualPosTilt, *fTextButtonAlignUpdateActualPosTilt, *fTextButtonAlignUpdateCfgFile;
    TGButtonGroup *fTextButtonAlignUpdateCfgFile_UpdateMode;
    TGRadioButton *fGRadioButton_AnalyseModeUpWithBck, *fGRadioButton_AnalyseModeUpNoBck, *fGRadioButton_AnalyseModeNewFile;
    TGTextEntry *fTextEntry_NewCfgFileName;
    
    // Analysis control/
    //                          / Process events/
    TGTextButton *fTextButtonProcess1event, *fTextButtonProcess10events, *fTextButtonProcess100events, *fTextButtonProcessXevents, *fTextButtonProcessAllEvents;
    TGNumberEntry *fNumberEntry_EventsToProcess;
    //                          / Go to /
    TGTextButton *fTextButtonGoToEventWithNhits,*fTextButtonGoToEventWithNtracks,*fTextButtonGoToEventNb;
    TGNumberEntry *fNumberEntryGoToEventWithNhits, *fNumberEntryGoToEventWithNtracks;
    //                          / Action
    TGTextButton *fTextButtonTurnVerbose, *fTextButtonSaveDisplays, *fTextButtonSaveHistograms, *fTextButtonQuitTaf, *fTextButtonSetTafDebugLevel;
    TGNumberEntry *fNumberEntryTafDebugLevel;
    TGCheckButton *fTextButtonBeepWhenFinished;
    TGNumberEntry *fNumberEntry_GoToEvent;
    //                          / Display
    TGNumberEntry *fNumberEntry_RefreshFreq;
    TGTextButton *fTextButtonRefreshView, *fTextButtonResetView;
    //                          / Save
    TGCheckButton *fTextButtonSaveCanvas_inPDF,*fTextButtonSaveCanvas_inPNG, *fTextButtonSaveCanvas_inROOT;
    TGCheckButton *fTextButtonSaveTracks, *fTextButtonSaveVertice;
    TGTextEntry *fTextEntry_FilesPrefix, *fTextEntry_FilesSuffix;
    TGTextButton *fTextButtonSaveResults;
    
    
//______________________________________________________________________________________________________
//
//	MISC TOOLS
//______________________________________________________________________________________________________
//

  //****************************************************************
  //		Print messages with colors
  //****************************************************************
private :
  void printf_info (Char_t *text="");
  void printf_warning (Char_t *text="");
  void printf_error(Char_t *text="");


//______________________________________________________________________________________________________
//
//		Commons variables
//______________________________________________________________________________________________________
//
private:
  //****************************************************************
  //		Display functions
  //****************************************************************
  // ROOT Display
  Char_t name[200];
  Char_t title[200];
  Char_t canvasTitle[200];
  TCanvas *aCanvas;
  
  TString aFileName;
  TString resultsDirPath;
  TString FileNamePrefix;
  TString FileNameSuffix;
  
  TString canvasName;
  TString canvasFileFullPath;
  
  TString histosName;
  TString histosFileFullPath;
  TFile   *histosFile;
  
  TString FileNameTreeTracks;
  TString FileNameTreeVertex;
  
  TString treeTracksFileFullPath;
  TString treeVertexFileFullPath;
  
  Int_t	HitMarkerStyle_Cluster[8];
  Int_t	HitMarkerStyle_Tracked[8];
  Int_t HitMarkerStyle_UnTracked[8];
  Int_t HitMarkerColor[8];
  Int_t LineColor[8];
  Int_t TracksLinesColor[9];
  
  Bool_t configBoolVerbose;


  //****************************************************************
  //		Tracker system
  //****************************************************************
  Int_t                 eventsProcessed;
  Int_t			RunNumber;
  DTracker		*tTracker;
  DAcq                  *tDAcq;
  Int_t			nbPlanes;
  DPlane		*tPlane;
  vector<DPixel*>	*aList;
  DHit			*aHit;
  DTrack		*aTrack;
  DR3 			aHitPos;
  Int_t			iPlane, jPlane;
  Int_t			iHit;
  Int_t			iTrack;
  Int_t			iEventWithHits;
  Int_t			nbHits;
  Int_t			nbTracks;
  Int_t			nbTriggers;

  //****************************************************************
  //		Misc
  //****************************************************************
  DGlobalTools fTool;
  DSession     *fSession;


//______________________________________________________________________________________________________
//
//		MRax class gestion (contructor, init functions, ...)
//______________________________________________________________________________________________________
//
private:
    static MRax* fgInstanceRax;

public:
    MRax(DSession *aSession);
    virtual ~MRax(){;}
    static  MRax*& InstanceRax( DSession *aSession);
    ClassDef(MRax,1) // Integrate MRax classe into the ROOT system, see http://root.cern.ch/root/Using.html
};



#endif
