// @(#)maf/dtools:$Name:  $:$Id:DStrip.cxx  v.3 2005/10/02 18:03:46 sha Exp $
// Author   : ?
// Last Modified, JB 2009/05/25
// Last Modified, JB 2014/01/07 SetPedestal

  ////////////////////////////////////////////////////////////
  // Class Description of DStrip                            //
  //                                                        //
  //                                                        //
  ////////////////////////////////////////////////////////////


#include "DStrip.h"
#include "DSetup.h"
#include "DPlane.h"
#include "DR3.h"

ClassImp(DStrip) // Description of Single Detector DStrip 
//______________________________________________________________________________
//  
DStrip::DStrip()
{ 
  // DStrip default constructor
}

//______________________________________________________________________________
//  

DStrip::~DStrip()
{
  // DStrip default destructor

  delete [] fNeighbourList;
}

//______________________________________________________________________________
//  
DStrip::DStrip(DPlane& aPlane, const Int_t aStripIndex, DR3&  aPosition, DR3& aSize)
{
  // constructor of a Strip or Pixel 
  //
  // Last modified: JB, 2008/10/15

  //cout << "BUILDING strip " << aStripIndex << endl;

  fPlane           = &aPlane;
  fc               = &fPlane->GetSetup();
  fDebugStrip      =  fPlane->GetDebug();
  fPlaneNumber     =  fPlane->GetPlaneNumber();
  fPosition        =  aPosition;
  fSize            =  aSize;
  fStripIndex      =  aStripIndex;

  if(fDebugStrip>1 && (fStripIndex<2 || fStripIndex>fPlane->GetStripsN()-3) ) printf("DStrip: building strip %d from plane %d with (%d,%d) strips and (%f,%f)=%f bound\n", aStripIndex, fPlaneNumber, fPlane->GetStripsNu(), fPlane->GetStripsNv(), fc->GetPlanePar(fPlaneNumber).ClusterLimit(0), fc->GetPlanePar(fPlaneNumber).ClusterLimit(1), fc->GetPlanePar(fPlaneNumber).ClusterLimit.Length());
  fBound           =  fc->GetPlanePar(fPlaneNumber).ClusterLimit.Length();
 
 //--calculate max number of elements in a cluster for 2-dim plane PIXELS (MIMOSA):
  if( fPlane->GetStripsNu() > 1 && fPlane->GetStripsNv() > 1 ) {
    fNeighbourCountMaximum = (Int_t) ( (1 + 2 * fc->GetPlanePar(fPlaneNumber).ClusterLimit(0)/fc->GetPlanePar(fPlaneNumber).Pitch(0)) * (1 + 2 * fc->GetPlanePar(fPlaneNumber).ClusterLimit(1)/fc->GetPlanePar(fPlaneNumber).Pitch(1)) );
  }  // -- and for STRIP planes:
  else {
    fNeighbourCountMaximum = (Int_t) (1 + 2 * fBound/fc->GetPlanePar(fPlaneNumber).Pitch(0));
  }

  if(fDebugStrip>2 && fStripIndex<2) printf("DStrip: building strip %d from plane %d with %d neighbours max in %f um.\n", fStripIndex, fPlaneNumber, fNeighbourCountMaximum, fBound);

  // at fBound 0.2 mm and a pitch perp. to strips get 0.2/0.05 = 4
  // take this times two for both sides, adjacent to seed strip
  // add 1 since seed is part of its own neighbourhood

  fNeighbourCount        = 0;
  fNeighbourList         = new DStrip*[fNeighbourCountMaximum];

  fFound      = kFALSE;
  fUsePulse   = kTRUE;

  fRawValue       = 0.;
  fRawFrame1Value = 0.;
  fRawFrame2Value = 0.;

  fInitialPedestal= 0.;
  fInitialTMS     = 0.;
  fSumCount       = 0;
  fSumSquareCount = 0;
  fSumValue       = 0.;
  fSumSquareValue = 0.;
  fCommonMode     = 0.;
  fWeightPedestal = 10;
  fWeightNoise    = 10;
  // three following lines to avoid use of UpdateXXX(), JB
  fPedestal       = 0; 
  fCommonMode     = 0; 
  fNoise          = 0.01; 

  fNoiseCacheIndex   = 0;
  fPedestalCacheIndex= 0;
  
  fCacheSize         =  fc->GetPlanePar(fPlaneNumber).CacheSize;

  for(Int_t i = 0; i < fCacheSize; i++) {
    fPedestalCache[i] = 0.;
    fNoiseCache[i]    = 0.;
  }  
}
//______________________________________________________________________________

