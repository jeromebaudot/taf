                                  
/*******************************************************************************
File      : x:\lib\com\time\time.c
Goal      : Functions of  time lib.
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


#ifndef TIME_C
#define TIME_C


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
Date      : 28/02/2010
Doc date  : 28/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

TIME__TUDateL TIME_FConvDateS2DateL ( TIME__TUDateS DateS ) {
  
  TIME__TUDateL VDateL;
  
  VDateL.Date.Day   = DateS.Date.Day;
  VDateL.Date.Month = DateS.Date.Month;
  VDateL.Date.Year  = 2000 + DateS.Date.Year;
  
  return (VDateL);
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
Date      : 28/02/2010
Doc date  : 28/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

TIME__TUDateS TIME_FConvDateL2DateS ( TIME__TUDateL DateL ) {
  
  TIME__TUDateS VDateS;
  
  VDateS.Date.Day   = DateL.Date.Day;
  VDateS.Date.Month = DateL.Date.Month;
  VDateS.Date.Year  = DateL.Date.Year % 2000;
  
  return (VDateS);
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
Remark    : Optional return parameter PtDestStr, not used if NULL or DestStrSz < TIME__STR_DATE_SZ
:
WARNING   : If return pointer to string is used, a copy OF STRING ( not pointer ) must be done
: after each TIME__FDateL2Str () call otherwise the string content will be the one of
: last call because all pointers will point to last string stored in local variable
: of function.
:
Level     :
Date      : 23/05/2010
Doc date  : 23/05/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

char* TIME__FDateS2Str ( TIME__TUDateS Date, char* PtDestStr, SInt32 DestStrSz ) {
  
  static char VStrDate[TIME__STR_DATE_SZ];
  
  sprintf ( VStrDate, "%.2d/%.2d/%.2d", Date.Date.Day, Date.Date.Month, Date.Date.Year );
  
  if ( (PtDestStr != NULL) && (DestStrSz >= TIME__STR_DATE_SZ) ) {
    sprintf ( PtDestStr, "%s", VStrDate );
  }
  
  return (VStrDate);
}

// 23/05/2010

char* TIME__FDateS2Str ( UInt32 Date, char* PtDestStr, SInt32 DestStrSz ) {
  
  TIME__TUDateS VDate;
  
  VDate.W32 = Date;
  
  return ( TIME__FDateS2Str ( VDate, PtDestStr, DestStrSz ) );
}

// 23/05/2010

char* TIME__FDateS2Str ( SInt32 Date, char* PtDestStr, SInt32 DestStrSz ) {
  
  TIME__TUDateS VDate;
  
  VDate.W32 = (UInt32) Date;
  
  return ( TIME__FDateS2Str ( VDate, PtDestStr, DestStrSz ) );
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
Remark    : Optional return parameter PtDestStr, not used if NULL or DestStrSz < TIME__STR_DATE_SZ
          :
WARNING   : If return pointer to string is used, a copy OF STRING ( not pointer ) must be done
          : after each TIME__FDateL2Str () call otherwise the string content will be the one of
          : last call because all pointers will point to last string stored in local variable
          : of function.
          :
Level     :
Date      : 21/02/2010
Doc date  : 21/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

char* TIME__FDateL2Str ( TIME__TUDateL Date, char* PtDestStr, SInt32 DestStrSz ) {

  static char VStrDate[TIME__STR_DATE_SZ];
  
  sprintf ( VStrDate, "%.2d/%.2d/%.4d", Date.Date.Day, Date.Date.Month, Date.Date.Year );
  
  if ( (PtDestStr != NULL) && (DestStrSz >= TIME__STR_DATE_SZ) ) {
    sprintf ( PtDestStr, "%s", VStrDate );
  }

  return (VStrDate);
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
Remark    : Optional return parameter PtDestStr, not used if NULL or DestStrSz < TIME__STR_DATE_SZ
:
WARNING   : If return pointer to string is used, a copy OF STRING ( not pointer ) must be done
: after each TIME__FDateL2Str () call otherwise the string content will be the one of
: last call because all pointers will point to last string stored in local variable
: of function.
:
Level     :
Date      : 06/03/2010
Doc date  : 06/03/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

char* TIME__FDateL2StrExt ( TIME__TUDateL Date, char SepChar, char* PtDestStr, SInt32 DestStrSz ) {
  
  static char VStrDate[TIME__STR_DATE_SZ];
  
  sprintf ( VStrDate, "%.2d%c%.2d%c%.4d", Date.Date.Day, SepChar, Date.Date.Month, SepChar, Date.Date.Year );
  
  if ( (PtDestStr != NULL) && (DestStrSz >= TIME__STR_DATE_SZ) ) {
    sprintf ( PtDestStr, "%s", VStrDate );
  }
  
  return (VStrDate);
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
Date      : 22/02/2010
Doc date  : 22/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef ROOT_ROOT

TIME__TUDateL TIME__FConvDateTime2DateL ( TDateTime Src ) {
  
         TIME__TUDateL   VDate;
         unsigned short  VYear;
         unsigned short  VMonth;
         unsigned short  VDay;

  Src.DecodeDate (  &VYear,  &VMonth,  &VDay );

  VDate.Date.Year  = VYear;
  VDate.Date.Month = VMonth;
  VDate.Date.Day   = VDay;

  return (VDate);
}

#endif

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
Date      : 22/02/2010
Doc date  : 22/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef ROOT_ROOT

TIME__TUDateL TIME__FGetDateLOfNextDay ( TIME__TUDateL Cur ) {
  
          TDateTime       VCurDate ( Cur.Date.Year, Cur.Date.Month, Cur.Date.Day );
          TIME__TUDateL    VNextDate;
          unsigned short  VYear;
          unsigned short  VMonth;
          unsigned short  VDay;

  ++VCurDate;
  
  VCurDate.DecodeDate (  &VYear,  &VMonth,  &VDay );

  VNextDate.Date.Year  = VYear;
  VNextDate.Date.Month = VMonth;
  VNextDate.Date.Day   = VDay;

  return (VNextDate);
}

#endif

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
WARNING   : If return pointer to list is used, a copy of LIST ( not pointer ) must be done
          : after each TIME__FFillListDatesLOfWeek () call otherwise the list content will be
          : the one of last call because all pointers will point to last list stored in local
          : variable of function.
          :
          :
Level     : 
Date      : 23/02/2010
Doc date  : 23/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifndef ROOT_ROOT

TIME__TListDatesLOfWeek* TIME__FFillListDatesLOfWeek ( TIME__TUDateL MondayDate, TIME__TListDatesLOfWeek* PtList ) {

  static TIME__TListDatesLOfWeek VList;
         TIME__TUDateL           VDayDate;
         SInt8                  ViDay;   
  
  
  VDayDate = MondayDate;
  
  for ( ViDay=0; ViDay < 7; ViDay++ ) {
    VList.ADates[ViDay] = VDayDate;
    VDayDate = TIME__FGetDateLOfNextDay ( VDayDate );
  }
  
  if ( PtList != NULL ) {
    *PtList = VList;
  }
  
  return (&VList);
}

#endif

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
Date      : 23/02/2010
Doc date  : 23/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 TIME__FPrintListDatesLOfWeek ( TIME__TListDatesLOfWeek* PtList ) {
  
  SInt8                  ViDay;
  
  
  err_retnull ( PtList, (ERR_OUT,"PtList == NULL") );
  
    
  for ( ViDay=0; ViDay < 7; ViDay++ ) {
    msg (( MSG_OUT, "Date : %s", TIME__FDateL2Str ( PtList->ADates[ViDay], NULL /* PtDestStr */, 0 /* DestStrSz */ )  ));
  }
  
  
  err_retok (( ERR_OUT, "" ));
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
WARNING   : A copy of return STRING ( not pointer ) must be done after each TIME_FGetDayName call
          :  otherwise the string content will be the one of last call because all pointers will
          : point to last string stored in local variable of function.
