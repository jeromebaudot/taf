// Creation date: 21/05/2021
// Author: Ziad EL BITAR
// Author: ziad.elbitar@iphc.cnrs.fr
// Based on daq_lib.h in PXIeBoardReader


#ifndef PXI_DAQ_LIB_VERSION_4_1
#define PXI_DAQ_LIB_VERSION_4_1

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

//---------------------------------------------------------------------------------
// Conditionnal compilation switches
// ---------------------------------------------------------------------------------


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

/* ========================== */
/* Application constants      */
/* ========================== */

#define MIS1__TBtRunRead_DEF_ERR_LOG_FILE "./Results/err_MIS1__TBtRunRead.txt"
#define MIS1__TBtRunRead_DEF_MSG_LOG_FILE "./Results/msg_MIS1__TBtRunRead.txt"
/* ========================================= */
/* Includes DAQ source files                 */
/* ========================================= */

// C4Pi test team general lib interface

#include "c4pi_test_team.h"
#include "globals_root.def"
#include "types.def"

// C4Pi test team general errors and general messages lib interface

#include "errors_light.h"
#include "msg_light.h"

// C4Pi test team Mimosis 1 data lib interface


#include "msis1_data.def"
#include "msis1_data.typ"
#include "msis1_data_exp.h"

// C4Pi test team general errors and general messages lib
// C code inclusion in application
//
// Remark : Ugly piggy way, but fast way and no time to do better ;-)
// anyway if someone wants to do better => please do


#include "errors_light.c"

#include "msg_light.c"


// C4Pi test team general Mimossi 1 data lib
// C code inclusion in application
//
// Remark : Ugly piggy way, but fast way and no time to do better ;-)
// anyway if someone wants to do better => please do


#include "msis1_data.c" // From common source tree "X:/lib/com/maps/msis1/Data/msis1_data.c"


#endif
