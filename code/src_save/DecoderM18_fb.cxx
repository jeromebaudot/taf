
/* $Id: DecoderM18_fb.h 0 2012-11-14 15:26:05Z valtini $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to MIMOSA digits in raw data.
/// Developer: fb
//////////////////////////////////////////////////////////////////////////////

#include "DecoderM18_fb.h"

ClassImp(DecoderM18_fb)

// #############################################################################

DecoderM18_fb::DecoderM18_fb( Int_t aBoardNumber, Int_t aRunNumber, Int_t aSensorNumber) :
fFileInput(0x0),
fDataChar1(0),
fDataChar2(0),
fDataChar3(0),
fDataChar4(0),
fEventCounterCDH(0),
fWordCounterCDH(0),
fTriggerExternalCounter(0),
fLastEvt(0),
fTrigger(0),
fModeRead(-1),
fMimosaMode(0),
fChipNbr(0),
fNbrChip(0),
fEventCounterLH(0),
fWordCounterLH(0),
fEvent(0),
fEvent2(0),
fCount(0),
fEvt(0),
fjump(0),
fcol(0),
frow(0),
famp(0),
Index_Max(0)
{
  fBoardNumber = aBoardNumber;
  fRunNumber = aRunNumber;
  fSensorNumber = aSensorNumber;
  
  for(Int_t gg=0; gg<=1000; gg++) PIXEL[gg]=0x2bc;
    
  for(Int_t i=0;i<kChipsPerDDL;i++){fActiveChip[i]=kFALSE;fChipKey[i]=kFALSE;}
  for(Int_t i=0;i<kRowPerChip;i++){
     for(Int_t j=0;j<kColPerChip;j++){
       fDataFrame[i][j]=-1; fDataFrame2[i][j]=-1;fDataFrame3[i][j]=-1;}}
  NewEvent();
}
// #############################################################################

DecoderM18_fb::DecoderM18_fb(const DecoderM18_fb& rstream) :
TObject(rstream),
fFileInput(0x0),
fDataChar1(0),
fDataChar2(0),
fDataChar3(0),
fDataChar4(0),
fEventCounterCDH(0),
fWordCounterCDH(0),
fTriggerExternalCounter(0),
fLastEvt(0),
fTrigger(0),
fModeRead(-1),
fMimosaMode(0),
fChipNbr(0),
fNbrChip(0),
fEventCounterLH(0),
fWordCounterLH(0),
fEvent(0),
fEvent2(0),
fCount(0),
fEvt(0),
fjump(0),
fcol(0),
frow(0),
famp(0),
Index_Max(0)
{
  for(Int_t gg=0; gg<=1000; gg++) PIXEL[gg]=0x2bc;
    
  // copy constructor
  printf("Copy constructor should not be used.\n");
}

// #############################################################################

DecoderM18_fb& DecoderM18_fb::operator=(const DecoderM18_fb& rstream) {
  // assignment operator
  if (this!=&rstream) {}
  printf("Assignment opertator should not be used.\n");
  return *this;
}

// #############################################################################

//Bool_t DecoderM18_fb::ReadNextInt() {
UInt_t DecoderM18_fb::ReadNextInt() {

    if(!fFileInput){ printf("Error No Input File\n"); return kFALSE; }
   
    fread(&fDataChar1,1,4,fFileInput);
    
 return fDataChar1;
}

// #############################################################################

void DecoderM18_fb::NewEvent(){
  // call this to reset flags for a new event
  /*
  fEventCounterCDH = 0;
  fWordCounterCDH = 0;
  for(Int_t i=0;i<kChipsPerDDL;i++){fActiveChip[i]=kFALSE; fChipKey[i]=kFALSE;}
  for(Int_t i=0;i<kRowPerChip;i++){
     for(Int_t j=0;j<kColPerChip;j++){ 
       fDataFrame[i][j]=-1; fDataFrame2[i][j]=-1; fDataFrame3[i][j]=-1;}}
*/
   }
//#############################################################################

Bool_t DecoderM18_fb::ReadCDH() {
    
  fDataChar1 = ReadNextInt();
    
  if (fDebugLevel) {
    printf("DecoderM18_fb::ReadCDH first word=%x for file\n", fDataChar1);
  }
    
return !feof(fFileInput);
    
}

