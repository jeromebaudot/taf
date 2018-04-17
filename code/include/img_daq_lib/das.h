// @(#)maf/maf:$Name:  $:$Id das.h  v.1 2005/10/02 18:03:46 sha Exp $
//  Author : Gilles CLAUS    23/11/02 

///////////////////////////////////////////////////////////////////////////////////////////////
// das.h :                                                                              //
// MIMOSA 9 : has a special feature. Matrices 64 x 64 and 32 x 32 recorded                   //
// in the same time.                                                                         //
// So while one frame is read on the first submatrix, 4 frames are read on the               //
// second submatrix.                                                                         //
// This has to be taken into account mainly in DPlane.cxx, mi9_mat_a3.c, event_header.typ .  //
// Put readout = 8 in the config file.                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////

#undef TCD_COMPIL

#define DAS_EVENT_MAX_SZ (5*1024*1024)

#define DAS_HEADER_SZ 112
#define DAS_REFDET_SZ 2048
#define DAS_TRAILER_SZ 4 


typedef struct{
  
  TPEventHeader PtHeader;
  int* PtRef;
  int* PtMimosa;
  int* PtEOR;
  
}
DAS_TEvent;
