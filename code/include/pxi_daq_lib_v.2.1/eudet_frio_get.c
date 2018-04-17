
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio.c
Goal      : Functions of flex rio board library for EUDET Mimosa 26 DAQ
Prj date  : 05/08/2010
File date : 05/08/2010
Doc date  : 05/08/2010
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


#ifndef EUDET_FRIO_GET_C
#define EUDET_FRIO_GET_C





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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 09/08/2010
Rev       : 25/10/2010
          : - Rename EFRIO__FGetZsFFrameRawFields in EFRIO__FGetFrameRawFields
          : - Handle both IPHC "ZsFFrameRaw" and EUDET "Frame"
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


UInt32* EFRIO__FGetFrameFields ( SInt32 AcqId, SInt8 Mi26Id, SInt32 FrameIdInAcq, UInt32* PtDest, SInt8 DestW32Sz ) {
  
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  SInt8               ViField;
  SInt8               VFieldNb = 15;
  EFRIO__TTriggerRec* VPtTrigRec;
  SInt8               ViTrigInf;
  
  static UInt32 VAFields[15];
  
  
  if ( AcqId > 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }

  for ( ViField=0; ViField < VFieldNb; ViField++ ) {
    VAFields[ViField] = 0;
  }


  // Set default values used in case of error

  VAFields [0] = 0x11111111; // Header
  VAFields [1] = 100;        // FrameCnt
  VAFields [2] = 200;        // DataLength
  VAFields [3] = 0x11111111; // Trailer
  VAFields [4] = 0x01;       // Zero
  VAFields [5] = 0x02;       // Zero2
  VAFields [6] = 0;          // Trigger number
  VAFields [7] = 0;          // Trigger 1
  VAFields [8] = 0;          // Trigger 2
  VAFields [9] = 0;          // Trigger 3
  VAFields[10] = 0;          // Last trigger
  VAFields[11] = 0;          // Time stamp 1
  VAFields[12] = 0;          // Time stamp 2
  VAFields[13] = 0;          // Time stamp 3
  VAFields[14] = 0;          // Last stamp


  while (1) {
    
    // IPHC mode
    
    if ( VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_IPHC ) {
            
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
            
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      
      VAFields[0] = VPtFrame->Header;
      VAFields[1] = VPtFrame->FrameCnt;
      VAFields[2] = VPtFrame->DataLength;
      VAFields[3] = VPtFrame->Trailer;
      VAFields[4] = VPtFrame->Zero;
      VAFields[5] = VPtFrame->Zero2;
      VAFields [6] = (VPtFrame->Zero & 0xFFFF0000) >> 16;    // Trigger number
      VAFields [7] = (VPtFrame->Zero & 0x0000FFFF);          // Trigger 1
      VAFields [8] = (VPtFrame->Zero2 & 0xFFFF0000) >> 16;   // Trigger 2
      VAFields [9] = (VPtFrame->Zero2 & 0x0000FFFF);         // Trigger 3
      VAFields[10] = 0;          // Last trigger
      VAFields[11] = 0;          // Time stamp 1
      VAFields[12] = 0;          // Time stamp 2
      VAFields[13] = 0;          // Time stamp 3
      VAFields[14] = 0;          // Last stamp
      
      break;
    }

    // EUDET 1 mode
    
    if ( VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN ) {

      if ( VPtFrList->AFramePtr[FrameIdInAcq] == NULL ) {
        break;
      }

      VPtTrigRec = (EFRIO__TTriggerRec*) ( ((UInt8*) VPtFrList->AFramePtr[FrameIdInAcq]) + VPtFrList->AFramePtr[FrameIdInAcq]->TrigRecOffset );
            
      VAFields[0] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsHeader[Mi26Id];
      VAFields[1] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsFrameCnt[Mi26Id];
      VAFields[2] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsDataLength[Mi26Id];
      VAFields[3] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrailer[Mi26Id];
      VAFields[4] = 0; // Zero
      VAFields[5] = 0; // Zero2

      VAFields[6] = VPtTrigRec->TrigNb;
      
      if ( VPtTrigRec->TrigNb > 0 ) {
        VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
        VAFields [8] = VPtTrigRec->ATrig[1]; // Trigger 2
        VAFields [9] = VPtTrigRec->ATrig[2]; // Trigger 3
        VAFields[10] = 0;          // Last trigger
        VAFields[11] = 0;          // Time stamp 1
        VAFields[12] = 0;          // Time stamp 2
        VAFields[13] = 0;          // Time stamp 3
        VAFields[14] = 0;          // Last stamp
      }
      
      else {
        VAFields [7] = 0; // Trigger 1
        VAFields [8] = 0; // Trigger 2
        VAFields [9] = 0; // Trigger 3
        VAFields[10] = 0; // Last trigger
        VAFields[11] = 0; // Time stamp 1
        VAFields[12] = 0; // Time stamp 2
        VAFields[13] = 0; // Time stamp 3
        VAFields[14] = 0; // Last stamp
      }
      

      break;
    }

    // EUDET 2 & 3 mode
    
    if ( (VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES) ||(VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG) ) {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] == NULL ) {
        if ( AcqId >= 0 ) {
          err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
        }
        break;
      }
              
      VPtTrigRec = (EFRIO__TTriggerRec*) ( ((UInt8*) VPtFrList->AFramePtr[FrameIdInAcq]) + VPtFrList->AFramePtr[FrameIdInAcq]->TrigRecOffset );
            
      VAFields[0] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsHeader[Mi26Id];
      VAFields[1] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsFrameCnt[Mi26Id];
      VAFields[2] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsDataLength[Mi26Id];
      VAFields[3] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrailer[Mi26Id];
      VAFields[4] = 0; // Zero
      VAFields[5] = 0; // Zero2
      
      VAFields[6] = VPtTrigRec->TrigNb;
       
      if ( VPtTrigRec->TrigNb > 0 ) {
        
        switch ( VPtTrigRec->TrigNb ) {
          
          case 1 : {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            break; }
          
          case 2: {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields [8] = VPtTrigRec->ATrig[2]; // Trigger 2
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            VAFields[12] = VPtTrigRec->ATrig[3]; // Time stamp 2
            break; }
          
          case 3 : {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields [8] = VPtTrigRec->ATrig[2]; // Trigger 2
            VAFields [9] = VPtTrigRec->ATrig[4]; // Trigger 3
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            VAFields[12] = VPtTrigRec->ATrig[3]; // Time stamp 2
            VAFields[13] = VPtTrigRec->ATrig[5]; // Time stamp 3
            break; }
          
          default : {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields [8] = VPtTrigRec->ATrig[2]; // Trigger 2
            VAFields [9] = VPtTrigRec->ATrig[4]; // Trigger 3
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            VAFields[12] = VPtTrigRec->ATrig[3]; // Time stamp 2
            VAFields[13] = VPtTrigRec->ATrig[5]; // Time stamp 3
            break; }          

        } // End switch
        
        if ( VPtTrigRec->TrigNb > 3 ) {
          VAFields[10] = VPtTrigRec->ATrig[(2 * (VPtTrigRec->TrigNb - 1))];     // Last trigger
          VAFields[14] = VPtTrigRec->ATrig[(2 * (VPtTrigRec->TrigNb - 1)) + 1]; // Last time stamp
        }
        
        else {
          VAFields[10] = 0;          // Last trigger
          VAFields[14] = 0;          // Last time stamp
        }
        
      } // End if ( VPtTrigRec->TrigNb > 0 )
      
      else {
        VAFields [7] = 0; // Trigger 1
        VAFields [8] = 0; // Trigger 2
        VAFields [9] = 0; // Trigger 3
        VAFields[10] = 0; // Last trigger
        VAFields[11] = 0; // Time stamp 1
        VAFields[12] = 0; // Time stamp 2
        VAFields[13] = 0; // Time stamp 3
        VAFields[14] = 0; // Last stamp        
      }
      
      break;
    }

    // If case not handled -> Return default values

    VAFields [0] = 0x11111111; // Header
    VAFields [1] = 100;        // FrameCnt
    VAFields [2] = 200;        // DataLength
    VAFields [3] = 0x11111111; // Trailer
    VAFields [4] = 0x01;       // Zero
    VAFields [5] = 0x02;       // Zero2
    VAFields [6] = 0;          // Trigger number
    VAFields [7] = -1;         // Trigger 1
    VAFields [8] = -1;         // Trigger 2
    VAFields [9] = -1;         // Trigger 3
    VAFields[10] = -1;         // Last trigger
    VAFields[11] = -1;         // Time stamp 1
    VAFields[12] = -1;         // Time stamp 2
    VAFields[13] = -1;         // Time stamp 3
    VAFields[14] = -1;         // Last stamp
    break;
  
  } // End while (1)
  
  
  if ( (PtDest != NULL) && (DestW32Sz >= VFieldNb) ) {
    
    for ( ViField=0; ViField < VFieldNb; ViField++ ) {
      PtDest[ViField] = VAFields[ViField];
    }
    
    return (PtDest);
  }
  
  err_warning (( ERR_OUT, "Use local array because : PtDest=%x == NULL or DestW32Sz=%d < 10", PtDest, DestW32Sz ));
  
  return (VAFields);
}



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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameHeader ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  
  EFRIO__TTriggerRec* VPtTrigRec;
  SInt8               ViTrigInf;
    
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
  
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;            
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = VPtFrame->Header; // FrameCnt, DataLength, Trailer, Zero, Zero2
      break; }

    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      // VPtTrigRec = (EFRIO__TTriggerRec*) ( ((UInt8*) VPtFrList->AFramePtr[FrameIdInAcq]) + VPtFrList->AFramePtr[FrameIdInAcq]->TrigRecOffset );
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsHeader[Mi26Id];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }

    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch


