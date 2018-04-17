
#ifndef FIL_C
#define FIL_C


#ifdef CC_UNIX

UInt32 GetTickCount () {
  err_warning (( ERR_OUT, "Not supported under Unix" ));
  return (0);
}


SInt32 GetLastError () {
  err_warning (( ERR_OUT, "Not supported under Unix" ));
  return (0);
}

#endif

/*******************************************************************************
Prototype : SInt32 FIL_FFileExists ( char* FilePath )
Goal      : Test if the file exists.
Inputs    : The file name ( full path if needed ).
Ouputs    : 1 if the file exists, else 0.
Globals   :
Remark    : The result is not guaranted in case of multi application lock to file.
Level     : This is a user level function.
Date      : 17/04/2003
Doc date  : 17/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FFileExists ( char* FilePath ) {
  
  SInt32 VRet;
  FILE*  VPf;
  
  VPf = fopen ( FilePath, "r" );
  
  if ( VPf == NULL ) {
    VRet = 0;
  }
  
  else {
    fclose ( VPf );
    VRet = 1;
  }
  
  
  return (VRet);
}

/* 13/10/2004 */

SInt32 FIL_FFileSize ( char* FilePath ) {
  
  FILE* VPf;
  SInt32 VFileSz;
  
  if ( ( VPf = fopen ( FilePath, "rb" ) ) == NULL ) {
    err_retfail ( -1, (ERR_OUT,_strerror ( "fopen fail !" ) ));
  }
  
  /* Calculate file size */
  
  if ( fseek ( VPf, 0, SEEK_SET ) != 0 ) {
    err_retfail ( -1, (ERR_OUT,_strerror ( "fseek SEEK_SET fail !" ) ));
  }
  
  if ( fseek ( VPf, 0, SEEK_END ) != 0 ) {
    err_retfail ( -1, (ERR_OUT,_strerror ( "fseek SEEK_END fail !" ) ));
  }
  
  if ( (VFileSz = ftell ( VPf )) == -1 )  {
    err_retfail ( -1, (ERR_OUT,_strerror ( "ftell fail !" ) ));
  }
  
  if ( fseek ( VPf, 0, SEEK_SET ) != 0 ) {
    err_retfail ( -1, (ERR_OUT,_strerror ( "fseek SEEK_SET fail !" ) ));
  }
  
  if ( fclose (VPf) != 0 ) {
    err_retfail ( -1, (ERR_OUT,_strerror ( "fclose fail !" ) ));
  }
  
  err_retval ( VFileSz, ( ERR_OUT, "%d Bytes", VFileSz ) );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 FIL_FCpyFile ( char* Src, char* Dest )
          :
Goal      : Copy SrcFile to DestFile.
          :
Inputs    : Src  - Source file
          : Dest - Destination file
          :
Ouputs    : The function returns
          :  = 0 ok
          :  < 0 in case of error
          :
Globals   : None
          :
Remark    : None
          :
Level     : This is a user level function.
Date      : 09/06/2005
Doc date  : 09/06/2005
Modif     : 18/06/2005
          : - fonction moved from WDAQ
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 FIL_FCpyFile ( char* Src, char* Dest ) {
  
  FILE*  VPfSrc;
  FILE*  VPfDest;
  SInt8  VBuffer[512]; // Debug 02/06/2007 move storage class static to auto
  SInt32 VReadBytesCnt;
  SInt32 VTotBytesCnt;
  SInt32 VRet;
  
  VPfSrc  = fopen ( Src, "rb" );
  
  err_retnull ( VPfSrc, (ERR_OUT,"Source file %d open failed", Src ) );
  
  VPfDest = fopen ( Dest, "wb" );
  err_retnull ( VPfDest, (ERR_OUT,"Destination file %s creation failed", VPfDest ) );
  
  VTotBytesCnt = 0;
  
  while ( ( VReadBytesCnt = fread ( VBuffer, 1 /* byte size */ , 512 /* bytes number */, VPfSrc ) ) > 0 ) {
    fwrite ( VBuffer, 1 /* byte size */, VReadBytesCnt, VPfDest );
    VTotBytesCnt += VReadBytesCnt;
  }
  
  fclose (VPfSrc);
  fclose (VPfDest);
  
  err_retok (( ERR_OUT, "file %s copied in %s => %d bytes", Src, Dest, VTotBytesCnt ));
}



/*******************************************************************************
Prototype : SInt32 FIL_FRemoveFile ( char* FilePath )
Goal      : 
Inputs    : 
Ouputs    : 
Globals   :
Remark    : 
Level     : This is a user level function.
Date      : 18/06/2005
Doc date  : 
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FRemoveFile ( char* FilePath ) {
  
  SInt32 VRet;
  char*  VStrError;

  VRet = remove ( FilePath );

  err_retfail ( VRet, (ERR_OUT,"Remove file=%s failed => %s", FilePath, _strerror ("")) );
  err_retok (( ERR_OUT, ""));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 FIL_FDirOutFile ( char* SrcDir, char* DestDirFile )
          :
Goal      : List the directory SrcDir and write the result in DestDirFile.
          :
Inputs    : SrcDir      - The directory to list
          : DestDirFile - The result file
          :
Ouputs    : The function returns
          :  = 0 ok
          :  < 0 in case of error
          :
Globals   : None
          :
Remark    : None
          :
Level     : This is a user level function.
Date      : 09/06/2005
Doc date  : 09/06/2005
Modif     : 18/06/2005
          : - Function moved from WDAQ
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifdef CC_APP_WINDOWS


SInt32 FIL_FDosDirOutFile ( char* SrcDir, char* DestDirFile ) {
  
  FILE*  VPf;
  struct ffblk VFileInfo; 
  char   VSrcDirPlusJoker[GLB_FILE_PATH_SZ];
  SInt32 VItemCnt;
  SInt32 VRet;
  
  VPf = fopen ( DestDirFile, "wt" );
  
  if ( VPf == NULL ) {
    err_retfail ( -1, (ERR_OUT, "Dir file %s creation failed ", DestDirFile ) );
  }
  
  sprintf ( VSrcDirPlusJoker, "%s\\*.*", SrcDir );
  
  VItemCnt = 0;
  VRet     = findfirst( VSrcDirPlusJoker, &VFileInfo, FA_NORMAL );
  
  while ( VRet == 0 ) {
    ++VItemCnt;
    fprintf( VPf, "%s\n", VFileInfo.ff_name);
    VRet = findnext(&VFileInfo);
  }
  
  fclose ( VPf );
  
  return (0);
}


#endif


SInt32 FIL_FDirOutFile ( char* SrcDir, char* DestDirFile ) {

#ifdef CC_APP_WINDOWS
  return ( FIL_FDosDirOutFile ( SrcDir, DestDirFile ) );
#else
  err_retfail ( -1, (ERR_OUT,"Function not handled under Linux or LynxOs") );
#endif

}



/*******************************************************************************
Prototype : SInt32 FIL_FDosDelDir ( char* SrcDir ) 
Goal      :
Inputs    :
Ouputs    : 
Globals   :
Remark    : Compiled only if CC_APP_WINDOWS is defined
Level     : This is a user level function.
Date      : 18/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/


#ifdef CC_APP_WINDOWS
  
  
SInt32 FIL_FDosRmDirFiles ( char* SrcDir ) {
  
  FILE*  VPf;
  struct ffblk VFileInfo; 
  char   VSrcDirPlusJoker[GLB_FILE_PATH_SZ];
  char   VFileToDelete[GLB_FILE_PATH_SZ];
  SInt32 VItemCnt;
  SInt32 VRet;
  SInt32 VRetRemove;
  SInt32 VFuncRet;
    
  sprintf ( VSrcDirPlusJoker, "%s\\*.*", SrcDir );
  
  VFuncRet    = 0;
  VItemCnt    = 0;
  VRet        = findfirst( VSrcDirPlusJoker, &VFileInfo, FA_NORMAL );
  
  while ( VRet == 0 ) {
    sprintf ( VFileToDelete, "%s\\%s", SrcDir, VFileInfo.ff_name );
    VRetRemove = FIL_FRemoveFile ( VFileToDelete );
    
    if ( VRetRemove < 0 ) {
      VFuncRet = -1;
      err_error (( ERR_OUT, "Remove file=%s failed => %s", VFileToDelete, _strerror ("") ));
    }
    
    ++VItemCnt;
    VRet = findnext(&VFileInfo);
  }
  
  
  return (VFuncRet);
}

#endif


/*******************************************************************************
Prototype : SInt32 FIL_FRmDirFiles ( char* SrcDir )
Goal      : Remove all the files in directory SrcDir but let the direcory
Inputs    :
Ouputs    :
Globals   :
Remark    : 
Level     : This is a user level function.
Date      : 18/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/



SInt32 FIL_FRmDirFiles ( char* SrcDir ) {
  
#ifdef CC_APP_WINDOWS
  return ( FIL_FDosRmDirFiles (SrcDir) );
  
#else
  err_retfail ( -1, (ERR_OUT,"FIL_FRmDirFiles (SrcDir=%s) IS NOT available !", SrcDir) );
#endif

  
}


/*******************************************************************************
Prototype : SInt32 FIL_FRmDir ( char* SrcDir )
Goal      : Remove the directory SrcDir if empty
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 19/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/



SInt32 FIL_FRmDir ( char* SrcDir ) {

  SInt32 VRet;

#ifdef CC_APP_WINDOWS
  
  // VRet = _rtl_chmod( SrcDir, 1 , FA_NORMAL  );
  // err_retfail ( VRet, (ERR_OUT,"Chmod failed on directory=%s", SrcDir) );
  
  VRet = rmdir ( SrcDir );  
  err_retfail ( VRet, (ERR_OUT,"Remove directory=%s failed => %s ", SrcDir, _strerror ("") ) );
  err_retok (( ERR_OUT, "" ));

#else
  err_retfail ( -1, (ERR_OUT,"Function handled only under Windows") );
#endif
    
}

/*******************************************************************************
Prototype :
Goal      : Delete direcory files and directory itself
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 19/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FDelDir ( char* SrcDir ) {
  
  SInt32 VRet;
  char VNewName[GLB_FILE_PATH_SZ]; // Debug 02/06/2007 move storage class static to auto

    
  sprintf ( VNewName, "%s.old", SrcDir );
  
  VRet = rename ( SrcDir, VNewName );
  
  err_retfail ( VRet, (ERR_OUT,"Rename Dir=%s to %s failed !", SrcDir, VNewName ) );

  err_warning (( ERR_OUT, "Directory %s renamed in %s because delete is impossible !", SrcDir, VNewName  ));
  
  err_retok (( ERR_OUT, "" ));
  
/*
  VRet = FIL_FRmDirFiles ( SrcDir );
  err_retfail ( VRet, (ERR_OUT,"FIL_FRmDirFiles (%s) failed ", SrcDir) );
  
  VRet = FIL_FRmDir ( VNewName );
  err_retfail ( VRet, (ERR_OUT,"FIL_FRmDir (%s) failed ", SrcDir) );

  err_retok (( ERR_OUT, "" ));
*/


}
  