Float_t DStrip::GetPulseHeightToNoise(){ //YV 04/06/09 convert to double
  //Double_t DStrip::GetPulseHeightToNoise(){
  //----ADC
  //----abm8
  Int_t tReadout;
  tReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if((tReadout==10)||(tReadout==12)||(tReadout==13)){
    if (fNoise > 0.0)
      fNoise = 0.01; 
    if((tReadout==12)||(tReadout==13)){
      fNoise = 1.0; 
    }
      return fPulseHeight / fNoise;
  }else{
    if (fNoise > 0.0)
      return fPulseHeight / fNoise;
    else 
      return 0.0;
  }
}
//______________________________________________________________________________
//  

DStrip* DStrip::GetNeighbour(Int_t aNI)
{ 

  if ( aNI <= fNeighbourCount )
    return fNeighbourList[aNI]; 
  else {
    printf(" WARNING from DStrip: Impossible to return requested NeighbourStrip %d over %d neighbours, return this instead\n", aNI, fNeighbourCount);
    return this;
  }
}

//______________________________________________________________________________
//  
void DStrip::SumValue() 
{
  // external value replaced by data member fRawValue
  // JB 2009/05/25

  Float_t bV;

  if(fPedestalCacheIndex >= fCacheSize) {
    fPedestalCacheIndex = 0;
    bV = aSignalSupressedValue(fPedestalCache, fCacheSize);   
    //if( fStripIndex==25) printf("SV: index=%d, valueCache=%f, count=%d, newV=%f\n", fStripIndex, bV, fSumCount, fRawValue);

    fSumValue    += bV;  // test 
    fSumCount++; 
  }

  fPedestalCache[fPedestalCacheIndex++] = fRawValue;  

}

//______________________________________________________________________________
//  
void DStrip::SumSquareValue() 
{ 
  // external value replaced by data member fRawValue
  // JB 2009/05/25

  Float_t  bV;

  if(fNoiseCacheIndex >= fCacheSize ) {
    fNoiseCacheIndex = 0;
    bV = aSignalSupressedValue(fNoiseCache, fCacheSize); // suppress extrema
    //      cout << fNoiseCacheIndex << " -  " << bV << endl; 
    //if( fStripIndex==25) printf("SQV: index=%d, valueCache=%f, count=%d, newV=%f\n", fStripIndex, bV, fSumSquareCount, fRawValue);

    fSumSquareValue  += (bV*bV); 
    fSumSquareCount++;
  }

  fNoiseCache[fNoiseCacheIndex++] = fRawValue;

}

//______________________________________________________________________________
//  
void DStrip::InitPedestal()
 {
  if (fSumCount != 0) {
    fInitialPedestal  = fSumValue / fSumCount;    
    fPedestal = fInitialPedestal;
    fSumValue = 0.; 
    fSumCount = 0;
    fPedestalCacheIndex = 0;
    for(Int_t i = 0; i < fCacheSize; i++)
      fPedestalCache[i] = 0.;
  }  else {
    printf("<DStrip::InitPedestal()> strip %d has zero entries for pedestal initialization, investigate\n", fStripIndex);
    fInitialPedestal = 0.;
  }
  //----ADC
  Int_t tReadout;
  tReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if((tReadout==12)||(tReadout==13)){
    fInitialPedestal = 0.;
  }
  //----ADC
}
//______________________________________________________________________________
//  
void DStrip::InitNoise() 
{
  // initializes the noise using the first N events
  // the initial noise is normally higher.
  // the initial noise is convoluted with common mode error, 
  // common mode variation are order of 15% from single strip noise in telescope silicon

  Float_t tVariance;

  if (fPedestal == 0. ) {
    printf("<DStrip::InitNoise()> warning for strip %d, will try to initialize pedestal", fStripIndex);
    InitPedestal();
  }

   tVariance = fabs(fSumSquareValue - fSumValue * fSumValue / fSumSquareCount ) / (fSumSquareCount - 1 );
    
  fNoise = sqrt( tVariance );
  //----abm8
  Int_t tReadout;
  tReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if(tReadout==10){
    fNoise = 0.01;
  }
  //----ADC
  if((tReadout==12)||(tReadout==13)){
    //    if(fNoise < 0.01){    
      fNoise = 1.0;
      //    }
  }
  //----ADC
    printf("<DStrip::InitNoise()> fSumSquareValue %f, fSumValue %f, fSumSquareCount %d, fSumCount %d, fPedestal %f, tVariance %f, fNoise %f\n", fSumSquareValue,  fSumValue, fSumSquareCount, fSumCount, fPedestal, tVariance, fNoise);


  // reset buffer and couter 

  fPedestalCacheIndex = 0;
  fNoiseCacheIndex    = 0;
  for(Int_t i = 0; i < fCacheSize; i++) {
    fPedestalCache[i] = 0.;
    fNoiseCache[i]    = 0.;
  }  

}

