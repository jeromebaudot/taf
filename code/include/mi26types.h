#ifndef _mi26types_h
#define _mi26types_h

typedef struct {
  unsigned int Header;
  unsigned int TriggerCnt;
  unsigned int TriggerLine; // ? not sure
  unsigned int FrameCnt;
  unsigned int DataLength;
  unsigned int ADataW16[140]; //data MI26
  unsigned int Trailer;
  
} MI26_FrameRaw;


typedef union {
  
  unsigned int W16;
  
  struct {
    
    unsigned int StateNb  :  4;
    unsigned int LineAddr : 11;
    unsigned int Ovf      :  1;
    
  } F;
  
} MI26__TStatesLine;


typedef union {
  
  unsigned int W16;
  
  struct {
    
    unsigned int HitNb   :  2;
    unsigned int ColAddr : 11;
    unsigned int NotUsed :  3;
    
  } F;
  
} MI26__TState;



#endif