//      VAFields[0] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsHeader[Mi26Id];
//      VAFields[1] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsFrameCnt[Mi26Id];
//      VAFields[2] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsDataLength[Mi26Id];
//      VAFields[3] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrailer[Mi26Id];
  
  return (VField);
}


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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameFrameCnt ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = VPtFrame->FrameCnt; 
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsFrameCnt[Mi26Id];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
    
  return (VField);
}

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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameDataLength ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
    
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = VPtFrame->DataLength; 
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsDataLength[Mi26Id];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_error (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
    
  return (VField);
}

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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameTrailer ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
    
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = VPtFrame->Trailer; 
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrailer[Mi26Id];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_error (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
    
  return (VField);
}


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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 09/06/2010
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetFrameTrigStatus ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  SInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  SInt32              ViFrame;
  
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      VField   = -1; // Not available
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG :
    {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.TrigStatus;
      }
      
      else {
        VField  = -1; 
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField  = -1; 
      break; }
    
  } // End switch
  
  return (VField);
}



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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       : 07/06/2011
          : - Handle TrigNb < 0 => Workarround of fw trigger bug => function returns SInt32 now
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetFrameTrigCnt ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  SInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
    
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
        
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = (VPtFrame->Zero & 0xFFFF0000) >> 16; 
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG :  
    {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.TriggerNb;
      }
      
      else {
        // VField  = 0xFFFFFFFF;
        VField  = 666; // 07/06/2011
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      // VField  = 0xFFFFFFFF;
      VField  = 666; // 07/06/2011
      break; }
    
  } // End switch
  
  return (VField);
}

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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameMi26Trig0 ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
    
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = (VPtFrame->Zero & 0x0000FFFF); 
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrigInfo[0];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
  
  
  
  return (VField);
}

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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameMi26Trig1 ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
    
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = (VPtFrame->Zero2 & 0xFFFF0000) >> 16;
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrigInfo[1];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
  
    
  return (VField);
}


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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameMi26Trig2 ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
    
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = (VPtFrame->Zero2 & 0x0000FFFF);
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrigInfo[2];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
  
  return (VField);
}

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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameTluTrig ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq, SInt16 TrigNo ) {
  
  
  UInt32              VField;
  SInt32              VFieldIndex;
  SInt32              VTrigNb;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  
  EFRIO__TTriggerRec* VPtTrigRec;
  SInt8               ViTrigInf;
  
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  if ( TrigNo < 0 ) {
    err_warning (( ERR_OUT, "TrigNo=%d < 0", TrigNo ));
    return (-1);
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : 
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN : {
      VField   = 0xFFFFFFFF;
      err_error (( ERR_OUT, "No TLU trigger field in EFRIO__TRF_MODE_IPHC & EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN" ));
      break; }
    
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VTrigNb     = VPtFrList->AFramePtr[FrameIdInAcq]->Header.TriggerNb;
        VFieldIndex = TrigNo * 2;
        
        if ( VFieldIndex >= (2 * VTrigNb) ) {
          VField  = 0xFFFFFFFF;
          err_warning (( ERR_OUT, "Trigger index=%d out of range - TrigNb=%d", VFieldIndex, VTrigNb ));
        }
        
        else {
          VPtTrigRec = (EFRIO__TTriggerRec*) ( ((UInt8*) VPtFrList->AFramePtr[FrameIdInAcq]) + VPtFrList->AFramePtr[FrameIdInAcq]->TrigRecOffset );
          VField     = VPtTrigRec->ATrig[VFieldIndex];
        }
        
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
    
  
  return (VField);
}

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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 08/11/2010
Rev       : 07/06/2011
          : - Handle TrigNb < 0 => Workarround of fw trigger bug
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 EFRIO__FGetFrameFlexRioTrig ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq, SInt16 TrigNo ) {
  
  
  UInt32              VField;
  SInt32              VFieldIndex;
  SInt32              VTrigNb;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  
  EFRIO__TTriggerRec* VPtTrigRec;
  SInt8               ViTrigInf;
  
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }

  if ( TrigNo < 0 ) {
    err_warning (( ERR_OUT, "TrigNo=%d < 0", TrigNo ));
    return (-1);
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC :
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN : {
      VField   = 0xFFFFFFFF;
      err_error (( ERR_OUT, "No Flex RIO trigger field in EFRIO__TRF_MODE_IPHC & EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN" ));
      break; }
    
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        
        VTrigNb     = VPtFrList->AFramePtr[FrameIdInAcq]->Header.TriggerNb;
        
        VTrigNb = abs ( VTrigNb ); // 07/06/2011 => TrigNb < 0 if fw bug shift two frames
        
        VFieldIndex = (TrigNo * 2) + 1; 
                
        if ( /* !!! 08/06/2011 Force read !!! */ /* VFieldIndex >= (2 * VTrigNb) */ 0 ) {
          VField  = 0xFFFFFFFF;
          err_warning (( ERR_OUT, "Trigger index=%d out of range - TrigNb=%d", VFieldIndex, VTrigNb ));
        }
        
        else {
          VPtTrigRec = (EFRIO__TTriggerRec*) ( ((UInt8*) VPtFrList->AFramePtr[FrameIdInAcq]) + VPtFrList->AFramePtr[FrameIdInAcq]->TrigRecOffset );
          VField     = VPtTrigRec->ATrig[VFieldIndex];
        }
        
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
  
  
  return (VField);
}

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
Date      : 09/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

