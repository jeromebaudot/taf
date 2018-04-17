
/*******************************************************************************
File      : x:\lib\com\asic\asic.c
Goal      : Functions of ASIC common constants / strcutures librairy
Prj date  : 29/06/2009
File date : 29/06/2009
Doc date  : //200
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef ASIC_C
#define ASIC_C


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
E-mail    : claus@lepsi.in2p3.fr
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
Date      : 22/05/2010
Doc date  : 22/05/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 ASIC__FPrintFrameStatus ( ASIC__TFrameStatus* PtFrStatus ) {
  
  err_retnull ( PtFrStatus, (ERR_OUT,"PtFrStatus = NULL") );  

  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "-----------------------------------------------------" ));
  msg (( MSG_OUT, "AsicNo                                  = %d", PtFrStatus->AsicNo                                  ));
  msg (( MSG_OUT, "AcqNo                                   = %d", PtFrStatus->AcqNo                                   ));
  msg (( MSG_OUT, "FrameNoInAcq                            = %d", PtFrStatus->FrameNoInAcq                            ));
  msg (( MSG_OUT, "FrameNoInRun                            = %d", PtFrStatus->FrameNoInRun                            ));
  msg (( MSG_OUT, "HitCnt                                  = %d", PtFrStatus->HitCnt                                  ));
  msg (( MSG_OUT, "ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = %d", PtFrStatus->ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] ));
  msg (( MSG_OUT, "ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = %d", PtFrStatus->ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  ));
  msg (( MSG_OUT, "ATrigRes[ASIC__MI26_TRIG_RES__LINE]     = %d", PtFrStatus->ATrigRes[ASIC__MI26_TRIG_RES__LINE]     ));
  msg (( MSG_OUT, "ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = %d", PtFrStatus->ATrigRes[ASIC__MI26_TRIG_TOT_NB]        ));
  msg (( MSG_OUT, "-----------------------------------------------------" ));
  
  err_retok (( ERR_OUT, "" ));
}


#endif