//#############################################################################
Int_t DecoderM18_fb::Get_Nevent() {
    
    if(!fFileInput){ printf("Error No Input File\n"); return kFALSE; }
   
    while(!feof(fFileInput))
    {
        fDataChar4=ReadNextInt();
        if(fDataChar4 == 0xfafafafa){fCount++;}
    }

    return fCount-2;
}
// #############################################################################
Int_t DecoderM18_fb::Get_Event_Info() {
    //Int_t fEvent=0;
    unsigned int word1;
    unsigned int *pointer1;
    pointer1 = &word1;
    fread(pointer1, 1, 4, fFileInput);
    //printf("ciao");
    Int_t NonNcount=-111111111;
    Int_t NumeroDiPixelAccesi=0;
    
    while(!feof(fFileInput)){
        if(word1 == 0xfafafafa){ NonNcount=0;
        //    printf("############# fevent---->  %x\n",word1);
        }
        //START THE COUNTER OF WORDS
        NonNcount++;
        //IF THE COUNTER == 2 YOU GET THE "REAL EVENT"
        if(NonNcount==2){fEvent=word1;printf("--fevent---->  %d \n",fEvent);}
        if(NonNcount==3){             printf("--3 word---->  %d \n",word1); }
        if(NonNcount==4){             printf("--4 word---->  %d \n",word1); }
        if(NonNcount==5){fjump =word1;printf("-- fjump --->  %d \n",fjump); }
        //JUST PRINT THE WORD TO JUMP;
        if(NonNcount>5 && NonNcount< 6 + fjump){printf("---->  %d \n",word1);}
       // printf("NonNcount = %d \n", NonNcount);
        // START THE ANALYSIS OF THE PIXEL;
        if(NonNcount>= 6 + fjump){
            // GET INFO ON THE PIXEL FIRED, I.E., N EVT, COL, ROW, AMPL;
            GetPixeltrs(word1, fcol, frow, famp, fEvent);
            // CODE TO PRINT THE N OF FIRED PIXEL FOR EVENT;
            for(Int_t jk=10; jk<=NonNcount; jk++){
                Int_t npixelfired=jk-9;
                if(NonNcount==jk) NumeroDiPixelAccesi=npixelfired;
            }
           printf("N pixel fired = %d \n", NumeroDiPixelAccesi);
        }
        fread(pointer1, 1, 4, fFileInput);
    }
    return fEvent; // fjump;
}
// #############################################################################

Int_t DecoderM18_fb::GetPixel_i_index(Int_t i){

  // Return the index of the pixel
  
  Int_t row = PIXEL[i] & 0x000000FF;
  Int_t col = (PIXEL[i] & 0x0000FF00) >> 8;
  Int_t index = row*256 + col;
  
  if (fDebugLevel>1) {
    printf("DecoderM18:: Getting index for pixel %d, row=%d, col=%d => index=%d\n", i, row, col, index);
  }
  
  return index ;

}
  // #############################################################################
  
  Bool_t DecoderM18_fb::GetPixeltrs(unsigned int  WORD, Int_t FCOL, Int_t FROW, Int_t FAMP, Int_t FEVENT){
    

    FROW = WORD & 0x000000FF;
    FCOL = (WORD & 0x0000FF00) >> 8;
    FAMP = (WORD & 0xFFFF0000) >> 16;
    
    printf("PIXEL INFO--- EVT:     -- > (row, col, amp) (WORD: ..), %d, %d, %d, %d, %x \n", FEVENT, FROW, FCOL, FAMP, WORD);
    
    return kTRUE; //fcol, frow, famp, fEvent;
}

// #############################################################################
Int_t DecoderM18_fb::Read_Evento(Int_t evt){
    
    UInt_t word = 0;
    
    fEvt=0;
    
    Int_t evento;
    Int_t indice=-1000;
    
    //for(Int_t jk=0; jk<=1000; jk++){
    while(!feof(fFileInput)){
    //do{
        if(!ReadNextInt())return kFALSE;
        
        word = fDataChar1;
        
        if(word == 0xfafafafa) indice=0;
        if(indice==1) evento = word;
        if(evento==evt){printf("----------EVENTO ---->  %x",word);printf("\n");}
        printf("----------WORD ---->  %x",word); printf("\n");
    
    
    //}while(fDataChar1 != 0xfafafafa);
    
    //Read_Evento(0);
    }
    
    if(!ReadNextInt())return kFALSE;
    
    return 0;
}