float EFRIO__FGetDataRateMBPerSec ( SInt8 PrintInMsgFile ) {
    
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;

  if ( PrintInMsgFile == 1 ) {
    msg (( MSG_OUT, "Data rate - ResAcqCnt=%d - Time=%d [ms] - Size=%d [Bytes] - DR=%.3f [MB/s]))", VPtRunCont->ResAcqCnt, VPtRunCont->InfDataRateMeasTotalTimeMs, VPtRunCont->InfDataRateMeasTotalSz, VPtRunCont->ResDataRateMBytesPerSec ));            
  }

  return (VPtRunCont->ResDataRateMBytesPerSec);
}



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
Remark    : Param AcqId not used NOW
          : DEBUG tools, process ONLY first state in data stream, ignore Mi26Id param.
          :
Level     :
Date      : 21/06/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetFrameFirstStateLineWordStatesNb ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  SInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  MI26__TStatesLine   VStatesLines;
  SInt32              ViFrame;
  
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }

  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      VField   = -1;
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Data.ADataW32[0];
        VStatesLines.W16 = VField;
        VField = VStatesLines.F.StateNb; // LineAddr
      }
      
      else {
        VField  = -1;
        err_error (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
  
  return (VField);
}




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
Remark    : Param AcqId not used NOW
          : DEBUG tools, process ONLY first state in data stream, ignore Mi26Id param.
          :
Level     :
Date      : 21/06/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetFrameFirstStateLineWordLineNo ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  SInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  MI26__TStatesLine   VStatesLines;
  SInt32              ViFrame;
  
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      VField   = -1;
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Data.ADataW32[0];
        VStatesLines.W16 = VField;
        VField = VStatesLines.F.LineAddr;
      }
      
      else {
        VField  = -1;
        err_error (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
  
  return (VField);
}



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
Remark    : Param AcqId not used NOW
          : DEBUG tools
          : -> process ONLY first state in data stream, ignore Mi26Id param.
          : -> works only if 6 Ultimate are acquired ( can be emul mode ) + extra chan enabled            
:
Level     :
Date      : 21/06/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetFrameFirstGroupStatesNb ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  SInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  MI26__TStatesLine   VStatesLines;
  SInt32              ViFrame;
  UInt16*             VPtDataW16;
  SInt8               VStatesNbInLine;
  SInt8               ViState;
  SInt8               VFirstGroupStatesNb;
  MI26__TState        VState;
  
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      VField   = -1;
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        
        VStatesLines.W16    = VPtFrList->AFramePtr[FrameIdInAcq]->Data.ADataW32[0];
        VPtDataW16          = (UInt16*) VPtFrList->AFramePtr[FrameIdInAcq]->Data.ADataW32;
        ++VPtDataW16; // Point on first W16 following State/Line W16
        VStatesNbInLine     = VStatesLines.F.StateNb;       
        VFirstGroupStatesNb = 0;
        
        for ( ViState = 0; ViState < VStatesNbInLine; ViState++ ) {
          VState.W16 = VPtDataW16[(ViState * 7)];
          
          if ( (VState.F.ColAddr >= 0) && (VState.F.ColAddr <= 63) ) {
            ++VFirstGroupStatesNb;
          }
          
        }
               
        VField = VFirstGroupStatesNb;
      }
      
      else {
        VField  = -1;
        err_error (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  } // End switch
  
  return (VField);
}




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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 23/01/2014
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetFrameHitCnt ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq ) {
  
  
  UInt32              VField;
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  EFRIO__TTestOnDataCont*  VPtTestCont = &EFRIO__VGContext.TestOnDataCont;  
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
    return (-1);
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }


  // return ( EFRIO__FGetFrameDataLength ( AcqId, Mi26Id, FrameIdInAcq ) );
  
  // 04/02/14
  // WARNING => Tmp code to return hits count, No chekc of Mi26Id !!!!
  
  return ( VPtTestCont->ResAMapsMatrixHitCnt[Mi26Id] );
  