:
Level     :
Date      : 27/02/2010
Doc date  : 27/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

char* TIME_FGetDayName ( UInt8 DayIndex, SInt8 Language ) {
  
  static char* VAStrDayNameFr[8] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche" };
  static char* VAStrDayNameEn[8] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
  
  if ( (DayIndex < 0) || (DayIndex > 7) ) {
    err_error (( ERR_OUT, "Bad day index = %d out of rnage 0..7", DayIndex ));
    return ( "?" );
  }
  
  switch ( Language ) {
    
    case TIME__LANG_FRENCH : {
      return ( VAStrDayNameFr[DayIndex] );
      break; }
    
    case TIME__LANG_ENGLISH : {
      return ( VAStrDayNameEn[DayIndex] );
      break; }
    

    default : {
      err_error (( ERR_OUT, "Bad language = %d out of range", Language ));
      return ( "?");
    }
    
  }
  
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
Date      : 21/05/2010
Doc date  : 21/05/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef ROOT_ROOT

TIME__TUTime TIME__FConvDateTime2Time ( TDateTime Src ) {
  
  TIME__TUTime    VTime;
  unsigned short  VHour;
  unsigned short  VMin;
  unsigned short  VSec;
  unsigned short  VMSec;
  
  Src.DecodeTime (  &VHour, &VMin,  &VSec, &VMSec );
  
  VTime.Time.Hour  = VHour;
  VTime.Time.Min   = VMin;
  VTime.Time.Sec   = VSec;
  VTime.Time.Cent  = VMSec / 10;
  
  return (VTime);
}

#endif



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
Remark    : Optional return parameter PtDestStr, not used if NULL or DestStrSz < TIME__STR_DATE_SZ
:
WARNING   : If return pointer to string is used, a copy OF STRING ( not pointer ) must be done
          : after each TIME__FTime2Str () call otherwise the string content will be the one of
          : last call because all pointers will point to last string stored in local variable
          : of function.
          :
Level     :
Date      : 21/05/2010
Doc date  : 21/05/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

char* TIME__FTime2Str ( TIME__TUTime Time, char* PtDestStr, SInt32 DestStrSz ) {
  
  static char VStrTime[TIME__STR_TIME_SZ];
  
  sprintf ( VStrTime, "%.2d:%.2d:%.2d|%.2d", Time.Time.Hour, Time.Time.Min, Time.Time.Sec, Time.Time.Cent );
  
  if ( (PtDestStr != NULL) && (DestStrSz >= TIME__STR_TIME_SZ) ) {
    sprintf ( PtDestStr, "%s", VStrTime );
  }
  
  return (VStrTime);
}


// 23/05/2010

char* TIME__FTime2Str ( UInt32 Time, char* PtDestStr, SInt32 DestStrSz ) {

  TIME__TUTime VTime;
  
  VTime.W32 = Time;
  
  return ( TIME__FTime2Str ( VTime, PtDestStr, DestStrSz ) );
  
}

// 23/05/2010

char* TIME__FTime2Str ( SInt32 Time, char* PtDestStr, SInt32 DestStrSz ) {
  
  TIME__TUTime VTime;
  
  VTime.W32 = (UInt32) Time;
  
  return ( TIME__FTime2Str ( VTime, PtDestStr, DestStrSz ) );
  
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
Date      : 21/02/2010
Doc date  : 21/02/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */





#endif