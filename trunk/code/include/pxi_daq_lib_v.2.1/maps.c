
/*******************************************************************************
File      : x:\lib\com\maps\maps.c
Goal      : Functions of MAPS library.
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


#ifndef MAPS_C
#define MAPS_C


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : 
          :
Goal      : 
          :
Inputs    :
          :
Ouputs    : 
          :
Globals   : 
          :
Remark    :
          :
Level     : 
Date      : //2004
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 24/11/2008
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */                          


#ifndef ROOT_ROOT


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Rev       : 20/05/2010
          : - Set PubPt_ADispCumFrTrigNb items to NULL because they willnot be available
          : on previous GUI version => they will be updated by PubFSetGuiRes ONLY if <> NULL.
          :
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 MAPS__TCDigTelMon::PrivFInit () {
    
  SInt8 Vi;  
    
  // Reset all parameters and results fields

  memset ( &ProPar      , 0, sizeof (MAPS__TDigTelPar) );
  memset ( &ProInf      , 0, sizeof (MAPS__TDigTelInf) );
  memset ( ProAPlanePar , 0, MAPS__TCDigTelMon_MAX_PLANE_NB * sizeof (MAPS__TDigTelPlanePar) );
  memset ( ProAPlaneData, 0, MAPS__TCDigTelMon_MAX_PLANE_NB * sizeof (MAPS__TDigTelPlaneData) );
  memset ( ProAPlaneRes , 0, MAPS__TCDigTelMon_MAX_PLANE_NB * sizeof (MAPS__TDigTelPlaneRes) );

  // Set default values for flags

  PrivGuiConnected      = -1;
  PrivSetGuiParDontRead = 0;
  ProRequestPlot        = 0;

  // Set default values for GUI parameters

  ProPar.ProcOneAcq = 1;
  ProPar.ProcEvNb   = 100;


  for ( Vi=0; Vi < MAPS__TCDigTelMon_MAX_PLANE_NB; Vi++ ) {
    ProAPlaneInf[Vi].PrevProcState = -1;
    
    PubPt_ADispCumFrTrigNb[Vi] = NULL;
  }

}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PrivChkPlaneId ( SInt8 Id ) {
  
  if ( (Id < 0 ) ||(Id >= MAPS__TCDigTelMon_MAX_PLANE_NB) ) {
    return (-1);
  }
  
  return (0);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 05/08/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PrivFSetGuiLabelsDispFrNbWithTrigOrFrNbWithHit () {
    
  // Display frame nb with trigger
  
  if ( ProPar.DispFrNbWithTrigOrFrNbWithHit == 0 ) {
    PubPt_LbFrNbWithTrigOrHit->Caption    = "Frames nb with trig";
    PubPt_LbFrPCentWithTrigOrHit->Caption = "Frames % with trig";
  }
  
  // Display frame nb with hit
  
  else {
    PubPt_LbFrNbWithTrigOrHit->Caption    = "Frames nb with hit";
    PubPt_LbFrPCentWithTrigOrHit->Caption = "Frames % with hit";
  }
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

MAPS__TCDigTelMon::MAPS__TCDigTelMon () {
  
  err_error (( ERR_OUT, "TRACE" ));
  
  PrivFInit ();
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

MAPS__TCDigTelMon::~MAPS__TCDigTelMon () {    
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFBegin ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl ) {
  


  err_retok (( ERR_OUT, "" ));
}

// GUI BEGIN


    
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFConnectGui () {

  SInt8 Vi;

  PrivGuiConnected = 1;

  // Set default paramters values

  PubFSetGuiPar ();
  
  // Disable all planes processing
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++) {
    PubFGuiEnablePlane (Vi, 0);
  }
  
  
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 21/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFGuiEnablePlane ( SInt8 Id, SInt8 Enable ) {
  
  MAPS__TCDigTelMon_CHK_PLANE_ID (Id);
  
  PubPt_ARbPlotCum[Id]->Enabled   = (bool) Enable;
  PubPt_AChkPlotCoin[Id]->Enabled = (bool) Enable;
  PubPt_ARdPlotFr[Id]->Enabled    = (bool) Enable;
  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFSetGuiPar () {
  
  SInt8 Vi;
    
  err_retfail ( PrivGuiConnected, (ERR_OUT,"GUI not connected !") );
  
  PrivSetGuiParDontRead = 1;

  PubPt_ChkEnableMonitor->Checked = (bool) ProPar.MonEnabled;
  

  FDecInt2Edit ( ProPar.ProcEvNb, PubPt_EdFrNbToProc );
  FDecInt2Edit ( ProPar.GotoRawFrNo, PubPt_EdGotoFr );
  
  PubPt_ChkProcOneAcq->Checked                    = (bool) ProPar.ProcOneAcq;
  PubPt_ChkProcOnlyFrWithTrig->Checked            = (bool) ProPar.ProcOnlyFrWithTrig;
  
  PubPt_CbListHitAllPlanesMode->ItemIndex         = ProPar.ListHitAllPlanesMode;
  
  PubPt_ChkTrigPosHanling->Checked                = (bool) ProPar.HandleTrigPos;
  PubPt_ChkStopAtEndOfProc->Checked               = (bool) ProPar.StopAtEndOfProc;
  PubPt_ChkStoreFr->Checked                       = (bool) ProPar.StoreEvents;
  PubPt_ChkDispFrNbWithTrigOrFrNbWithHit->Checked = (bool) ProPar.DispFrNbWithTrigOrFrNbWithHit;
  
  PubPt_ChkDispFullMatrix->Checked     = (bool) ProPar.DispFullMatrix;
  PubPt_ChkCumPlotGrayOrBlueLevels->Checked  = (bool) ProPar.CumPlotGrayOrBlueLevels;
  PubPt_ChkCumPlotHitOrHitCnt->Checked       = (bool) ProPar.CumPlotHitOrHitCnt;

  PubPt_CbProcMode->ItemIndex          = ProPar.ProcMode;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    PubPt_AChkProc[Vi]->Checked       = (bool) ProAPlanePar[Vi].Process;
    PubPt_AChkRevertXDir[Vi]->Checked = (bool) ProAPlanePar[Vi].RevertXDir;
    PubPt_ARbPlotCum[Vi]->Checked     = (bool) ProAPlanePar[Vi].PlotCum;
    PubPt_AChkPlotCoin[Vi]->Checked   = (bool) ProAPlanePar[Vi].PlotCoin;
    PubPt_ARdPlotFr[Vi]->Checked      = (bool) ProAPlanePar[Vi].PlotFrame;
  }
  
  
  PrivFSetGuiLabelsDispFrNbWithTrigOrFrNbWithHit ();

  PrivSetGuiParDontRead = 0;
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 26/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFSetGuiParGotoFr () {
  
  SInt8 Vi;
  
  
  err_retfail ( PrivGuiConnected, (ERR_OUT,"GUI not connected !") );
  
  PrivSetGuiParDontRead = 1;

  FDecInt2Edit ( ProPar.GotoRawFrNo, PubPt_EdGotoFr );
    
  PrivSetGuiParDontRead = 0;
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFGetGuiPar () {
  
  SInt8 Vi;
  
  err_retfail ( PrivGuiConnected, (ERR_OUT,"GUI not connected !") );
    
  if ( PrivSetGuiParDontRead ) {
    return (0);
  }


  ProPar.MonEnabled = (SInt8) PubPt_ChkEnableMonitor->Checked;

  ProPar.ProcOnlyFrWithTrig     = (SInt8) PubPt_ChkProcOnlyFrWithTrig->Checked;
  
  if ( ProPar.ProcOnlyFrWithTrig == 1 ) {
    PrivSetGuiParDontRead = 1;
    
    ProPar.ProcOneAcq = 0;   
    PubPt_ChkProcOneAcq->Checked = false;
    PubPt_ChkProcOneAcq->Enabled = false;

    PrivSetGuiParDontRead = 0;
  }
  
  else {
    PubPt_ChkProcOneAcq->Enabled = True;
    ProPar.ProcOneAcq            = (SInt8) PubPt_ChkProcOneAcq->Checked;
  }

  ProPar.ListHitAllPlanesMode   =  PubPt_CbListHitAllPlanesMode->ItemIndex;
    
  ProPar.ProcEvNb    = FEdit2DecInt ( PubPt_EdFrNbToProc );
  ProPar.GotoRawFrNo = FEdit2DecInt ( PubPt_EdGotoFr     );
  
  ProPar.HandleTrigPos          = (SInt8) PubPt_ChkTrigPosHanling->Checked;
  ProPar.StopAtEndOfProc        = (SInt8) PubPt_ChkStopAtEndOfProc->Checked;
  ProPar.StoreEvents            = (SInt8) PubPt_ChkStoreFr->Checked;
  ProPar.DispFullMatrix         = (SInt8) PubPt_ChkDispFullMatrix->Checked;
  
  ProPar.DispFrNbWithTrigOrFrNbWithHit = (SInt8) PubPt_ChkDispFrNbWithTrigOrFrNbWithHit->Checked;
  
  
  ProPar.ProcMode               = (SInt8) PubPt_CbProcMode->ItemIndex;

  ProPar.CumPlotGrayOrBlueLevels = (SInt8) PubPt_ChkCumPlotGrayOrBlueLevels->Checked;
  ProPar.CumPlotHitOrHitCnt      = (SInt8) PubPt_ChkCumPlotHitOrHitCnt->Checked;


  ProInf.LastPlaneSelForCoin = -1;

  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
        
    ProAPlanePar[Vi].Process    = (SInt8) PubPt_AChkProc[Vi]->Checked;
    ProAPlanePar[Vi].RevertXDir = (SInt8) PubPt_AChkRevertXDir[Vi]->Checked;
    
    PubFGuiEnablePlane (Vi, ProAPlanePar[Vi].Process );
    
    ProAPlanePar[Vi].PlotCum    = (SInt8) PubPt_ARbPlotCum[Vi]->Checked;
    ProAPlanePar[Vi].PlotCoin   = (SInt8) PubPt_AChkPlotCoin[Vi]->Checked;
    ProAPlanePar[Vi].PlotFrame  = (SInt8) PubPt_ARdPlotFr[Vi]->Checked;
    
    if ( ProAPlanePar[Vi].PlotCoin == 1 ) {
      ProInf.LastPlaneSelForCoin = Vi;
    }
    
  }
    
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 05/08/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFOnGuiParDispModeChangeFrNbWithTrigOrFrNbWithHit () {

  err_retfail ( PrivGuiConnected, (ERR_OUT,"GUI not connected !") );
  
  if ( PrivSetGuiParDontRead ) {
    return (0);
  }
  
  // Get display mode parameter
  
  ProPar.DispFrNbWithTrigOrFrNbWithHit = (SInt8) PubPt_ChkDispFrNbWithTrigOrFrNbWithHit->Checked;

  // Update labels on fields Trig / Hit frame nb and %

  PrivFSetGuiLabelsDispFrNbWithTrigOrFrNbWithHit ();

  // Update results of fields Trig / Hit frame nb and %

  PubFSetGuiRes ();

  err_retok (( ERR_OUT, "" ));
}
  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 23/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 MAPS__TCDigTelMon::PubFSetGuiStatus () {
  
  SInt8 Vi;
  
  err_retfail ( PrivGuiConnected, (ERR_OUT,"GUI not connected !") );
    
  FDecInt2Edit( ProRes.AcqFrNb  , PubPt_DispAcqFrNb    );
  FDecInt2Edit( ProRes.ProFrNo  , PubPt_DispProFrNo    );
  FDecInt2Edit( ProRes.ProTimeMs, PubPt_DispProcTimeMs );
    
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Rev       : 20/05/2010
          : - Add PubPt_ADispCumFrTrigNb update
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 MAPS__TCDigTelMon::PubFSetGuiRes () {
  
  SInt8 Vi;
  
  err_retfail ( PrivGuiConnected, (ERR_OUT,"GUI not connected !") );
    
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    
    // Display frame nb and % with trigger
    
    if ( ProPar.DispFrNbWithTrigOrFrNbWithHit == 0 ) {
      FDecInt2Edit( ProAPlaneRes[Vi].FrameNbWithTrig    , PubPt_ADispFrNbWithTrigOrHit[Vi]    );
      FFloat2Edit ( ProAPlaneRes[Vi].FramePCentWithTrig , PubPt_ADispFrPCentWithTrigOrHit[Vi] );
    }

    // Display frame nb and % with hit(s)
    
    else {
      FDecInt2Edit( ProAPlaneRes[Vi].FrameNbWithHit    , PubPt_ADispFrNbWithTrigOrHit[Vi]    );
      FFloat2Edit ( ProAPlaneRes[Vi].FramePCentWithHit , PubPt_ADispFrPCentWithTrigOrHit[Vi] );
    }
    
    // Update mean trig nb / frame ONLY if display pointer <> NULL
    // because this GUI display is NOT available on GUI version < 20/05/2010
    
    if ( PubPt_ADispCumFrTrigNb[Vi] != NULL ) {
      FFloat2Edit ( ProAPlaneRes[Vi].CumFrameTrigNb, PubPt_ADispCumFrTrigNb[Vi] );
    }
    
    FDecInt2Edit( ProAPlaneRes[Vi].CumTotHitNb       , PubPt_ADispCumTotHitNb[Vi]    );
    FFloat2Edit ( ProAPlaneRes[Vi].CumFrHitNb        , PubPt_ADispCumFrHitNb[Vi]     );
  }
  
  err_retok (( ERR_OUT, "" ));
}



// GUI END



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFSetPar ( MAPS__TDigTelPar* Pt ) {
  
  err_retnull ( Pt, (ERR_OUT,"Pt == NULL !") );
  
  ProPar = *Pt;

  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFSetPlanePar ( SInt8 Id, MAPS__TDigTelPlanePar*  Pt ) {

  MAPS__TCDigTelMon_CHK_PLANE_ID (Id);
  
  err_retnull ( Pt, (ERR_OUT,"Pt == NULL !") );
  
  ProAPlanePar[Id] = *Pt;
  
  err_retok (( ERR_OUT, "" ));
}

    
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

MAPS__TDigTelPar* MAPS__TCDigTelMon::PubFGetPar () {
  return (&ProPar);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

MAPS__TDigTelPlanePar*  MAPS__TCDigTelMon::PubFGetPlanePar ( SInt8 Id ) {

  err_retfailnull ( PrivChkPlaneId (Id), (ERR_OUT,"Bad Id=%d MUST be 0..%d", Id, MAPS__TCDigTelMon_MAX_PLANE_NB-1) );
  
  return ( &ProAPlanePar[Id] );
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

MAPS__TDigTelPlaneData* MAPS__TCDigTelMon::PubFGetPlaneData ( SInt8 Id ) {

  err_retfailnull ( PrivChkPlaneId (Id), (ERR_OUT,"Bad Id=%d MUST be 0..%d", Id, MAPS__TCDigTelMon_MAX_PLANE_NB-1) );
  
  return ( &ProAPlaneData[Id] );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 23/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFDisAllPlanesProcessing () {

  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    ProAPlanePar[Vi].Process = 0;
  }

  PubFSetGuiPar ();

  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 21/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFDisCumPlot () {

  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    ProAPlanePar[Vi].PlotCum = 0;
  }
  
  PubFSetGuiPar ();

  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 21/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFDisCoinPlot () {
  
  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    ProAPlanePar[Vi].PlotCoin = 0;
  }
  
  PubFSetGuiPar ();
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 21/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFDisFramePlot () {
  
  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    ProAPlanePar[Vi].PlotFrame = 0;
  }
  
  PubFSetGuiPar ();
  
  err_retok (( ERR_OUT, "" ));
}
    


    
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

MAPS__TDigTelPlaneRes*  MAPS__TCDigTelMon::PubFSetPlaneRes  ( SInt8 Id ) {
  
  err_retfailnull ( PrivChkPlaneId (Id), (ERR_OUT,"Bad Id=%d MUST be 0..%d", Id, MAPS__TCDigTelMon_MAX_PLANE_NB-1) );
  
  return ( &ProAPlaneRes[Id] );
}
    
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 23/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFGetPlotRqStatus () {
  
  return ( ProRequestPlot );
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 23/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFResetPlotRqStatus () {
  ProRequestPlot = 0;
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 23/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt8* MAPS__TCDigTelMon::PubFGetPtPlotRq () {
    
  return ( &ProRequestPlot );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 24/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt8* MAPS__TCDigTelMon::PubFGetPtMonEnabled () {
  return ( &ProPar.MonEnabled );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintPar () {
  
  msg (( MSG_OUT, "=============================" ));
  msg (( MSG_OUT, "MonEnabled              %4d", ProPar.MonEnabled ));
  msg (( MSG_OUT, "-----------------------------" ));
  msg (( MSG_OUT, "MaxBuffEvNb             %4d", ProPar.MaxBuffEvNb ));
  msg (( MSG_OUT, "-----------------------------" ));
  msg (( MSG_OUT, "MapsNb                  %4d", ProPar.MapsNb ));
  msg (( MSG_OUT, "MapsName                %4d", ProPar.MapsName ));  
  msg (( MSG_OUT, "PlaneNb                 %4d", ProPar.PlaneNb ));
  msg (( MSG_OUT, "-----------------------------" ));
  msg (( MSG_OUT, "AcqFrNb                 %4d", ProPar.AcqFrNb ));
  msg (( MSG_OUT, "ProcOneAcq              %4d", ProPar.ProcOneAcq ));
  msg (( MSG_OUT, "ProcEvNb                %4d", ProPar.ProcEvNb ));
  msg (( MSG_OUT, "ProcOnlyFrWithTrig      %4d", ProPar.ProcOnlyFrWithTrig ));
  msg (( MSG_OUT, "ListHitAllPlanesMode    %4d", ProPar.ListHitAllPlanesMode ));
  msg (( MSG_OUT, "HandleTrigPos           %4d", ProPar.HandleTrigPos ));
  msg (( MSG_OUT, "StopAtEndOfProc         %4d", ProPar.StopAtEndOfProc ));
  msg (( MSG_OUT, "StoreEvents             %4d", ProPar.StoreEvents ));
  msg (( MSG_OUT, "DispFrNbWith Trig / Hit %4d", ProPar.DispFrNbWithTrigOrFrNbWithHit ));
  msg (( MSG_OUT, "DispFullMatrix          %4d", ProPar.DispFullMatrix ));
  msg (( MSG_OUT, "ProcMode                %4d", ProPar.ProcMode ));
  msg (( MSG_OUT, "CumPlotGrayOrBlueLevels %4d", ProPar.CumPlotGrayOrBlueLevels ));
  msg (( MSG_OUT, "CumPlotHitOrHitCnt      %4d", ProPar.CumPlotHitOrHitCnt ));
  msg (( MSG_OUT, "PlotRawFrNo             %4d", ProPar.GotoRawFrNo ));
  msg (( MSG_OUT, "=============================" ));
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintInf () {

  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "LastPlaneSelForCoin %4d", ProInf.LastPlaneSelForCoin ));
  msg (( MSG_OUT, "CurEvNo             %4d", ProInf.CurEvNo ));
  msg (( MSG_OUT, "FirstEvToProc       %4d", ProInf.FirstEvToProc ));
  msg (( MSG_OUT, "LastEvToProc        %4d", ProInf.LastEvToProc ));
  msg (( MSG_OUT, "EvNbToProc          %4d", ProInf.EvNbToProc ));
  msg (( MSG_OUT, "=========================" ));
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintPlanePar  ( SInt8 Id ) {  
  
  MAPS__TCDigTelMon_CHK_PLANE_ID (Id);
  
  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "Plane %d parameters", Id ));
  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "MapsId     %4d", ProAPlanePar[Id].MapsId     ));
  msg (( MSG_OUT, "Process    %4d", ProAPlanePar[Id].Process    ));
  msg (( MSG_OUT, "Revert X   %4d", ProAPlanePar[Id].RevertXDir ));
  msg (( MSG_OUT, "PlotCum    %4d", ProAPlanePar[Id].PlotCum    ));
  msg (( MSG_OUT, "PlotCoin   %4d", ProAPlanePar[Id].PlotCoin   ));
  msg (( MSG_OUT, "PlotFrame  %4d", ProAPlanePar[Id].PlotFrame  ));
  msg (( MSG_OUT, "=========================" ));
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintAllPlanesPar () {
  
  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    PubFPrintPlanePar (Vi);
  }

  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 22/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintPlaneInf  ( SInt8 Id ) {
  
  MAPS__TCDigTelMon_CHK_PLANE_ID (Id);
  
  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "Plane %d inf", Id ));
  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "PrevProcState %4d", ProAPlaneInf[Id].PrevProcState ));
  msg (( MSG_OUT, "CurProcStat   %4d", ProAPlaneInf[Id].CurProcState   ));
  msg (( MSG_OUT, "=========================" ));
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 22/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintAllPlanesInf () {
  
  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    PubFPrintPlaneInf (Vi);
  }
  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintPlaneData ( SInt8 Id ) {
  
  MAPS__TCDigTelMon_CHK_PLANE_ID (Id);

  
  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "Plane %d data", Id ));
  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "Nothing to print" ));
  msg (( MSG_OUT, "=========================" ));
  
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintAllPlanesData () {
  
  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    PubFPrintPlaneData (Vi);
  }
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Rev       : 20/05/2010
          : - Add CumTotTrigNb & CumFrameTrigNb
          :
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintPlaneRes  ( SInt8 Id ) {
  
  MAPS__TCDigTelMon_CHK_PLANE_ID (Id);

  msg (( MSG_OUT, "=========================" ));
  msg (( MSG_OUT, "Plane %d results", Id ));
  msg (( MSG_OUT, "=============================" ));
  msg (( MSG_OUT, "PtMatCumTotHitCnt      %4x", ProAPlaneRes[Id].PtMatCumTotHitCnt ));
  msg (( MSG_OUT, "PtMatDiscriBit         %4x", ProAPlaneRes[Id].PtMatDiscriBit ));
  msg (( MSG_OUT, "-----------------------------" ));
  msg (( MSG_OUT, "FrameNbWithTrig        %4d", ProAPlaneRes[Id].FrameNbWithTrig ));
  msg (( MSG_OUT, "FramePCentWithTrig     %.2f",ProAPlaneRes[Id].FramePCentWithTrig ));
  msg (( MSG_OUT, "CumTotTrigNb           %.2f",ProAPlaneRes[Id].CumTotTrigNb ));
  msg (( MSG_OUT, "CumFrameTrigNb         %.2f",ProAPlaneRes[Id].CumFrameTrigNb ));
  msg (( MSG_OUT, "-----------------------------" ));
  msg (( MSG_OUT, "FrameNbWithHit         %4d", ProAPlaneRes[Id].FrameNbWithHit ));
  msg (( MSG_OUT, "FramePCentWithHit      %.2f", ProAPlaneRes[Id].FramePCentWithHit ));
  msg (( MSG_OUT, "CumTotHitNb            %4d", ProAPlaneRes[Id].CumTotHitNb ));
  msg (( MSG_OUT, "CumFrHitNb             %.2f", ProAPlaneRes[Id].CumFrHitNb ));
  msg (( MSG_OUT, "=============================" ));
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 17/07/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MAPS__TCDigTelMon::PubFPrintAllPlanesRes  () {
  
  SInt8 Vi;
  
  for ( Vi=0; Vi < ProPar.PlaneNb; Vi++ ) {
    PubFPrintPlaneRes (Vi);
  }
  
  err_retok (( ERR_OUT, "" ));
}


#endif



#endif
