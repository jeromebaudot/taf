#include "mimo_daq_lib.h"

#include "errors_light.c"
#include "msg_light.c"


/* ========================== */
/* Application constants      */
/* ========================== */

#define MIS1__TBtRunRead_DEF_ERR_LOG_FILE "./Results/err_MIS1__TBtRunRead.txt"
#define MIS1__TBtRunRead_DEF_MSG_LOG_FILE "./Results/msg_MIS1__TBtRunRead.txt"

/* ========================== */
/* Application macros         */
/* ========================== */

#define APP__READ_CR { while ( getchar () != '\n' ); };

/* ============================= */
/* Application context type      */
/* ============================= */


