// --------------------------------------------------------------------------------------
// Includes required by DAQ library
// imported directly from Gilles Claus
// all the .def, .typ, .var or .c files are located in the include dir
// JB 2011/03/14
// Modified: JB 2012/07/19 to include sync_index_rec
// Modified: VR 2014/03/21 Configuration of APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_ CC1 and CC2 is now made in the "pxi_daq_lib_config.h" file

#ifndef DAQ_LIB_INCLUDE
#define DAQ_LIB_INCLUDE

#define _FILE_OFFSET_BITS 64  // Mandatory for large files ( > 2 GB ) handling
// MUST be set before standard C header files

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // types
#include <string.h>
#include <errno.h>


/* ========================================= */
/* Conditional compilation for DAQ sources   */
/* ========================================= */

#define ROOT_ROOT // Disable code parts which can't compile under ROOT
#define CC_UNIX   // Specify operating system

//**********************************
//  LIB CONFIGURATION
//**********************************
// Made in the "pxi_daq_lib_config.h" file
//**********************************
#include "pxi_daq_lib_config.h"
// test to check if configuration in "pxi_daq_lib_config.h" is well done
// (is one of the APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 variable defined ?)
#ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
  #ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
    #ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613
      #error in pxi_daq_lib_v.1.2 (daq_lib.h) : bad configuration for DATA_FORMAT_CC1 in file include/pxi_daq_lib_config.h 
    #endif
  #endif
#endif

#ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
  #ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012
    #error in pxi_daq_lib_v.1.2 (daq_lib.h) : bad configuration for DATA_FORMAT_CC2 in file include/pxi_daq_lib_config.h 
  #endif
#endif

//**********************************

/* ========================================= */
/* DAQ commun files -> types & constants     */
/* ========================================= */


#include "globals.def"
#include "globals_root.def"
#include "types.typ"

/* ================================================================ */
/* Definition of WIndows types & constant not available under Linux */
/* ---------------------------------------------------------------- */
/* Work like this now = allow to compile program, but should be     */
/* done in a better way later !!!!!!!!!                             */
/* ================================================================ */

#define HANDLE UInt32
#define DWORD  UInt32
#define WINAPI
#define LPVOID void*
#define INVALID_HANDLE_VALUE 0xFFFFFFFF


/* ========================================= */
/* Macros needed to compile DAQ sources      */
/* ========================================= */

#define EXTERN
#define VAR_STATIC
#define VAR_INIT(x) =x
#define VAR_INIT_A2(x,y) ={x,y}  
#define VAR_DCL(Extern,Static,Var) Extern Static Var
#define VAR_DCL_INIT(Extern,Static,Var,Init) Extern Static Var VAR_INIT (Init)
#define VAR_DCL_INIT_A2(Extern,Static,Var,Init0,Init1) Extern Static Var VAR_INIT_A2 (Init0,Init1)


/* ========================================= */
/* Includes DAQ source files                 */
/* ========================================= */

// Errors messages logging library interface 

#include "errors.def"
#include "errors.typ"
#include "errors.var"

// General messages logging library interface

#include "msg.def"
#include "msg.typ"
#include "msg.var"

// Time library interface

#include "time.def"
#include "time.typ"
#include "time.var"

// Files library interface

#include "files.def"
#include "files.typ"
#include "files.var"

// ASIC library interface

#include "asic.def"
#include "asic.typ"
#include "asic.var"

// MAPS library interface

#include "maps.def"
#include "maps.typ"
#include "maps.var"

// Mimosa 26 library interface

#include "mi26_usr.def"
#include "mi26_usr.typ"

// Eudet flex rio DAQ library interface

#include "eudet_frio.def"
#include "eudet_frio.typ"
#include "eudet_frio.var"

#endif