//______________________________________________________________________________
//  

void DStrip::InitNoiseAndPedestal() {
  Float_t tVariance;

   if (fSumCount != 0) {
    fInitialPedestal  = fSumValue / fSumCount;    
        tVariance = fabs(fSumSquareValue-fSumValue*fSumValue/fSumSquareCount )/(fSumSquareCount-1);

    fNoise = sqrt( tVariance );
    //if( fStripIndex==25) printf("Init: index=%d, sum=%f, count=%d, var=%f, count=%d\n", fStripIndex, fSumValue, fSumCount, tVariance, fSumSquareCount);
    
    fPedestal = fInitialPedestal;
    fPedestalCacheIndex = 0;
    fNoiseCacheIndex    = 0;
    for(Int_t i = 0; i < fCacheSize; i++) {
      fPedestalCache[i] = 0.;
      fNoiseCache[i]    = 0.;
    }
   }  
   else {
     printf("<DStrip::InitNoiseAndPedestal> strip %d has zero entries for pedestal initialization, ped & noise set to 0 BUT investigate!\n", fStripIndex);
    fInitialPedestal = 0.;
    fPedestal = 0.;
    fNoise = 0. ;
  }
  //----abm8
  Int_t tReadout;
  tReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if(tReadout==10){
    fNoise = 0.01;
  }
  //----ADC
  if((tReadout==12)||(tReadout==13)){
    // if(fNoise < 0.01){    
      fNoise = 1.0;
      fInitialPedestal = 0.;
      fPedestal = 0.;
      // }
  }
  //----ADC
  //---- When only absolute value of charge is available, force pedestal at 0., JB & CD, Sept 2007
  if( tReadout==16 ){
    fInitialPedestal = 0.;
    fPedestal = 0.;
    tVariance = fSumSquareValue/(fSumSquareCount-1);
    fNoise = sqrt( tVariance );
  }
  //----absolute value
}

//______________________________________________________________________________//  


void DStrip::Update() {
  
  // update pedestal (for ref.system)
  UpdatePedestal(fRawValue);

  // update signal
  UpdateSignal(fRawValue);

  // update noise (for ref.system)
  UpdateNoise(fPulseHeight);

}

//______________________________________________________________________________//  

void DStrip::UpdatePedestalAndNoise() 
{
  Float_t tVariance;

//  if ((fStripIndex==1) || (fStripIndex==2)){
//  }
 
  if((fRawValue-fPedestal-fCommonMode)/fNoise<3.){
    fSumSquareValue+=(fRawValue-fPedestal-fCommonMode)*(fRawValue-fPedestal-fCommonMode);
    fSumValue+=(fRawValue-fCommonMode-fPedestal);
    fSumSquareCount++;
    fSumCount++;
    tVariance = (fSumSquareValue-fSumValue*fSumValue/fSumSquareCount )/(fSumSquareCount-1);
    //if( fStripIndex==25) printf("Updte: index=%d, sum=%f, count=%d, var=%f, count=%d\n", fStripIndex, fSumValue, fSumCount, tVariance, fSumSquareCount);
    //----pedestal 
    //fPedestal=fSumValue/fSumCount; //standard version -> BUG ????
    fPedestal= fPedestal * (fSumCount-1.0)/ (fSumCount) + (fRawValue-fCommonMode) / (fSumCount);
    //UpdatePedestal(fRawValue);
    //------
    fNoise = sqrt( tVariance );
  }
//  if((fPlaneNumber==9)&&(fStripIndex<512)){
//  }

// if ((fStripIndex==1) || (fStripIndex==2)){
// }

  //----abm8
  Int_t tReadout;
  tReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if(tReadout==10){
    fNoise = 0.01;
  }
  //----ADC
  if((tReadout==12)||(tReadout==13)){
    // if(fNoise < 0.01){    
      fNoise = 1.0;
      fInitialPedestal = 0.;
      fPedestal = 0.;
      // }
  }
  //----ADC
}


