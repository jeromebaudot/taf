
/**
* ----------------------------------------------------------------------------------
* \file              X:\lib\com\maps\msis1\data\msis1_data.c
* \brief Goal      : Functions of Mimosis 1 lib
* \brief            
* \brief            
* \version         : 1.0
* \date Prj date   : 03/05/2019
* \date File date  : 03/05/2019
* \date Doc date   : 03/05/2019
* \author          : Gilles CLAUS
* \author          : gilles.claus@iphc.cnrs.fr
* \author          : CNRS - IN2P3 - IHC 23 Rue du Loess 67037 STRASBOURG
*
* Remark : None
*
* ----------------------------------------------------------------------------------
* License   : GNU General Public License
*
* ----------------------------------------------------------------------------------
*/



#ifndef MIMOSIS1_DATA_C
#define MIMOSIS1_DATA_C


#ifndef CC_MSIS1_BDF_LIGHT



// ===================================================================================
// * Functions (comment not processed by DOXYGEN)
// *
// ===================================================================================




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FVersionGet ( char* VerStr, UInt32* PtVerMajor, UInt32* PtVerMinor, UInt32* PtVerDate  )
*
* \brief   : Template / demonstration function for documentation generation
*            extra info on function DISPLAYED IN THE FIRST LINE
*
* \brief   : Extra info on the function if long explanation is needed, it
*            will NOT be displayed in the FIRST LINE, it will be a new
*            bloc of info displayed before the parameters list
*
* \param   : InNumber  - Input number \n
*          :             Range of values allowed is 0 to 45000
* \param   : InStr     - Input string (char* type)
*          :
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    : MUST not use err and msg macro because this function can be called before libs init 
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */
#undef  CC_APP_GUI_CPP       // Disables C++ Builder GUI application => Memo for error and general messagess logging

#define CC_MSIS1_BDF_LIGHT   // Reduces functions, class integrated from library X:/lib/com/maps/msis1/Data
                             // in oder to minimize the risk of compilation warnings / errors

#define CC_NOT_CPP_BUILDER   // Removes items (cst, functions, etc ... ) only available under C++ Builder



#undef CC_INTERFACE_FILE     // To get variables created in this module


// #define APP_ERR_MSG_MACROS_NOT_SUPPORTED  // To be defined in case of compilation errors on errors and general messages libs
                                             // ..\..\com\errors_light.c, errors_light.h
                                             // ..\..\com\msg_light.c, msg_light.h
                                                 

#undef CC_APP_ERR_LOG_VIA_PRINTF             // If enabled => Error log message are automatically printed in console via printf () function
                                             // Rq : The sames messages are written in MIS1__TBtRunRead_DEF_ERR_LOG_FILE

#define CC_APP_MSG_LOG_VIA_PRINTF            // If enabled => General message log are automatically  printed in console via printf () function
                                             // Rq : The sames messages are written in MIS1__TBtRunRead_DEF_MSG_LOG_FILE

//


#include <stdio.h>
#include <string.h>

#include "C4pi_test_team.h"
#include "globals_root.def"
#include "types.def"

#include "errors_light.h"
#include "msg_light.h"

#include "msis1_data.def"
#include "msis1_data.typ"
#include "msis1_data_exp.h"

#include "errors_light.c"
#include "msg_light.c"

SInt32 MIS1__FVersionGet ( char* VerStr, UInt32* PtVerMajor, UInt32* PtVerMinor, UInt32* PtVerDate  ) {
  

  sprintf ( MIS1__VGVerStr, "Lib Mimosis data : Ver %d.%d date %x", MIS1__VGVerMajor, MIS1__VGVerMinor, MIS1__VGVerDate );
  

  if ( VerStr != NULL ) {
    strcpy ( VerStr, MIS1__VGVerStr );
  }


  if ( PtVerMajor != NULL ) {
    *PtVerMajor = MIS1__VGVerMajor;
  }
  
  
  if ( PtVerMinor != NULL ) {
    *PtVerMinor = MIS1__VGVerMinor;
  }
  

  if ( PtVerDate != NULL ) {
    *PtVerDate = MIS1__VGVerDate;
  }
  
  return (0);
  
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FTemplateForDoc ( SInt32 InNumber, char* InStr )
*          
* \brief   : Template / demonstration function for documentation generation
*            extra info on function DISPLAYED IN THE FIRST LINE
*          
* \brief   : Extra info on the function if long explanation is needed, it
*            will NOT be displayed in the FIRST LINE, it will be a new
*            bloc of info displayed before the parameters list
*          
* \param   : InNumber  - Input number \n
*          :             Range of values allowed is 0 to 45000 
* \param   : InStr     - Input string (char* type)
*          :
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*          
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/04/2019
* \date    : Doc date  : 11/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FTemplateForDoc ( SInt32 InNumber, char* InStr ) {
  
  const SInt32 IN_NUMBER_MIN = 0;
  const SInt32 IN_NUMBER_MAX = 45000;
  
  
  if ( MIS1__VGBeginDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FBegin (...) has not been called !") );
  }
  
  if ( (InNumber < IN_NUMBER_MIN) || (InNumber > IN_NUMBER_MAX) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => InNumber = %d is ouf range %d..%d", InNumber, IN_NUMBER_MIN, IN_NUMBER_MAX ) );
  }
  
  
  return (0);
  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FBegin ( SInt8 ErrLogLvl, char* ErrFile, SInt8 MsgLogLvl, char* MsgFile )
*          :
* \brief   : Init error and info messages handling, variables initialization, etc ...
*          :
* \param   : ErrLogLvl - Eror logging level, can be ERR_LOG_LVL_NONE, ERR_LOG_LVL_ALL,
*          :             ERR_LOG_LVL_WARNINGS_ERRORS, ERR_LOG_LVL_ERRORS
* \param   : ErrFile   - Error log text file name + path
* \param   : MsgLogLvl - Info messages logging level, 0 = no print, 127 = prints all messages
* \param   : MsgFile   - Info messages log text file name + path
*          :
* \return  : Error code
*          :   0  - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Now 08/04/2019 the function returns always 0
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/04/2019
* \date    : Doc date  : 11/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FBegin ( SInt8 ErrLogLvl, char* ErrFile, SInt8 MsgLogLvl, char* MsgFile ) {
  
  MIS1__VGBeginDone = 0;
    
  ERR_FBegin             ( ( ErrLogLvl != 0 ) /* Enable */, ErrFile );
  ERR_FSetFileLogLevel   ( ErrLogLvl );
    
  MSG_FBegin ( (MsgLogLvl != 0) /* Enable */, MsgFile );
  MSG_FSetFileLogLevel ( MsgLogLvl );
      
  
  MIS1__VGBeginDone = 1;
  
  err_retok (( ERR_OUT, "" ));
   
}

// 888

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FAcqProcInitPtrFields ( MIS1__TAcqProc* Pt, MIS1__TRunPar* PtRunPar, MIS1__TRunInf* PtRunInf, MIS1__TRunStatus PtRunStatus, FIL__TCBinFile* PtOStepFile, FIL__TCBinFile* PtOSubStepsFile )
:
* \brief   : Init external records pointers fields of MIS1__TAcqProc variable \n
*
* \param   : Pt - Pointer on the record
:
* \return  : Error code  \n
*          :   0  - OK   \n
*          : < 0 - Error \n
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/06/2020
* \date    : Doc date  : 26/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FAcqProcInitPtrFields ( MIS1__TAcqProc* Pt, MIS1__TRunPar* PtRunPar, MIS1__TRunInf* PtRunInf, MIS1__TRunStatus* PtRunStatus, FIL__TCBinFile* PtOStepFile, FIL__TCBinFile* PtOSubStepsFile )  {
  

  
  // Check param
  
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  err_retnull ( PtRunPar,        (ERR_OUT,"Abort => PtRunPar == NULL") );
  err_retnull ( PtRunInf,        (ERR_OUT,"Abort => PtRunInf == NULL") );
  err_retnull ( PtRunStatus,     (ERR_OUT,"Abort => PtRunStatus == NULL") );
  err_retnull ( PtOStepFile,     (ERR_OUT,"Abort => PtOStepFile == NULL") );
  err_retnull ( PtOSubStepsFile, (ERR_OUT,"Abort => PtOSubStepsFile == NULL") );
  
  // Set fields
  
  Pt->PtRunPar         = PtRunPar;
  Pt->PtRunInf         = PtRunInf;
  Pt->PtRunStatus      = PtRunStatus;
  Pt->PtOStepFile      = PtOStepFile;
  Pt->PtOSubStepsFile  = PtOSubStepsFile;
  

  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FAcqProcFieldsFree ( MIS1__TAcqProc* Pt )
:
* \brief   : Free all the arrays field of a MIS1__TAcqProc variable \n
*
* \param   : Pt - Pointer on the record
:
* \return  : Error code  \n
*          :   0  - OK   \n
*          : < 0 - Error \n
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 08/06/2020
* \date    : Doc date  : 08/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FAcqProcFieldsFree ( MIS1__TAcqProc* Pt ) {
  
  SInt16 Vi;
  
  // Check param
  
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  // Free mem
  
  if ( Pt->AAcqFrListRaw != NULL    ) {
    free ( Pt->AAcqFrListRaw );
    Pt->AAcqFrListRaw = NULL;
  }
  
  if ( Pt->AAcqFrMapRaw != NULL     ) {
    free ( Pt->AAcqFrMapRaw );
    Pt->AAcqFrMapRaw = NULL;
  }
  
  if ( Pt->AFrListDec != NULL    ) {
    free ( Pt->AFrListDec );
    Pt->AFrListDec = NULL;
  }
  
  if ( Pt->AFrMapDec != NULL     ) {
    free ( Pt->AFrMapDec );
    Pt->AFrMapDec = NULL;
  }

  if ( Pt->PtSubStepPixCnt != NULL       ) {
    free ( Pt->PtSubStepPixCnt );
    Pt->PtSubStepPixCnt = NULL;
  }
  
  if ( Pt->PtStepPixCnt != NULL ) {
    free ( Pt->PtStepPixCnt );
    Pt->PtStepPixCnt = NULL;
  }
  
  if ( Pt->ADecFrPix != NULL    ) {
    free ( Pt->ADecFrPix );
    Pt->ADecFrPix = NULL;
  }
  
  
  // Remark
  
  // void* AAcqFrDecBuff[MIS1__MAX_BUFFERED_ACQ_NB] is NOT freed HERE
  
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FAcqProcFieldsAlloc ( MIS1__TAcqProc* Pt )
*
* \brief   : Allocate all the arrays field of a MIS1__TAcqProc variable \n
*            because size is too big to get them in the record (C++ B crash)
*
* \param   : Pt       - Pointer on the record
*           
* \param   : ProcMode - Processing mode, defines fields to alloc
*   
* \return  : Error code  \n
*          :   0  - OK   \n
*          : < 0 - Error \n
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 08/06/2020
* \date    : Doc date  : 08/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FAcqProcFieldsAlloc ( MIS1__TAcqProc* Pt, MIS1__TProcMode ProcMode, SInt32* PtBuffNb, UInt32* PtBuffSzW8, UInt32* PtBuffTotSzW8, float* PtBuffTotSzMB ) {
  
  SInt16 Vi;
  UInt32 VBuffTotSzW8;
  UInt32 VAllBuffTotSzW8;
  float  VAllBuffTotSzMB;
  
  // ================================================================
  // Check param
  // ================================================================
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  // Free all fled (if not already done) and set ptr to NULL
  
  MIS1__FAcqProcFieldsFree ( Pt );
  
  // ================================================================
  // Alloc mem
  // ================================================================
  
  VBuffTotSzW8 = 0;
  
  // ----------------------------------------------------------------
  // Fr list and fr map raw always allocated
  // ----------------------------------------------------------------
  
  Pt->AAcqFrListRaw  = (MIS1__TAcqFrList*) malloc ( MIS1__MAX_BUFFERED_ACQ_NB * sizeof (MIS1__TAcqFrList) );

  err_retnull ( Pt->AAcqFrListRaw, (ERR_OUT,"Abort => Allocate AAcqFrListRaw failed !") );
  
  VBuffTotSzW8 += sizeof (MIS1__TAcqFrList);
  
  Pt->AAcqFrMapRaw   = (MIS1__TAcqFrMap*) malloc ( MIS1__MAX_BUFFERED_ACQ_NB * sizeof (MIS1__TAcqFrMap) );

  err_retnull ( Pt->AAcqFrMapRaw, (ERR_OUT,"Abort => Allocate AAcqFrMapRaw failed !") );

  VBuffTotSzW8 += sizeof (MIS1__TAcqFrMap);

  // ----------------------------------------------------------------
  // Decoded pixels buffers
  // ----------------------------------------------------------------
    
  if ( ProcMode >= PM_DECODE_FR ) {

    Pt->AFrListDec  = (MIS1__TDecFrList*) malloc ( MIS1__MAX_BUFFERED_ACQ_NB * MIS1__MAX_FR_NB_PER_ACQ * sizeof (MIS1__TDecFrList) );
  
    err_retnull ( Pt->AFrListDec, (ERR_OUT,"Abort => Allocate AFrListDec failed !") );
  
    VBuffTotSzW8 += MIS1__MAX_FR_NB_PER_ACQ * sizeof (MIS1__TDecFrList);
    
    Pt->AFrMapDec   = (MIS1__TDecFrMap*) malloc ( MIS1__MAX_BUFFERED_ACQ_NB * MIS1__MAX_FR_NB_PER_ACQ * sizeof (MIS1__TDecFrMap) );
  
    err_retnull ( Pt->AFrMapDec, (ERR_OUT,"Abort => Allocate AAcqFrMapDec failed !") );
    
    VBuffTotSzW8 += MIS1__MAX_FR_NB_PER_ACQ * sizeof (MIS1__TDecFrMap);

    Pt->ADecFrPix = (MIS1__TDecFrPix*) malloc ( MIS1__MAX_BUFFERED_ACQ_NB * MIS1__MAX_FR_NB_PER_ACQ * sizeof (MIS1__TDecFrPix) );

    err_retnull ( Pt->ADecFrPix, (ERR_OUT,"Abort => Allocate AFrPix failed !") );
    
    VBuffTotSzW8 += MIS1__MAX_FR_NB_PER_ACQ * sizeof (MIS1__TDecFrPix) ;
  
  } // End if ( ProcMode >= PM_DECODE_FR )
  

  // ----------------------------------------------------------------
  // Fired pixels counting buffers
  // ----------------------------------------------------------------
  
  if ( ProcMode >= PM_DECODE_FR_CNT_PIX ) {

    Pt->PtSubStepPixCnt  = (MIS1__TMatPixCntU16*) malloc ( sizeof (MIS1__TMatPixCntU16) );
  
    err_retnull ( Pt->PtSubStepPixCnt, (ERR_OUT,"Abort => Allocate PtSubStepPixCnt failed !") );
    
    VBuffTotSzW8 += sizeof (MIS1__TMatPixCntU16);
    
    Pt->PtStepPixCnt    = (MIS1__TMatPixCntU16*) malloc ( sizeof (MIS1__TMatPixCntU16) );
  
    err_retnull ( Pt->PtStepPixCnt, (ERR_OUT,"Abort => Allocate PtStepPixCnt failed !") );

    VBuffTotSzW8 += sizeof (MIS1__TMatPixCntU16);
  
  } // End if ( ProcMode >= PM_DECODE_FR_CNT_PIX )


  // Remark
  
  // void* AAcqFrDecBuff[MIS1__MAX_BUFFERED_ACQ_NB] is NOT allocated HERE
  
  // Caclulate buffers sz
  
  VAllBuffTotSzW8 = (VBuffTotSzW8 * MIS1__MAX_BUFFERED_ACQ_NB) + sizeof (MIS1__TMatPixCntU16);
  
  VAllBuffTotSzMB = VAllBuffTotSzW8/ 1024 / 1024; 
  
  
  // Update buffers size info
   
  if ( PtBuffNb != NULL ) {
    *PtBuffNb = MIS1__MAX_BUFFERED_ACQ_NB;
  }
  
  if ( PtBuffSzW8 != NULL ) {
    *PtBuffSzW8 = VBuffTotSzW8;
  }
  

  if ( PtBuffTotSzW8 != NULL ) {
    *PtBuffTotSzW8 = VAllBuffTotSzW8;
  }
  

  if ( PtBuffTotSzMB != NULL ) {
    *PtBuffTotSzMB = VAllBuffTotSzMB;
  }
  
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FEnd ()
:
* \brief   : Terminates library operation => To be called at end of program
:
* \param   : None
:
* \return  : Error code ( On 11/04/2019 it returns always 0 )
*          :   0  - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/04/2019
* \date    : Doc date  : 11/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FEnd () {
    
  MIS1__VGBeginDone = 0;
    
    
  err_retok (( ERR_OUT, "" ));
   
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPrintMsg ( SInt32 DummyS32In, SInt8 PrintPar, SInt32 ParS32, float ParFloat, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg )
*          :
* \brief   : Print messages in msg or error log file from LabView.
*          : If PrintPar = 1 => Print ParS32 and ParFloat in log file specified by PrintAs ...
*          : If PrintPar = 0 => Print Msg in log file specified by PrintAs ...
*          :
* \param   : DummyS32In      - Dummy value used under Labview to easily " chain " function execution.
*          :
*          :                   To execute it after a Vi call, connect any output of this Vi to the
*          :                   DummyS32In pin and it will be automatically called after Vi end.
*          :                   To execute it before a Vi call, if this Vi has an integer parameter
*          :                   cut the wire and insert DummyS32In input and function output on it.
*          :
* \param   : PrintPar        - 1 = print parameters / 0 = print message
*          :
* \param   : ParS32          - S32 parameter to print
* \param   : ParFloat        - Float parameter to print
*          :
* \param   :                 Printing mode flags
*          :
* \param   : PrintAsMsg      - If 1 -> Print in messages log file
* \param   : PrintAsTrace    - If 1 -> Print in errors log file as a trace message
* \param   : PrintAsWarning  - If 1 -> Print in errors log file as a warning message
* \param   : PrintAsError    - If 1 -> Print in errors log file as an error message
*          :
* \param   : Msg             - Message to print ( string )
*          :
* \return  : The function always returns the input parameter DummyS32In.
*          :
* \warning : Globals   :
* \warning : Remark    : Return the value of the input parameter name DummyS32In ( SInt32 )
*           It can be used to insert this function call on an integer datapath under LabView
*          :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/04/2019
* \date    : Doc date  : 11/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPrintMsg ( SInt32 DummyS32In, SInt8 PrintPar, SInt32 ParS32, float ParFloat, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg ) {
  
  if ( PrintPar == 1 ) {
    if ( PrintAsMsg     ) msg         (( MSG_OUT, "ParS32=%d - ParFloat=%.3f", Msg ));
    if ( PrintAsTrace   ) err_trace   (( ERR_OUT, "ParS32=%d - ParFloat=%.3f", Msg ));
    if ( PrintAsWarning ) err_warning (( ERR_OUT, "ParS32=%d - ParFloat=%.3f", Msg ));
    if ( PrintAsError   ) err_error   (( ERR_OUT, "ParS32=%d - ParFloat=%.3f", Msg ));
  }
  
  else {
    if ( PrintAsMsg     ) msg         (( MSG_OUT, "%s", Msg ));
    if ( PrintAsTrace   ) err_trace   (( ERR_OUT, "%s", Msg ));
    if ( PrintAsWarning ) err_warning (( ERR_OUT, "%s", Msg ));
    if ( PrintAsError   ) err_error   (( ERR_OUT, "%s", Msg ));
  }
  
  
  return (DummyS32In);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPrintStr ( char* Str, SInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*          :
* \brief   : Prints str to log file, memo, both in function of PrintDest param value
*          :
* \param   : Str       - String to print
*          :
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : Memo      - Pointer to destination Memo if PrintDest = 2 or 3, NULL if not used
*          :
* \param   : PtFile    - Pointer to text file
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 23/05/2019
* \date    : Doc date  : 23/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPrintStr ( char* Str, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {
  
  // Check param
  
  // PrintDest, Memo, PtFile MUST already have been tested => No test to reduce execution time
  
  // Str tested because string NULL pointer may always happen ...
  
  err_retnull ( Str, (ERR_OUT,"Abort => Str = NULL") );
  
  // Prints
  
  switch ( PrintDest ) {
    
    // Nothing to print
    
    case 0 : {
      break; }
    
    // Msg file
    
    case 1 : {
      msg (( MSG_OUT, "%s", Str ));
      break; }
    
    // Memo
    
    case 2 : {
      Memo->Lines->Add ( Str );
      break; }
    
    // Msg file + Memo
    
    case 3 : {
      msg (( MSG_OUT, "%s", Str ));
      Memo->Lines->Add ( Str );
      break; }
    
    // Text file
    
    case 4 : {
      fprintf ( PtFile, "%s \n", Str );
      break; }

    // Text file + Memo
    
    case 5 : {
      Memo->Lines->Add ( Str );
      fprintf ( PtFile, "%s \n", Str );
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Abort => Unknown PrintDest = %d", PrintDest) );
      break; }
    
  } // End switch ( PrintMth )
  
  
  return (0);
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FGenPRBS16Fib ( UInt16* PtDest, SInt32 DestSzW16, UInt16 StartVal, SInt32 W16Nb )
*          :
* \brief   : Generates a 16 bits Fibonacci LFSR / PRBS, stores values in PtDest[0 to W16Nb-1]
*          :
* \param   : PtDest    - Pointer to destination array
*          :
* \param   : DestSzW16 - Destination array max size in W16
*          :
* \param   : StartVal  - Firdt value of PRBS, MUST NOT be 0
*          :
* \param   : W16Nb     - Number of W16 to generate 
*          :
* \return  : = 0 if OK
*          : < 0 in case of error
*          :
* \warning : Globals   :
* \warning : Remark    : Code from https://en.wikipedia.org/wiki/Linear-feedback_shift_register
*          :           :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 15/11/2019
* \date    : Doc date  : 12/11/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FGenPRBS16Fib ( UInt16* PtDest, SInt32 DestSzW16, UInt16 StartVal, SInt32 W16Nb )

{
  UInt16 start_state = 0xACE1u;  /* Any nonzero start state will work. */
  UInt16 lfsr = start_state;
  UInt16 bit;                    /* Must be 16-bit to allow bit<<15 later in the code */
  unsigned period = 0;
  char   VStr[GLB_CMT_SZ];
  
  
  // Check param
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL !") );
  
  err_retnull ( StartVal, (ERR_OUT,"Abort => StartVal == 0, it is NOT allowed !") );
  
  
  // Generats PRBS
  
  do
  {   /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
    bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) /* & 1u */;
    lfsr = (lfsr >> 1) | (bit << 15);
    
    PtDest[period] = lfsr;
    
    // sprintf ( VStr, "PRBS[%.4d] = %x", period, lfsr );
        
    ++period;
    
    if ( period >= W16Nb ) {
      break;
    }
    
  }
  while (lfsr != start_state);



  return period;
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FDxErrCntReset ( MIS1__TDxErrCnt* PtDest )
*          :
* \brief   : Resets the Dx errors counters
*          :
* \param   : PtDest    - Pointer to counters record
*          :
* \return  : = 0 if OK
*          : < 0 in case of error
*          :
* \warning : Globals   :
* \warning : Remark    : 
*          :           : 
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 14/11/2019
* \date    : Doc date  : 14/11/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FDxErrCntReset ( MIS1__TDxErrCnt* PtDest ) {
  
  SInt8 ViDx;
  
  // Check destination header pointer
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  // Reset errors counters 
  
  for ( ViDx = 0; ViDx < MIS1__MAX_OUT_NB; ViDx++ ) {
    PtDest->AErrCnt[ViDx] = 0;
  }
   
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FDxErrCntCumul ( MIS1__TDxErrCnt* PtDestCumul, MIS1__TDxErrCnt* PtSrc)
*          :
* \brief   : Cumul Dx errors counters, add content of PtSrc to PtDestCumul
*          :
* \param   : PtDestCumul - Pointer to cumul destination
*          :
* \param   : PtSrc       - Pointer to dx counter to add to cumul
*          :
* \return  : = 0 if OK
*          : < 0 in case of error
*          :
* \warning : Globals   :
* \warning : Remark    :
*          :           :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 14/11/2019
* \date    : Doc date  : 14/11/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FDxErrCntCumul ( MIS1__TDxErrCnt* PtDestCumul, MIS1__TDxErrCnt* PtSrc) {
  
  SInt8 ViDx;
  
  // Check destination header pointer
  
  err_retnull ( PtDestCumul, (ERR_OUT,"Abort => PtDestCumul == NULL") );

  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  // Cumul errors counters
  
  for ( ViDx = 0; ViDx < MIS1__MAX_OUT_NB; ViDx++ ) {
    PtDestCumul->AErrCnt[ViDx] += PtSrc->AErrCnt[ViDx];
  }
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFrHeaderSetDefTags ( MIS1__TDsFrHeader* PtDest )
*          :
* \brief   : Sets the default values (MIS1__DS_TAG_HEADER) of frame header tags
*          :
* \param   : PtDest    - A pointer to the header
*          :
* \return  : Error code 
*          :   0  - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/05/2019
* \date    : Doc date  : 13/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FFrHeaderSetDefTags ( MIS1__TDsFrHeader* PtDest ) {
  
  SInt32 VStrLen;
  UInt32 VFrCnt;
  
  // Check destination header pointer
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  // Sets frame counter fields
  
  PtDest->F.W7  = MIS1__DS_TAG_HEADER;
  PtDest->F.W6  = MIS1__DS_TAG_HEADER;
  PtDest->F.W5  = MIS1__DS_TAG_HEADER;
  PtDest->F.W4  = MIS1__DS_TAG_HEADER;
  PtDest->F.W3H = MIS1__DS_TAG_HEADER >> 8;
  PtDest->F.W2H = MIS1__DS_TAG_HEADER >> 8;
  PtDest->F.W1H = MIS1__DS_TAG_HEADER >> 8;
  PtDest->F.W0H = MIS1__DS_TAG_HEADER >> 8;
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFrHeaderSetFrCnt ( UInt32 FrCnt, MIS1__TDsFrHeader* PtDest )
*          :
* \brief   : Sets the frame counter fields of header from a frame count value
*          :
* \param   : FrCnt     - The frame counter value
*          :
* \param   : PtDest    - A pointer to the header
*          :
* \return  : Error code 
*          :   0  - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 07/05/2019
* \date    : Doc date  : 07/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FFrHeaderSetFrCnt ( UInt32 FrCnt, MIS1__TDsFrHeader* PtDest ) {
  
  SInt32 VStrLen;
  UInt32 VFrCnt;
  
  // Check destination header pointer
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  // Sets frame counter fields
  
  PtDest->F.FC0 = (FrCnt & 0x000000FF);
  PtDest->F.FC1 = (FrCnt & 0x0000FF00) >> 8;
  PtDest->F.FC2 = (FrCnt & 0x00FF0000) >> 16;
  PtDest->F.FC3 = (FrCnt & 0xFF000000) >> 24;
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFrHeaderGetFrCnt ( MIS1__TDsFrHeader* Pt )
*          :
* \brief   : Gets the frame counter fields of header
*          :
* \param   : FrCnt     - The frame counter value
*          :
* \param   : Pt        - A pointer to the header
*          :
* \return  : The frame counter or error
*          : Frame counter >= 0 is OK
*          : < 0 in case of error
*          :
* \warning : Globals   :
* \warning : Remark    : Returned value is SInt32 for error handling \n
*          :           : => Cast on UInt32 to get full fame counter dynamic
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/05/2019
* \date    : Doc date  : 13/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FFrHeaderGetFrCnt ( MIS1__TDsFrHeader* Pt ) {
  
  UInt32 VFrCnt;

  // Check destination header pointer
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  // Builts frame counter
    
  VFrCnt = Pt->F.FC0 + (Pt->F.FC1 << 8) + (Pt->F.FC2 << 16) + (Pt->F.FC3 << 24);
   
  return ( VFrCnt );
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : char* MIS1__FFrHeader2Str ( MIS1__TDsFrHeader* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode )
*          :
* \brief   : Converts the data stream frame header field in a string
*          :
* \param   : PtSrc     - A pointer to the header
*          :            
* \param   : DestStr   - Destination string
*          :
* \param   : DestStrSz - Size of destination string
*          :
* \param   : Mode      - Print mode, reserved for future use
*          :
* \return  : A string which contains the frame header
*          : - DestStr if provided as non NULL pointer and if size > required
*          : - A static local string in case DestStr can't be used
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 07/05/2019
* \date    : Doc date  : 07/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

char* MIS1__FFrHeader2Str ( MIS1__TDsFrHeader* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode ) {
  
  static   char   VRetStr[GLB_CMT_SZ];
           SInt32 VStrLen;
           UInt32 VFrCnt; 

  // Check header pointer

  if ( PtSrc == NULL ) {
    sprintf ( VRetStr, "Abort => PtSrc == NULL" );
    err_error (( ERR_OUT, "%s", VRetStr ));
    return (VRetStr);
  }

  // Builts frame counter
  
  VFrCnt = PtSrc->F.FC0 + (PtSrc->F.FC1 << 8) + (PtSrc->F.FC2 << 16) + (PtSrc->F.FC3 << 24);

  // Builts result string
  
  sprintf ( VRetStr, "#H : [W7]=%x  [W6]=%x  [W5]=%x  [W4]=%x  [W3H]=%x  [W2H]=%x  [W1H]=%x  [W0H]=%x(H)  -  FrCnt = %d (D))", PtSrc->F.W7, PtSrc->F.W6, PtSrc->F.W5, PtSrc->F.W4, PtSrc->F.W3H, PtSrc->F.W2H, PtSrc->F.W1H, PtSrc->F.W0H, VFrCnt );
  
  VStrLen = strlen ( VRetStr );
  
  // Copy result to DestStr if possible + errors messages
  
  if ( DestStr == NULL ) {
    err_error (( ERR_OUT, "DestStr == NULL => Returns local variable, DestStr not updated" ));
  }
  
  if ( DestStrSz <= VStrLen ) {
    err_error (( ERR_OUT, "DestStrSz=%d <= VStrLen=%d => Returns local variable, DestStrSz is too small", DestStrSz, VStrLen ));
  }
  
  if ( (DestStr != NULL) && (DestStrSz > VStrLen) ) {
    strcpy ( DestStr, VRetStr );
    return (DestStr);
  }
  
  else {
    return (VRetStr);
  }
  
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFrHeaderCheck8Out ( MIS1__TDsFrHeader* Pt, UInt32 AcqNoForPrint, UInt32 FrNoForPrint, UInt16 W7, UInt16 W6, UInt16 W5, UInt16 W4, UInt8 W3H, UInt8 W2H, UInt8 W1H, UInt8 W0H, UInt32 FrCnt, MIS1__TDxErrCnt* PtResDxErrCnt, SInt8 PrintDest, TMemo* Memo, FILE* PtFile  )
*          :
* \brief   : Checks the header fields / expected values, for MSIS1 in 8 outputs mode
*          :
* \param   : Pt           - A pointer to the header
*          :
* \param   : AcqNoForPrint- No of the acquisition for error message text
*          :
* \param   : FrNoForPrint - No of the frame for error message text
*          :
* \param   : W7           - Expected value for W7 field (16 bits)
*          :
* \param   : W6           - Expected value for W6 field (16 bits)
*          :
* \param   : W5           - Expected value for W5 field (16 bits)
*          :
* \param   : W4           - Expected value for W4 field (16 bits)
*          :
* \param   : W3H          - Expected value for high byte of W3 field (16 bits)
*          :
* \param   : W2H          - Expected value for high byte of W2 field (16 bits)
*          :
* \param   : W1H          - Expected value for high byte of W1 field (16 bits)
*          :
* \param   : W0H          - Expected value for high byte of W0 field (16 bits)
*          :
* \param   : FrCnt        - Expected value for frames counter (32 bits), FrCnt : B0B7 = W0L, B8B15 = W1L, B16B23 = W2L, B24B31 = W3L
*          :
* \param   : PtResDxErrCnt - Stores errors count on each Dx, set it to NULL if not used. Warning, it adds errors to current count, no reset at beginning of function.
*          :
* \param   : PrintDest    - Print destination for errors
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : Memo         - Pointer to destination Memo if PrintDest = 2 or 3, NULL if not used
*          :
* \param   : PtFile       - Pointer to text file if PrintDest = 4 or 5, NULL if not used
*          :
* \return  : < 0 in case of sw error, example Pt == NULL
*          : = 0 if no error on header fields
*          : > 0 = number of errors on header fields
*          :
* \warning : Globals   :
* \warning : Remark    :
*          :           
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/11/2019
* \date    : Doc date  : 13/11/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FFrHeaderCheck8Out ( MIS1__TDsFrHeader* Pt, UInt32 AcqNoForPrint, UInt32 FrNoForPrint, UInt16 W7, UInt16 W6, UInt16 W5, UInt16 W4, UInt8 W3H, UInt8 W2H, UInt8 W1H, UInt8 W0H, UInt32 FrCnt, MIS1__TDxErrCnt* PtResDxErrCnt, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {
  
  SInt32 VErrCnt;
  char   VStrErr[GLB_CMT_SZ];
  UInt32 VFrCnt;
  
  // Check destination header pointer
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  // Builts frame counter
  
  VFrCnt = Pt->F.FC0 + (Pt->F.FC1 << 8) + (Pt->F.FC2 << 16) + (Pt->F.FC3 << 24);
  
  // Resets errors cnt
  
  VErrCnt = 0;
  
  // Check W7 to W4 16 bits fields
  
  if ( Pt->F.W7 != W7 ) {
    
    ++VErrCnt;
    
    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[7];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W7 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W7, W7 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
  
  if ( Pt->F.W6 != W6 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[6];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W6 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W6, W6 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }


  if ( Pt->F.W5 != W5 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[5];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W5 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W5, W5 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }


  if ( Pt->F.W4 != W4 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[4];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W4 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W4, W4 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }

  // Check high byte of W3 to W0 fields
  
  if ( Pt->F.W3H != W3H ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[3];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W3H = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W3H, W3H );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }


  if ( Pt->F.W2H != W2H ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[2];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W2H = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W2H, W2H );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
  
  if ( Pt->F.W1H != W1H ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[1];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W1H = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W1H, W1H );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }


  if ( Pt->F.W0H != W0H ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[0];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W0H = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.W0H, W0H );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  

  // Check frame counter
  

  if ( VFrCnt != FrCnt ) {
    
    ++VErrCnt;
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : FrCnt = %.6d <> expected = %.6d", AcqNoForPrint, FrNoForPrint, VFrCnt, FrCnt );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
    // Search Dx source of errors
    
    if ( PtResDxErrCnt != NULL ) {
      
      if ( (VFrCnt & 0x000000FF) != (FrCnt & 0x000000FF) ) {
        ++PtResDxErrCnt->AErrCnt[0];
      }
      
      if ( (VFrCnt & 0x0000FF00) != (FrCnt & 0x0000FF00) ) {
        ++PtResDxErrCnt->AErrCnt[1];
      }
      
      if ( (VFrCnt & 0x00FF0000) != (FrCnt & 0x00FF0000) ) {
        ++PtResDxErrCnt->AErrCnt[2];
      }
      
      if ( (VFrCnt & 0xFF000000) != (FrCnt & 0xFF000000) ) {
        ++PtResDxErrCnt->AErrCnt[3];
      }
      
    } // End if ( PtResDxErrCnt != NULL )
    
  } // End if ( VFrCnt != FrCnt )


  // If errors have been printed => Add a line "======" at end of text
  
  if ( (PrintDest != 0) && (VErrCnt > 0) ) {
      MIS1__FPrintStr ( "============================================================", PrintDest, Memo, PtFile );
  }
    
   

  return ( VErrCnt );
}



    



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : char* MIS1__FFrTrailer2Str ( MIS1__TDsFrTrailer* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode )
*          :
* \brief   : Converts the data stream frame trailer field in a string
*          :
* \param   : PtSrc     - A pointer to the trailer
*          :
* \param   : DestStr   - Destination string
*          :
* \param   : DestStrSz - Size of destination string
*          :
* \param   : Mode      - Print mode, reserved for future use
*          :
* \return  : A string which contains the frame trailer
*          : - DestStr if provided as non NULL pointer and if size > required
*          : - A static local string in case DestStr can't be used
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/07/2019
* \date    : Doc date  : 02/07/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

char* MIS1__FFrTrailer2Str ( MIS1__TDsFrTrailer* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode ) {
  
  static   char   VRetStr[GLB_CMT_SZ];
  SInt32 VStrLen;
  UInt32 VFrCnt;
  
  // Check header pointer
  
  if ( PtSrc == NULL ) {
    sprintf ( VRetStr, "Abort => PtSrc == NULL" );
    err_error (( ERR_OUT, "%s", VRetStr ));
    return (VRetStr);
  }
  
   
  // Builts result string
  
  sprintf ( VRetStr, "[W1]= Tag = %x  - [W0] = Flags = %x (H) = FrOvf %d - RegOvf %d - SRegOvf %d - FSLimit %d", PtSrc->F.Tag, PtSrc->F.Flags, PtSrc->B.FrOvf, PtSrc->B.RegOvf, PtSrc->B.SRegOvf, PtSrc->B.FSLimit );
  
  VStrLen = strlen ( VRetStr );
  
  // Copy result to DestStr if possible + errors messages
  
  if ( DestStr == NULL ) {
    err_error (( ERR_OUT, "DestStr == NULL => Returns local variable, DestStr not updated" ));
  }
  
  if ( DestStrSz <= VStrLen ) {
    err_error (( ERR_OUT, "DestStrSz=%d <= VStrLen=%d => Returns local variable, DestStrSz is too small", DestStrSz, VStrLen ));
  }
  
  if ( (DestStr != NULL) && (DestStrSz > VStrLen) ) {
    strcpy ( DestStr, VRetStr );
    return (DestStr);
  }
  
  else {
    return (VRetStr);
  }
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : char* MIS1__FFrEwChkSumTrailer2Str ( MIS1__TDsFrEwChkSumTrailer* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode )
*          :
* \brief   : Converts the data stream frame trailer field in a string
*          :
* \param   : PtSrc     - A pointer to the trailer
*          :
* \param   : DestStr   - Destination string
*          :
* \param   : DestStrSz - Size of destination string
*          :
* \param   : Mode      - Print mode, reserved for future use
*          :
* \return  : A string which contains the frame trailer
*          : - DestStr if provided as non NULL pointer and if size > required
*          : - A static local string in case DestStr can't be used
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/07/2019
* \date    : Doc date  : 02/07/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

char* MIS1__FFrEwChkSumTrailer2Str ( MIS1__TDsFrEwChkSumTrailer* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode ) {
  
  static   char   VStrEw[GLB_CMT_SZ];
  static   char   VStrChkSum[GLB_CMT_SZ];
  static   char   VStrTrailer[GLB_CMT_SZ];
  
  static   char   VRetStr[GLB_CMT_SZ];
  SInt32 VStrLen;
  UInt32 VFrCnt;
  
  // Check header pointer
  
  if ( PtSrc == NULL ) {
    sprintf ( VRetStr, "Abort => PtSrc == NULL" );
    err_error (( ERR_OUT, "%s", VRetStr ));
    return (VRetStr);
  }
  
  // Convert empty words
  
  sprintf ( VStrEw, "#T : [W7]=%x  [W6]=%x  [W5]=%x  [W4]=%x  [W3 ]=%x  [W2 ]=%x [H]", PtSrc->AW16[7], PtSrc->AW16[6], PtSrc->AW16[5], PtSrc->AW16[4], PtSrc->AW16[3], PtSrc->AW16[2] );
  
  
  // Convert Check sum to string
  
  sprintf ( VStrChkSum, "Check sum = %x [H]", PtSrc->F.CheckSum.W16 );
  
  // Convert Trailer to string
  
  MIS1__FFrTrailer2Str ( &(PtSrc->F.Trailer), VStrTrailer, GLB_CMT_SZ, 0 /* Mode */ );
  
  // Builts result string
    
  sprintf ( VRetStr, "%s %s %s", VStrEw, VStrChkSum, VStrTrailer );
  
  VStrLen = strlen ( VRetStr );
  
  // Copy result to DestStr if possible + errors messages
  
  if ( DestStr == NULL ) {
    err_error (( ERR_OUT, "DestStr == NULL => Returns local variable, DestStr not updated" ));
  }
  
  if ( DestStrSz <= VStrLen ) {
    err_error (( ERR_OUT, "DestStrSz=%d <= VStrLen=%d => Returns local variable, DestStrSz is too small", DestStrSz, VStrLen ));
  }
  
  if ( (DestStr != NULL) && (DestStrSz > VStrLen) ) {
    strcpy ( DestStr, VRetStr );
    return (DestStr);
  }
  
  else {
    return (VRetStr);
  }
  
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFrEwChkSumTrailerCheck8Out ( MIS1__TDsFrEwChkSumTrailer* Pt, UInt32 AcqNoForPrint, UInt32 FrNoForPrint, UInt16 W7, UInt16 W6, UInt16 W5, UInt16 W4, UInt16 W3, UInt16 W2, UInt16 CheckSum, MIS1__TDsFrTrailer Trailer, MIS1__TDxErrCnt* PtResDxErrCnt, SInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*          :
* \brief   : Checks the Empty Words, check sum, trailer fields / expected values, , for MSIS1 in 8 outputs mode
*          :
* \param   : Pt           - A pointer to the empty words, check sum and trailer record
*          :
* \param   : AcqNoForPrint- No of the acquisition for error message text
*          :
* \param   : FrNoForPrint - No of the frame for error message text
*          :
* \param   : W7           - Expected value for W7 field (16 bits)
*          :
* \param   : W6           - Expected value for W6 field (16 bits)
*          :
* \param   : W5           - Expected value for W5 field (16 bits)
*          :
* \param   : W4           - Expected value for W4 field (16 bits)
*          :
* \param   : W3           - Expected value for W3 field (16 bits)
*          :
* \param   : W2           - Expected value for W2 field (16 bits)
*          :
* \param   : CheckSum     - Expected value for the checksum field
*          :
* \param   : Flags        - Expected value for the flags field
*          :
* \param   : PtResDxErrCnt - Stores errors count on each Dx, set it to NULL if not used. Warning, it adds errors to current count, no reset at beginning of function.
*          :
* \param   : PrintDest    - Print destination for errors
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : Memo         - Pointer to destination Memo if PrintDest = 2 or 3, set it to NULL if not used
*          :
* \param   : PtFile       - Pointer to text file if PrintDest = 4 or 5, set it to NULL if not used
*          :
* \return  : < 0 in case of sw error, example Pt == NULL
*          : = 0 if no error on header fields
*          : > 0 = number of errors on header fields
*          :
* \warning : Globals   :
* \warning : Remark    :
*          :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/11/2019
* \date    : Doc date  : 13/11/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FFrEwChkSumTrailerCheck8Out ( MIS1__TDsFrEwChkSumTrailer* Pt, UInt32 AcqNoForPrint, UInt32 FrNoForPrint, UInt16 W7, UInt16 W6, UInt16 W5, UInt16 W4, UInt16 W3, UInt16 W2, UInt16 CheckSum, MIS1__TDsFrTrailer Trailer, MIS1__TDxErrCnt* PtResDxErrCnt, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {
  
  SInt32 VErrCnt;
  char   VStrErr[GLB_CMT_SZ];
  
  // Check destination header pointer
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  
  // Resets errors cnt
  
  VErrCnt = 0;
  
  // Check W7 to W4 16 bits fields
  
  if ( Pt->AW16[7] != W7 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[7];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W7 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->AW16[7], W7 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
  
  if ( Pt->AW16[6] != W6 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[6];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W6 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->AW16[6], W6 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
  
  if ( Pt->AW16[5] != W5 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[5];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W5 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->AW16[5], W5 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
  
  if ( Pt->AW16[4] != W4 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[4];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W4 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->AW16[4], W4 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
  
  if ( Pt->AW16[3] != W3 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[3];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W3 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->AW16[3], W3 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }


  if ( Pt->AW16[2] != W2 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[2];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : W2 = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->AW16[2], W2 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
    
  
  if ( Pt->F.CheckSum.W16 != CheckSum ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[1];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : CheckSum = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.CheckSum, CheckSum );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
  
  if ( Pt->F.Trailer.W16 != Trailer.W16 ) {
    
    ++VErrCnt;

    if ( PtResDxErrCnt != NULL ) {
      ++PtResDxErrCnt->AErrCnt[0];
    }
    
    if ( PrintDest != 0 ) {
      sprintf ( VStrErr, "Header error Acq %.6d - Fr %.6d : Flags = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, Pt->F.Trailer.W16, Trailer.W16 );
      MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
    }
    
  }
  
        
  
  // If errors have been printed => Add a line "======" at end of text
  
  if ( (PrintDest != 0) && (VErrCnt > 0) ) {
    MIS1__FPrintStr ( "============================================================", PrintDest, Memo, PtFile );
  }
  
  
  
  return ( VErrCnt );
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFrDataAsW16Check8Out ( UInt16* PtSrc, SInt32 W16Nb, UInt32 AcqNoForPrint, UInt32 FrNoForPrint, UInt16* PtExpVal, MIS1__TDxErrCnt* PtResDxErrCnt, SInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*          :
* \brief   : Checks the data fields / expected values, for MSIS1 in 8 outputs mode
*          :
* \param   : PtSrc        - A pointer to the source data (as a W16 array))
*          :
* \param   : W16Nb        - W16 number to check = size of PtSrc array
*          :
* \param   : AcqNoForPrint- No of the acquisition for error message text
*          :
* \param   : FrNoForPrint - No of the frame for error message text
*          :
* \param   : PtExpVal     - Array of expected values, its size must be W16Nb
*          :
* \param   : PtResDxErrCnt - Stores errors count on each Dx, set it to NULL if not used. Warning, it adds errors to current count, no reset at beginning of function.
*          :
* \param   : PrintDest    - Print destination for errors
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : Memo         - Pointer to destination Memo if PrintDest = 2 or 3, set it to NULL if not used
*          :
* \param   : PtFile       - Pointer to text file if PrintDest = 4 or 5, set it to NULL if not used
*          :
* \return  : < 0 in case of sw error, example Pt == NULL
*          : = 0 if no error on data fields
*          : > 0 = number of errors on data fields
*          :
* \warning : Globals   :
* \warning : Remark    :
*          :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     : MIS1__FFrHeader2Str ( MIS1__TDsFrHeader* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode )
*          :
* \date    : Date      : 14/11/2019
* \date    : Doc date  : 13/11/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FFrDataAsW16Check8Out ( UInt16* PtSrc, SInt32 W16Nb, UInt32 AcqNoForPrint, UInt32 FrNoForPrint, UInt16* PtExpVal, MIS1__TDxErrCnt* PtResDxErrCnt, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {
  
  SInt32 VErrCnt;
  char   VStrErr[GLB_CMT_SZ];
  SInt8  ViOut;
  SInt32 ViW16;
  SInt32 ViW128;
  
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  err_retnull ( PtExpVal, (ERR_OUT,"Abort => PtExpVal == NULL") );
  
  if ( (W16Nb < 0) || (W16Nb == 0) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => W16Nb = %d <= 0", W16Nb) );
  }
  
  // Resets errors cnt
  
  VErrCnt = 0;
  
  // ----------------------
  // Check data fields
  // ----------------------

  ViW16 = 0;

  while (1) {

    // Outputs D0..D7 loop
  
    for ( ViOut = 0 ; ViOut < 8; ViOut++ ) {
       
      if ( PtSrc[ViW16] != PtExpVal[ViW16] ) {
        
        ++VErrCnt;

        if ( PtResDxErrCnt != NULL ) {
          ++PtResDxErrCnt->AErrCnt[ViOut];
        }

        if ( PrintDest != 0 ) {
          sprintf ( VStrErr, "Data error Acq %.6d - Fr %.6d W16[%.6d] : W%d = D0%d = %x H <> expected = %x H", AcqNoForPrint, FrNoForPrint, ViW16, ViOut, ViOut, PtSrc[ViW16], PtExpVal[ViW16] );
          MIS1__FPrintStr ( VStrErr, PrintDest, Memo, PtFile );
        }
    
      } // End if ( PtSrc[ViW16] != PtExpVal[ViW16] )
    
      ++ViW16;
        
    } // End for ( ViOut )
    

    // End of src buffer reached
      
    if ( ViW16 >= W16Nb ) {
      break;
    }
      
  } // End while (1)

    
  // VDataW16NbChecked = ViW16;
  
  // ++VDataW16NbChecked;
  
  // If errors have been printed => Add a line "======" at end of text
  
  if ( (PrintDest != 0) && (VErrCnt > 0) ) {
    MIS1__FPrintStr ( "============================================================", PrintDest, Memo, PtFile );
  }
  
  
  
  return ( VErrCnt );
}






/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRegHeaderSetDefTags ( MIS1__TDsRegHeader* PtDest )
*          :
* \brief   : Sets the default values (MIS1__DS_TAG_REGION) of region header tags
*          :
* \param   : PtDest     - A pointer to the region header
*          :
* \return  : Error code
*          :   0  - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/05/2019
* \date    : Doc date  : 13/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRegHeaderSetDefTags ( MIS1__TDsRegHeader* PtDest ) {
  
  
  // Check destination header pointer
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  // Sets tag field
  
  PtDest->F.Tag  = MIS1__DS_TAG_REGION;
  PtDest->F.Zero = 0;
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRegHeaderSetReg ( MIS1__TDsRegHeader* PtDest, UInt8 Reg )
*          :
* \brief   : Sets the region value of region header 
*          :
* \param   : PtDest     - A pointer to the region header field
*          :
* \param   : Reg        - The region
*          :
* \return  : Error code
*          :   0  - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/05/2019
* \date    : Doc date  : 13/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRegHeaderSetReg ( MIS1__TDsRegHeader* PtDest, UInt8 Reg ) {
  
  
  // Check destination header pointer
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  // Check the region value
  
  if ( Reg >= MIS1__DS_MAX_REGION_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Reg=%d >= MIS1__DS_MAX_REGION_NB=%d", Reg, MIS1__DS_MAX_REGION_NB ) );
  }
  
  // Sets region field
  
  PtDest->F.Reg  = Reg;
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 SInt32 MIS1__FRegHeaderGetReg ( MIS1__TDsRegHeader* Pt )
*          :
* \brief   : Gets region value of region header field
*          :
* \param   : PtDest     - A pointer to the region header
*          :
* \return  : The region value of an error code
*          : >= 0 - Region value
*          : < 0  - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/05/2019
* \date    : Doc date  : 13/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRegHeaderGetReg ( MIS1__TDsRegHeader* Pt ) {
  
  
  // Check destination header pointer
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  return (Pt->F.Reg);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : char* MIS1__FRegHeader2Str ( MIS1__TDsRegHeader* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode )
*          :
* \brief   : Converts the data stream region field in a string
*          :
* \param   : PtSrc     - A pointer to the region header
*          :
* \param   : DestStr   - Destination string
*          :
* \param   : DestStrSz - Size of destination string
*          :
* \param   : Mode      - Print mode, reserved for future use
*          :
* \return  : A string which contains the region header
*          : - DestStr if provided as non NULL pointer and if size > required
*          : - A static local string in case DestStr can't be used
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 07/05/2019
* \date    : Doc date  : 07/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

char* MIS1__FRegHeader2Str ( MIS1__TDsRegHeader* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode ) {
  
  static   char   VRetStr[GLB_CMT_SZ];
  SInt32 VStrLen;
  
  // Check header pointer
  
  if ( PtSrc == NULL ) {
    sprintf ( VRetStr, "Abort => PtSrc == NULL" );
    err_error (( ERR_OUT, "%s", VRetStr ));
    return (VRetStr);
  }
  
  
  // Builts result string
  
  sprintf ( VRetStr, "Tag = %x (H) - Zero = %d - Region = %d", PtSrc->F.Tag, PtSrc->F.Zero, PtSrc->F.Reg );
  
  VStrLen = strlen ( VRetStr );
  
  // Copy result to DestStr if possible + errors messages
  
  if ( DestStr == NULL ) {
    err_error (( ERR_OUT, "DestStr == NULL => Returns local variable, DestStr not updated" ));
  }
  
  if ( DestStrSz <= VStrLen ) {
    err_error (( ERR_OUT, "DestStrSz=%d <= VStrLen=%d => Returns local variable, DestStrSz is too small", DestStrSz, VStrLen ));
  }
  
  if ( (DestStr != NULL) && (DestStrSz > VStrLen) ) {
    strcpy ( DestStr, VRetStr );
    return (DestStr);
  }
  
  else {
    return (VRetStr);
  }
  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32Save ( MIS1__TMatPixS32* PtSrc, char* FileName )
*
* \brief   : Save MIS1__TMatPixS32* to file
*
* \param   : PtSrc    - Pointer to source matrix
*
* \param   : FileName - File name
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/08/2020
* \date    : Doc date  : 13/08/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixS32Save ( MIS1__TMatPixS32* PtSrc, char* FileName ) {
  
  SInt32 VRet;
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  
  // Save
  
  VRet = FIL_FWriteRecord ( FileName, PtSrc, sizeof (MIS1__TMatPixS32) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => FIL_FWriteRecord (PtSrc, File=%s) Ret = %d", FileName, VRet) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32Load ( MIS1__TMatPixS32* PtDest, char* FileName )
*
* \brief   : Load MIS1__TMatPixS32* from file
*
* \param   : PtDest   - Pointer to destination matrix
*
* \param   : FileName - File name
*
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/08/2020
* \date    : Doc date  : 13/08/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixS32Load ( MIS1__TMatPixS32* PtDest, char* FileName ) {
  
  SInt32 VRet;
  SInt32 VExpectedFileSz;

  
  // Check param
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  
  // Check file format via file size
  
  VRet = FIL_FFileSize ( FileName );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => FIL_FFileSize (File=%s) failed Ret = %d", FileName, VRet) );
  
  // Check size
  
  VExpectedFileSz = sizeof (MIS1__TMatPixS32);  
  
  if ( VRet != VExpectedFileSz ) {
    err_retfail ( -1, (ERR_OUT,"Abort => File size = %d <> MIS1__TMatPixS32 size = %d => Bad file format, U16 instead of S32 ?", VRet, VExpectedFileSz ) );
  }
  
  // Load
  
  VRet = FIL_FReadRecord ( FileName, PtDest, sizeof (MIS1__TMatPixS32) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => FIL_FReadRecord (PtDest, File=%s) Ret = %d", FileName, VRet) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FRstAllCnt ( SInt8 MSisId )
*          :
* \brief   : Reset the Acq OVF counters, to be called at scan beginning   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FRstAllCnt ( SInt8 MSisId ) {
  
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  memset ( AWarnErrCnt[MSisId], 0, sizeof(AWarnErrCnt[0][0]) * MIS1__WE_NB );
  
  return (0);
}




// 999

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixCntU16FreeAlloc ( MIS1__TMatPixCntU16** PtPtMat, SInt8 FreeAlloc )
*          :
* \brief   : Free or allocate pixels matrix
*          :
* \param   : PtPtMat   - Address of a pointer on matrix seen as 2D array MIS1__TMatPixS32
*          :
* \param   : FreeAlloc - 0 => Free, 1 = Allocate
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/09/2020
* \date    : Doc date  : 17/09/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



SInt32 MIS1__FMatPixCntU16FreeAlloc ( MIS1__TMatPixCntU16** PtPtMat, SInt8 FreeAlloc ) {
  
  // Check param
  
  err_retnull ( PtPtMat, (ERR_OUT,"Abort => PtPtMat == NULL") );
  
  // Free
  
  if ( FreeAlloc == 0 ) {
    free ( *PtPtMat );
    *PtPtMat = NULL;
    err_retok (( ERR_OUT, "" ));
  }
  
  // Alloc
  
  *PtPtMat = (MIS1__TMatPixCntU16*) malloc ( sizeof (MIS1__TMatPixCntU16) );
  
  err_retnull ( *PtPtMat, (ERR_OUT,"Abort => Allocation has failed !") );
  
  err_retok (( ERR_OUT, "" ));
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32FreeAlloc ( MIS1__TMatPixS32** PtPtMat, SInt8 FreeAlloc )
*          :
* \brief   : Free or allocate pixels matrix
*          :
* \param   : PtPtMat   - Address of a pointer on matrix seen as 2D array MIS1__TMatPixS32
*          :
* \param   : FreeAlloc - 0 => Free, 1 = Allocate
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/09/2020
* \date    : Doc date  : 17/09/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32FreeAlloc ( MIS1__TMatPixS32** PtPtMat, SInt8 FreeAlloc ) {
  
  // Check param
  
  err_retnull ( PtPtMat, (ERR_OUT,"Abort => PtPtMat == NULL") );
  
  // Free
  
  if ( FreeAlloc == 0 ) {
    free ( *PtPtMat );
    *PtPtMat = NULL;
    err_retok (( ERR_OUT, "" ));
  }
  
  // Alloc
  
  *PtPtMat = (MIS1__TMatPixS32*) malloc ( sizeof (MIS1__TMatPixS32) );
  
  err_retnull ( *PtPtMat, (ERR_OUT,"Abort => Allocation has failed !") );
  
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixFloatFreeAlloc ( MIS1__TMatPixFloat** PtPtMat, SInt8 FreeAlloc )
*          :
* \brief   : Free or allocate pixels matrix
*          :
* \param   : PtPtMat   - Address of a pointer on matrix seen as 2D array MIS1__TMatPixFloat
*          :
* \param   : FreeAlloc - 0 => Free, 1 = Allocate
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/09/2020
* \date    : Doc date  : 17/09/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixFloatFreeAlloc ( MIS1__TMatPixFloat** PtPtMat, SInt8 FreeAlloc ) {
  
  // Check param
  
  err_retnull ( PtPtMat, (ERR_OUT,"Abort => PtPtMat == NULL") );
  
  // Free
  
  if ( FreeAlloc == 0 ) {
    free ( *PtPtMat );
    *PtPtMat = NULL;
    err_retok (( ERR_OUT, "" ));
  }
  
  // Alloc
  
  *PtPtMat = (MIS1__TMatPixFloat*) malloc ( sizeof (MIS1__TMatPixFloat) );
  
  err_retnull ( *PtPtMat, (ERR_OUT,"Abort => Allocation has failed !") );
  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32ResetAll ( MIS1__TMatPixS32* PtMat )
*          :
* \brief   : Clears or fires all pixels of matrix, for tests purpose
*          :
* \param   : PtMat        - Pointer on matrix seen as "bit" 2D array MIS1__TMatPixS32
*          :
* \param   : ClearFireAll - 0 => Clears all pixels, 1 = Fires all pixels
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 15/10/2020
* \date    : Doc date  : 15/10/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32ResetAll ( MIS1__TMatPixS32* PtMat ) {
  
  SInt32 VRet;
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  // Clears
  
  memset ( PtMat, 0, sizeof (MIS1__TMatPixS32) );
  
    
  return (0);
  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32ResetSetAll ( MIS1__TMatPixS32* PtMat, SInt8 ResetSetAll )
*          :
* \brief   : Clears or fires all pixels of matrix, for tests purpose
*          :
* \param   : PtMat        - Pointer on matrix seen as "bit" 2D array MIS1__TMatPixS32
*          :
* \param   : ClearFireAll - 0 => Clears all pixels, 1 = Fires all pixels
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 22/05/2019
* \date    : Doc date  : 22/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32ResetSetAll ( MIS1__TMatPixS32* PtMat, SInt8 ResetSetAll ) {
  
  SInt32 VRet;
  SInt16 ViRow;
  SInt16 ViCol;
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  // Clears / Fires
  
  for ( ViRow=0; ViRow < MIS1__ROW_NB; ViRow++ ) {
    
    for ( ViCol=0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      if ( ResetSetAll == 0 ) {
        (*PtMat)[ViCol][ViRow] = 0;
      }
      
      else {
        (*PtMat)[ViCol][ViRow] = 1;
      }
            
    }
        
  }
  
  return (0);
  
}




#define MIS1_NEW_RST_FUNC
#ifdef MIS1_NEW_RST_FUNC


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixFloatResetSetAll ( MIS1__TMatPixFloat* PtMat, SInt8 ResetSetAll )
*          :
* \brief   : Clears or fires all pixels of matrix, for tests purpose
*          :
* \param   : PtMat        - Pointer on matrix seen as "bit" 2D array MIS1__TMatPixS32
*          :
* \param   : ClearFireAll - 0 => Clears all pixels, 1 = Fires all pixels
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/09/2020
* \date    : Doc date  : 17/09/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixFloatResetSetAll ( MIS1__TMatPixFloat* PtMat, SInt8 ResetSetAll ) {
  
  SInt32 VRet;
  SInt16 ViRow;
  SInt16 ViCol;
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  // Clears / Fires
  
  for ( ViRow=0; ViRow < MIS1__ROW_NB; ViRow++ ) {
    
    for ( ViCol=0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      if ( ResetSetAll == 0 ) {
        (*PtMat)[ViCol][ViRow] = 0;
      }
      
      else {
        (*PtMat)[ViCol][ViRow] = 1;
      }
      
    }
    
  }
  
  return (0);
  
}


#endif


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32ResetSetPix ( MIS1__TMatPixS32* PtMat, SInt8 PixState, UInt16 PixX, UInt16 PixY  )
*          :
* \brief   : Clears or fires all pixels of matrix, for tests purpose
*          :
* \param   : PtMat        - Pointer on matrix seen as "bit" 2D array MIS1__TMatPixS32
*          :
* \param   : PixState - Pixel state 0 / 1
*          :
* \param   : PixX     - Pixel X coordinate = column
*          :
* \param   : PixY     - Pixel Y coordinate = row
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 22/05/2019
* \date    : Doc date  : 22/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32ResetSetPix ( MIS1__TMatPixS32* PtMat, SInt8 PixState, UInt16 PixX, UInt16 PixY  ) {
  
  SInt32 VRet;
  SInt16 ViRow;
  SInt16 ViCol;
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  if ( PixX >= MIS1__COL_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PixX = %d > %d", PixX, MIS1__COL_NB - 1 ) );
  }
  
  if ( PixY >= MIS1__ROW_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PixY = %d > %d", PixY, MIS1__ROW_NB - 1 ) );
  }
  
  // Clears / Fires pixel
  
  (*PtMat)[PixX][PixY] = PixState;
  
  return (0);
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32ResetSetFromPixList ( MIS1__TMatPixS32* PtMat, SInt8 ResetSet, MIS1__TPixXY* APixList, SInt32 PixNb )
*          :
* \brief   : resets / Sets pixels from list APixList in matrix PtMat, for tests purpose
*          :
* \param   : PtMat    - Pointer on matrix seen as "bit" 2D array MIS1__TMatPixS32
*          :
* \param   : ResetSet - 0 => reset pixels / 1 => sets pixels 
*          :
* \param   : APixList - Pixel list (X, Y) of typz MIS1__TPixXY, used to setr pixels
*          :
* \param   : PixNb    - Number of pixels in list APixList
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 22/05/2019
* \date    : Doc date  : 22/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32ResetSetFromPixList ( MIS1__TMatPixS32* PtMat, SInt8 ResetSet, MIS1__TPixXY* APixList, SInt32 PixNb ) {

  SInt32 VRet;
  SInt32 ViPix;
  SInt16 VRow;
  SInt16 VCol;
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  err_retnull ( APixList, (ERR_OUT,"Abort => APixList == NULL") );


  if ( PixNb > MIS1__PIX_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PixNb = %d > MIS1__PIX_NB = %d", PixNb, MIS1__PIX_NB ) );
  }


  // reset / Fires pixels from list in matrix
  
  for ( ViPix=0; ViPix < PixNb; ViPix++ ) {
    VRow = APixList[ViPix].C.y;
    VCol = APixList[ViPix].C.x;
    (*PtMat)[VCol][VRow] = ResetSet;
  }
  
  return (0);
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32Print ( MIS1__TMatPixS32* PtMat, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, SInt32 CountThL, SInt32 CountThH, TMemo* Memo, char* FileName )
*          :
* \brief   : Prints pixels states of Mimosis 1 matrix MIS1__TMatPixS32
*          :
* \param   : PtMat      - Pointer on Mimosis 1 matrix
*          :
* \param   : EncodeMth  - Encoding method (for future use) =>  is used now
*          :
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : PrintFormat - Print format ...
*          :   0 = No print
*          :   1 = fired pixels list,
*          :   2 = all pixels 2D matrix, with column no print in first line
*          :   3 = all pixels 2D matrix, with column no print in each line
*          :
* \param   : CountThL - If PrintFormat == 1, print pixel if CountThL <= value <= CountThH
*
* \param   : CountThH - If PrintFormat == 1, print pixel if CountThL <= value <= CountThH
:
* \param   : Memo        - Pointer to destination Memo if PrintDest = 2 or 3
*          :
* \param   : FileName     - Pointer to destination text file name if PrintDest = 4 or 5
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 22/05/2019
* \date    : Rev       : 12/08/2020
*                      : - Add param CountTh
* \date    : Doc date  : 22/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32Print ( MIS1__TMatPixS32* PtMat, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, SInt32 CountThL, SInt32 CountThH, TMemo* Memo, char* FileName, char* CsvFileName, char* HistoFileName  ) {
  
  SInt32  VRet;
  SInt32  VMatCol; // Source matrix column = X
  SInt32  VMatRow; // Source matrix row = Y
  SInt32  VPixCol; // Pixel column = X calculated via EncodeMth
  SInt32  VPixRow; // Pixel row = Y calculated via EncodeMth
  
  SInt32  VPixVal;
  
  static char    VStrColPixList[MIS1__STR_COL_LIST_MAX_SZ];
  static char    VStrColPix[GLB_CMT_SZ];
  static char    VStrInfo[GLB_CMT_SZ];
  
  FILE*  VPtFile;    
  FILE*  VPtFileCSV;
  FILE*  VPtFileHisto;
  
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );

  // Rq : EncodeMth param is checked in printing loops

  if ( PrintDest > 5 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintDest=%d > 5", PrintDest) );
  }
  
  if ( PrintFormat > 3 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintFormat=%d > 3", PrintFormat) );
  }

  if ( ((PrintDest == 2) || (PrintDest == 3)) && (Memo == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to memo requested BUT Memo = NULL !") );
  }

  if ( ((PrintDest == 4) || (PrintDest == 5)) && (FileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to texte file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (CsvFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to CSV file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (HistoFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to histo file requested BUT FileName = NULL !") );
  }
  


  // Create text file if PrintDest = 1, 4 or 5
  
  if ( (PrintDest == 1) ||(PrintDest == 4) || (PrintDest == 5) ) {
    
    VPtFile = fopen ( FileName, "wt" );   
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );
    
    VPtFileCSV = fopen ( CsvFileName, "wt" );
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );
    

    VPtFileHisto = fopen ( HistoFileName, "wt" );
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );
       
  }


  // Print info header
  
  MIS1__FPrintStr ( "-------------------------------------------------", PrintDest, Memo, VPtFile );
  sprintf ( VStrInfo, "List pixels with fired count in range %d to %d", CountThL, CountThH );
  MIS1__FPrintStr ( VStrInfo, PrintDest, Memo, VPtFile );
  MIS1__FPrintStr ( "-------------------------------------------------", PrintDest, Memo, VPtFile );
  MIS1__FPrintStr ( "", PrintDest, Memo, VPtFile );
  
  
   
  // Extracts pixels info & Prints  
  
  for ( VMatRow=0; VMatRow < MIS1__ROW_NB; VMatRow++ ) {
    
    // PrintFormat = 2 => Prints pixels as 2D matrix
    // - Print colmuns list at top of matrix
    
    if ( (PrintFormat == 2) && (VMatRow == 0) ) {
      
      sprintf ( VStrColPixList, "Col  : " );
      
      for ( VMatCol=0; VMatCol < MIS1__COL_NB; VMatCol++  ) {
        sprintf ( VStrColPix, "%.4d ", VMatCol );
        strcat ( VStrColPixList, VStrColPix );
      }
      
      MIS1__FPrintStr ( VStrColPixList, PrintDest, Memo, VPtFile );
      
    } // End if ( PrintFormat == 2 )
    
  
    if ( (PrintFormat == 2) || (PrintFormat == 3) ) {
      
      sprintf ( VStrColPixList, "%4d : ", VMatRow );
      
    } // End if ( PrintFormat == 2 )
    

    for ( VMatCol=0; VMatCol < MIS1__COL_NB; VMatCol++ ) {

      // Extracts pixels info

      switch ( EncodeMth ) {
        
        case 0 : {
          VPixRow = VMatRow;
          VPixCol = VMatCol;
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort => Unknown EncodeMth = %d", EncodeMth) );
          break; }
      }
      
      // PrintFormat = 1 => Prints pixels list
      
      // Before 30/07/2020
      //
      // if ( (PrintFormat == 1) && ((*PtMat)[VPixCol][VPixRow] == 1) ) {
      //  
      //  sprintf ( VStrColPix, "Pixel[x=%.4d,y=%.4d] = 1", VPixCol, VPixRow);

      //Since 30/07/2020
    
      VPixVal = (*PtMat)[VPixCol][VPixRow];
    
      if ( (PrintFormat == 1) && (VPixVal  >= CountThL) && (VPixVal  <= CountThH) ) {
        
        
        sprintf ( VStrColPix, "Pixel[x=%.4d,y=%.4d] = %.4d", VPixCol, VPixRow, VPixVal   );
        

        // Prints
        
        MIS1__FPrintStr ( VStrColPix, PrintDest, Memo, VPtFile );
        
        // Print to CSV file
        
        fprintf ( VPtFileCSV, "%d,%d,%d \n",  VPixCol, VPixRow, VPixVal  );
                
        // Print to histo file
        
        fprintf ( VPtFileHisto, "%d \n", VPixVal );
        
        
        
      } // End if (PrintFormat == 1)
      
      
      // PrintFormat = 2 => Prints pixels as 2D matrix => Builds line
      
      if ( PrintFormat == 2 ) {
        sprintf ( VStrColPix, "%.4d ", (*PtMat)[VPixCol][VPixRow] );
        strcat ( VStrColPixList, VStrColPix );
      } // End if ( PrintFormat == 2 )
               
      // PrintFormat = 3 => Prints pixels as 2D matrix => Builds line
      
      if ( PrintFormat == 3 ) {
        sprintf ( VStrColPix, "%.4d,%.4d ", VPixCol, (*PtMat)[VPixCol][VPixRow] );
        strcat ( VStrColPixList, VStrColPix );
      } // End if ( PrintFormat == 2 )
      
    } // End for (VMatCol)
    
        
    // PrintFormat = 2 => Prints pixels as 2D matrix => Prints line
    
    if ( (PrintFormat == 2) ||(PrintFormat == 3) ) {
      MIS1__FPrintStr ( VStrColPixList, PrintDest, Memo, VPtFile );
    } // End if ( PrintFormat == 2 )
    
    
  } // Enf for ( VMatRow )


  if ( (PrintDest == 1) || (PrintDest == 4) || (PrintDest == 5) ) {
    
    if ( fclose ( VPtFile ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", FileName) );
    }    
    
    if ( fclose ( VPtFileCSV ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", CsvFileName) );
    }
    
    
    if ( fclose ( VPtFileHisto ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", HistoFileName) );
    }
                
  }
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32ListFPCInCntRange ( MIS1__TMatPixS32* PtMat, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, SInt32 CountThL, SInt32 CountThH, SInt32* PtResCountMin, SInt32* PtResCountMax, float* PtResCountMean, TMemo* Memo, char* FileName )*          :
*          
* \brief   : Counts, and prints pixels with CountThL <= value ()fired count) <= CountThH of Mimosis 1 matrix MIS1__TMatPixS32
*          
* \param   : PtMat      - Pointer on Mimosis 1 matrix
*          
* \param   : EncodeMth  - Encoding method (for future use) =>  is used now
*          
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          
* \param   : PrintFormat - Print format ...
*          :   0 = No print (count only)
*          :   1 = fired pixels list
*          
* \param   : CountThL - Low count threshold
*
* \param   : CountThH - High count threshold
*          
* \param   : PtResCountMin  - Ptr to count min, set it to NULL if not used. Min for calculated for count > 0            
*            
* \param   : PtResCountMax  - Ptr to count max, set it to NULL if not used. Max for calculated for count >= 0
*          
* \param   : PtResCountMean - Ptr to count mean, set it to NULL if not used. Mean calculated for CountThL <= Count <= CountThH
*          
* \param   : Memo           - Pointer to destination Memo if PrintDest = 2 or 3
*          
* \param   : FileName       - Pointer to destination text file name if PrintDest = 4 or 5
*          
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 12/08/2020
* \date    : Doc date  : 12/08/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32ListFPCInCntRange ( MIS1__TMatPixS32* PtMat, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, SInt32 CountThL, SInt32 CountThH, SInt32* PtResCountMin, SInt32* PtResCountMax, float* PtResCountMean, TMemo* Memo, char* FileName ) {
  
  SInt32  VRet;
  SInt32  VMatCol; // Source matrix column = X
  SInt32  VMatRow; // Source matrix row = Y
  SInt32  VPixCol; // Pixel column = X calculated via EncodeMth
  SInt32  VPixRow; // Pixel row = Y calculated via EncodeMth
  
  SInt32  VPixVal;
  SInt32  VPixNbInRange;
  SInt8   VPixIsInRange;
  
  static char    VStrColPixList[MIS1__STR_COL_LIST_MAX_SZ];
  static char    VStrColPix[GLB_CMT_SZ];
  
  SInt32 VResCountMin;
  SInt32 VResCountMax;
  SInt64 VResCountSum;
  SInt8  VCountSumOvf;
  
  float  VResCountMean;
  SInt32 VVResPixNbFPCNot0; // Calculated but not returen as result
  
  SInt8  VMinFound;
  SInt8  VMaxFound;
  
  
  FILE*  VPtFile;
  
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  // Rq : EncodeMth param is checked in printing loops
  
  if ( PrintDest > 5 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintDest=%d > 5", PrintDest) );
  }
  
  if ( PrintFormat > 1 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintFormat=%d > 1", PrintFormat) );
  }
  
  if ( ((PrintDest == 2) || (PrintDest == 3)) && (Memo == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to memo requested BUT Memo = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (FileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to texte file requested BUT FileName = NULL !") );
  }
  
  
  // Create text file if PrintDest = 1, 4 or 5
  
  if ( (PrintDest == 1) ||(PrintDest == 4) || (PrintDest == 5) ) {
    VPtFile = fopen ( FileName, "wt" );
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );
  }
  
  
  // Extracts pixels info & Prints
  
  VPixNbInRange = 0;
  
  VResCountMin = 0x7FFFFFFF;
  VResCountMax = 0;
  VResCountSum = 0;
  
  VVResPixNbFPCNot0 = 0;
  
  VMinFound = 0;
  VMaxFound = 0;
    
  VCountSumOvf = 0;
  
  
  for ( VMatRow=0; VMatRow < MIS1__ROW_NB; VMatRow++ ) {
            
    for ( VMatCol=0; VMatCol < MIS1__COL_NB; VMatCol++ ) {
      
      // Extracts pixels info
      
      switch ( EncodeMth ) {
        
        case 0 : {
          VPixRow = VMatRow;
          VPixCol = VMatCol;
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort => Unknown EncodeMth = %d", EncodeMth) );
          break; }
      }
  
      VPixVal = (*PtMat)[VPixCol][VPixRow];
      
  
      // Check pix cnt range
      
      if ( (VPixVal >= CountThL) && (VPixVal <= CountThH) ) {
        VPixIsInRange = 1;
        VPixNbInRange++;
      }
      
      else {
        VPixIsInRange = 0;
      }
      

      // Update counter of pixels with FPC N > 0
      
      if ( VPixVal > 0 ) {
        ++VVResPixNbFPCNot0;
      }
      
  
      // Update count max, for % = 0 to 100 %
      
      if ( VPixVal > VResCountMax ) {
        VResCountMax = VPixVal;
        VMaxFound    = 1;
      }
      
      // Update count min, for % > 0 to 100 %
      // > 0 because otherwiase min always = 0 as there are always pixels % = 0
            
      if ( (VPixVal > 0) && (VPixVal  < VResCountMin) ) {
        VResCountMin = VPixVal;
        VMinFound    = 1;
      }
      
      // Stop summing if OVF
      
      if ( VResCountSum < 0 ) {
        VCountSumOvf = 1;
      }
      
      else {
        
        if ( VPixIsInRange == 1 ) {
          VResCountSum += VPixVal;
        }
        
      }
              

      // PrintFormat = 1 => Prints pixels list
              
      if ( (PrintFormat == 1) && (VPixIsInRange == 1) ) {
                   
        sprintf ( VStrColPix, "Pixel[x=%.4d,y=%.4d] = %.4d", VPixCol, VPixRow, (*PtMat)[VPixCol][VPixRow]  );
            
        // Prints
          
        MIS1__FPrintStr ( VStrColPix, PrintDest, Memo, VPtFile );
          
      } // End if (PrintFormat == 1)
        
                
    } // End for (VMatCol)
                  
      
  } // End for ( VMatRow )
    
  
  if ( VMinFound == 0 ) {
    VResCountMin = -1;
  }
  
  if ( VMaxFound == 0 ) {
    VResCountMax = -1;
  }
  
  
  
  if ((VCountSumOvf == 1) || ( VPixNbInRange > MIS1__PIX_NB)  ) {
    VResCountMean = -1;
  }
  
  else {
    VResCountMean = (float) VResCountSum / (float) VPixNbInRange;
  }

    
  if ( (PrintDest == 1) || (PrintDest == 4) || (PrintDest == 5) ) {
      
    if ( fclose ( VPtFile ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", FileName) );
    }
    
  }
    
    
  if ( PtResCountMin != NULL ) {
    *PtResCountMin = VResCountMin;
  }
    
  if ( PtResCountMax != NULL ) {
    *PtResCountMax = VResCountMax;
  }
  
  if ( PtResCountMean != NULL ) {
    *PtResCountMean = VResCountMean;
  }
   
  return (VPixNbInRange);
}
  
  
  

  
  
/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixFloatListFPCInPCentRange ( MIS1__TMatPixFloat* PtMat, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, float PCentThL, float PCentThH, float* PtResPCentMin, float* PtResPCentMax, TMemo* Memo, char* FileName )
*          :
* \brief   : Counts, and prints pixels with PCentThL <= value ()fired count) <= PCentThH of Mimosis 1 matrix MIS1__TMatPixFloat
*          :
* \param   : PtMat      - Pointer on Mimosis 1 matrix
*          :
* \param   : EncodeMth  - Encoding method (for future use) =>  is used now
*          :
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : PrintFormat - Print format ...
*          :   0 = No print (count only)
*          :   1 = fired pixels list
*          :
* \param   : PCentThL - Low % threshold
*
* \param   : PCentThH - High % threshold
*          :
* \param   : PtResPCentMin  - Ptr to % min, set it to NULL if not used. Min for calculated for % > 0
*          
* \param   : PtResPCentMax  - Ptr to % max, set it to NULL if not used. Max for calculated for % >= 0
*          
* \param   : PtResPCentMean - Ptr to % mean, set it to NULL if not used. Mean calculated for PCentThL <= % <= PCentThH
*          
* \param   : Memo           - Pointer to destination Memo if PrintDest = 2 or 3
*          :
* \param   : FileName       - Pointer to destination text file name if PrintDest = 4 or 5
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/09/2020
* \date    : Doc date  : 17/09/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixFloatListFPCInPCentRange ( MIS1__TMatPixFloat* PtMat, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, float PCentThL, float PCentThH, float* PtResPCentMin, float* PtResPCentMax, float* PtResPCentMean, TMemo* Memo, char* FileName, char* CsvFileName, char* HistoFileName ) {
  
  SInt32  VRet;
  SInt32  VMatCol; // Source matrix column = X
  SInt32  VMatRow; // Source matrix row = Y
  SInt32  VPixCol; // Pixel column = X calculated via EncodeMth
  SInt32  VPixRow; // Pixel row = Y calculated via EncodeMth
  
  float   VPixVal;
  SInt32  VPixNbInRange;
  SInt8   VPixIsInRange;
  
  static char    VStrColPixList[MIS1__STR_COL_LIST_MAX_SZ];
  static char    VStrColPix[GLB_CMT_SZ];
  
  float  VResPCentMin;
  float  VResPCentMax;
  SInt64 VResPCentSum;
  float  VResPCentMean;
  SInt8  VPCentSumOvf;
  
  
  SInt8  VMinFound;
  SInt8  VMaxFound;
  
  
  SInt32 VResPixNbSupMinPCent; // Nb of pxiel with % >= VMinPCent, Calculated but not returned
  float  VMinPCent = 1; // Minimum % used to calculate VResPixNbSupMinPCent
  
  FILE*  VPtFile;
  FILE*  VPtFileCSV;
  FILE*  VPtFileHisto;
  
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  // Rq : EncodeMth param is checked in printing loops
  
  if ( PrintDest > 5 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintDest=%d > 5", PrintDest) );
  }
  
  if ( PrintFormat > 1 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintFormat=%d > 1", PrintFormat) );
  }
  
  if ( ((PrintDest == 2) || (PrintDest == 3)) && (Memo == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to memo requested BUT Memo = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (FileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to texte file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (CsvFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to CSV file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (HistoFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to histo file requested BUT FileName = NULL !") );
  }
  
  
  
  // Create text file if PrintDest = 1, 4 or 5
  
  if ( (PrintDest == 1) ||(PrintDest == 4) || (PrintDest == 5) ) {
    
    VPtFile = fopen ( FileName, "wt" );
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );
    
    VPtFileCSV = fopen ( CsvFileName, "wt" );
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );

    VPtFileHisto = fopen ( HistoFileName, "wt" );
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );  
    
  }
  
  
  // Extracts pixels info & Prints
  
  VPixNbInRange = 0;
  
  VResPCentMin = 100;
  VResPCentMax = 0;
  VResPCentSum = 0;
  
  VMinFound = 0;
  VMaxFound = 0;
  
  VResPixNbSupMinPCent = 0;
  
  
  for ( VMatRow=0; VMatRow < MIS1__ROW_NB; VMatRow++ ) {
    
    for ( VMatCol=0; VMatCol < MIS1__COL_NB; VMatCol++ ) {
      
      // Extracts pixels info
      
      switch ( EncodeMth ) {
        
        case 0 : {
          VPixRow = VMatRow;
          VPixCol = VMatCol;
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort => Unknown EncodeMth = %d", EncodeMth) );
          break; }
      }
      
      VPixVal = (*PtMat)[VPixCol][VPixRow];


      // Check pix cnt range
      
      if ( (VPixVal >= PCentThL) && (VPixVal <= PCentThH) ) {
        VPixIsInRange = 1;
        VPixNbInRange++;
      }
      
      else {
        VPixIsInRange =0;
      }
      

      // Update counter of pixels with FPC % >= VMinPCent
      
      if ( VPixVal >= VMinPCent ) {
        ++VResPixNbSupMinPCent;
      }
      
      // Update % count max, for % = 0 to 100 %
      
      if ( VPixVal > VResPCentMax ) {
        VResPCentMax = VPixVal;
        VMaxFound = 1;
      }
      
      // Update % count min, for % > 0 to 100 %
      // > 0 because otherwiase min always = 0 as there are always pixels % = 0
            
      if ( (VPixVal > 0) && (VPixVal  < VResPCentMin) ) {
        VResPCentMin = VPixVal;
        VMinFound = 1;
      }
            
      
      // Stop summing if OVF or pixel value is out of thresholds range
      
      if ( VResPCentSum < 0 ) {
        VPCentSumOvf = 1;
      }
      
      else {
        
        if ( VPixIsInRange == 1 ) {
          VResPCentSum += VPixVal;
        }
        
      }
      
      
      
      // PrintFormat = 1 => Prints pixels list
      
      if ( (PrintFormat == 1) && (VPixIsInRange == 1) ) {
        
        sprintf ( VStrColPix, "Pixel[x=%.4d,y=%.4d] = %.1f", VPixCol, VPixRow, VPixVal  );
        
        // Prints
        
        MIS1__FPrintStr ( VStrColPix, PrintDest, Memo, VPtFile );
        
        // Print to CSV file
        
        fprintf ( VPtFileCSV, "%d,%d,%.1f \n",  VPixCol, VPixRow, VPixVal  );
        
        // Print to histo file
        
        fprintf ( VPtFileHisto, "%.1f \n", VPixVal );
        
        
        
      } // End if (PrintFormat == 1)
      
      
    } // End for (VMatCol)
    
    
  } // End for ( VMatRow )
  
  
  if ( VMinFound == 0 ) {
    VResPCentMin = -1;
  }
  
  if ( VMaxFound == 0 ) {
    VResPCentMax = -1;
  }
  
  
  if ( (VPCentSumOvf == 1) || (VPixNbInRange > MIS1__PIX_NB) ) {
    VResPCentMean = -1;
  }
  
  
  else {
    VResPCentMean = (float) VResPCentSum  / (float) ( VPixNbInRange );
  }
   
  
  if ( (PrintDest == 1) || (PrintDest == 4) || (PrintDest == 5) ) {
    
    if ( fclose ( VPtFile ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", FileName) );
    }
    
    if ( fclose ( VPtFileCSV ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", CsvFileName) );
    }
    
    
    if ( fclose ( VPtFileHisto ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", HistoFileName) );
    }
    
    
  }
  
  
  if ( PtResPCentMin != NULL ) {
    *PtResPCentMin = VResPCentMin;
  }
  
  if ( PtResPCentMax != NULL ) {
    *PtResPCentMax = VResPCentMax;
  }
  
  if ( PtResPCentMean != NULL ) {
    *PtResPCentMean = VResPCentMean;
  }
  
  return (VPixNbInRange);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixFloatListFPCInPCentRangeExt ( MIS1__TMatPixFloat* PtMat, UInt16 FirstLine, UInt16 LastLine, UInt16 FirstCol, UInt16 LastCol, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, float PCentThL, float PCentThH, float* PtResPCentMin, float* PtResPCentMax, TMemo* Memo, char* FileName )
*          :
* \brief   : Counts, and prints pixels with PCentThL < value ()fired count) <= PCentThH of Mimosis 1 matrix MIS1__TMatPixFloat
*          :
* \param   : PtMat     - Pointer on Mimosis 1 matrix
*          :
* \param   : FirstLine - 
*          :
* \param   : LastLine  -
*          :
* \param   : FirstCol  -
*          :
* \param   : FirstCol  -
*          :
* \param   : EncodeMth  - Encoding method (for future use) =>  is used now
*          :
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : PrintFormat - Print format ...
*          :   0 = No print (count only)
*          :   1 = fired pixels list
*          :
* \param   : PCentThL - Low % threshold
*
* \param   : PCentThH - High % threshold
*
* \param   : PtResPCentMin  - Ptr to % min, set it to NULL if not used. Min for calculated for % > 0
*
* \param   : PtResPCentMax  - Ptr to % max, set it to NULL if not used. Max for calculated for % >= 0
*
* \param   : PtResPCentMean - Ptr to % mean, set it to NULL if not used. Mean calculated for PCentThL <= % <= PCentThH
*          
* \param   : Memo           - Pointer to destination Memo if PrintDest = 2 or 3
*          :
* \param   : FileName       - Pointer to destination text file name if PrintDest = 4 or 5
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/09/2020
* \date    : Doc date  : 17/09/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixFloatListFPCInPCentRangeExt ( MIS1__TMatPixFloat* PtMat, UInt16 FirstLine, UInt16 LastLine, UInt16 FirstCol, UInt16 LastCol, UInt8 EncodeMth, UInt8 PrintDest, UInt8 PrintFormat, float PCentThL, float PCentThH, float* PtResPCentMin, float* PtResPCentMax, float* PtResPCentMean, TMemo* Memo,  char* FileName, char* CsvFileName, char* HistoFileName  ) {
  
  SInt32  VRet;
  SInt32  VMatCol; // Source matrix column = X
  SInt32  VMatRow; // Source matrix row = Y
  SInt32  VPixCol; // Pixel column = X calculated via EncodeMth
  SInt32  VPixRow; // Pixel row = Y calculated via EncodeMth
  
  float   VPixVal;
  SInt32  VPixNbInRange;
  SInt8   VPixIsInRange;
  
  static char    VStrColPixList[MIS1__STR_COL_LIST_MAX_SZ];
  static char    VStrColPix[GLB_CMT_SZ];
  
  float  VResPCentMin;
  float  VResPCentMax;
  SInt64 VResPCentSum;
  float  VResPCentMean;
  SInt8  VPCentSumOvf;
  
  
  SInt32 VResPixNbSupMinPCent; // Nb of pxiel with % >= VMinPCent, Calculated but not returned
  float  VMinPCent = 1; // Minimum % used to calculate VResPixNbSupMinPCent

  SInt8  VMinFound;
  SInt8  VMaxFound;
  
  
  FILE*  VPtFile;
  FILE*  VPtFileCSV;
  FILE*  VPtFileHisto;
  
  
  // Check param
  
  err_retnull ( PtMat, (ERR_OUT,"Abort => PtMat == NULL") );
  
  if ( (FirstLine >= MIS1__ROW_NB) || (LastLine >= MIS1__ROW_NB) || (FirstLine > LastLine)  ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Bad lines range %d to %d MUST be in 0 to %d", FirstLine, LastLine, MIS1__ROW_NB - 1 ) );
  }
  
  if ( (FirstCol >= MIS1__COL_NB) || (LastCol >= MIS1__COL_NB) || (FirstCol > LastCol) )  {
    err_retfail ( -1, (ERR_OUT,"Abort => Bad columns range %d to %d MUST be in 0 to %d", FirstCol, LastCol, MIS1__COL_NB - 1 ) );
  }
  
    
  // Rq : EncodeMth param is checked in printing loops
  
  if ( PrintDest > 5 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintDest=%d > 5", PrintDest) );
  }
  
  if ( PrintFormat > 1 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintFormat=%d > 1", PrintFormat) );
  }
  
  if ( ((PrintDest == 2) || (PrintDest == 3)) && (Memo == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to memo requested BUT Memo = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (FileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to texte file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (CsvFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to CSV file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (HistoFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to histo file requested BUT FileName = NULL !") );
  }
  
  
  
  // Create text file if PrintDest = 1, 4 or 5
  
  if ( (PrintDest == 1) ||(PrintDest == 4) || (PrintDest == 5) ) {
    
    VPtFile = fopen ( FileName, "wt" );
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create file %s failed !", FileName) );
    
    VPtFileCSV = fopen ( CsvFileName, "wt" );
    err_retnull ( VPtFileCSV, (ERR_OUT,"Abort => Create file %s failed !", CsvFileName) );
    
    
    VPtFileHisto = fopen ( HistoFileName, "wt" );
    err_retnull ( VPtFileHisto, (ERR_OUT,"Abort => Create file %s failed !", HistoFileName) );
    
    
  }
  
  
  // Extracts pixels info & Prints
  
  VPixNbInRange = 0;
  
  VResPCentMin = 100;
  VResPCentMax = 0;
  VResPCentSum = 0;
  
  VPCentSumOvf = 0;
  
  VResPixNbSupMinPCent = 0; // Calculated but not returned
  
  VMinFound = 0;
  VMaxFound = 0;
  
  
  for ( VMatRow=FirstLine; VMatRow <= LastLine; VMatRow++ ) {
    
    for ( VMatCol=FirstCol; VMatCol <= LastCol; VMatCol++ ) {
      
      // Extracts pixels info
      
      switch ( EncodeMth ) {
        
        case 0 : {
          VPixRow = VMatRow;
          VPixCol = VMatCol;
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort => Unknown EncodeMth = %d", EncodeMth) );
          break; }
      }
      
      VPixVal = (*PtMat)[VPixCol][VPixRow];
    
  
      // Check pix cnt range
      
      if ( (VPixVal > PCentThL) && (VPixVal <= PCentThH) ) {
        VPixIsInRange = 1;
        VPixNbInRange++;
      }
      
      else {
        VPixIsInRange =0;
      }
      
  
      // Update counter of pixels with FPC % >= VMinPCent
      
      if ( VPixVal >= VMinPCent ) {
        ++VResPixNbSupMinPCent;
      }
      
      // Update % count max, for % = 0 to 100 %
      
      if ( VPixVal > VResPCentMax ) {
        VResPCentMax = VPixVal;
        VMaxFound    = 1;
      }
      
      // Update % count min, for % > 0 to 100 %
      // > 0 because otherwiase min always = 0 as there are always pixels % = 0
      
      if ( (VPixVal  > 0) && (VPixVal  < VResPCentMin) ) {
        VResPCentMin = VPixVal;
        VMinFound    = 1;
      }
      
      // Stop summing if OVF or pixel value is out of thresholds range
      
      if ( VResPCentSum < 0 ) {
        VPCentSumOvf = 1;
      }
      
      else {
        
        if ( VPixIsInRange == 1 ) {
          VResPCentSum += VPixVal;
        }
        
      }
      
                            
      // PrintFormat = 1 => Prints pixels list
      
      if ( (PrintFormat == 1) && (VPixIsInRange == 1) ) {
        
        sprintf ( VStrColPix, "Pixel[x=%.4d,y=%.4d] = %3.9f", VPixCol, VPixRow, VPixVal  );
        
        // Prints
        
        MIS1__FPrintStr ( VStrColPix, PrintDest, Memo, VPtFile );
        
        
        // Print to CSV file
        
        fprintf ( VPtFileCSV, "%d,%d,%3.9f \n",  VPixCol, VPixRow, VPixVal  );
        
        // Print to histo file
        
        fprintf ( VPtFileHisto, "%3.9f \n", VPixVal );
        
        
        
      } // End if (PrintFormat == 1)
      
      
    } // End for (VMatCol)
    
    
  } // End for ( VMatRow )
  
    

  if ( VMinFound == 0 ) {
    VResPCentMin = -1;
  }
  
  if ( VMaxFound == 0 ) {
    VResPCentMax = -1;
  }
  
  

  if ( (VPCentSumOvf == 1) || (VPixNbInRange > MIS1__PIX_NB) ) {
    VResPCentMean = -1;
  }
  
  
  else {
    
    if ( VPixNbInRange == 0 ) {
      VResPCentMean = -1; 
    }
    
    else {
      VResPCentMean = (float) VResPCentSum / (float) ( VPixNbInRange );
    }

    
  }
  
    
  
  if ( (PrintDest == 1) || (PrintDest == 4) || (PrintDest == 5) ) {
    
    if ( fclose ( VPtFile ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", FileName) );
    }
    
    if ( fclose ( VPtFileCSV ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", CsvFileName) );
    }
    
    
    if ( fclose ( VPtFileHisto ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", HistoFileName) );
    }
    
    
    
  }
  
  
  if ( PtResPCentMin != NULL ) {
    *PtResPCentMin = VResPCentMin;
  }
  
  if ( PtResPCentMax != NULL ) {
    *PtResPCentMax = VResPCentMax;
  }
  
  if ( PtResPCentMean != NULL ) {
    *PtResPCentMean = VResPCentMean;
  }
  
  return (VPixNbInRange);
}





// 1664

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1_FMatCumToMatPCent ( MIS1__TMatPixS32* PtMatSrc, MIS1__TMatPixFloat* PtMatDest, SInt32 FrNb )
*          :
* \brief   : This function convert cumul matrix to % / S curve data mattrix    \n
*          :
* \param   : PtMatSrc - Source matrix
*          :
* \param   : PtMatDest - Destination matrix
*          :
* \param   : FrNb      - Frames number
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : The user should not modify this function
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/09/2020
* \date    : Doc date  :
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1_FMatCumToMatPCent ( MIS1__TMatPixS32* PtMatSrc, MIS1__TMatPixFloat* PtMatDest, SInt32 FrNb ) {
  
  SInt32 Vx, Vy;
  float  VPixVal;
  
  // Check param
  
  err_retnull ( PtMatSrc, (ERR_OUT,"Abort => PtMatSrc == NULL") );
  
  err_retnull ( PtMatDest, (ERR_OUT,"Abort => PtMatDest == NULL") );
  
  // Scan matrix
  // [MIS1__COL_NB][MIS1__ROW_NB]
  
  for ( Vx=0; Vx < MIS1__COL_NB; Vx++ ) {
    
    
    for ( Vy=0; Vy < MIS1__ROW_NB; Vy++ ) {
      
      VPixVal = (*PtMatSrc)[Vx][Vy];
      
      if ( VPixVal == 0 ) {
        (*PtMatDest)[Vx][Vy] = 0;
      }
      
      else {
        (*PtMatDest)[Vx][Vy] = ( (float) VPixVal / (float) FrNb ) * 100;
      }
      
      
    } // End for ( Vy )
    
  } // End for ( Vx )
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanAlloc ( )
*          :
* \brief   : 
*          :
* \param   : 
*          :
* \param   : 
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanAlloc ( ) {
  
  
  MIS1__VGPtRecResScan = (MIS1__TRecResScan*) malloc ( sizeof (MIS1__TRecResScan) );
  
  err_retnull ( MIS1__VGPtRecResScan, (ERR_OUT,"Abort => Alloc MIS1__VGPtRecResScan failed !") );
  

  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanFree ( )
*          :
* \brief   :
*          :
* \param   :
*          :
* \param   :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanFree ( ) {
  

  err_retnull ( MIS1__VGPtRecResScan, (ERR_OUT,"Abort => MIS1__VGPtRecResScan already == NULL") );
  
  free ( MIS1__VGPtRecResScan );
  
  MIS1__VGPtRecResScan = NULL;  
  
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanResetAll ( MIS1__TRecResScan* PtRec )
*          :
* \brief   : Fill record with 0
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanResetAll ( MIS1__TRecResScan* PtRec ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );  
  
  // Reset
  
  memset ( PtRec, 0, sizeof (MIS1__TRecResScan) );    
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanResetAll ()
*          :
* \brief   : Fill record with 0
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanResetAll ( ) {
  
  return ( MIS1__FRecResScanResetAll ( MIS1__VGPtRecResScan ) );
  
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanReset ( MIS1__TRecResScan* PtRec )
*          :
* \brief   : Fill record with 0
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/10/2020
* \date    : Doc date  : 01/10/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanResetRes ( MIS1__TRecResScan* PtRec ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  
  // Reset
  
  memset ( &(PtRec->R.ResMat), 0, sizeof (MIS1__TMatScanFloat) );
   
  memset ( PtRec->R.APCentMin                 , -1, MIS1__COL_NB * MIS1__ROW_NB * sizeof (PtRec->R.APCentMin[0][0]) );
  memset ( PtRec->R.APCentMax                 , -1, MIS1__COL_NB * MIS1__ROW_NB * sizeof (PtRec->R.APCentMax[0][0]) );
  memset ( PtRec->R.AResScanRegValForLowPCent , -1, MIS1__COL_NB * MIS1__ROW_NB * sizeof (PtRec->R.AResScanRegValForLowPCent[0][0]) );
  memset ( PtRec->R.AResScanRegValForHighPCent, -1, MIS1__COL_NB * MIS1__ROW_NB * sizeof (PtRec->R.AResScanRegValForHighPCent[0][0]) );
  memset ( PtRec->R.AResScanRegValForMidPCent , -1, MIS1__COL_NB * MIS1__ROW_NB * sizeof (PtRec->R.AResScanRegValForMidPCent[0][0]) );
    
    
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanReset ()
*          :
* \brief   : Fill record with 0
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanResetRes () {

  return ( MIS1__FRecResScanResetRes (MIS1__VGPtRecResScan) );
  
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSaveBin ( MIS1__TRecResScan* PtRec, char* FileName )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/10/2020
* \date    : Doc date  : 01/10/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSaveBin ( MIS1__TRecResScan* PtRec, char* FileName ) {
  
  SInt32  VRet;


  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL" ) );
  
  // Save file
  
  VRet = FIL_FWriteRecord ( FileName, PtRec, sizeof ( MIS1__TRecResScan) );
  
  err_retfail ( VRet, (ERR_OUT,"Saving record to file %s failed !", FileName ) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSaveBin ( char* FileName )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSaveBin ( char* FileName ) {

  return ( MIS1__FRecResScanSaveBin ( MIS1__VGPtRecResScan, FileName ) );
  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanParSaveBin ( MIS1__TRecResScanPar* PtRec, char* FileName )
*          :
* \brief   : Save scan record PARAMETERS part in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/11/2020
* \date    : Doc date  : 24/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanParSaveBin ( MIS1__TRecResScanPar* PtRec, char* FileName ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL" ) );
  
  // Save file
  
  VRet = FIL_FWriteRecord ( FileName, PtRec, sizeof (MIS1__TRecResScanPar) );
  
  err_retfail ( VRet, (ERR_OUT,"Saving record to file %s failed !", FileName ) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanParSaveBin ( char* FileName )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/11/2020
* \date    : Doc date  : 24/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanParSaveBin ( char* FileName ) {
  
  return ( MIS1__FRecResScanParSaveBin ( &(MIS1__VGPtRecResScan->P), FileName ) );
  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanLoadBin ( MIS1__TRecResScan* PtRec, char* FileName )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/10/2020
* \date    : Doc date  : 01/10/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanLoadBin ( MIS1__TRecResScan* PtRec, char* FileName ) {
  
  SInt32  VRet;


  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL" ) );
  
  
  // Read file
  
  VRet = FIL_FReadRecord ( FileName, PtRec, sizeof ( MIS1__TRecResScan)  );

  err_retfail ( VRet, (ERR_OUT,"Loading record from file %s failed !", FileName ) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanLoadBin ( char* FileName )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanLoadBin ( char* FileName ) {
  
  return ( MIS1__FRecResScanLoadBin ( MIS1__VGPtRecResScan, FileName) );
  
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanParLoadBin ( MIS1__TRecResScanPar* PtRec, char* FileName )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/11/2020
* \date    : Doc date  : 24/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanParLoadBin ( MIS1__TRecResScanPar* PtRec, char* FileName ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL" ) );
  
  
  // Read file
  
  VRet = FIL_FReadRecord ( FileName, PtRec, sizeof (MIS1__TRecResScanPar)  );
  
  err_retfail ( VRet, (ERR_OUT,"Loading record from file %s failed !", FileName ) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanParLoadBin ( char* FileName )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanParLoadBin ( char* FileName ) {
  
  return ( MIS1__FRecResScanParLoadBin ( &(MIS1__VGPtRecResScan->P), FileName) );
  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetDac ( MIS1__TRecResScan* PtRec, UInt32* ASrcDac, UInt32 SrcDacMaxESz  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetDac ( MIS1__TRecResScan* PtRec, UInt32* ASrcDac, UInt32 SrcDacMaxESz  ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );

  err_retnull ( ASrcDac, (ERR_OUT,"Abort => ASrcDac == NULL") );
  
  if ( SrcDacMaxESz < MIS1__DAC_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort source array size = %d < MIS1__DAC_NB = %d", SrcDacMaxESz, MIS1__DAC_NB ) );
  }
  
  // Set DAC
  
  memcpy ( PtRec->P.ParADac, ASrcDac, MIS1__DAC_NB * sizeof (PtRec->P.ParADac[0]) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetDac ( UInt32* ASrcDac, UInt32 SrcDacMaxESz  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetDac ( UInt32* ASrcDac, UInt32 SrcDacMaxESz  ) {
  
  return ( MIS1__FRecResScanSetDac ( MIS1__VGPtRecResScan, ASrcDac, SrcDacMaxESz  ) );
    
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetDac ( MIS1__TRecResScan* PtRec, UInt32* ADestDac, UInt32 DestDacMaxESz, UInt32 Print )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetDac ( MIS1__TRecResScan* PtRec, UInt32* ADestDac, UInt32 DestDacMaxESz, UInt32 Print  ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( ADestDac, (ERR_OUT,"Abort => ADestDac == NULL") );
  
  if ( DestDacMaxESz < MIS1__DAC_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort destination array size = %d < MIS1__DAC_NB = %d", DestDacMaxESz, MIS1__DAC_NB ) );
  }
  
  // Return DAC
  
  memcpy ( ADestDac, PtRec->P.ParADac, MIS1__DAC_NB * sizeof (PtRec->P.ParADac[0]) );
  
  // Print
  
  if ( Print ) {
    msg (( MSG_OUT, "DAC regsiters print is not yet implemented => TBD" ));
  }
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetDac ( UInt32* ADestDac, UInt32 DestDacMaxESz, UInt32 Print  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetDac ( UInt32* ADestDac, UInt32 DestDacMaxESz, UInt32 Print  ) {

  return ( MIS1__FRecResScanGetDac ( MIS1__VGPtRecResScan, ADestDac, DestDacMaxESz, Print  ) );
  
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetScCnfFile ( MIS1__TRecResScan* PtRec, char* SrcScCnfFile )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetScCnfFile ( MIS1__TRecResScan* PtRec, char* SrcScCnfFile ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( SrcScCnfFile, (ERR_OUT,"Abort => SrcScCnfFile == NULL") );
    
  // Set sc cnf file  
  
  strcpy ( PtRec->P.ParScConfFile, SrcScCnfFile );
    
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetScCnfFile ( char* SrcScCnfFile )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetScCnfFile ( char* SrcScCnfFile ) {
  
  return ( MIS1__FRecResScanSetScCnfFile ( MIS1__VGPtRecResScan, SrcScCnfFile ) );
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetScCnfFile ( MIS1__TRecResScan* PtRec, char* DestScCnfFile, UInt32 DestScCnfFileMaxSz, UInt32 Print )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetScCnfFile ( MIS1__TRecResScan* PtRec, char* DestScCnfFile, UInt32 DestScCnfFileMaxSz, UInt32 Print ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( DestScCnfFile, (ERR_OUT,"Abort => SrcScCnfFile == NULL") );
  
  if ( DestScCnfFileMaxSz < GLB_FILE_PATH_SZ ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Destination string size = %d < Max src size = %d", DestScCnfFileMaxSz, GLB_FILE_PATH_SZ) );
  }
  
  // Set sc cnf file
  
  strcpy ( DestScCnfFile, PtRec->P.ParScConfFile );
  
  // Print
  
  if ( Print ) {
    msg (( MSG_OUT, "SC conf file = %s", PtRec->P.ParScConfFile ));
  }
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetScCnfFile ( MIS1__TRecResScan* PtRec, char* DestScCnfFile, UInt32 DestScCnfFileMaxSz, UInt32 Print )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetScCnfFile (  char* DestScCnfFile, UInt32 DestScCnfFileMaxSz, UInt32 Print ) {
  
  return ( MIS1__FRecResScanGetScCnfFile ( MIS1__VGPtRecResScan, DestScCnfFile, DestScCnfFileMaxSz, Print ) );
  
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetScanReg ( MIS1__TRecResScan* PtRec, UInt32* ASrcScanReg, UInt32 SrcScanRegMaxESz  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetScanReg ( MIS1__TRecResScan* PtRec, UInt32* ASrcScanReg, UInt32 SrcScanRegMaxESz  ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( ASrcScanReg, (ERR_OUT,"Abort => ASrcScanReg == NULL") );
  
  if ( SrcScanRegMaxESz < MIS1__CAR_MAX_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort source array size = %d < MIS1__CAR_MAX_STEP_NB = %d", SrcScanRegMaxESz, MIS1__CAR_MAX_STEP_NB ) );
  }
  
  // Set scan reg
  
  memcpy ( PtRec->P.AParScanRegVal, ASrcScanReg, MIS1__CAR_MAX_STEP_NB * sizeof (PtRec->P.AParScanRegVal[0]) );
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetScanReg ( UInt32* ASrcScanReg, UInt32 SrcScanRegMaxESz  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetScanReg ( UInt32* ASrcScanReg, UInt32 SrcScanRegMaxESz  ) {
  
  return ( MIS1__FRecResScanSetScanReg ( MIS1__VGPtRecResScan, ASrcScanReg, SrcScanRegMaxESz  ) );
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetScanReg ( MIS1__TRecResScan* PtRec, UInt32* ADestScanReg, UInt32 DestScanRegMaxESz, UInt32 Print  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetScanReg ( MIS1__TRecResScan* PtRec, UInt32* ADestScanReg, UInt32 DestScanRegMaxESz, UInt32 Print ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( ADestScanReg, (ERR_OUT,"Abort => ADestScanReg == NULL") );
  
  if ( DestScanRegMaxESz < MIS1__CAR_MAX_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort destination array size = %d < MIS1__CAR_MAX_STEP_NB = %d", DestScanRegMaxESz, MIS1__CAR_MAX_STEP_NB ) );
  }
  
  // Return DAC
  
  memcpy ( ADestScanReg, PtRec->P.AParScanRegVal, MIS1__CAR_MAX_STEP_NB * sizeof (PtRec->P.AParScanRegVal[0]) );
    
    
  // Print

  if ( Print ) {
    
    VRet = MIS1__FRecResScanPrintScanRegVal ( PtRec, 1 /* All */ );
    
    err_retfail ( VRet, (ERR_OUT,"Abort => Print scan reg failed ! Ret = %d", VRet) );
  }
    
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetScanReg ( UInt32* ADestScanReg, UInt32 DestScanRegMaxESz, UInt32 Print  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetScanReg ( UInt32* ADestScanReg, UInt32 DestScanRegMaxESz, UInt32 Print  ) {
  
  return ( MIS1__FRecResScanGetScanReg ( MIS1__VGPtRecResScan, ADestScanReg, DestScanRegMaxESz, Print  ) );
  
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetStepFrNb ( MIS1__TRecResScan* PtRec, UInt32* PtFirstStep, UInt32* PtLastStep, UInt32* PtStepNb, UInt322* PtFrNb, UInt32 Print  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetStepFrNb ( MIS1__TRecResScan* PtRec, UInt32* PtFirstStep, UInt32* PtLastStep, UInt32* PtStepNb, UInt32* PtFrNb, UInt32 Print ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );

  err_retnull ( PtFirstStep, (ERR_OUT,"Abort => PtFirstStep == NULL") );
  err_retnull ( PtLastStep , (ERR_OUT,"Abort => PtLastStep  == NULL") );
  err_retnull ( PtStepNb   , (ERR_OUT,"Abort => PtStepNb    == NULL") );
  err_retnull ( PtFrNb     , (ERR_OUT,"Abort => PtFrNb      == NULL") );
  
  
  // Return steps info
  
  *PtFirstStep = PtRec->P.ParFirstStep;
  *PtLastStep  = PtRec->P.ParLastStep;
  *PtStepNb    = PtRec->P.ParStepNb;
  *PtFrNb      = PtRec->P.ParFrNb;
  
  if ( Print ) {
    msg (( MSG_OUT, "First step = %d, last step = %d, steps nb = %d, fr nb = %d", PtRec->P.ParFirstStep, PtRec->P.ParLastStep, PtRec->P.ParStepNb, PtRec->P.ParFrNb ));
  }
   
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetStepFrNb ( UInt32* PtFirstStep, UInt32* PtLastStep, UInt32* PtStepNb, UInt32* PtFrNb, UInt32 Print  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetStepFrNb ( UInt32* PtFirstStep, UInt32* PtLastStep, UInt32* PtStepNb, UInt32* PtFrNb, UInt32 Print  ) {
  
  return ( MIS1__FRecResScanGetStepFrNb ( MIS1__VGPtRecResScan, PtFirstStep, PtLastStep, PtStepNb, PtFrNb, Print ) );
  
}


// 9999



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetPulsePar ( MIS1__TRecResScan* PtRec, char* Mat, UInt32 FirstCol, UInt32 LastCol, UInt32 FirstRow, UInt32 LastRow, UInt32 PixPulseAA, UInt32 PixPulseAB  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 19/11/2020
* \date    : Doc date  : 19/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetPulsePar ( MIS1__TRecResScan* PtRec, char* Mat, UInt32 FirstCol, UInt32 LastCol, UInt32 FirstRow, UInt32 LastRow, UInt32 PixPulseAA, UInt32 PixPulseAB  ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  
  // Set pulse param
  
  PtRec->P.ParMatrixPulsed   = toupper (Mat[0]); // A, B, C, D
  
  PtRec->P.ParFirstColPulsed = FirstCol;
  PtRec->P.ParLastColPulsed  = LastCol;
  PtRec->P.ParFirstRowPulsed = FirstRow;
  PtRec->P.ParLastRowPulsed  = LastRow;
  
  PtRec->P.ParPixPulseAA     = PixPulseAA;
  PtRec->P.ParPixPulseAB     = PixPulseAB;
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanSetPulsePar ( char* Mat, UInt16 FirstCol, UInt32 LastCol, UInt32 FirstRow, UInt32 LastRow, UInt32 PixPulseAA, UInt32 PixPulseAB  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 19/11/2020
* \date    : Doc date  : 19/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanSetPulsePar ( char* Mat, UInt32 FirstCol, UInt32 LastCol, UInt32 FirstRow, UInt32 LastRow, UInt32 PixPulseAA, UInt32 PixPulseAB  ) {
  
  return ( MIS1__FRecResScanSetPulsePar (  MIS1__VGPtRecResScan, Mat, FirstCol, LastCol, FirstRow, LastRow, PixPulseAA, PixPulseAB  ) );
    
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetPulsePar ( MIS1__TRecResScan* PtRec, char* Mat, UInt32* PtFirstCol, UInt32* PtLastCol, UInt32* PtFirstRow, UInt32* PtLastRow, UInt32* PtPixPulseAA, UInt32* PtPixPulseAB, UInt32 Print )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetPulsePar ( MIS1__TRecResScan* PtRec, char* Mat, UInt32* PtFirstCol, UInt32* PtLastCol, UInt32* PtFirstRow, UInt32* PtLastRow, UInt32* PtPixPulseAA, UInt32* PtPixPulseAB, UInt32 Print  ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( PtFirstCol  , (ERR_OUT,"Abort => PtFirstCol   == NULL") );
  err_retnull ( PtLastCol   , (ERR_OUT,"Abort => PtLastCol    == NULL") );
  err_retnull ( PtFirstRow  , (ERR_OUT,"Abort => PtFirstRow   == NULL") );
  err_retnull ( PtLastRow   , (ERR_OUT,"Abort => PtLastRow    == NULL") );
  err_retnull ( PtPixPulseAA, (ERR_OUT,"Abort => PtPixPulseAA == NULL") );
  err_retnull ( PtPixPulseAB, (ERR_OUT,"Abort => PtPixPulseAB == NULL") );
  
  
  // Get pulse param
    
  Mat[0] = PtRec->P.ParMatrixPulsed;
  Mat[1] = 0;
  
  
  *PtFirstCol = PtRec->P.ParFirstColPulsed;
  *PtLastCol  = PtRec->P.ParLastColPulsed;
  *PtFirstRow = PtRec->P.ParFirstRowPulsed;
  *PtLastRow  = PtRec->P.ParLastRowPulsed;
  
  *PtPixPulseAA = PtRec->P.ParPixPulseAA;
  *PtPixPulseAB = PtRec->P.ParPixPulseAB;
  
  // Print
  
  if ( Print ) {
    msg (( MSG_OUT, "FirstCol = %d, LastCol = %d, FirstRow = %d, LasrRow = %d, PixPulseAA = %d, PixPulseAB = %d", *PtFirstCol, *PtLastCol, *PtFirstRow, *PtLastRow, *PtPixPulseAA, *PtPixPulseAB ));
  }
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetPulsePar ( char* Mat, UInt32* PtFirstCol, UInt32* PtLastCol, UInt32* PtFirstRow, UInt32* PtLastRow, UInt32* PtPixPulseAA, UInt32* PtPixPulseAB, UInt32 Print )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetPulsePar ( char* Mat, UInt32* PtFirstCol, UInt32* PtLastCol, UInt32* PtFirstRow, UInt32* PtLastRow, UInt32* PtPixPulseAA, UInt32* PtPixPulseAB, UInt32 Print  )  {
  
  return ( MIS1__FRecResScanGetPulsePar ( MIS1__VGPtRecResScan, Mat, PtFirstCol, PtLastCol, PtFirstRow, PtLastRow, PtPixPulseAA, PtPixPulseAB, Print  )  );
  
}






/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetPixelSteps ( MIS1__TRecResScan* PtRec, UInt32 PixX, UInt32 PixY, float* ADestPixSteps, UInt32 DestPixStepsMaxESz, UInt32 Print  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetPixelSteps ( MIS1__TRecResScan* PtRec, UInt32 PixX, UInt32 PixY, float* ADestPixSteps, UInt32 DestPixStepsMaxESz, UInt32 Print  ) {
  
  SInt32  VRet;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( ADestPixSteps, (ERR_OUT,"Abort => ADestPixSteps == NULL") );

  if ( PixX >= MIS1__COL_NB ) {
    err_retfail ( 1, (ERR_OUT,"Abort => PixX = %d > Max = %d", PixX, MIS1__COL_NB - 1) );
  }
  
  if ( PixY >= MIS1__ROW_NB ) {
    err_retfail ( 1, (ERR_OUT,"Abort => PixY = %d > Max = %d", PixY, MIS1__ROW_NB - 1) );
  }

  if ( DestPixStepsMaxESz < MIS1__CAR_MAX_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => DestPixStepsMaxESz = %d < MIS1__CAR_MAX_STEP_NB = %d", DestPixStepsMaxESz, MIS1__CAR_MAX_STEP_NB) );
  }

  
  // Return pixels steps %
  
  memcpy ( ADestPixSteps, &(PtRec->R.ResMat[PixX][PixY][PtRec->P.ParFirstStep]), PtRec->P.ParStepNb * sizeof (float) );
  
  
  if ( Print ) {
    MIS1__FRecResScanPrintPixSteps ( PtRec, PixX, PixY, 1 /* All */ );
  }
  
  
  // err_retok (( ERR_OUT, "" ));  // No err_retok (...) in order to save execution time
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanGetPixelSteps ( UInt32 PixX, UInt32 PixY, float* ADestPixSteps, UInt32 DestPixStepsMaxESz, UInt32 Print  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanGetPixelSteps ( UInt32 PixX, UInt32 PixY, float* ADestPixSteps, UInt32 DestPixStepsMaxESz, UInt32 Print  ) {
   
  return ( MIS1__FRecResScanGetPixelSteps ( MIS1__VGPtRecResScan, PixX, PixY, ADestPixSteps, DestPixStepsMaxESz, Print  ) );
   
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanPrintScanRegVal ( MIS1__TRecResScan* PtRec, UInt32 All  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanPrintScanRegVal ( MIS1__TRecResScan* PtRec, UInt32 All  ) {
  
  SInt32  VRet;
  SInt16  ViReg;
  SInt16  ViRegFirst;
  SInt16  ViRegLast;
  
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  if ( All > 1 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => All = %d <> 0, 1", All) );
  }
  
  // Print
  
  if ( All ) {
    ViRegFirst = 0;
    ViRegLast  = MIS1__CAR_MAX_STEP_NB - 1;
  }
  
  else {
    ViRegFirst = PtRec->P.ParFirstStep;
    ViRegLast  = PtRec->P.ParLastStep;
  }


  msg (( MSG_OUT, "==========================================================" ));
  msg (( MSG_OUT, " Scan reg step values" ));
  
  if ( All ) {
    msg (( MSG_OUT, " - Print ALL" ))
  }
  
  else {      
    msg (( MSG_OUT, " - Print only first to last steps" ));
  }
  
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, " First step = %d, last step = %d, step nb = %d", PtRec->P.ParFirstStep, PtRec->P.ParLastStep, PtRec->P.ParStepNb ));
  msg (( MSG_OUT, "==========================================================" ));
  
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "" ));
  
  for ( ViReg = ViRegFirst; ViReg <= ViRegLast; ViReg++ ) {
    msg (( MSG_OUT, "Scan reg [Step = %.2d] = %.3d", ViReg, PtRec->P.AParScanRegVal[ViReg] ));
  }
 
  err_retok (( ERR_OUT, "" ));  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanPrintScanRegVal ( UInt32 All  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanPrintScanRegVal ( UInt32 All  ) {
  
  return ( MIS1__FRecResScanPrintScanRegVal ( MIS1__VGPtRecResScan, All  ) );
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanPrintPixSteps ( MIS1__TRecResScan* PtRec, UInt32 PixX, UInt32 PixY, UInt32 All  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanPrintPixSteps ( MIS1__TRecResScan* PtRec, UInt32 PixX, UInt32 PixY, UInt32 All  ) {
  
  SInt32  VRet;
  SInt16  ViStep;
  SInt16  ViStepFirst;
  SInt16  ViStepLast;
  
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  if ( All > 1 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => All = %d <> 0, 1", All) );
  }
  
  if ( PixX >= MIS1__COL_NB ) {
    err_retfail ( 1, (ERR_OUT,"Abort => PixX = %d > Max = %d", PixX, MIS1__COL_NB - 1) );
  }
  
  if ( PixY >= MIS1__ROW_NB ) {
    err_retfail ( 1, (ERR_OUT,"Abort => PixY = %d > Max = %d", PixY, MIS1__ROW_NB - 1) );
  }
    
    
  
  // Print
  
  if ( All ) {
    ViStepFirst = 0;
    ViStepLast  = MIS1__CAR_MAX_STEP_NB - 1;
  }
  
  else {
    ViStepFirst = PtRec->P.ParFirstStep;
    ViStepLast  = PtRec->P.ParLastStep;
  }
  
  
  msg (( MSG_OUT, "==========================================================" ));
  msg (( MSG_OUT, " Pixel X = %.4d, Y = %.4d steps %% results", PixX, PixY ));
  
  if ( All ) {
    msg (( MSG_OUT, " - Print ALL" ))
  }
  
  else {
    msg (( MSG_OUT, " - Print only first to last steps" ));
  }
    
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, " First step = %d, last step = %d, step nb = %d", PtRec->P.ParFirstStep, PtRec->P.ParLastStep, PtRec->P.ParStepNb ));
  msg (( MSG_OUT, "==========================================================" ));
  
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "" ));
  
  for ( ViStep = ViStepFirst; ViStep <= ViStepLast; ViStep++ ) {
    msg (( MSG_OUT, "Pix [X = %.4d, Y = %.4d, Step = %.2d] = %.1f %%", PixX, PixY, ViStep, PtRec->R.ResMat[PixX][PixY][ViStep] ));
  }
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanPrintPixSteps ( UInt32 PixX, UInt32 PixY, UInt32 All  )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/11/2020
* \date    : Doc date  : 18/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanPrintPixSteps ( UInt32 PixX, UInt32 PixY, UInt32 All  ) {
  
  return ( MIS1__FRecResScanPrintPixSteps ( MIS1__VGPtRecResScan, PixX, PixY, All  ));
    
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanBuild ( MIS1__TRecResScan* PtRec, char* RunRootDir, UInt32 RunNo, UInt32 FirstStep, UInt32 LastStep, UInt32 FrNb, UInt32 FileFormatU16S32 )
*          :
* \brief   : Save scan record in a binary file
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : FileName   - The file          :
*          :
* \return  : Error code
*          : >=  0 - Number of pixels in the threshold range
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/11/2020
* \date    : Doc date  : 17/11/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanBuild ( MIS1__TRecResScan* PtRec, char* RunRootDir, UInt32 RunNo, UInt32 FileFormatU16S32, UInt32 FirstStep, UInt32 LastStep, UInt32* AScanRegVal, UInt32 ScanRegValESz, UInt32 FrNb  ) {
  
  SInt32  VRet;
  SInt16  ViStep;
  char    VStepSrcFile[GLB_FILE_PATH_SZ];
  SInt16  VPixX;
  SInt16  VPixY;
  float   VPixPCent;
  
  MIS1__TMatPixS32*    VPtMatStepS32;
  MIS1__TMatPixCntU16* VPtMatStepU16;
  MIS1__TMatPixFloat*  VPtMatPCent;


  err_error (( ERR_OUT, "PtRec = %X, RunRootDir = %s, RunNo = %d, FileFormatU16S32 = %d, FirstStep = %d, LastStep = %d, AScanRegVal = %X, ScanRegValESz = %d, FrNb = %d", PtRec, RunRootDir, RunNo, FileFormatU16S32, FirstStep, LastStep, AScanRegVal, ScanRegValESz, FrNb ));
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( RunRootDir, (ERR_OUT,"Abort => RunRootDir == NULL" ) );
  
  if ( LastStep < FirstStep ) {
    err_retfail ( -1, (ERR_OUT,"Abort : LastStep = %d < FirstStep = %d", LastStep, FirstStep ) );
  }
  
  if ( (LastStep - FirstStep + 1) > MIS1__CAR_MAX_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Total step nb = %d > Max = %d", LastStep - FirstStep + 1, MIS1__CAR_MAX_STEP_NB ) );
  }
  
  
  if ( LastStep >= ScanRegValESz ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Scan reg array too small = %d items > last step index = %d ", ScanRegValESz, LastStep ) );
  }
  
  if ( (FileFormatU16S32 < 0) || (FileFormatU16S32 > 1 ) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Bad file format = %d <> 0 = U16, 1 = S32", FileFormatU16S32) );
  }
  
    
  // Check if run exists
  
  if ( ! DirectoryExists ( RunRootDir ) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Run directory = %s does not exists", RunRootDir ) );
  }
  
  // Set param to record
    
  PtRec->P.ParFirstStep = FirstStep;
  PtRec->P.ParLastStep  = LastStep;
  PtRec->P.ParStepNb    = LastStep - FirstStep + 1;
  
  PtRec->P.ParFrNb      = FrNb;
  
  
  if ( AScanRegVal != NULL ) {
  
    for ( ViStep = 0; ViStep < MIS1__CAR_MAX_STEP_NB; ViStep++ ) {
      PtRec->P.AParScanRegVal[ViStep] = 0;
    }
    
    for ( ViStep = PtRec->P.ParFirstStep; ViStep <= PtRec->P.ParLastStep; ViStep++ ) {
      PtRec->P.AParScanRegVal[ViStep] = AScanRegVal[ViStep];
    }
    
  } // End if ( AScanRegVal != NULL )
  
  
  // Alloc buffers
  
  if ( FileFormatU16S32 == 0 ) {
    
    VPtMatStepU16 = (MIS1__TMatPixCntU16*) malloc ( sizeof (MIS1__TMatPixCntU16) );
    
    err_retnull ( VPtMatStepU16, (ERR_OUT,"Abort => Alloc step file buffer U16 failed") );
    
  }
  
  VPtMatStepS32 = (MIS1__TMatPixS32*) malloc ( sizeof (MIS1__TMatPixS32) );

  err_retnull ( VPtMatStepS32, (ERR_OUT,"Abort => Alloc step file buffer S32 failed") );
     
  VPtMatPCent = (MIS1__TMatPixFloat*) malloc ( sizeof (MIS1__TMatPixFloat) );
  
  err_retnull ( VPtMatPCent, (ERR_OUT,"Abort => Alloc step mat pcent failed") );
      
    
    
  // Reset results
  
  MIS1__FRecResScanResetRes ( PtRec );
  
  
  
  // Process the run files
  
  for ( ViStep = FirstStep; ViStep <= LastStep; ViStep++ ) {
  
    // Build step source file name
    
    sprintf ( VStepSrcFile, "%s/run_%d/data/run_%d_%d_step.bin", RunRootDir, RunNo, RunNo, ViStep );
      
    msg (( MSG_OUT, "File = %s", VStepSrcFile ));
      
    // Load src step file

    if ( FileFormatU16S32 == 0 ) {
      
      // Load U16 files
      
      VRet = MIS1__FMatPixCntU16Load ( VPtMatStepU16, VStepSrcFile );
      
      err_retfail ( VRet, (ERR_OUT, "Error loading cumul matrix U16 from %s file - May be no file or bad format U16/S32 ?", VStepSrcFile ) );
      
      // Convert U16 to S32 file
      
      VRet = MIS1__FMatPixCntU16ToMatPixS32 ( VPtMatStepU16, VPtMatStepS32 );
      
      err_retfail ( VRet, (ERR_OUT, "Abort => Matrix U16 to S32 conversion" ) );
      
    } // End if ( FileFormatU16S32 == 0 )
    
    // Load a S32 file
    
    else {
      
      VRet =  MIS1__FMatPixS32Load ( VPtMatStepS32, VStepSrcFile );
      
      err_retfail ( VRet, (ERR_OUT, "Error loading cumul matrix S32 from %s file - May be no file or bad format U16/S32 ?", VStepSrcFile) );
      
    } // End else
    
if (1) {

    // Calc ResMat %
    
    VRet = MIS1_FMatCumToMatPCent ( VPtMatStepS32, VPtMatPCent, FrNb );
      
    err_retfail ( VRet, (ERR_OUT, "Abort => Calculate APP__VGPtMatPCent failed !" ) );
            
        
    for ( VPixX = 0; VPixX < MIS1__COL_NB; VPixX++ ) {
      
      for ( VPixY = 0; VPixY < MIS1__ROW_NB; VPixY++ ) {
        
        VPixPCent = (*VPtMatPCent)[VPixX][VPixY];
        
        PtRec->R.ResMat[VPixX][VPixY][ViStep] = VPixPCent;
        
      }  // End for ( VPixY )
      
    } // End for ( VPixX )
    

  } // End for ( ViStep )
  
  
}
  
  
  // Free buffers
  
  
  if ( FileFormatU16S32 == 0 ) {    
    free ( VPtMatStepU16 );    
  }
  
  free ( VPtMatStepS32 );
  
  free ( VPtMatPCent );

  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecResScanExtractPixel ( MIS1__TRecResScan* PtRec, UInt16 PixX, UInt16 PixY, UInt16* AScanPar, SInt16 ScanParNb, UInt8 PrintDest, UInt8 PrintFormat, TMemo* Memo, char* LogFileName, char* CsvFileName, char* PlotFileNameX, char* PlotFileNameY  )           :
*          :
* \brief   : Extract one pixel and save it as text files
*          :
* \param   : PtRec      - Pointer on Mimosis 1 scan record
*          :
* \param   : PixX       - Pixel X
*          :
* \param   : PixY       - Pixel Y
*          :
* \param   : AScanPar   - Array which contains scan parameter value for each step
*          :
* \param   : ScanParNb  - Items nb = steps nb in AScanPar
*          :
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \param   : PrintFormat - Print format ... for future use
*          :
* \param   : Memo           - Pointer to destination Memo if PrintDest = 2 or 3
*          :
* \param   : LogFileName    - Pointer to destination text  log file name if PrintDest = 4 or 5
*          :
* \param   : CsvFileName    - Pointer to destination text  CSV file name if PrintDest = 4 or 5
*          :
* \param   : PlotFileNameX  - Pointer to destination text X data file for S curve plot 
*          :
* \param   : PlotFileNameY  - Pointer to destination text U data file for S curve plot
*          :
* \return  : Error code
*          : >  0 - Error, returns nb of times the pixel is out of range 0 .. 100 %
*          : =  0 - ok
*          : < 0  - SW Error 
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 30/09/2020
* \date    : Doc date  : 30/09/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRecResScanExtractPixel ( MIS1__TRecResScan* PtRec, UInt16 PixX, UInt16 PixY, UInt16* AScanPar, SInt16 ScanParNb, UInt8 PrintDest, UInt8 PrintFormat, TMemo* Memo, char* LogFileName, char* CsvFileName, char* PlotFileNameX, char* PlotFileNameY  ) {
  
  SInt32  VRet;  
  
  FILE*  VPtFileLog;
  FILE*  VPtFileCSV;
  FILE*  VPtFilePlotX;
  FILE*  VPtFilePlotY;
  
  SInt32 ViStep;
  float  VPixPCent;
  
  char   VStrPixXYPCent[GLB_CMT_SZ];
  
  SInt32 VPixBadPCentCnt;
  
  
  // Check param
  
  err_retnull ( PtRec, (ERR_OUT,"Abort => PtRec == NULL") );
  
  err_retnull ( AScanPar, (ERR_OUT,"Abort => AScanPar == NULL") );
  
  if ( ScanParNb > MIS1__CAR_MAX_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ScanParNb = %d > MIS1__CAR_MAX_STEP_NB = %d", ScanParNb, MIS1__CAR_MAX_STEP_NB ) );
  }
  
  if ( ScanParNb != PtRec->P.ParStepNb ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ScanParNb = %d != Scan par nb for scan record = %d", ScanParNb, PtRec->P.ParStepNb ) );
  }
  
  if ( PixX >= MIS1__COL_NB )  {
    err_retfail ( -1, (ERR_OUT,"Abort => Bad column = %d MUST be in 0 to %d", PixX, MIS1__COL_NB - 1 ) );
  }
  
  
  if ( PixY >= MIS1__ROW_NB  ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Bad line = %d MUST be in 0 to %d", PixY, MIS1__ROW_NB - 1 ) );
  }
  
  
    
  if ( PrintDest > 5 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PrintDest=%d > 5", PrintDest) );
  }
  
  
  if ( ((PrintDest == 2) || (PrintDest == 3)) && (Memo == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to memo requested BUT Memo = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (LogFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to log text file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (CsvFileName == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to CSV file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (PlotFileNameX == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to plot X file requested BUT FileName = NULL !") );
  }
  
  if ( ((PrintDest == 4) || (PrintDest == 5)) && (PlotFileNameY == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to plot Y file requested BUT FileName = NULL !") );
  }
  
  
  // Create text file if PrintDest = 1, 4 or 5
  
  if ( (PrintDest == 1) ||(PrintDest == 4) || (PrintDest == 5) ) {
    
    VPtFileLog = fopen ( LogFileName, "wt" );
    err_retnull ( VPtFileLog, (ERR_OUT,"Abort => Create file %s failed !", LogFileName) );
    
    VPtFileCSV = fopen ( CsvFileName, "wt" );
    err_retnull ( VPtFileCSV, (ERR_OUT,"Abort => Create file %s failed !", CsvFileName) );
    
    
    VPtFilePlotX = fopen ( PlotFileNameX, "wt" );
    err_retnull ( VPtFilePlotX, (ERR_OUT,"Abort => Create file %s failed !", PlotFileNameX) );
    

    VPtFilePlotY = fopen ( PlotFileNameY, "wt" );
    err_retnull ( VPtFilePlotY, (ERR_OUT,"Abort => Create file %s failed !", PlotFileNameY) );

  }
  
  
  // Extracts pixel info & Prints
  
  VPixBadPCentCnt = 0;
  

  for ( ViStep = 0; ViStep < PtRec->P.ParStepNb; ViStep++ ) {
    
    // typedef float MIS1__TTMatScanFloat[MIS1__COL_NB][MIS1__ROW_NB][MIS1__CAR_MAX_STEP_NB];
    
    VPixPCent = PtRec->R.ResMat[PixX][PixY][ViStep];
      
    // Check
  
    if ( (VPixPCent < 0) || (VPixPCent > 100) ) {
      ++VPixBadPCentCnt;
      err_warning (( ERR_OUT, "Pix %% = %.1f not in range 0..100%% : Step = %.2d, ScanPar = %.3d, x = %.3d, y = %.3d ", VPixPCent, ViStep, AScanPar[ViStep], PixX, PixY ));  
    }
      

    if ( PrintDest != 0) {
      
      // Prints
    
      sprintf ( VStrPixXYPCent, "Pixel[x=%.4d,y=%.4d,step=%.4d] = %.1f", PixX, PixY, ViStep, VPixPCent  );
    
      MIS1__FPrintStr ( VStrPixXYPCent, PrintDest, Memo, VPtFileLog );
    
   
      // Print to CSV file
    
      fprintf ( VPtFileCSV, "%d,%d,%.1f \n",  PixX, PixY, VPixPCent   );
    
      // Print to plot files
    
      fprintf ( VPtFilePlotX, "%d \n", AScanPar[ViStep] );
    
      fprintf ( VPtFilePlotY, "%.1f \n", VPixPCent );
    
    } // End if ( PrintDest != 0)
    
    
  }

  
  
  if ( (PrintDest == 1) || (PrintDest == 4) || (PrintDest == 5) ) {
    
    if ( fclose ( VPtFileLog ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", LogFileName) );
    }
    
    if ( fclose ( VPtFileCSV ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", CsvFileName) );
    }
    
    
    if ( fclose ( VPtFilePlotX ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", PlotFileNameX) );
    }
    
    if ( fclose ( VPtFilePlotY ) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Abort => fclose file %s failed !", PlotFileNameY) );
    }
    
    
  }
  
 
  return (VPixBadPCentCnt);
}



  

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32ToMatPixXYSP ( MIS1__TMatPixS32* PtMatSrc, MIS1__TMatPixXYSP* PtMatDest, UInt8 EncodeMth )
*          :
* \brief   : Convert a matrix view as MIS1__TMatPixS32 to MIS1__TMatPixXYSP view
*          :
* \param   : PtMatSrc  - Pointer on source matrix MIS1__TMatPixS32
*          :
* \param   : PtMatDest  - Pointer on destination matrix MIS1__TMatPixXYSP
*          :
* \param   : EncodeMth  - Encode method (Only 0 is handled on 29/05/2019)
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 29/05/2019
* \date    : Doc date  : 29/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FMatPixS32ToMatPixXYSP ( MIS1__TMatPixS32* PtMatSrc, MIS1__TMatPixXYSP* PtMatDest, UInt8 EncodeMth ) {
  
  SInt32           VRet;
  SInt16           VSrcIRow;
  SInt16           VSrcICol;
  SInt16           VDestIRow;
  SInt16           VDestICol;
  SInt32           VPixS32Val;
  MIS1__TPixXYSP*  VPtPixXYSP;
  
  SInt16           VDestCol;
  SInt16           VDestRow;
  SInt16           VDestRegion;
  SInt16           VDestSRegion;
  SInt16           VDestPeNoInMatrix;
  SInt16           VDestPeNoInRegion;
  SInt16           VDestPeNoInSRegion;
  SInt16           VDestPeRow;
  SInt16           VDestPeAddr;
  
  // Check param
  
  err_retnull ( PtMatSrc, (ERR_OUT,"Abort => PtMatSrc == NULL") );

  err_retnull ( PtMatDest, (ERR_OUT,"Abort => PtMatDest == NULL") );
  
  if ( EncodeMth != 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => EncodeMth = %d <> 0", EncodeMth ) );
  }
  
  // Convert matrix
  
  for ( VSrcIRow=0; VSrcIRow < MIS1__ROW_NB; VSrcIRow++ ) {
    
    for ( VSrcICol=0; VSrcICol < MIS1__COL_NB; VSrcICol++ ) {
      
      switch ( EncodeMth ) {
        
        case 0 : {
          VDestICol          = VSrcICol;
          VDestIRow          = VSrcIRow;
          
          VPixS32Val         = (*PtMatSrc)[VSrcICol][VSrcIRow];
          VDestCol           = VSrcICol;
          VDestRow           = VSrcIRow;          
          VPtPixXYSP         = &((*PtMatDest)[VDestICol][VDestIRow]);
          VDestRegion        = VDestCol / MIS1__COL_PER_REG;
          VDestSRegion       = VDestCol / MIS1__COL_PER_SREG;
          
          VDestPeNoInMatrix  = VDestCol / MIS1__COL_PER_PE;
          VDestPeNoInRegion  = VDestCol % MIS1__PE_PER_REG;
          VDestPeNoInSRegion = VDestCol % MIS1__PE_PER_SREG;
          VDestPeRow         = VSrcIRow;
          VDestPeAddr        = 0;
            
          break; }
        
        // No default case because EncodeMth is checked at function beginning
        
      }
      
      VPtPixXYSP->PixXYS.PixState  = VPixS32Val;
      VPtPixXYSP->PixXYS.PixXY.C.x = VDestCol;
      VPtPixXYSP->PixXYS.PixXY.C.y = VDestRow;
      VPtPixXYSP->Region           = VDestRegion;
      VPtPixXYSP->SRegion          = VDestSRegion;
      
      VPtPixXYSP->PeNoInMatrix     = VDestPeNoInMatrix;
      VPtPixXYSP->PeNoInRegion     = VDestPeNoInRegion;
      VPtPixXYSP->PeNoInSRegion    = VDestPeNoInSRegion;
      VPtPixXYSP->PeRow            = VDestPeRow;
      VPtPixXYSP->PeAddr           = VDestPeAddr;
      
    } // End for ( VSrcICol )
    
  } // End for ( VSrcIRow )
  
    
  return (0);
  
}

#ifdef CPY_TYPES_

typedef struct {
  
  MIS1__TPixXYS PixXYS; /*!< Pixel coordinates + state MIS1__TPixXYS */
  
  SInt16 PeNoInMatrix;   /*!< Priority encoder No in matrix       => 0..MIS1__PE_NB-1       = 511  */
  SInt16 PeNoInRegion;   /*!< Priority encoder No in region       => 0..MIS1__PE_PER_REG-1  = 7    */
  SInt16 PeNoInSRegion;  /*!< Priority encoder No in super region => 0..MIS1__PE_PER_SREG-1 = 31   */
  SInt16 PeRow;          /*!< Priority encoder row                => 0..MIS1__ROW_NB-1      = 503  */
  
} MIS1__TPixXYSP;


typedef struct {
  
  MIS1__TPixXY PixXY; /*!< Pixel coordinates MIS1__TPixXY */
  
  SInt8 PixState;        /*!< Pixel state 0 / 1 */
  
} MIS1__TPixXYS;


typedef union {
  
  SInt16 Axy[2]; /*!< Pixel coordinates array [0] = x, [1] = y */
  
  struct {
    SInt16 x; /*!< Pixel coordinates structure C.x, C.y */
    SInt16 y;
  } C;
  
} MIS1__TPixXY;

#endif



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPeResetSetAll ( MIS1__TPe* PtPe, SInt8 ResetSetAll )
*          :
* \brief   : Clears or fireds all pixels of a PE, for tests purpose
*          :
* \param   : PtPe         - Pointer on PE
*          :
* \param   : ClearFireAll - 0 => Clears all pixels, 1 = Fires all pixels
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 14/05/2019
* \date    : Doc date  : 14/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPeResetSetAll ( MIS1__TPe* PtPe, SInt8 ResetSetAll ) {
  
  SInt32 VRet;
  SInt16 ViRow;

  // Check param
  
  err_retnull ( PtPe, (ERR_OUT,"Abort => PtPe == NULL") );
  
  // Clears / Fires
  
  for ( ViRow=0; ViRow < MIS1__ROW_NB; ViRow++ ) {
    
    if ( ResetSetAll == 0 ) {
      PtPe->AColLeft[ViRow]  = 0;
      PtPe->AColRight[ViRow] = 0;
    }
    
    else {
      PtPe->AColLeft[ViRow]  = 1;
      PtPe->AColRight[ViRow] = 1;      
    }
    
  }

  return (0);
}
  


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPeResetSetPix ( MIS1__TPe* PtPe, SInt8 PixState, UInt16 PixX, UInt16 PixY )
*          :
* \brief   : Clears or fireds all pixels of a PE, for tests purpose
*          :
* \param   : PtPe     - Pointer on PE
*          :
* \param   : PixState - Pixel state 0 / 1
*
* \param   : PixX     - Pixel X coordinate = PE column = 0 (left) / 1 (right)
*          :
* \param   : PixY     - Pixel Y coordinate = row
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 14/05/2019
* \date    : Doc date  : 14/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPeResetSetPix ( MIS1__TPe* PtPe, SInt8 PixState, UInt16 PixX, UInt16 PixY ) {
  
  SInt32 VRet;
  SInt16 ViRow;
  
  // Check param
  
  err_retnull ( PtPe, (ERR_OUT,"Abort => PtPe == NULL") );
  
  if ( PixX > 1 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PixX = %d > 1", PixX ) );
  }
  
  if ( PixY >= MIS1__ROW_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => PixY = %d > %d", PixY, MIS1__ROW_NB - 1 ) );
  }
 
  // Clears / Fires pixel
  
  if ( PixX == 0 ) {
    PtPe->AColLeft[PixY]  = PixState;
  }
  
  else {
    PtPe->AColRight[PixY] = PixState;    
  }
  
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPePrint ( MIS1__TPe* PtPe, SInt16 PeReg, SInt8 PeNoInReg, UInt8 EncodeMth, UInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*          :
* \brief   : Prints PE states of MIS1__TPe structure
*          :
* \param   : PtPe      - Pointer on PE
*          :             
* \param   : PeReg     - Region of PE (0..63)
*          :
* \param   : PeNoInReg - No of PE in region (0..7)
*          :
* \param   : EncodeMth - Encoding method (for future use)
*          :
* \param   : PrintDest  - Print to ... 0 = No print, 1 = Msg file, 2 = Memo, 3 = Msg file + Memo
*          :
* \param   : Memo        - Pointer to destination Memo if PrintDest = 2 or 3
*          :
* \param   : PtFile      - Pointer to destination text file if PrintDest = 4 or 5
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 14/05/2019
* \date    : Doc date  : 14/05/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPePrint ( MIS1__TPe* PtPe, SInt16 PeReg, SInt8 PeNoInReg, UInt8 EncodeMth, UInt8 PrintMth, TMemo* Memo, FILE* PtFile ) {
  
  SInt32  VRet;
  SInt16  ViRow;
  SInt16  VPixRow;
  SInt16  VPixColLeft;
  SInt16  VPixColRight;
  SInt16  VPixAddr;
  SInt8   VPixColLeftState;
  SInt8   VPixColRightState;
  char    VMsg[GLB_CMT_SZ];
  
  
  // Check param
  
  err_retnull ( PtPe, (ERR_OUT,"Abort => PtPe == NULL") );
  
  if ( (EncodeMth >= 2) && (Memo == NULL) ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Print to memo requested BUT Memo = NULL !") );
  }


  // Encodes PE, Pixels info
    
  
  for ( ViRow=0; ViRow < MIS1__ROW_NB; ViRow++ ) {
        
    switch ( EncodeMth ) {
      
      case 0 : {
        VPixRow      = ViRow;
        VPixColLeft  = (PeReg * MIS1__COL_PER_REG) + (PeNoInReg * 2);
        VPixColRight = VPixColLeft + 1;
      break; }
      
      default : {
        err_retfail ( -1, (ERR_OUT,"Abort => Unknown EncodeMth = %d", EncodeMth) );
        break; }
    }
    
    // Converts PE, Pixelsinfo to string
    
    if ( (PtPe->AColLeft[ViRow] == 1) || (PtPe->AColRight[ViRow] == 1) ) {
      VPixColLeftState = PtPe->AColLeft[ViRow];
      VPixColRightState = PtPe->AColRight[ViRow];
      sprintf ( VMsg, "Region %.2d PE[%.1d,%.3d] = %d, %d = Pix [%.4d,%.3d]=%d, [%.4d,%.3d]=%d", PeReg, PeNoInReg, ViRow, VPixColLeftState, VPixColRightState, VPixColLeft, VPixRow, VPixColLeftState, VPixColRight, VPixRow, VPixColRightState );
    }
    
    // Prints
    
    MIS1__FPrintStr ( VMsg, PrintMth, Memo, PtFile );
  }
  
  return (0);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPeConvRowCol2Addr ( UInt16 PeRow, UInt16 PeRegionCol, UInt8 Mode, UInt8 EncodeMth )
*          :
* \brief   : Converts priority encoder Row, Col (in region) to address
*          :
* \param   : PeRow       - PE row => 0..MIS1__ROW_NB-1 => 0..503
*          :
* \param   : PeRegionCol - PE column in region => 0..MIS1__COL_PER_REG-1 => 0..15
*
* \param   : Mode        - 0 = returns PE address corresponding to PeRow, PeRegionCol
*          :             - 1 = Initialize the lookup table
*          :
* \param   : EncodeMth   - Encoding method, only 0 is used now
*          :
* \return  : Address or Error code
*          : >=  0 - Address
*          :  < 0  - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Before beeing used to convert (Mode = 0), it must be called one time with param Mode = 1
* \warning : Level     :
*          :
* \warning : Item not filled now :
* \todo    : Code is not finished (in fact not written) => Functions always returns -1
*          :
*  bug     :
*          :
* \date    : Date      : 04/06/2019
* \date    : Doc date  : 04/06/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPeConvRowCol2Addr ( UInt16 PeRow, UInt16 PeRegionCol, UInt8 Mode, UInt8 EncodeMth ) {
  
  static SInt16 VAPeAdd[MIS1__PE_NB][MIS1__COL_PER_PE];
  UInt8 VPeCol;

  // ====================================================================
  // WARNING => Code not finsihed !!!!!!!!!!!!!!!!!
  // ====================================================================

  err_retfail ( -1, (ERR_OUT,"Code is not finished (in fact not written) => Functions always returns -1" ) );

  // VPeCol = PeRegionCol %  // Code not finished on 02/07/2019

  // Convert

  if ( Mode == 0 ) {
    // return ( VAPeAdd);
  }

}
  


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPrintRawAcqFrMapTable ( MIS1__TAcqFrMap* SrcPtFrMap, UInt32 AcqId, SInt16 FirstFr, SInt16 LastFr, SInt8 PrintDecHex )
*          :
* \brief   : Prints RAW frames map table info (fields data offset) of one acq for debug purpose
*
* \param   : SrcPtFrMap   - Pointer to the map table
*          :
* \param   : AcqId        - Id of the acquisition, only for printing, not used to find info
*          :
* \param   : FirstFr      - First frame to print
*          :
* \param   : LastFr       - Last frame to print,                                           \n
*                           set LastFr = -1 to display all frames starting at FirstFr \n
*                           set LastFr = FirstFr = -1 to disable frames list printing
*          :
* \param   : PrintDecHex  - 0 => Print in decimal, 1 => Print in hex
*          :
* \return  : Error code
*          : < 0 - Error
*          :   0 - Not frames in this Acq => ???
*          : > 0 - Frames number in this Acq
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 09/04/2020
* \date    : Doc date  : 09/04/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPrintRawAcqFrMapTable ( MIS1__TAcqFrMap* SrcPtFrMap, UInt32 AcqId, SInt16 FirstFr, SInt16 LastFr, SInt8 PrintDecHex ) {
  
  UInt16 ViFr;
  UInt16 VTotFrNb;
  
  // Check param
  
  err_retnull ( SrcPtFrMap, (ERR_OUT,"Abort => SrcPtFrMap == NULL") );
  
  VTotFrNb = SrcPtFrMap->ResFrNb;
  
  if ( VTotFrNb == 0 ) {
    msg (( MSG_OUT, "============================" ));
    msg (( MSG_OUT, " No RAW frames in Acq No %d ", AcqId ));
    msg (( MSG_OUT, "============================" ));
    return (0);
  }
  
  if ( FirstFr >= VTotFrNb ) {
    err_retfail ( -1, (ERR_OUT,"Abort => FirstFr = %d > LastFr in table = %d", FirstFr, VTotFrNb - 1 ) );
  }
  
  if ( LastFr >= VTotFrNb ) {
    err_retfail ( -1, (ERR_OUT,"Abort => LastFr = %d > LastFr in table = %d", LastFr, VTotFrNb - 1 ) );
  }
  
  // User wants to display all frames
  
  if ( LastFr == -1 ) {
    LastFr = VTotFrNb - 1;
  }
    
  // msg (( MSG_OUT, "Frames map : Par Acq No %.4d, Rec Acq No %d, Rec FrNb %d, Rec Acq sz %d W8, Acq pos %d W8", AcqId, SrcPtFrMap->ResAcqId, SrcPtFrMap->ResAcqSz, SrcPtFrMap->ResFrNb, SrcPtFrMap->ResAcqPos ));
  
  msg (( MSG_OUT, "=================================" ));
  msg (( MSG_OUT, "RAW Frames map : Par Acq No %.4d, Rec Acq No %d, Rec FrNb %d", AcqId, SrcPtFrMap->ResAcqId, SrcPtFrMap->ResFrNb ));
  msg (( MSG_OUT, "RAW Frames map : Rec Acq sz %d W8, Acq pos %d W8", SrcPtFrMap->ResAcqSz, SrcPtFrMap->ResAcqPos ));
  msg (( MSG_OUT, "=================================" ));
  msg (( MSG_OUT, "" ));
  
  
  // User wants to display only acq info not fames list
  
  if ( (FirstFr == -1) && (LastFr == -1) ) {
    return ( VTotFrNb );
  }
  
  
  // Print offsets in decimal
  
  if ( PrintDecHex == 0 ) {
    
    for ( ViFr = FirstFr; ViFr <= LastFr; ViFr++ ) {
      msg (( MSG_OUT, "==================================" ));
      msg (( MSG_OUT, "RAW  Fr No %.4d", ViFr  ));
      msg (( MSG_OUT, "----------------------------------" ));
      msg (( MSG_OUT, "RAW Header  = %.6d", SrcPtFrMap->ResAPosFrHead[ViFr]  ));
      msg (( MSG_OUT, "RAW  Data    = %.6d", SrcPtFrMap->ResAPosFrData[ViFr]  ));
      msg (( MSG_OUT, "RAW Trailer = %.6d", SrcPtFrMap->ResAPosFrTrail[ViFr] ));
      msg (( MSG_OUT, "==================================" ));
    }
    
  }
  
  // Print offsets in hexadecimal
  
  else {
    
    for ( ViFr = FirstFr; ViFr <= LastFr; ViFr++ ) {
      msg (( MSG_OUT, "==================================" ));
      msg (( MSG_OUT, "RAW Fr No %.4d", ViFr  ));
      msg (( MSG_OUT, "----------------------------------" ));
      msg (( MSG_OUT, "RAW Header  = %.6X", SrcPtFrMap->ResAPosFrHead[ViFr]  ));
      msg (( MSG_OUT, "RAW Data    = %.6X", SrcPtFrMap->ResAPosFrData[ViFr]  ));
      msg (( MSG_OUT, "RAW Trailer = %.6X", SrcPtFrMap->ResAPosFrTrail[ViFr] ));
      msg (( MSG_OUT, "==================================" ));
    }
    
    
  }
  
  return ( VTotFrNb );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPrintDecAcqFrMapTable ( MIS1__TDecFrMap* SrcPtFrMap, UInt32 AcqId, SInt16 FirstFr, SInt16 LastFr, SInt16 FrNbInAcq )
*          :
* \brief   : Prints DEC frames map table info (fields data offset) of one acq for debug purpose
*
* \param   : SrcPtFrMap   - Pointer to the FIRST frame map table of the acq \n
*                           It is not like RAW acq map table, it is not one table listing all frames \n
*                           of the acq, there is one table for each frame, we provide pointer on the \n
*                           first table of the acq.
*          :
* \param   : AcqId        - Id of the acquisition, only for printing, not used to find info
*          :
* \param   : FirstFr      - First frame to print
*          :
* \param   : LastFr       - Last frame to print,                                           \n
*                           set LastFr = -1 to display all frames starting at FirstFr      \n
*
* \param   : FrNbInAcq    - Frames n b in the acq, to check param FirstFr, LastFr
*         :
* \return  : Error code
*          : < 0 - Error
*          :   0 - No frames in this Acq => ???
*          : > 0 - Frames number in this Acq
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/06/2020
* \date    : Doc date  : 11/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FPrintDecAcqFrMapTable ( MIS1__TDecFrMap* SrcPtFrMap, UInt32 AcqId, SInt16 FirstFr, SInt16 LastFr, SInt16 FrNbInAcq ) {
  
  UInt16 ViFr;
  MIS1__TDecFrMap* VSrcPtFrMap;

  // Check param
  
  err_retnull ( SrcPtFrMap, (ERR_OUT,"Abort => SrcPtFrMap == NULL") );

  
  if ( FrNbInAcq == 0 ) {
    msg (( MSG_OUT, "============================" ));
    msg (( MSG_OUT, " No DEC frames in Acq No %d ", AcqId ));
    msg (( MSG_OUT, "============================" ));
    return (0);
  }
  
  if ( FirstFr >= FrNbInAcq ) {
    err_retfail ( -1, (ERR_OUT,"Abort => FirstFr = %d > LastFr in table = %d", FirstFr, FrNbInAcq - 1 ) );
  }
  
  if ( LastFr >= FrNbInAcq ) {
    err_retfail ( -1, (ERR_OUT,"Abort => LastFr = %d > LastFr in table = %d", LastFr, FrNbInAcq - 1 ) );
  }
  
  // User wants to display all frames
  
  if ( LastFr == -1 ) {
    LastFr = FrNbInAcq - 1;
  }


  // Print

  for ( ViFr = FirstFr; ViFr <= LastFr; ViFr++ ) {

    VSrcPtFrMap = &SrcPtFrMap[ViFr];

    // Check ptr

    if ( VSrcPtFrMap == NULL ) {
      msg (( MSG_OUT, "============================" ));
      msg (( MSG_OUT, "DEC frame map listing stops on FR = %d not available (FrMap ptr == NULL) ", ViFr  ));
      msg (( MSG_OUT, "============================" ));
      break;
    }

    // Display

    msg (( MSG_OUT, "==================================" ));
    msg (( MSG_OUT, "DEC Fr No = %.4d info", ViFr     ));
    msg (( MSG_OUT, "----------------------------------" ));
    msg (( MSG_OUT, "DEC Acq id in run    = %.4d", VSrcPtFrMap->ResAcqIdInRun  ));   // Id of acquisition
    msg (( MSG_OUT, "DEC Fr nb in acq     = %.4d", VSrcPtFrMap->ResFrNbInAcq     ));
    msg (( MSG_OUT, "DEC Fr id in acq     = %.4d", VSrcPtFrMap->ResFrIdInAcq   ));  // Id frame in its acquisition 0 to max fr nb per acq  - 1
    msg (( MSG_OUT, "DEC Pixels data size = %.4d", VSrcPtFrMap->ResFrSz        ));  // Size of frame in W8
    msg (( MSG_OUT, "DEC ResPtFrBeg       = %.8X", VSrcPtFrMap->ResPtFrBeg    ));
    msg (( MSG_OUT, "==================================" ));
    

  }


  return (FrNbInAcq);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FAcqFrMapToAcqFrList ( UInt8* PtU8AcqBeg, MIS1__TAcqFrMap* SrcPtFrMap, MIS1__TAcqFrList* DestPtFrList, UInt32 AcqId )
*
* \brief   : Prints frames map table info (fields data offset) of one acq for debug purpose
*
* \param   : SrcPtFrMap   - Pointer to the source map table
*          :
* \param   : DestPtFrList - Pointer to the destination frames list 
*          :
* \param   : AcqId        - Id of the acquisition, only for debug printing, not used for processing
*          :
* \return  : Error code
*          : < 0 - Error. Rq : No frame in this Acq is ans error.
*          : > 0 - Frames number in this Acq
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 09/04/2020
* \date    : Doc date  : 09/04/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FAcqFrMapToAcqFrList ( UInt8* PtU8AcqBeg, MIS1__TAcqFrMap* SrcPtFrMap, MIS1__TAcqFrList* DestPtFrList, UInt32 AcqId ) {
  
  UInt16 ViFr;
  UInt16 VTotFrNb;
  
  // Check param
  
  err_retnull ( PtU8AcqBeg, (ERR_OUT,"Abort => PtU8AcqBeg == NULL") );
  
  err_retnull ( SrcPtFrMap, (ERR_OUT,"Abort => SrcPtFrMap == NULL") );
  
  err_retnull ( DestPtFrList, (ERR_OUT,"Abort => DestPtFrList == NULL") );
  
  
  VTotFrNb = SrcPtFrMap->ResFrNb;
  
  if ( VTotFrNb == 0 ) {
    err_retfail ( -1, (ERR_OUT,"No frame in Acq No %d", AcqId) );
  }
  
  // Reset frames list
  
  memset ( DestPtFrList, 0, sizeof (MIS1__TAcqFrList) );
  
  // Build the list
  
  for ( ViFr = 0; ViFr < VTotFrNb; ViFr++ ) {
    
    DestPtFrList->ResAPtFrHead[ViFr]   = (void*) ( (UInt32) PtU8AcqBeg + SrcPtFrMap->ResAPosFrHead[ViFr]);
    DestPtFrList->ResAPtFrData[ViFr]   = (void*) ( (UInt32) PtU8AcqBeg + SrcPtFrMap->ResAPosFrData[ViFr]);
    DestPtFrList->ResAPtFrTrail[ViFr]  = (void*) ( (UInt32) PtU8AcqBeg + SrcPtFrMap->ResAPosFrTrail[ViFr]);
  }
     
  DestPtFrList->ResFrNb = VTotFrNb;
   
  return ( VTotFrNb );
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunInfBuild ( MIS1__TRunPar* PtPar, MIS1__TRunInf* PtInf )
*
* \brief   : Get run parameters from GUI.
*
* \param   : PtPar - Pointer on run param record
*
* \param   : PtInf - Pointer on run info record
*          :
* \return  : Error code
*          : < 0 - Error
*          :   0 - OK
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/05/2020
* \date    : Doc date  : 11/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunInfBuild ( MIS1__TRunPar* PtPar, MIS1__TRunInf* PtInf ) {
  
  // Check param
  
  err_retnull ( PtPar, (ERR_OUT,"Abort => PtPar == NULL") );
  err_retnull ( PtInf, (ERR_OUT,"Abort => PtInf == NULL") );
    
  // Build info
  
  PtInf->InfSave = PtPar->ParSaveRaw || PtPar->ParSaveDec || PtPar->ParSaveFPC;
  
  sprintf ( PtInf->InfRunDir, "%s\\%d", PtPar->ParRunRootDir, PtPar->ParRunNo );
  
  sprintf ( PtInf->InfSCFileName,        "%s\\%s_%d_slow_ctrl.bin", PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );
  sprintf ( PtInf->InfRunConfFileName,   "%s\\%s_%d_run_conf.bin",  PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );
  sprintf ( PtInf->InfRunStatusFileName, "%s\\%s_%d_run_status.bin",  PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );
  
  sprintf ( PtInf->InfRawIndexFileName,  "%s\\%s_%d_raw_index.bin", PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );
  sprintf ( PtInf->InfRawDataFileName ,  "%s\\%s_%d_raw_data.bin", PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );
  
  sprintf ( PtInf->InfDecIndexFileName,  "%s\\%s_%d_dec_index.bin", PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );
  sprintf ( PtInf->InfDecDataFileName,   "%s\\%s_%d_dec_data.bin", PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );

  sprintf ( PtInf->InfFPCSubStepsFileName, "%s\\%s_%d_fpc_substeps.bin" , PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );
  sprintf ( PtInf->InfFPCStepFileName    , "%s\\%s_%d_fpc_step.bin" , PtInf->InfRunDir, PtPar->ParRunPrefix, PtPar->ParRunNo  );

  err_retok (( ERR_OUT, "MsgOk" ));
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunParamPrint ( MIS1__TRunPar* PtPar, MIS1__TRunInf* PtInf )
*          :
* \brief   : Print run parameters in log file.
*
* \param   : PtPar - Pointer on run param record
*          :
* \param   : PtInf - Pointer on run info record
*          :
* \return  : Error code
*          : < 0 - Error
*          :   0 - OK
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/05/2020
* \date    : Doc date  : 11/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunParamInfPrint ( MIS1__TRunPar* PtPar, MIS1__TRunInf* PtInf ) {
  
  // Check param
  
  err_retnull ( PtPar, (ERR_OUT,"Abort => PtPar == NULL") );
  err_retnull ( PtInf, (ERR_OUT,"Abort => PtInf == NULL") );
  
  // Print
  
  msg (( MSG_OUT, "==============================================" ));
  msg (( MSG_OUT, " Run parameters" ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "File format            : %X", PtPar->FileFormat ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, " General parameters" ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "Allow RC               : %d", PtPar->ParAllowRc ));
  msg (( MSG_OUT, "Run prefix             : %s", PtPar->ParRunPrefix ));
  msg (( MSG_OUT, "Run No                 : %d", PtPar->ParRunNo ));
  msg (( MSG_OUT, "Tot fr nb              : %d", PtPar->ParTotFrNb ));
  msg (( MSG_OUT, "Run forever            : %d", PtPar->ParRunForever ));
  msg (( MSG_OUT, "Fr nb / Acq            : %d", PtPar->ParFrNbPerAcq ));
  msg (( MSG_OUT, "Buff Acq nb            : %d", PtPar->ParBufferedAcqNb ));
  msg (( MSG_OUT, "Acq nb / file          : %d", PtPar->ParAcqNbPerFile ));
  msg (( MSG_OUT, "Fr nb / file (not used): %d", PtPar->ParFrNbPerFile ));
  msg (( MSG_OUT, "Proc mode              : %d", (UInt32) PtPar->ParProcMode ));
  msg (( MSG_OUT, "Proc print level       : %d", PtPar->ParProcPrintLvl ));
  msg (( MSG_OUT, "Root dir               : %s", PtPar->ParRunRootDir ));
  msg (( MSG_OUT, "Save raw               : %d", PtPar->ParSaveRaw ));
  msg (( MSG_OUT, "Save decoded           : %d", PtPar->ParSaveDec ));
  msg (( MSG_OUT, "Decoded format         : %d", PtPar->ParDecFormat ));
  msg (( MSG_OUT, "Save fired pix count   : %d", PtPar->ParSaveFPC ));
  msg (( MSG_OUT, "Processing enabled     : %d", PtPar->ParProc ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  
  if ( PtPar->ParCarMode > 0 ) {
  
  msg (( MSG_OUT, " Characterization parameters" ));
  msg (( MSG_OUT, "Characterisation mode  : %d", PtPar->ParCarMode ));
  msg (( MSG_OUT, "Thresholds steps nb    : %d", PtPar->ParCarStepNb ));
  msg (( MSG_OUT, "Sub steps nb           : %d", PtPar->ParCarSubStepNb ));
  msg (( MSG_OUT, "Fr nb / sub step       : %d", PtPar->ParCarSubStepFrNb ));
  msg (( MSG_OUT, "Acq nb / sub step      : %d", PtPar->ParCarSubStepAcqNb ));
  msg (( MSG_OUT, "Fr nb / Acq            : %d", PtPar->ParCarFrNbPerAcq ));
  msg (( MSG_OUT, "Fields not printed" ));
  msg (( MSG_OUT, " - ParCarAAStepTh [][]"));
  msg (( MSG_OUT, " - ParCarSubStepConfId[]" ));
  
  }
    
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, " Info built from run parameters" ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "Save data              : %d", PtInf->InfSave ));
  msg (( MSG_OUT, "Run dir                : %s", PtInf->InfRunDir ));
  msg (( MSG_OUT, "SC file                : %s", PtInf->InfSCFileName ));
  msg (( MSG_OUT, "File raw index         : %s", PtInf->InfRawIndexFileName ));
  msg (( MSG_OUT, "File raw data          : %s", PtInf->InfRawDataFileName ));
  msg (( MSG_OUT, "File decoded index     : %s", PtInf->InfDecIndexFileName )); 
  msg (( MSG_OUT, "File decoded data      : %s", PtInf->InfDecDataFileName ));
  msg (( MSG_OUT, "File FPC sub steps     : %s", PtInf->InfFPCSubStepsFileName  ));
  msg (( MSG_OUT, "File FPC step          : %s", PtInf->InfFPCStepFileName  ));
  msg (( MSG_OUT, "==============================================" ));
  
  
  err_retok (( ERR_OUT, "MsgOk" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunConfFileSave ( char* FilePath, MIS1__TRunConf* PtConf )
*
* \brief   : Save the run configuration in a binary file
*                    
* \param   : FilePath - Name of file, full path
*
* \param   : PtConf   - Pointer to the record to save
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FRunConfFileSave ( char* FilePath, MIS1__TRunConf* PtConf ) {
  
  SInt32 VRet;
  
  // Check param
  
  err_retnull ( FilePath, (ERR_OUT,"Abort => FilePath == NULL") );

  err_retnull ( PtConf, (ERR_OUT,"Abort => PtConf == NULL") );
  
  // Set file format tag
  
  PtConf->RunPar.FileFormat = MIS1__RUN_CONF_FFORMAT_TAG;
  
  // Save record
  
  VRet = FIL_FWriteRecord ( FilePath, PtConf, sizeof (MIS1__TRunConf) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Save run conf file = %s failed !", FilePath) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunConfFileLoad ( char* FilePath, MIS1__TRunConf* PtConf )
*
* \brief   : Load the run configuration from a binary file
*
* \param   : FilePath - Name of file, full path
*
* \param   : PtConf   - Pointer to the destination record
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FRunConfFileLoad ( char* FilePath, MIS1__TRunConf* PtConf ) {
  
  SInt32 VRet;
  
  // Check param
  
  err_retnull ( FilePath, (ERR_OUT,"Abort => FilePath == NULL") );
  
  err_retnull ( PtConf, (ERR_OUT,"Abort => PtConf == NULL") );
    
  // Load record
  
  VRet = FIL_FReadRecord ( FilePath, PtConf, sizeof (MIS1__TRunConf) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Load run conf file = %s failed !", FilePath) );
  
  // Check file format tag
  
  
  if ( PtConf->RunPar.FileFormat != MIS1__RUN_CONF_FFORMAT_TAG ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Bad file format = %X <> Expected = %X", PtConf->RunPar.FileFormat, MIS1__RUN_CONF_FFORMAT_TAG) )
  }

  
  err_retok (( ERR_OUT, "" ));
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunStatusPrint (  MIS1__TRunStatus* PtStatus )
*          
* \brief   : Print run parameters in log file.
*
* \param   : PtPar - Pointer on run param record
*          
* \param   : PtInf - Pointer on run info record
*          
* \return  : Error code  \n
*          : < 0 - Error \n
*          :   0 - OK    \n
*          
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunStatusPrint (  MIS1__TRunStatus* PtStatus ) {
  
  // Check param
  
  err_retnull ( PtStatus, (ERR_OUT,"Abort => PtStatus == NULL") );

  
  // Print
  
  msg (( MSG_OUT, "==============================================" ));
  msg (( MSG_OUT, " Run status" ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "File format          : %X", PtStatus->FileFormat ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "ParGuiUpdRate        : %d", PtStatus->ParGuiUpdRate ));
  msg (( MSG_OUT, "ParUpdOnlyAcqErrCnt  : %d", PtStatus->ParUpdOnlyAcqErrCnt ));
  msg (( MSG_OUT, "ParOptDaqSpeed       : %d", PtStatus->ParOptDaqSpeed ));
  msg (( MSG_OUT, "----------------------------------------------" ));  
  msg (( MSG_OUT, "ResRunInProgress     : %d", PtStatus->ResRunInProgress ));
  msg (( MSG_OUT, "ResAcqCnt            : %d", PtStatus->ResAcqCnt ));
  msg (( MSG_OUT, "ResFrCnt             : %d", PtStatus->ResFrCnt ));
  msg (( MSG_OUT, "ResAcqRateHz         : %.1f", PtStatus->ResAcqRateHz ));
  msg (( MSG_OUT, "ResFrRateHz          : %.1f", PtStatus->ResFrRateHz ));
  msg (( MSG_OUT, "ResMSis1FrRateHz     : %.1f", PtStatus->ResMSis1FrRateHz ));
  msg (( MSG_OUT, "ResDaqEffPCent       : %.1f", PtStatus->ResDaqEffPCent ));
  msg (( MSG_OUT, "ResFrNbInCurAcq      : %d", PtStatus->ResFrNbInCurAcq ));  
  msg (( MSG_OUT, "==============================================" ));
   
  
  err_retok (( ERR_OUT, "MsgOk" ));
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunStatusFileSave ( char* FilePath, MIS1__TRunStatus* PtStatus )
*
* \brief   : Save the run configuration in a binary file
*
* \param   : FilePath - Name of file, full path
*
* \param   : PtStatus   - Pointer to the record to save
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FRunStatusFileSave ( char* FilePath, MIS1__TRunStatus* PtStatus ) {
  
  SInt32 VRet;
  
  // Check param
  
  err_retnull ( FilePath, (ERR_OUT,"Abort => FilePath == NULL") );
  
  err_retnull ( PtStatus, (ERR_OUT,"Abort => PtStatus == NULL") );

  // Set file format tag
  
  PtStatus->FileFormat = MIS1__RUN_STATUS_FFORMAT_TAG;
  
  // Save record
  
  VRet = FIL_FWriteRecord ( FilePath, PtStatus, sizeof (MIS1__TRunStatus) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Save run status file = %s failed !", FilePath) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunStatusFileLoad ( char* FilePath, MIS1__TRunStatus* PtStatus )
*
* \brief   : Load the run configuration from a binary file
*
* \param   : FilePath - Name of file, full path
*
* \param   : PtStatus - Pointer to the destination record
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FRunStatusFileLoad ( char* FilePath, MIS1__TRunStatus* PtStatus ) {
  
  SInt32 VRet;
  
  // Check param
  
  err_retnull ( FilePath, (ERR_OUT,"Abort => FilePath == NULL") );
  
  err_retnull ( PtStatus, (ERR_OUT,"Abort => PtStatus == NULL") );
  
  // Load record
  
  VRet = FIL_FReadRecord ( FilePath, PtStatus, sizeof (MIS1__TRunStatus) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Load run status file = %s failed !", FilePath) );
  
  // Check file format tag
  
  if ( PtStatus->FileFormat != MIS1__RUN_STATUS_FFORMAT_TAG ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Bad file format = %X <> Expected = %X", PtStatus->FileFormat, MIS1__RUN_STATUS_FFORMAT_TAG) )
  }

  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesBegin ( UInt8 ObjId = 0 )
*
* \brief   : Initialize run files records, objects, needed to access to run files
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */




#ifdef MIS1__CC_DATA_FORMAT_SINCE_V211

SInt32 MIS1__FRunFilesBegin ( char* ErrLogFile, SInt8 ErrLogLvl, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles* VPtRF;
  
  // Check param
  
  if ( ObjId >= MIS1__MAX_RUN_FILES ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ObjId = %d out of range [0..%d]", ObjId, MIS1__MAX_RUN_FILES - 1) );
  }
  
  // Set ptr to record
  
  VPtRF = &MIS1__VGARunFile[ObjId];
  
  // Reset record
  
  memset ( VPtRF, 0, sizeof (MIS1__TRunFiles) );
  
  // Init fields 

  VPtRF->RawLoadedFileSuffix        = -1;
  VPtRF->DecLoadedFileSuffix        = -1;
  VPtRF->FpcStepLoadedFileSuffix    = -1;
  VPtRF->FpcSubStepLoadedFileSuffix = -1;
  
  VPtRF->RawCurAcqId             = -1;
  VPtRF->RawCurFrId              = -1;

  VPtRF->DecCurAcqId             = -1;
  VPtRF->DecCurFrId              = -1;
  
  VPtRF->FpcCurStepId            = -1;
  VPtRF->FpcCurSubStepId         = -1;


  VPtRF->RawCurAcqFileSuffix     = -1;

  VPtRF->DecCurAcqFileSuffix     = -1;
  
  VPtRF->FpcCurStepFileSuffix    = -1;
  
  VPtRF->FpcCurSubStepFileSuffix = -1;

  
  // Init OBuff objects IDs
  
  VPtRF->RawFBuffIndexId = ObjId * MIS1__MAX_OBUFF_PER_RUN_FILES;
  VPtRF->RawFBuffDataId  = VPtRF->RawFBuffIndexId + 1;
  
  
  VPtRF->DecFBuffIndexId = VPtRF->RawFBuffIndexId + 2;  
  VPtRF->FBuffDecDataId  = VPtRF->RawFBuffIndexId + 3;
  

  
  // Create RAW data files objects
  // - Index file

  VRet = BF__FOBuffInit ( VPtRF->RawFBuffIndexId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for raw index file, ObjId = %d failed !", ObjId) );

  // Create RAW data files objects
  // - Data file
  
  VRet = BF__FOBuffInit ( VPtRF->RawFBuffDataId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for raw data file, ObjId = %d failed !", ObjId) );
     


  // Create DEC data files objects
  // - Index file
  
  VRet = BF__FOBuffInit ( VPtRF->DecFBuffIndexId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for Dec index file, ObjId = %d failed !", ObjId) );
  
  // Create DEC data files objects
  // - Data file
  
  VRet = BF__FOBuffInit ( VPtRF->FBuffDecDataId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for Dec data file, ObjId = %d failed !", ObjId) );
  
  // -----------------------------------
  // Create FPC data files objects
  // -----------------------------------
  
  // Init ptr
  
  VPtRF->PtFPCSubStepsBinFile = &MIS1__VGFPCSubStepsBinFile;
  VPtRF->PtFPCStepBinFile     = &MIS1__VGFPCStepBinFile;
  
  // Init objects
  
  VRet = VPtRF->PtFPCSubStepsBinFile->PubFBegin ( ErrLogFile, 1 /* EnableErrLog */, ErrLogLvl );

  err_retfail ( VRet, (ERR_OUT,"Abort => Create TCBinFile for sub steps data file, ObjId = %d failed !", ObjId) );

  VRet = VPtRF->PtFPCStepBinFile->PubFBegin ( ErrLogFile, 1 /* EnableErrLog */, ErrLogLvl );

  err_retfail ( VRet, (ERR_OUT,"Abort => Create TCBinFile for steps data file, ObjId = %d failed !", ObjId) );

  // Configure objects

  VRet = VPtRF->PtFPCSubStepsBinFile->PubFConf  ( "" /* DataFile */, FIL__TCBinFile_RWB_MODE_READ, sizeof (MIS1__TMatPixCntU16) /* MaxBlocSz */, sizeof (MIS1__TMatPixCntU16) /* BlocSz */, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );

  err_retfail ( VRet, (ERR_OUT,"Abort => Conf TCBinFile for steps data file, ObjId = %d failed !", ObjId) );

  VRet = VPtRF->PtFPCStepBinFile->PubFConf  ( "" /* DataFile */, FIL__TCBinFile_RWB_MODE_READ, sizeof (MIS1__TMatPixCntU16) /* MaxBlocSz */, sizeof (MIS1__TMatPixCntU16) /* BlocSz */, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Conf TCBinFile for sub steps data file, ObjId = %d failed !", ObjId) );
  

  // Init done

  VPtRF->FBeginDone = 1;
    
  err_retok (( ERR_OUT, "" ));
}


#else


SInt32 MIS1__FRunFilesBegin ( UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles* VPtRF;
  
  // Check param
  
  if ( ObjId >= MIS1__MAX_RUN_FILES ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ObjId = %d out of range [0..%d]", ObjId, MIS1__MAX_RUN_FILES - 1) );
  }
  
  // Set ptr to record
  
  VPtRF = &MIS1__VGARunFile[ObjId];
  
  // Reset record
  
  memset ( VPtRF, 0, sizeof (MIS1__TRunFiles) );
  
  // Init fields
  
  VPtRF->RawLoadedFileSuffix = -1;
  VPtRF->DecLoadedFileSuffix = -1;
  
  VPtRF->RawCurAcqId            = -1;
  VPtRF->RawCurFrId             = -1;
  
  VPtRF->DecCurAcqId            = -1;
  VPtRF->DecCurFrId             = -1;
  
  
  VPtRF->RawCurAcqFileSuffix    = -1;
  
  
  VPtRF->DecCurAcqFileSuffix    = -1;
  
  // Init OBuff objects IDs
  
  VPtRF->RawFBuffIndexId = ObjId * MIS1__MAX_OBUFF_PER_RUN_FILES;
  VPtRF->RawFBuffDataId  = VPtRF->RawFBuffIndexId + 1;
  
  
  VPtRF->DecFBuffIndexId = VPtRF->RawFBuffIndexId + 2;
  VPtRF->FBuffDecDataId  = VPtRF->RawFBuffIndexId + 3;
  
  
  
  // Create RAW data files objects
  // - Index file
  
  VRet = DAMIS1__FOBuffInit ( VPtRF->RawFBuffIndexId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for raw index file, ObjId = %d failed !", ObjId) );
  
  // Create RAW data files objects
  // - Data file
  
  VRet = DAMIS1__FOBuffInit ( VPtRF->RawFBuffDataId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for raw data file, ObjId = %d failed !", ObjId) );
  
  
  
  // Create DEC data files objects
  // - Index file
  
  VRet = DAMIS1__FOBuffInit ( VPtRF->DecFBuffIndexId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for Dec index file, ObjId = %d failed !", ObjId) );
  
  // Create DEC data files objects
  // - Data file
  
  VRet = DAMIS1__FOBuffInit ( VPtRF->FBuffDecDataId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Create OBuff for Dec data file, ObjId = %d failed !", ObjId) );
  
  
  
  VPtRF->FBeginDone = 1;
  
  err_retok (( ERR_OUT, "" ));
}


#endif





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesOpen ( char* RunConfFile, SInt8 ReadRaw, SInt8 ReadDec, SInt8 ReadFP, UInt8 ObjId = 0 )
*
* \brief   : Open a raw data run file = load conf, status files, allocate buffers
*
* \param   : RunConfFile - Run config file, ex : MSIS1_777_run_conf.bin
*
* \param   : ReadRaw     - The run raw data files will be loaded, please read remark
*
* \param   : ReadDec     - The run decoded data files will be loaded, please read remark
*
* \param   : ReadFP      - The run fired pixels data files will be loaded, please read remark
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    : One or all modes ReadRaw, ReadDec, ReadFP can be enabled \n
*                        but each mode requires memory => don't enable unsed modes
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


#ifdef MIS1__CC_DATA_FORMAT_SINCE_V211

SInt32 MIS1__FRunFilesOpen ( char* RunConfFile, SInt8 ReadRaw, SInt8 ReadDec, SInt8 ReadFP, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  // Check param
  
  if ( ObjId >= MIS1__MAX_RUN_FILES ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ObjId = %d out of range [0..%d]", ObjId, MIS1__MAX_RUN_FILES - 1) );
  }
  
  err_retnull ( RunConfFile, (ERR_OUT,"Abort => RunRootDir == NULL") );

  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesBegin () has been called
  
  if ( VPtRF->FBeginDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesBegin () has not been called !") );
  }
  
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Load run conf file
  
  VRet = MIS1__FRunConfFileLoad ( RunConfFile, VPtRConf );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Load run conf file = %s failed !", RunConfFile) );
  
  // Load run status

  VRet = MIS1__FRunStatusFileLoad ( VPtRInf->InfRunStatusFileName, VPtRStatus );

  err_retfail ( VRet, (ERR_OUT,"Abort => Load run status file = %s failed !", VPtRInf->InfRunStatusFileName) );

  // Calculate Acq max sz
  
  VPtRF->AcqMaxSzW64 = MIS1__DS_TOT_FRAME_SZ_W64 * VPtRPar->ParFrNbPerAcq;
  
  VPtRF->AcqMaxSzW8  = MIS1__DS_TOT_FRAME_SZ_W8  * VPtRPar->ParFrNbPerAcq;
  

  // Alloc RAW data files objects
  
  if ( ReadRaw == 1 ) {
    
    // - Index file
    
    // 600 VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParBufferedAcqNb * sizeof (MIS1__TAcqFrMap) / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffIndexId );

    VRet = BF__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * sizeof (MIS1__TAcqFrMap) /* BuffW8Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffIndexId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for raw index file, ObjId = %d failed !", ObjId) );
    
    // Alloc RAW data files objects
    // - Data file

    // 600 VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParBufferedAcqNb * VPtRF->AcqMaxSzW64 / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffDataId );

    VRet = BF__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * VPtRF->AcqMaxSzW64 /* BuffW8Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffDataId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for raw file, ObjId = %d failed !", ObjId) );
  
  } // End if ( ReadRaw == 1 )
  
  
  
  
  
  // Alloc DEC data files objects
  
  if ( ReadDec == 1 ) {
    
    // - Index file
    
    // 600 VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParBufferedAcqNb * VPtRPar->ParFrNbPerAcq * sizeof (MIS1__TDecFrMap) / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->DecFBuffIndexId );

    VRet = BF__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * VPtRPar->ParFrNbPerAcq * sizeof (MIS1__TDecFrMap) /* BuffW8Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->DecFBuffIndexId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for dec index file, ObjId = %d failed !", ObjId) );
    
    // Alloc DEC data files objects
    // - Data file
    
    // 600 VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParBufferedAcqNb * VPtRPar->ParFrNbPerAcq * sizeof (MIS1__TDecFrPix) / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->FBuffDecDataId );

    VRet = BF__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * VPtRPar->ParFrNbPerAcq * sizeof (MIS1__TDecFrPix) /* BuffW8Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->FBuffDecDataId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for dec file, ObjId = %d failed !", ObjId) );
    
  } // End if ( ReadRaw == 1 )
  
  
  
  VPtRF->FOpenDone = 1;

  err_retok (( ERR_OUT, "" ));
}


#else

SInt32 MIS1__FRunFilesOpen ( char* RunConfFile, SInt8 ReadRaw, SInt8 ReadDec, SInt8 ReadFP, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  // Check param
  
  if ( ObjId >= MIS1__MAX_RUN_FILES ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ObjId = %d out of range [0..%d]", ObjId, MIS1__MAX_RUN_FILES - 1) );
  }
  
  err_retnull ( RunConfFile, (ERR_OUT,"Abort => RunRootDir == NULL") );
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesBegin () has been called
  
  if ( VPtRF->FBeginDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesBegin () has not been called !") );
  }
  
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Load run conf file
  
  VRet = MIS1__FRunConfFileLoad ( RunConfFile, VPtRConf );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Load run conf file = %s failed !", RunConfFile) );
  
  // Load run status
  
  VRet = MIS1__FRunStatusFileLoad ( VPtRInf->InfRunStatusFileName, VPtRStatus );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Load run status file = %s failed !", VPtRInf->InfRunStatusFileName) );
  
  // Calculate Acq max sz
  
  VPtRF->AcqMaxSzW64 = MIS1__DS_TOT_FRAME_SZ_W64 * VPtRPar->ParFrNbPerAcq;
  
  VPtRF->AcqMaxSzW8  = MIS1__DS_TOT_FRAME_SZ_W8  * VPtRPar->ParFrNbPerAcq;
  
  
  // Alloc RAW data files objects
  
  if ( ReadRaw == 1 ) {
    
    // - Index file
    
    VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * sizeof (MIS1__TAcqFrMap) / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffIndexId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for raw index file, ObjId = %d failed !", ObjId) );
    
    // Alloc RAW data files objects
    // - Data file
    
    VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * VPtRF->AcqMaxSzW64 / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffDataId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for raw file, ObjId = %d failed !", ObjId) );
    
  } // End if ( ReadRaw == 1 )
  
  
  
  
  
  // Alloc DEC data files objects
  
  if ( ReadDec == 1 ) {
    
    // - Index file
    
    VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * VPtRPar->ParFrNbPerAcq * sizeof (MIS1__TDecFrMap) / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->DecFBuffIndexId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for dec index file, ObjId = %d failed !", ObjId) );
    
    // Alloc DEC data files objects
    // - Data file
    
    VRet = DAMIS1__FOBuffAllocFree ( 1 /* FreeAlloc */, VPtRPar->ParAcqNbPerFile * VPtRPar->ParFrNbPerAcq * sizeof (MIS1__TDecFrPix) / 8 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->FBuffDecDataId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Alloc OBuff for dec file, ObjId = %d failed !", ObjId) );
    
  } // End if ( ReadRaw == 1 )
  
  
  
  VPtRF->FOpenDone = 1;
  
  err_retok (( ERR_OUT, "" ));
}


#endif



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesPrintConf ( UInt8 ObjId = 0 )
*
* \brief   : Print run config file
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    : 
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/05/2020
* \date    : Doc date  : 26/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesPrintConf ( UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;


  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }

  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  VRet = MIS1__FRunParamInfPrint ( VPtRPar, VPtRInf );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunParamInfPrint (...) failed !") );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesPrintStatus ( UInt8 ObjId = 0 )
*
* \brief   : Print run status file
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/05/2020
* \date    : Doc date  : 26/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesPrintStatus ( UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  VRet = MIS1__FRunStatusPrint ( VPtRStatus );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunStatusPrint (...) failed !") );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesGetAcqFrNb ( UInt32* PtAcqNb, UInt32* PtFrNb, UInt8 ObjId = 0 )
*
* \brief   : Print run status file
*
* \param   : PtAcqNb - Pointer to acq number
*
* \param   : PtFrNb  - Pointer to frames number
*
* \param   : ObjId   - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 30/05/2020
* \date    : Doc date  : 30/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesGetAcqFrNb ( UInt32* PtAcqNb, UInt32* PtFrNb, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunStatus* VPtRStatus;
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Update results
  
  if ( PtAcqNb != NULL ) {
    *PtAcqNb = VPtRStatus->ResAcqCnt;
  }
  
  
  if ( PtFrNb != NULL ) {
    *PtFrNb = VPtRStatus->ResFrCnt;
  }
  
    
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesRawPrivFindAcq ( SInt32 AcqId, UInt8 ObjId = 0 )
*
* \brief   : Private function, calculate info needed to access to AcqId, file No, etc ...
*
* \param   : AcqId - The no, id of the aquisition
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 27/05/2020
* \date    : Doc date  : 27/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesRawPrivFindAcq ( SInt32 AcqId, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  
  // Check AcqId
  
  err_retfail ( AcqId, (ERR_OUT,"Abort => AcqId = %d < 0 !", AcqId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Set AcqId, FrId
  
  VPtRF->RawCurAcqId = AcqId;
  VPtRF->RawCurFrId  = 0; // Set first frame by default
  
  // Check if AcqId is in run files
  
  if ( AcqId >= VPtRStatus->ResAcqCnt ) {
    VPtRF->RawCurAcqFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => AcqId = %d > Run acq nb = %d", AcqId, VPtRStatus->ResAcqCnt ) );
  }
  
  
  // Calculate info

  VPtRF->RawCurAcqFileSuffix = AcqId / VPtRPar->ParAcqNbPerFile;
  VPtRF->RawCurAcqIdInFile   = AcqId % VPtRPar->ParAcqNbPerFile;


  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesDecPrivFindAcq ( SInt32 AcqId, UInt8 ObjId = 0 )
*
* \brief   : Private function, calculate info needed to access to AcqId, file No, etc ...
*
* \param   : AcqId - The no, id of the aquisition
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 10/06/2020
* \date    : Doc date  : 10/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesDecPrivFindAcq ( SInt32 AcqId, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  
  // Check AcqId
  
  err_retfail ( AcqId, (ERR_OUT,"Abort => AcqId = %d < 0 !", AcqId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Set AcqId, FrId
  
  VPtRF->DecCurAcqId = AcqId;
  VPtRF->DecCurFrId  = 0; // Set first frame by default
  
  // Check if AcqId is in run files
  
  if ( AcqId >= VPtRStatus->ResAcqCnt ) {
    VPtRF->DecCurAcqFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => AcqId = %d > Run acq nb = %d", AcqId, VPtRStatus->ResAcqCnt ) );
  }
  
  
  // Calculate info

  VPtRF->DecCurAcqFileSuffix = AcqId / VPtRPar->ParAcqNbPerFile;
  VPtRF->DecCurAcqIdInFile   = AcqId % VPtRPar->ParAcqNbPerFile;


  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesPrivFindFr ( SInt32 FrId, UInt8 ObjId = 0 )
*
* \brief   : Private function, calculate info needed to access to AcqId, file No, etc ...
*
* \param   : FrId  - The no of the frame
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 27/05/2020
* \date    : Doc date  : 27/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesRawPrivFindFr ( SInt32 FrId, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;


  err_retfail ( FrId, (ERR_OUT,"Abort => This function has not been checked !") );


  // Check AcqId

  err_retfail ( FrId, (ERR_OUT,"Abort => FrId = %d < 0 !", FrId) );

  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;

  
  // Set AcqId, FrId
  
  VPtRF->RawCurAcqId = FrId / VPtRPar->ParFrNbPerAcq;
  VPtRF->RawCurFrId  = FrId;

  // Check if FrId is in run files

  if ( FrId >= VPtRStatus->ResFrCnt ) {
    VPtRF->RawCurAcqFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => FrId = %d > Run fr nb = %d", FrId, VPtRStatus->ResFrCnt ) );
  }
  
  // Calculate info
    
  VPtRF->RawCurAcqFileSuffix = VPtRF->RawCurAcqId / VPtRPar->ParAcqNbPerFile;
  VPtRF->RawCurAcqIdInFile   = VPtRF->RawCurAcqId % VPtRPar->ParAcqNbPerFile;

  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesDecPrivFindFr ( SInt32 FrId, UInt8 ObjId = 0 )
*
* \brief   : Private function, calculate info needed to access to AcqId, file No, etc ...
*
* \param   : FrId  - The no of the frame
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 10/06/2020
* \date    : Doc date  : 10/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesDecPrivFindFr ( SInt32 FrId, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;


  err_retfail ( FrId, (ERR_OUT,"Abort => This function has not been checked !") );


  // Check AcqId

  err_retfail ( FrId, (ERR_OUT,"Abort => FrId = %d < 0 !", FrId) );

  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;

  
  // Set AcqId, FrId
  
  VPtRF->DecCurAcqId = FrId / VPtRPar->ParFrNbPerAcq;
  VPtRF->DecCurFrId  = FrId;

  // Check if FrId is in run files

  if ( FrId >= VPtRStatus->ResFrCnt ) {
    VPtRF->DecCurAcqFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => FrId = %d > Run fr nb = %d", FrId, VPtRStatus->ResFrCnt ) );
  }
  
  // Calculate info
    
  VPtRF->DecCurAcqFileSuffix = VPtRF->DecCurAcqId / VPtRPar->ParAcqNbPerFile;
  VPtRF->DecCurAcqIdInFile   = VPtRF->DecCurAcqId % VPtRPar->ParAcqNbPerFile;

  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesFpcPrivFindStep ( SInt32 StepId, UInt8 ObjId = 0 )
*
* \brief   : Private function, calculate info needed to access to StepId, file No, etc ...
*
* \param   : StepId - The no, id of the step
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 03/07/2020
* \date    : Doc date  : 03/07/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesFpcPrivFindStep ( SInt32 StepId, UInt8 ObjId = 0 ) {
 
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  SInt32            VLastAcqOfStep;
  
  
  // Check StepId
  
  err_retfail ( StepId, (ERR_OUT,"Abort => StepId = %d < 0 !", StepId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Set AcqId, FrId => Not used
  
  VPtRF->RawCurAcqId = -1;
  VPtRF->RawCurFrId  = -1;
  
  // Set FpcCurStepId,
  
  VPtRF->FpcCurStepId    = StepId;
  
  // Check if StepId is in run files
  
  
  if ( StepId >= VPtRPar->ParCarStepNb ) {
    VPtRF->FpcCurStepFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => StepId = %d > Run step nb = %d", StepId, VPtRPar->ParCarStepNb ) );
  }
  
  
  VLastAcqOfStep = ((StepId + 1) * VPtRPar->ParCarSubStepNb * VPtRPar->ParCarSubStepAcqNb) - 1;
  
  if ( VLastAcqOfStep >= VPtRStatus->ResAcqCnt ) {
    VPtRF->FpcCurStepFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => StepId = %d requires %d Acq in run, there are only %d acq", StepId, VLastAcqOfStep + 1, VPtRStatus->ResAcqCnt ) );
  }
  
  
  // Calculate info
  
  VPtRF->FpcCurStepFileSuffix     = StepId;

  
     
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesFpcPrivFindSubStep ( SInt32 StepId, SInt32 SubStepId, UInt8 ObjId = 0 )
*
* \brief   : Private function, calculate info needed to access to StepId, SubStepId file No, etc ...
*
* \param   : StepId - The no, id of the step
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 06/07/2020
* \date    : Doc date  : 06/07/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesFpcPrivFindSubStep ( SInt32 StepId, SInt32 SubStepId, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  SInt32            VLastAcqOfStep;
  
  
  // Check StepId, SubStepId
  
  err_retfail ( StepId, (ERR_OUT,"Abort => StepId = %d < 0 !", StepId) );
  
  err_retfail ( SubStepId, (ERR_OUT,"Abort => SubStepId = %d < 0 !", SubStepId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Set AcqId, FrId => Not used
  
  VPtRF->RawCurAcqId = -1;
  VPtRF->RawCurFrId  = -1;
  
  // Set FpcCurStepId,
  

  VPtRF->FpcCurSubStepId = SubStepId; 
  
  // Check if StepId is in run files
    
  if ( StepId >= VPtRPar->ParCarStepNb ) {
    VPtRF->FpcCurStepFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => StepId = %d > Run step nb = %d", StepId, VPtRPar->ParCarStepNb ) );
  }
  
  
  VLastAcqOfStep = ((StepId + 1) * VPtRPar->ParCarSubStepNb * VPtRPar->ParCarSubStepAcqNb) - 1;
  
  if ( VLastAcqOfStep >= VPtRStatus->ResAcqCnt ) {
    VPtRF->FpcCurStepFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => StepId = %d requires %d Acq in run, there are only %d acq", StepId, VLastAcqOfStep + 1, VPtRStatus->ResAcqCnt ) );
  }
  
  
  // Check sub step id
  
  if ( SubStepId > VPtRPar->ParCarSubStepNb ) {
    VPtRF->FpcCurSubStepFileSuffix = -1;
    err_retfail ( -1, (ERR_OUT,"Abort => SubStepId = %d out of range [0..%d]", SubStepId, VPtRPar->ParCarSubStepNb ) );
  }
  
  
  // Calculate info
  

  VPtRF->FpcCurSubStepFileSuffix  = StepId; // Only one file for all sub steps => same suffix as step file
  
  
  err_retok (( ERR_OUT, "" ));
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesRawPrintCurAcqInf ( UInt8 ObjId = 0 )
*
* \brief   : Print current acq information field, for test / debug
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 27/05/2020
* \date    : Rev       : 11/06/2020 \n
*                        - Three functions Raw, Dec, Fpc
* \date    : Doc date  : 27/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesRawPrintCurAcqInf ( UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
    
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Print
  
  msg (( MSG_OUT, "==============================================" ));
  msg (( MSG_OUT, " Current acq info" ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "RawCurAcqId          : %d", VPtRF->RawCurAcqId ));
  msg (( MSG_OUT, "RawCurFrId           : %d", VPtRF->RawCurFrId ));
  msg (( MSG_OUT, "RawCurAcqFileSuffix     : %d", VPtRF->RawCurAcqFileSuffix ));
  msg (( MSG_OUT, "RawCurAcqIdInFile       : %d", VPtRF->RawCurAcqIdInFile ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "RawLoadedFileSuffix  : %d", VPtRF->RawLoadedFileSuffix ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "----------------------------------------------" ));

    
  if ( VPtRF->RawCurAcqId >= 0 ) {    
    MIS1__FPrintRawAcqFrMapTable ( &(VPtRF->PtFileRawFrMap[VPtRF->RawCurAcqIdInFile]), VPtRF->RawCurAcqId, -1 /* FirstFr */, -1 /* LastFr */, 0 /* PrintDecHex */ );
  }
  
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesDecPrintCurAcqInf ( UInt8 ObjId = 0 )
*
* \brief   : Print current acq information field, for test / debug
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/06/2020
* \date    : Doc date  : 11/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunFilesDecPrintCurAcqInf ( UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
    
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Print
  
  msg (( MSG_OUT, "==============================================" ));
  msg (( MSG_OUT, " Current acq info" ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "DecCurAcqId          : %d", VPtRF->DecCurAcqId ));
  msg (( MSG_OUT, "DecCurFrId           : %d", VPtRF->DecCurFrId ));
  msg (( MSG_OUT, "DecCurAcqFileSuffix  : %d", VPtRF->DecCurAcqFileSuffix ));
  msg (( MSG_OUT, "DecCurAcqIdInFile    : %d", VPtRF->DecCurAcqIdInFile ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "DecLoadedFileSuffix  : %d", VPtRF->DecLoadedFileSuffix ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "----------------------------------------------" ));

  // Displays only first and last frame of acq

  if ( VPtRF->DecCurAcqId >= 0 ) {
    MIS1__FPrintDecAcqFrMapTable ( &(VPtRF->PtFileDecFrMap[VPtRF->DecCurAcqIdInFile * VPtRPar->ParFrNbPerAcq]), VPtRF->RawCurAcqId, 0 /* FirstFr */, 0 /* LastFr */, VPtRPar->ParFrNbPerAcq /* FrNbInAcq */ );
    MIS1__FPrintDecAcqFrMapTable ( &(VPtRF->PtFileDecFrMap[VPtRF->DecCurAcqIdInFile * VPtRPar->ParFrNbPerAcq]), VPtRF->RawCurAcqId, VPtRPar->ParFrNbPerAcq-1 /* FirstFr */, VPtRPar->ParFrNbPerAcq-1 /* LastFr */, VPtRPar->ParFrNbPerAcq /* FrNbInAcq */ );
  }
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesRawGotoAcq ( SInt32 AcqId, UInt8 ObjId = 0 )
*
* \brief   : Print current acq information field, for test / debug
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 27/05/2020
* \date    : Doc date  : 27/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

#ifdef MIS1__CC_DATA_FORMAT_SINCE_V211

SInt32 MIS1__FRunFilesRawGotoAcq ( SInt32 AcqId, MIS1__TAcqFrList** PtPtAcqFrList, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  MIS1__TAcqFrMap*  VPtCurAcqFrMap;
  
  
  // Check AcqId
  
  err_retfail ( AcqId, (ERR_OUT,"Abort => AcqId = %d < 0 !", AcqId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Calc acq info
  
  VRet = MIS1__FRunFilesRawPrivFindAcq ( AcqId, ObjId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunFilesPrivFindAcq ( AcqId = %d, ObjId = %d ) failed !", AcqId, ObjId ) );
  
  // Load the raw run files if not already in memory
  
  if ( VPtRF->RawCurAcqFileSuffix != VPtRF->RawLoadedFileSuffix ) {
    
    //
    
    // 600 VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfRawIndexFileName /* FileName */, VPtRF->RawCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->RawFBuffIndexId );

    VRet = BF__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfRawIndexFileName /* FileName */, VPtRF->RawCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->RawFBuffIndexId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Load raw index file = %s, suffix = %d failed !", VPtRInf->InfRawIndexFileName, VPtRF->RawCurAcqFileSuffix ) );
  
    // 600 VPtRF->PtFileRawFrMap = (MIS1__TAcqFrMap*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->RawFBuffIndexId  );

    VPtRF->PtFileRawFrMap = (MIS1__TAcqFrMap*) BF__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->RawFBuffIndexId  ); // 600

    // 600 VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfRawDataFileName /* FileName */, VPtRF->RawCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->RawFBuffDataId );

    VRet = BF__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfRawDataFileName /* FileName */, VPtRF->RawCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->RawFBuffDataId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Load raw data file = %s, suffix = %d failed !", VPtRInf->InfRawDataFileName, VPtRF->RawCurAcqFileSuffix ) );

    // 600 VPtRF->PtAcqRawMem = (UInt8*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->RawFBuffDataId  );

    VPtRF->PtAcqRawMem = (UInt8*) BF__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->RawFBuffDataId  );
    

    // InfRawIndexFileName
    // InfRawDataFileName
    //
    // RawFBuffIndexId
    // RawFBuffDataId
    
    VPtRF->RawLoadedFileSuffix = VPtRF->RawCurAcqFileSuffix;
    
  }
  
  
  VPtCurAcqFrMap = &(VPtRF->PtFileRawFrMap[VPtRF->RawCurAcqIdInFile]);
  
  // MIS1__FAcqFrMapToAcqFrList ( UInt8* PtU8AcqBeg, MIS1__TAcqFrMap* SrcPtFrMap, MIS1__TAcqFrList* DestPtFrList, UInt32 AcqId )
  
  VRet = MIS1__FAcqFrMapToAcqFrList ( &(VPtRF->PtAcqRawMem[VPtCurAcqFrMap->ResAcqPos]) /* PtU8AcqBeg */, VPtCurAcqFrMap /* SrcPtFrMap */, &(VPtRF->CurAcqRawFrList) /* DestPtFrList */, AcqId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Building acq frames list has failed !") );


  if ( PtPtAcqFrList != NULL ) {
    *PtPtAcqFrList = &(VPtRF->CurAcqRawFrList);
  }
  

  err_retok (( ERR_OUT, "" ));
}

#else

SInt32 MIS1__FRunFilesRawGotoAcq ( SInt32 AcqId, MIS1__TAcqFrList** PtPtAcqFrList, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  MIS1__TAcqFrMap*  VPtCurAcqFrMap;
  
  
  // Check AcqId
  
  err_retfail ( AcqId, (ERR_OUT,"Abort => AcqId = %d < 0 !", AcqId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Calc acq info
  
  VRet = MIS1__FRunFilesRawPrivFindAcq ( AcqId, ObjId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunFilesPrivFindAcq ( AcqId = %d, ObjId = %d ) failed !", AcqId, ObjId ) );
  
  // Load the raw run files if not already in memory
  
  if ( VPtRF->RawCurAcqFileSuffix != VPtRF->RawLoadedFileSuffix ) {
    
    //
    
    VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfRawIndexFileName /* FileName */, VPtRF->RawCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->RawFBuffIndexId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Load raw index file = %s, suffix = %d failed !", VPtRInf->InfRawIndexFileName, VPtRF->RawCurAcqFileSuffix ) );
    
    VPtRF->PtFileRawFrMap = (MIS1__TAcqFrMap*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->RawFBuffIndexId  );
        
    VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfRawDataFileName /* FileName */, VPtRF->RawCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->RawFBuffDataId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Load raw data file = %s, suffix = %d failed !", VPtRInf->InfRawDataFileName, VPtRF->RawCurAcqFileSuffix ) );
    
    VPtRF->PtAcqRawMem = (UInt8*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->RawFBuffDataId  );
          
    // InfRawIndexFileName
    // InfRawDataFileName
    //
    // RawFBuffIndexId
    // RawFBuffDataId
    
    VPtRF->RawLoadedFileSuffix = VPtRF->RawCurAcqFileSuffix;
    
  }
  
  
  VPtCurAcqFrMap = &(VPtRF->PtFileRawFrMap[VPtRF->RawCurAcqIdInFile]);
  
  // MIS1__FAcqFrMapToAcqFrList ( UInt8* PtU8AcqBeg, MIS1__TAcqFrMap* SrcPtFrMap, MIS1__TAcqFrList* DestPtFrList, UInt32 AcqId )
  
  VRet = MIS1__FAcqFrMapToAcqFrList ( &(VPtRF->PtAcqRawMem[VPtCurAcqFrMap->ResAcqPos]) /* PtU8AcqBeg */, VPtCurAcqFrMap /* SrcPtFrMap */, &(VPtRF->CurAcqRawFrList) /* DestPtFrList */, AcqId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Building acq frames list has failed !") );
  
  
  if ( PtPtAcqFrList != NULL ) {
    *PtPtAcqFrList = &(VPtRF->CurAcqRawFrList);
  }
  
  
  err_retok (( ERR_OUT, "" ));
}


#endif


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesDecGotoAcq ( SInt32 AcqId, UInt8 ObjId = 0 )
*
* \brief   : Print current acq information field, for test / debug
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 10/06/2020
* \date    : Doc date  : 10/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

#ifdef MIS1__CC_DATA_FORMAT_SINCE_V211

SInt32 MIS1__FRunFilesDecGotoAcq ( SInt32 AcqId, MIS1__TDecFrList** PtPtFrList, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  SInt32 VIdInMapTableOfFirstFrOfAcq;
  SInt32 ViFrInAcq;
  
  MIS1__TDecFrMap*   VPtDecFrMap;
  MIS1__TDecFrList*  VPtAcqDecFrList;
  
  
  
  // Check AcqId
  
  err_retfail ( AcqId, (ERR_OUT,"Abort => AcqId = %d < 0 !", AcqId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf    = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar     = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf     = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus  = &MIS1__VGARunFile[ObjId].RunStatus;
  VPtDecFrMap = &MIS1__VGARunFile[ObjId].PtFileDecFrMap[0];
  
  // Calc acq info
  
  VRet = MIS1__FRunFilesDecPrivFindAcq ( AcqId, ObjId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunFilesPrivFindAcq ( AcqId = %d, ObjId = %d ) failed !", AcqId, ObjId ) );
  
  // Load the raw run files if not already in memory
  
  if ( VPtRF->DecCurAcqFileSuffix != VPtRF->DecLoadedFileSuffix ) {
    
    //
    
    // 600 VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfDecIndexFileName /* FileName */, VPtRF->DecCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->DecFBuffIndexId );

    VRet = BF__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfDecIndexFileName /* FileName */, VPtRF->DecCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->DecFBuffIndexId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Load dec index file = %s, suffix = %d failed !", VPtRInf->InfDecIndexFileName, VPtRF->DecCurAcqFileSuffix ) );

    // 600 VPtRF->PtFileDecFrMap = (MIS1__TDecFrMap*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->DecFBuffIndexId  );

    VPtRF->PtFileDecFrMap = (MIS1__TDecFrMap*) BF__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->DecFBuffIndexId  ); // 600

    // 600 VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfDecDataFileName /* FileName */, VPtRF->DecCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->FBuffDecDataId );

    VRet = BF__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfDecDataFileName /* FileName */, VPtRF->DecCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->FBuffDecDataId ); // 600

    err_retfail ( VRet, (ERR_OUT,"Abort => Load dec data file = %s, suffix = %d failed !", VPtRInf->InfDecDataFileName, VPtRF->DecCurAcqFileSuffix ) );

    // 600 VPtRF->PtAcqDecMem = (UInt8*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->FBuffDecDataId  );

    VPtRF->PtAcqDecMem = (UInt8*) BF__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->FBuffDecDataId  ); // 600
    
    
    // InfRawIndexFileName
    // InfRawDataFileName
    //
    // RawFBuffIndexId
    // RawFBuffDataId
    
    VPtRF->DecLoadedFileSuffix = VPtRF->DecCurAcqFileSuffix;
        
    // msg (( MSG_OUT, "Dec MAP : AcqInRun = %d, FrInAcq = %d, DataSz = %d, DataPos = %d", VPtRF->PtDecFrMap->ResAcqIdInRun, VPtRF->PtDecFrMap->ResFrIdInAcq, VPtRF->PtDecFrMap->ResFrSz, VPtRF->PtDecFrMap->ResFrPos ));
  }
  
  // ------------------------------------
  // Building frame list of acq
  // ------------------------------------

  // We assume that acq nb / file and fr nb / acq are constant
  // we check, if it is not the case => exit => upgrade of lib is needed to handle this case
  
  VIdInMapTableOfFirstFrOfAcq = (AcqId % VPtRPar->ParAcqNbPerFile) * VPtRPar->ParFrNbPerAcq;

  VPtDecFrMap = &MIS1__VGARunFile[ObjId].PtFileDecFrMap[VIdInMapTableOfFirstFrOfAcq];

  // Check

  if ( VPtDecFrMap->ResAcqIdInRun != AcqId ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Map table AcqIdInRun = %d <> AcqId = %d => Not constant acq/file or fr/acq => Lib upgrade neeeded, if not constant fr/acq processing can be done with VPtDecFrMap->ResFrNbInAcq ", VPtDecFrMap->ResAcqIdInRun, AcqId) );
  }

  if (  VPtDecFrMap->ResFrIdInAcq != 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Map table FrIdInAcq of first frame of acq = %d <> 0 => Not constant acq/file or fr/acq => Lib upgrade neeeded", VPtDecFrMap->ResAcqIdInRun, AcqId) );
  }

  // Build list
  
  for ( ViFrInAcq = 0; ViFrInAcq < VPtRPar->ParFrNbPerAcq; ViFrInAcq++ ) {

    VPtDecFrMap     = &MIS1__VGARunFile[ObjId].PtFileDecFrMap[VIdInMapTableOfFirstFrOfAcq + ViFrInAcq];
    VPtAcqDecFrList = &MIS1__VGARunFile[ObjId].ACurAcqDecFrList[ViFrInAcq];
        
    
    VPtAcqDecFrList->ResAcqIdInRun = VPtDecFrMap->ResAcqIdInRun;
    VPtAcqDecFrList->ResFrNbInAcq  = VPtDecFrMap->ResFrNbInAcq;
    VPtAcqDecFrList->ResFrIdInAcq  = VPtDecFrMap->ResFrIdInAcq;
    VPtAcqDecFrList->ResFrSz       = VPtDecFrMap->ResFrSz;
      
    VPtAcqDecFrList->ResPtFrBeg = (void*) ( (UInt32) VPtRF->PtAcqDecMem + VPtDecFrMap->ResFrPos );
          
  } // End for
    
    
  if ( PtPtFrList != NULL ) {
    *PtPtFrList = &MIS1__VGARunFile[ObjId].ACurAcqDecFrList[0];
  }
  
  
  err_retok (( ERR_OUT, "" ));
}


#else

SInt32 MIS1__FRunFilesDecGotoAcq ( SInt32 AcqId, MIS1__TDecFrList** PtPtFrList, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  SInt32 VIdInMapTableOfFirstFrOfAcq;
  SInt32 ViFrInAcq;
  
  MIS1__TDecFrMap*   VPtDecFrMap;
  MIS1__TDecFrList*  VPtAcqDecFrList;
  
  
  
  // Check AcqId
  
  err_retfail ( AcqId, (ERR_OUT,"Abort => AcqId = %d < 0 !", AcqId) );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf    = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar     = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf     = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus  = &MIS1__VGARunFile[ObjId].RunStatus;
  VPtDecFrMap = &MIS1__VGARunFile[ObjId].PtFileDecFrMap[0];
  
  // Calc acq info
  
  VRet = MIS1__FRunFilesDecPrivFindAcq ( AcqId, ObjId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunFilesPrivFindAcq ( AcqId = %d, ObjId = %d ) failed !", AcqId, ObjId ) );
  
  // Load the raw run files if not already in memory
  
  if ( VPtRF->DecCurAcqFileSuffix != VPtRF->DecLoadedFileSuffix ) {
    
    //
    
    VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfDecIndexFileName /* FileName */, VPtRF->DecCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->DecFBuffIndexId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Load dec index file = %s, suffix = %d failed !", VPtRInf->InfDecIndexFileName, VPtRF->DecCurAcqFileSuffix ) );
    
    VPtRF->PtFileDecFrMap = (MIS1__TDecFrMap*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->DecFBuffIndexId  );
        
    VRet = DAMIS1__FOBuffLoadBuffFromBinFile ( 0 /* BuffId */, VPtRInf->InfDecDataFileName /* FileName */, VPtRF->DecCurAcqFileSuffix, 0 /* Action */, 0 /* Mode */, VPtRF->FBuffDecDataId );
        
    err_retfail ( VRet, (ERR_OUT,"Abort => Load dec data file = %s, suffix = %d failed !", VPtRInf->InfDecDataFileName, VPtRF->DecCurAcqFileSuffix ) );
    
    VPtRF->PtAcqDecMem = (UInt8*) DAMIS1__FOBuffGetBuffPtrVoid ( 0 /* BuffId */, VPtRF->FBuffDecDataId  );
        
    
    // InfRawIndexFileName
    // InfRawDataFileName
    //
    // RawFBuffIndexId
    // RawFBuffDataId
    
    VPtRF->DecLoadedFileSuffix = VPtRF->DecCurAcqFileSuffix;
    
    // msg (( MSG_OUT, "Dec MAP : AcqInRun = %d, FrInAcq = %d, DataSz = %d, DataPos = %d", VPtRF->PtDecFrMap->ResAcqIdInRun, VPtRF->PtDecFrMap->ResFrIdInAcq, VPtRF->PtDecFrMap->ResFrSz, VPtRF->PtDecFrMap->ResFrPos ));
  }
  
  // ------------------------------------
  // Building frame list of acq
  // ------------------------------------
  
  // We assume that acq nb / file and fr nb / acq are constant
  // we check, if it is not the case => exit => upgrade of lib is needed to handle this case
  
  VIdInMapTableOfFirstFrOfAcq = (AcqId % VPtRPar->ParAcqNbPerFile) * VPtRPar->ParFrNbPerAcq;
  
  VPtDecFrMap = &MIS1__VGARunFile[ObjId].PtFileDecFrMap[VIdInMapTableOfFirstFrOfAcq];
  
  // Check
  
  if ( VPtDecFrMap->ResAcqIdInRun != AcqId ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Map table AcqIdInRun = %d <> AcqId = %d => Not constant acq/file or fr/acq => Lib upgrade neeeded, if not constant fr/acq processing can be done with VPtDecFrMap->ResFrNbInAcq ", VPtDecFrMap->ResAcqIdInRun, AcqId) );
  }
  
  if (  VPtDecFrMap->ResFrIdInAcq != 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Map table FrIdInAcq of first frame of acq = %d <> 0 => Not constant acq/file or fr/acq => Lib upgrade neeeded", VPtDecFrMap->ResAcqIdInRun, AcqId) );
  }
  
  // Build list
  
  for ( ViFrInAcq = 0; ViFrInAcq < VPtRPar->ParFrNbPerAcq; ViFrInAcq++ ) {
    
    VPtDecFrMap     = &MIS1__VGARunFile[ObjId].PtFileDecFrMap[VIdInMapTableOfFirstFrOfAcq + ViFrInAcq];
    VPtAcqDecFrList = &MIS1__VGARunFile[ObjId].ACurAcqDecFrList[ViFrInAcq];
    
    
    VPtAcqDecFrList->ResAcqIdInRun = VPtDecFrMap->ResAcqIdInRun;
    VPtAcqDecFrList->ResFrNbInAcq  = VPtDecFrMap->ResFrNbInAcq;
    VPtAcqDecFrList->ResFrIdInAcq  = VPtDecFrMap->ResFrIdInAcq;
    VPtAcqDecFrList->ResFrSz       = VPtDecFrMap->ResFrSz;
    
    VPtAcqDecFrList->ResPtFrBeg = (void*) ( (UInt32) VPtRF->PtAcqDecMem + VPtDecFrMap->ResFrPos );
    
  } // End for
  
  
  if ( PtPtFrList != NULL ) {
    *PtPtFrList = &MIS1__VGARunFile[ObjId].ACurAcqDecFrList[0];
  }
  
  
  err_retok (( ERR_OUT, "" ));
}




#endif



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesFpcGotoStep ( SInt32 StepId, MIS1__TMatPixCntU16** PtPtMatPixCnt, UInt8 ObjId = 0 )
*
* \brief   : 
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 03/07/2020
* \date    : Doc date  : 03/07/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FRunFilesFpcGotoStep ( SInt32 StepId, MIS1__TMatPixCntU16** PtPtMatPixCnt, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
 
  
  // Check param
  
  err_retfail ( StepId, (ERR_OUT,"Abort => StepId = %d < 0 !", StepId) );
   
  err_retnull ( PtPtMatPixCnt, (ERR_OUT,"Abort => PtPtMatPixCnt == NULL") );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Step info
    
  VRet = MIS1__FRunFilesFpcPrivFindStep ( StepId, ObjId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunFilesFpcPrivFindStep ( StepId = %d, ObjId = %d ) failed !", StepId, ObjId ) );
  
  // Load the fpc run files if not already in memory
  
  if ( VPtRF->FpcCurStepFileSuffix != VPtRF->FpcStepLoadedFileSuffix ) {
    
    // Build file name
   
    VRet = MIS1__FBuildFileNameSuffix ( VPtRInf->InfFPCStepFileName, VPtRInf->InfCurFPCStepFileName, GLB_FILE_PATH_SZ, VPtRF->FpcCurStepFileSuffix );
    
    err_retfail ( VRet, (ERR_OUT, "Building FPC file name base = %s, prefix = %d failed", VPtRInf->InfFPCStepFileName, VPtRF->FpcCurStepFileSuffix ) );
    
    // Open file
    
    VRet = VPtRF->PtFPCStepBinFile->PubFSetFileName ( VPtRInf->InfCurFPCStepFileName );
    
    err_retfail ( VRet, (ERR_OUT,"Set FPC step file name = %s to TCBinFile failed !", VPtRInf->InfCurFPCStepFileName) );
    
    VRet = VPtRF->PtFPCStepBinFile->PubFOpen ();

    err_retfail ( VRet, (ERR_OUT,"Open FPC step file name = %s to TCBinFile failed !", VPtRInf->InfCurFPCStepFileName) );
    
    // Read 
    
    VRet = VPtRF->PtFPCStepBinFile->PubFBlocRead ( 0 /* BlocNo */, &VPtRF->StepMatPixCnt, sizeof (MIS1__TMatPixCntU16) );

    err_retfail ( VRet, (ERR_OUT,"Read FPC step file name = %s to TCBinFile failed !", VPtRInf->InfCurFPCStepFileName) );
    
        
    VPtRF->FpcStepLoadedFileSuffix = VPtRF->FpcCurStepFileSuffix;
    
  }
  
  
  *PtPtMatPixCnt = &VPtRF->StepMatPixCnt;
    
  
  return (0);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesFpcGotoSubStep ( SInt32 StepId, SInt32 SubStepId, MIS1__TMatPixCntU16** PtPtMatPixCnt, UInt8 ObjId = 0 )
*
* \brief   :
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 06/07/2020
* \date    : Doc date  : 06/07/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__FRunFilesFpcGotoSubStep ( SInt32 StepId, SInt32 SubStepId, MIS1__TMatPixCntU16** PtPtMatPixCnt, UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles*  VPtRF;
  MIS1__TRunConf*   VPtRConf;
  MIS1__TRunPar*    VPtRPar;
  MIS1__TRunInf*    VPtRInf;
  MIS1__TRunStatus* VPtRStatus;
  
  
  
  // Check param
  
  err_retfail ( StepId, (ERR_OUT,"Abort => StepId = %d < 0 !", StepId) );

  err_retfail ( SubStepId, (ERR_OUT,"Abort => SubStepId = %d < 0 !", SubStepId) );
  
  err_retnull ( PtPtMatPixCnt, (ERR_OUT,"Abort => PtPtMatPixCnt == NULL") );
  
  
  // Set ptr to record
  
  VPtRF      = &MIS1__VGARunFile[ObjId];
  
  // Check if MIS1__FRunFilesOpen () has been called
  
  if ( VPtRF->FOpenDone == 0) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FRunFilesOpen () has not been called !") );
  }
  
  VPtRConf   = &MIS1__VGARunFile[ObjId].RunConf;
  VPtRPar    = &MIS1__VGARunFile[ObjId].RunConf.RunPar;
  VPtRInf    = &MIS1__VGARunFile[ObjId].RunConf.RunInf;
  VPtRStatus = &MIS1__VGARunFile[ObjId].RunStatus;
  
  // Step, sub step info
  
  VRet = MIS1__FRunFilesFpcPrivFindSubStep ( StepId, SubStepId, ObjId );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => MIS1__FRunFilesFpcPrivFindSubStep ( StepId = %d, SubStepId = %d, ObjId = %d ) failed !", StepId, SubStepId, ObjId ) );
  
  // Load the fpc run files if not already in memory
  
  if ( VPtRF->FpcCurSubStepFileSuffix != VPtRF->FpcSubStepLoadedFileSuffix ) {
    
    // Build file name
    
    VRet = MIS1__FBuildFileNameSuffix ( VPtRInf->InfFPCSubStepsFileName, VPtRInf->InfCurFPCSubStepsFileName, GLB_FILE_PATH_SZ, VPtRF->FpcCurSubStepFileSuffix );
    
    err_retfail ( VRet, (ERR_OUT, "Building FPC sub step file name base = %s, prefix = %d failed", VPtRInf->InfFPCSubStepsFileName, VPtRF->FpcCurSubStepFileSuffix ) );
    
    // Open file
    
    VRet = VPtRF->PtFPCSubStepsBinFile->PubFSetFileName ( VPtRInf->InfCurFPCSubStepsFileName );
    
    err_retfail ( VRet, (ERR_OUT,"Set FPC sub step file name = %s to TCBinFile failed !", VPtRInf->InfCurFPCSubStepsFileName) );
    
    VRet = VPtRF->PtFPCSubStepsBinFile->PubFOpen ();
    
    err_retfail ( VRet, (ERR_OUT,"Open FPC sub step file name = %s to TCBinFile failed !", VPtRInf->InfCurFPCSubStepsFileName) );    
        
    VPtRF->FpcSubStepLoadedFileSuffix = VPtRF->FpcCurSubStepFileSuffix;
    
  }
  

  // Read
    
  VRet = VPtRF->PtFPCSubStepsBinFile->PubFBlocRead ( SubStepId /* BlocNo */, &VPtRF->SubStepMatPixCnt, sizeof (MIS1__TMatPixCntU16) );
    
  err_retfail ( VRet, (ERR_OUT,"Read FPC sub step = %d of step = %d, file name = %s to TCBinFile failed !", SubStepId, StepId, VPtRInf->InfCurFPCSubStepsFileName) );
    

  *PtPtMatPixCnt = &VPtRF->SubStepMatPixCnt;
  
  
  return (0);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunFilesEnd ( UInt8 ObjId = 0 )
*
* \brief   : Close run files records, objects, needed to access to run files
*
* \param   : ObjId - Id of the RunFiles instance, up to MIS1__MAX_RUN_FILES - 1
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 25/05/2020
* \date    : Doc date  : 25/05/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

#ifdef MIS1__CC_DATA_FORMAT_SINCE_V211

SInt32 MIS1__FRunFilesEnd ( UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles* VPtRF;
  
  // Check param
  
  if ( ObjId >= MIS1__MAX_RUN_FILES ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ObjId = %d out of range [0..%d]", ObjId, MIS1__MAX_RUN_FILES - 1) );
  }
  
  // Set ptr to record
  
  VPtRF = &MIS1__VGARunFile[ObjId];
  
  
  VPtRF->FBeginDone = 0;
  
 
  // Free RAW data files objects
  // - Index file
  
  // 600 VRet = DAMIS1__FOBuffAllocFree ( 0 /* FreeAlloc */, 0 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffIndexId );

  VRet = BF__FOBuffAllocFree ( 0 /* FreeAlloc */, 0 /* BuffW8Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffIndexId ); // 600

  err_retfail ( VRet, (ERR_OUT,"Abort => Free OBuff for raw index file, ObjId = %d failed !", ObjId) );
  
  // Free RAW data files objects
  // - Data file
  
  // 600 VRet = DAMIS1__FOBuffAllocFree ( 0 /* FreeAlloc */, 0 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffDataId );

  VRet = BF__FOBuffAllocFree ( 0 /* FreeAlloc */, 0 /* BuffW8Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffDataId ); // 600

  err_retfail ( VRet, (ERR_OUT,"Abort => Free OBuff for raw index file, ObjId = %d failed !", ObjId) );
  
  
  // Free FPC TCBinFiles
  
  VPtRF->PtFPCSubStepsBinFile->~FIL__TCBinFile ();
    
  VPtRF->PtFPCStepBinFile->~FIL__TCBinFile ();
  
  // Reset record
  
  memset ( VPtRF, 0, sizeof (MIS1__TRunFiles) );
  
  
  err_retok (( ERR_OUT, "" ));
}


#else

SInt32 MIS1__FRunFilesEnd ( UInt8 ObjId = 0 ) {
  
  SInt32 VRet;
  MIS1__TRunFiles* VPtRF;
  
  // Check param
  
  if ( ObjId >= MIS1__MAX_RUN_FILES ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ObjId = %d out of range [0..%d]", ObjId, MIS1__MAX_RUN_FILES - 1) );
  }
  
  // Set ptr to record
  
  VPtRF = &MIS1__VGARunFile[ObjId];
  
  
  VPtRF->FBeginDone = 0;
  
  
  // Free RAW data files objects
  // - Index file
  
  VRet = DAMIS1__FOBuffAllocFree ( 0 /* FreeAlloc */, 0 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffIndexId );
    
  err_retfail ( VRet, (ERR_OUT,"Abort => Free OBuff for raw index file, ObjId = %d failed !", ObjId) );
  
  // Free RAW data files objects
  // - Data file
  
  VRet = DAMIS1__FOBuffAllocFree ( 0 /* FreeAlloc */, 0 /* BuffW64Sz */, 0 /* BuffIdToAlloc */, 0 /* BuffIdToFree */, VPtRF->RawFBuffDataId );
    
  err_retfail ( VRet, (ERR_OUT,"Abort => Free OBuff for raw index file, ObjId = %d failed !", ObjId) );
  
  // Reset record
  
  memset ( VPtRF, 0, sizeof (MIS1__TRunFiles) );
  
  
  err_retok (( ERR_OUT, "" ));
}


#endif


/**
===================================================================================
* \fn      : SInt32 MIS1__FProcDec ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile,  SInt32 AcqIdInBuff,  SInt8 PrintLvl )
*
* \brief   : Executes decode pixels processing
*
* \param   : FirstAcqOfRun - Flag, 1 => It is the first acq of the run
*
* \param   : PtAcqProc     - Pointer to acq processing record
*
* \param   : ProcMode      - Processing mode
*
* \param   : AcqIdInRun    - Id of acq in run 0 to acq nb in run - 1
*
* \param   : AcqIdInFile   - Id of acq in one file of the run, if characterization mode => acq id in current step
*
* \param   : AcqIdInBuff   - Id of acq in the bloc of acq bufferiezd in mem = for step file
*
* \param   : PrintLvl      - Print level
*
* \return  : Error code  \n
*          :   0  - OK   \n
*          : < 0 - Error \n
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 08/06/2020
* \date    : Rev       : 30/06/2020 \n
*                        - Reduced paramters list, moved to MIS1__TAcqProc
* \date    : Doc date  : 08/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */
  
SInt32 MIS1__FProcDec ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile,  SInt32 AcqIdInBuff,  SInt8 PrintLvl ) {
  
  MIS1__TRunPar*    VPtRunPar;
  MIS1__TRunInf*    VPtRunInf;
  MIS1__TRunStatus* VPtRunStatus;  
  
  // Variables for parameters removed on 30/06/2020, acces is now done via MIS1__TAcqProc
  
  SInt32           VAcqNbPerFile;
  SInt32           VAcqNbPerSubStep;
  SInt32           VAcqNbPerBuff;
  FIL__TCBinFile*  VPtStepBinFile;
  FIL__TCBinFile*  VPtSubStepBinFile;

  
    
  
  // Check param
  
  err_retnull ( PtAcqProc, (ERR_OUT,"Abort => PtAcqProc == NULL") );
  

  // Get some parameters
    
  VPtRunPar           = PtAcqProc->PtRunPar;
  VPtRunInf           = PtAcqProc->PtRunInf;
  VPtRunStatus        = PtAcqProc->PtRunStatus;
  
  VAcqNbPerFile        = PtAcqProc->PtRunPar->ParAcqNbPerFile;
  VAcqNbPerSubStep    = PtAcqProc->PtRunPar->ParCarSubStepAcqNb;
  VAcqNbPerBuff        = PtAcqProc->PtRunPar->ParBufferedAcqNb;
  VPtStepBinFile      = PtAcqProc->PtOStepFile;
  VPtSubStepBinFile   = PtAcqProc->PtOSubStepsFile;

  
  // Check
  
  if ( AcqIdInBuff >= VAcqNbPerBuff ) {
    err_retfail ( -1, (ERR_OUT,"Abort => AcqIdInBuff >= VAcqNbPerBuff", AcqIdInBuff, VAcqNbPerBuff) );
  }
  
  
  
  
  // Processing

  if ( PrintLvl > 0 ) {
    msg (( MSG_OUT, "Proc Dec : AcqIdInRun = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d ", AcqIdInRun, AcqIdInBuff, VAcqNbPerBuff ));
  }

  
  return (0);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FEmulDecFrPix ( MIS1__TDecFrPix* PtDest, SInt32 PixelsNb )
*
* \brief   : Emulate PixelsNb pixels, header fields are not modified
*
* \param   : PtSrc    - Pointer to the header
*
\param     : PixelsNb - Number of pixels to emulate
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    : 
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/06/2020
* \date    : Doc date  : 11/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FEmulDecFrPixPatt0 ( MIS1__TDecFrPix* PtDest, SInt32 PixelsNb ) {
  
  SInt32 ViPix;

  
  // Check param
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtSrc == NULL") );
  
 
  // Set pixels
  
  
  for ( ViPix=0; ViPix < PixelsNb; ViPix++ ) {
    
    PtDest->APix[ViPix].C.y = ViPix / 1024; // MSis 1 = 1024 col
    
    PtDest->APix[ViPix].C.x = ViPix % 1024; // MSis 1 = 504 lines
           
  }
  
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixCntU16ToMatPixS32 ( MIS1__TMatPixCntU16* PtSrcMatPixCntU16, MIS1__TMatPixS32* PtDestMatPixS32 )
*
* \brief   : Copy a MIS1__TMatPixCntU16 to a MIS1__TMatPixS32
*
* \param   : PtSrcMatPixCntU16 - Pointer to source
*
* \param   : PtDestMatPixS32   - Pointer to destination
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/08/2020
* \date    : Doc date  : 13/08/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixCntU16ToMatPixS32 ( MIS1__TMatPixCntU16* PtSrcMatPixCntU16, MIS1__TMatPixS32* PtDestMatPixS32 ) {
  
  SInt32 VRow;
  SInt32 VCol;
  
  
  // Check param
  
  err_retnull ( PtSrcMatPixCntU16, (ERR_OUT,"Abort => PtSrcMatPixCntU16 == NULL") );

  err_retnull ( PtDestMatPixS32, (ERR_OUT,"Abort => PtDestMatPixS32 == NULL") );
  

  // Copy
  //
  // For info, types definition
  //
  // typedef UInt16 MIS1__TTMatPixCntU16[MIS1__COL_NB][MIS1__ROW_NB];
  // typedef MIS1__TTMatPixCntU16 MIS1__TMatPixCntU16;
  //
  // typedef SInt32 MIS1__TTMatPixS32[MIS1__COL_NB][MIS1__ROW_NB] ;
  // typedef MIS1__TTMatPixS32 MIS1__TMatPixS32;

  
  for ( VRow = 0; VRow < MIS1__ROW_NB; VRow++ ) {
    
    for ( VCol = 0; VCol < MIS1__COL_NB; VCol++ ) {
      
      (*PtDestMatPixS32)[VCol][VRow] = (*PtSrcMatPixCntU16)[VCol][VRow];
      
    }  // End for ( VCol )
    
  } // End for ( VRow )


  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32ToMatPixCntU16 ( MIS1__TMatPixS32* PtSrcMatPixS32, MIS1__TMatPixCntU16* PtDestMatPixCntU16, SInt8 ListOvf )
*
* \brief   : Copy a MIS1__TMatPixS32 to MIS1__TMatPixCntU16 \n
*            It checks for source pixel value overflow, if < 0 or > 65535 \n
*            sets destination pixel value to 65535
*
* \param   : PtSrcMatPixS32      - Pointer to source
*
* \param   : PtDestMatPixCntU16  - Pointer to destination
*
* \param   : ListOvf - 1 => Generate an error message for pixel value overflow
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - SW Error  \n
*          : > 0 - SW pixels value overflow => Warning / Error ?  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/08/2020
* \date    : Doc date  : 13/08/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixS32ToMatPixCntU16 ( MIS1__TMatPixS32* PtSrcMatPixS32, MIS1__TMatPixCntU16* PtDestMatPixCntU16, SInt8 ListOvf ) {
  
  SInt32 VRow;
  SInt32 VCol;
  SInt32 VSrcPixVal;
  UInt16 VDestPixVal;
  SInt32 VOvfCnt;
  
  
  // Check param
  
  err_retnull ( PtSrcMatPixS32, (ERR_OUT,"Abort => PtSrcMatPixS32 == NULL") );
  
  err_retnull ( PtDestMatPixCntU16, (ERR_OUT,"Abort => PtDestMatPixCntU16 == NULL") );
  
  
  // Copy
  //
  // For info, types definition
  //
  // typedef UInt16 MIS1__TTMatPixCntU16[MIS1__COL_NB][MIS1__ROW_NB];
  // typedef MIS1__TTMatPixCntU16 MIS1__TMatPixCntU16;
  //
  // typedef SInt32 MIS1__TTMatPixS32[MIS1__COL_NB][MIS1__ROW_NB] ;
  // typedef MIS1__TTMatPixS32 MIS1__TMatPixS32;
  
  
  VOvfCnt = 0;
  
  for ( VRow = 0; VRow < MIS1__ROW_NB; VRow++ ) {
    
    for ( VCol = 0; VCol < MIS1__COL_NB; VCol++ ) {
      
      VSrcPixVal = (*PtSrcMatPixS32)[VCol][VRow];
            
      while (1) {
        
        if ( VSrcPixVal < 0 ) {
          
          VDestPixVal = 65535;
          ++VOvfCnt;
          
          if ( ListOvf ) {
            err_error (( ERR_OUT, "Overflox Pix[x=%.4d,y=%.4d] = %d not in range 0..65535", VCol, VRow, VSrcPixVal ));
          }
          
          break;
        }
      
        
        if ( VSrcPixVal > 65535 ) {
          
          VDestPixVal = 65535;
          ++VOvfCnt;

          if ( ListOvf ) {
            err_error (( ERR_OUT, "Overflox Pix[x=%.4d,y=%.4d] = %d not in range 0..65535", VCol, VRow, VSrcPixVal ));
          }          
          
          break;
        }
      
        VDestPixVal = VSrcPixVal;
        break;
        
      } // End while (1)
      
      (*PtDestMatPixCntU16)[VCol][VRow] = VDestPixVal;
           
    } // End for ( VCol )
    
  } //  End for ( VRow )
  
    
  if ( VOvfCnt > 0 ) {
    err_error (( ERR_OUT, "Copy done, but %d pixels with ovf on source value ( < 0 or > 65535 ) => set to 65535 in destination", VOvfCnt ));
    return (VOvfCnt);
  }
  
  err_retok (( ERR_OUT, "" ));
}


// 88888


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32AddV1 ( MIS1__TMatPixS32* PtSrc, MIS1__TMatPixS32* PtDest )
*
* \brief   : Add content of matrix PtSrc to matrix PtDest \n
*            This version No 1 of function add all items of source matrix  \n
*
* \param   : PtSrc     - Pointer to source
*
* \param   : PtDest  - Pointer to destination
*
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - SW Error  \n
*          : > 0 - SW pixels value overflow => Warning / Error ?  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 15/108/2020
* \date    : Doc date  : 15/108/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixS32AddV1 ( MIS1__TMatPixS32* PtSrc, MIS1__TMatPixS32* PtDest ) {

  SInt32 VRow;
  SInt32 VCol;
  SInt32 VSrcPixVal;
  SInt32 VDestPixVal;
  
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  
  // Add
  //
  // For info, types definition
  //
  // typedef SInt32 MIS1__TTMatPixS32[MIS1__COL_NB][MIS1__ROW_NB] ;
  // typedef MIS1__TTMatPixS32 MIS1__TMatPixS32;
  
  
  
  for ( VRow = 0; VRow < MIS1__ROW_NB; VRow++ ) {
    
    for ( VCol = 0; VCol < MIS1__COL_NB; VCol++ ) {
      
      VSrcPixVal = (*PtSrc)[VCol][VRow];
      
      VDestPixVal = (*PtDest)[VCol][VRow];
      
      (*PtDest)[VCol][VRow] = VDestPixVal + VSrcPixVal;
                  
    } // End for ( VCol )
    
  } //  End for ( VRow )
  
    
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixS32AddV2 ( MIS1__TMatPixS32* PtSrc, MIS1__TMatPixS32* PtDest )
*
* \brief   : Add content of matrix PtSrc to matrix PtDest \n
*            This version No 2 of function add only items of source matrix != 0 \n
*            with the hope that it will save execution time => TBC
*
* \param   : PtSrc     - Pointer to source
*
* \param   : PtDest  - Pointer to destination
*
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - SW Error  \n
*          : > 0 - SW pixels value overflow => Warning / Error ?  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 15/108/2020
* \date    : Doc date  : 15/108/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixS32AddV2 ( MIS1__TMatPixS32* PtSrc, MIS1__TMatPixS32* PtDest ) {
  
  SInt32 VRow;
  SInt32 VCol;
  SInt32 VSrcPixVal;
  SInt32 VDestPixVal;
  
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  
  // Add
  //
  // For info, types definition
  //
  // typedef SInt32 MIS1__TTMatPixS32[MIS1__COL_NB][MIS1__ROW_NB] ;
  // typedef MIS1__TTMatPixS32 MIS1__TMatPixS32;
  
  
  
  for ( VRow = 0; VRow < MIS1__ROW_NB; VRow++ ) {
    
    for ( VCol = 0; VCol < MIS1__COL_NB; VCol++ ) {
      
      VSrcPixVal = (*PtSrc)[VCol][VRow];
      
      if ( VSrcPixVal != 0 ) {
        VDestPixVal = (*PtDest)[VCol][VRow];
        (*PtDest)[VCol][VRow] = VDestPixVal + VSrcPixVal;
      }
           
      
    } // End for ( VCol )
    
  } //  End for ( VRow )
  
  
  err_retok (( ERR_OUT, "" ));
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixCntU16Save ( MIS1__TMatPixCntU16* PtSrc, char* FileName )
*
* \brief   : Save MIS1__TMatPixCntU16 to file
*
* \param   : PtSrc    - Pointer to source matrix
*
* \param   : FileName - File name        
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/08/2020
* \date    : Doc date  : 13/08/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixCntU16Save ( MIS1__TMatPixCntU16* PtSrc, char* FileName ) {
  
  SInt32 VRet;
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  
  // Save 
  
  VRet = FIL_FWriteRecord ( FileName, PtSrc, sizeof (MIS1__TMatPixCntU16) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => FIL_FWriteRecord (PtSrc, File=%s) Ret = %d", FileName, VRet) );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixCntU16Load ( MIS1__TMatPixCntU16* PtDest, char* FileName )
*
* \brief   : Load MIS1__TMatPixCntU16 from file
*
* \param   : PtDest   - Pointer to destination matrix
*
* \param   : FileName - File name
*
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 13/08/2020
* \date    : Doc date  : 13/08/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixCntU16Load ( MIS1__TMatPixCntU16* PtDest, char* FileName ) {
  
  SInt32 VRet;
  SInt32 VExpectedFileSz;
  
  
  // Check param
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  
  // Check file format via file size
  
  VRet = FIL_FFileSize ( FileName );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => FIL_FFileSize (File=%s) failed Ret = %d", FileName, VRet) );
  
  // Check size
  
  VExpectedFileSz = sizeof (MIS1__TMatPixCntU16);
  
  if ( VRet != VExpectedFileSz ) {
    err_retfail ( -1, (ERR_OUT,"Abort => File size = %d <> MIS1__TMatPixCntU16 = %d => Bad file format, S32 instead of U16 ?", VRet, VExpectedFileSz ) );
  }
  
  
  // Load
    
  VRet = FIL_FReadRecord ( FileName, PtDest, sizeof (MIS1__TMatPixCntU16) );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => FIL_FReadRecord (PtDest, File=%s) Ret = %d", FileName, VRet) );
  
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixCntU16Rst ( MIS1__TMatPixCntU16* PtDestFpc )
*
* \brief   : Reset mat pix count
*
* \param   : PtSrc    - Pointer to the header
*
\param     : PixelsNb - Number of pixels to emulate
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 23/06/2020
* \date    : Doc date  : 23/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FMatPixCntU16Rst ( MIS1__TMatPixCntU16* PtDestFpc ) {
  
  SInt32 VPixX;
  SInt32 VPixY;

  // Check param

  err_retnull ( PtDestFpc, (ERR_OUT,"Abort => PtDestFpc == NULL") );
  
  // MIS1__TTMatPixCntU16[MIS1__COL_NB][MIS1__ROW_NB];
  
  for ( VPixY = 0; VPixY < MIS1__ROW_NB; VPixY++ ) {
    
    for ( VPixX = 0; VPixX < MIS1__COL_NB; VPixX++ ) {
      (*PtDestFpc)[VPixX][VPixY] = 0;
    }
    
  } // End for ( VPixY )
  

}

  
/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FMatPixCntU16Print ( MIS1__TMatPixCntU16* PtSrc, SInt32 CountTh, char* Title, SInt8 Print, SInt32 ExitAFterNPrints, SInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*
* \brief   : Count pixels from MIS1__TMatPixCntU16 with count > CountTh, print them if Print == 1
*
* \param   : PtSrc       - Pointer to the header
*
* \param   : CountTh     - List pixels with hit count > PrintTh, print only if Print == 1
*
* \param   : Print       - 0 => No print, 1 => Print
*
* \param   : ExitAFterNPrints - Exit function after N pixels print in order to not lock the system \n
*                               n <= 0 => Not used, print all pixels \n
*                               n  > 0 => Exit after printing n pixels
*
* \param   : PrintDest - Print destination, \n
*          :   0 => No print, \n
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo \n
*          :   4 => text file, 5 => Text file + Memo \n
*
* \return  : Error code   \n
*          : >= 0 - Number of pixel with counter > PrintTh \n
*          : <  0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    : 
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/06/2020
* \date    : Doc date  : 11/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



SInt32 MIS1__FMatPixCntU16Print ( MIS1__TMatPixCntU16* PtSrc, SInt32 CountTh, char* Title, SInt8 Print, SInt32 ExitAFterNPrints, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {
  
  SInt32 VxPix;
  SInt32 VyPix;
  SInt32 VPixNb;
  UInt16 VValPix;
  char   VMsg[GLB_CMT_SZ];
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  

  // Print all pixels detected at 0 / 1
  
  if ( ExitAFterNPrints <= 0 ) {
    ExitAFterNPrints = MIS1__COL_NB * MIS1__ROW_NB;
  }
  
  
    
  sprintf ( VMsg, "Print pixels with counter > %d - Cmt : %s", CountTh, Title );
  
  MIS1__FPrintStr ( "================================================", PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( "================================================", PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( "", PrintDest, Memo, PtFile );
  
  // MIS1__TMatPixCntU16[MIS1__COL_NB][MIS1__ROW_NB]
  
  VPixNb = 0;
  
  for ( VyPix = 0; VyPix < MIS1__ROW_NB; VyPix++ ) {
  
    for ( VxPix = 0; VxPix < MIS1__COL_NB; VxPix++ ) {
      
      VValPix = (*PtSrc)[VxPix][VyPix];
      
      if ( VValPix > CountTh ) {
        
        if ( Print == 1 ) {
          sprintf ( VMsg, "Pixel [%.4d] = X=%.4d, Y=%.4d => %.4d", VPixNb, VxPix, VyPix, VValPix );
          MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
        }
           
        VPixNb++;
        
        if ( VPixNb > ExitAFterNPrints ) {
          msg (( MSG_OUT, "Abort before end => Max pix nb to print = %d reached", ExitAFterNPrints ));
          MIS1__FPrintStr ( MSG_OUT, PrintDest, Memo, PtFile );
          err_retfail ( -1, (ERR_OUT,"%s", MSG_OUT) );          
        }
        
      
      } // End if ( VValPix > PrintTh )
      
    } // End for ( VxPix = 0; VxPix < MIS1__COL_NB; VxPix++ )

    
  } // End for ( VyPix = 0; VyPix < ; VyPix++ )
  
    
  
  return (VPixNb);
}

  


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FEmulDecFrPixFPC ( MIS1__TDecFrPix* PtDest, SInt32 PixelsNb )
*
* \brief   : Emulate PixelsNb pixels, header fields are not modified
*
* \param   : PtSrc    - Pointer to the header
*
\param     : PixelsNb - Number of pixels to emulate
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 23/06/2020
* \date    : Doc date  : 23/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



SInt32 MIS1__FEmulDecFpcFrPixPatt0 ( MIS1__TDecFrPix* PtDestDec, MIS1__TMatPixCntU16* PtDestFpcSubStep, MIS1__TMatPixCntU16* PtDestFpcStep, SInt32 PixelsNb ) {

  SInt32 ViPix;
  SInt32 VPixX;
  SInt32 VPixY;
  
  
  // Check param
  
  err_retnull ( PtDestDec, (ERR_OUT,"Abort => PtDestDec == NULL") );
  
  err_retnull ( PtDestFpcSubStep, (ERR_OUT,"Abort => PtDestFpcSubStep == NULL") );

  err_retnull ( PtDestFpcStep, (ERR_OUT,"Abort => PtDestFpcStep == NULL") );

  // Set pixels
  
  
  for ( ViPix=0; ViPix < PixelsNb; ViPix++ ) {
    
    VPixY = ViPix / 1024; // MSis 1 = 1024 col    
    VPixX = ViPix % 1024; // MSis 1 = 504 lines
    
    PtDestDec->APix[ViPix].C.y = VPixY;
    PtDestDec->APix[ViPix].C.x = VPixX;
    
    // MIS1__TTMatPixCntU16[MIS1__COL_NB][MIS1__ROW_NB];
    
    (*PtDestFpcSubStep)[VPixX][VPixY]++;
    (*PtDestFpcStep)[VPixX][VPixY]++;

  }
  
  
  return (0);
}




/**
===================================================================================
* \fn      : SInt32 MIS1__FProcEmulDec ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile,  SInt32 AcqIdInBuff,  SInt8 PrintLvl )
*
* \brief   : Execute emul decode pixels processing fro DAQ tests
*
* \param   : FirstAcqOfRun    - Flag, 1 => It is the first acq of the run
*
* \param   : PtAcqProc        - Pointer to acq processing record
*
* \param   : ProcMode         - Processing mode
*
* \param   : AcqIdInRun       - Id of acq in run 0 to acq nb in run - 1
*
* \param   : AcqIdInFile      - Id of acq in one file of the run, if characterization mode => acq id in current step
*
* \param   : AcqIdInBuff      - Id of acq in the bloc of acq bufferiezd in mem = for step file
*
* \param   : PrintLvl         - Print level
*
* \return  : Error code  \n
*          :   0  - OK   \n
*          : < 0 - Error \n
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 09/06/2020
* \date    : Rev       : 30/06/2020 \n
*                        - Reduced paramters list, moved to MIS1__TAcqProc
* \date    : Doc date  : 09/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FProcEmulDec ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile,  SInt32 AcqIdInBuff,  SInt8 PrintLvl ) {

  
  // =======================================================================================================
  // Info on processinf method, data source, destination and buffers
  // =======================================================================================================
  // - A number of acq are stored in a memory buffer, this number is defined via the GUI param "Buffered acq nb"
  // - The processing can be done acquisition by acquisition or only when this memory buffer is fulled.
  //
  // - This function is called for each acquisition in the DAQ loop, it knows
  //
  // -- When the run starts by testing  FirstAcqOfRun == 1
  // -- The processing mode via ProcMode, you can use it to check if this function is not called by error ;-)
  // -- The id of the acq in the Run via AcqIdInRun, starting at 0
  // -- The id of the acq in the current file via AcqIdInFile, starting at 0
  // -- The id of the acq in the memory buffer via AcqIdInBuff, starting at 0
  // -- When filling of the memory buffer starts by testing AcqIdInBuff == 0
  // -- The maximum number of acq in the memory buffer via VAcqNbPerBuff
  // -- The acq number per file via VAcqNbPerFile
  // -- The acq number per sub step VAcqNbPerSubStep
  //
  // Some of the above info avec function parameters ( FirstAcqOfRun, AcqIdInRun, ...) they contain updated
  // values since the beginning of the functiuon, others are variables (VAcqNbPerBuff, VAcqNbPerFile ... identifier
  // starting by V) their value is only updated after their intialization block.
  //
  // - Any other run param, info, status can be accessed via four pointers
  // -- PtAcqProc, available since beginning of function
  // -- VPtRunPar, available after initialzation block
  // -- VPtRunInf, available after initialzation block
  // -- VPtRunStatus, available after initialzation block
  //
  // - The source data are organized as an array of Acq, because the FlexRIo returns data acq by acq,
  //   the destination data are function of the processing goal => different for each processing function
  //
  // - The source data for the processing and destination results buffers are already allocated and
  //   access is done via pointers fields in the MIS1__TAcqProc record, local variables are set here for this
  //
  // -- VPtAcqFrList = Source acq, VPtAcqFrList[n] = acq n in memory buffer, subfields give access to frames
  //    n = 0 to AcqNbInBuff - 1
  //
  // -- VPtDecFrPix = Destination result, per frame, VPtDecFrPix[m] = pixels list for frame m
  //    m = 0 to ( MIS1__MAX_BUFFERED_ACQ_NB * MIS1__MAX_FR_NB_PER_ACQ ) - 1, buffer should be big enough
  //    but it is better to check that no access out of buffer space is done.
  //    As the pixels number can vary each VPtDecFrPix record has a fixed size header MIS1__TDecFrPixHeader
  //    and a variable pixels list field APix, size information MUST be set at two places via header field
  //    VPtFrMapDec and the records VPtFrListDec, VPtFrMapDec corresponding to the frame.
  //
  // -- Records VPtFrListDec, VPtFrMapDec contains information needed to access to the result records of
  //    processing function, because results records have a variable length. VPtFrListDec, VPtFrMapDec
  //    MUST be updated by THIS FUNCTION after processing of each frame. The record VPtFrListDec gives
  //    pointers to results record + some info, the VPtFrMapDec gives same info by pointer are converted
  //    to offset needed to create the index file.
  //
  // - The results of the processing, stored in buffer VPtDecFrPix are saved (if enabled) in a file at the
  //   end of the processing (after processing of AcqNbInBuff - 1)


  MIS1__TRunPar*    VPtRunPar;
  MIS1__TRunInf*    VPtRunInf;
  MIS1__TRunStatus* VPtRunStatus;
  
  // Variables for parameters removed on 30/06/2020, acces is now done via MIS1__TAcqProc
  
  SInt32           VAcqNbPerFile;
  SInt32           VAcqNbPerSubStep;
  SInt32           VAcqNbPerBuff;
  FIL__TCBinFile*  VPtStepBinFile;
  FIL__TCBinFile*  VPtSubStepBinFile;
  


         SInt32 VCurAcqFrNb; // Fr nb in the current acq processed by this function
         SInt32 VCurAcqFrId; // Fr id fro scanning frame of the current acq processed by this function
  static SInt32 VDestFrId;   // Result, destination Fr id for indexing destination array
  static SInt32 VFrIdInBuff; // Fr id in the memory buffer of size "Buffered acq nb" in GUI
  static SInt32 VFrIdInRun;  // Fr id since beginning of the run
  static UInt32 VFrPos;      // Position in W8 / beginning of memory buffer = / beginning of file
                              // One memory buffer = "Buffered acq nb" = One file, One Run = Many files
                              
         SInt32 VPixNbInFr;
    
    
  MIS1__TAcqProc*    VPtAcqProc;    // Acquisition processing record, pointers fields to data buffers
  MIS1__TAcqFrList*  VPtAcqFrList;  // List of acq, frames in the source acq to processed
  MIS1__TAcqFrMap*   VPtAcqFrMap;   // Mapping of acq, frames (positions for index file) in the source acq to processed
  MIS1__TDecFrList*  VPtFrListDec;  // List of frames in the destination buffer (already allocated) 
  MIS1__TDecFrMap*   VPtFrMapDec;   // Mapping of frames (positions for index file) in the destination buffer (already allocated)
  MIS1__TDecFrPix*   VPtDecFrPix;   // Destination decoded pixels list for each frame, (already allocated)
  
    
  
  // Check param
  
  err_retnull ( PtAcqProc, (ERR_OUT,"Abort => PtAcqProc == NULL") );
  
  // Get some parameters
  
  VPtRunPar           = PtAcqProc->PtRunPar;
  VPtRunInf           = PtAcqProc->PtRunInf;
  VPtRunStatus        = PtAcqProc->PtRunStatus;
  
  VAcqNbPerFile        = PtAcqProc->PtRunPar->ParAcqNbPerFile;
  VAcqNbPerSubStep    = PtAcqProc->PtRunPar->ParCarSubStepAcqNb;
  VAcqNbPerBuff        = PtAcqProc->PtRunPar->ParBufferedAcqNb;
  VPtStepBinFile      = PtAcqProc->PtOStepFile;
  VPtSubStepBinFile   = PtAcqProc->PtOSubStepsFile;
  
  
  
  if ( AcqIdInBuff >= VAcqNbPerBuff ) {
    err_retfail ( -1, (ERR_OUT,"Abort => AcqIdInBuff >= VAcqNbPerBuff", AcqIdInBuff, VAcqNbPerBuff) );
  }
  
  // Set ptr
  
  VPtAcqProc   = PtAcqProc;                              // Acquisition processing : scan to find frames list, count fired pixels, etc
  VPtAcqFrList = &PtAcqProc->AAcqFrListRaw[AcqIdInBuff]; // List of frames in one acq - Now ONLY ONE acq used
  VPtAcqFrMap  = &PtAcqProc->AAcqFrMapRaw[AcqIdInBuff];  // List of frames in one acq - Now ONLY ONE acq used

  // Processing
  
  if ( PrintLvl == 2 ) {
    msg (( MSG_OUT, "Proc Dec : AcqIdInRun = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d ", AcqIdInRun, AcqIdInBuff, VAcqNbPerBuff ));
  }
  
  // Detect beginning of the run = First acq of the run
  
  if ( FirstAcqOfRun == 1 ) {
    VFrIdInRun = 0;
  }
  
  // Detect beginnig of processing = First acq stored in memory buffer <=> First acq of test step file
  
  if ( AcqIdInBuff == 0 ) {
    VDestFrId   = 0;
    VFrIdInBuff = 0;
    VFrPos      = 0;
  }
  
  VCurAcqFrNb = VPtAcqFrList->ResFrNb;


  if ( PrintLvl == 1 ) {
    msg (( MSG_OUT, "Proc Dec : AcqIdInRun = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d, FrNb = %d", AcqIdInRun, AcqIdInBuff, VAcqNbPerBuff, VCurAcqFrNb ));
  }
    
    
  randomize ();  
    
    
  // Process all frames of current acq
  
  for ( VCurAcqFrId = 0; VCurAcqFrId < VCurAcqFrNb; VCurAcqFrId++ ) {
    
    VPtDecFrPix  = &PtAcqProc->ADecFrPix[VDestFrId];
    VPtFrListDec = &PtAcqProc->AFrListDec[VDestFrId];
    VPtFrMapDec  = &PtAcqProc->AFrMapDec[VDestFrId];
      
      
    // msg (( MSG_OUT, "Proc Dec : AcqIdInRun = %d, AcqIdInBuff = %d, AcqNbInBuff = %d, FrNb = %d, VCurAcqFrId = %d", AcqIdInRun, AcqIdInBuff, AcqNbInBuff, VCurAcqFrNb, VCurAcqFrId ));
    
      
    // Decode pixels here => Pixels loop
    
    // VPixNbInFr = random (100);
    
    // VPixNbInFr = 0; // VCurAcqFrId;
    
    VPixNbInFr = VCurAcqFrId;
    
    MIS1__FEmulDecFrPixPatt0 ( VPtDecFrPix, VPixNbInFr );
    
    // Update MIS1__TDecFrPix header

    VPtDecFrPix->H.AcqIdInBuff = AcqIdInBuff;  /*!< AcqId <= 65535             */
    VPtDecFrPix->H.FrIdInBuff  = VFrIdInBuff;  /*!< FrId <= 65535              */
    VPtDecFrPix->H.PixNb       = VPixNbInFr;   /*!< Number of pixels           */
    VPtDecFrPix->H.MSis1FrCnt  = 0;           /*!< Frame counter from MSis 1  */
    VPtDecFrPix->H.MSis1ChkSum = 0;           /*!< Check sum from MSis 1      */
    VPtDecFrPix->H.MSis1Flags  = 0;           /*!< Flags from MSis 1          */
    
    
    // Update AFrListDec
        
    VPtFrListDec->ResAcqIdInRun = AcqIdInRun;       // Id of acquisition
    VPtFrListDec->ResFrNbInAcq  = VCurAcqFrNb;      // Frames nb in the acq which contains this frame
    VPtFrListDec->ResFrIdInAcq  = VCurAcqFrId;      // Id frame in its acquisition 0 to max fr nb per acq  - 1
    VPtFrListDec->ResFrSz       = sizeof ( MIS1__TDecFrPixHeader ) + (VPixNbInFr * sizeof (MIS1__TPixXY) );  // Size of frame in W8
    VPtFrListDec->ResPtFrBeg    = VPtDecFrPix;
  
    
    // Update AFrMapDec
    
    VPtFrMapDec->ResAcqIdInRun  = AcqIdInRun;            // Id of acquisition
    VPtFrMapDec->ResFrNbInAcq   = VCurAcqFrNb;           // Frames nb in the acq which contains this frame
    VPtFrMapDec->ResFrIdInAcq   = VCurAcqFrId;           // Id frame in its acquisition 0 to max fr nb per acq  - 1
    VPtFrMapDec->ResFrSz        = VPtFrListDec->ResFrSz; // Size of frame in W8
  
    VPtFrMapDec->ResPtFrBeg     = VPtDecFrPix;
  
    VPtFrMapDec->ResFrPos       = VFrPos; // Position of fr / Beginning of file in W8
  
  
    VFrPos                      += VPtFrMapDec->ResFrSz;

    VFrIdInBuff++;
    VFrIdInRun++;
    VDestFrId++;
  }
  
  
 
  
  return (0);
}




/**
===================================================================================
* \fn      : SInt32 MIS1__FProcEmulDecFpc ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile,  SInt32 AcqIdInBuff,  SInt8 PrintLvl )
*
* \brief   : Execute emul decode pixels + FPC processing for DAQ tests
*
* \param   : FirstAcqOfRun    - Flag, 1 => It is the first acq of the run
*
* \param   : PtAcqProc        - Pointer to acq processing record
*
* \param   : ProcMode         - Processing mode
*
* \param   : AcqIdInRun       - Id of acq in run 0 to acq nb in run - 1
*
* \param   : AcqIdInFile      - Id of acq in one file of the run, if characterization mode => acq id in current step
*
* \param   : AcqIdInBuff      - Id of acq in the bloc of acq bufferiezd in mem = for step file
*
* \param   : PrintLvl         - Print level
*
* \return  : Error code  \n
*          :   0  - OK   \n
*          : < 0 - Error \n
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 23/06/2020
* \date    : Rev       : 30/06/2020 \n
*                        - Reduced paramters list, moved to MIS1__TAcqProc
* \date    : Doc date  : 23/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FProcEmulDecFpc ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile,  SInt32 AcqIdInBuff,  SInt8 PrintLvl ) {

  
  // =======================================================================================================
  // Info on processinf method, data source, destination and buffers
  // =======================================================================================================
  // - A number of acq are stored in a memory buffer, this number is defined via the GUI param "Buffered acq nb"
  // - The processing can be done acquisition by acquisition or only when this memory buffer is fulled.
  //
  // - This function is called for each acquisition in the DAQ loop, it knows
  //
  // -- When the run starts by testing  FirstAcqOfRun == 1
  // -- The processing mode via ProcMode, you can use it to check if this function is not called by error ;-)
  // -- The id of the acq in the Run via AcqIdInRun, starting at 0
  // -- The id of the acq in the current file via AcqIdInFile, starting at 0
  // -- The id of the acq in the memory buffer via AcqIdInBuff, starting at 0
  // -- When filling of the memory buffer starts by testing AcqIdInBuff == 0
  // -- The maximum number of acq in the memory buffer via VAcqNbPerBuff
  // -- The acq number per file via VAcqNbPerFile
  // -- The acq number per sub step VAcqNbPerSubStep
  //
  // Some of the above info avec function parameters ( FirstAcqOfRun, AcqIdInRun, ...) they contain updated
  // values since the beginning of the function, others are variables (VAcqNbPerBuff, VAcqNbPerFile ... identifier
  // starting by V) their value is only updated after their intialization block.
  //
  // - Any other run param, info, status can be accessed via four pointers
  // -- PtAcqProc, available since beginning of function
  // -- VPtRunPar, available after initialzation block
  // -- VPtRunInf, available after initialzation block
  // -- VPtRunStatus, available after initialzation block
  //
  // - The source data are organized as an array of Acq, because the FlexRIo returns data acq by acq,
  //   the destination data are function of the processing goal => different for each processing function
  //
  // - The source data for the processing and destination results buffers are already allocated and
  //   access is done via pointers fields in the MIS1__TAcqProc record, local variables are set here for this
  //
  // -- VPtAcqFrList = Source acq, VPtAcqFrList[n] = acq n in memory buffer, subfields give access to frames
  //    n = 0 to AcqNbInBuff - 1
  //
  // -- VPtDecFrPix = Destination result, per frame, VPtDecFrPix[m] = pixels list for frame m
  //    m = 0 to ( MIS1__MAX_BUFFERED_ACQ_NB * MIS1__MAX_FR_NB_PER_ACQ ) - 1, buffer should be big enough
  //    but it is better to check that no access out of buffer space is done.
  //    As the pixels number can vary each VPtDecFrPix record has a fixed size header MIS1__TDecFrPixHeader
  //    and a variable pixels list field APix, size information MUST be set at two places via header field
  //    VPtFrMapDec and the records VPtFrListDec, VPtFrMapDec corresponding to the frame.
  //
  // -- Records VPtFrListDec, VPtFrMapDec contains information needed to access to the result records of
  //    processing function, because results records have a variable length. VPtFrListDec, VPtFrMapDec
  //    MUST be updated by THIS FUNCTION after processing of each frame. The record VPtFrListDec gives
  //    pointers to results record + some info, the VPtFrMapDec gives same info by pointer are converted
  //    to offset needed to create the index file.
  //
  // - The results of the processing, stored in buffer VPtDecFrPix are saved (if enabled) in a file at the
  //   end of the processing (after processing of AcqNbInBuff - 1)
  

  MIS1__TRunPar*    VPtRunPar;
  MIS1__TRunInf*    VPtRunInf;
  MIS1__TRunStatus* VPtRunStatus;
  
  // Variables for parameters removed on 30/06/2020, acces is now done via MIS1__TAcqProc
  
  SInt32           VAcqNbPerFile;
  SInt32           VAcqNbPerSubStep;
  SInt32           VAcqNbPerBuff;
  FIL__TCBinFile*  VPtStepBinFile;
  FIL__TCBinFile*  VPtSubStepBinFile;

  
  SInt32 VRet;
  
  SInt32 VCurAcqFrNb; // Fr nb in the current acq processed by this function
  SInt32 VCurAcqFrId; // Fr id fro scanning frame of the current acq processed by this function
  static SInt32 VDestFrId;   // Result, destination Fr id for indexing destination array
  static SInt32 VFrIdInBuff; // Fr id in the memory buffer of size "Buffered acq nb" in GUI
  static SInt32 VFrIdInRun;  // Fr id since beginning of the run
  static UInt32 VFrPos;      // Position in W8 / beginning of memory buffer = / beginning of file
  // One memory buffer = "Buffered acq nb" = One file, One Run = Many files
  
  static SInt32 ViSubStep;
  static SInt32 ViStep;
  
  SInt32 VPixNbInFr;
  
  
  MIS1__TAcqProc*      VPtAcqProc;        // Acquisition processing record, pointers fields to data buffers
  MIS1__TAcqFrList*    VPtAcqFrList;      // List of acq, frames in the source acq to processed
  MIS1__TAcqFrMap*     VPtAcqFrMap;       // Mapping of acq, frames (positions for index file) in the source acq to processed
  MIS1__TDecFrList*    VPtFrListDec;      // List of frames in the destination buffer (already allocated)
  MIS1__TDecFrMap*     VPtFrMapDec;       // Mapping of frames (positions for index file) in the destination buffer (already allocated)
  MIS1__TDecFrPix*     VPtDecFrPix;       // Destination decoded pixels list for each frame, (already allocated)
  MIS1__TMatPixCntU16* VPtSubStepPixCnt;  // Destination for sub step fired pixels count matrix
  MIS1__TMatPixCntU16* VPtStepPixCnt;     // Destination for step fired pixels count matrix
  
  
  
  // Check param
  
  err_retnull ( PtAcqProc, (ERR_OUT,"Abort => PtAcqProc == NULL") );

  // Get some parameters
  
  VPtRunPar           = PtAcqProc->PtRunPar;
  VPtRunInf           = PtAcqProc->PtRunInf;
  VPtRunStatus        = PtAcqProc->PtRunStatus;
  
  VAcqNbPerFile       = PtAcqProc->PtRunPar->ParAcqNbPerFile;
  VAcqNbPerSubStep    = PtAcqProc->PtRunPar->ParCarSubStepAcqNb;
  VAcqNbPerBuff       = PtAcqProc->PtRunPar->ParBufferedAcqNb;
  VPtStepBinFile      = PtAcqProc->PtOStepFile;
  VPtSubStepBinFile   = PtAcqProc->PtOSubStepsFile;


  if ( AcqIdInBuff >= VAcqNbPerBuff ) {
    err_retfail ( -1, (ERR_OUT,"Abort => AcqIdInBuff >= VAcqNbPerBuff", AcqIdInBuff, VAcqNbPerBuff) );
  }
  
  // Set ptr
  
  VPtAcqProc   = PtAcqProc;                              // Acquisition processing : scan to find frames list, count fired pixels, etc
  VPtAcqFrList = &PtAcqProc->AAcqFrListRaw[AcqIdInBuff]; // List of frames in one acq - Now ONLY ONE acq used
  VPtAcqFrMap  = &PtAcqProc->AAcqFrMapRaw[AcqIdInBuff];  // List of frames in one acq - Now ONLY ONE acq used
  
  VPtSubStepPixCnt = PtAcqProc->PtSubStepPixCnt;
  VPtStepPixCnt    = PtAcqProc->PtStepPixCnt;
  
  // Processing

  if ( PrintLvl == 2 ) {
    msg (( MSG_OUT, "Proc Dec : AcqIdInRun = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d ", AcqIdInRun, AcqIdInBuff, VAcqNbPerBuff ));
  }
  
  // Detect beginning of the run = First acq of the run
  
  if ( FirstAcqOfRun == 1 ) {
    VFrIdInRun = 0;
    ViStep     = 0; 
            

    if ( PrintLvl == 3 ) {
      msg (( MSG_OUT, "Proc FPC : Beginning of run : AcqIdInRun = %d, AcqIdInFile = %d, VAcqNbPerFile = %d, VAcqNbPerSubStep = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d", AcqIdInRun, AcqIdInFile, VAcqNbPerFile, VAcqNbPerSubStep, AcqIdInBuff, VAcqNbPerBuff ));
    }

  }

  // Detect beginning of a new file, if characterization mode => beginning of new step

  if ( AcqIdInFile == 0 ) {

    if ( PrintLvl == 3 ) {
      msg (( MSG_OUT, "Proc FPC : Beginning of file : AcqIdInRun = %d, AcqIdInFile = %d, VAcqNbPerFile = %d, VAcqNbPerSubStep = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d", AcqIdInRun, AcqIdInFile, VAcqNbPerFile, VAcqNbPerSubStep, AcqIdInBuff, VAcqNbPerBuff ));
    }
    
    
    ViSubStep = 0;

    // Count pixels

    MIS1__FMatPixCntU16Rst ( VPtSubStepPixCnt );
    MIS1__FMatPixCntU16Rst ( VPtStepPixCnt    );


    if ( (VPtRunPar->ParSaveFPC == MIS1__SFPC_SUB_STEPS) || (VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL) ) {

      // Create a sub step file
      
      VRet = MIS1__FBuildFileNameSuffix ( VPtRunInf->InfFPCSubStepsFileName, VPtRunInf->InfCurFPCSubStepsFileName, GLB_FILE_PATH_SZ, VPtRunInf->InfSaveFileNo );
      
      err_retfail ( VRet, (ERR_OUT,"Abort => Build file name with prefix for step = %d, sub step = %d failed !", ViStep, ViSubStep ) );
      
      VRet = VPtSubStepBinFile->PubFSetFileName ( VPtRunInf->InfCurFPCSubStepsFileName );
      
      VRet = VPtSubStepBinFile->PubFCreate ();
      
      err_retfail ( VRet, (ERR_OUT,"Abort => Create file for step = %d, sub step = %d failed !", ViStep, ViSubStep ) );
      
    } // End if ( (VPtRunPar->ParSaveFPC == MIS1__SFPC_SUB_STEPS) || (VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL) )

          
  }


  // Detect beginnig of processing = First acq stored in memory buffer <=> First acq of test step file
  
  if ( AcqIdInBuff == 0 ) {
    VDestFrId   = 0;
    VFrIdInBuff = 0;
    VFrPos      = 0;
  }
  
  VCurAcqFrNb = VPtAcqFrList->ResFrNb;
  
  
  if ( PrintLvl == 1 ) {
    msg (( MSG_OUT, "Proc FPC : AcqIdInRun = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d, FrNb = %d", AcqIdInRun, AcqIdInBuff, VAcqNbPerBuff, VCurAcqFrNb ));
  }
  
  
  randomize ();
  
  
  // Process all frames of current acq
  
  for ( VCurAcqFrId = 0; VCurAcqFrId < VCurAcqFrNb; VCurAcqFrId++ ) {
    
    VPtDecFrPix  = &PtAcqProc->ADecFrPix[VDestFrId];
    VPtFrListDec = &PtAcqProc->AFrListDec[VDestFrId];
    VPtFrMapDec  = &PtAcqProc->AFrMapDec[VDestFrId];
    
    
    // msg (( MSG_OUT, "Proc Dec : AcqIdInRun = %d, AcqIdInBuff = %d, AcqNbInBuff = %d, FrNb = %d, VCurAcqFrId = %d", AcqIdInRun, AcqIdInBuff, AcqNbInBuff, VCurAcqFrNb, VCurAcqFrId ));
    
    
    // Decode pixels here => Pixels loop
    
    // VPixNbInFr = random (100);
    
    VPixNbInFr = VCurAcqFrId;
    

    MIS1__FEmulDecFpcFrPixPatt0 ( VPtDecFrPix, VPtSubStepPixCnt, VPtStepPixCnt, VPixNbInFr );
    
    
    // Update MIS1__TDecFrPix header
    
    VPtDecFrPix->H.AcqIdInBuff = AcqIdInBuff;  /*!< AcqId <= 65535             */
    VPtDecFrPix->H.FrIdInBuff  = VFrIdInBuff;  /*!< FrId <= 65535              */
    VPtDecFrPix->H.PixNb       = VPixNbInFr;   /*!< Number of pixels           */
    VPtDecFrPix->H.MSis1FrCnt  = 0;           /*!< Frame counter from MSis 1  */
    VPtDecFrPix->H.MSis1ChkSum = 0;           /*!< Check sum from MSis 1      */
    VPtDecFrPix->H.MSis1Flags  = 0;           /*!< Flags from MSis 1          */
    
    
    // Update AFrListDec
    
    VPtFrListDec->ResAcqIdInRun = AcqIdInRun;       // Id of acquisition
    VPtFrListDec->ResFrNbInAcq  = VCurAcqFrNb;      // Frames nb in the acq which contains this frame
    VPtFrListDec->ResFrIdInAcq  = VCurAcqFrId;      // Id frame in its acquisition 0 to max fr nb per acq  - 1
    VPtFrListDec->ResFrSz       = sizeof ( MIS1__TDecFrPixHeader ) + (VPixNbInFr * sizeof (MIS1__TPixXY) );  // Size of frame in W8
    VPtFrListDec->ResPtFrBeg    = VPtDecFrPix;
    
    
    // Update AFrMapDec
    
    VPtFrMapDec->ResAcqIdInRun  = AcqIdInRun;            // Id of acquisition
    VPtFrMapDec->ResFrNbInAcq   = VCurAcqFrNb;           // Frames nb in the acq which contains this frame
    VPtFrMapDec->ResFrIdInAcq   = VCurAcqFrId;           // Id frame in its acquisition 0 to max fr nb per acq  - 1
    VPtFrMapDec->ResFrSz        = VPtFrListDec->ResFrSz; // Size of frame in W8
    
    VPtFrMapDec->ResPtFrBeg     = VPtDecFrPix;
    
    VPtFrMapDec->ResFrPos       = VFrPos; // Position of fr / Beginning of file in W8
    
    
    VFrPos                      += VPtFrMapDec->ResFrSz;
    
    VFrIdInBuff++;
    VFrIdInRun++;
    VDestFrId++;
  }
  
  // Detect end of sub step => save to file

  if ( (AcqIdInFile + 1) % VAcqNbPerSubStep == 0 ) {
    
    if ( PrintLvl == 3 ) {
      msg (( MSG_OUT, "Proc FPC : End of sub step No %.4d : AcqIdInRun = %d, AcqIdInFile = %d, VAcqNbPerFile = %d, VAcqNbPerSubStep = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d", ViSubStep, AcqIdInRun, AcqIdInFile, VAcqNbPerFile, VAcqNbPerSubStep, AcqIdInBuff, VAcqNbPerBuff ));
    }
    
    
    if ( (VPtRunPar->ParSaveFPC == MIS1__SFPC_SUB_STEPS) || (VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL) ) {
      
      // Save sub step record to sub step file file
    
      VRet = VPtSubStepBinFile->PubFSeqWrite ( VPtAcqProc->PtSubStepPixCnt, sizeof (MIS1__TMatPixCntU16) );
    
      err_retfail ( VRet, (ERR_OUT,"Abort => Saving sub step file = %s failed !", VPtSubStepBinFile->PubFGetFileName () ));
    
    } // End if ( (VPtRunPar->ParSaveFPC == MIS1__SFPC_SUB_STEPS) || (VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL) )
      
      
    ViSubStep++;
  }
  
  // Detect end of step

  if ( (AcqIdInFile + 1) == VAcqNbPerFile ) {
    
    if ( PrintLvl == 3 ) {
      msg (( MSG_OUT, "Proc FPC : End of step No %.4d : AcqIdInRun = %d, AcqIdInFile = %d, VAcqNbPerFile = %d, VAcqNbPerSubStep = %d, AcqIdInBuff = %d, VAcqNbPerBuff = %d", ViStep, AcqIdInRun, AcqIdInFile, VAcqNbPerFile, VAcqNbPerSubStep, AcqIdInBuff, VAcqNbPerBuff ));
    }
        
    
    if ( VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL ) {
      
      // Save STEP record to SUB STEP file file
            
      VRet = VPtSubStepBinFile->PubFSeqWrite ( VPtAcqProc->PtStepPixCnt, sizeof (MIS1__TMatPixCntU16) );
      
      err_retfail ( VRet, (ERR_OUT,"Abort => Saving sub step file = %s failed !", VPtSubStepBinFile->PubFGetFileName () ));
      
    } // End if ( VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL )
    
    
    if ( (VPtRunPar->ParSaveFPC == MIS1__SFPC_SUB_STEPS) || (VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL) ) {
      
      // Close sub step file
      
      VRet = VPtSubStepBinFile->PubFClose ();
      
      err_retfail ( VRet, (ERR_OUT,"Abort => Close sub step file = %s failed !", VPtSubStepBinFile->PubFGetFileName () ));
      
    } // End if ( (VPtRunPar->ParSaveFPC == MIS1__SFPC_SUB_STEPS) || (VPtRunPar->ParSaveFPC == MIS1__SFPC_ALL) )
      
      

    ViStep++;
  }
  
  return (0);
}




/**
===================================================================================
* \fn      : SInt32 MIS1__FProc ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile, SInt32 AcqNbInFile, SInt32 AcqNbPerSubStep, SInt32 AcqIdInBuff, SInt32 AcqNbInBuff, SInt8 PrintLvl )
*
* \brief   : Execute acq processing, call processing function selected by ProcMode param 
*
* \param   : FirstAcqOfRun   - Flag, 1 => It is the first acq of the run
*
* \param   : PtAcqProc       - Pointer to acq processing record
*
* \param   : ProcMode        - Processing mode 
*
* \param   : AcqIdInRun      - Id of acq in run 0 to acq nb in run - 1
*
* \param   : AcqIdInFile     - Id of acq in one file of the run, if characterization mode => acq id in current step
*
* \param   : AcqNbInFile     - Number of acq in one file of the run, if characterization mode => acq nb per step
*
* \param   : AcqNbPerSubStep - If characterization mode => number of acq in current sub step
*
* \param   : AcqIdInBuff     - Id of acq in the bloc of acq bufferiezd in mem = for step file
*
* \param   : AcqNbInBuff     - Number of acq in the bloc of acq bufferiezd in mem = for step file
*
* \param   : PrintLvl         - Print level
*
* \param   : PtStepBinFile    - Step file TCBinFile object pointer
*
* \param   : PtSubStepBinFile - Sub step file TCBinFile object pointer
*
* \return  : Error code  \n
*          :   0  - OK   \n
*          : < 0 - Error \n
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 08/06/2020
* \date    : Doc date  : 08/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

  
SInt32 MIS1__FProc ( SInt8 FirstAcqOfRun, MIS1__TAcqProc* PtAcqProc, MIS1__TProcMode ProcMode, SInt32 AcqIdInRun, SInt32 AcqIdInFile, SInt32 AcqIdInBuff, SInt8 PrintLvl ) {
    
  SInt32 VRet;
  
  switch ( ProcMode ) {
    
    case PM_NONE : {
      break; }
    
    case PM_DECODE_FR : {
      VRet = MIS1__FProcDec ( FirstAcqOfRun, PtAcqProc, ProcMode, AcqIdInRun, AcqIdInFile, AcqIdInBuff, PrintLvl );
      break; }
    
    case PM_DECODE_FR_CNT_PIX : {
      break; }
    
    case PM_EMUL_DECODE_FR_DAQ_TEST : {
      VRet = MIS1__FProcEmulDec ( FirstAcqOfRun, PtAcqProc, ProcMode, AcqIdInRun, AcqIdInFile, AcqIdInBuff, PrintLvl );
      break; }


    case PM_EMUL_DECODE_FR_FPC_DAQ_TEST : {
      VRet = MIS1__FProcEmulDecFpc ( FirstAcqOfRun, PtAcqProc, ProcMode, AcqIdInRun, AcqIdInFile, AcqIdInBuff, PrintLvl );
      break; }

    
    default : {
      err_retfail ( -1, (ERR_OUT,"Abort => Unknown ProcMode = %d ", ProcMode ) );
      break; }
  }

  err_retfail ( VRet, (ERR_OUT,"Abort => Prcoessing function failed") );
  
  return (0);
}
  


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPrintDecFrPixHeader ( MIS1__TDecFrPixHeader* PtSrc, SInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*
* \brief   : Print the header of MIS1__TDecFrPix record
*
* \param   : PtSrc - Pointer to the header
*
* \param   : PrintDest - Print destination, 			                \n
*             0 => No print, 					                             \n
*             1 => msg file , 2 => Memo, 3 => Msg file + Memo  \n
*             4 => text file, 5 => Text file + Memo 		         \n
*           							                                            \n
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/06/2020
* \date    : Doc date  : 11/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FPrintDecFrPixHeader ( MIS1__TDecFrPixHeader* PtSrc, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {
  
  char VMsg[GLB_CMT_SZ];
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  // Print
  
  MIS1__FPrintStr ( "==================================", PrintDest, Memo, PtFile );
  sprintf ( VMsg, "Acq id in buffer / file = %.4d", PtSrc->AcqIdInBuff );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  sprintf ( VMsg, "Fr id in buffer / file  = %.4d", PtSrc->FrIdInBuff );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  sprintf ( VMsg, "Pixel number in frame   = %.4d", PtSrc->PixNb );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  sprintf ( VMsg, "MSis 1 frame cnt        = %.4d",PtSrc->MSis1FrCnt );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  sprintf ( VMsg, "MSis 1 check sum        = %.4X [H]", PtSrc->MSis1ChkSum );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  sprintf ( VMsg, "MSis 1 trailer + flags  = %.4X [H]", PtSrc->MSis1Flags);
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( "==================================", PrintDest, Memo, PtFile );
  
  return (0);
}

  


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPrintDecFrPix ( MIS1__TDecFrPix* PtSrc, SInt8 PrintHeader, SInt32 FirstPix, SInt32 LastPix, SInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*
* \brief   : Print MIS1__TDecFrPix record header and / or pixels list
*
* \param   : PtSrc       - Pointer to the header
*
\param     : PrintHeader - 1 => Print header, 0 => No
*
* \param   : FirstPix    - First pixel to print
*
* \param   : LastPix     - Last pixel to print, or -1 to print ALL pixel starting at FirstPix
*
* \param   : PrintDest - Print destination, \n
*          :   0 => No print, \n
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo \n
*          :   4 => text file, 5 => Text file + Memo \n
*
* \return  : Error code   \n
*          :   0 - OK     \n
*          : < 0 - Error  \n
*
* \warning : Globals   :
* \warning : Remark    : To print ONLY the header set PrintHeader= 1 and FirstPix = LastPix= -1 
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 11/06/2020
* \date    : Doc date  : 11/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



SInt32 MIS1__FPrintDecFrPix ( MIS1__TDecFrPix* PtSrc, SInt8 PrintHeader, SInt32 FirstPix, SInt32 LastPix, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {
  
  SInt32 ViPix;
  SInt32 VPixNb;
  char   VMsg[GLB_CMT_SZ];
  
  // Check param
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  // Print header
  
  if ( PrintHeader ) {
    MIS1__FPrintDecFrPixHeader ( &(PtSrc->H), PrintDest, Memo, PtFile );
  }
  
  // No print of pixels list
  
  if ( (FirstPix== -1 ) && (LastPix== -1 ) ) {
    return (0);
  }
  
  // Print all pixels
  
  VPixNb = PtSrc->H.PixNb;

  if ( VPixNb == 0 ) {
    
    sprintf ( VMsg, "Acq in buff / file = %.4d, Fr = %.4d => No pixel ", PtSrc->H.AcqIdInBuff,  PtSrc->H.FrIdInBuff );
    
    MIS1__FPrintStr ( "================================================", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "================================================", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "", PrintDest, Memo, PtFile );
    
    return (0);
  }


  sprintf ( VMsg, "Acq in buff / file = %.4d, Fr = %.4d => %d pixels ", PtSrc->H.AcqIdInBuff,  PtSrc->H.FrIdInBuff, VPixNb );
  
  MIS1__FPrintStr ( "================================================", PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( "================================================", PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( "", PrintDest, Memo, PtFile );
  

    
  for ( ViPix=0; ViPix < VPixNb; ViPix++ ) {
    sprintf ( VMsg, "Pixel [%.4d] = X=%.4d, Y=%.4d", ViPix, PtSrc->APix[ViPix].C.x , PtSrc->APix[ViPix].C.y );    
    MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );    
  }


  return (0);  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FSelPixMatResetSet (  MIS1__TMatPixSelPix* PtDestMat, SInt8 ResetSet )
*
* \brief   : Reset or set all pixels of pîxels select matrix
*
* \param   : PtDestMat - Pointer to destination matrix
*
* \param   : ResetSet  - 0 => Reset, 1 => Set
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     : 
*
* \date    : Date      : 18/06/2020
* \date    : Doc date  : 18/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FSelPixMatResetSet (  MIS1__TMatPixSelPix* PtDestMat, SInt8 ResetSet ) {
  
  UInt16 ViCol;
  UInt16 ViRow;
    

  // Check param
  
  err_retnull ( PtDestMat, (ERR_OUT,"Abort => PtDestMat == NULL") );
  
   
  // Fill matrix
  
  // MIS1__TMatPixSelPix[MIS1__COL_NB][MIS1__ROW_NB]
  
  for ( ViCol = 0; ViCol < MIS1__COL_NB; ViCol++ ) {
    
    for ( ViRow = 0; ViRow < MIS1__ROW_NB; ViRow++ ) {
      (*PtDestMat)[ViCol][ViRow] = ResetSet;
    }
    
  } 
  

  err_retok (( ERR_OUT, "" ));
}
  

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FSelPixMatPrint (  MIS1__TMatPixSelPix* PtSrcMat, SInt8 PrintZeroOne, SInt32 ExitAFterNPrints, SInt8 PrintDest, TMemo* Memo, FILE* PtFile  )
*
* \brief   :
*
* \param   : PtSrcMat         - Pointer to source matrix
*
* \param   : PrintZeroOne     - 0 => Print pixels at 0, 1 => Print pixel at 1
*
* \param   : ExitAFterNPrints - Exit function after N pixels print in order to not lock the system \n
*                               n <= 0 => Not used, print all pixels \n
*                               n  > 0 => Exit after printing n pixels
*
* \param   : PrintDest - Print destination, \n
*          :   0 => No print, \n
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo \n
*          :   4 => text file, 5 => Text file + Memo \n
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 18/06/2020
* \date    : Doc date  : 18/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FSelPixMatPrint (  MIS1__TMatPixSelPix* PtSrcMat, SInt8 PrintZeroOne, SInt32 ExitAFterNPrints, SInt8 PrintDest, TMemo* Memo, FILE* PtFile  ) {
  
  UInt16 ViCol;
  UInt16 ViRow;
  SInt32 ViPix;
  char   VMsg[GLB_CMT_SZ];
   
  // Check param
  
  err_retnull ( PtSrcMat, (ERR_OUT,"Abort => PtSrcMat == NULL") );
  
  // Print matrix
  
  
  if ( PrintZeroOne == 0  ) {    
    MIS1__FPrintStr ( "---------------------------------", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "        List pixels at 0         ", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "---------------------------------", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "", PrintDest, Memo, PtFile );
  }
  
  else {
    MIS1__FPrintStr ( "---------------------------------", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "        List pixels at 1         ", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "---------------------------------", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "", PrintDest, Memo, PtFile );
  }
  
  
  // MIS1__TMatPixSelPix[MIS1__COL_NB][MIS1__ROW_NB]
  
  // Print all pixels detected at 0 / 1
  
  if ( ExitAFterNPrints <= 0 ) {
    ExitAFterNPrints = MIS1__COL_NB * MIS1__ROW_NB;
  }
  
  
  ViPix = 0;
  
  // Print 0
  
  if ( PrintZeroOne == 0 ) {

    for ( ViCol = 0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      for ( ViRow = 0; ViRow < MIS1__ROW_NB; ViRow++ ) {
        
        if ( (*PtSrcMat)[ViCol][ViRow] == 0 ) {
          sprintf ( VMsg, "Pix [%.04d,%.04d] = 0",  ViCol, ViRow );
          MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
          ViPix++;
        }
        
                
        if ( ViPix >= ExitAFterNPrints ) {
          msg (( MSG_OUT, "Abort before end => Max pix nb to print = %d reached", ExitAFterNPrints ));
          MIS1__FPrintStr ( MSG_OUT, PrintDest, Memo, PtFile );
          err_retfail ( -1, (ERR_OUT,"%s", MSG_OUT) );
        }
        
        
      } // End for ( ViCol )
      
    } // End for ( ViRow )
    
  }
  
  // Print 1
   
  else {
    
    for ( ViCol = 0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      for ( ViRow = 0; ViRow < MIS1__ROW_NB; ViRow++ ) {
        
        if ( (*PtSrcMat)[ViCol][ViRow] == 1 ) {
          sprintf ( VMsg, "Pix [%.04d,%.04d] = 1",  ViCol, ViRow );
          MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
          ViPix++;
        }        

               
        if ( ViPix >= ExitAFterNPrints ) {
          msg (( MSG_OUT, "Abort before end => Max pix nb to print = %d reached", ExitAFterNPrints ));
          MIS1__FPrintStr ( MSG_OUT, PrintDest, Memo, PtFile );
          err_retfail ( -1, (ERR_OUT,"%s", MSG_OUT) );
        }
        
      } // End for ( ViCol )
      
    } // End for ( ViRow )
    
    
  }
  
   
  err_retok (( ERR_OUT, "" ));  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__FSelPixMatToList ( MIS1__TMatPixSelPix* PtSrcMat, MIS1__TPixXY* PtDest, UInt32 MaxDestESz, SInt8 ListZeroOne )
*
* \brief   : Build a list from the matrix
*
* \param   : None
*
* \param   : ListZeroOneAll - 0 => List only pixels at 0 \n
*                           - 1 => List only pixels at 1 \n
*                           - 2 => List all pixels, regardless of their state \n
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          : >= 0 - Nb of pixels at 0 or 1 detected  and filled in destination list \n
*          : < 0  - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     : 
*
* \date    : Date      : 22/06/2020
* \date    : Doc date  : 22/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FSelPixMatToList ( MIS1__TMatPixSelPix* PtSrcMat, MIS1__TPixXY* PtDest, UInt32 MaxDestESz, SInt8 ListZeroOne ) {
  
  UInt16 ViCol;
  UInt16 ViRow;
  SInt32 ViPix;


  // Check param
  
  err_retnull ( PtSrcMat, (ERR_OUT,"Abort => PtSrcMat == NULL") );
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
  
  // Fill list
  
  ViPix = 0;
  
  // Print 0
  
  if ( ListZeroOne == 0 ) {
    
    for ( ViCol = 0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      for ( ViRow = 0; ViRow < MIS1__ROW_NB; ViRow++ ) {
        
        if ( (*PtSrcMat)[ViCol][ViRow] == 0 ) {
          
          PtDest[ViPix].C.x = ViCol;
          PtDest[ViPix].C.y = ViRow;
          
          ViPix++;
          
          if ( ViPix >= MaxDestESz) {
            err_retfail ( -1, (ERR_OUT,"Abort => Stopped %d pixels at 0 listed, destination array size reached", ViPix) );
          }
          
        }                        
        
      } // End for ( ViCol )
      
    } // End for ( ViRow )
    
  }
  
  // Print 1
  
  else {
    
    for ( ViCol = 0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      for ( ViRow = 0; ViRow < MIS1__ROW_NB; ViRow++ ) {
        
        if ( (*PtSrcMat)[ViCol][ViRow] == 1 ) {
          
          PtDest[ViPix].C.x = ViCol;
          PtDest[ViPix].C.y = ViRow;
          
          ViPix++;
          
          if ( ViPix >= MaxDestESz) {
            err_retfail ( -1, (ERR_OUT,"Abort => Stopped %d pixels at 1 listed, destination array size reached", ViPix) );
          }          
          
        }
        
                
      } // End for ( ViCol )
      
    } // End for ( ViRow )
    
    
  }
  
  return (ViPix);
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FSelPixListToMat ( MIS1__TPixXY* PtSrcList, UInt32 SrcPixNb , MIS1__TMatPixSelPix* PtDestMat, SInt8 SrcListZeroOne )
*
* \brief   : Build a matrix from the list
*
* \param   : None
*
* \param   : SrcListZeroOne  - 0 => Source list contains pixels at 0 \n
*                            - 1 => Source list contains pixels at 1 \n
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 22/06/2020
* \date    : Doc date  : 22/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FSelPixListToMat ( MIS1__TPixXY* PtSrcList, UInt32 SrcPixNb , MIS1__TMatPixSelPix* PtDestMat, SInt8 SrcListZeroOne ) {
  
  UInt16 ViCol;
  UInt16 ViRow;
  SInt32 ViPix;
  
  
  // Check param
  
  err_retnull ( PtSrcList, (ERR_OUT,"Abort => PtSrcList == NULL") );
  
  err_retnull ( PtDestMat, (ERR_OUT,"Abort => PtDestMat == NULL") );
  
  // ---------------------------------------------
  // Source list contains pixels at 0
  // ---------------------------------------------
  
  if ( SrcListZeroOne == 0 ) {
    
    // Fill matrix with 1
    
    for ( ViCol = 0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      for ( ViRow = 0; ViRow < MIS1__ROW_NB; ViRow++ ) {
        
        (*PtDestMat)[ViCol][ViRow] = 1;
        
      } // End for ( ViCol )
      
    } // End for ( ViRow )
    
    
    // Set pixels at 0
    
    for ( ViPix = 0 ; ViPix < SrcPixNb; ViPix++ ) {
      (*PtDestMat)[PtSrcList[ViPix].C.x][PtSrcList[ViPix].C.y] = 0;
    }
    
    
  } // End if ( SrcListZeroOne == 0 )


  // ---------------------------------------------
  // Source list contains pixels at 1
  // ---------------------------------------------
  
  else {
    
    // Fill matrix with 0
    
    for ( ViCol = 0; ViCol < MIS1__COL_NB; ViCol++ ) {
      
      for ( ViRow = 0; ViRow < MIS1__ROW_NB; ViRow++ ) {
        
        (*PtDestMat)[ViCol][ViRow] = 0;
        
      } // End for ( ViCol )
      
    } // End for ( ViRow )
    
    
    // Set pixels at 1
    
    for ( ViPix = 0 ; ViPix < SrcPixNb; ViPix++ ) {
      (*PtDestMat)[PtSrcList[ViPix].C.x][PtSrcList[ViPix].C.y] = 1;
    }
        
    
  } // End else
  
  err_retok (( ERR_OUT, "" ));
}
  
  


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FPixListPrint ( MIS1__TPixXY* PtSrcList, UInt32 SrcPixNb, SInt32 ExitAFterNPrints, SInt8 PrintDest, TMemo* Memo, FILE* PtFile  )
*
* \brief   : Print pixels x, y list
*
* \param   : None
*
*
* \param   : ExitAFterNPrints - Exit function after N pixels print in order to not lock the system \n
*                               n <= 0 => Not used, print all pixels \n
*                               n  > 0 => Exit after printing n pixels
*
* \param   : PrintDest - Print destination, 			                  \n
*              0 => No print, 					                              \n
*              1 => msg file , 2 => Memo, 3 => Msg file + Memo   \n
*              4 => text file, 5 => Text file + Memo 		          \n
*           							                                              \n
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 22/06/2020
* \date    : Doc date  : 22/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FPixListPrint ( MIS1__TPixXY* PtSrcList, UInt32 SrcPixNb, SInt32 ExitAFterNPrints, SInt8 PrintDest, TMemo* Memo, FILE* PtFile  ) {
  
  SInt32 ViPix;
  
  char VMsg[GLB_CMT_SZ];
  
  
  // Check param
  
  err_retnull ( PtSrcList, (ERR_OUT,"Abort => PtSrcList == NULL") );
  
  
  // Print
  
  MIS1__FPrintStr ( "------------------------------", PrintDest, Memo, PtFile );
  sprintf ( VMsg, "Print pixels list = %d pixels", SrcPixNb );
  MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( "------------------------------", PrintDest, Memo, PtFile );
  MIS1__FPrintStr ( "", PrintDest, Memo, PtFile );
    
  for ( ViPix = 0; ViPix < SrcPixNb; ViPix++ ) {

     if ( ViPix >= ExitAFterNPrints ) {
       msg (( MSG_OUT, "Abort before end => Max pix nb to print = %d reached", ExitAFterNPrints ));
       MIS1__FPrintStr ( MSG_OUT, PrintDest, Memo, PtFile );
       err_retfail ( -1, (ERR_OUT,"%s", MSG_OUT) );
    }
        
    sprintf ( VMsg, "Pix [%.04d,%.04d] = 1",  PtSrcList[ViPix].C.x, PtSrcList[ViPix].C.y );
      
    MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
            
      
  }

  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FBuildFileNameSuffix ( char* Src, char* Dest, UInt16 MaxDestSz, SInt32 Suffix)
*
* \brief   : Add a suffix to a file name
*
* \param   : Src       - Source file name string
*
* \param   : Dest      - Destination file name string
*
* \param   : MaxDestSz - Maximum size of destination string, including terminal 0
*
* \param   : Suffix     - The number suffix to add, inot used if < 0 
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*          
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 23/06/2020
* \date    : Doc date  : 23/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

MIS1__FBuildFileNameSuffix ( char* Src, char* Dest, UInt16 MaxDestSz, SInt32 Suffix) {
  
  SInt16 VFileNameLength;
  char   VFileNameWithoutExt[GLB_FILE_PATH_SZ];
  
  // Check param
    
  err_retnull ( Src, (ERR_OUT,"Abort => Src == NULL ") );

  err_retnull ( Dest,(ERR_OUT,"Abort => Dest == NULL ") );
  
  // Brute force method for dest size checking
  
  if ( MaxDestSz < GLB_FILE_PATH_SZ ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Destination max sz = %d < GLB_FILE_PATH_SZ = %d ", MaxDestSz, GLB_FILE_PATH_SZ) );
  }
  
  // Build file name

  VFileNameLength = strlen (Src);

  if ( Suffix >= 0) {
    strncpy ( VFileNameWithoutExt, Src, VFileNameLength-4 ); // Cpy src without extension
    VFileNameWithoutExt[VFileNameLength-4] = 0;
    sprintf ( Dest, "%s_%.4d.bin", VFileNameWithoutExt, Suffix );
  }
  
  else {
    sprintf ( Dest, "%s", Src );
  }
  
   
  err_retok (( ERR_OUT, "" ));
  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunCnfLUpdStepSubStep ( UInt32 Step, UInt32 SubStep, MIS1__TRunCnfLight* PtDest )
*
* \brief   : Update files names for Step, SubStep
*
* \param   : RunRootDir - Run root directory, without terminal /
*
* \param   : RunNo      - Run number
*
* \param   : PtDest     - Pointer to MIS1__TRunCnfLight record
*
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 23/06/2020
* \date    : Doc date  : 23/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunCnfLUpdStepSubStep ( UInt32 Step, UInt32 SubStep, MIS1__TRunCnfLight* PtDest ) {
  
  // Check param
  
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );
    
  
  if ( Step >= MIS1__CAR_MAX_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Step = %d > Max = %d", Step, MIS1__CAR_MAX_STEP_NB - 1) );
  }
  
  if ( SubStep >= MIS1__CAR_MAX_SUB_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => SubStep = %d > Max = %d", SubStep, MIS1__CAR_MAX_SUB_STEP_NB - 1) );
  }
  
  
  // Set
  
  PtDest->CurStepNo     = Step;
  PtDest->CurSubStepNo  = SubStep;
  
  // Build files names
  
  sprintf ( PtDest->CurStepFileFpcBin    , "%s/run_%d_%d_step.bin"    , PtDest->DirData, PtDest->ParRunNo, PtDest->CurStepNo );
  sprintf ( PtDest->CurSubStepsFileFpcBin, "%s/run_%d_%d_sub_step.bin", PtDest->DirData, PtDest->ParRunNo, PtDest->CurStepNo );
  sprintf ( PtDest->CurSubStepsFileRawBin, "%s/run_%d_%d_raw.bin", PtDest->DirData, PtDest->ParRunNo, PtDest->CurStepNo );
  
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunCnfLSet ( UInt32 Format, char* RunRootDir, UInt32 RunNo, UInt32 StepNb, UInt32 SubStepNb, UInt32 SubStepMode, UInt32 FrNb, SInt8 SaveRaw, SInt8 SaveSubStepFpc, SInt8 SaveStepFpc, MIS1__TRunCnfLight* PtDest )
*
* \brief   : Set run conf light
*
* \param   : RunRootDir - Run root directory, without terminal /
*
* \param   : RunNo      - Run number
*
* \param   : PtDest     - Pointer to MIS1__TRunCnfLight record
*
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 23/06/2020
* \date    : Doc date  : 23/06/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunCnfLSet ( UInt32 Format, char* RunRootDir, UInt32 RunNo, UInt32 StepNb, UInt32 SubStepNb, UInt32 SubStepMode, UInt32 FrNb, SInt8 SaveRaw, SInt8 SaveSubStepFpc, SInt8 SaveStepFpc, MIS1__TRunCnfLight* PtDest ) {
   
  // Check param
  
  err_retnull ( RunRootDir, (ERR_OUT,"Abort => RunRootDir == NULL") );
  
  err_retnull ( PtDest, (ERR_OUT,"Abort => PtDest == NULL") );

  // Reset destination record
  
  memset ( PtDest, 0, sizeof (MIS1__TRunCnfLight) );
  

  if ( StepNb > MIS1__CAR_MAX_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => StepNb = %d > Max = %d", StepNb, MIS1__CAR_MAX_STEP_NB) );
  }
  
  if ( SubStepNb > MIS1__CAR_MAX_SUB_STEP_NB ) {
    err_retfail ( -1, (ERR_OUT,"Abort => SubStepNb = %d > Max = %d", SubStepNb, MIS1__CAR_MAX_SUB_STEP_NB) );
  }
    
  /*!< Data format version in a human readable way  */
  
  switch ( Format ) {
    
    case 0 : {
      sprintf ( PtDest->ParFormatStr, "ParFormat = %d => Default format", Format );
      break; }

    case 1 : {
      sprintf ( PtDest->ParFormatStr, "ParFormat = %d => Format No 1", Format );
      break; }
    
    default : {
      sprintf ( PtDest->ParFormatStr, "ParFormat = %d => Unknown format => Abort", Format );
      err_retfail ( -1, (ERR_OUT,"MsgFail") );
      break; }
    
  }
    
  /*!< SubStepMode version in a human readable way  */
  
  switch ( SubStepMode ) {
    
    case 0 : {
      sprintf ( PtDest->ParSubStepModeStr, "SubStepMode = %d => Default sub step mode", SubStepMode );
      break; }
  
    case 1 : {
      sprintf ( PtDest->ParSubStepModeStr, "SubStepMode = %d => Sub step mode 1", SubStepMode );
      break; }
    
    default : {
      sprintf ( PtDest->ParSubStepModeStr, "SubStepMode = %d => Unknown sub step mode => Abort", SubStepMode );
      err_retfail ( -1, (ERR_OUT,"MsgFail") );
      break; }
    
  }
  
    
  // Build
  
  // Param
  
  PtDest->ParFormat = Format;                /*!< Data format version  */
  
  
  PtDest->ParRunNo      = RunNo;       /*!< Run number  */
  PtDest->ParStepNb     = StepNb;      /*!< Steps nb = scan param step nb  */
  PtDest->ParSubStepNb  = SubStepNb;   /*!< Sub steps nb = number of pixels groups needed to perform a step  */
  
  PtDest->ParSubStepMode = SubStepMode;                 /*!< Sub step mode = how is configured each sub step  */
  
  PtDest->ParFrNb = FrNb;        /*!< Frame number used for each sustep / step */
  
  // TO BE DONE
  // Building ParASteps, ParASubSteps, in function of SubStepMode
  //
  //
  // MIS1__TStepPar    ParASteps[MIS1__CAR_MAX_STEP_NB];    /*!< Parametrs of each step   */
  // MIS1__TSubStepPar ParASubSteps[MIS1__CAR_MAX_SUB_STEP_NB]; /*!< Parametrs of each sub step = pixels tested  */
  
  PtDest->ParSaveRaw        = SaveRaw;          /*!< Save raw data for each sub steps  */
  PtDest->ParSaveSubStepFpc = SaveSubStepFpc;   /*!< Save FPC for each sub step  */
  PtDest->ParSaveStepFpc    = SaveStepFpc;      /*!< Save FPC for each step  */
  
  
  sprintf ( PtDest->ParRootDir, "%s", RunRootDir ); // Ex : C:/tmp/iphc/msis1/res/
  
  
  // Inf
  
  sprintf ( PtDest->Dir, "%s/run_%d", PtDest->ParRootDir, PtDest->ParRunNo ); // Ex : C:/tmp/iphc/msis1/res/run_0
  
  sprintf ( PtDest->DirData, "%s/data", PtDest->Dir ); // Ex : C:/tmp/iphc/msis1/res/run_0/data
  sprintf ( PtDest->DirTxt , "%s/txt" , PtDest->Dir ); // Ex : C:/tmp/iphc/msis1/res/run_0/txt
  sprintf ( PtDest->DirPlot, "%s/plot", PtDest->Dir ); // Ex : C:/tmp/iphc/msis1/res/run_0/plot
  
        
  
//   sprintf ( VCumFile, "%s/data/run_%d_%d_cum.bin", APP__VGRunDir, APP__VGRunNo, APP__VGRunFileIndex0 );

  
  // Files
  
  PtDest->CurStepNo     = 0;
  PtDest->CurSubStepNo  = 0;
    
    
  MIS1__FRunCnfLUpdStepSubStep ( PtDest->CurStepNo, PtDest->CurSubStepNo, PtDest );
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRunCnfLPrint ( MIS1__TRunCnfLight* PtSrc, SInt8 PrintDest, TMemo* Memo, char* FileName)
*
* \brief   : Print MIS1__TRunCnfLight record
*
* \param   : PtSrc - Ptr to record
*
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*
* \param   : FileName - Fine path  +name if PrintDest = 4 or 5
*
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 07/10/2020
* \date    : Doc date  : 07/10/2020
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FRunCnfLPrint ( MIS1__TRunCnfLight* PtSrc, SInt8 PrintDest, TMemo* Memo, char* FileName) {
  
  FILE* VPtFile;
  char  VStr[GLB_CMT_SZ];
  
  // Check param
    
  err_retnull ( PtSrc, (ERR_OUT,"Abort => PtSrc == NULL") );
  
  // Create text file
  
  if ( (PrintDest == 4) || (PrintDest == 5) ) {
    
    err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
    
    VPtFile = fopen ( FileName, "wt" );
    
    err_retnull ( VPtFile, (ERR_OUT,"Abort => Create text file = %s failed !", FileName) );
  }
  
  // Print
  
  MIS1__FPrintStr ( "================================================", PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParFormat             = %d", PtSrc->ParFormat );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParFormatStr          = %s ", PtSrc->ParFormatStr );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );

  MIS1__FPrintStr ( "------------------------------------------------", PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParRunNo              = %d", PtSrc->ParRunNo );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParStepNb             = %d", PtSrc->ParStepNb );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );

  sprintf ( VStr, "ParSubStepNb          = %d", PtSrc->ParSubStepNb );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParSubStepMode        = %d", PtSrc->ParSubStepMode );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParSubStepModeStr     = %s ", PtSrc->ParSubStepModeStr );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParFrNb               = %d", PtSrc->ParFrNb );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );

  sprintf ( VStr, "MIS1__TStepPar - Not printed" );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "MIS1__TSubStepPar - Not printed");
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );

  MIS1__FPrintStr ( "------------------------------------------------", PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParSaveRaw            = %d", PtSrc->ParSaveRaw );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "ParSaveSubStepFpc     = %d", PtSrc->ParSaveSubStepFpc );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );

  sprintf ( VStr, "ParSaveStepFpc        = %d", PtSrc->ParSaveStepFpc );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
    
  MIS1__FPrintStr ( "------------------------------------------------", PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "Dir                   = %s", PtSrc->Dir );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );

  sprintf ( VStr, "DirData               = %s", PtSrc->DirData );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "DirTxt                = %s", PtSrc->DirTxt );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "DirPlot ", PtSrc->DirPlot );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  
  sprintf ( VStr, "CurStepFileFpcBin     = %s", PtSrc->CurStepFileFpcBin );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );


  sprintf ( VStr, "CurSubStepsFileFpcBin = %s", PtSrc->CurSubStepsFileFpcBin );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  
  sprintf ( VStr, "CurSubStepsFileRawBin = %s", PtSrc->CurSubStepsFileRawBin );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "CurStepNo             = %d", PtSrc->CurStepNo );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );
  
  sprintf ( VStr, "CurSubStepNo          = %d", PtSrc->CurSubStepNo );
  MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );

  
  
  // sprintf ( VStr, " ", );
  // MIS1__FPrintStr ( VStr, PrintDest, Memo, VPtFile );



  // Close text file

  if ( (PrintDest == 4) || (PrintDest == 5) ) {
    fclose ( VPtFile );
  }

  err_retok (( ERR_OUT, "" ));

}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFuncNotFinished ()
*
* \brief   : Terminates library operation => To be called at end of program
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n 
*          : < 0 - Error                                      \n
*          
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          
* \todo    : This function code has not been finished, please write the end ;-)
*          
* Items not filled now :
*  bug     : No
*          
* \date    : Date      : 11/04/2019
* \date    : Doc date  : 11/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FFuncNotFinished () {
  
  MIS1__VGBeginDone = 0;


  if ( MIS1__VGBeginDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FBegin (...) has not been called !") );
  }
  
  err_retok (( ERR_OUT, "" ));
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FFuncWithBug ()
*
* \brief   : Terminates library operation => To be called at end of program
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
* \bug     : There may be a bug in this function, please fix it ;-)
*
* \date    : Date      : 11/04/2019
* \date    : Doc date  : 11/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__FFuncWithBug () {
  
  MIS1__VGBeginDone = 0;
  
  
  if ( MIS1__VGBeginDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => MIS1__FBegin (...) has not been called !") );
  }
  
  
  err_retok (( ERR_OUT, "" ));
  
}



// Class MIS1__TCCarWarnErr



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 _FClearAcqTrailInfo  ( SInt8 MSisId )
*
* \brief   : MIS1__TCCarWarnErr constructor.
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    : Private method
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     : 
*
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::_FClearAcqTrailInfo  ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );


  memset ( &ATrailFlags[MSisId][0], 0, MIS1__MAX_FR_NB_PER_ACQ * sizeof (ATrailFlags [0][0]) );
  memset ( &ATrailFrCnt[MSisId][0], 0, MIS1__MAX_FR_NB_PER_ACQ * sizeof (ATrailFrCnt [0][0]) );
  
  return (0);
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TCCarWarnErr ()
*
* \brief   : MIS1__TCCarWarnErr constructor.
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
* \bug     : There may be a bug in this function, please fix it ;-)
*
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

MIS1__TCCarWarnErr::MIS1__TCCarWarnErr () {
  
  SafetyTag   = 0x16646660; // To check if loaded file has been created by this class
  
  DateVersion = 0x23032101; // Date = 23/03/2021 - Version = 1
  
  WarnErrCnt        = 0;
  WarnErrCntTot     = 0;
  ListIsFull        = 0;

  sprintf ( CstWarnErrType2Str [MIS1__WE_RECORD_EMPTY], "MIS1__WE_RECORD_EMPTY" );
  sprintf ( CstWarnErrType2Str [MIS1__WE_SW_ERR]      , "MIS1__WE_SW_ERR"       );
  sprintf ( CstWarnErrType2Str [MIS1__WE_SW_REG_OVF]  , "MIS1__WE_SW_REG_OVF"   );
  sprintf ( CstWarnErrType2Str [MIS1__WE_SW_ACQ_TRUNC], "MIS1__WE_SW_ACQ_TRUNC" );
  sprintf ( CstWarnErrType2Str [MIS1__WE_MSIS_FR_OVF] , "MIS1__WE_MSIS_FR_OVF"  );
    
  FReset ();
      
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : ~MIS1__TCCarWarnErr ()
*
* \brief   : MIS1__TCCarWarnErr destructor.
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
* \bug     : There may be a bug in this function, please fix it ;-)
*
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



MIS1__TCCarWarnErr::~MIS1__TCCarWarnErr () {
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32  FReset ()
*
* \brief   : Clear all W/E => Reset all fields.
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     : 
*
* \date    : Date      : 23/03/2021
* \date    : Doc date  : 23/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FReset () {
  
  WarnErrCnt    = 0;
  WarnErrCntTot = 0;
  ListIsFull    = 0;
  
  
  memset ( AWarnErrTotCnt, 0, sizeof(AWarnErrTotCnt[0][0]) * MIS1__WE_MAX_MSIS_NB * MIS1__WE_NB );
  memset ( AWarnErrCnt   , 0, sizeof(AWarnErrCnt[0][0])    * MIS1__WE_MAX_MSIS_NB * MIS1__WE_NB );
  
  memset ( ATrailFlags, 0, sizeof(ATrailFlags[0][0]) * MIS1__WE_MAX_MSIS_NB * MIS1__MAX_FR_NB_PER_ACQ );
  memset ( ATrailFrCnt, 0, sizeof(ATrailFrCnt[0][0]) * MIS1__WE_MAX_MSIS_NB * MIS1__MAX_FR_NB_PER_ACQ );
  
  memset ( AWarnErr   , 0, sizeof(AWarnErr[0]) * MIS1__MAX_CAR_WARN_ERR_NB );
  
  err_retok (( ERR_OUT, "MsgOk" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32  FResetList ()
*
* \brief   : Clear the list of W/E records, W/E counters are not resetted.
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 24/03/2021
* \date    : Doc date  : 24/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FResetList () {
  
  WarnErrCnt = 0;
  ListIsFull = 0;
    
  memset ( ATrailFlags, 0, sizeof(ATrailFlags[0][0]) * MIS1__WE_MAX_MSIS_NB * MIS1__MAX_FR_NB_PER_ACQ );
  memset ( ATrailFrCnt, 0, sizeof(ATrailFrCnt[0][0]) * MIS1__WE_MAX_MSIS_NB * MIS1__MAX_FR_NB_PER_ACQ );
  
  memset ( AWarnErr   , 0, sizeof(AWarnErr[0]) * MIS1__MAX_CAR_WARN_ERR_NB );
  
  err_retok (( ERR_OUT, "MsgOk" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : ~MIS1__TCCarWarnErr ()
*
* \brief   : MIS1__TCCarWarnErr destructor.
*
* \param   : None
*
* \return  : Error code ( On 11/04/2019 it returns always 0 ) \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
* \bug     : There may be a bug in this function, please fix it ;-)
*
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FChkMSisId ( SInt8 MSisId ) {

  if ( (MSisId < 0) || (MSisId >= MIS1__WE_MAX_MSIS_NB) ) {
    return (-1);
  }

  return (MSisId);
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwErrRstTotCnt ( SInt8 MSisId )
*          :
* \brief   : Reset the total sw errors counter   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :  
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwErrRstTotCnt ( SInt8 MSisId  ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  AWarnErrTotCnt[MSisId][MIS1__WE_SW_ERR] = 0;
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfRstTotCnt ( SInt8 MSisId  )
*          :
* \brief   : Reset the total sw errors region ovf 
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfRstTotCnt ( SInt8 MSisId  ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  AWarnErrTotCnt[MSisId][MIS1__WE_SW_REG_OVF] = 0;
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncRstTotCnt ( SInt8 MSisId )
*          :
* \brief   : Reset the total acq truncated total counter
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncRstTotCnt ( SInt8 MSisId ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  AWarnErrTotCnt[MSisId][MIS1__WE_SW_ACQ_TRUNC] = 0;
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FMSisFrOvfRstTotCnt ( SInt8 MSisId )
*          :
* \brief   : Reset the total MSis fr OVF counter   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FMSisFrOvfRstTotCnt ( SInt8 MSisId ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  AWarnErrTotCnt[MSisId][MIS1__WE_MSIS_FR_OVF] = 0;
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FRstAllTotCnt ( SInt8 MSisId )
*          :
* \brief   : Reset ALL the total counters   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FRstAllTotCnt ( SInt8 MSisId  ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  memset ( AWarnErrTotCnt, 0, sizeof(AWarnErrTotCnt[0][0]) * MSisId * MIS1__WE_NB );
  
  return (0);
}








/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwErrGetTotCnt ( SInt8 MSisId )
*          :
* \brief   : Returns the total sw errors counter   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwErrGetTotCnt ( SInt8 MSisId ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  return (AWarnErrTotCnt[MSisId][MIS1__WE_SW_ERR]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfGetTotCnt ()
*          :
* \brief   : Returns the total sw errors region ovf
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfGetTotCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  return (AWarnErrTotCnt[MSisId][MIS1__WE_SW_REG_OVF]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncGetTotCnt ( SInt8 MSisId )
*          :
* \brief   : Returns the total acq truncated total counter
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncGetTotCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  return (AWarnErrTotCnt[MSisId][MIS1__WE_SW_ACQ_TRUNC]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FMSisFrOvfGetTotCnt ( SInt8 MSisId )
*          :
* \brief   : Returns the total MSis fr OVF counter   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FMSisFrOvfGetTotCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  return (AWarnErrTotCnt[MSisId][MIS1__WE_MSIS_FR_OVF]);
}



// 998

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwErrRstCnt ( SInt8 MSisId )
*          :
* \brief   : Reset the sw errors counter   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/03/2021
* \date    : Doc date  : 24/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwErrRstCnt ( SInt8 MSisId  ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  AWarnErrCnt[MSisId][MIS1__WE_SW_ERR] = 0;
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfRstCnt ( SInt8 MSisId  )
*          :
* \brief   : Reset the sw errors region ovf
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/03/2021
* \date    : Doc date  : 24/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfRstCnt ( SInt8 MSisId  ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  AWarnErrCnt[MSisId][MIS1__WE_SW_REG_OVF] = 0;
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncRstCnt ( SInt8 MSisId )
*          :
* \brief   : Reset the acq truncated total counter
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/03/2021
* \date    : Doc date  : 24/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncRstCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  AWarnErrCnt[MSisId][MIS1__WE_SW_ACQ_TRUNC] = 0;
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FMSisFrOvfRstCnt ( SInt8 MSisId )
*          :
* \brief   : Reset the MSis fr OVF counter   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/03/2021
* \date    : Doc date  : 24/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FMSisFrOvfRstCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  AWarnErrCnt[MSisId][MIS1__WE_MSIS_FR_OVF] = 0;
  
  return (0);
}




// 999






/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwErrGetCnt ( SInt8 MSisId )
*          :
* \brief   : Returns the sw errors counter   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwErrGetCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  return (AWarnErrCnt[MSisId][MIS1__WE_SW_ERR]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfGetCnt ()
*          :
* \brief   : Returns the  sw errors region ovf
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfGetCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  return (AWarnErrCnt[MSisId][MIS1__WE_SW_REG_OVF]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncGetCnt ( SInt8 MSisId )
*          :
* \brief   : Returns the  acq truncated total counter
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 02/03/2021
* \date    : Doc date  : 02/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncGetCnt ( SInt8 MSisId ) {
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  return (AWarnErrCnt[MSisId][MIS1__WE_SW_ACQ_TRUNC]);
}



// 888





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FDaqErrAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val, MIS1__EWarnErr ErrType )
*          :
* \brief   : Add a DAQ error, generic function called by FDaqSwErrAdd (), FDaqSwRegOvfrAdd (), FDaqAcqTruncAdd ()     \n
*          :
* \param   :          - 
*          :
* \return  : Errors counter or sw error code
*          : >= 0 - Daq error counter
*          : <  0 - Sw error code
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FDaqErrAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val, MIS1__EWarnErr ErrType ) {
  
  SInt32 VRet;
  MIS1__TWarnErr* VPtWarnErr;
  
  
  
  // Check param

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  // Update errors counters
  
  AWarnErrTotCnt[MSisId][ErrType]++;
  AWarnErrCnt[MSisId][ErrType]++;
  
  WarnErrCntTot++;
  
  // Add record
    
  if ( WarnErrCnt >= MIS1__MAX_CAR_WARN_ERR_NB ) {
    err_warning (( ERR_OUT, "Abort list is full => WarnErrCnt = %d >= MIS1__MAX_CAR_WARN_ERR_NB = %d", WarnErrCnt, MIS1__MAX_CAR_WARN_ERR_NB ));
    return (-2);
  }
  
         
  VPtWarnErr = &AWarnErr[WarnErrCnt];
    
  VPtWarnErr->MSisId    = MSisId;
  VPtWarnErr->RunId     = RunId;
  VPtWarnErr->AcqId     = AcqId;
  VPtWarnErr->FrIdInAcq = FrId;
  VPtWarnErr->StepId    = StepId;
  VPtWarnErr->SubStepId = SubStepId;
  VPtWarnErr->Type      = ErrType;
  VPtWarnErr->MSisFrCnt = MSisFrCnt;
  VPtWarnErr->Cnt       = AWarnErrTotCnt[MSisId][ErrType]; // Total errors counter is stored in record
  VPtWarnErr->Code      = Code;
  VPtWarnErr->Val       = Val;
   
  WarnErrCnt++;
      
     
  return (AWarnErrTotCnt[MSisId][ErrType]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FDaqSwErrAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val )
*          :
* \brief   : Add a DAQ sw error     \n
*          :
* \param   :          -
*          :
* \return  : Errors counter or sw error code
*          : >= 0 - Daq error counter
*          : <  0 - Sw error code
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FDaqSwErrAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val ) {

  return ( FDaqErrAdd ( MSisId, RunId, AcqId, FrId, StepId, SubStepId, MSisFrCnt, Code, Val, MIS1__WE_SW_ERR ) );
 
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FDaqSwRegOvfrAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val )
*          :
* \brief   : Add a DAQ sw region OVF error     \n
*          :
* \param   :          -
*          :
* \return  : Errors counter or sw error code
*          : >= 0 - Daq error counter
*          : <  0 - Sw error code
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FDaqSwRegOvfrAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val ) {

  return ( FDaqErrAdd ( MSisId, RunId, AcqId, FrId, StepId, SubStepId, MSisFrCnt, Code, Val, MIS1__WE_SW_REG_OVF ) );
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FDaqAcqTruncAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val )
*          :
* \brief   : Add a DAQ acq truncated error     \n
*          :
* \param   :          -
*          :
* \return  : Errors counter or sw error code
*          : >= 0 - Daq error counter
*          : <  0 - Sw error code
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FDaqAcqTruncAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val ) {
  
  return ( FDaqErrAdd ( MSisId, RunId, AcqId, FrId, StepId, SubStepId, MSisFrCnt, Code, Val, MIS1__WE_SW_ACQ_TRUNC ) );
  
}







/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfAdd ( SInt8 MSisId, UInt8 OvfFlags, UInt32 MSisFrCnt, UInt32 AcqFrId )
*          :
* \brief   : Add MSis frame OVF flags + fr cnt to the list, to be called on each fr of one acq   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/02/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfAdd ( SInt8 MSisId, UInt8 OvfFlags, UInt32 MSisFrCnt, UInt32 AcqFrId ) {
  
  SInt32 VRet;
  UInt32* VPtCnt;
  UInt32* VPtTotCnt;
  
  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );


  // Since 23/03/2021
  
  VPtCnt    = &AWarnErrCnt[MSisId][MIS1__WE_MSIS_FR_OVF];
  VPtTotCnt = &AWarnErrTotCnt[MSisId][MIS1__WE_MSIS_FR_OVF];
  
  if ( *VPtCnt < UINT_MAX ) {
    (*VPtCnt)++;
  }
  
  if ( *VPtTotCnt < UINT_MAX ) {
    (*VPtTotCnt)++;
  }  

  if ( AcqFrId >= MIS1__MAX_FR_NB_PER_ACQ ) {
    err_warning  (( ERR_OUT, "Abort, update counters only, no list built => AcqFrId = %d >= MIS1__MAX_FR_NB_PER_ACQ = %d", AcqFrId, MIS1__MAX_FR_NB_PER_ACQ ));
    return (-2);
  }
 

  ATrailFlags[MSisId][AcqFrId] = OvfFlags;
  ATrailFrCnt[MSisId][AcqFrId] = MSisFrCnt;
  
 
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfCount ( SInt8 List, SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt16 StepId, UInt16 SubStepId, UInt32 AcqFrNb )
*          :
* \brief   : Update the Acq OVF counters, to be called at end of each acq     \n
*          :
* \param   : List         - List or not the OVF, 0 => count OVF but no list done, 1 => count OVF + build list
*          :
* \return  : Ovf nb + Error code
*          : >= 0 - OVF nb
*          : <  0 - Error
*          :
* \warning : Globals   :  APP_VGResATrail
* \warning : Remark    : Must be called only ONE time at end of ACQ, after counting W/R it resets ACq trailers infos
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/02/2021
* \date    : Rev       : 23/03/2021
* \date    : Doc date  : 26/02/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */
  

SInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfCount (  SInt8 List, SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt16 StepId, UInt16 SubStepId, UInt32 AcqFrNb ) {
  
  SInt32 VRet;
  SInt32 ViFr;
  SInt32 VOvfCnt;
  MIS1__TWarnErr* VPtWarnErr;


  // Check param

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );
  
  if ( AcqFrNb > MIS1__MAX_FR_NB_PER_ACQ ) {
    _FClearAcqTrailInfo  ( MSisId );
    err_retfail ( -1, (ERR_OUT,"Abort => AcqFrNb = %d > MIS1__MAX_FR_NB_PER_ACQ = %d", AcqFrNb, MIS1__MAX_FR_NB_PER_ACQ) );
  }
  

  // Scan frames
  
  
  // Count OVF in current acq in all cases
  

  VOvfCnt = 0;
  
  for ( ViFr = 0; ViFr < AcqFrNb; ViFr++ ) {
      
    if ( ATrailFlags[MSisId][ViFr] > 0 ) {
      VOvfCnt++;
    }
      
  } // End for ( ViFr )
    
  
  // No space in list => Return ovf nb in current acq
  
  if ( WarnErrCnt >= MIS1__MAX_CAR_WARN_ERR_NB ) {
    
    WarnErrCntTot += VOvfCnt;
    
    _FClearAcqTrailInfo  ( MSisId );
    
    // err_warning (( ERR_OUT, "VOvfCnt = %d, MSis OVF cnt = %d, WE nb = %d", VOvfCnt, FMSisFrOvfGetCnt ( 0 /* MSisId */ ), FWarnErrGetNbTot () ));
    
    if ( ListIsFull == 0 ) {
      err_warning (( ERR_OUT, "Abort list is full => WarnErrCnt = %d >= MIS1__MAX_CAR_WARN_ERR_NB = %d", WarnErrCnt, MIS1__MAX_CAR_WARN_ERR_NB ));
      ListIsFull = 1; 
    }
    
    return (VOvfCnt);
   }
        
  
  
  if ( List ) {
    
    for ( ViFr = 0; ViFr < AcqFrNb; ViFr++ ) {
      
      if ( ATrailFlags[MSisId][ViFr] > 0 ) {
          
        if ( WarnErrCnt >= MIS1__MAX_CAR_WARN_ERR_NB ) {
          
          WarnErrCntTot += VOvfCnt;
          
          _FClearAcqTrailInfo  ( MSisId );
                            
          if ( ListIsFull == 0  ) {
            err_warning (( ERR_OUT, "Abort list is full => WarnErrCnt = %d >= MIS1__MAX_CAR_WARN_ERR_NB = %d", WarnErrCnt, MIS1__MAX_CAR_WARN_ERR_NB ));
            ListIsFull = 1;
          }
                    
          return (VOvfCnt);
        }
        
        VPtWarnErr = &AWarnErr[WarnErrCnt];
        
        VPtWarnErr->RunId     = RunId;
        VPtWarnErr->AcqId     = AcqId;
        VPtWarnErr->FrIdInAcq = ViFr;
        VPtWarnErr->StepId    = StepId;
        VPtWarnErr->SubStepId = SubStepId;
        VPtWarnErr->Type      = MIS1__WE_MSIS_FR_OVF;
        VPtWarnErr->MSisFrCnt = ATrailFrCnt[MSisId][ViFr];
        VPtWarnErr->Cnt       = VOvfCnt; // Cpt of MSis 1 OVF in this acquisiiton
        VPtWarnErr->Val       = ATrailFlags[MSisId][ViFr];
        
        WarnErrCnt++;
                
      }
      
    } // End for ( ViFr )
    
  }
  
      
  WarnErrCntTot += VOvfCnt;
    
  // Clear Acq trailers info
  
  _FClearAcqTrailInfo  ( MSisId );
  
  return (VOvfCnt);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : UInt32 MIS1__TCCarWarnErr::FMSisFrOvfGetCnt ( SInt8 MSisId )
*          :
* \brief   : Returns the MSis fr OVF counter, to be called at any time   \n
*          :
* \param   : None         -
*          :
* \return  : MSis fr OVF counter
*          :
* \warning : Globals   :  
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/03/2021
* \date    : Doc date  : 01/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


UInt32 MIS1__TCCarWarnErr::FMSisFrOvfGetCnt ( SInt8 MSisId ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  return (AWarnErrCnt[MSisId][MIS1__WE_MSIS_FR_OVF] );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : UInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfGetFlags ( SInt8 MSisId, UInt32 FrId )
*          :
* \brief   : Returns the MSis OVF flags of one frame of current acq, to be called at any time   \n
*          :
* \param   : None         -
*          :
* \return  : MSis fr OVF counter
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/03/2021
* \date    : Doc date  : 01/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


UInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfGetFlags ( SInt8 MSisId, UInt32 FrId ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  if ( FrId >= MIS1__MAX_FR_NB_PER_ACQ ) {
    err_retfail ( -1, (ERR_OUT,"Abort => FrId = %d >= MIS1__MAX_FR_NB_PER_ACQ = %d", FrId, MIS1__MAX_FR_NB_PER_ACQ ) );
  }
  
  return (ATrailFlags[MSisId][FrId]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : UInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfGetFrCnt ( SInt8 MSisId, UInt32 FrId )
*          :
* \brief   : Returns the MSis fr cnt of one frame of current acq, to be called at any time   \n
*          :
* \param   : None         -
*          :
* \return  : MSis fr OVF counter
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/03/2021
* \date    : Doc date  : 01/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


UInt32 MIS1__TCCarWarnErr::FAcqMSisFrOvfGetFrCnt ( SInt8 MSisId , UInt32 FrId ) {

  err_retfail ( FChkMSisId (MSisId), (ERR_OUT,"Abort => MSisId = %d is not allowed", MSisId ) );

  if ( FrId >= MIS1__MAX_FR_NB_PER_ACQ ) {
    err_retfail ( -1, (ERR_OUT,"Abort => FrId = %d >= MIS1__MAX_FR_NB_PER_ACQ = %d", FrId, MIS1__MAX_FR_NB_PER_ACQ ) );
  }
  
  return (ATrailFrCnt[MSisId][FrId]);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FWarnErrGetNb ()
*          :
* \brief   : Returns the warning, erros nb, to be called at any time   \n
*          :
* \param   : None         -
*          :
* \return  :
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/03/2021
* \date    : Doc date  : 01/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FWarnErrGetNb () {

  return ( WarnErrCnt );
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FWarnErrGetNbTot ()
*          :
* \brief   : Returns the warning, erros nb, to be called at any time   \n
*          :
* \param   : None         -
*          :
* \return  :
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/03/2021
* \date    : Doc date  : 01/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FWarnErrGetNbTot () {
  
  return ( WarnErrCntTot );
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FWarnErrPrint ( UInt32 First, UInt32 Nb, SInt8 PrintListHeader, SInt8 PrintDest, TMemo* Memo, FILE* PtFile )
*          :
* \brief   : Returns the warning, erros nb, to be called at any time   \n
*          :
* \param   : PrintDest - Print destination,
*          :   0 => No print,
*          :   1 => msg file , 2 => Memo, 3 => Msg file + Memo
*          :   4 => text file, 5 => Text file + Memo
*          :
* \return  :
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 01/03/2021
* \date    : Doc date  : 01/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FWarnErrPrint ( UInt32 First, UInt32 Nb, SInt8 PrintListHeader, SInt8 PrintDest, TMemo* Memo, FILE* PtFile ) {

  UInt32 Vi;
  UInt32 ViLast;
  MIS1__TWarnErr* VPtRec;
  char   VMsg[GLB_CMT_SZ];

  // Check param

  ViLast = First + Nb - 1;

  if ( First >= WarnErrCnt ) {
    err_retfail ( -1, (ERR_OUT,"Abort record out of list => First = %d >= WarnErrCnt = %d", First, WarnErrCnt) );
  }

  if ( ViLast >= WarnErrCnt ) {
    err_retfail ( -1, (ERR_OUT,"Abort record out of list => ViLast = %d >= WarnErrCnt = %d", ViLast, WarnErrCnt) );
  }

  // Print
  
  if ( PrintListHeader ) {
    MIS1__FPrintStr ( "==================================", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( " Characterization Warning / Error ", PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "----------------------------------", PrintDest, Memo, PtFile );
    sprintf ( VMsg, "Total nb of W/E   = %d", FWarnErrGetNbTot () );
    MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
    sprintf ( VMsg, "Nb of W/E in file = %d", FWarnErrGetNb () );
    MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
    MIS1__FPrintStr ( "==================================", PrintDest, Memo, PtFile );
  }
  
  
  
  for (  Vi = First;  Vi <= ViLast;  Vi++ ) {
    
    VPtRec = &AWarnErr[Vi];
    
    MIS1__FPrintStr ( "--------------------------------------------", PrintDest, Memo, PtFile );
       
    sprintf ( VMsg, "Rec [%.4d] : MSisId = %d, Run = %d, Acq = %d, FrInAcq = %d, Step = %d, SubStep = %d ",  Vi, VPtRec->MSisId, VPtRec->RunId, VPtRec->AcqId, VPtRec->FrIdInAcq, VPtRec->StepId, VPtRec->SubStepId );
    MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
  
    sprintf ( VMsg, "Rec [%.4d] : Type = %s : Cnt = %d, Code = %d, Val = %d, MSis fr cnt = %d", Vi, CstWarnErrType2Str[VPtRec->Type], VPtRec->Cnt, VPtRec->Code, VPtRec->Val, VPtRec->MSisFrCnt  );
    MIS1__FPrintStr ( VMsg, PrintDest, Memo, PtFile );
    
    if  ( (Vi % 100) == 0 ) {
      Application->ProcessMessages ();
    }    

  }
   
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FBinFileSave ( char* FileName )
*          :
* \brief   : Save warning / errors list to a binary file   \n
*          :
* \param   : FileName - Name of the file
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 23/03/2021
* \date    : Doc date  : 23/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FBinFileSave ( char* FileName ) {
  
  SInt32         VRet;
  UInt32         VMaxRecSz;
  FIL__TCBinFile VBinFile ( ERR_FGetLogFilePath (), ERR_FGetFileLogEnabled () /* EnableErrLog */, ERR_FGetFileLogLevel () );
  
  // Check param
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  // Calc size of biggest record to be written by "PubFSeqWrite ()" to file
  
  VMaxRecSz = MIS1__MAX_CAR_WARN_ERR_NB * sizeof ( MIS1__TWarnErr );
  
  // Conf TCBinFile
  
  VRet = VBinFile.PubFConf       ( FileName, FIL__TCBinFile_RWB_MODE_WRITE, VMaxRecSz, VMaxRecSz, 1 /* FlushAfterWrite */, 0 /* MeasTime */ );
  err_retfail ( VRet, (ERR_OUT,"Abort=> Conf for file %s creation failed !", FileName ) );
  
  // Create file
  
  VRet = VBinFile.PubFCreate ();
  err_retfail ( VRet, (ERR_OUT,"Abort=> File %s creation failed !", FileName ) );
  
  
  // Write safety tag
  
  VRet = VBinFile.PubFSeqWrite ( &SafetyTag, sizeof (SafetyTag) );
  err_retfail ( VRet, (ERR_OUT,"Abort=> Saving record to file %s failed !", FileName ) );
  
  
  // Write date + version
  
  VRet = VBinFile.PubFSeqWrite ( &DateVersion, sizeof (DateVersion) );
  err_retfail ( VRet, (ERR_OUT,"Abort=> Saving record to file %s failed !", FileName ) );
  
  // Write records nb
    
  VRet = VBinFile.PubFSeqWrite ( &WarnErrCnt, sizeof (WarnErrCnt) );
  err_retfail ( VRet, (ERR_OUT,"Abort=> Saving record to file %s failed !", FileName ) );
     
  // Write tot warnings, errors nb
  
  VRet = VBinFile.PubFSeqWrite ( &WarnErrCntTot, sizeof (WarnErrCntTot) );
  err_retfail ( VRet, (ERR_OUT,"Abort=> Saving record to file %s failed !", FileName ) );
     
  // Write records
  
  VRet = VBinFile.PubFSeqWrite ( AWarnErr, MIS1__MAX_CAR_WARN_ERR_NB * sizeof ( MIS1__TWarnErr ) );
  err_retfail ( VRet, (ERR_OUT,"Abort=> Saving record to file %s failed !", FileName ) );
  
  
  VRet = VBinFile.PubFClose ();
  err_retfail ( VRet, (ERR_OUT,"Abort=> Closing file %s failed !", FileName ) );
  
  err_retok (( ERR_OUT, "" ));

}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FBinFileSave ( char* Directory, char* FileName )
*          :
* \brief   : Save warning / errors list to a binary file   \n
*          :
* \param   : FileName - Name of the file
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/03/2021
* \date    : Doc date  : 24/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FBinFileSave ( char* Directory, char* FileName ) {
  
  char VFilePath[GLB_FILE_PATH_SZ];
   
  sprintf ( VFilePath, "%s\\%s", Directory, FileName );
  
  return ( FBinFileSave ( VFilePath ) );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FBinFileLoad ( char* FileName )
*          :
* \brief   : Load warning / errors list to a binary file   \n
*          :
* \param   : FileName - Name of the file
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 23/03/2021
* \date    : Doc date  : 23/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FBinFileLoad ( char* FileName ) {
  
  SInt32         VRet;
  UInt32         VMaxRecSz;
  UInt32         VRecSz;
  UInt32         VDateVersionFromFile;
  UInt32         VSafetyTagFromFile;
  FIL__TCBinFile VBinFile ( ERR_FGetLogFilePath (), ERR_FGetFileLogEnabled () /* EnableErrLog */, ERR_FGetFileLogLevel () );
  
  // Check param
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  
  // Reset class fields
  
  FReset ();
    
  
  
  // Calc size of biggest record to be written by "PubFSeqWrite ()" to file
  
  VMaxRecSz = MIS1__MAX_CAR_WARN_ERR_NB * sizeof ( MIS1__TWarnErr );
  
  // Conf TCBinFile
  
  VRet = VBinFile.PubFConf ( FileName, FIL__TCBinFile_RWB_MODE_READ, VMaxRecSz, VMaxRecSz, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
    
  
  err_retfail ( VRet, (ERR_OUT,"Abort=> Conf for file %s loading failed !", FileName ) );
  
  // Open file  
  
  VRet = VBinFile.PubFOpen();
  err_retfail ( VRet, (ERR_OUT,"Abort=> Loading file %s failed !", FileName ) );
  
  
  // Read safety tag
  
  VRecSz = sizeof (SafetyTag);
  
  VRet = VBinFile.PubFSeqRead  ( &VSafetyTagFromFile, VRecSz /* MaxDestSz */, VRecSz /* DataSzToRead */ );
  
  if ( VRet < 0 ) {
    VBinFile.PubFClose ();
    err_retfail ( VRet, (ERR_OUT,"Abort=> Reading safety tag from  file %s failed !", FileName ) );
  }
  
  if ( VSafetyTagFromFile != SafetyTag ) {
    VBinFile.PubFClose ();
    err_retfail ( VRet, (ERR_OUT,"Abort=> SafetyTag from file = %X <> Class SafetyTag = %X", VSafetyTagFromFile, SafetyTag ) );
  }
  
  
  
  
  // Read date + version
  
  VRecSz = sizeof (DateVersion);
  
  VRet = VBinFile.PubFSeqRead  ( &VDateVersionFromFile, VRecSz /* MaxDestSz */, VRecSz /* DataSzToRead */ );
  
  if ( VRet < 0 ) {
    VBinFile.PubFClose ();
    err_retfail ( VRet, (ERR_OUT,"Abort=> Reading date / version from  file %s failed !", FileName ) );
  }  
  
  if ( VDateVersionFromFile != DateVersion ) {
    VBinFile.PubFClose ();
    err_retfail ( VRet, (ERR_OUT,"Abort=> DateVersion from file = %X <> Class DateVersion = %X", VDateVersionFromFile, DateVersion ) );
  }
  
  // Read records nb
  
  VRecSz = sizeof (WarnErrCnt);
  
  VRet = VBinFile.PubFSeqRead  ( &WarnErrCnt, VRecSz /* MaxDestSz */, VRecSz /* DataSzToRead */ );
  
  if ( VRet < 0 ) {
    VBinFile.PubFClose ();
    err_retfail ( VRet, (ERR_OUT,"Abort=> Reading records nb from  file %s failed !", FileName ) );
  }
  
  // Read tot warnings, erros nb
  
  VRecSz = sizeof (WarnErrCntTot);
  
  VRet = VBinFile.PubFSeqRead  ( &WarnErrCntTot, VRecSz /* MaxDestSz */, VRecSz /* DataSzToRead */ );
  
  if ( VRet < 0 ) {
    VBinFile.PubFClose ();
    err_retfail ( VRet, (ERR_OUT,"Abort=> Reading total warnings, errors nb from  file %s failed !", FileName ) );
  }
    
  
  // Read records
    
  VRecSz = MIS1__MAX_CAR_WARN_ERR_NB * sizeof ( MIS1__TWarnErr );
  
  VRet = VBinFile.PubFSeqRead  ( AWarnErr, VRecSz /* MaxDestSz */, VRecSz /* DataSzToRead */ );
  
  if ( VRet < 0 ) {
    VBinFile.PubFClose ();
    err_retfail ( VRet, (ERR_OUT,"Abort=> Reading records from  file %s failed !", FileName ) );
  }
  
  
  VRet = VBinFile.PubFClose ();
  err_retfail ( VRet, (ERR_OUT,"Abort=> Closing file %s failed !", FileName ) );
  
  err_retok (( ERR_OUT, "" ));  
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 FBinFileLoad ( char* Directory, char* FileName )
*          :
* \brief   : Load warning / errors list to a binary file   \n
*          :
* \param   : FileName - Name of the file
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 24/03/2021
* \date    : Doc date  : 24/03/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__TCCarWarnErr::FBinFileLoad ( char* Directory, char* FileName ) {

  char VFilePath[GLB_FILE_PATH_SZ];
  
  sprintf ( VFilePath, "%s\\%s", Directory, FileName );
  
  return ( FBinFileLoad ( VFilePath ) );
  
}


#endif // End of #ifndef CC_MSIS1_BDF_LIGHT



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FAcqIdsRecPrint ( MIS1__TBtAcqIds* Pt )
*
* \brief   :
*
* \param   : Pt                - Pointer to record
*
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 14/05/2021
* \date    : Doc date  : 14/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__BT_FAcqIdsRecPrint ( MIS1__TBtAcqIds* Pt ) {

  // Check param
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  
  // Print
  
  
  msg (( MSG_OUT, "----------------------------------------" ));
  msg (( MSG_OUT, "        Acq Ids fields  " ));
  msg (( MSG_OUT, "----------------------------------------" ));
  msg (( MSG_OUT, "" ));  
  msg (( MSG_OUT, " AcqIdInDaq    = %.4d", Pt->AcqIdInDaq ));
  msg (( MSG_OUT, " AcqIdInRun    = %.4d", Pt->AcqIdInRun ));
  msg (( MSG_OUT, " AcqIdInBuff   = %.4d", Pt->AcqIdInBuff ));
  msg (( MSG_OUT, " AcqIdInFile   = %.4d", Pt->AcqIdInFile ));
  msg (( MSG_OUT, " FileId        = %.4d", Pt->FileId ));
  msg (( MSG_OUT, " TimeStamp1    = %.4d", Pt->TimeStamp1 ));
  msg (( MSG_OUT, " TimeStamp2    = %.4d", Pt->TimeStamp2 ));
  msg (( MSG_OUT, " DataType      = %.4d", Pt->DataType ));
  
  
  err_retok (( ERR_OUT, "Ok" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FAcqResRecPrint ( MIS1__TBtAcqRes* Pt, SInt32 PrintFrCnt, SInt32 PrintFiredPixels )
*
* \brief   :
*
* \param   : Pt                - Pointer to record
*
* \param   : PrintFrCnt        - Print frames counters values, < 0 => All, 0 => None, > 0 => PrintFrCnt frames
*
* \param   : PrintFiredPixels  - Print fired pixels 0 / 1
*
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 14/05/2021
* \date    : Doc date  : 14/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__BT_FAcqResRecPrint ( MIS1__TBtAcqRes* Pt, SInt32 PrintFrCnt, SInt32 PrintFiredPixels ) {

  SInt32 ViFr;
  SInt8  ViMSis;


  // Check param
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );


  // Print
  
  msg (( MSG_OUT, "----------------------------------------" ));
  msg (( MSG_OUT, "        Acq results  " ));
  msg (( MSG_OUT, "----------------------------------------" ));
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "ErrNb          = %.4d", Pt->ErrNb ));
  msg (( MSG_OUT, "TrigNb         = %.4d", Pt->TrigNb ));
  msg (( MSG_OUT, "FrNb           = %.4d", Pt->FrNb ));
  msg (( MSG_OUT, "FrNbTruncated  = %.4d", Pt->FrNbTruncated ));
  msg (( MSG_OUT, "FrNbOvf        = %.4d", Pt->FrNbOvf ));
  msg (( MSG_OUT, "FrNbSwErr      = %.4d", Pt->FrNbSwErr ));
  msg (( MSG_OUT, "" ));


  if ( PrintFrCnt < 0 ) {
    PrintFrCnt = Pt->FrNb;
  }

  if ( PrintFrCnt ) {
    
    msg (( MSG_OUT, "Frame conters list" ));
    
    for ( ViFr = 0; ViFr < PrintFrCnt; ViFr++ ) {
      msg (( MSG_OUT, "Frame[%.4d] Fr cnt MSis : [0] = %.6d, [1] = %.6d, [2] = %.6d, [3] = %.6d, [4] = %.6d, [5] = %.6d ", ViFr, Pt->FrCnt[ViFr][0], Pt->FrCnt[ViFr][1], Pt->FrCnt[ViFr][2], Pt->FrCnt[ViFr][3], Pt->FrCnt[ViFr][4], Pt->FrCnt[ViFr][5] ));
    }
    
    msg (( MSG_OUT, "" ));
  }


  msg (( MSG_OUT, "Tot fired pixels nb = %.4d", Pt->FiredPixNb ));
  
  // 14/05/2021
    
  if ( PrintFiredPixels ) {
    
    for ( ViMSis = 0; ViMSis < MIS1__BT_MAX_REAL_MSIS_NB_ACQ; ViMSis++ ) {
      msg (( MSG_OUT, "Fired pixels MSis [%d] Matrix [0] = %.4d, [1] = %.4d[2] = %.4d, [3] = %.4d", ViMSis , Pt->AFiredPixNb[ViMSis][0], Pt->AFiredPixNb[ViMSis][1], Pt->AFiredPixNb[ViMSis][2], Pt->AFiredPixNb[ViMSis][3] ));
    }
    
  }
    

  err_retok (( ERR_OUT, "Ok" ));        
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FTrigRecPrint ( SInt32 TrigNb, MIS1__TBtTrigRec* Pt )
*
* \brief   :
*
* \param   : TrigNb - Nmber of triggers to print if > 0, -1 => All, 0 => None
*
* \param   : Pt     - Pointer to record
*
* \param   : 
*
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 04/05/2021
* \date    : Doc date  : 04/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__BT_FTrigRecPrint ( SInt32 TrigNb, MIS1__TBtTrigRec* Pt ) {

  UInt32 ViTrig;

  // Check param
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  
  if ( TrigNb < 0 ) {
    TrigNb = Pt->TrigNb;
  }
  
  if ( TrigNb > MIS1__BT_MAX_TRIG_NB_PER_ACQ ) {
    err_warning (( ERR_OUT, "TrigNb = %d > MIS1__BT_MAX_TRIG_NB_PER_ACQ = %d => Prints only first %d", TrigNb, MIS1__BT_MAX_TRIG_NB_PER_ACQ, MIS1__BT_MAX_TRIG_NB_PER_ACQ ));
    TrigNb = MIS1__BT_MAX_TRIG_NB_PER_ACQ;
  }
  
  
  if ( TrigNb > Pt->TrigNb ) {
    err_warning (( ERR_OUT, "TrigNb = %d > TrigRec.TrigNb = %d => Prints only first %d", TrigNb, Pt->TrigNb, Pt->TrigNb ));
    TrigNb = Pt->TrigNb;
  }
  
    
  // Print
  
  msg (( MSG_OUT, "----------------------------------------" ));
  msg (( MSG_OUT, "        Triggers list  " ));
  msg (( MSG_OUT, "----------------------------------------" ));
  msg (( MSG_OUT, "Trig nb = %d", Pt->TrigNb  ));
  msg (( MSG_OUT, "" ));
  
  
  
  for ( ViTrig = 0; ViTrig < TrigNb; ViTrig++ ) {
    msg (( MSG_OUT, "Trig [%.4d] = %.4d [D]= %.4X [H]", ViTrig, Pt->ATrig[ViTrig], Pt->ATrig[ViTrig] ));
  }
  
  
  err_retok (( ERR_OUT, "Ok" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FAcqRawHeadRecPrint ( MIS1__TBtAcqRawHead* Pt, SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels  )
*
* \brief   :
*
* \param   : Pt                - Pointer to record
*
* \param   : PrintTriggers     - Print triggers, -1 => All, 0 => No, > 0 => value = nb of triggers to print
*
* \param   : PrintFrCnt        - Print frames counters, -1 => All, 0 => No, > 0 => value = nb of frames to print
*
* \param   : PrintFiredPixels  - Print fired pixels, 0 / 1
*
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 14/05/2021
* \date    : Doc date  : 14/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__BT_FAcqRawHeadRecPrint ( MIS1__TBtAcqRawHead* Pt, SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels ) {


  // Check param

  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  
  // Print
  
  msg (( MSG_OUT, "****************************************" ));
  msg (( MSG_OUT, "        Acq header  " ));
  msg (( MSG_OUT, "****************************************" ));
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "TagBeg     = %.8X", Pt->TagBeg  ));
  msg (( MSG_OUT, "Offset     = %.4d", Pt->Offset  ));
  msg (( MSG_OUT, "FullRecSz  = %.4d", Pt->FullRecSz ));
  msg (( MSG_OUT, "DataSz     = %.4d", Pt->DataSz  ));
  msg (( MSG_OUT, "" ));
      

  MIS1__BT_FAcqIdsRecPrint ( &Pt->Ids );
  
  
  if ( PrintTriggers != 0 ) {
    MIS1__BT_FTrigRecPrint ( PrintTriggers /* TrigNb */, &Pt->Trigs );
  }
  
  MIS1__BT_FAcqResRecPrint   ( &Pt->Res, PrintFrCnt, PrintFiredPixels );
      
  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FAcqRawPrint ( MIS1__TBtAcqRawRec* Pt, SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels, UInt8 DataWSize, UInt32 FirstDataW, UInt32 NbDataW, UInt8 VRS )
*
* \brief   : 
*
* \param   : Pt - Pointer to acq raw  /
*
* \param   : PrintTriggers      - Print triggers, < 0 => ALl, 0 => No, value > 0 => value = nb of triggers to print
*
* \param   : PrintFrCnt         - Print frames counters, < 0 => ALL, 0 => No, > 0 for PrintFrCnt frames
*
* \param   : PrintFiredPixels   - Print fired pixels for all MSis, all matrices 0 / 1

* \param   : DataWSize          - Size of W to print for data part, can be 8, 16, 32, 64 bits
*
* \param   : FirstDataW         - First W to print
*
* \param   : NbDataWW           - Nb W to print
*
* \param   : VRS                 - Record type 0 => Fixed Record Size, 1 => Variable Record Size
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 04/05/2021
* \date    : Doc date  : 04/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 MIS1__BT_FAcqRawPrint ( MIS1__TBtAcqRawRec* Pt, SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels, UInt8 DataWSize, UInt32 FirstDataW, UInt32 NbDataW, UInt8 VRS ) {


  SInt32 VRet;
  SInt32 ViW;      
  SInt32 VLastW; // Signed to handle case NbDataW == 0 => VLastW = -1
  SInt32 VMaxAcqDataWSz; // Signed to handle case NbDataW == 0 => VLastW = -1
  UInt32 V64High;
  UInt32 V64Low;


  // Check param

  if ( VRS > 1 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => VRS = %d <> 0, 1", VRS) );
  }

  // ****************************
  // Find max data part size
  // ****************************
  

  if ( VRS == 0 ) {

    switch ( DataWSize ) {

      case 8 : {
        VMaxAcqDataWSz = MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W8;
        break; }

      case 16 : {
        VMaxAcqDataWSz = MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W16;
        break; }

      case 32 : {
        VMaxAcqDataWSz = MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W32;
        break; }

      case 64 : {
        VMaxAcqDataWSz = MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W64;
        break; }

      default : {
        err_retfail ( -1, (ERR_OUT,"Abort => DataWSize = %d <> 8, 16, 32, 64", DataWSize ) );
        break; }
    }

  }

  else {


    switch ( DataWSize ) {

      case 8 : {
        VMaxAcqDataWSz = MIS1__BT_VRS_MAX_ACQ_DATA_SZ_W8;
        break; }

      case 16 : {
        VMaxAcqDataWSz = MIS1__BT_VRS_MAX_ACQ_DATA_SZ_W16;
        break; }

      case 32 : {
        VMaxAcqDataWSz = MIS1__BT_VRS_MAX_ACQ_DATA_SZ_W32;
        break; }

      case 64 : {
        VMaxAcqDataWSz = MIS1__BT_VRS_MAX_ACQ_DATA_SZ_W64;
        break; }

      default : {
        err_retfail ( -1, (ERR_OUT,"Abort => DataWSize = %d <> 8, 16, 32, 64", DataWSize ) );
        break; }
    }


  }



  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );

  VLastW = FirstDataW + NbDataW - 1;



  
  if ( NbDataW > VMaxAcqDataWSz ) {
    err_retfail ( -1, (ERR_OUT,"Abort => NbDataW%d = %d > Max nb = %d ", DataWSize, NbDataW, VMaxAcqDataWSz ) );
  }
  
  if ( VLastW >= VMaxAcqDataWSz ) {
    err_retfail ( -1, (ERR_OUT,"Abort => VLastW%d = %d >= Max nb = %d ", DataWSize, VLastW, VMaxAcqDataWSz ) );
  }
  
  
  // ****************************
  // Print
  // ****************************
  
  
  // Print header
  
  MIS1__BT_FAcqRawHeadRecPrint ( &Pt->Head, PrintTriggers, PrintFrCnt, PrintFiredPixels );

  // Print data part
  
  
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "----------------------------------------" ));
  msg (( MSG_OUT, " Data fields " ));


  switch ( DataWSize ) {


    case 8 : {

      for ( ViW = FirstDataW; ViW <= VLastW; ViW++ ) {
        msg (( MSG_OUT, "Data U8[%.4d] = %.2X", ViW, Pt->MSisData.Au8[ViW] ));
      }

      break; }


    case 16 : {

      for ( ViW = FirstDataW; ViW <= VLastW; ViW++ ) {
        msg (( MSG_OUT, "Data U16[%.4d] = %.4X", ViW, Pt->MSisData.Au16[ViW] ));
      }

      break; }


    case 32 : {

      for ( ViW = FirstDataW; ViW <= VLastW; ViW++ ) {
        msg (( MSG_OUT, "Data U32[%.4d] = %.8X", ViW, Pt->MSisData.Au32[ViW] ));
      }

      break; }


    case 64 : {

      for ( ViW = FirstDataW; ViW <= VLastW; ViW++ ) {
        
        V64High = (Pt->MSisData.Au64[ViW] & 0xFFFFFFFF00000000) >> 32;
        V64Low  = (Pt->MSisData.Au64[ViW] & 0x00000000FFFFFFFF);
        msg (( MSG_OUT, "Data U64[%.4d] = %.8X %.8X", ViW, V64High, V64Low ));
      }

      break; }

  
  }


  err_retok (( ERR_OUT, "Ok" ));
      
  
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FAcqRawRecCheckSz ( UInt32* PtSz )
*
* \brief   : Check AcqRaw record size / compiler options
*
* \param   : PtSz - Pointer to get size, optional => set to NULL if not used
*
*
*
* \return  :   \n
*          :   1 - Size if OK                                \n
*          :   0 - Size if not = size under Windows => Check compiler options  \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 12/05/2021
* \date    : Doc date  : 12/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__BT_FAcqRawRecCheckSz ( UInt32* PtSz ) {
  
  UInt32 VSz;
  
  VSz = sizeof (MIS1__TBtAcqRawRec);


  if ( PtSz != NULL ) {
    *PtSz = VSz;
  }

  if ( VSz == 100 ) {
    return (1);
  }


  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FUserEndianConv ( void* Pt, UInt32 Sz )
*
* \brief   : Convert binary data if CPU is not little endian
*
* \param   : Pt - Pointer to record to convert
*
* \param   : Sz - record size in W8

* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 19/05/2021
* \date    : Doc date  : 19/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FUserEndianConv ( void* Pt, UInt32 Sz ) {

  // Pointers to access the source buffer as W8, W16 or W32

  UInt8*  VPtW8  = (UInt8*)  Pt;
  UInt16* VPtW16 = (UInt16*) Pt;
  UInt32* VPtW32 = (UInt32*) Pt;

  // RAW data from DAQ are saved in little endian bytes ordering because CPU is Intel
  // If this library is executed by a CPU which is not little endian, byte must be swapped
  // Motorola CPU for exampel are bg endian
  
  
  // To convert byte order you can either use the source buffer and a small temporary buffer
  // of few bytes to convert for example W32 by W32. You can also allocate a big buffer in
  // this function is it is easier or more efficient but don't forget to free it at end of function
  
  
  err_warning (( ERR_OUT, "User endian conversion function called BUT it does nothing => User should implement little / big endian conversion !" )); // Remove this message when data conv will be implemented
  
  
  // The function must returns 0 if ok, -1 or any negative number in case of error
  
  
  // return (-1); // Value to return in case of error
  
  
  return (0); // Value returned in case there is no error
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecordLoad ( char* FileName, void* Pt, UInt32 Sz, UInt8 FormatVers )
*
* \brief   : Loads a record from disk + check HeadChk fields
*
* \param   : FileName - File
*
* \param   : Pt - Pointer to record
*
* \param   : Sz - record size in W8

* \return  : Error code  \n
*          : 0, 1  - OK  1 = endian conversion done, 0 = no conversion done \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FRecordLoad ( char* FileName, void* Pt, UInt32 Sz, UInt8 FormatVers ) {
  
  SInt32 VRet;
  SInt32 VEndianConvDone;
  FILE*  VPfSrc;
  SInt32 VReadSz;
  UInt32 VExpectedTagBegin;
  // MIS1__TBtRecHeadChk* VPtHeadChk = ( MIS1__TBtRecHeadChk*) Pt;
  
  MIS1__TBtRecHeadChk* VPtHeadChk;
  
  
  VPtHeadChk = ( MIS1__TBtRecHeadChk*) Pt;
  
  // Chekc param
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  err_retnull ( Sz, (ERR_OUT,"Abort => Sz == 0") );
  
  // Init
  
  VExpectedTagBegin = MIS1__BT_REC_TAG_BEGIN | FormatVers;
  
  VEndianConvDone = 0;
  
  // Reset destination record
  
  memset ( Pt, 0, Sz );
   
  // Read
  
  
  VPfSrc  = fopen ( FileName, "rb" );
  
  err_retnull ( VPfSrc, (ERR_OUT,"Source file %s open failed", FileName ) );
  
  VReadSz = fread ( Pt, 1 /* byte size */ , Sz /* bytes number */, VPfSrc );
  
  // Check size read
  
  if ( VReadSz != Sz ) {
    fclose (VPfSrc);
    err_retfail ( -1, (ERR_OUT,"Abort => Record sz on disk = %d W8 <> Record size = %d => W linker align pb or bad format", VReadSz, Sz  ) );
  }
  
  // Close file
  
  fclose (VPfSrc);
  
  
  // Check endianness => Call conersion function if needed
  
  if ( VPtHeadChk->TagEndian !=  MIS1__BT_REC_TAG_ENDIAN ) {
    
    VEndianConvDone = 1;

    err_warning (( ERR_OUT, "CPU must be little endian, it is %d (0 => little endian, 1 => big endian, -1 => ?)", MIS1__FIsCpuBigEndian () ));
    
    VRet = MIS1__FUserEndianConv ( Pt, Sz );
    
    err_retfail ( VRet, (ERR_OUT,"Abort => User endian conversion function MIS1__FUserEndianConv ( Pt, Sz ) has failed ! Ret = %d", VRet) );
        
  }
  
  
  
  // Check endianness + begin tag after endian conversion
  
  if ( VPtHeadChk->TagEndian !=  MIS1__BT_REC_TAG_ENDIAN ) {
    
    err_error (( ERR_OUT, "CPU must be little endian, it is %d (0 => little endian, 1 => big endian, -1 => ?)", MIS1__FIsCpuBigEndian () ));
    
    err_retfail ( -1, (ERR_OUT,"Abort => Tag endian field = %X <> MIS1__BT_REC_TAG_ENDIAN = %X", VPtHeadChk->TagEndian, MIS1__BT_REC_TAG_ENDIAN ) );
  }
  
  
  if ( VPtHeadChk->TagBeg != VExpectedTagBegin ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Tag begin field = %X <> (MIS1__BT_REC_TAG_BEGIN | Version) = %X", VPtHeadChk->TagBeg, VExpectedTagBegin ) );
  }
   
      
  return ( VEndianConvDone );
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__FRecordSave ( char* FileName, void* Pt, UInt32 Sz, UInt8 FormatVers )
*
* \brief   : Saves a record to disk
*
* \param   : FileName - File
*
* \param   : Pt - Pointer to record
*
* \param   : Sz - record size in W8
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__FRecordSave ( char* FileName, void* Pt, UInt32 Sz, UInt8 FormatVers ) {
  
  FILE*  VPfDest;
  SInt32 VWriteSz;
  SInt32 VRet;
  // MIS1__TBtRecHeadChk* VPtHeadChk = ( MIS1__TBtRecHeadChk*) Pt;
  
  MIS1__TBtRecHeadChk* VPtHeadChk;
  
  
  VPtHeadChk = ( MIS1__TBtRecHeadChk*) Pt;
  
  // Chekc param
  
  err_retnull ( FileName, (ERR_OUT,"Abort => FileName == NULL") );
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL") );
  
  err_retnull ( Sz, (ERR_OUT,"Abort => Sz == 0") );
  
  
  // Set tags
  
  VPtHeadChk->TagBeg     = MIS1__BT_REC_TAG_BEGIN | FormatVers;
  VPtHeadChk->TagEndian  = MIS1__BT_REC_TAG_ENDIAN;
  
  // Write
  
  
  VPfDest  = fopen ( FileName, "wb" );
  
  err_retnull ( VPfDest, (ERR_OUT,"Destination file %s open failed", FileName ) );
  
  VWriteSz = fwrite ( Pt, 1 /* byte size */, Sz, VPfDest );
  
  
  // Check
  
  if ( VWriteSz != Sz ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Record sz written on disk = %d W8 <> Record size = %d => File I/O problem", VWriteSz, Sz  ) );
  }
    
   
  fclose (VPfDest);
  
  err_retok (( ERR_OUT, "Ok" ));
  
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FBtRunCnfRecLoad ( char* FileName, MIS1__TBtRunCnfRec* Pt, UInt8 FormatVers )
*
* \brief   : Loads from disk run conf record
*
* \param   : FileName - File
*
* \param   : Pt - Pointer to record
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__BT_FBtRunCnfRecLoad ( char* FileName, MIS1__TBtRunCnfRec* Pt, UInt8 FormatVers ) {

  SInt32 VRet;

  VRet =  MIS1__FRecordLoad ( FileName, Pt, sizeof (MIS1__TBtRunCnfRec), FormatVers );

  err_retfail ( VRet, (ERR_OUT,"Load run conf record has failed ! Ret = %d", VRet) );
  
  err_retok (( ERR_OUT, "Ok" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FBtRunCnfRecSave ( char* FileName, MIS1__TBtRunCnfRec* P, UInt8 FormatVerst )
*
* \brief   : Saves run conf recordto disk
*
* \param   : FileName - File
*
* \param   : Pt - Pointer to record
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__BT_FBtRunCnfRecSave ( char* FileName, MIS1__TBtRunCnfRec* Pt, UInt8 FormatVers ) {
  
  SInt32 VRet;
  
  VRet = MIS1__FRecordSave ( FileName, Pt, sizeof (MIS1__TBtRunCnfRec), FormatVers );
  
  err_retfail ( VRet, (ERR_OUT,"Save run conf record has failed ! Ret = %d", VRet) );
  
  err_retok (( ERR_OUT, "Ok" ));  
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FBtRunCnfRecPrint ( MIS1__TBtRunCnfRec* Pt )
*
* \brief   : Prints run conf record
*
* \param   : Pt - Pointer to record
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__BT_FBtRunCnfRecPrint ( MIS1__TBtRunCnfRec* Pt ) {
  
  
  // Check param
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL" ) );
  
  // Print
  
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "===========================================================================" ));
  msg (( MSG_OUT, " Run config record" ));
  msg (( MSG_OUT, "===========================================================================" ));
  msg (( MSG_OUT, "" ));
  
  
  msg (( MSG_OUT, "TagBeg         = %.8X", Pt->HeadChk.TagBeg    ));         /*!< Should be == MIS1__BT_REC_TAG_BEGIN, with B7B0 = Data format version */
  msg (( MSG_OUT, "TagEndian      = %.8X", Pt->HeadChk.TagEndian ));         /*!< Should be == M MIS1__BT_REC_TAG_ENDIAN */
  
  // WHen a parameter is not used it can be set to -1
  
  msg (( MSG_OUT, "RunNo          = %.4d", Pt->RunNo         ));     /*!< No of run */
  msg (( MSG_OUT, "AcqNb          = %.4d", Pt->AcqNb         ));     /*!< Acquisitions nb in run  */
  msg (( MSG_OUT, "FrNbPerAcq     = %.4d", Pt->FrNbPerAcq    ));     /*!< Frames nb / acquisition */
  msg (( MSG_OUT, "AcqNbPerFile   = %.4d", Pt->AcqNbPerFile  ));     /*!< Acquisitions nb / file */
  msg (( MSG_OUT, "MinTrigNb      = %.4d", Pt->MinTrigNb     ));     /*!< Minimun triggers nb to stored and acquisition */
  msg (( MSG_OUT, "MaxTrigNb      = %.4d", Pt->MaxTrigNb     ));     /*!< Maximum triggers nb to stored and acquisition */
  msg (( MSG_OUT, "RunMode        = %.4d", Pt->RunMode       ));     /*!< Run mode => TB defined later => Probably RAW / TRIGGERS ONLY */
  msg (( MSG_OUT, "ProcMode       = %.4d", Pt->ProcMode      ));     /*!< Processing  mode => TB defined later  */
  msg (( MSG_OUT, "SaveMode       = %.4d", Pt->SaveMode      ));     /*!< Save  mode => TB defined later, 0 => don't save */
  msg (( MSG_OUT, "TrigMode       = %.4d", Pt->TrigMode      ));     /*!< Trigger  mode => TB defined later */
  msg (( MSG_OUT, "RunTrgPreFrNb  = %.4d", Pt->RunTrgPreFrNb ));     /*!< For trigger only runs => Nb of frames stored before trigger */
  msg (( MSG_OUT, "RunTrgPostFrNb = %.4d", Pt->RunTrgPostFrNb  ));   /*!< For trigger only runs => Nb of frames stored after trigger */
  
  
  msg (( MSG_OUT, "DmaPacketW64Sz = %.4d", Pt->DmaPacketW64Sz ));    /*!< DMA packet size used for Flex RIO => for info only */
  
  msg (( MSG_OUT, "RunCmt                = %s", Pt->RunCmt ));                /*!< User run comment */
  
  msg (( MSG_OUT, "RunRawIndexFilePrefix = %s", Pt->RunRawIndexFilePrefix ));       /*!< Name of the index file Prefix for raw data run  */
  msg (( MSG_OUT, "RunRawDataFilePrefix  = %s", Pt->RunRawDataFilePrefix ));  /*!< Name of the raw data file prefix = name without the file number ext RUN_777_ */
  
  msg (( MSG_OUT, "RunTrgIndexFilePrefix = %s", Pt->RunTrgIndexFilePrefix ));       /*!< Name of the index file Prefix for "triggers only" data run  */
  msg (( MSG_OUT, "RunTrgDataFilePrefix  = %s", Pt->RunTrgDataFilePrefix ));  /*!< Name of the "triggers only" data file prefix = name without the file number ext RUN_777_ */
  
  
  msg (( MSG_OUT, "" ));

  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FBtRunIndexfRecLoad ( char* FileName, MIS1__TBtRunRawIndexRec* Pt, UInt8 FormatVers )
*
* \brief   : Loads from disk run raw index record
*
* \param   : FileName - File
*
* \param   : Pt - Pointer to record
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



SInt32 MIS1__BT_FBtRunIndexRecLoad ( char* FileName, MIS1__TBtRunRawIndexRec* Pt, UInt8 FormatVers ) {
  
  SInt32 VRet;
  
  VRet =  MIS1__FRecordLoad ( FileName, Pt, sizeof (MIS1__TBtRunRawIndexRec), FormatVers );
  
  err_retfail ( VRet, (ERR_OUT,"Load run raw index record has failed ! Ret = %d", VRet) );
  
  err_retok (( ERR_OUT, "Ok" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FBtRunIndexfReccSave ( char* FileName, MIS1__TBtRunRawIndexRec* Pt, UInt8 FormatVers )
*
* \brief   : Saves run conf recordto disk
*
* \param   : FileName - File
*
* \param   : Pt - Pointer to record
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__BT_FBtRunIndexfRecSave ( char* FileName, MIS1__TBtRunRawIndexRec* Pt, UInt8 FormatVers ) {
  
  SInt32 VRet;
  
  VRet = MIS1__FRecordSave ( FileName, Pt, sizeof (MIS1__TBtRunRawIndexRec), FormatVers );
  
  err_retfail ( VRet, (ERR_OUT,"Save raw index record has failed ! Ret = %d", VRet) );
  
  err_retok (( ERR_OUT, "Ok" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__BT_FBtRunIndexRecPrint ( MIS1__TBtRunRawIndexRec* Pt, UInt32 PrintTriggers, UInt8 PrintFrCnt, UInt8 PrintFiredPixels )
*
* \brief   : Prints run conf record
*
* \param   : Pt - Pointer to record
*
* \return  : Error code  \n
*          :   0  - OK                                        \n
*          : < 0 - Error                                      \n
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 13/05/2021
* \date    : Doc date  : 13/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__BT_FBtRunIndexRecPrint ( MIS1__TBtRunRawIndexRec* Pt, UInt32 PrintTriggers, UInt8 PrintFrCnt, UInt8 PrintFiredPixels ) {
  
  SInt32 VRet;
  SInt32 Vi;
  MIS1__TBtAcqRawHead* VPtAcqHead;
  
  // Check param
  
  err_retnull ( Pt, (ERR_OUT,"Abort => Pt == NULL" ) );
  
  // Print
  
  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "===========================================================================" ));
  msg (( MSG_OUT, " Run index record" ));
  msg (( MSG_OUT, "===========================================================================" ));
  msg (( MSG_OUT, "" ));
  
  msg (( MSG_OUT, "TagBeg         = %.8X", Pt->HeadChk.TagBeg    ));         /*!< Should be == MIS1__BT_REC_TAG_BEGIN, with B7B0 = Data format version */
  msg (( MSG_OUT, "TagEndian      = %.8X", Pt->HeadChk.TagEndian ));         /*!< Should be == M MIS1__BT_REC_TAG_ENDIAN */
  
  msg (( MSG_OUT, "AcqNbInFile    = %.4d", Pt->AcqNbInFile         ));     /*!< Acquisition number in the file (index and data) */
  

  

  msg (( MSG_OUT, "" ));

  for ( Vi=0; Vi < 10 /* Pt->AcqNbInFile */; Vi++  ) {
    
    VPtAcqHead = &Pt->AAcqHead[Vi];
    
    MIS1__BT_FAcqRawHeadRecPrint ( VPtAcqHead, PrintTriggers, PrintFrCnt, PrintFiredPixels );
  }

  
  msg (( MSG_OUT, "" ));
  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : int32_t MIS1__FIsCpuBigEndian
*
* \brief   : Returns 1 if CPU is big endian, 0 if little endian, -1 is unknown 
*
* \param   : None -
*
* \return  : Returns 1 if CPU isbig endian, 0 if little endian, -1 is unknown
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*
* Items not filled now : \n
*  todo    :
*
*  bug     :
*
* \date    : Date      : 19/05/2021
* \date    : Doc date  : 19/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


int32_t MIS1__FIsCpuBigEndian () {
  
  unsigned long int VTest = 0x11223344;
  
  unsigned char* VPtTest = (UInt8*) &VTest;
  
  if ( (VPtTest[0] == 0x11 ) && (VPtTest[1] == 0x22 ) && (VPtTest[2] == 0x33 ) && (VPtTest[3] == 0x44 ) ) {
    return (1);
  }
  
  if ( (VPtTest[0] == 0x44 ) && (VPtTest[1] == 0x33 ) && (VPtTest[2] == 0x22 ) && (VPtTest[3] == 0x11 ) ) {
    return (0);
  }
  
  return (-1);  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtRunRead ()
*          :
* \brief   : Constructor   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtRunRead::MIS1__TBtRunRead () {
    
  _FBegin ();

}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : ~MIS1__TBtRunRead ()
*          :
* \brief   : Destructor   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtRunRead::~MIS1__TBtRunRead () {
  
  
  err_error (( ERR_OUT, "~MIS1__TBtRunRead () called" ));
  
  _FEnd ();
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::FIsCpuBigEndian ()
*          :
* \brief   : Destructor   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 19/05/2021
* \date    : Doc date  : 19/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::FIsCpuBigEndian () {
  
  _CpuIsBigEndian = MIS1__FIsCpuBigEndian ();
  
  return ( _CpuIsBigEndian );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::_FBegin ()
*          :
* \brief   : Class initilization = Init variables, alloc meme, eror + general messages enable / disable   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by constructor
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::_FBegin () {

  SInt32 VRet;
  SInt32 VDemoVar;
  
  
  // -----------------------------------------------------------
  // Init error messages lib
  // -----------------------------------------------------------
  
  ERR_FBegin ( 1 /* Enable */, FErrLogSetFilename () /* FilePath */ );
  
  // Set log level, can be
  //
  // ERR_LOG_LVL_NONE
  // ERR_LOG_LVL_ALL
  // ERR_LOG_LVL_WARNINGS_ERRORS
  // ERR_LOG_LVL_ERRORS
  
  _ErrLogSetLevel = ERR_LOG_LVL_ALL;
  
  ERR_FSetFileLogLevel ( _ErrLogSetLevel /* LogLevel */ );
  ERR_FSetUserLogLevel ( _ErrLogSetLevel /* LogLevel */ );
  
  // Install error log print user function
  
  ERR_FSetUserErrorFunc ( &MIS1__TBtRunRead_FErrLogUsrPrint );
  
  
  // Test error logging
  
  err_trace   (( ERR_OUT, "Trace message   : VDemoVar = %d", VDemoVar ));
  err_error   (( ERR_OUT, "Warning message : VDemoVar = %d", VDemoVar ));
  err_warning (( ERR_OUT, "Error message   : VDemoVar = %d", VDemoVar ));
  
  
  
  // -----------------------------------------------------------
  // Init general messages lib
  // -----------------------------------------------------------
  
  MSG_FBegin ( 1 /* Enable */, FMsgLogSetFilename () /* FilePath */ );
  
  // Set log level, only 127 available now
  
  _MsgLogSetLevel = 127;
  
  MSG_FSetFileLogLevel ( _MsgLogSetLevel /* Level */ );
  MSG_FSetUserLogLevel ( _MsgLogSetLevel /* Level */ );
  
  // Install user function
  
  MSG_FSetUserMsgFunc ( &MIS1__TBtRunRead_FMsgLogUsrPrint );
  
  
  // Test messages logging
  
  msg (( MSG_OUT, "General message : VDemoVar = %d", VDemoVar ));
  
  
  // -----------------------------------------------------------
  // Get CPU endianness
  // -----------------------------------------------------------
  
  FIsCpuBigEndian (); // Result in _CpuIsBigEndian : 0 = Little, 1 = Big, -1 = ??? 
  
  
  // -----------------------------------------------------------
  // Reset fields
  // -----------------------------------------------------------
  
  _LastError      = 0;
  
  _RunConfLoaded  = 0;
  _RunNo          = 0;

  
  
  _AcqHeadSzW8    = 0;
  _AcqMaxTotSzW8  = 0;
  
  _PtAcqRaw       = NULL;
  
  _CurAcqId       =  0;
  _CurRawFileId   = -1;
  _CurAcqIdInFile = -1;
  
  _NewAcqId       =  0;
  _NewRawFileId   = -1;
  _NewAcqIdInFile = -1;
    
    
  _CurRawFilePt   = NULL;
  _CurIndexFilePt = NULL;
  
  
  _HeadPrintTriggers    = 10;  // Print triggers, -1 => All, 0 => No, > 0 => value = nb of triggers to print
  _HeadPrintFrCnt       = 10;  // Print frames counters, -1 => All, 0 => No, > 0 => value = nb of frames to print
  _HeadPrintFiredPixels = 1;   // Print fired pixels, 0 / 1
      
  
  memset ( &_RunCnfRec, 0, sizeof (MIS1__TBtRunCnfRec) );
  
  
  
  
  _DbgPrint = 1;
  
  
  err_retok (( ERR_OUT, "Ok" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::_FEnd ()
*          :
* \brief   : Class finalization = free meme etc   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by destructor
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::_FEnd () {
  
  SInt32 VRet;
  
   
  
  err_retok (( ERR_OUT, "Ok" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : char* MIS1__TBtRunRead::FErrLogSetFilename ()
*          :
* \brief   : Sets errors messages log file name and returns it  \n
*          : Can be called to locate the error file directory and name
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by constructor => File name can' t be modified later !
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


char* MIS1__TBtRunRead::FErrLogSetFilename () {
  
  SInt32 VRet;
  static char  VErrLogFile[GLB_FILE_PATH_SZ] = MIS1__TBtRunRead_DEF_ERR_LOG_FILE;
  
    
  return (  VErrLogFile );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : char* MIS1__TBtRunRead::FMsgLogSetFilename ()
*          :
* \brief   : Sets general messages log file name and returns it  \n
*          : Can be called to locate the general messages file directory and name
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by constructor => File name can' t be modified later !
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


char* MIS1__TBtRunRead::FMsgLogSetFilename () {
  
  SInt32 VRet;
  static char VMsgLogFile[GLB_FILE_PATH_SZ]  = MIS1__TBtRunRead_DEF_MSG_LOG_FILE;
    
  
  return (  VMsgLogFile );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::FErrLogSetLevel ( SInt8 LogLevel )
*          :
* \brief   : Sets error log level, by default it is set to ERR_LOG_LVL_ALL, \n
*            it can be                     \n
*                                          \n
*            ERR_LOG_LVL_NONE              \n
*            ERR_LOG_LVL_ALL               \n
*            ERR_LOG_LVL_WARNINGS_ERRORS   \n
*            ERR_LOG_LVL_ERRORS            \n
*
*          :
* \param   : LogLevel         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by desconstructor
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::FErrLogSetLevel ( SInt8 LogLevel ) {
  
  SInt32 VRet;
  
  _ErrLogSetLevel = LogLevel;
  
  ERR_FSetFileLogLevel ( _ErrLogSetLevel /* LogLevel */ );
  ERR_FSetUserLogLevel ( _ErrLogSetLevel /* LogLevel */ );
  
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::FMsgLogSetLevel ( SInt8 LogLevel )
*          :
* \brief   : Sets general log level, by default it is set 127 <=> ALL, it can be 0..127
*          :
* \param   : LogLevel         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by desconstructor
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::FMsgLogSetLevel ( SInt8 LogLevel ) {
  
  SInt32 VRet;
  
  _MsgLogSetLevel = LogLevel;
  
  
  MSG_FSetFileLogLevel ( _MsgLogSetLevel /* Level */ );
  MSG_FSetUserLogLevel ( _MsgLogSetLevel /* Level */ );
  
  
  err_retok (( ERR_OUT, "" ));
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtRunRead::FErrGetLastMsg ()
*          :
* \brief   : Gets last error message as a string
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by desconstructor
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 19/05/2021
* \date    : Doc date  : 19/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


char* MIS1__TBtRunRead::FErrGetLastMsg () {

  return ( ERR_FGetLastErrMsg () );
  
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : char* MIS1__TBtRunRead::FMsgGetLastMsg  ()
*          :
* \brief   : Gets last general message as a string
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Called by desconstructor
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 19/05/2021
* \date    : Doc date  : 19/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


char* MIS1__TBtRunRead::FMsgGetLastMsg  () {
  
  return ( MSG_FGetLastMsgLong ( 0 /* Chan */ ) );
  
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead_FErrLogUsrPrint ( char Type, char* ErrLocation, char* ErrUserMsg, char* FullMsg )
*          :
* \brief   : User can implement here his own errors messages printing function  \n
*          :
* \param   : Type        - Type of message = T, W, E => Trace, Warning, Error
*          :
* \param   : ErrLocation - File and function names + source file line where the error has occured 
*          :
* \param   : ErrUserMsg  - The message set by user in addition to Type and ErrLocation
*          :
* \param   : FullMsg     - The full message as written in log file = Type + ErrLocation + ErrUserMsg
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Not called by user, automatically called each time an error macro is executed
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead_FErrLogUsrPrint ( char Type, char* ErrLocation, char* ErrUserMsg, char* FullMsg ) {
  
  SInt32 VRet;
  
  char VErrMsg[GLB_CMT_SZ];
  
  // Message reformating can be done like this
  //
  // sprintf ( VErrMsg, "Type = %c, Location = %s, Msg = %s ", Type, ErrLocation, ErrUserMsg );
  

  #ifdef CC_APP_GUI_CPP
    FrmMain->PgMain_MemErr->Lines->Add ( FullMsg );
  #endif
    
  #ifdef CC_APP_ERR_LOG_VIA_PRINTF
    printf ( "%s", FullMsg );
  #endif


  
  return (0);
}

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead_FMsgLogUsrPrint { char* Msg )
*          :
* \brief   : User can implement here his own general messages printing function \n
*          :
* \param   : Msg - The message to print
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Not called by user, automatically called each time a general message macro is executed
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead_FMsgLogUsrPrint ( char* Msg ) {
  
  SInt32 VRet;
  char VMsg[GLB_CMT_SZ];
  
  
  #ifdef CC_APP_GUI_CPP
    FrmMain->PgMain_MemMsg->Lines->Add ( Msg );
  #endif
  
  #ifdef CC_APP_MSG_LOG_VIA_PRINTF  
    printf ( "%s", Msg );
  #endif

  
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::FRunConf ( char* RunRootDir, UInt32 RunNo, UInt8 PrintRunHeader, UInt8 FormatVers )
*          :
* \brief   : Configures the run to read  \n
*          :
* \param   : RunRootDir     - Root directory of run, ex : c:\data if run 777 is in c:\data\777
*          :
* \param   : RunNo          - Run number, ex : 777
*          :
* \param   : FormatVers     - Version of record format, please set it to MIS1__BT_RUN_RD_FILE_FORMAT
*          :
* \param   : PrintRunHeader - If 1, the run header info will be printed in messages log file
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : 
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/05/2021
* \date    : Doc date  : 17/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::FRunConf ( char* RunRootDir, UInt32 RunNo, UInt8 PrintRunHeader, UInt8 FormatVers ) {
  
  SInt32 VRet;  
  
  
  // Check param
  
  err_retnull ( RunRootDir, (ERR_OUT,"Abort => RunRootDir == NULL") );
  
  // Check status
  
  if ( _RunConfLoaded == 1 ) {
    err_warning (( ERR_OUT, "Abort => A run (No %d) is already loaded => Close it before loading a new one", _RunNo ));
    err_retok (( ERR_OUT, "Ok" ));
  }
  
  
  _RunNo     = RunNo;
  _LastError = -1;
  

  // Resets variable & record
  
  _RunConfLoaded = 0;
  
  memset ( &_RunCnfRec, 0, sizeof (MIS1__TBtRunCnfRec) );
  
  // Build run header filename
  
  sprintf ( _RunRootDir, "%s", RunRootDir );
  
  sprintf ( _RunConfFileName, "%s/%d/RUN_%d_CONF.bin", RunRootDir, RunNo, RunNo ); // C:\Data\msis1\777\RUN_777_CONF.bin
  
  err_trace (( ERR_OUT, "Try to load run no %d, run conf / header file = %s", RunNo, _RunConfFileName ));
  
  // Loads record
  
  VRet = MIS1__BT_FBtRunCnfRecLoad (  _RunConfFileName, &_RunCnfRec, FormatVers );
  
  err_retfail ( VRet, (ERR_OUT,"Loading run no %d, run conf / header file = %s failed ! Ret = %d", RunNo, _RunConfFileName, VRet ) );
  
  err_trace (( ERR_OUT, "Run conf file loaded :-)" ));
  
  // Prints info
  
  if ( PrintRunHeader ) {
    
    VRet = MIS1__BT_FBtRunCnfRecPrint ( &_RunCnfRec );
  
    err_retfail ( VRet, (ERR_OUT,"Abort => Printing run conf file record failed ! Ret = %d", VRet) );
    
  }

  // ------------------------------
  // Calculated Acq maw size
  // ------------------------------

  _PtAcqRaw = (MIS1__TBtAcqRawRec*) malloc ( sizeof (MIS1__TBtAcqRawRec) );  // Alloc Acq (fixed size) only to get a record for header size calcualtion

  err_retnull ( _PtAcqRaw, (ERR_OUT,"Abort => Allocation of one Acq buffer (for sizes calculation) of %d W8 failed !", sizeof (MIS1__TBtAcqRawRec) ) );

  // Before 19/05/2021, using static allocation of Acq record for sizes calculation
  //
  // _AcqHeadSzW8 = (UInt32) ((UInt8*) &_AcqRaw.MSisData.Au64[0] - (UInt8*) &_AcqRaw); // Since 17/05/2021


  // Since  19/05/2021, using dynamic allocation of Acq record for sizes calculation (to reduce static variables volume)

  _AcqHeadSzW8 = (UInt32) ((UInt8*) &_PtAcqRaw->MSisData.Au64[0] - (UInt8*) _PtAcqRaw);

  _AcqMaxTotSzW8 = _AcqHeadSzW8 + ( _RunCnfRec.FrNbPerAcq * MIS1__BT_MAX_MSIS_NB_ACQ * MIS1__BT_TOT_FRAME_SZ_W8 ); // Since 17/05/2021

  free ( _PtAcqRaw ); // It will be allocated hereafter with the add-hoc size

  
  err_trace (( ERR_OUT, "1 Acq = %d frames : _AcqHeadSzW8 = %d W8, _AcqMaxTotSzW8 = %d W8", _RunCnfRec.FrNbPerAcq, _AcqHeadSzW8, _AcqMaxTotSzW8 ));
  

  // ------------------------------
  // Alloc memory for one Acq
  // ------------------------------

  _PtAcqRaw = (MIS1__TBtAcqRawRec*) malloc ( _AcqMaxTotSzW8 );
  
  err_retnull ( _PtAcqRaw, (ERR_OUT,"Abort => Allocation of one Acq buffer (for Acq storage) of %d W8 failed !", _AcqMaxTotSzW8) );

  
  // Set fields
  
  
  _CurAcqId       =  0;
  _CurRawFileId   = -1;
  _CurAcqIdInFile = -1;
  
  _NewAcqId       =  0;
  _NewRawFileId   = -1;
  _NewAcqIdInFile = -1;
  
  _CurRawFilePt   = NULL;
  _CurIndexFilePt = NULL;
    
  _LastRawFileId = (_RunCnfRec.AcqNb / _RunCnfRec.AcqNbPerFile) - 1;
  
  _RunConfLoaded = 1;
  _LastError     = 0; 
  
  err_retok (( ERR_OUT, "Ok" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::FRunClose ()
*          :
* \brief   : Closes a run (free mem, reset variables, etc ...)  \n
*          :
* \param   : None  
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : 
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 16/05/2021
* \date    : Doc date  : 16/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

  
SInt32 MIS1__TBtRunRead::FRunClose () {
    
  SInt32 VRet;
  char VMsg[GLB_CMT_SZ];
    
    
  VRet = 0; 
    
  // Free memory 

  #ifndef CC_NOT_CPP_BUILDER

    try {
  
      if ( _PtAcqRaw != NULL ) {
        free ( _PtAcqRaw );
      }
      
      _PtAcqRaw = NULL; 
    
    }
    
    catch ( Exception& VException ) {
      err_error (( ERR_OUT, "free Acq exception = %s", VException.Message.c_str() ));
      VRet= -1;
    }
    
  
  #else

      if ( _PtAcqRaw != NULL ) {
        free ( _PtAcqRaw );
      }
      
      _PtAcqRaw = NULL;
  
  #endif
  
  
  
  // Close files

  #ifndef CC_NOT_CPP_BUILDER

    try {
    
      if ( _CurRawFilePt != NULL ) {
        fclose ( _CurRawFilePt );
      }
    
      if ( _CurIndexFilePt != NULL ) {
        fclose ( _CurIndexFilePt );
      }
      
    }
    
    catch ( Exception& VException ) {
      err_error (( ERR_OUT, "Close files exception = %s", VException.Message.c_str() ));
      VRet= -1;
    }
    

  #else

      if ( _CurRawFilePt != NULL ) {
        fclose ( _CurRawFilePt );
      }
      
      if ( _CurIndexFilePt != NULL ) {
        fclose ( _CurIndexFilePt );
      }
     
  
  #endif
  

  // Reset variables
    
  _RunConfLoaded  = 0;
  _RunNo          = 0; 

  _AcqHeadSzW8   = 0;
  _AcqMaxTotSzW8 = 0;
  
  _CurRawFilePt   = NULL;
  _CurIndexFilePt = NULL;
  
  
  
  err_retval ( VRet, ( ERR_OUT, "Error = %d", VRet ) );
}
  
  

/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtRunCnfRec* MIS1__TBtRunRead::FRunHeaderGet ( UInt8 Print )
*          :
* \brief   : Returns a pointer to current run header, prints info in log file if Print == 1  \n
*          :
* \param   : Print - Prints info in log file if Print == 1
*          :
* \return  : A pointer to run conf record MIS1__TBtRunCnfRec*, NULL if error
*          :
* \warning : Globals   :
* \warning : Remark    : 
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/05/2021
* \date    : Doc date  : 17/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

MIS1__TBtRunCnfRec* MIS1__TBtRunRead::FRunHeaderGet ( UInt8 Print ) {

  SInt32 VRet;

  // Check
  
  if ( _RunConfLoaded < 1 ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => No run conf loaded !") );
  }


  // Print

  if ( Print ) {
    
    VRet = MIS1__BT_FBtRunCnfRecPrint ( &_RunCnfRec );
    
    err_retfailnull ( VRet, (ERR_OUT,"Abort => Printing run conf file record failed ! Ret = %d", VRet) );
    
  }


  return ( &_RunCnfRec ); 
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqFirst ( UInt8 ChkHead, UInt8 PrintHead )
*          :
* \brief   : Returns a pointer to the first Acq of run \n
*          : \n
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Sequential / random access via index selection to be implemented in this method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqFirst ( UInt8 ChkHead, UInt8 PrintHead ) {
  
  SInt32 VRet;
  MIS1__TBtAcqRawRec* VPtAcq;
  
  
  // Now on 18/05/2021 only sequential access is available
  // Selection between sequential and random access via index will be implemented here
  //
  // Rq : It can seen as useless to implement random access to get the first acq of a run
  // and it is, but some initialization for FAcqNextSeq (...) can be required to be done here
  // that is why I have written this long block of comment ;-)
  
  VPtAcq = _FAcqFirstSeq ( ChkHead, PrintHead );
  
  return ( VPtAcq );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqNext ( UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd )
*          :
* \brief   : Returns a pointer to the xext Acq of run \n
*          : \n
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
* \param   : PtReachEn - A pointer on a flag set to 1 if end of file is reached, set it to NULL if not used
*          :
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Sequential / random access via index selection to be implemented in this method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqNext ( UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd ) {
  
  SInt32 VRet;
  MIS1__TBtAcqRawRec* VPtAcq;
  
  
  // Now on 18/05/2021 only sequential access is available
  // Selection between sequential and random access via index will be implemented here
  
  VPtAcq = _FAcqNextSeq ( ChkHead, PrintHead, PtReachEnd );
  
  return ( VPtAcq );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqGoto ( UInt32 AcqId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd )
*          :
* \brief   : Returns a pointer to the xext Acq of run \n
*          : \n
*          :
* \param   : AcqId     - Id / No of the Acq
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
* \param   : PtReachEn - A pointer on a flag set to 1 if end of file is reached, set it to NULL if not used
*          :
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Sequential / random access via index selection to be implemented in this method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqGoto ( UInt32 AcqId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd ) {
  
  SInt32 VRet;
  MIS1__TBtAcqRawRec* VPtAcq;
  
  
  // Now on 18/05/2021 only sequential access is available
  // Selection between sequential and random access via index will be implemented here
  
  VPtAcq = _FAcqGotoSeq ( AcqId, ChkHead, PrintHead, PtReachEnd );
  
  return ( VPtAcq );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqGetPt ()
*          :
* \brief   : Returns a pointer to current Acq loaded in memory  \n
*          :
* \param   : None         -
*          :
* \return  : A pointer to the Acq
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

 
MIS1__TBtAcqRawRec* MIS1__TBtRunRead::FAcqGetPt () {
    
  return ( _PtAcqRaw );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::FAcqPrint ( MIS1__TBtAcqRawRec* Pt, SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels, UInt8 DataWSize, UInt32 FirstDataW, UInt32 NbDataW, UInt8 VRS )
*          :
* \brief   : Print one Acq : the one pointer by Pt or the current one loaded n memory if Pt == NULL  \n
*          :
* \param   : Pt               - Pointer to Acq, if NULL => Uses Acq loaded in mem = _PtAcqRaw
*          :
* \param   : PrintTriggers    - Prints trigger, -1 => All, 0 => None, N > 0 => Prints first N triggers
*          :
* \param   : PrintFrCnt       - Prints frames counters x 6 MSis 1, -1 => All frames, 0 => None, N > 0 => Prints first N frames
*          :
* \param   : PrintFiredPixels - Prints fired pixels for each MSis 1, each sub matrix,  0 => No, 1 => Yes
*          :
* \param   : DataWSize        - Size of W to print for data part, can be 8, 16, 32, 64 bits
*
* \param   : FirstDataW       - First W to print
*
* \param   : NbDataWW         - Nb W to print
*
* \param   : VRS              - Record type 0 => Fixed Record Size, 1 => Variable Record Size

* \param   :          -
*          :
* \param   :          -
*          :
* \param   :          -
*          :


* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::FAcqPrint ( MIS1__TBtAcqRawRec* Pt, SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels, UInt8 DataWSize, UInt32 FirstDataW, UInt32 NbDataW, UInt8 VRS ) {
  
  SInt32 VRet;  
  
  if ( Pt == NULL ) {
    Pt = _PtAcqRaw;
  }
    
  VRet = MIS1__BT_FAcqRawPrint ( Pt, PrintTriggers, PrintFrCnt, PrintFiredPixels, DataWSize, FirstDataW, NbDataW, VRS );
  
  err_retfail ( VRet, (ERR_OUT,"Abort => Printing acq failed !" ) );
  
  err_retok (( ERR_OUT, "Ok" ));

}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::FAcqHeadPrintOptSet ( SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels )
*          :
* \brief   : Confifures the header printing option used by Acq scanning functions like FAcqFirst (...), FAcqNext (...), FAcqGoto (...)  \n
*          :
* \param   : PrintTriggers    - Print triggers, -1 => All, 0 => No, > 0 => value = nb of triggers to print
*
* \param   : PrintFrCnt       - Print frames counters, -1 => All, 0 => No, > 0 => value = nb of frames to print
*
* \param   : PrintFiredPixels - Print fired pixels, 0 / 1
*
* \return  : Always 0
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 19/05/2021
* \date    : Doc date  : 19/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


// Confifures the header printing option used by Acq scanning functions like FAcqFirst (...), FAcqNext (...), FAcqGoto (...)

SInt32 MIS1__TBtRunRead::FAcqHeadPrintOptSet ( SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels ) {

  _HeadPrintTriggers     = PrintTriggers;             // Print triggers, -1 => All, 0 => No, > 0 => value = nb of triggers to print
  _HeadPrintFrCnt        = PrintFrCnt;                // Print frames counters, -1 => All, 0 => No, > 0 => value = nb of frames to print
  _HeadPrintFiredPixels  = PrintFiredPixels;          // Print fired pixels, 0 / 1
    

  err_retok (( ERR_OUT, "Ok" ));
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : UInt32 MIS1__TBtRunRead::FMeasTimeStart ()
*          :
* \brief   : Starts execution time measurement  \n
*          :
* \param   : None         -
*          :
* \return  : Always 0
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


UInt32 MIS1__TBtRunRead::FMeasTimeStart () {
  

  _MeasTimeEndMS = 0;
  _MeasTimeMS    = 0;


  #ifndef CC_NOT_CPP_BUILDER
    _MeasTimeBegMS = GetTickCount ();
    
  #else
    _MeasTimeBegMS = 0;
  #endif
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : UInt32 MIS1__TBtRunRead::FMeasTimeStop ()
*          :
* \brief   :  Stops execution time measurement  \n
*          :
* \param   : None         -
*          :
* \return  : Execution time in ms
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


UInt32 MIS1__TBtRunRead::FMeasTimeStop () {
  
  
  #ifndef CC_NOT_CPP_BUILDER
    _MeasTimeEndMS = GetTickCount (); 
    _MeasTimeMS = _MeasTimeEndMS - _MeasTimeBegMS;
    
  #else
    _MeasTimeEndMS = 0;
    _MeasTimeMS = _MeasTimeEndMS - _MeasTimeBegMS;
  #endif
  
  
  return ( _MeasTimeMS );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : UInt32 MIS1__TBtRunRead::FMeasTimeGetMs ()
*          :
* \brief   : Gets result in ms of execution time measurement  \n
*          :
* \param   : None         -
*          :
* \return  : Execution time in ms
*          :
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


UInt32 MIS1__TBtRunRead::FMeasTimeGetMs () {
  
  
  return ( _MeasTimeMS );
}




UInt32 MIS1__FConvMsToTime ( UInt32 Ms, UInt8* PtHour, UInt8* PtMin, UInt8* PtSec, UInt16* PtMs ) {
  
  UInt8  VHour;
  UInt8  VMin;
  UInt8  VSec;
  UInt16 VMs;
  UInt32 VRem1;
  UInt32 VRem2;

  
  
  VHour = Ms / 3600000;
  
  VRem1 = Ms % 3600000;
  
  VMin = VRem1 / 60000;
  
  VRem2 = VRem1 % 60000;
  
  VSec = VRem2 / 1000;
  
  VMs = VRem2 % 1000;
  

  
  if ( PtHour != NULL ) {
    *PtHour = VHour;
  }
  
  if ( PtMin != NULL ) {
    *PtMin = VMin;
  }
  
  if ( PtSec != NULL ) {
    *PtSec = VSec;
  }
  
  if ( PtMs != NULL ) {
    *PtMs = VMs;
  }
  
   
  
  return (Ms);
}






/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : UInt32 MIS1__TBtRunRead::FMeasTimeGetHHMMSSCC ( UInt8* PtHour, UInt8* PtMin, UInt8* PtSec, UInt16* PtMs )
*          :
* \brief   : Gets result in HH:MM:SS:1/100 of execution time measurement  \n
*          :
* \param   : PtHour    -
*
* \param   : PtMin     -
*
* \param   : PtSec     -
*
* \param   : PtCent    -
*
* \return  : Execution time in ms
*
* \warning : Globals   :
* \warning : Remark    :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

// 

UInt32 MIS1__TBtRunRead::FMeasTimeGetHMSMS ( UInt8* PtHour, UInt8* PtMin, UInt8* PtSec, UInt16* PtMs ) {
  
  
  MIS1__FConvMsToTime ( _MeasTimeMS, PtHour, PtMin, PtSec, PtMs );
  
  
  return ( _MeasTimeMS );
}





/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::
*          :
* \brief   :   \n
*          :
* \param   : None         -
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : 
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/05/2021
* \date    : Doc date  : 17/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::F () {
  
  SInt32 VRet;
  
  
  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::_FBuildNewAcqFile ( UInt32 AcqNo, UInt8 Print )
*          :
* \brief   : Create the file name which should contains one Acq, result sets in \n
*          : _NewRawFileId, _NewAcqIdInFile , _NewRawFileName, _NewIndexFileName  \n
*          :
* \param   : AcqNo - No of the acquisiton 0 to acq nb in run - 1
*          :
* \param   : Print - If 1 => Prints result via an error trace message
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 17/05/2021
* \date    : Doc date  : 17/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::_FBuildNewAcqFile ( UInt32 AcqNo, UInt8 Print ) {
  
  SInt32 VRet;
  
  
  MIS1__TBtRunRead_CHECK_RET_S32
  
  // Calc
  
  _NewAcqId       = AcqNo;
  
  _NewRawFileId   = AcqNo / _RunCnfRec.AcqNbPerFile;
  _NewAcqIdInFile = AcqNo % _RunCnfRec.AcqNbPerFile;
  
  // Check
  
  if ( _NewRawFileId > _LastRawFileId ) {
    err_retfail ( -1, (ERR_OUT,"Abort => AcqNo = %d > AcqNb in run = %d => AcqNo = 00..%d", AcqNo, _RunCnfRec.AcqNb, 0, _RunCnfRec.AcqNb - 1) );
  }
  
  
  if ( _NewAcqIdInFile >= _RunCnfRec.AcqNbPerFile ) {
    err_retfail ( -1, (ERR_OUT,"Abort => AcqId in run = %d is out of run fileAcqId =  0..%d", _NewAcqIdInFile, _RunCnfRec.AcqNbPerFile - 1) );
  }
    
  
  
  sprintf ( _NewRawFileName  , "%s/%d/RUN_%d_RD_%d.bin",  _RunRootDir, _RunNo, _RunNo, _NewRawFileId );  // RUN_777_RD_0.bin
  
  sprintf ( _NewIndexFileName, "%s/%d/RUN_%d_RDI_%d.bin",  _RunRootDir, _RunNo, _RunNo, _NewRawFileId );  // RUN_777_RDI_0.bin
    
  
  if ( Print ) {
    err_trace (( ERR_OUT, "AcqNo = %d is in file %s which contains %d acq, its position in file is acq no = %d", AcqNo, _NewRawFileName, _RunCnfRec.AcqNbPerFile, _NewAcqIdInFile ));
    err_trace (( ERR_OUT, "Index file is %s", _NewIndexFileName ));
  }
  
  return (0);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::_FBuildNewFile ( UInt32 FileId, UInt8 Print )
*          :
* \brief   : Create the file name which corresponds to FileId, result sets in \n
*          : _NewRawFileId, _NewRawFileName, _NewIndexFileName  \n
*          :
* \param   : FileId - File id
*          :
* \param   : Print - If 1 => Prints result via an error trace message
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


SInt32 MIS1__TBtRunRead::_FBuildNewFile ( UInt32 FileId, UInt8 Print ) {
  
  SInt32 VRet;
  
  
  MIS1__TBtRunRead_CHECK_RET_S32
  
  // Calc
    
  _NewRawFileId   = FileId;
  
  // Check
  
  if ( _NewRawFileId > _LastRawFileId ) {
    err_retfail ( -1, (ERR_OUT,"Abort => _NewRawFileId = %d > _LastRawFileId = %d", _NewRawFileId, _LastRawFileId) );
  }
  
     
  sprintf ( _NewRawFileName  , "%s/%d/RUN_%d_RD_%d.bin",  _RunRootDir, _RunNo, _RunNo, _NewRawFileId );  // RUN_777_RD_0.bin
  
  sprintf ( _NewIndexFileName, "%s/%d/RUN_%d_RDI_%d.bin",  _RunRootDir, _RunNo, _RunNo, _NewRawFileId );  // RUN_777_RDI_0.bin
  
  
  if ( Print ) {
    err_trace (( ERR_OUT, "Raw dat file is %s, Index file is %s", _NewRawFileName, _NewIndexFileName ));
  }
  
  return (0);
}




/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FFirstAcqOfRawFileGetSeq ( UInt32 RawFileId, UInt8 ChkHead, UInt8 PrintHead )
*          :
* \brief   : Returns a pointer to the first Acq of one raw data file \n
*          : \n
*          :
* \param   : RawFileId - The NO / Id of raw data file
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
*          :
* \return  : A pointer to Acq record MIS1__TBtAcqRawRec*, NULL if error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FFirstAcqOfRawFileGetSeq ( UInt32 RawFileId, UInt8 ChkHead, UInt8 PrintHead ) {

  SInt32 VRet;
  SInt32 VRecNbRead;
  
  // Check
  
  MIS1__TBtRunRead_CHECK_RET_NULL


  // Check param
  
  if ( RawFileId > _LastRawFileId ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => RawFileId > Last file id of run = %d", RawFileId, _LastRawFileId) );
  }
  
  
  // Sets first acq of file
  
  _CurAcqIdInFile = 0;
  
  // --------------------------
  // Open file
  // --------------------------


  // If current file is not the one we want to read from (including if no file opened => _CurRawFileId == -1)
  // => Close it
  // => Create new filename
  // => Open new file
  
  if ( _CurRawFileId != RawFileId ) {
    
    // Close current file
    
    err_trace (( ERR_OUT, "Close raw file Id = %d", _CurRawFileId ));
    
    if ( _CurRawFilePt != NULL ) {
      fclose ( _CurRawFilePt );
    }
    
    else {
      err_warning (( ERR_OUT, "Strange : raw file id = %d is opened but file ptr == NULL !!!", _CurRawFileId ));
    }
    
    _CurRawFilePt = NULL;
    
    // Create new file name
    
    VRet = _FBuildNewFile ( RawFileId, _DbgPrint );
  
    err_retfailnull ( VRet, (ERR_OUT,"Abort => Build raw file name Id = %d failed ! Ret = %d", RawFileId, VRet) );
  
    // Set current file name, id to new
  
    _CurRawFileId = _NewRawFileId;
  
    sprintf ( _CurRawFileName, "%s", _NewRawFileName );
  
    // Open new file
  
    _CurRawFilePt = fopen ( _CurRawFileName, "rb" );
    
    if ( _CurRawFilePt == NULL ) {
      err_error (( ERR_OUT, "Abort => Open raw file = %s failed !", _CurRawFileName ));
      return (NULL);
    }
    
    
  } // End if ( _CurRawFileId != RawFileId )
  
  
  // If current file is the one we want to read from
  // => Goto beginning of file
  
  if ( _CurRawFileId == RawFileId ) {
    
    err_trace (( ERR_OUT, "Set filez ptr to beginning of file %s", _CurRawFileName ));
    
    if ( fseek ( _CurRawFilePt, 0 /* ofs */, SEEK_SET ) != 0 ) {
      err_retfailnull ( -1, (ERR_OUT,"Abort => Set file cursor at beginning of file = %s failed !", _CurRawFileName ) );
    }
    
  }
  
  // ----------------------
  // Read first Acq
  // ----------------------

  // Read header
    
  VRecNbRead = fread ( &_PtAcqRaw->Head, _AcqHeadSzW8 /* Record size */ , 1 /* Record number */, _CurRawFilePt );
  
  if ( VRecNbRead != 1 ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read acq header of first acq, file = %s failed !", _CurRawFileName ) );
  }
  
  
  // Check endianness & convert 
  
  if ( _CpuIsBigEndian ) {
    
    VRet = MIS1__FUserEndianConv ( &_PtAcqRaw->Head, _AcqHeadSzW8 );
    
    err_retfailnull ( VRet, (ERR_OUT,"Abort => MIS1__FUserEndianConv (...) convert Acq header failed ! Ret = %d", VRet) );
    
    // Check if endianness ok
    
    if ( _PtAcqRaw->Head.TagBeg != MIS1__BT_REC_TAG_BEGIN ) {
      
      err_retfailnull ( -1, (ERR_OUT,"Abort => Record TagBeg = %X <> MIS1__BT_REC_TAG_BEGIN = %X => Bad endian conversion or bad data format version", _PtAcqRaw->Head.TagBeg, MIS1__BT_REC_TAG_BEGIN ) );
      
    }
    
  }
  
  
  if ( PrintHead ) {
    VRet = MIS1__BT_FAcqRawHeadRecPrint ( &_PtAcqRaw->Head, _HeadPrintTriggers, _HeadPrintFrCnt, _HeadPrintFiredPixels );
    
    err_retfailnull ( VRet, (ERR_OUT,"Abort => Print first acq header failed ! Ret = %d", VRet) );
  }
  
  
  // Read data
  
  err_trace (( ERR_OUT, "Read data sz = %d W8", _PtAcqRaw->Head.DataSz ));
  
  VRecNbRead = fread ( &_PtAcqRaw->MSisData, _PtAcqRaw->Head.DataSz /* Record size */ , 1 /* Record number */, _CurRawFilePt );
 
  if ( VRecNbRead != 1 ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read acq data of first acq, file = %s failed !", _CurRawFileName ) );
  }
  
  
  // Check endianness & convert
  
  if ( _CpuIsBigEndian ) {
    
    VRet = MIS1__FUserEndianConv ( &_PtAcqRaw->MSisData, _PtAcqRaw->Head.DataSz );
    
    err_retfailnull ( VRet, (ERR_OUT,"Abort => MIS1__FUserEndianConv (...) convert Acq data failed ! Ret = %d", VRet) );    
    
  }
  
  

  return (_PtAcqRaw);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FNextAcqOfRawFileGetSeq  ( UInt32 RawFileId, , UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd )
*          :
* \brief   : Returns a pointer to next Acq of one raw data file \n
*          : \n
*          :
* \param   : RawFileId - The NO / Id of raw data file
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
* \param   : PtReachEn - A pointer on a flag set to 1 if end of file is reached, set it to NULL if not used
*          :
* \return  : A pointer to Acq record MIS1__TBtAcqRawRec*, NULL if error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FNextAcqOfRawFileGetSeq  ( UInt32 RawFileId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd ) {


  SInt32 VRet;
  SInt32 VRecNbRead;
  
  // Check
  
  MIS1__TBtRunRead_CHECK_RET_NULL
  
  
  // Check param
  
  if ( RawFileId > _LastRawFileId ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => RawFileId > Last file id of run = %d", RawFileId, _LastRawFileId) );
  }
  
    
  // Update current acq in file
  
  _CurAcqIdInFile++;


  err_trace (( ERR_OUT, "_CurAcqIdInFile = %d, _RunCnfRec.AcqNbPerFile = %d", _CurAcqIdInFile, _RunCnfRec.AcqNbPerFile ));
  
  // If end of file has been reached
  
  if ( _CurAcqIdInFile >= _RunCnfRec.AcqNbPerFile ) {
        
    _CurAcqIdInFile = _RunCnfRec.AcqNbPerFile;
        
    if ( PtReachEnd != NULL ) {
      *PtReachEnd = 1;
    }
    
    return (_PtAcqRaw);
  }
    
  
  if ( PtReachEnd != NULL ) {
    *PtReachEnd = 0;
  }
  
  
  // Check if file is already open
  // => If not Call _FFirstAcqOfRawFileGetSeq (...)

  if ( _CurRawFileId != RawFileId ) {
    
    err_warning (( ERR_OUT, "Try to go to next Acq of file %s which is not opened => Call _FFirstAcqOfRawFileGetSeq ()", _CurRawFileName ));
    
    return ( _FFirstAcqOfRawFileGetSeq ( RawFileId, ChkHead, PrintHead ) );
    
  }
  
  
  // Check if file ptr is ok
  
  if ( _CurRawFilePt == NULL ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => File pointer is NULL, Raw file = %s", _CurRawFileName ) );
  }
  
  // ---------------------------------------
  // Read next acq
  // ---------------------------------------
  
  // Read header
    
  VRecNbRead = fread ( &_PtAcqRaw->Head, _AcqHeadSzW8 /* Record size */ , 1 /* Record number */, _CurRawFilePt );
  
  if ( VRecNbRead != 1 ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read acq header of first acq, file = %s failed !", _CurRawFileName ) );
  }
  
  
  // Check endianness  & convret
  
  if ( _CpuIsBigEndian ) {
    
    VRet = MIS1__FUserEndianConv ( &_PtAcqRaw->Head, _AcqHeadSzW8 );
    
    err_retfailnull ( VRet, (ERR_OUT,"Abort => MIS1__FUserEndianConv (...) Acq header failed ! Ret = %d", VRet) );
    
    // Check if endianness ok
    
    if ( _PtAcqRaw->Head.TagBeg != MIS1__BT_REC_TAG_BEGIN ) {
      
      err_retfailnull ( -1, (ERR_OUT,"Abort => Record TagBeg = %X <> MIS1__BT_REC_TAG_BEGIN = %X => Bad endian conversion or bad data format version", _PtAcqRaw->Head.TagBeg, MIS1__BT_REC_TAG_BEGIN ) );
      
    }
    
  }
  
  
  
  if ( PrintHead ) {
    VRet = MIS1__BT_FAcqRawHeadRecPrint ( &_PtAcqRaw->Head, _HeadPrintTriggers, _HeadPrintFrCnt, _HeadPrintFiredPixels );
    
    err_retfailnull ( VRet, (ERR_OUT,"Abort => Print first acq header failed ! Ret = %d", VRet) );
  }
  
  
  // Read data
  
  err_trace (( ERR_OUT, "Read data sz = %d W8", _PtAcqRaw->Head.DataSz ));
 
  
  VRecNbRead = fread ( &_PtAcqRaw->MSisData, _PtAcqRaw->Head.DataSz /* Record size */ , 1 /* Record number */, _CurRawFilePt );
  
  if ( VRecNbRead != 1 ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read acq data of first acq, file = %s failed !", _CurRawFileName ) );
  }   
  


  // Check endianness & convert
  
  if ( _CpuIsBigEndian ) {
    
    VRet = MIS1__FUserEndianConv ( &_PtAcqRaw->MSisData, _PtAcqRaw->Head.DataSz );
    
    err_retfailnull ( VRet, (ERR_OUT,"Abort => MIS1__FUserEndianConv (...) convert Acq data failed ! Ret = %d", VRet) );
    
  }


  return (_PtAcqRaw);
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FAcqFirstSeq ( UInt8 ChkHead, UInt8 PrintHead )
*          :
* \brief   : Returns a pointer to the first Acq of run \n
*          : \n
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FAcqFirstSeq ( UInt8 ChkHead, UInt8 PrintHead ) {
  
  SInt32 VRet;
  MIS1__TBtAcqRawRec* VPtAcq;
  
  // Check
  
  MIS1__TBtRunRead_CHECK_RET_NULL
  
  
  // Set first Acq  
  
  _CurAcqId = 0;
  
  // Read first Acq
  
  VPtAcq = _FFirstAcqOfRawFileGetSeq ( 0 /* RawFileId */, ChkHead, PrintHead );
  
  if ( VPtAcq == NULL ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read first acq of run failed ! file = %s", _CurRawFileName ) );
  }
  
  return ( VPtAcq );
}


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FAcqNextSeq ( UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd )
*          :
* \brief   : Returns a pointer to the xext Acq of run \n
*          : \n
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
* \param   : PtReachEn - A pointer on a flag set to 1 if end of file is reached, set it to NULL if not used
*          :
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FAcqNextSeq ( UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd ) {

  SInt32 VRet;
  UInt32 VRawFileId;
  MIS1__TBtAcqRawRec* VPtAcq;
  
  // Check
  
  MIS1__TBtRunRead_CHECK_RET_NULL
  
  
  // Set current Acq and calc raw file id
  
  _CurAcqId++;
  
  if ( _CurAcqId >= _RunCnfRec.AcqNb ) {
        
    _CurAcqId = _RunCnfRec.AcqNb;
    
    if ( PtReachEnd != NULL ) {
      *PtReachEnd = 1;
    }
    
    return ( _PtAcqRaw );
    
  }
  

  if ( PtReachEnd != NULL ) {
    *PtReachEnd = 0;
  }
  
  // Calc raw file id
  
  VRawFileId = _CurAcqId / _RunCnfRec.AcqNbPerFile;
  
  // Read next Acq
  
  if ( ( _CurAcqId % _RunCnfRec.AcqNbPerFile) == 0 ) {
    
    VPtAcq = _FFirstAcqOfRawFileGetSeq ( VRawFileId, ChkHead, PrintHead );
    
  }
  
  else {
    VPtAcq = _FNextAcqOfRawFileGetSeq ( VRawFileId /* RawFileId */, ChkHead, PrintHead, PtReachEnd );
  }
    
  
  
  if ( VPtAcq == NULL ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read next acq No = %d of run failed ! file = %s", _CurAcqId, _CurRawFileName ) );
  }
  
  return ( VPtAcq );
  
}






/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FAcqGotoSeq ( UInt32 AcqId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd )
*          :
* \brief   : Returns a pointer to the xext Acq of run \n
*          : \n
*          :
* \param   : AcqId     - Id / No of the Acq
*          :
* \param   : ChkHead   - Checks header fields values
*          :
* \param   : PrintHead - Prints header fields
*          :
* \param   : PtReachEn - A pointer on a flag set to 1 if end of file is reached, set it to NULL if not used
*          :
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */


MIS1__TBtAcqRawRec* MIS1__TBtRunRead::_FAcqGotoSeq ( UInt32 AcqId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd ) {
  
  SInt32 VRet;
  UInt32 VRawFileId;
  SInt32 ViAcq;
  MIS1__TBtAcqRawRec* VPtAcq;
  
  // Check
  
  MIS1__TBtRunRead_CHECK_RET_NULL
  
  // Check param
  
  if ( _CurAcqId >= _RunCnfRec.AcqNb ) {    
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read acq No = %d > Nb of acq in run = %d", AcqId, _RunCnfRec.AcqNb ) );
  }
  
    
  // Calc raw file id
  
  VRawFileId = AcqId / _RunCnfRec.AcqNbPerFile;
  
  
  // Scan the acq of file
  
  
  if ( PtReachEnd != NULL ) {
    *PtReachEnd = 0;
  }
  
  
  VPtAcq = _FFirstAcqOfRawFileGetSeq ( VRawFileId, ChkHead, 0 /* PrintHead */ );
  
  if ( VPtAcq == NULL ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort => Read acq No = %d of run failed ! file = %s", AcqId, _CurRawFileName ) );
  }
  
  
  if ( VPtAcq->Head.Ids.AcqIdInRun == AcqId) {
    
    
    if ( PrintHead ) {
      VRet = MIS1__BT_FAcqRawHeadRecPrint ( &VPtAcq->Head, _HeadPrintTriggers, _HeadPrintFrCnt, _HeadPrintFiredPixels );
      
      err_retfailnull ( VRet, (ERR_OUT,"Abort => Print acq header failed ! Ret = %d", VRet) );
    }
    
    
    return ( VPtAcq );
  }
   

  for ( ViAcq = 1; ViAcq < _RunCnfRec.AcqNbPerFile; ViAcq++ ) {
        
    VPtAcq = _FNextAcqOfRawFileGetSeq ( VRawFileId /* RawFileId */, ChkHead, 0 /* PrintHead */, PtReachEnd  );
    
    if ( VPtAcq == NULL ) {
      err_retfailnull ( -1, (ERR_OUT,"Abort => Read acq No = %d of run failed ! file = %s", AcqId, _CurRawFileName ) );
    }  
    
    if ( VPtAcq->Head.Ids.AcqIdInRun == AcqId) {
      
      if ( PrintHead ) {
        VRet = MIS1__BT_FAcqRawHeadRecPrint ( &VPtAcq->Head, _HeadPrintTriggers, _HeadPrintFrCnt, _HeadPrintFiredPixels );
        
        err_retfailnull ( VRet, (ERR_OUT,"Abort => Print acq header failed ! Ret = %d", VRet) );
      }
           
      
      return ( VPtAcq );
    }
    
  }
  
    
  return ( NULL );  
}



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::
*          :
* \brief   : \n
*          : \n
*          :
* \param   :  -
*          :
* \param   :
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 MIS1__TBtRunRead::
*          :
* \brief   : \n
*          : \n
*          :
* \param   :  -
*          :
* \param   :
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : Privare method
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 18/05/2021
* \date    : Doc date  : 18/05/2021
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */



#endif



