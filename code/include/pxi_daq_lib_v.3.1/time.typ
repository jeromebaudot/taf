
/*******************************************************************************
File      : x:\lib\com\time\time.typ
Goal      : Types definition of time lib.
Prj date  : 21/02/2010
File date : 21/02/2010
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


#ifndef TIME_TYP
#define TIME_TYP


/* =========================== */
/* Date S => Short date format */
/* Eg : 01/01/10               */
/* =========================== */

typedef struct {
  
  // Fields order MUST be Year, Month, Day -> Don't modify it
  // Because with this order date is in French format jj/mm/aa in W32
  // B31B24 B23B16 B15B08 B07B00
  // 00     Day    Month  Year
  
  SInt8  Year;
  SInt8  Month;
  SInt8  Day;
  SInt8  NotUsed;
  
  // Signed values are used rather than unsigned because they allow to
  // tag dates by setting negatives values in fields
  
} TIME__TSDateS;


typedef union {
  
  UInt32 W32;
  
  TIME__TSDateS Date;
  
} TIME__TUDateS;



/* ========================== */
/* Date L => Long date format */
/* Eg : 01/01/2010            */
/* ========================== */


typedef struct {
    
  // Fields order MUST be Year, Month, Day -> Don't modify it
  // Because with this order date is in French format jj/mm/aaaa in W32
  // B31B24 B23B16 B15B00
  // Day    Month  Year

  SInt16 Year;
  SInt8  Month;
  SInt8  Day;
  
  // Signed values are used rather than unsigned because they allow to
  // tag dates by setting negatives values in fields
  
} TIME__TSDateL;


typedef union {
    
  UInt32 W32;
  
  TIME__TSDateL Date;
  
} TIME__TUDateL;



/* ============== */
/*  */
/* ============== */


typedef struct {
  
  TIME__TUDateL ADates[7];
  
} TIME__TListDatesLOfWeek;


/* =============================== */
/* Time : Hour - Min - Sec - 1/100 */
/* Eg : 21/05/2010                 */
/* =============================== */


typedef struct {
  
  // Fields order MUST be Hour - Min - Sec - 1/100 -> Don't modify it
  // B31B24 B23B16 B15B08 B07B00
  // Hour   Min    Sec    Cent

  SInt8  Cent;
  SInt8  Sec;
  SInt8  Min;  
  SInt8  Hour;
  
  // Signed values are used rather than unsigned because they allow to
  // tag dates by setting negatives values in fields
  
} TIME__TSTime;


typedef union {
  
  UInt32 W32;
  
  TIME__TSTime Time;
  
} TIME__TUTime;



/* ============== */
/*  */
/* ============== */


#endif