/*  
  
  switch ( VPtRunCont->ParDataTransferMode ) {
    
    case EFRIO__TRF_MODE_IPHC : {
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      VField   = VPtFrame->FrameCnt;
      break; }
    
    case EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN :
    case EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES :
    case EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG : {
      
      if ( VPtFrList->AFramePtr[FrameIdInAcq] != NULL ) {
        VField  = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsFrameCnt[Mi26Id];
      }
      
      else {
        VField  = 0xFFFFFFFF;
        err_warning (( ERR_OUT, "Frame=%d NOT in list (pointer = NULL) !", FrameIdInAcq ));
      }
      
      break; }
    
    default : {
      err_error (( ERR_OUT, "Unknown ParDataTransferMode=%d", VPtRunCont->ParDataTransferMode ));
      VField = 0xFFFFFFFF;
      break; }
    
  }
  
*/  
  
  return (VField);
}



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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 06/11/2010
Rev       : 
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


EFRIO__TFrame* EFRIO__FGetFramePt ( SInt32 AcqId, SInt32 FrameIdInAcq ) {
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];


  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
          
  if ( VPtFrList->AFramePtr[FrameIdInAcq] == NULL ) {
    err_retfailnull ( -1, (ERR_OUT,"No frame=%d in list => Pointer NULL", FrameIdInAcq) );
  }
  
  return ( VPtFrList->AFramePtr[FrameIdInAcq] );
}







