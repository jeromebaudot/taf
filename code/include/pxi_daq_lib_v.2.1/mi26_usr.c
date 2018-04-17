
/*******************************************************************************
File      : x:\lib\com\maps\mi26\mi26_usr.c
Goal      : Functions of Mi26 library.
          : It provides Mi26 types definition and data handling functions.
Prj date  : 05/08/2010
File date : 205/08/2010
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


#ifndef MI26_USR_C
#define MI26_USR_C


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
Date      : //2004
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


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
Date      : 24/11/2008
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */                          

SInt32 MI26__FBegin ( SInt8 FileErrLogLvl, char* FileErrFile ) {

  MI26__VGContext.FileErrLogLvl = FileErrLogLvl;
  strcpy ( MI26__VGContext.FileErrFile, FileErrFile );
   
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
Level     :
Date      : 24/11/2008
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 MI26__FEnd () {
  
  
  err_retok (( ERR_OUT, "" ));
}




#endif


