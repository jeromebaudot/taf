/*******************************************************************************
File      : sync_index_rec.c
Goal      : Functions to synchronized two DAQ files
Prj date  : 18/07/2012
File date : 18/07/2012
Doc date  : 18/07/2012
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


#ifndef SYNC_INDEX_REC_C
#define SYNC_INDEX_REC_C



/* DOC_FUNC_BEGIN */
/* ===================================================================================
 Return file size or negative value upon failure.
 Author    : Gilles CLAUS
*/
 /* =================================================================================== */
/* DOC_FUNC_END */
int GetFileSize ( char* FilePath ) {
  
  FILE* VPf;
  int VFileSz = 0;
  
  if ( ( VPf = fopen ( FilePath, "rb" ) ) == NULL ) {
    printf( "GetFileSize: fopen fail !\n" );
    return -1;
  }
  
  /* Calculate file size */
  
  if ( fseek ( VPf, 0, SEEK_SET ) != 0 ) {
    printf( "GetFileSize: fseek SEEK_SET fail !\n" );
    return -1;
  }
  
  if ( fseek ( VPf, 0, SEEK_END ) != 0 ) {
    printf( "GetFileSize: fseek SEEK_END fail !\n" );
    return -1;
  }
  
  if ( (VFileSz = ftell ( VPf )) == -1 )  {
    printf( "GetFileSize: ftell fail !" );
    return -1;
  }
  
  if ( fseek ( VPf, 0, SEEK_SET ) != 0 ) {
    printf( "GetFileSize: fseek SEEK_SET fail !");
    return -1;
  }
  
  if ( fclose (VPf) != 0 ) {
    printf( "GetFileSize: fclose fail !" );
    return -1;
  }
  
  return VFileSz;
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
 Prototype :
 :
 Goal      : Read the file containing the synchronization information
 :            between two boards, each with its data file.
 :           For instance a board reading binary sensors
 :            and another board reading analog sensors.
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
 Date      : 10/07/2012
 Doc date  :
 Modif     :
 Author    : Gilles CLAUS
 E-mail    : claus@lepsi.in2p3.fr
 Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

int GetSyncPointer ( char* FilePath, unsigned char* PtDest, int DestSz ) {
  
  int VRet;
  FILE*  VPfDest;
  int VFileSz;
  
  
  // Check param
  
  if( PtDest==NULL ) {
    printf( "GetSyncPointer: PtDest == NULL !\n");
    return -1;
  }
  
  // Measure file size
  
  VFileSz = GetFileSize ( FilePath );
  
  // Check buffer size / file size
  
  if ( DestSz < VFileSz ) {
    printf( "GetSyncPointer: Buffer size=%d < File size=%d.\n", DestSz, VFileSz );
    return -1;
  }
  
  printf( "GetSyncPointer: TRACE => FileSize=%d\n", VFileSz );
  
  // Open file
  
  VPfDest = fopen ( FilePath, "rb" );
  
  if( VPfDest == NULL ) {
    printf( "GetSyncPointer: Source file %s open failed.\n", FilePath );
    return -1;
  }
  
  // Load file in buffer
  
  VRet = fread ( PtDest, VFileSz /* size */, 1 /* Elt nb */, VPfDest );
  
  printf("SYNC Version is %d\n", ((APP__TSyncIndexRec*)PtDest)->Version);
  
  if ( VRet != 1 ) {
    printf( "GetSyncPointer: Error reading source file %s.\n", FilePath );
    return -1;
  }
  
  VRet = fclose ( VPfDest );
  
  if ( VRet != 0 ) {
    printf( "GetSyncPointer: Error fclose source file %s.\n", FilePath );
    return -1;
  }
  
  
  // Return record nb
  
  return ( VFileSz / sizeof (APP__TSyncIndexRec) );
  
}


#endif