/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Get board conf fields
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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtDest ) {
  
  err_trace (( ERR_OUT, "begin" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  err_retnull ( PtDest, (ERR_OUT,"Abort : PtDest == NULL") );
  
  *PtDest = EFRIO__VGContext.ABoardsConf[BoardId];
  
  err_retok (( ERR_OUT, "end" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Get board conf fields
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
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FGetBoardConfBoardId ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].BoardId);
}

// 10/08/2010

char* EFRIO__FGetBoardConfAsicName ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    return (NULL);
  }
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].AsicName);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfAsicNb ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].AsicNb);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfReadoutMode ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].ReadoutMode);
}

// 11/08/2010

SInt8 EFRIO__FGetBoardConfEmuleChannels ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EmuleChannels);
}


// 10/08/2010

float EFRIO__FGetBoardConfDataClkFrequency ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].DataClkFrequency);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfDmaHostSz ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].DmaHostSz);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfFrameNbPerAcq ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].FrameNbPerAcq);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfEnableExtraChannel ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EnableExtraChannel);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfAcqNbPerTrig ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].AcqNbPerTrig);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfTriggerMode ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TriggerMode);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfTriggerDetectTimeWindow ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectTimeWindow);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfTriggerDetectOccurNb ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectOccurNb);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfTimeStampRes ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TimeStampRes);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfEnableTimeStamping ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EnableTimeStamping);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfEnableTrigCnt ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EnableTrigCnt);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfTagEventsStoredByDUT ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TagEventsStoredByDUT);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfReadTluTrigCntEachNTrig ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].ReadTluTrigCntEachNTrig);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Get board status fields
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
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FGetBoardStatusBoardId ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].BoardId);
}