//______________________________________________________________________________
//  

void DStrip::UpdatePedestal(Float_t tRV) {

  Float_t bRV;
  if(fPedestalCacheIndex >= fCacheSize) {

    fPedestalCacheIndex = 0;

    bRV = median(fPedestalCache, fCacheSize); 

    fPedestal = ( (fWeightPedestal - 1. ) * fPedestal + bRV ) / fWeightPedestal;
  }

  fPedestalCache[fPedestalCacheIndex++] = tRV - fCommonMode;
  //----ADC
  Int_t tReadout;
  tReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if((tReadout==12)||(tReadout==13)){
    // if(fNoise < 0.01){    
      fInitialPedestal = 0.;
      fPedestal = 0.;
      // }
  }
  //----ADC
}

//______________________________________________________________________________
//  
void DStrip::UpdateNoise(Float_t tSV) 
{

  Float_t  bSV;

  if(fNoiseCacheIndex >= fCacheSize ) {
    fNoiseCacheIndex = 0;
    bSV = aSignalSupressedValue(fNoiseCache, fCacheSize);
    
    fNoise = sqrt(((fWeightNoise - 1.)*fNoise*fNoise+bSV*bSV)/fWeightNoise ); 
  }

  fNoiseCache[fNoiseCacheIndex++] = tSV;

  //----abm8
  Int_t tReadout;
  tReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if(tReadout==10){
    fNoise = 0.01;
  }
  //----ADC
  if((tReadout==12)||(tReadout==13)){
    //   if(fNoise < 0.01){    
      fNoise = 1.0;
      //  }
  }
  //----ADC
}

//______________________________________________________________________________
//  

void DStrip::UpdateSignal() {
  fPulseHeight = fRawValue - fPedestal - fCommonMode;
}

void DStrip::UpdateSignal(Float_t tRV) {
  fPulseHeight = tRV - fPedestal - fCommonMode;

}
//______________________________________________________________________________

void DStrip::SetNoise(Float_t tNV){
  
  fNoise = tNV;
  
  fPedestalCacheIndex = 0;
  fNoiseCacheIndex    = 0;
  for(Int_t i = 0; i < fCacheSize; i++) {
    fPedestalCache[i] = 0.;
    fNoiseCache[i]    = 0.;
  }
}
//______________________________________________________________________________

void DStrip::SetPedestal(Float_t tPV){
  
  // added JB 2014/01/07
  
  fPedestal = tPV;
  
  fPedestalCacheIndex = 0;
  fNoiseCacheIndex    = 0;
  for(Int_t i = 0; i < fCacheSize; i++) {
    fPedestalCache[i] = 0.;
    fNoiseCache[i]    = 0.;
  }
}


//______________________________________________________________________________//  

void DStrip::SetRawValue(Float_t tRV) {
  
   fRawValue = tRV;
 
}
//______________________________________________________________________________//  

void DStrip::SetRawFrame1Value(Float_t tRV)
 {
    
  fRawFrame1Value = tRV;
}
//______________________________________________________________________________//  

void DStrip::SetRawFrame2Value(Float_t tRV) 
{

  fRawFrame2Value = tRV;
}

//______________________________________________________________________________
//  

Float_t DStrip::median(Float_t* z, const Int_t N)
{
  // median is an estimator for mean 
  // order array ascending or descending, then take value at half size

  Float_t  r;

  if (N==1) {

    r=z[1];

  } else {

    for(Int_t k = 0; k < N-1; k++){
      r      = z[k];
      z[k]   = (r <= z[k+1]) ? r : z[k+1];
      z[k+1] = (r >  z[k+1]) ? r : z[k+1];
    }    
    if (N%2 == 0)
      r = (z[N/2-1] + z[N/2])/2.;
    else
      r = z[(N+1)/2-1]; 

  }
  return r;
}

//______________________________________________________________________________
//  

Float_t DStrip::aSignalSupressedValue(Float_t* pfc, const Int_t N) {
  Int_t   exti;
  Float_t result;

  if (N==1) {
    result = pfc[0];
  }
  else {
    exti = extremumIndex(pfc, N);   
    result = fabs(pfc[0]) < fabs(pfc[exti]) ? pfc[0] : pfc[1];
  }

  return result;

}