// #############################################################################
/*
UInt_t DecoderM18_fb::ReadData() // READ THE SINGLE EVENT;
{
    
    //UInt_t fElem[100000]={0x0};
    
    Int_t NonNcount2=0;
    //UInt_t PIXEL[];
    
    do{
        //if(!ReadNextInt())return kFALSE;
        fDataChar2 = ReadNextInt();
        NonNcount2++;
        //printf("-------fdatachar2---->  %x \n",fDataChar2);
        //printf("-------NonNcount2---->  %d \n",NonNcount2);
        if(NonNcount2==1){ fEvent2 = fDataChar2;}
        if(NonNcount2==4){ fjump   = fDataChar2;} // printf("-------fjump ---->  %d\n",fjump);}
        if(NonNcount2>= 5 + fjump){
            
            Int_t npixel = NonNcount2-5-fjump;
            if(fDataChar2!=0xfafafafa){
                printf("---che cazzo di evento è ---->  %d \n",npixel);
                //GetPixel(fDataChar2, fcol, frow, famp, fEvent2);
                frow = fDataChar2 & 0x000000FF;
                fcol = (fDataChar2 & 0x0000FF00) >> 8;
                famp = (fDataChar2 & 0xFFFF0000) >> 16;
                printf("PIXEL INFO - - - -EVT - - - -> (row, col, amp) (WORD: ..), %d, %d, %d, %d, %x \n", fEvent2, frow, fcol, famp, fDataChar2);
            }
        }
        
    }while(fDataChar2!=0xfafafafa);
    
    
    return fEvent2, fcol, frow, famp;
}*/
// #############################################################################
// #############################################################################
Bool_t DecoderM18_fb::ReadData() // READ THE SINGLE EVENT;
{
    
    for(Int_t i = 0; i<=1000; i++){
      
        fDataChar1 = ReadNextInt();
        
        //printf("i:   %d---------------------------------- generic words ->->->-> %x \n",i, fDataChar1);
        
        if(i==0){ 
          fEvent2 = fDataChar1;
          if (fDebugLevel) printf("  Real event number is %d for event %d\n", fEvent2, fCount);
        }
        
        if(i==3){ 
          fjump   = fDataChar1;
          if (fDebugLevel>1) printf("    words to jump %d for event %d\n", fjump, fCount);
          }
        
        if(i >= 4 + fjump){
            
          if(fDataChar1==0xfafafafa) {
            if (fDebugLevel) printf("  Trailer %x found => end of event %d\n", fDataChar1, fCount);
            break;
          }
            
          if(fDataChar1==0x2bc) {
            if (fDebugLevel) printf("  word %x found => break reading for event %d\n", fDataChar1, fCount);
              break;
          }
          
            Int_t index = i-4-fjump;
            
            PIXEL[index]=fDataChar1-32756;
            //printf("PIXEL -> %x\n",  PIXEL[index]);
            
            Index_Max=index+1;
            //printf("Valore max dell'indice => %d \n", Index_Max);
            
            
        }
        
    }
    
  //  printf("---- the end of event ------ \n");
    
    
return true;
}

//__________________________________________________________________________
Bool_t DecoderM18_fb::HasData() 
{

  if (fDebugLevel) printf("DecoderM18::HasData reading event %d\n", fCount);

  Index_Max = 0; // reset nb of pixels fired
    
  if( !ReadData() ) return false;
  
  if (fDebugLevel) printf("  event %d (real #=%d) completed with %d pixels found\n", fCount, fEvent2, Index_Max);
  
  fCount++;
  return true;
  
}


//__________________________________________________________________________
void DecoderM18_fb::PrintStatistics(ostream &stream) 
{
  
  // Print statistics on the events read by this board
  //
  // JB, 2014/05/14
  
  stream << "***********************************************" << endl;
  stream << " Board DecoderM18 " << fBoardNumber << " found:" << endl;
  stream << fCount << " events in total," << endl;
  stream << "***********************************************" << endl;
  
  
}
// #############################################################################
// #############################################################################

