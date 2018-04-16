
/*******************************************************************************
File      : x:\lib\com\maps\maps.typ
Goal      : Types definition of MAPS library.
          : It provides MAPS types definition and data handling functions.
Prj date  : 17/07/2009
File date : 17/07/2009
Doc date  : //200
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef MAPS_TYP
#define MAPS_TYP


#ifdef ROOT_ROOT
  typedef UInt32 MAPS__TColor;
  #define  clWhite 0
  #define  clBlack 0
  #define  clBlue  0
  #define  clRed 0  
#else  
  typedef TColor MAPS__TColor;
#endif



// 17/07/2009

typedef struct {

  SInt32 MaxBuffEvNb;

  // Hard coded parameters in next classes

  SInt8  MapsNb;    // Nb of MAPS acquired by DAQ
  SInt8  MapsName;
  SInt8  PlaneNb;
  
  // Parameters from GUI
  
  SInt8  MonEnabled;
  
  SInt32 AcqFrNb;            // Nb of frame in current acq to process
  
  SInt8  ProcOneAcq;         // 1 => Process current ONLY acq
                             // 0 => Process ProcEvNb coming from GUI
                             // It can be more or less than one acq
  
  SInt32 ProcEvNb;           // Ev nb to process, either
                             // - if ProcOneAcq = 1 => ev nb of one acq
                             // - if ProcOneAcq = 0 => from GUI
  
  SInt8  ProcOnlyFrWithHitOnAllPlanes;
  
  SInt8  ListHitAllPlanesMode; 
  
  SInt8  ProcOnlyFrWithTrig; // Process only frames with hit
  SInt8  HandleTrigPos;      // Use trigger position to extract events
  SInt8  StopAtEndOfProc;    // Stop at end of processing, no automatic processing of next acq
  SInt8  StoreEvents;        // Store events
  
  SInt8  DispFrNbWithTrigOrFrNbWithHit; // Display in PubPt_ADispFrNbWithTrigOrHit & PubPt_ADispFrPCentWithTrigOrHit trig or hit nb
                                        // 0 => Display frame nb & % with trigger
                                        // 1 => Display frame nb & % with at least one hit
  
  SInt8  DispFullMatrix;     // 0 => Display 1/2 scale matrix
                             // 1 => Display full matrix 
  
  SInt8  ProcMode;           // Processing mode
                             // MAPS__TCDigTelMon_MODE_PLOT_PLANE_FRAME                    
                             // MAPS__TCDigTelMon_MODE_PLOT_PLANE_CUMUL
                             // MAPS__TCDigTelMon_MODE_PLOT_PLANE_COIN_PER_FRAME_6_COLORS
                             // MAPS__TCDigTelMon_MODE_PLOT_PLANE_COIN_CUMUL_1_COLOR
                             // MAPS__TCDigTelMon_MODE_PLOT_PLANE_COIN_CUMUL_6_COLORS
                             
   SInt8 CumPlotGrayOrBlueLevels; // Cumul plot
                                  // 0 => in gray levels
                                  // 1 => in blue levels
    
   SInt8 CumPlotHitOrHitCnt;      // Cumul plot
                                  // 0 => binary display : count > 0 => set pixel in black / blue
                                  // 1 => gray or blue levels display
      
  
  SInt32 GotoRawFrNo;        // Plot raw data of frame No 

} MAPS__TDigTelPar;


// 17/07/2009

typedef struct {
    
  SInt8  RunInProgress;
  SInt8  LastPlaneSelForCoin; // Index of last plane selected for coincidence
    
  SInt32 CurEvNo;        // Index of last event buffered
  SInt32 FirstEvToProc;  // Index of first event to process
  SInt32 LastEvToProc;   // Index of last event to process
  SInt32 CurEvToProc;    // Index of event to process
  SInt32 EvNbToProc;     // Ev nb to process

} MAPS__TDigTelInf;


// 17/07/2009

typedef struct {
  
  SInt32 AcqFrNb;   // Frame nb received from current acq
  SInt32 ProFrNo;   // Index of currently processed frame
  SInt32 ProTimeMs; // Processing time
    
} MAPS__TDigTelRes;


// 17/07/2009

typedef struct {

  SInt8  MapsId;     // Id of the MAPS
  MAPS__TColor PlotColor;
  
  SInt8  Process;    // Process data of this plane
  SInt8  RevertXDir; // Revert numbering of X direction ( columns ) in case planes are mounted back to back
  SInt8  PlotCum;    // Plot cumulated hit count over N events
  SInt8  PlotCoin;   // Plot coincidence
  SInt8  PlotFrame;  // Plot frame raw data
  
} MAPS__TDigTelPlanePar;


// 22/07/2009

typedef struct {
  
  SInt8 PrevProcState; // Not used on 22/07/2009
  SInt8 CurProcState;  // Not used on 22/07/2009
  
  
} MAPS__TDigPlaneInf;


// 17/07/2009

typedef struct {

  void* AEvPt[MAPS__TCDigTelMon_MAX_EV_NB];
  void* CurEvPt;
  
} MAPS__TDigTelPlaneData;


// 17/07/2009
// 20/05/2010
// - Calculation of mean trig nb / frame => Add CumTotTrigNb & CumFrameTrigNb

typedef struct {

  void*  PtMatDiscriBit;      // Flat view of matrix X col x Y lines
  void*  PtMatCumTotHitCnt;   // Hit count of each pixel over all events 

  SInt32 FrameNbWithTrig;     // Nb of frame with trigger
  float  FramePCentWithTrig;  // % of frame with trigger
  SInt32 CumTotTrigNb;        // Total number of triggers
  float  CumFrameTrigNb;      // Cumul MEAN trigger nb on frames WITH trigger
  
  SInt32 FrameNbWithHit;      // Nb of frame with at least one hit
  float  FramePCentWithHit;   // % of frame with at least one hit
  SInt32 CumTotHitNb;         // Total cumul hit nb over ALL frames ( with and without hit ) 
  float  CumFrHitNb;          // Cumul MEAN hit nb on frames WITH hit  
  
} MAPS__TDigTelPlaneRes;


// 17/07/2009
// 20/05/2010
// - Calculation of mean trig nb / frame => Add PubPt_ADispCumFrTrigNb

#ifndef ROOT_ROOT

class MAPS__TCDigTelMon {
  
  private:

    SInt8 PrivGuiConnected;
    SInt8 PrivSetGuiParDontRead;
    
    SInt32 PrivFInit ();
  
    SInt32 PrivChkPlaneId ( SInt8 Id );
  
    SInt32 PrivFSetGuiLabelsDispFrNbWithTrigOrFrNbWithHit ();
  
  protected:

    SInt8 ProRequestPlot; // Request plot from sw external to object
    
    MAPS__TDigTelPar       ProPar;
    MAPS__TDigTelInf       ProInf;
    MAPS__TDigTelRes       ProRes;
    MAPS__TDigTelPlanePar  ProAPlanePar[MAPS__TCDigTelMon_MAX_PLANE_NB];
    MAPS__TDigPlaneInf     ProAPlaneInf[MAPS__TCDigTelMon_MAX_PLANE_NB];
    MAPS__TDigTelPlaneData ProAPlaneData[MAPS__TCDigTelMon_MAX_PLANE_NB];
    MAPS__TDigTelPlaneRes  ProAPlaneRes[MAPS__TCDigTelMon_MAX_PLANE_NB];
  
  public:

      // ----------------------------------
      // Fields to connect GUI components
      // ----------------------------------
      
      // Group
      
      TGroupBox*     PubPt_GrpTelMon;
      
      // Label
      
      TLabel*        PubPt_LbFrNbWithTrigOrHit;
      TLabel*        PubPt_LbFrPCentWithTrigOrHit;
      
      // Controls
      
      TCheckBox*     PubPt_ChkEnableMonitor;
      
      TCheckBox*     PubPt_AChkProc[MAPS__TCDigTelMon_MAX_PLANE_NB];
      TCheckBox*     PubPt_AChkRevertXDir[MAPS__TCDigTelMon_MAX_PLANE_NB];
      
      TCheckBox*     PubPt_ChkProcOneAcq;
      TEdit*         PubPt_EdFrNbToProc;
      TCheckBox*     PubPt_ChkProcOnlyFrWithTrig;
      
      TComboBox*     PubPt_CbListHitAllPlanesMode;
      
      TCheckBox*     PubPt_ChkTrigPosHanling;
      TCheckBox*     PubPt_ChkStopAtEndOfProc;
      TCheckBox*     PubPt_ChkStoreFr;
      
      TCheckBox*     PubPt_ChkDispFrNbWithTrigOrFrNbWithHit;
      TCheckBox*     PubPt_ChkDispFullMatrix;
      
      TComboBox*     PubPt_CbProcMode;
      TCheckBox*     PubPt_ChkCumPlotGrayOrBlueLevels;
      TCheckBox*     PubPt_ChkCumPlotHitOrHitCnt;
      
      TEdit*         PubPt_EdGotoFr;
      
      TLabel*        PubPt_ALbPlane[MAPS__TCDigTelMon_MAX_PLANE_NB];
      
      TRadioButton*  PubPt_ARbPlotCum[MAPS__TCDigTelMon_MAX_PLANE_NB];
      TCheckBox*     PubPt_AChkPlotCoin[MAPS__TCDigTelMon_MAX_PLANE_NB];
      TRadioButton*  PubPt_ARdPlotFr[MAPS__TCDigTelMon_MAX_PLANE_NB];
      
      // Indicators
      
      TEdit*         PubPt_DispAcqFrNb;
      TEdit*         PubPt_DispCurProcFr;
      
      TEdit*         PubPt_ADispFrNbWithTrigOrHit[MAPS__TCDigTelMon_MAX_PLANE_NB];
      TEdit*         PubPt_ADispFrPCentWithTrigOrHit[MAPS__TCDigTelMon_MAX_PLANE_NB];
      
      TEdit*         PubPt_ADispCumFrTrigNb[MAPS__TCDigTelMon_MAX_PLANE_NB];
      TEdit*         PubPt_ADispCumTotHitNb[MAPS__TCDigTelMon_MAX_PLANE_NB];
      TEdit*         PubPt_ADispCumFrHitNb[MAPS__TCDigTelMon_MAX_PLANE_NB];
      
      TEdit*         PubPt_DispProFrNo;
      TEdit*         PubPt_DispProcTimeMs;
      
      // Display
      
      TImage*        PubPt_ImgMiniMatrix;
      TImage*        PubPt_ImgFullMatrix;
      
          
    // -------------
    // Methods
    // -------------
    

    MAPS__TCDigTelMon ();
    ~MAPS__TCDigTelMon ();
  
    SInt32 PubFBegin ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl );

    
    // GUI interface
    
    SInt32 PubFConnectGui ();
    
    SInt32 PubFGuiEnablePlane ( SInt8 Id, SInt8 Enable );
    
    SInt32 PubFSetGuiPar ();
    SInt32 PubFSetGuiParGotoFr ();
      
    SInt32 PubFGetGuiPar ();
    SInt32 PubFOnGuiParDispModeChangeFrNbWithTrigOrFrNbWithHit (); // Specific GUI behaviour
    SInt32 PubFSetGuiStatus ();
    SInt32 PubFSetGuiRes ();
    
    
    // Set parameters
  
    SInt32 PubFSetPar       ( MAPS__TDigTelPar* Pt );
    SInt32 PubFSetPlanePar  ( SInt8 Id, MAPS__TDigTelPlanePar*  Pt );

    // Get parameters

    MAPS__TDigTelPar*       PubFGetPar       ();
    MAPS__TDigTelPlanePar*  PubFGetPlanePar  ( SInt8 Id );
    MAPS__TDigTelPlaneData* PubFGetPlaneData ( SInt8 Id );
        
    // Disable all planes processing
    
    SInt32 PubFDisAllPlanesProcessing ();
    
    // Disable group
    
    SInt32 PubFDisCumPlot ();
    SInt32 PubFDisCoinPlot ();
    SInt32 PubFDisFramePlot ();
    
    // Set results
    
    MAPS__TDigTelPlaneRes*  PubFSetPlaneRes  ( SInt8 Id );
    
    // Poll plot request
    
    SInt32 PubFGetPlotRqStatus ();
    SInt32 PubFResetPlotRqStatus ();
    
    SInt8* PubFGetPtPlotRq  ();
    SInt8* PubFGetPtMonEnabled   ();
    
    
    // Debug print
  
    SInt32 PubFPrintPar ();
    SInt32 PubFPrintInf ();
    
    SInt32 PubFPrintPlanePar  ( SInt8 Id );
    SInt32 PubFPrintAllPlanesPar ();
    
    SInt32 PubFPrintPlaneInf  ( SInt8 Id );
    SInt32 PubFPrintAllPlanesInf ();
    
    SInt32 PubFPrintPlaneData ( SInt8 Id );
    SInt32 PubFPrintAllPlanesData ();
    
    SInt32 PubFPrintPlaneRes  ( SInt8 Id );
    SInt32 PubFPrintAllPlanesRes  ();
    
    
};

#endif


/* ============================= */
/*  Lib context                  */
/*  Contain all global variables */
/* ----------------------------- */
/* Date : 17/07/2009             */
/* ============================= */

typedef struct {

  SInt8 Dummy;

} MAPS__TContext;



#endif