// 10/08/2010

SInt8  EFRIO__FGetBoardStatusBoardPresent ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].BoardPresent);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardStatusFwLoaded ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].FwLoaded);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardStatusConfDone ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].ConfDone);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardStatusStatusCode ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].StatusCode);
}

// 10/08/2010

char* EFRIO__FGetBoardStatusStatusStr ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
    
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    err_error (( ERR_OUT, "Abort : BoardId=%d out of range [0..%d]", BoardId, EFRIO__MAX_BOARDS_NB-1 ));
    return (NULL);
  }
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].StatusStr);
}


// 10/08/2010

char* EFRIO__FGetBoardStatusErrorMsgList ( SInt32 BoardId, SInt32 Index ) {
  
  SInt32 VMsgLen;
  EFRIO__TBoardStatus* VPtStatus;
  
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    err_error (( ERR_OUT, "Abort : BoardId=%d out of range [0..%d]", BoardId, EFRIO__MAX_BOARDS_NB-1 ));
    return (NULL);
  }
  
  VPtStatus = &EFRIO__VGContext.ABoardsStatus[BoardId];
  
  // Check index limits
  
  if ( (Index < 0) || (Index >= EFRIO__ERROR_MSG_LIST_MAX_NB ) ) {
    err_error (( ERR_OUT, "Abort : Index=%d out of range [0..%d]", Index, EFRIO__ERROR_MSG_LIST_MAX_NB-1 ));
    return (NULL);
  }
  
  return ( VPtStatus->ErrorMsgList[Index] );  
}

// 10/08/2010

char* EFRIO__FGetBoardStatusLastErrorMsg ( SInt32 BoardId ) {
  
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    err_error (( ERR_OUT, "Abort : BoardId=%d out of range [0..%d]", BoardId, EFRIO__MAX_BOARDS_NB-1 ));
    return (NULL);
  }
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].LastErrorMsg);
}

// 21/12/2010

SInt32 EFRIO__FGetCmdRun () {
  
  if ( (EFRIO__VGContext.RunCont.CmdRun == 0) || (EFRIO__VGContext.RunCont.CmdRun == 1) ) {
    // msg (( MSG_OUT, "### EFRIO__FGetCmdRun () = %d", EFRIO__VGContext.RunCont.CmdRun ));
  }
    
  return ( EFRIO__VGContext.RunCont.CmdRun );
}

// 15/02/2011

SInt32 EFRIO__FGetMonInfFrameNbToSend () {
  
  return (EFRIO__VGContext.MonCont.InfFrameNbToSend);
}

// 15/02/2011

SInt32 EFRIO__FGetMonInfSzToSend () {
  
  return (EFRIO__VGContext.MonCont.InfSzToSend);
}

// 16/02/2011

SInt32 EFRIO__FGetRunContSendOnEth () {
  
  return (EFRIO__VGContext.RunCont.ParSendOnEth);
}

// 11/03/2011

SInt32 EFRIO__FGetRunContFrameNbPerAcq () {
  
  return (EFRIO__VGContext.RunCont.ParFrameNbPerAcq);
}

// 02/05/2011

SInt32 EFRIO__FGetTestOnDataHeaderErrCnt ( SInt8 MapsId ) {

  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  
  if ( (MapsId < 0) ||(MapsId >= VPtRunCont->ParMi26Nb) ) {
    err_warning ((ERR_OUT,"Abort : Bad MapsId=%d out of range [0..%d]", MapsId, VPtRunCont->ParMi26Nb-1 ) );
    return (0);
  }
  
  return (EFRIO__VGContext.TestOnDataCont.ResAMapsHeaderErrCnt[MapsId]);
}

// 02/05/2011

SInt32 EFRIO__FGetTestOnDataTotErrCnt () {
  
  return (EFRIO__VGContext.TestOnDataCont.ResTotErrCnt);
}

// 25/04/2013

SInt32 EFRIO__FGetTestOnDataErrOnCurrentAcq () {
  
  return (EFRIO__VGContext.TestOnDataCont.ResErrOnCurrentAcq);
}



#endif