//______________________________________________________________________________
//  

Int_t DStrip::extremumIndex(const Float_t* d, 
			    const Int_t N)
{

  //  it's the index of the element with maximum amplitude (+ or -).
  
  Int_t ext;
  ext = N-1;
  
  for (Int_t k = 0; k < N-1; k++)
    ext = (fabs(d[ext]) > fabs(d[k])) ? ext : k;
  return ext;
}

//______________________________________________________________________________
//  
Int_t DStrip::Introduce(DStrip &aStrip)
{
  //  find out about neighbouring strips, fill list with neighbours
  // the strip itself might get introduced to itself!!

  DStrip *sh;
  Int_t k;
  
  // only for debug messages
  Int_t minIndex=200, maxIndex=260;

  if(fDebugStrip>2 && minIndex<fStripIndex && fStripIndex<maxIndex) printf("  DStrip: trying to introduce strip %d to strip %d with distance %f um (<?> %f).\n", fStripIndex, aStrip.GetStripIndex(), Distance(aStrip), fBound);
  if ((Distance(aStrip) <= fBound)  // check wheather strip in Neighbourhood 
      && (fNeighbourCount < fNeighbourCountMaximum)) {   // and wheather limit reached
    fNeighbourList[fNeighbourCount] = &aStrip;           // assign neighbour 
    
    
    // For STRIPS detector
    // order neighbours with distance
    // so that neighbour[0] is seed itself
    // and higher indices are adjacent with increasing distance

    if ( fc->GetPlanePar(fPlaneNumber).AnalysisMode < 2 ) {
      k = fNeighbourCount;                      
      while (k > 0 && Distance(aStrip) < Distance(*fNeighbourList[k-1]) ) {
	sh                  = fNeighbourList[k-1];
	fNeighbourList[k-1] = &aStrip;
	fNeighbourList[k]   = sh;
	k--;
      }
    }
    
    // For PIXELS detector
    // do nothing
   
  
    fNeighbourCount++;
    if(fDebugStrip>2 && minIndex<fStripIndex && fStripIndex<maxIndex) printf("  DStrip: strip %d has %d neighbours in %f um.\n", fStripIndex, fNeighbourCount, fBound);
    return 0;
  }

  // In case the tested strip can not be in the neighbourhood return 1
  else {
    return 1;
  }


}

//______________________________________________________________________________
//  
Float_t DStrip::Distance(DStrip &aStrip)
{
  return Distance(aStrip.GetPosition());
}

//______________________________________________________________________________
//  
Float_t DStrip::Distance(const DR3& aPosition)
{
  DR3 result(fPosition);
  result -= aPosition; 
  return result.Length();
}

//______________________________________________________________________________
//  
Float_t DStrip::DistanceU(DStrip &aStrip)
{
  return DistanceU(aStrip.GetPosition());
}

//______________________________________________________________________________
//  
Float_t DStrip::DistanceU(const DR3& aPosition)
{
  DR3 result(fPosition);
  result -= aPosition; 
  return result(0);
}

//______________________________________________________________________________
//  
Float_t DStrip::DistanceV(DStrip &aStrip)
{
  return DistanceV(aStrip.GetPosition());
}

//______________________________________________________________________________
//  
Float_t DStrip::DistanceV(const DR3& aPosition)
{
  DR3 result(fPosition);
  result -= aPosition; 
  return result(1);
}

//______________________________________________________________________________
//  
Bool_t DStrip::KillNoise()
{
  // discard a pixel signal in MIMOSA if it's a standalone one
  //Bool_t answer;

  Float_t NeighbourNoise = 0.;
  Float_t NeighbourCharge= 0.;
  if ( fc->GetPlanePar(fPlaneNumber).AnalysisMode == 2 ) {
    for (Int_t i=1; i< fNeighbourCount; i++){
      if(i!=fStripIndex){
	NeighbourCharge += fNeighbourList[i]->GetPulseHeight();
	NeighbourNoise += fNeighbourList[i]->GetNoise()*
	                  fNeighbourList[i]->GetNoise();
      }
    }
    NeighbourNoise=sqrt(NeighbourNoise);
    //Float_t s2n = NeighbourCharge/NeighbourNoise;
        //if(s2n<3) answer=kTRUE;
  }                     
 return 0; // end of method return 
}

