
/*******************************************************************************
File      : X:\prj\win\Beast\supervisor\lib\sup_exp\sup_exp.typ
Goal      : Types definition of supervisor sw export library
Prj date  : 06/09/2017
File date : 17/01/2018
Doc date  : //200
Author    : Gilles CLAUS
E-mail    : gilles.claus@iphc.cnrs.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef SEXP_TYP
#define SEXP_TYP

#define CC_UNIX

#ifndef GLB_CMT_SZ
#define GLB_CMT_SZ 256
#endif

#define fTimeRefRecordSize 540

/* ======================================================================== */
/*  If NOT compiled in DAQ src architecture => Standard types redefinition  */
/* ======================================================================== */


  #ifndef CC__NOT_EXPORT_SW
  
    #ifndef UInt8
      typedef unsigned char UInt8;
    #endif
    
    #ifndef UByte
      typedef UInt8 UByte;
    #endif
    
    #ifndef SInt8
      typedef char SInt8;
    #endif
    
    #ifndef SByte
      typedef SInt8 SByte;
    #endif
    
    #ifndef UInt16
      typedef unsigned 	short UInt16;
    #endif
    
    #ifndef UWord
      typedef UInt16 UWord;
    #endif
    
    #ifndef SInt16
      typedef short SInt16;
    #endif
    
    #ifndef SWord
      typedef SInt16 SWord;
    #endif
    
    #ifndef UInt32
      //typedef unsigned long int UInt32;
      typedef unsigned int UInt32;
    #endif
    
    #ifndef ULong
      typedef UInt32 ULong;
    #endif
    
    #ifndef SInt32
      //typedef long int SInt32;
      typedef int SInt32;
    #endif
    
    #ifndef SLong
      typedef SInt32 SLong;
    #endif
    
    
    #ifdef CC_UNIX
    
      #ifndef UInt64
        //typedef int64_t UInt64;
      #endif

      #ifndef SInt64
        //typedef  uint64_t SInt64;
        //typedef ULong64_t SInt64;
      #endif

    #else
      
      #ifndef UInt64
        typedef unsigned __int64 UInt64;
      #endif
      
      #ifndef SInt64
        typedef __int64 SInt64;
      #endif
    
    #endif
    
  #endif


/* ================= */
/*  Type example     */
/* ================= */

typedef struct {
  char  name[20];
  SInt8 age;
} SEXP__TMyStruct;


/* ============== */
/* Latchup types  */
/* ============== */

// 17/01/2018

typedef struct {

  UInt8 Day;
  UInt8 Month;
  UInt8 Year;
  UInt8 Reserved;
  
} SEXP__TDate;


// 17/01/2018

typedef struct {
  
  UInt8 Ms;
  UInt8 Sec;
  UInt8 Min;
  UInt8 Hour;
  
} SEXP__TTime;


// 17/01/2018

typedef struct {
  
  union {
  
    UInt32 DateAsInt32;
    SEXP__TDate Date;    

  }; 

  union {
    
    UInt32 TimeAsInt32;
    SEXP__TTime Time;
    
  };
  

} SEXP__TDateTime;


// 17/01/2018

typedef struct {
  
  UInt32 FormatVersionDate;         // Date of the record format
                                    // B31B25 = Version
                                    // B24B16 = Year
                                    // B15B08 = Month
                                    // B07B00 = Day
  
  SInt16 RecCnt;                    // Record counter = Time stamp counter, starting at 1
  
  SInt16 ReStartCnt;                // Ladder (Mi26) restart counter, must be equal to RecCnt
                                    // used to detect errors in case ReStartCnt <> RecCnt
  
                                    // Cyclic time stamping flag
                                    //
  SInt16 InfoCyclicTS;              // = 0 on for first TS = the one taken when Mi26 is started at beginning of the run
                                    // = 1 on following TS if automatic time stamping is enabled during the run
                                    // = 0 on a TS which is not thje first one => This TS correspond to a Mi26 restart after a latchup
  
  UInt8 RtcTimeAvailable;           // 0 = RTC time is NOT available in RtcDateTime field, 1 = RTC time is available
  
  UInt8 NtpTimeAvailable;           // 0 = NTP time is NOT available in NtpDateTime field, 1 = NTP time is available
  
  SEXP__TDateTime RtcDateTime;      // RTC date and time record 

  SEXP__TDateTime NtpDateTime;      // NTP date and time record
  
  char RtcDateTimeStr[GLB_CMT_SZ];  // RTC date and time converted in a string
  
  char NtpDateTimeStr[GLB_CMT_SZ];  // NTP date and time converted in a string
  
} SEXP_TTsRec;


#endif
