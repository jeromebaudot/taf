/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/u/u.typ
Goal      : Types definition of
Prj date  : 12/01/2004
File date : //2004
Doc date  : //2002
Rev       : 30/12/2005
          : - stb lib for DLL building
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/

#ifndef PPORT_TYP
#define PPORT_TYP


/* ================= */
/*  Type example     */
/* ================= */

typedef union {
  
  UInt8 w8;
  
  struct {
    UInt8 b0 : 1;
    UInt8 b1 : 1;
    UInt8 b2 : 1;
    UInt8 b3 : 1;
    UInt8 b4 : 1;
    UInt8 b5 : 1;
    UInt8 b6 : 1;
    UInt8 b7 : 1;
  } b;
  
} PPO_UInt8;

typedef union {
  
  UInt8 w8;
  
  struct {
    UInt8 b0 : 1;
    UInt8 b1 : 1;
    UInt8 b2 : 1;
    UInt8 b3 : 1;
    UInt8 b4 : 1;
    UInt8 b5 : 1;
    UInt8 b6 : 1;
    UInt8 b7 : 1;
  } b;
  
} PPO_Data;

typedef union {
  
  UInt8 w8;
  
  struct {
    UInt8 StrobeB   : 1; /* B0 */ 
    UInt8 AutoFeedB : 1; /* B1*/
    UInt8 Init      : 1; /* B2 */
    UInt8 SelectB   : 1; /* B3 */
    UInt8 b4        : 1; /* B4 */
    UInt8 b5        : 1; /* B5 */
    UInt8 b6        : 1; /* B6 */
    UInt8 b7        : 1; /* B7 */
  } b;
  
} PPO_Ctrl;

#ifdef  CC_APP_SPI_INOUTWRAPPER
     typedef short _stdcall (*inpfuncPtr)(short portaddr);
     typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);
#endif


/* ============== */
/*  */
/* ============== */


#endif