/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 19/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FMkDir ( char* SrcDir ) {
  
  SInt32 VRet;
  char VStrCmd[GLB_FILE_PATH_SZ]; // Debug 02/06/2007 move storage class static to auto
  
  #ifdef APP_DAQ
  
    err_retfail ( -1, (ERR_OUT,"Function NOT available under LynxOS !") );
    
  #endif
  
  #ifdef CC_APP_LINUX
    sprintf ( VStrCmd, "mkdir %s", SrcDir );
    system  ( VStrCmd );
  #endif
  
  
  #ifdef CC_APP_WINDOWS
    VRet = mkdir ( SrcDir );
  
    err_retfail ( VRet, (ERR_OUT,"Creation of directory=%s failed => %s", SrcDir, _strerror ("") ) );
    err_retok (( ERR_OUT, "" ));

  #else
    err_retfail ( -1, (ERR_OUT,"FIL_FMkDir (SrcDir=%s) IS NOT available !", SrcDir) );
  #endif
  
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    : The file number
Globals   :
Remark    : If file number is > FIL_LIST_DIR_FILES_MAX_CNT, all the files in
          : the directory are counted, but files with index >= FIL_LIST_DIR_FILES_MAX_CNT
          : are not written in list.
Level     : This is a user level function.
Date      : 23/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

#ifdef CC_APP_WINDOWS

SInt32 FIl_FDosListDirFiles ( char* SrcDir, FIL_TDirFilesList* PtList ) {
  
  struct ffblk VFileInfo; 
  char  VSrcDirPlusJoker[GLB_FILE_PATH_SZ]; // Debug 02/06/2007 move storage class static to auto
  SInt32 VItemCnt;
  SInt32 VRet;

  err_retnull ( PtList, (ERR_OUT,"PtList == NULL") ); /* GC 29/09/05 */
  
  sprintf ( VSrcDirPlusJoker, "%s\\*.*", SrcDir ); /* !!! */
  
  VItemCnt = 0;
  VRet     = findfirst( VSrcDirPlusJoker, &VFileInfo, FA_NORMAL );

  while ( VRet == 0 ) {
    
    if ( VItemCnt < FIL_DIR_FILES_LIST_MAX_CNT ) {
      sprintf ( PtList->AList[VItemCnt], "%s", VFileInfo.ff_name );
    }
    
    ++VItemCnt;
    VRet = findnext(&VFileInfo);
  }

  return ( VItemCnt );
}


#endif


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    : The file number
Globals   :
Remark    : If file number is > FIL_LIST_DIR_FILES_MAX_CNT, all the files in
: the directory are counted, but files with index >= FIL_LIST_DIR_FILES_MAX_CNT
: are not written in list.
Level     : This is a user level function.
Date      : 27/05/2006
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

#ifdef CC_APP_WINDOWS

SInt32 FIL_FExtDosListDirFiles ( char* SrcDir, char* Joker, FIL_TDirFilesList* PtList ) {
  
  struct ffblk VFileInfo;
  char  VSrcDirPlusJoker[GLB_FILE_PATH_SZ]; // Debug 02/06/2007 move storage class static to auto
  SInt32 VItemCnt;
  SInt32 VRet;
  
  err_retnull ( PtList, (ERR_OUT,"PtList == NULL") ); /* GC 29/09/05 */
  
  sprintf ( VSrcDirPlusJoker, "%s\\%s", SrcDir, Joker ); /* !!! */
  
  VItemCnt = 0;
  VRet     = findfirst( VSrcDirPlusJoker, &VFileInfo, FA_NORMAL );
  
  while ( VRet == 0 ) {
    
    if ( VItemCnt < FIL_DIR_FILES_LIST_MAX_CNT ) {
      sprintf ( PtList->AList[VItemCnt], "%s", VFileInfo.ff_name );
    }
    
    ++VItemCnt;
    VRet = findnext(&VFileInfo);
  }
  
  return ( VItemCnt );
}


#endif


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 23/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIl_FListDirFiles ( char* SrcDir, FIL_TDirFilesList* PtList ) {

  err_retnull ( PtList, (ERR_OUT,"PtList == NULL") );
  
#ifdef CC_APP_WINDOWS
  return ( FIl_FDosListDirFiles ( SrcDir, PtList ) );
#else
  err_retfail ( -1, (ERR_OUT,"Function not handled under Linux or LynxOs") );
#endif
  
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 27/05/2006
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FExtListDirFiles ( char* SrcDir, char* Joker, FIL_TDirFilesList* PtList ) {
  
  err_retnull ( PtList, (ERR_OUT,"PtList == NULL") );
  
#ifdef CC_APP_WINDOWS
  return ( FIL_FExtDosListDirFiles ( SrcDir, Joker, PtList ) );
#else
  err_retfail ( -1, (ERR_OUT,"Function not handled under Linux or LynxOs") );
#endif
  
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 23/06/2005
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FCntRmpFiles ( char* RmpDataPath, FIL_TDirFilesList* PtList ) {
  
  SInt32 VRet;
  SInt32 VFilesCnt;
  char VRmpDirPath[GLB_FILE_PATH_SZ];

  err_retnull ( PtList, (ERR_OUT,"PtList == NULL") );
  
  sprintf ( VRmpDirPath, "%s", RmpDataPath );
  VRmpDirPath[strlen (VRmpDirPath) - 3] = 0; /* To remove the "RUN" word from path */
  
  VFilesCnt = FIl_FListDirFiles ( VRmpDirPath, PtList );
    
  return ( VFilesCnt );
}



#ifndef APP_DAQ

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 18/06/2005
Rev       : 30/07/2006
          : - Extended version which handles conf extension i,j,k
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FExtDelRmpFiles ( char* RmpDataPath, char ConfExt, SInt32 RunNo, SInt32 MaxFileCnt ) {
  
  SInt32 VRet;
  SInt32 ViFile;
  SInt32 VFilesCnt;
  char VRmpDirPath[GLB_FILE_PATH_SZ];     // Debug 02/06/2007 move storage class static to auto
  char VInfoFilePath[GLB_FILE_PATH_SZ];   // Debug 02/06/2007 move storage class static to auto
  char VData0Filepath[GLB_FILE_PATH_SZ];  // Debug 02/06/2007 move storage class static to auto
  FIL_TDirFilesList VFilesList;           // Debug 02/06/2007 move storage class static to auto
  

  char   VFileToDelete[GLB_FILE_PATH_SZ];
    
  
  sprintf ( VRmpDirPath, "%s", RmpDataPath );
  VRmpDirPath[strlen (VRmpDirPath) - 3] = 0; /* To remove the "RUN" word from path */
  
  sprintf ( VInfoFilePath , "RUN_%d_%c.rz",  RunNo, ConfExt );
  sprintf ( VData0Filepath, "RUN_%d_0.rz" ,  RunNo );
       
  VFilesCnt = FIL_FCntRmpFiles ( RmpDataPath, &VFilesList );
  
  /* !!! DESY 20/09/05 !!! MUST BE CHECKED ! */
  /* previous line replaced by this one in order to be able to compile boards_db_man */
  /* VFilesCnt = FIL_FCntRmpFiles ( RmpDataPath, &(VPtAFilesPath) );  */
    
  err_retfail ( VFilesCnt, (ERR_OUT,"FIL_FCntRmpFiles (%s) failed Ret=%d", RmpDataPath, VFilesCnt ) );    
  
  if ( VFilesCnt < MaxFileCnt ) {
    err_retok (( ERR_OUT, "Nothing to do VFilesCnt=%d < MaxFileCnt=%d", VFilesCnt, MaxFileCnt ));
  }
  
  err_warning (( ERR_OUT, "%d RMP files are not deleted by monitoring => I will remove them", VFilesCnt ));
  
  for ( ViFile = 0; ViFile < VFilesCnt; ViFile++ ) {

    if ( strcmp ( VFilesList.AList[ViFile], VInfoFilePath ) == 0 ) {
      continue;
    }

    if ( strcmp ( VFilesList.AList[ViFile], VData0Filepath ) == 0 ) {
      continue;
    }
    
    sprintf ( VFileToDelete, "%s\\%s", VRmpDirPath, VFilesList.AList[ViFile] );
    
    err_warning (( ERR_OUT, "FIL_FDelRmpFilesv => File = %s", VFileToDelete ));
    
    FIL_FRemoveFile ( VFileToDelete );
    
  }

  return (0);
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 18/06/2005
Rev       : 30/07/2006
          : - Creation of FIL_FExtDelRmpFiles to replace FIL_FDelRmpFiles
          : - FIL_FDelRmpFiles call FIL_FExtDelRmpFiles ( RmpDataPath, 'i', RunNo, MaxFileCnt )
Doc date  :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 FIL_FDelRmpFiles ( char* RmpDataPath, SInt32 RunNo, SInt32 MaxFileCnt ) {
  return ( FIL_FExtDelRmpFiles ( RmpDataPath, 'i', RunNo, MaxFileCnt ) );
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
          :
Ouputs    : The function returns
          :  = 0 ok
          :  < 0 in case of error
          :
Globals   : None
          :
Remark    : None
          :
Level     : This is a user level function.
Date      : 24/02/2007
Doc date  : 
Modif     : 
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 FIL_FFloatVectorsToTextFile ( float** SrcVectors, SInt16 VectorsNb, SInt32 EltNbPerVector, char* HeaderLine, char** VectorsNames, char DataSep, char* DestFile ) {

  char VFuncName[] = "FIL_FFloatVectorsToTextFile";
  
  FILE*  VPfDest;
  SInt32 ViVector;
  SInt32 ViLine;
  SInt32 VRet;
  
  
  VPfDest = fopen ( DestFile, "wt" );
  err_retnull ( VPfDest, (ERR_OUT,"Destination file %s creation failed", DestFile ) );
  

  if ( HeaderLine != NULL ) {
    VRet = fprintf ( VPfDest, "%s \n", HeaderLine );
  }
  
  else {
    VRet = fprintf ( VPfDest, "No header line \n", HeaderLine );
  }

  if ( VRet == EOF ) {
    fflush (VPfDest);
    fclose (VPfDest);
    err_retfail ( -1, (ERR_OUT,"Error writing Header line file=%s => %s", DestFile, _strerror ( "System says :" ) ) );
  }
    

  if ( VectorsNames != NULL ) {
    
    for ( ViVector=0; ViVector < VectorsNb; ViVector++ ) {
      VRet = fprintf ( VPfDest, "%-21s", VectorsNames[ViVector]);
      
      if ( VRet == EOF ) {
        fflush (VPfDest);
        fclose (VPfDest);
        err_retfail ( -1, (ERR_OUT,"Error writing vectors names line - Vector=%d file=%s => %s", ViVector, DestFile, _strerror ( "System says :" ) ) );
      }    
      
    }
  
      
    VRet = fprintf ( VPfDest, "\n" );
  
    if ( VRet == EOF ) {
      fflush (VPfDest);
      fclose (VPfDest);
      err_retfail ( -1, (ERR_OUT,"Error writing CR file=%s => %s", DestFile, _strerror ( "System says :" ) ) );
    }
    
  }
  
  else {
    fprintf ( VPfDest, "No vectors names line \n", HeaderLine );    
  }


  for ( ViLine=0; ViLine < EltNbPerVector; ViLine++ ) {
    
    for ( ViVector=0; ViVector < VectorsNb; ViVector++ ) {
            
      VRet = fprintf ( VPfDest, "%.6f%c", SrcVectors[ViVector][ViLine], DataSep);
      
      if ( VRet == EOF ) {
        fflush (VPfDest);
        fclose (VPfDest);
        err_retfail ( -1, (ERR_OUT,"Error writing line=%d in file=%s => %s", ViLine, DestFile, _strerror ( "System says :" ) ) );
      }
      
    }
  
    fprintf ( VPfDest, "\n" );

    if ( VRet == EOF ) {
      fflush (VPfDest);
      fclose (VPfDest);
      err_retfail ( -1, (ERR_OUT,"Error writing CR line=%d file=%s => %s", ViLine, DestFile, _strerror ( "System says :" ) ) );
    }
    
  }

  
  fflush (VPfDest);
  fclose (VPfDest);
  
  err_retok (( ERR_OUT, "%d line written in file", EltNbPerVector, DestFile ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
:
Ouputs    : The function returns
:  = 0 ok
:  < 0 in case of error
:
Globals   : None
:
Remark    : None
:
Level     : This is a user level function.
Date      : 24/02/2007
Doc date  :
Modif     :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#undef CC_FIL

#ifdef CC_FIL

SInt32 FIL_FTextFileToFloatVectors ( char* SrcFile, char DataSep, float** DestVectors, SInt16 VectorsNb, SInt32 EltNbPerVector, char* HeaderLine, char** AVectorsNames, char* StrVectorsNames  ) {
  
  char VFuncName[] = "FIL_FTextFileToFloatVectors";
  
  FILE*  VPf;
  SInt32 ViVector;
  SInt32 ViLine;
  SInt32 VRet;
  char   VStrCR[2];
  char   VDataSep;
  
  /* Check parameters */
  
  err_retnull ( SrcFile          , (ERR_OUT,"SrcFile          == NULL" ) );
  err_retnull ( DestVectors      , (ERR_OUT,"DestVectors      == NULL" ) );
  err_retnull ( HeaderLine       , (ERR_OUT,"HeaderLine       == NULL" ) );
  err_retnull ( AVectorsNames    , (ERR_OUT,"AVectorsNames    == NULL" ) );
  err_retnull ( StrVectorsNames  , (ERR_OUT,"StrVectorsNames  == NULL" ) );
  
  err_trace (( ERR_OUT, "FIL_FTextFileToFloatVectors - 1" ));
  
  /* Try to open file */

  VPf = fopen ( SrcFile, "rt" );
  err_retnull ( VPf, (ERR_OUT,"Open source file %s failed", SrcFile ) );

  err_trace (( ERR_OUT, "FIL_FTextFileToFloatVectors - 2" ));
  
  /* Read header line */

  VRet = (SInt32) fgets( HeaderLine, 255, VPf );

  
  if ( VRet == 0 ) {
    fclose (VPf);
    err_retfail ( -1, (ERR_OUT,"Read header line failed file=%s => %s ", SrcFile, _strerror ( "System says :" ) ) );
  }
  
  /* Read vectors names line */


  VRet = (SInt32) fgets( StrVectorsNames, 255, VPf );

  if ( VRet == 0 ) {
    fclose (VPf);
    err_retfail ( -1, (ERR_OUT,"Read vectors names line failed file=%s => %s ", SrcFile, _strerror ( "System says :" ) ) );
  }

  for ( ViVector=0; ViVector < VectorsNb; ViVector++ ) {
    sprintf ( AVectorsNames[ViVector], "" );
  };

  
  /* Read vectors */
    
  for ( ViLine=0; ViLine < EltNbPerVector; ViLine++ ) {
    
    for ( ViVector=0; ViVector < VectorsNb; ViVector++ ) {

      if ( ViVector == VectorsNb-1 ) {
        VRet = fscanf ( VPf, "%f%c", &DestVectors[ViVector][ViLine], &VDataSep );
      }
      
      else {
        VRet = fscanf ( VPf, "%f%c\n", &DestVectors[ViVector][ViLine], &VDataSep );        
      }


      if ( VRet != 2 ) {
        fflush (VPf);
        fclose (VPf);
        err_retfail ( -1, (ERR_OUT,"Error reading line=%d in file=%s VRet=%d => %s", ViLine, SrcFile, VRet, _strerror ( "System says :" ) ) );
      }
      
    }
        
  }
  
  
  fflush (VPf);
  fclose (VPf);
  
  err_retok (( ERR_OUT, "%d line Read in file", EltNbPerVector, SrcFile ));
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
:
Ouputs    : The function returns
:  = 0 ok
:  < 0 in case of error
:
Globals   : None
:
Remark    : None
:
Level     : This is a user level function.
Date      : 24/02/2007
Doc date  :
Modif     :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifdef CC_APP_WINDOWS
  
  SInt32 FIL_FGetAppRunDir ( char* StrRunDir, TApplication* PtApp  ) {
  
    char*      VPtStrExeName;
    AnsiString VAStrExeName;
    char       VExeDriveLetter;
    char       VExeDriveId; /* 0 = default, 1 = A, 2 = B ... */
    char       VStrCurDir[GLB_FILE_PATH_SZ];
    char       VStrCurDirPath[GLB_FILE_PATH_SZ];
    
    err_retfailnull ( StrRunDir, (ERR_OUT,"StrRunDir == NULL") );
    
    VPtStrExeName = FStr2PCh ( &(PtApp->ExeName) );
  
    VExeDriveLetter = toupper ( VPtStrExeName[0] );
    
    switch ( VExeDriveLetter ) {
      
      case 'A' : {
        VExeDriveId = 1;
        break; }
      
      case 'B' : {
        VExeDriveId = 2;
        break; }

      case 'C' : {
        VExeDriveId = 3;
        break; }

      case 'D' : {
        VExeDriveId = 4;
        break; }
    
      case 'E' : {
        VExeDriveId = 5;
        break; }

      case 'F' : {
        VExeDriveId = 6;
        break; }

      case 'G' : {
        VExeDriveId = 7;
        break; }
      
      default : {
        err_retfail ( -1, (ERR_OUT,"Unknown drive id=%d", VExeDriveId) );
        VExeDriveId = 0;
      }
      
    }

    getcurdir( VExeDriveId, VStrCurDir );
    
    sprintf ( VStrCurDirPath, "%c:\\%s", VExeDriveLetter, VStrCurDir );
    sprintf ( StrRunDir, "%s", VStrCurDirPath );
  
    // msg (( MSG_OUT, "VStrCurDirPath = %s", VStrCurDirPath ));
    
    err_retok (( ERR_OUT, "" ));
  }
  
#else

  SInt32 FIL_FGetAppRunDir ( char* RunDirStr ) {
      
    err_retnull ( RunDirStr, (ERR_OUT,"RunDirStr == NULL") );
    
    sprintf ( RunDirStr, "%s", "Available only under Windows !" );
    
    return (0);
      
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
:
Ouputs    : The function returns
:  = 0 ok
:  < 0 in case of error
:
Globals   : None
:
Remark    : None
:
Level     : This is a user level function.
Date      : 07/03/2011
Doc date  :
Modif     :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 FIL_FGetDiskSectorSz ( char* Drive ) {


#ifdef CC_UNIX

  err_retfail ( -1, (ERR_OUT,"Not supported under Unix !" ) );
  
#else

  DWORD VDummyW;
  DWORD VBytesPerSector;
  BOOL  VRetBool;
  

  // BOOL GetDiskFreeSpace(
  //
  //     LPCTSTR lpRootPathName,	// address of root path
  //     LPDWORD lpSectorsPerCluster,	// address of sectors per cluster
  //     LPDWORD lpBytesPerSector,	// address of bytes per sector
  //     LPDWORD lpNumberOfFreeClusters,	// address of number of free clusters
  //     LPDWORD lpTotalNumberOfClusters 	// address of total number of clusters
  //    );
  

  VRetBool = GetDiskFreeSpace(
    Drive,	          // address of root path
    &VDummyW,	        // address of sectors per cluster
    &VBytesPerSector,	// address of bytes per sector
    &VDummyW,	        // address of number of free clusters
    &VDummyW 	        // address of total number of clusters
  );
  
  if ( VRetBool == True ) {
    return ( VBytesPerSector );
  }
  
  else {
    return (-1);
  }

#endif

}

/*******************************************************************************
Prototype : 
Goal      : 
Inputs    :
Ouputs    : 
Globals   :
Remark    : 
Level     : 
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/


FIL__TCBinFile :: FIL__TCBinFile ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl ) {

  PubFBegin ( ErrLogFile, EnableErrLog, ErrLogLvl );

  err_trace (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/


FIL__TCBinFile :: ~FIL__TCBinFile () {
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFBegin ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl ) {
    
  ProConfDone         = -1;
  ProParEnableErrLog  = EnableErrLog;
  ProParErrLogLvl     = ErrLogLvl;
  
  sprintf ( ProParErrLogFile, "%s", ErrLogFile );
  
  // --------------------------------------
  // Init all variables / parameters
  // --------------------------------------
  
  // Parameters from constructor
  
  sprintf ( ProParErrLogFile, "" );
  
  ProParEnableErrLog    = 0;
  ProParErrLogLvl       = ERR_LOG_LVL_NONE;
  
  // Parameters from conf
  
  sprintf ( ProParDataFile, "" );
  
  ProParRWBMode         = FIL__TCBinFile_RWB_MODE_READ;
  ProParMaxBlocSz        = 0;
  ProParFlushAfterWrite = 0;
  ProParMeasTime        = 0;
  
  // Variables for internal processing
  
  ProReadyToWrite       = -1;
  ProReadyToRead        = -1;
  
  ProCurRdEltId         = 0;
  ProCurRdSz            = 0;
  
  ProCurWrEltId         = 0;
  ProCurWrSz            = 0;
  ProTotWrSz            = 0;
  
  ProPtrBuffRdData      = NULL;
  ProSzBuffRdData       = 0;
  
  ProPtFile             = NULL;
  
  err_trace (( ERR_OUT, "" ));

  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFConf ( char* DataFile, SInt8 RWBMode, SInt32 MaxBlocSz, SInt32 BlocSz, SInt8 FlushAfterWrite, SInt8 MeasTime ) {

  sprintf ( ProParDataFile, "%s", DataFile );
  
  ProParRWBMode         = RWBMode;
  ProParMaxBlocSz       = MaxBlocSz;
  ProParBlocSz          = BlocSz;
  ProParFlushAfterWrite = FlushAfterWrite;
  ProParMeasTime        = MeasTime;

  ProConfDone           = 1;


  // Allocate memory buffer for data read from file

  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_WRITE ) {
  
    ProSzBuffRdData   = MaxBlocSz;
    ProPtrBuffRdData  = malloc ( ProSzBuffRdData );
        
    err_retnull ( ProPtrBuffRdData, (ERR_OUT,"Malloc of %d bytes failed !", ProSzBuffRdData) );
  }

  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFSetFileName ( char* DataFile ) {
  
  sprintf ( ProParDataFile, "%s", DataFile );
  
  err_retok (( ERR_OUT, "%s", DataFile ));
}
  

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 03/02/2009
Doc date  : 03/02/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFSetFlushMode ( SInt8 FlushAfterWrite ) {

  ProParFlushAfterWrite = FlushAfterWrite;
  

  err_retok (( ERR_OUT, "" ));
}
    


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/


SInt32 FIL__TCBinFile :: PubFGetFileSz  () {

  SInt32 VFileSz;
  SInt32 VCurPos;
  
  // If object conf not done => Read file size with FIL_FFileSize ()
  
  if ( ProConfDone == 0 ) {
    VFileSz = FIL_FFileSize ( ProParDataFile );
     err_retval ( VFileSz, ( ERR_OUT, "File size = %d Bytes", VFileSz ) );
  }

  // If conf done

  // If file in read mode
  
  if ( ProParRWBMode == FIL__TCBinFile_RWB_MODE_READ ) {
    
    // If file is closed
    
    if ( ProPtFile == NULL ) {
      VFileSz = FIL_FFileSize ( ProParDataFile );
      err_retval ( VFileSz, ( ERR_OUT, "File size = %d Bytes", VFileSz ) );
    }
    
    // If file is already open
    
    else {
      
      // Store current ( initial ) position in file
      
      if ( (VCurPos = ftell ( ProPtFile )) == -1 )  {
        err_retfail ( -1, (ERR_OUT,_strerror ( "ftell fail !" ) ));
      }
      
      // Goto end of file
            
      if ( fseek ( ProPtFile, 0, SEEK_END ) != 0 ) {
        err_retfail ( -1, (ERR_OUT,_strerror ( "fseek SEEK_END fail !" ) ));
      }
      
      // Get current position = END in file => it's file size 
      
      if ( (VFileSz = ftell ( ProPtFile )) == -1 )  {
        err_retfail ( -1, (ERR_OUT,_strerror ( "ftell fail !" ) ));
      }
      
      // Restor initial position in file
      
      if ( fseek ( ProPtFile, VCurPos, SEEK_SET ) != 0 ) {
        err_retfail ( -1, (ERR_OUT,_strerror ( "fseek SEEK_SET fail !" ) ));
      }
      
      err_retval ( VFileSz, ( ERR_OUT, "File size = %d Bytes", VFileSz ) );
      
    } // End file is already open
    
  }
  
  // File is in write mode OR in RW mode
  
  else {
    err_retval ( ProTotWrSz, ( ERR_OUT, "Current file size = %d Bytes", ProTotWrSz ) );
  }
  

}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFGetBlocNb  () {

  SInt32 VBlocNb;
  SInt32 VRemainder;
  SInt32 VFileSz;

  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );

  VFileSz = PubFGetFileSz ();
  
  err_retfail ( VFileSz, (ERR_OUT,"File size calculation failed !" ) );
  
  VBlocNb    = VFileSz / ProParBlocSz;
  VRemainder = VFileSz % ProParBlocSz;
  
  if ( VRemainder != 0 ) {
    err_retfail ( -VBlocNb, (ERR_OUT,"Not integer bloc number ! %d blocs + %d bytes !", VBlocNb, VRemainder ) );
  }
  
  err_retval ( VBlocNb, ( ERR_OUT, "File %s contains %d blocs ", ProParDataFile, VBlocNb ) );
}
    


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFCreate () {
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_WRITE ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can create a file when RWB mode IS NOT Write !") );
  }
  
  ProPtFile = fopen ( ProParDataFile, "wb" );
    
  err_retnull ( ProPtFile, (ERR_OUT,"Open for wb of file=%s failed ! => System : %s", ProParDataFile, _strerror ("") ) );
  
  ProReadyToWrite =  1;
  ProReadyToRead  = -1;

  ProCurWrEltId   = 0;
  ProCurWrSz      = 0;
  ProTotWrSz      = 0;
  
  err_retok (( ERR_OUT, "" ));
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFOpen () {
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_READ ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can't open a file when RWB mode IS NOT Read !") );
  }

  ProPtFile = fopen ( ProParDataFile, "rb" );
  
  err_retnull ( ProPtFile, (ERR_OUT,"Open for rb of file=%s failed ! => System : %s", ProParDataFile, _strerror ("") ) );
  
  ProReadyToWrite = -1;
  ProReadyToRead  =  1;
  
  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFSeqWrite ( void* PtrData, SInt32 DataSz ) {  
  SInt8 VBlocNbWritten;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToWrite, (ERR_OUT,"Abort => File not open in WB !") );
  
  err_retnull ( PtrData, (ERR_OUT,"PtrData == NULL") );
  
  if ( DataSz > ProParMaxBlocSz ) {
    err_retfail ( -1, (ERR_OUT,"Write abort : DataSz=%d > ProParMaxBlocSz=%d", DataSz, ProParMaxBlocSz) );
  }
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_WRITE ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can write to a file when RWB mode IS NOT Write !") );
  }
  
  ProCurWrSz = DataSz;
  
  #ifdef FIL__TCBinFile_MEAS_TIME
    #ifndef CC_ROOT_DLL
      if (ProParMeasTime) ProTime1 = GetTickCount ();
    #endif  
  #endif
  
  VBlocNbWritten = fwrite ( PtrData, DataSz /* Bloc size */, 1 /* Bloc NB */, ProPtFile );
  
  if ( VBlocNbWritten != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Bloc write failed ! => System : %s", _strerror ("") ) );
  }

  if ( ProParFlushAfterWrite == 1 ) {
    fflush ( ProPtFile );
  }

  ++ProCurWrEltId;
  ProTotWrSz += ProCurWrSz;

  #ifdef FIL__TCBinFile_MEAS_TIME
    if (ProParMeasTime) {
      #ifndef CC_ROOT_DLL
        ProTime2 = GetTickCount ();
        ProTimeExec = ProTime2 - ProTime1;
        err_trace (( ERR_OUT, "Bloc of %d bytes written in %d [ms]", DataSz, ProTimeExec ));
      #endif  
    }
  #endif
  
  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32  FIL__TCBinFile :: PubFSeqRead  ( void* DestPtr, SInt32 MaxDestSz, SInt32 DataSzToRead ) {
  
  SInt8 VBlocNbRead;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;

  err_retnull ( DestPtr, (ERR_OUT,"DestPtr == NULL") );
  
  if ( DataSzToRead > MaxDestSz ) {
    err_retfail ( -1, (ERR_OUT,"Read abort : DataSzToRead=%d > MaxDestSz=%d", DataSzToRead, MaxDestSz) );
  }
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_READ ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can read from a file when RWB mode IS NOT Read !") );
  }
  
  ProCurRdSz = DataSzToRead;
  
  #ifdef FIL__TCBinFile_MEAS_TIME
    #ifndef CC_ROOT_DLL
      if (ProParMeasTime) ProTime1 = GetTickCount ();
    #endif
  #endif
  
  VBlocNbRead = fread ( DestPtr, DataSzToRead /* Bloc size */, 1 /* Bloc NB */, ProPtFile );
  
  if ( VBlocNbRead != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Bloc read of %d bytes failed ! - VBlocNbRead = %d => System : %s", DataSzToRead, VBlocNbRead, _strerror ("") ) );
  }
  
  ++ProCurRdEltId;
  
  #ifdef FIL__TCBinFile_MEAS_TIME
  if (ProParMeasTime) {
    #ifndef CC_ROOT_DLL
      ProTime2 = GetTickCount ();
      ProTimeExec = ProTime2 - ProTime1;
      err_trace (( ERR_OUT, "Bloc of %d bytes read in %d [ms]", DataSzToRead, ProTimeExec ));
    #endif
  }
  #endif
  
  err_retok (( ERR_OUT, "" ));
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

void*  FIL__TCBinFile :: PubFSeqRead  ( SInt32 DataSzToRead ) {
  
  SInt8 VBlocNbRead;
  
  err_retfailnull ( ProConfDone   , (ERR_OUT,"Abort => Conf not done ") );
  err_retfailnull ( ProReadyToRead, (ERR_OUT,"Abort => File not open in RB !") );
  
  if ( DataSzToRead > ProParMaxBlocSz ) {
    err_retfailnull ( -1, (ERR_OUT,"Read abort : DataSzToRead=%d > ProParMaxBlocSz=%d", DataSzToRead, ProParMaxBlocSz) );
  }
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_READ ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Can read from a file when RWB mode IS NOT Read !") );
  }

  ProCurRdSz = DataSzToRead;
  
  #ifdef FIL__TCBinFile_MEAS_TIME
    #ifndef CC_ROOT_DLL
      if (ProParMeasTime) ProTime1 = GetTickCount ();
    #endif
  #endif
  
  VBlocNbRead = fread ( ProPtrBuffRdData, DataSzToRead /* Bloc size */, 1 /* Bloc NB */, ProPtFile );
  
  if ( VBlocNbRead != 1 ) {
    err_retfailnull ( -1, (ERR_OUT,"Bloc read failed ! => System : %s", _strerror ("") ) );
  }

  ++ProCurRdEltId;
  
  #ifdef FIL__TCBinFile_MEAS_TIME
  if (ProParMeasTime) {
    #ifndef CC_ROOT_DLL
      ProTime2 = GetTickCount ();
      ProTimeExec = ProTime2 - ProTime1;
      err_trace (( ERR_OUT, "Bloc of %d bytes read in %d [ms]", DataSzToRead, ProTimeExec ));
    #endif
  }
  #endif
  
  err_retval ( ProPtrBuffRdData, ( ERR_OUT, "" ) );
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFGotoBloc  ( SInt32 BlocNo ) {

  SInt32 VOffset;

  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;
  
  VOffset = BlocNo * ProParBlocSz;

  if ( fseek ( ProPtFile, VOffset, SEEK_SET ) != 0 ) {
    err_retfail ( -1, (ERR_OUT,"Goto bloc %d => %d bytes from beginning failed !", BlocNo ) );
  }

  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFBlocRead  ( SInt32 BlocNo, void* DestPtr, SInt32 MaxDestSz ) {

  SInt32 VRet;

  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;
  
  #ifdef FIL__TCBinFile_MEAS_TIME
    #ifndef CC_ROOT_DLL
      if (ProParMeasTime) ProTime1 = GetTickCount ();
    #endif
  #endif
  
  VRet = PubFGotoBloc ( BlocNo );
  
  err_retfail ( VRet, (ERR_OUT,"Goto bloc %d failed !", BlocNo ) );
    
  VRet = PubFSeqRead ( DestPtr, MaxDestSz, ProParBlocSz );
  
  err_retfail ( VRet, (ERR_OUT,"Read bloc %d failed !", BlocNo) );

  #ifdef FIL__TCBinFile_MEAS_TIME
    if (ProParMeasTime) {
      #ifndef CC_ROOT_DLL
        ProTime2 = GetTickCount ();
        ProTimeExec = ProTime2 - ProTime1;
        err_trace (( ERR_OUT, "Bloc [%4d] of %d bytes read in %d [ms]", BlocNo, ProParBlocSz, ProTimeExec ));
      #endif
    }
  #endif
  
  err_retok (( ERR_OUT, "" ));
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

void*  FIL__TCBinFile :: PubFBlocRead  ( SInt32 BlocNo ) {

  void*  VPtData;
  SInt32 VRet;

  err_retfailnull ( ProConfDone   , (ERR_OUT,"Abort => Conf not done ") );
  err_retfailnull ( ProReadyToRead, (ERR_OUT,"Abort => File not open in RB !") );
  
  #ifdef FIL__TCBinFile_MEAS_TIME
    #ifndef CC_ROOT_DLL
      if (ProParMeasTime) ProTime1 = GetTickCount ();
    #endif
  #endif
  
  
  VRet = PubFGotoBloc ( BlocNo );
  
  err_retfailnull ( VRet, (ERR_OUT,"Goto bloc %d failed !", BlocNo ) );
  
  VPtData = PubFSeqRead ( ProParBlocSz );
    
  #ifdef FIL__TCBinFile_MEAS_TIME
    if (ProParMeasTime) {
      #ifndef CC_ROOT_DLL
        ProTime2 = GetTickCount ();
        ProTimeExec = ProTime2 - ProTime1;
        err_trace (( ERR_OUT, "Bloc [%4d] of %d bytes read in %d [ms]", BlocNo, ProParBlocSz, ProTimeExec ));
      #endif
    }
  #endif
  
  err_retval ( VPtData, ( ERR_OUT, "" ) );
}
    


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFFlush () {
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  fflush ( ProPtFile );
  
  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 30/01/2009
Doc date  : 30/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCBinFile :: PubFClose () {
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );

  if ( ProReadyToWrite == 1 ) {
    fflush ( ProPtFile );
  }
  
  fclose ( ProPtFile );
    
  if ( ProPtrBuffRdData != NULL ) {
    free ( ProPtrBuffRdData );
    ProPtrBuffRdData = NULL;
  }
  
  ProConfDone = 0;
  
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
Date      : 08/07/2009
Doc date  : 08/07/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 FIL_FWriteRecord ( char* FileName, void* PtSrc, SInt32 RecSz ) {
  
  SInt32         VRet;
  FIL__TCBinFile VBinFile ( ERR_FGetLogFilePath (), ERR_FGetFileLogEnabled () /* EnableErrLog */, ERR_FGetFileLogLevel () );
  
  err_retnull ( PtSrc, (ERR_OUT,"PtSrc == NULL") );
    
  VRet = VBinFile.PubFConf       ( FileName, FIL__TCBinFile_RWB_MODE_WRITE, RecSz, RecSz, 1 /* FlushAfterWrite */, 0 /* MeasTime */ );
  err_retfail ( VRet, (ERR_OUT,"") );
  
  VRet = VBinFile.PubFCreate ();
  err_retfail ( VRet, (ERR_OUT,"") );
  
  VRet = VBinFile.PubFSeqWrite ( PtSrc, RecSz );
  err_retfail ( VRet, (ERR_OUT,"") );
  
  VRet = VBinFile.PubFClose ();
  err_retfail ( VRet, (ERR_OUT,"") );
  
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
Date      : 08/07/2009
Doc date  : 08/07/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 FIL_FReadRecord ( char* FileName, void* PtDest, SInt32 RecSz ) {
    
  SInt32         VRet;
  FIL__TCBinFile VBinFile ( ERR_FGetLogFilePath (), ERR_FGetFileLogEnabled () /* EnableErrLog */, ERR_FGetFileLogLevel () );
  
  err_retnull ( PtDest, (ERR_OUT,"PtSrc == NULL") );
    
  VRet = VBinFile.PubFConf       ( FileName, FIL__TCBinFile_RWB_MODE_READ, RecSz, RecSz, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
  err_retfail ( VRet, (ERR_OUT,"") );
  
  VRet = VBinFile.PubFOpen();
  err_retfail ( VRet, (ERR_OUT,"") );
  
  VRet = VBinFile.PubFSeqRead  ( PtDest, RecSz /* MaxDestSz */, RecSz /* DataSzToRead */ );
  err_retfail ( VRet, (ERR_OUT,"") );
  
  VRet = VBinFile.PubFClose ();
  err_retfail ( VRet, (ERR_OUT,"") );
  
  err_retok (( ERR_OUT, "" ));
}


// msg (( MSG_OUT, "Msg" ));



// ****************************
// FIL__TCStreamFile
// ****************************




/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/


FIL__TCStreamFile :: FIL__TCStreamFile ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl, SInt32 DiskBlocSz ) {
  
  PubFBegin ( ErrLogFile, EnableErrLog, ErrLogLvl, DiskBlocSz );
  
  err_trace (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/


FIL__TCStreamFile :: ~FIL__TCStreamFile () {

  // Free info record

  if ( ProPtRecInfFile != NULL ) {
    free ( ProPtRecInfFile );
  }

#ifndef CC_UNIX
  TerminateThread ( ProThreadHnd , 0 /* Thread exit code */ );

  DeleteCriticalSection ( &ProCsPrintMsg );
#endif

}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : 05/11/2010
          : - Error handling on disk access is not properly done, a message is printed
          : but the error is not propagated => MUST be done.
Level     :
Date      : 01/05/2010
          : - First implementation with 2 buffers for testing DAQ
Rev       : 09/05/2010
          : - Circular buffer implementation
Doc date  : 07/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

#ifndef CC_UNIX

DWORD WINAPI FIL__TCStreamFile_FThread (  LPVOID lpParam ) {

  FIL__TCStreamFile* VPtObj = (FIL__TCStreamFile*) lpParam;

  SInt32 VRet;
  DWORD  VBuffFull;
  UInt32 VNbBytesToWrite;
  UInt32 VNbBytesWritten;
  char   VStrMsg[GLB_CMT_SZ];
  

  while (1) {

    // Wait on buffer to read

    VBuffFull = WaitForSingleObject ( VPtObj->ProSemRdBuffHnd, INFINITE );

    switch ( VBuffFull ) {
      
      case WAIT_OBJECT_0 : {
        
        
        VNbBytesToWrite = VPtObj->ProParBlocSz;
        
        WriteFile(
        VPtObj->ProFileHnd,	 // handle to file to write to
        VPtObj->ProABuff[VPtObj->ProIndexRdBuff],	// pointer to data to write to file
        VNbBytesToWrite,	    // number of bytes to write
        &VNbBytesWritten,	   // pointer to number of bytes written
        NULL 	               // pointer to structure needed for overlapped I/O
        );
        
        if ( VNbBytesWritten != VNbBytesToWrite ) {
          sprintf ( VStrMsg, "ThreadDisk  => Writing buff %d error !", VPtObj->ProIndexRdBuff );
          VPtObj->PubFPrintMsg ( VStrMsg );
        }
        
        else {
          sprintf ( VStrMsg, "ThreadDisk  => Buffer %d save to disk", VPtObj->ProIndexRdBuff );
          VPtObj->PubFPrintMsg ( VStrMsg );
        }
        
        if ( VPtObj->ProParFlushAfterWrite == 1 ) {
          
          VRet = (SInt32) FlushFileBuffers ( VPtObj->ProFileHnd );
          
          if ( VRet == 0 ) {
            err_retfail ( -1, (ERR_OUT,"FlushFileBuffers (...) failed !") );
          }
          
        }
        
        
        ++VPtObj->ProIndexRdBuff;

        err_retfail ( VPtObj->ProIndexRdBuff, (ERR_OUT,"Bad variable size => ProIndexRdBuff=%d < 0", VPtObj->ProIndexRdBuff) );
        
        if ( VPtObj->ProIndexRdBuff >= FIL__TCStreamFile_MAX_BUFF_NB ) {
          VPtObj->ProIndexRdBuff = 0;
        }

        ReleaseSemaphore ( VPtObj->ProSemWrBuffHnd, 1, NULL );
        
        
        break; }
      
      
      default : {
        err_error  (( ERR_OUT, "Unknown WaitForMultipleObjects call return value = %d", VBuffFull ));
        break; }
      
    }



  } // End while (1)

  return 0;
}

#endif

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 05/11/2010
Doc date  : 05/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: ProFCalcProParBlocSz ( SInt32 DataSz ) {

  SInt32    VNotIntegerDiskBlocNb;
  SInt32    VRecInfSz;
  
  VNotIntegerDiskBlocNb = DataSz % ProParDiskBlocSz;
  
  if ( VNotIntegerDiskBlocNb ) {
    ProParBlocSz = ( (DataSz / ProParDiskBlocSz) + 1 ) * ProParDiskBlocSz;
  }
  
  else {
    ProParBlocSz = DataSz;
  }
  
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFBegin ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl, SInt32 DiskBlocSz ) {
  
  SInt8 ViBuff;
  
  ProConfDone         = -1;
  ProParEnableErrLog  = EnableErrLog;
  ProParErrLogLvl     = ErrLogLvl;
  
  sprintf ( ProParErrLogFile, "%s", ErrLogFile );
  
  // --------------------------------------
  // Init all variables / parameters
  // --------------------------------------
  
  // Parameters from constructor
  
  sprintf ( ProParErrLogFile, "" );
  
  ProParEnableErrLog    = 0;
  ProParErrLogLvl       = ERR_LOG_LVL_NONE;
  ProParDiskBlocSz      = DiskBlocSz;
  
  // Parameters from conf
  
  sprintf ( ProParDataFile, "" );
  sprintf ( ProInfFileName, "" );
  
  ProParRWBMode         = FIL__TCBinFile_RWB_MODE_READ;
  ProParMaxBlocSz       = 0;
  ProParFlushAfterWrite = 0;
  ProParMeasTime        = 0;
  ProParFixedBlocSzMode = 1;
  
  // Variables for internal processing
  
  ProUseThread = 0;
  
  ProFileHnd   = INVALID_HANDLE_VALUE;
  ProPtInfFile = NULL;  
  
#ifndef CC_UNIX
  InitializeCriticalSection ( &ProCsPrintMsg );
#endif
  
  ProThreadHnd = INVALID_HANDLE_VALUE;
  ProThreadId  = 0;
  
  for ( ViBuff=0; ViBuff < FIL__TCStreamFile_MAX_BUFF_NB; ViBuff++ ) {
    ProABuff[ViBuff]         = NULL;
  }

  ProSemWrBuffHnd       = INVALID_HANDLE_VALUE;
  ProSemRdBuffHnd       = INVALID_HANDLE_VALUE;

  ProIndexWrBuff        = 0;
  ProIndexRdBuff        = 0;

  ProBuffSz             = 0;

  ProPtRecInfFile       = NULL;
  ProRecInfSz           = 0;

  ProReadyToWrite       = -1;
  ProReadyToRead        = -1;
  
  ProCurRdBlocId        = 0;
  ProCurRdSz            = 0;
  
  ProCurWrBlocId        = 0;
  ProCurWrSz            = 0;
  ProTotWrSz            = 0;
  
  ProPtrBuffRdData      = NULL;
  ProSzBuffRdData       = 0;
  
  ProResWrBlocFailCnt   = 0;
      
  err_retok (( ERR_OUT, "" ));
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 07/03/2011
Doc date  : 07/03/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFSetDiskSectorSz ( SInt32 DiskBlocSz ) {

  ProParDiskBlocSz      = DiskBlocSz;
  
}
  

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 07/03/2011
Doc date  : 07/03/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFGetDiskSectorSz () {
    
  return (ProParDiskBlocSz);
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

void FIL__TCStreamFile :: PubFPrintMsg ( char* Msg ) {

#ifndef CC_UNIX
  EnterCriticalSection( &ProCsPrintMsg );
#endif
    err_trace   (( ERR_OUT, "%s", Msg ));

#ifndef CC_UNIX
  LeaveCriticalSection( &ProCsPrintMsg );
#endif

}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFConf ( FIL__TCStreamFile* PtMyself, SInt8 UseThread, char* DataFile, SInt8 RWBMode, SInt8 FixedBlocSzMode, SInt32 MaxBlocSz, SInt32 BlocSz, SInt8 FlushAfterWrite, SInt8 MeasTime ) {
  
  SInt8     ViBuff;
  SInt32    VNotIntegerDiskBlocNb;
#ifndef CC_UNIX
  TDateTime VODateTime;
#endif
  
  
  
  // -------------------------------------------------------
  // Set class parameters from PubFConf parameters list
  // -------------------------------------------------------

  err_retnull ( PtMyself, (ERR_OUT,"PtMyself == NULL") );
  
  PriPtMyself = PtMyself;
  
  ProUseThread          = UseThread;
  
  sprintf ( ProParDataFile, "%s", DataFile );
  
  sprintf ( ProInfFileName, "%s.inf", ProParDataFile );
  
  ProParRWBMode         = RWBMode;
  
  ProParFixedBlocSzMode = FixedBlocSzMode;
  
  ProParRequestedBlocSz = BlocSz;
  
  VNotIntegerDiskBlocNb = ProParRequestedBlocSz % ProParDiskBlocSz;
  
  if ( VNotIntegerDiskBlocNb ) {
    ProParBlocSz = ( (ProParRequestedBlocSz / ProParDiskBlocSz) + 1 ) * ProParDiskBlocSz;
  }
  
  else {
    ProParBlocSz = ProParRequestedBlocSz;
  }
    
  err_trace (( ERR_OUT, "RequestedBlocSz=%d - BlocSz=%d", ProParRequestedBlocSz, ProParBlocSz ));
  
  
  ProParRequestedMaxBlocSz = MaxBlocSz;
  
  if ( ProParRequestedMaxBlocSz >= ProParBlocSz ) {
    ProParMaxBlocSz = ProParRequestedMaxBlocSz;
  }
  
  else {
    err_warning (( ERR_OUT, "ProParMaxBlocSz=%d must be adjusted -> Set to ProParBlocSz=%d", ProParMaxBlocSz, ProParBlocSz ));
    ProParMaxBlocSz = ProParBlocSz;
  }
  
  ProParFlushAfterWrite = FlushAfterWrite;
  ProParMeasTime        = MeasTime;

  // -------------------------------------------------------
  // Alloc information file record
  // -------------------------------------------------------
  
  if ( ProParFixedBlocSzMode == 1 ) {
    ProRecInfSz = sizeof (FIL__TCStreamFile_Old_TRecInfFile);
  }
  
  else {
    
    // BUG ?
    
    // ProRecInfSz = sizeof (FIL__TCStreamFile_TRecInfFile) + ( FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE * sizeof (UInt64) );
    
    ProRecInfSz = sizeof (FIL__TCStreamFile_TRecInfFile) + ( FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE * sizeof (FIL__TCStreamFile_TBlocInf) );
    
  }

  ProPtRecInfFile = (FIL__TCStreamFile_TRecInfFile*) malloc ( ProRecInfSz );
    
  err_retnull ( ProPtRecInfFile, (ERR_OUT,"Allocation of info record failed !") );

  memset ( ProPtRecInfFile, 0, ProRecInfSz );

  // -------------------------------------------------------
  // Create or open information file
  // -------------------------------------------------------
  
  // Write mode => Create info file

  if ( ProParRWBMode == FIL__TCBinFile_RWB_MODE_WRITE ) {
    
    ProPtRecInfFile->Version     = 1;
    ProPtRecInfFile->FixedBlocSz = ProParFixedBlocSzMode;
    ProPtRecInfFile->MaxBlocSz   = ProParMaxBlocSz;
    ProPtRecInfFile->BlocSz      = ProParBlocSz;
    ProPtRecInfFile->BlocNb      = 0;
    ProPtRecInfFile->ABlocInf[0].Offset = 0;
    ProPtRecInfFile->ABlocInf[0].Sz     = 0;

#ifndef CC_UNIX
    VODateTime = VODateTime.CurrentDateTime ();
    ProPtRecInfFile->DateCreate = TIME__FConvDateTime2DateL ( VODateTime );
    ProPtRecInfFile->TimeCreate = TIME__FConvDateTime2Time  ( VODateTime );
#else
    ProPtRecInfFile->DateCreate.W32 = 0;
    ProPtRecInfFile->TimeCreate.W32 = 0;
#endif

    ProPtInfFile = fopen ( ProInfFileName, "wb" );
    
    err_retnull ( ProPtInfFile, (ERR_OUT,"Creation of info file %s failed !", ProInfFileName) );
    
    if ( fwrite ( ProPtRecInfFile, ProRecInfSz, 1, ProPtInfFile ) != 1 ) {
      err_retfail ( -1, (ERR_OUT,"Write info file %s failed !", ProInfFileName) );
    }
        
    if ( fclose (ProPtInfFile) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Close info file %s failed !", ProInfFileName) );
    }
    
    err_trace   (( ERR_OUT, "Info file=%s created", ProInfFileName ));
  }
    
  // Read mode => Open info file
  
  else {
  
    ProPtInfFile = fopen ( ProInfFileName, "rb" );
    
    err_retnull ( ProPtInfFile, (ERR_OUT,"Open info file %s failed => system : %s", ProInfFileName, _strerror ( "" ) ));
    err_trace   (( ERR_OUT, "Info file=%s read", ProInfFileName ));
    
    if ( fread ( ProPtRecInfFile, ProRecInfSz, 1, ProPtInfFile ) != 1 ) {
      err_retfail ( -1, (ERR_OUT,"Read info file %s failed => system : %s", ProInfFileName, _strerror ( "" )) );
    }
    
    if ( fclose (ProPtInfFile) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"Close info file %s failed !", ProInfFileName) );
    }
    
    // Print info file 
    
    PubFPrintInfFile ();

    // Set bloc size
    
    ProParBlocSz = ProPtRecInfFile->BlocSz;
    
    if ( ProParRequestedBlocSz != ProParBlocSz ) {
      err_warning (( ERR_OUT, "Requested bloc sz=%d <> Info file bloc sz=%d => Use Info file bloc sz", ProParRequestedBlocSz, ProParBlocSz ));
    }
    
  }
  

  // -------------------------------------------------------------
  // Write mode  => Allocate buffers, create semaphores and thread
  // -------------------------------------------------------------


  if ( ProParRWBMode == FIL__TCBinFile_RWB_MODE_WRITE ) {
    
    if ( FIL__TCStreamFile_MAX_BUFF_NB > 4 ) {
      err_retfail ( -1, (ERR_OUT,"FIL__TCStreamFile_MAX_BUFF_NB = %d ! Handling of more than 4 buffers is NOT implemented !", FIL__TCStreamFile_MAX_BUFF_NB) );
    }
    
    // Allocate buffers
    
    ProBuffSz = ProParMaxBlocSz;
    
    for ( ViBuff=0; ViBuff < FIL__TCStreamFile_MAX_BUFF_NB; ViBuff++) {
    
      // If already allocated => Free  
    
      if ( ProABuff[ViBuff] != NULL ) {
        free ( ProABuff[ViBuff] );
      }
    
      // Allocate
    
      ProABuff[ViBuff] = (UInt8*) malloc ( ProBuffSz );
      
      err_retnull ( ProABuff[ViBuff], (ERR_OUT,"Allocation buffer[%d] of %d bytes failed !", ViBuff, ProBuffSz ) );
      err_trace   (( ERR_OUT, "Buffer[%d] of %d bytes allocated", ViBuff, ProBuffSz ));
      
      // Reset content with $FF
      
      memset ( ProABuff[ViBuff], 0xFF, ProBuffSz );
    }
        
    // Create & init semaphores

  #ifndef CC_UNIX
    ProSemWrBuffHnd = CreateSemaphore( NULL, FIL__TCStreamFile_MAX_BUFF_NB, FIL__TCStreamFile_MAX_BUFF_NB, NULL ); // Init = Max = Max buff nb
      
    err_retnull ( ProSemWrBuffHnd, (ERR_OUT,"Create SemWrBuff failed !  - LastError=%d", GetLastError ()) );
      
    ProSemRdBuffHnd = CreateSemaphore( NULL, 0, FIL__TCStreamFile_MAX_BUFF_NB, NULL ); // Init = 0, Max = Max buff nb
      
    err_retnull ( ProSemRdBuffHnd, (ERR_OUT,"Create SemRdBuff failed !  - LastError=%d", GetLastError ()) );
  #endif

    // Init buffers index
    
    ProIndexWrBuff = 0;
    ProIndexRdBuff = 0;
    
    // Create thread

  #ifndef CC_UNIX
    ProThreadHnd = CreateThread ( NULL, 0, FIL__TCStreamFile_FThread, (void*) PriPtMyself /* Param */ , 0, &ProThreadId );
    err_retnull ( ProThreadHnd, (ERR_OUT,"Thread creation failed ! LastError=%d", GetLastError ()) );
  #endif
  
  }
    
  // -------------------------------------------------------------
  // Read mode => Allocate memory buffer for data read from file
  // -------------------------------------------------------------
    
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_WRITE ) {
    
    ProSzBuffRdData   = ProParMaxBlocSz;
    ProPtrBuffRdData  = malloc ( ProSzBuffRdData );
    
    err_retnull ( ProPtrBuffRdData, (ERR_OUT,"Malloc of %d bytes failed !", ProSzBuffRdData) );
  }

  ProConfDone           = 1;
  
  err_retok (( ERR_OUT, "" ));
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFSetFileName ( char* DataFile ) {
  
  sprintf ( ProParDataFile, "%s", DataFile );
  
  err_retok (( ERR_OUT, "%s", DataFile ));
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 03/02/2009
Doc date  : 03/02/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFSetFlushMode ( SInt8 FlushAfterWrite ) {
  
  ProParFlushAfterWrite = FlushAfterWrite;
  
  
  err_retok (( ERR_OUT, "" ));
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 20/05/2010
Doc date  : 20/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFPrintInfFile () {
  
  SInt32 ViBloc;
  SInt16 ViSpareW32Info;
  
  msg (( MSG_OUT, "-------------------------" ));
  msg (( MSG_OUT, " Info file record " ));
  msg (( MSG_OUT, "-------------------------" ));
  msg (( MSG_OUT, "Version     = %d", ProPtRecInfFile->Version ));
  msg (( MSG_OUT, "-------------------------" ));
  msg (( MSG_OUT, "Date create = %s", TIME__FDateL2Str ( ProPtRecInfFile->DateCreate, NULL, 0 ) ));
  msg (( MSG_OUT, "Time create = %s", TIME__FTime2Str  ( ProPtRecInfFile->TimeCreate, NULL, 0 ) ));
  msg (( MSG_OUT, "Date close  = %s", TIME__FDateL2Str ( ProPtRecInfFile->DateClose , NULL, 0 ) ));
  msg (( MSG_OUT, "Time close  = %s", TIME__FTime2Str  ( ProPtRecInfFile->TimeClose , NULL, 0 ) ));
  msg (( MSG_OUT, "-------------------------" ));
  msg (( MSG_OUT, "FixedBlocSz = %d", ProPtRecInfFile->FixedBlocSz ));
  msg (( MSG_OUT, "MaxBlocSz   = %d", ProPtRecInfFile->MaxBlocSz ));
  msg (( MSG_OUT, "BlocSz      = %d", ProPtRecInfFile->BlocSz ));
  msg (( MSG_OUT, "BlocNb      = %d", ProPtRecInfFile->BlocNb ));
  msg (( MSG_OUT, "-------------------------" ));
  msg (( MSG_OUT, "ABlocInf[0].Offset             = %Lu", ProPtRecInfFile->ABlocInf[0].Offset ));
  msg (( MSG_OUT, "ABlocInf[0].Sz                 = %d  ", ProPtRecInfFile->ABlocInf[0].Sz ));
  msg (( MSG_OUT, "ABlocInf[0].SpareW32InfoFormat = %d  ", ProPtRecInfFile->ABlocInf[0].SpareW32InfoFormat ));
  msg (( MSG_OUT, "ABlocInf[0].SpareW32InfoNb     = %d  ", ProPtRecInfFile->ABlocInf[0].SpareW32InfoNb ));
  
  for ( ViSpareW32Info=0; ViSpareW32Info <  ProPtRecInfFile->ABlocInf[0].SpareW32InfoNb; ViSpareW32Info++ ) {
    msg (( MSG_OUT, "ABlocInf[0].ASpareW32Info[%.2d] = %d  ", ViSpareW32Info, ProPtRecInfFile->ABlocInf[0].ASpareW32Info[ViSpareW32Info] ));
  }
  
  
  if ( ProPtRecInfFile->FixedBlocSz == 0 ) {
    
    for ( ViBloc=0; ViBloc < ProPtRecInfFile->BlocNb; ViBloc++ ) {
      msg (( MSG_OUT, "Bloc[%.4d] : Offset=%.12Lu - Sz=%.8d [Bytes] - ASpare [0]=%.4d [1]=%.4d [2]=%.4d [3]=%.4d", ViBloc, ProPtRecInfFile->ABlocInf[ViBloc].Offset, ProPtRecInfFile->ABlocInf[ViBloc].Sz, ProPtRecInfFile->ABlocInf[ViBloc].ASpareW32Info[0], ProPtRecInfFile->ABlocInf[ViBloc].ASpareW32Info[1], ProPtRecInfFile->ABlocInf[ViBloc].ASpareW32Info[2], ProPtRecInfFile->ABlocInf[ViBloc].ASpareW32Info[3] ));
    }
    
  }
  
  msg (( MSG_OUT, "-------------------------" ));
  
  err_retok (( ERR_OUT, "" ));
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/


SInt32 FIL__TCStreamFile :: PubFGetFileSz  () {
  
  SInt32 VFileSz;
  SInt32 VCurPos;
  
  // If object conf not done => Read file size with FIL_FFileSize ()
  
  if ( ProConfDone == 0 ) {
    VFileSz = FIL_FFileSize ( ProParDataFile );
    err_retval ( VFileSz, ( ERR_OUT, "File size = %d Bytes", VFileSz ) );
  }
  
  // If conf done
  
  // If file in read mode
  
  if ( ProParRWBMode == FIL__TCBinFile_RWB_MODE_READ ) {
    
    // If file is closed
    
    if ( ProFileHnd == INVALID_HANDLE_VALUE ) {
      VFileSz = FIL_FFileSize ( ProParDataFile );
      err_retval ( VFileSz, ( ERR_OUT, "File size = %d Bytes", VFileSz ) );
    }
    
    // If file is already open
    
    else {
      err_retfail ( -1, (ERR_OUT,"File is opened => Get file size not coded in this state (TOB ...)") );
    }
    
  }
  
  // File is in write mode OR in RW mode
  
  else {
    err_retval ( ProTotWrSz, ( ERR_OUT, "Current file size = %d Bytes", ProTotWrSz ) );
  }
  
  
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 31/01/2009
          : - Calculation => file size / block size
Rev       : 20/05/2010
          : - Read from info file field BlocNb
          :
Doc date  : 31/01/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFGetBlocNb  () {
  
  SInt32 VBlocNb;
  SInt32 VRemainder;
  SInt32 VFileSz;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );

/* Before 20/05/2010

  VFileSz = PubFGetFileSz ();
  
  err_retfail ( VFileSz, (ERR_OUT,"File size calculation failed !" ) );
  
  VBlocNb    = VFileSz / ProParBlocSz;
  VRemainder = VFileSz % ProParBlocSz;
  
  if ( VRemainder != 0 ) {
    err_retfail ( -VBlocNb, (ERR_OUT,"Not integer bloc number ! %d blocs + %d bytes !", VBlocNb, VRemainder ) );
  }
  
*/

  VBlocNb = ProPtRecInfFile->BlocNb;

  err_retval ( VBlocNb, ( ERR_OUT, "File %s contains %d blocs ", ProParDataFile, VBlocNb ) );
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFCreate () {

#ifdef CC_UNIX

  err_retfail ( -1, (ERR_OUT,"Not supported undex UNIX !") );
  
#else

  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_WRITE ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can create a file when RWB mode IS NOT Write !") );
  }
  
  ProFileHnd = CreateFile(
    ProParDataFile,	                  // pointer to name of the file
    GENERIC_WRITE	| GENERIC_READ,	    // access (read-write) mode
    FILE_SHARE_READ	,	                // share mode
    NULL ,	                           // pointer to security attributes
    OPEN_ALWAYS,	                     // how to create
    FILE_FLAG_NO_BUFFERING,	          // file attributes
    NULL 	                            // handle to file with attributes to copy
  );
      
  
  if ( ProFileHnd == INVALID_HANDLE_VALUE ) {
    err_retfail ( -1, (ERR_OUT,"Open for wb of file=%s failed ! => System : %s", ProParDataFile, _strerror ("") ) );
  }


  ProReadyToWrite =  1;
  ProReadyToRead  = -1;
  
  ProCurWrBlocId  = 0;
  ProCurWrSz      = 0;
  ProTotWrSz      = 0;
  
  ProResWrBlocFailCnt = 0;
  
  err_retok (( ERR_OUT, "" ));
  
#endif  
  
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFOpen () {

#ifdef CC_UNIX

  ProFilePtUx = fopen ( ProParDataFile, "rb" );
  
  err_retnull ( ProFilePtUx, (ERR_OUT,"Open for rb of file=%s failed ! => System : %s", ProParDataFile, _strerror ("") ) );

  ProReadyToWrite = -1;
  ProReadyToRead  =  1;
  
  err_retok (( ERR_OUT, "" ));
  
#else

  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_READ ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can't open a file when RWB mode IS NOT Read !") );
  }
  

  ProFileHnd = CreateFile(
    ProParDataFile,	            // pointer to name of the file
    GENERIC_READ,	              // access (read-write) mode
    FILE_SHARE_READ	,	          // share mode
    NULL ,	                     // pointer to security attributes
    OPEN_ALWAYS,	               // how to create
    FILE_FLAG_NO_BUFFERING,	    // file attributes
    NULL 	                      // handle to file with attributes to copy
    );
    

  if ( ProFileHnd == INVALID_HANDLE_VALUE ) {
    err_retfail ( -1, (ERR_OUT,"Open for rb of file=%s failed ! => System : %s", ProParDataFile, _strerror ("") ) );
  }
  
  
  ProReadyToWrite = -1;
  ProReadyToRead  =  1;
  
  err_retok (( ERR_OUT, "" ));
  
#endif
}




/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
          : - First implementation with 2 buffers for testing DAQ
Rev       : 09/05/2010
          : - Circular buffer implementation
          :
          : 24/02/2011
          : - Handle SpareW32Info as an array now => New parameters
          :
Doc date  : 07/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

  
SInt32 FIL__TCStreamFile :: PubFSeqWrite ( void* PtrData, SInt32 DataSz, SInt32 DbgCallPar, SInt32 SpareW32InfoFormat, SInt32* PtSpareW32Info, SInt16 SpareW32InfoNb ) {
  

#ifdef CC_UNIX

  err_retfail ( -1, (ERR_OUT,"Not supported undex UNIX !") );
  
#else

  SInt32 VRet;
  SInt16 ViSpareW32Info;
  DWORD  VBuffFree;
  UInt32 VNbBytesWritten;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToWrite, (ERR_OUT,"Abort => File not open in WB !") );
  
  err_retnull ( PtrData, (ERR_OUT,"PtrData == NULL") );
  
  if ( DataSz > ProParMaxBlocSz ) {
    err_retfail ( -1, (ERR_OUT,"Write abort : DataSz=%d > ProParMaxBlocSz=%d", DataSz, ProParMaxBlocSz) );
  }
  
  err_retnull ( PtSpareW32Info, (ERR_OUT,"PtSpareW32Info == NULL") );
  
  if ( SpareW32InfoNb > FIL__TCStreamFile_BLOC_INF_MAX_SPARE_W32_INFO_NB ) {
    err_retfail ( -1, (ERR_OUT,"SpareW32InfoNb=%d > Max=%d", SpareW32InfoNb, FIL__TCStreamFile_BLOC_INF_MAX_SPARE_W32_INFO_NB ) );
  }
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_WRITE ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Cant write to a file when RWB mode IS NOT Write !") );
  }
  
  // If variable bloc sz mode => Test max nb of blocs allowed & Adjust ProParBlocSz 

  if ( ProParFixedBlocSzMode == 0 ) {
    
    if ( ProPtRecInfFile->BlocNb >= FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE ) {
      err_retfail ( -1, (ERR_OUT,"Max bloc nb = %d reached in variable bloc sz mode !", FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE) );
    }
     
    ProFCalcProParBlocSz ( DataSz );
  }

  // err_error (( ERR_OUT, "DataSz=%d - ProParBlocSz=%d bytes", DataSz, ProParBlocSz ));
  
  ProCurWrSz = ProParBlocSz;
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  #ifndef CC_ROOT_DLL
    if (ProParMeasTime) ProTime1 = GetTickCount ();
  #endif
  #endif
  
  // Let thread write data to disk in background task
  
  if ( ProUseThread == 1 ) {
    
    // Wait on buffer to write
    
    VBuffFree = WaitForSingleObject ( ProSemWrBuffHnd, 1 /* ms */ );
    
    switch ( VBuffFree ) {
      
      case WAIT_TIMEOUT : {
        ++ProResWrBlocFailCnt;
        err_retfail ( -1, (ERR_OUT,"Write failed for the %d time -> No buffer available - DbgCallPar=%d !", ProResWrBlocFailCnt, DbgCallPar ) );
        break; }
      
      case WAIT_OBJECT_0	 : {
        
        memcpy ( ProABuff[ProIndexWrBuff], PtrData, DataSz );
        
        // PubFPrintMsg ( "Thread => Buffer %d filled", ProIndexWrBuff );
        err_trace (( ERR_OUT, "Thread => Buffer %d filled - DbgCallPar=%d", ProIndexWrBuff, DbgCallPar ));
        
        ++ProIndexWrBuff;
        
        err_retfail ( ProIndexWrBuff, (ERR_OUT,"Bad variable size => ProIndexWrBuff=%d < 0", ProIndexWrBuff) );
        
        if ( ProIndexWrBuff >= FIL__TCStreamFile_MAX_BUFF_NB) {
          ProIndexWrBuff = 0;
        }
                
        ReleaseSemaphore ( ProSemRdBuffHnd, 1, NULL );
        break; }
                  
      default : {
        err_error  (( ERR_OUT, "Unknown WaitForMultipleObjects call return value = %d", VBuffFree ));
        break; }
      
    }
    
  }
  
  // Write NOW data to disk
  
  else {
    
    // Copy to buffer -> "Automatically" add padding bytes
    
    memcpy ( ProABuff[0], PtrData, DataSz );
    
    // Warning => Write ProParBlocSz bytes AND NOT the value passed via parameter DataSz
    //            because non buffered I/O functions MUST write multiple of DISK bloc size
    
    WriteFile(
    ProFileHnd,	// handle to file to write to
    ProABuff[0],	// pointer to data to write to file
    ProParBlocSz,	// number of bytes to write
    &VNbBytesWritten,	// pointer to number of bytes written
    NULL 	// pointer to structure needed for overlapped I/O
    );
    
    if ( VNbBytesWritten != ProParBlocSz ) {
      err_retfail ( VRet, (ERR_OUT,"Writing block %d failed %d bytes written - %d bytes requested ! - %s", ProCurWrBlocId, VNbBytesWritten, ProParBlocSz, strerror ( errno ) ) );
    }
    
    if ( ProParFlushAfterWrite == 1 ) {
      
      VRet = (SInt32) FlushFileBuffers ( ProFileHnd );
      
      if ( VRet == 0 ) {
        err_retfail ( -1, (ERR_OUT,"FlushFileBuffers (...) failed !") );
      }
      
    }
    
  }
  
  
  if ( ProParFixedBlocSzMode == 0 ) {
    ProPtRecInfFile->ABlocInf[ProPtRecInfFile->BlocNb].Offset             = ProTotWrSz;
    ProPtRecInfFile->ABlocInf[ProPtRecInfFile->BlocNb].Sz                 = ProCurWrSz;
    
    ProPtRecInfFile->ABlocInf[ProPtRecInfFile->BlocNb].SpareW32InfoFormat = SpareW32InfoFormat;
    ProPtRecInfFile->ABlocInf[ProPtRecInfFile->BlocNb].SpareW32InfoNb     = SpareW32InfoNb;
    
    for ( ViSpareW32Info=0; ViSpareW32Info < SpareW32InfoNb; ViSpareW32Info++ ) {
      ProPtRecInfFile->ABlocInf[ProPtRecInfFile->BlocNb].ASpareW32Info[ViSpareW32Info] = PtSpareW32Info[ViSpareW32Info];
    }
            
  }
  
  ++ProPtRecInfFile->BlocNb;

  ++ProCurWrBlocId;
  ProTotWrSz += ProCurWrSz;
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  if (ProParMeasTime) {
    #ifndef CC_ROOT_DLL
    ProTime2 = GetTickCount ();
    ProTimeExec = ProTime2 - ProTime1;
    
    if ( ProUseThread == 1 ) {
      err_trace (( ERR_OUT, "Copy bloc of %d bytes done in %d [ms]", DataSz, ProTimeExec ));
    }
    
    else {
      err_trace (( ERR_OUT, "Bloc of %d bytes written in %d [ms]", VNbBytesWritten, ProTimeExec ));
    }
    
    #endif
  }
  #endif
  
  err_retok (( ERR_OUT, "" ));
  
#endif

}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32  FIL__TCStreamFile :: PubFSeqRead  ( void* DestPtr, SInt32 MaxDestSz, SInt32 DataSzToRead ) {

#ifdef CC_UNIX

  SInt32 VNotIntegerDiskBlocNb;
  SInt32 VDataSzToRead;
  SInt32 VBlocNbRead;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;
  
  err_retnull ( DestPtr, (ERR_OUT,"DestPtr == NULL") );
  
  // Calculate real size to read => MUST be a multiple of DISK bloc size
  
  VNotIntegerDiskBlocNb = DataSzToRead % ProParDiskBlocSz;
  
  if ( VNotIntegerDiskBlocNb ) {
    VDataSzToRead = ( (DataSzToRead / ProParDiskBlocSz) + 1 ) * ProParDiskBlocSz;
  }
  
  else {
    VDataSzToRead = DataSzToRead;
  }
  
  if ( VDataSzToRead > MaxDestSz ) {
    err_retfail ( -1, (ERR_OUT,"Read abort : VDataSzToRead=%d > MaxDestSz=%d", VDataSzToRead, MaxDestSz) );
  }
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_READ ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can read from a file when RWB mode IS NOT Read !") );
  }
  
  
  ProCurRdSz = VDataSzToRead;
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  #ifndef CC_ROOT_DLL
  if (ProParMeasTime) ProTime1 = GetTickCount ();
  #endif
  #endif
  

  VBlocNbRead = fread ( DestPtr, VDataSzToRead /* Bloc size */, 1 /* Bloc NB */, ProFilePtUx );
      
  if ( VBlocNbRead != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Reading block %d failed of %d bytes ! - %s", ProCurRdBlocId, VDataSzToRead, strerror ( errno ) ) );
  }
  
  ++ProCurRdBlocId;
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  if (ProParMeasTime) {
    #ifndef CC_ROOT_DLL
    ProTime2 = GetTickCount ();
    ProTimeExec = ProTime2 - ProTime1;
    err_trace (( ERR_OUT, "Bloc of %d bytes read in %d [ms]", DataSzToRead, ProTimeExec ));
    #endif
  }
  #endif
  
  err_retok (( ERR_OUT, "" ));
  
#else

  SInt32 VNotIntegerDiskBlocNb;
  SInt32 VDataSzToRead;
  UInt32 VNbBytesRead;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;
  
  err_retnull ( DestPtr, (ERR_OUT,"DestPtr == NULL") );
  
  // Calculate real size to read => MUST be a multiple of DISK bloc size
  
  VNotIntegerDiskBlocNb = DataSzToRead % ProParDiskBlocSz;
  
  if ( VNotIntegerDiskBlocNb ) {
    VDataSzToRead = ( (DataSzToRead / ProParDiskBlocSz) + 1 ) * ProParDiskBlocSz;
  }
  
  else {
    VDataSzToRead = DataSzToRead;
  }

  if ( VDataSzToRead > MaxDestSz ) {
    err_retfail ( -1, (ERR_OUT,"Read abort : VDataSzToRead=%d > MaxDestSz=%d", VDataSzToRead, MaxDestSz) );
  }
  
  if ( ProParRWBMode != FIL__TCBinFile_RWB_MODE_READ ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Can read from a file when RWB mode IS NOT Read !") );
  }
  
  
  ProCurRdSz = VDataSzToRead;
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  #ifndef CC_ROOT_DLL
    if (ProParMeasTime) ProTime1 = GetTickCount ();
  #endif
  #endif


  ReadFile(
    ProFileHnd,	// handle of file to read
    DestPtr,	// address of buffer that receives data
    VDataSzToRead,	// number of bytes to read
    &VNbBytesRead,	// address of number of bytes read
    NULL 	// address of structure for data
  );
  
  if ( VNbBytesRead != VDataSzToRead ) {
    err_retfail ( -1, (ERR_OUT,"Reading block %d failed : %d bytes read - %d bytes requested ! - %s", ProCurRdBlocId, VNbBytesRead, VDataSzToRead, strerror ( errno ) ) );
  }
  
  ++ProCurRdBlocId;
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  if (ProParMeasTime) {
    #ifndef CC_ROOT_DLL
      ProTime2 = GetTickCount ();
      ProTimeExec = ProTime2 - ProTime1;
      err_trace (( ERR_OUT, "Bloc of %d bytes read in %d [ms]", DataSzToRead, ProTimeExec ));
    #endif
  }
  #endif
  
  err_retok (( ERR_OUT, "" ));
  
#endif
  
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

void*  FIL__TCStreamFile :: PubFSeqRead  ( SInt32 DataSzToRead ) {
  
  SInt32 VRet;
  
  VRet = PubFSeqRead  ( ProPtrBuffRdData, ProSzBuffRdData, DataSzToRead );
  
  err_retfailnull ( VRet, (ERR_OUT,"PubFSeqRead failed !") );

  err_retval ( ProPtrBuffRdData, ( ERR_OUT, "" ) );  
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
          : Limited to 4 GB files
          :
Rev       : 09/05/2010
          : - Use W64 offset => Not limited to 4 GB file
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFGotoBloc  ( SInt32 BlocNo ) {

#ifdef CC_UNIX

  SInt32  VRet;
  UInt64  VOffsetW64;
  UInt32* VPtOffsetLow32;
  UInt32* VPtOffsetHigh32;
  LPVOID  VMsgBuf;
  
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;
  
  if ( BlocNo >= ProPtRecInfFile->BlocNb ) {
    err_retfail ( -1, (ERR_OUT,"BlocNo=%d > Max=%d", BlocNo, ProPtRecInfFile->BlocNb-1 ) );
  }
  
  VPtOffsetLow32  = ((UInt32*) &VOffsetW64);
  VPtOffsetHigh32 = ((UInt32*) &VOffsetW64) + 1;
  
  if ( ProParFixedBlocSzMode == 0 ) {
    VOffsetW64 = ProPtRecInfFile->ABlocInf[BlocNo].Offset;
  }
  
  else {
    VOffsetW64 = (UInt64) BlocNo * (UInt64) ProParBlocSz; // Cast (UInt64) needed otherwise 32 bits arithmetic is used ;
  }
  
  
  VRet = fseeko ( ProFilePtUx, (off_t) VOffsetW64, SEEK_SET );
  
  err_retfail ( VRet, (ERR_OUT,"Bloc %d read failed ! => System : %s", BlocNo, _strerror ("") ) );
      
  err_retok (( ERR_OUT, "" ));
   
#else

  UInt32  VRet;
  UInt64  VOffsetW64;
  UInt32* VPtOffsetLow32;
  UInt32* VPtOffsetHigh32;
  LPVOID  VMsgBuf;
  
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;

  if ( BlocNo >= ProPtRecInfFile->BlocNb ) {
    err_retfail ( -1, (ERR_OUT,"BlocNo=%d > Max=%d", BlocNo, ProPtRecInfFile->BlocNb-1 ) );
  }

  VPtOffsetLow32  = ((UInt32*) &VOffsetW64);
  VPtOffsetHigh32 = ((UInt32*) &VOffsetW64) + 1;
    
  if ( ProParFixedBlocSzMode == 0 ) {
    VOffsetW64 = ProPtRecInfFile->ABlocInf[BlocNo].Offset;
  }
  
  else {
    VOffsetW64 = (UInt64) BlocNo * (UInt64) ProParBlocSz; // Cast (UInt64) needed otherwise 32 bits arithmetic is used ;  
  }
  

  VRet = SetFilePointer (
    ProFileHnd,	           // handle of file
    *VPtOffsetLow32,	      // number of bytes to move file pointer
    (SInt32*) VPtOffsetHigh32,	      // address of high-order word of distance to move
                           // lpDistanceToMoveHigh = 0 => Limited to 4 GB
    FILE_BEGIN 	           // how to move
  );


  if ( VRet == 0xFFFFFFFF ) {
    
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &VMsgBuf,
      0,
      NULL
    );
    
    err_retfail ( -1, (ERR_OUT,"Goto bloc %d failed => %s", BlocNo, VMsgBuf ) );
  }
  
  err_retok (( ERR_OUT, "" ));
  
#endif

}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Rev       : 24/02/2011
          : - Handle SpareW32Info as and array ASpareW32Info now => New parameters
          :
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFBlocRead  ( SInt32 BlocNo, void* DestPtr, SInt32 MaxDestSz, SInt32* PtSpareW32InfoFormat, SInt16 MaxSpareW32InfoNb, SInt32* PtSpareW32Info ) {
  
  SInt32 VRet;
  SInt16 ViSpareW32Info;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  err_retfail ( ProReadyToRead , (ERR_OUT,"Abort => File not open in RB !") );;
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  #ifndef CC_ROOT_DLL
    if (ProParMeasTime) ProTime1 = GetTickCount ();
  #endif
  #endif
  
  VRet = PubFGotoBloc ( BlocNo );
  
  err_retfail ( VRet, (ERR_OUT,"Goto bloc %d failed !", BlocNo ) );
  
  if ( ProParFixedBlocSzMode == 0 ) {
    
    ProParBlocSz = ProPtRecInfFile->ABlocInf[BlocNo].Sz;
    
    if ( (PtSpareW32InfoFormat != NULL) && (PtSpareW32Info != NULL) && (ProPtRecInfFile->ABlocInf[BlocNo].SpareW32InfoNb <= MaxSpareW32InfoNb) ) {
      
      *PtSpareW32InfoFormat = ProPtRecInfFile->ABlocInf[BlocNo].SpareW32InfoFormat;
      
      for ( ViSpareW32Info=0; ViSpareW32Info < ProPtRecInfFile->ABlocInf[BlocNo].SpareW32InfoNb; ViSpareW32Info++ ) {
        PtSpareW32Info[ViSpareW32Info] = ProPtRecInfFile->ABlocInf[BlocNo].ASpareW32Info[ViSpareW32Info];
      }      
      
    }
    
    else {
      err_warning (( ERR_OUT, "ASpareInfo bloc not updated ! Pointer NULL or Dest size too small ?" ));
    }
    
  }
    
  VRet = PubFSeqRead ( DestPtr, MaxDestSz, ProParBlocSz );
  
  err_retfail ( VRet, (ERR_OUT,"Read bloc %d failed !", BlocNo) );
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  if (ProParMeasTime) {
    #ifndef CC_ROOT_DLL
      ProTime2 = GetTickCount ();
      ProTimeExec = ProTime2 - ProTime1;
      err_trace (( ERR_OUT, "Bloc [%4d] of %d bytes read in %d [ms]", BlocNo, ProParBlocSz, ProTimeExec ));
    #endif
  }
  #endif
  
  return (ProParBlocSz);
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

void*  FIL__TCStreamFile :: PubFBlocRead  ( SInt32 BlocNo ) {
  
  void*  VPtData;
  SInt32 VRet;
  
  err_retfailnull ( ProConfDone   , (ERR_OUT,"Abort => Conf not done ") );
  err_retfailnull ( ProReadyToRead, (ERR_OUT,"Abort => File not open in RB !") );
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  #ifndef CC_ROOT_DLL
    if (ProParMeasTime) ProTime1 = GetTickCount ();
  #endif
  #endif
  
  
  VRet = PubFGotoBloc ( BlocNo );
  
  err_retfailnull ( VRet, (ERR_OUT,"Goto bloc %d failed !", BlocNo ) );

  if ( ProParFixedBlocSzMode == 0 ) {
    ProParBlocSz = ProPtRecInfFile->ABlocInf[BlocNo].Sz;
  }
  
  VPtData = PubFSeqRead ( ProParBlocSz );
  
  #ifdef FIL__TCStreamFile_MEAS_TIME
  if (ProParMeasTime) {
    #ifndef CC_ROOT_DLL
      ProTime2 = GetTickCount ();
      ProTimeExec = ProTime2 - ProTime1;
      err_trace (( ERR_OUT, "Bloc [%4d] of %d bytes read in %d [ms]", BlocNo, ProParBlocSz, ProTimeExec ));
    #endif
  }
  #endif
  
  err_retval ( VPtData, ( ERR_OUT, "" ) );
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFFlush () {

#ifdef CC_UNIX

  err_retfail ( -1, (ERR_OUT,"Not supported undex UNIX !") );
  
#else

  SInt32 VRet;
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  VRet = (SInt32) FlushFileBuffers ( ProFileHnd );
 
  if ( VRet == 0 ) {
    err_retfail ( -1, (ERR_OUT,"FlushFileBuffers (...) failed !") );
  }

  err_retok (( ERR_OUT, "" ));
  
#endif

}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 01/05/2010
Doc date  : 01/05/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FIL__TCStreamFile :: PubFClose () {
  
  SInt32    VRet;
  SInt8     ViBuff;
#ifndef CC_UNIX
  TDateTime VODateTime;
#endif
  
  err_retfail ( ProConfDone    , (ERR_OUT,"Abort => Conf not done !") );
  
  if ( ProReadyToWrite == 1 ) {
    PubFFlush ();
  }
  
  ProConfDone = 0;
    
#ifndef CC_UNIX
  CloseHandle ( ProFileHnd );
#endif

  // Free read buffers
  
  if ( ProPtrBuffRdData != NULL ) {
    free ( ProPtrBuffRdData );
    ProPtrBuffRdData = NULL;
  }

  // Free write buffers

  for ( ViBuff=0; ViBuff < FIL__TCStreamFile_MAX_BUFF_NB; ViBuff++) {
    
    if ( ProABuff[ViBuff] != NULL ) {
      free ( ProABuff[ViBuff] );
      ProABuff[ViBuff] = NULL;
    }
    
  }

  
  // Get close date & time

#ifndef CC_UNIX
  VODateTime = VODateTime.CurrentDateTime ();
  ProPtRecInfFile->DateClose = TIME__FConvDateTime2DateL ( VODateTime );
  ProPtRecInfFile->TimeClose = TIME__FConvDateTime2Time  ( VODateTime );
#else
  ProPtRecInfFile->DateClose.W32 = 0;
  ProPtRecInfFile->TimeClose.W32 = 0;
#endif
  

  if ( ProReadyToWrite == 1 ) {
  
    // Overwrite info file with update close time & date + bloc nb
    
    ProPtInfFile = fopen ( ProInfFileName, "wb" );
    
    err_retnull ( ProPtInfFile, (ERR_OUT,"Overwrite of info file %s failed !", ProInfFileName) );
    
    if ( fwrite ( ProPtRecInfFile, ProRecInfSz, 1, ProPtInfFile ) != 1 ) {
      err_retfail ( -1, (ERR_OUT,"Write info file %s failed !", ProInfFileName) );
    }
    
    if ( fclose (ProPtInfFile) != 0 ) {
      err_retfail ( -1, (ERR_OUT,"fclose on info file failed => System : %s", strerror ( errno ) ) );
    }
  
  }

  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
 Prototype :
 Goal      : returns the start run time in the day in seconds
 Inputs    :
 Ouputs    :
 Globals   :
 Remark    :
 Level     :
 Date      : 11/05/2012
 Doc date  : 11/05/2012
 Author    : Jerome BAUDOT
 E-mail    : baudot@in2p3.fr
 Labo      : IPHC */
/******************************************************************************/

int FIL__TCStreamFile :: GetTime () {
  
  TIME__TSTime aTime = (ProPtRecInfFile->TimeCreate).Time;
  
  return aTime.Hour*60*60 + aTime.Min*60 + aTime.Sec;
  
}


#endif


