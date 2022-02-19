
/**
* ----------------------------------------------------------------------------------
* \file              X:\prj\win\mimosis_1\run_read_light_bt\com\c4pi_test_team.h
* \brief Goal      : C4Pi test team C source definitions : Constants, types, functins proto, ...
* \brief
* \brief
* \version         : 1.0
* \date Prj date   : 12/05/2021
* \date File date  : 12/05/2021
* \date Doc date   : 12/05/2021
* \author          : Gilles CLAUS
* \author          : gilles.claus@iphc.cnrs.fr
* \author          : CNRS - IN2P3 - IPHC 23 Rue du Loess 67037 STRASBOURG
*
* Remark : None
*
* ----------------------------------------------------------------------------------
* License   : GNU General Public License
*
* ----------------------------------------------------------------------------------
*/


#ifndef C4PI_TEST_TEAM_H
#define C4PI_TEST_TEAM_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// -----------------------------------------------------------------------
// Compilation directives
// -----------------------------------------------------------------------

#define CC_MSIS1_BDF_LIGHT


// -----------------------------------------------------------------------
// Macro definition
// -----------------------------------------------------------------------




// -----------------------------------------------------------------------
// Types definition
// -----------------------------------------------------------------------


#ifndef UInt8
typedef uint8_t UInt8;
#endif

#ifndef UByte
typedef uint8_t UByte;
#endif

#ifndef SInt8
typedef int8_t SInt8;
#endif

#ifndef SByte
typedef int8_t SByte;
#endif

#ifndef UInt16
typedef uint16_t UInt16;
#endif

#ifndef UWord
typedef uint16_t UWord;
#endif

#ifndef SInt16
typedef int16_t SInt16;
#endif

#ifndef SWord
typedef int16_t SWord;
#endif

#ifndef UInt32
typedef uint32_t UInt32;
#endif

#ifndef ULong
typedef uint32_t ULong;
#endif

#ifndef SInt32
typedef int32_t SInt32;
#endif

#ifndef SLong
typedef int32_t SLong;
#endif


#ifndef UInt64
typedef uint64_t UInt64;  
#endif

#ifndef SInt64
typedef int64_t SInt64;
#endif



/* ROOT !
typedef single Real32;
typedef double Real64;
typedef extended Real80;
*/


/* Pointeurs */

typedef char* TPChar;

typedef UInt8* TPUInt8;
typedef TPUInt8 TPUByte;

typedef SInt8* TPSInt8;
typedef TPSInt8 TPSByte;

typedef UInt16* TPUInt16;
typedef TPUInt16 TPUWord;

typedef SInt16* TPSInt16;
typedef TPSInt16 TPSWord;

typedef UInt32* TPUInt32;
typedef TPUInt32 TPULong;

typedef SInt32* TPSInt32;
typedef TPSInt32 TPSLong;

/* ROOT !
typedef Real32* TPReal32;
typedef Real64* TPReal64;
typedef Real80* TPReal80;
*/


typedef UInt16 TW128As8W16[8]; // Endianness is function of CC_TW128_LITTLE_ENDIAN

//
// If CC_TW128_LITTLE_ENDIAN IS defined
// => TW128As8W16, DECOM__TW128As8W16 are little endian
// => [0] = LS W16, [7] = MS W16
// => W128 = 0000 1111 2222 3333 4444 5555 6666 7777
// = > [7] = 0000, [6] = 1111, [5] = 2222, [4] = 3333, [3] = 4444, [2] = 5555, [1] = 6666, [0] = 7777
//
// If CC_TW128_LITTLE_ENDIAN IS NOT defined
// => TW128As8W16, DECOM__TW128As8W16 are big endian
// => [0] = MS W16, [7] = LS W16
// => W128 = 0000 1111 2222 3333 4444 5555 6666 7777
// = > [0] = 0000, [1] = 1111, [2] = 2222, [3] = 3333, [4] = 4444, [5] = 5555, [6] = 6666, [7] = 7777




#endif
