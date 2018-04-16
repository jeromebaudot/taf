
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


#ifndef EUDET_FRIO_DBG_C
#define EUDET_FRIO_DBG_C




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
Date      : 20/10/2010
Doc date  : 20/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// Cmd = 0 => do nothing
// Cmd = 1 => alloc mem
// Cmd = 2 => free mem

// Cmd =  3 => Copy with memcpy
// Cmd =  4 => Copy with W8 loop Vi ++
// Cmd =  5 => Copy with W8 loop Vi --
// Cmd =  6 => Copy with W32 loop Vi ++
// Cmd =  7 => Copy with W32 loop Vi --
// Cmd =  8 => Copy with memmove
// .....
// Cmd = 12 => Copy with W64 loop Vi ++

SInt32 EFRIO__FDbgMeasRamCpyTime ( SInt8 Cmd, SInt32 SzMb ) {

  static UInt8* VBuffSrc  = NULL;
  static UInt8* VBuffDest = NULL;
  auto   SInt32 _VSzW32;
  auto   SInt32 _Vi;
  register   SInt32 r_VSzW32;
  register   SInt32 r_VSzW64;
  register   SInt32 r_Vi;
  SInt32  VSzW8;
          SInt32 VSzW32;
          SInt32 VSzW64;
          SInt32 Vi;
  UInt8*  VCpyBuffSrc;
  UInt8*  VCpyBuffDest;
  UInt64* VCpyBuffSrcW64;
  UInt64* VCpyBuffDestW64;
  

  if ( (Cmd < 0) || (Cmd > 7) ) {
    err_retfail ( Cmd, (ERR_OUT,"Bad Cmd=%d Our of range 0..7", Cmd) );
  }

  VSzW8    = SzMb * 1024 * 1024;
  VSzW32   = VSzW8 / 4;
  _VSzW32  = VSzW8 / 4;
  r_VSzW32 = VSzW8 / 4;
  VSzW64   = VSzW8 / 8;
  r_VSzW64 = VSzW8 / 8;
  
  
  while (1) {
    
    // Allocation mem
    
    if ( Cmd == 1 ) {
      
      err_error (( ERR_OUT, "TRACE => Cmd=1 Alloc mem" ));
      
      // Source
      
      if ( VBuffSrc != NULL ) {
        free ( VBuffSrc );
      }
      
      VBuffSrc = (UInt8*) malloc ( VSzW8 );
      
      err_retnull ( VBuffSrc, (ERR_OUT,"Allocation of src buffer %d bytes failed !", VSzW8 ) );

      // Dest

      if ( VBuffDest != NULL ) {
        free ( VBuffDest );
      }
      
      VBuffDest = (UInt8*) malloc ( VSzW8 );
      
      err_retnull ( VBuffDest, (ERR_OUT,"Allocation of dest buffer %d bytes failed !", VSzW8 ) );
      
      break;
    }

    // Free mem
    
    if ( Cmd == 2 ) {

      err_error (( ERR_OUT, "TRACE => Cmd=2 => Free mem" ));
      
      if ( VBuffSrc != NULL ) {
        free ( VBuffSrc );
        VBuffSrc = NULL;
      }
      
      if ( VBuffDest != NULL ) {
        free ( VBuffDest );
        VBuffDest = NULL;
      }      
      
     break; 
    }

    // Copy with memcpy
    
    if ( Cmd == 3 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
        
      memcpy ( VBuffDest, VBuffSrc, VSzW8 );
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W8 loop Vi ++
    
    if ( Cmd == 4 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
            
      for ( Vi=0; Vi < VSzW8; Vi++ ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
           
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W8 loop Vi --
    
    if ( Cmd == 5 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( Vi=VSzW8; Vi >= 0; Vi-- ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi ++
    
    if ( Cmd == 6 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( Vi=0; Vi < VSzW32; Vi++ ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi --
    
    if ( Cmd == 7 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( Vi=VSzW32; Vi >= 0; Vi-- ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }
    
    // Copy with memmove
    
    if ( Cmd == 8 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      memmove ( VBuffDest, VBuffSrc, VSzW8 );
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }


    // Copy with W32 loop Vi ++ with auto var
    
    if ( Cmd == 9 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( _Vi=0; _Vi < _VSzW32; _Vi++ ) {
        VBuffDest[_Vi] = VBuffSrc[_Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi ++ with local ptr
    
    if ( Cmd == 10 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
      VCpyBuffSrc  = VBuffSrc;
      VCpyBuffDest = VBuffDest;
  
      for ( Vi=0; Vi < VSzW32; Vi++ ) {
        VCpyBuffDest[Vi] = VCpyBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi ++ with register var
    
    if ( Cmd == 11 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( r_Vi=0; r_Vi < r_VSzW32; r_Vi++ ) {
        VBuffDest[r_Vi] = VBuffSrc[r_Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }
    

    // Copy with W64 loop Vi ++ with local ptr
    
    if ( Cmd == 12 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      VCpyBuffSrcW64  = (UInt64*) VBuffSrc;
      VCpyBuffDestW64 = (UInt64*) VBuffDest;
      
      for ( Vi=0; Vi < VSzW64; Vi++ ) {
        VCpyBuffDestW64[Vi] = VCpyBuffSrcW64[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }
    

    // Copy with W64 loop Vi ++ register var - with local ptr
    
    if ( Cmd == 13 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      VCpyBuffSrcW64  = (UInt64*) VBuffSrc;
      VCpyBuffDestW64 = (UInt64*) VBuffDest;
      
      for ( r_Vi=0; r_Vi < r_VSzW64; r_Vi++ ) {
        VCpyBuffDestW64[r_Vi] = VCpyBuffSrcW64[r_Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }
    

    // End while (1)
  }

  

  return (0);
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
Date      : 22/10/2010
Doc date  : 22/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */



// Cmd = 0 -> Reset frame counter & flags ( don't care about trigger param )
// Cmd = 1 -> Insert text line separator "=============" ( do nothing else )
// Cmd = 2 -> Emule frame

SInt32 EFRIO__FDbgCheckTrigAlgo ( SInt8 Cmd, SInt8 TriggerNb, SInt8 AddFrameBefore, SInt8 AddFrameAfter ) {

  static SInt32 VFrameCnt        = 0;
  static SInt8  VAddFrameNp1     = 0;
  static SInt8  VAddFrameNm1Done = 0;
         SInt32 VRetCurFrameId;
  
  while (1) {
    
    if ( Cmd == 0 ) {
      VFrameCnt        = 0;
      VAddFrameNp1     = 0;
      VAddFrameNm1Done = 0;
      VRetCurFrameId   = -1;
      msg (( MSG_OUT, "Cmd = 0 -> Reset frame cnt done" ));
      break;
    }
    
    
    if ( Cmd == 1 ) {
      VRetCurFrameId   = -1;
      msg (( MSG_OUT, "*******************************************************" ));
      break;
    }
    
    
    if ( Cmd == 2 ) {
      
      VRetCurFrameId = VFrameCnt;
      
      msg (( MSG_OUT, "-------------------------------------------------------" ));
      msg (( MSG_OUT, "Emule frame %d - %d triggers - Add F before = %d - Add F after = %d", VFrameCnt, TriggerNb, AddFrameBefore, AddFrameAfter ));
      msg (( MSG_OUT, "-------------------------------------------------------" ));
      

      if ( TriggerNb > 0 ) {
        
        if ( AddFrameAfter == 1 ) {
          VAddFrameNp1 = 1;
        }
        
        
        // Must add Fn-1
        
        if ( (VAddFrameNm1Done == 0) && (AddFrameBefore == 1) ) {
          msg (( MSG_OUT, "Add Fn-1 = %d", VFrameCnt-1 ));
        }
        
        msg (( MSG_OUT, "Add Fn = %d", VFrameCnt ));
        VAddFrameNm1Done = 1;
        
      } // End if ( TriggerNb > 0 )
      
      
      // TriggerNb == 0
      
      else {
      
        // Must add Fn+1
      
        if ( VAddFrameNp1 == 1 ) {
          VAddFrameNp1 = 0;

          msg (( MSG_OUT, "Add Fn = %d", VFrameCnt ));
          VAddFrameNm1Done = 1;
        }
        
        // Don't add Fn+1
        
        else {
          VAddFrameNm1Done = 0;
        }
    
      } // End TriggerNb == 0
      
      
      break; // End frame emulation
    }
    
    
    
    msg (( MSG_OUT, "Unknown command = %d", Cmd ));
    break;
  }
    
  ++VFrameCnt;
  
  return (VRetCurFrameId);
}


  


#endif
