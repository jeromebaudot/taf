// @(#)maf/dtools:$Name:  $:$Id: GlobalTools.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author   :  Dirk Meier   98/03/02

#ifndef _DGlobalTools_included_
#define _DGlobalTools_included_


////////////////////////////////////////////////////////////////
//                                                            //
//         Collection of Global Tools                         //
//                                                            //
//////////////////////////////////////////////////////////////// 


#include <math.h>
#include <map>
#include "TSystem.h"
#include "TString.h"
#include "DR3.h"
#include "Riostream.h"
#include "TMath.h"

using namespace std;

// this important to tell about ClassDef()
#ifndef ROOT_Rtypes
#include "Rtypes.h" 
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//////////////////
// COLORS FOR PRINTF
//////////////////
#define PRINTF_RED 	  printf("\033[31m");
#define PRINTF_RED_HL 	  printf("\033[31;01m");
#define PRINTF_GREEN 	  printf("\033[32m");
#define PRINTF_GREEN_HL	  printf("\033[32;01m");
#define PRINTF_BLUE 	  printf("\033[34m");
#define PRINTF_BLUE_HL 	  printf("\033[34;01m");
#define PRINTF_YELLOW 	  printf("\033[33m");
#define PRINTF_YELLOW_HL  printf("\033[33;01m");
#define PRINTF_DEFAULT	  printf("\033[00m");



// extern "C" Double_t denlan_(Float_t *x); // JB for Inline compilation
// here add more CERNLIB functions

class DGlobalTools {
  
 private:
  static DGlobalTools* fgInstance;
  
  float        poly(float aArg ,float* aParameter, int aOrder);
  float        fParameter[20];
  int          fNparameter;

  double       fProbabilityTest[460];   // test variables
  double       fProbabilityPhiZ[460];   // probability integral values


  float        fChi2[40][16];            // array of chi2 values at degree of freedom and probability 
  float        fChi2Probability[16];    // array of probabilities 

  double       gPI;
  double       gEULER;
  float	       gSpeedOfLight;

  // QL 2016/05/26 Multiple scattering info
  std::map<string,double> fMass;   // in GeV/c2
  std::map<string,int>    fCharge; // in units of electron charge
  std::map<string,double> fX0; // um
  
  
 public:
  DGlobalTools();
  virtual     ~DGlobalTools(){;}
  void         Dummy();
  double       GetPI() { return gPI; }
  double       GetEULER() { return gEULER; }
  float        GetSpeedOfLight() { return gSpeedOfLight; }
  float	       gMyAge(); // does not work
  float	       gPoly(float aArg ,float* aParameter, int aOrder); // does not work
  void         Swap(Float_t *a, Float_t *b);                          // swaps *a with *b
  void         OrderIndexDes(Int_t *aIndex, Float_t *aArg, Int_t aN); // order aIndex that aArg descending
  void         OrderIndexAsc(Int_t *aIndex, Float_t *aArg, Int_t aN); // order aIndex that aArg ascending

  float        Poly(float* pArg, float* pParameter);
  void         SetOrder(int aOrder)            { fNparameter = aOrder+1; }
  void         SetParameter(float* aParameter, int aNumber);
  float       *GetParameter()                  { return fParameter; }

  double       ProbabilityIntegral(double aTestVariable);

  float        P19(float aArg) { return poly(aArg, fParameter, 19); }
  float        P18(float aArg) { return poly(aArg, fParameter, 18); }
  float        P17(float aArg) { return poly(aArg, fParameter, 17); }
  float        P16(float aArg) { return poly(aArg, fParameter, 16); }
  float        P15(float aArg) { return poly(aArg, fParameter, 15); }
  float        P14(float aArg) { return poly(aArg, fParameter, 14); }
  float        P13(float aArg) { return poly(aArg, fParameter, 13); }
  float        P12(float aArg) { return poly(aArg, fParameter, 12); }
  float        P11(float aArg) { return poly(aArg, fParameter, 11); } 
  float        P10(float aArg) { return poly(aArg, fParameter, 10); }
  float        P9(float aArg)  { return poly(aArg, fParameter, 9); }
  float        P8(float aArg)  { return poly(aArg, fParameter, 8); }
  float        P7(float aArg)  { return poly(aArg, fParameter, 7); }
  float        P6(float aArg)  { return poly(aArg, fParameter, 6); }
  float        P5(float aArg)  { return poly(aArg, fParameter, 5); }
  float        P4(float aArg)  { return poly(aArg, fParameter, 4); }
  float        P3(float aArg)  { return poly(aArg, fParameter, 3); }
  float        P2(float aArg)  { return poly(aArg, fParameter, 2); }
  float        P1(float aArg)  { return poly(aArg, fParameter, 1); }
  float        P0(float aArg)  { return poly(aArg, fParameter, 0); }

  void         Confidence(int tGoodN, int tTotalN, float tConfidenceLimit, 
			  float& tUpperError, float& tLowerError);

  double       Chi2ToProbability(int tDegreeOfFreedom, double tChiSquare);  // converts a chi2 at given degree of fredoom into a probabilty 
  double       CLof7LegendrePol( double *x, double *coeff); // JB 2014/04/10
  //double       TrackMultiplicity( int nPlanes, double *efficiencyPlane, double *trackMultiplicity); // JB 2014/11/15
  void         TrackMultiplicity( Int_t nPlanes, Double_t *efficiencyPlane, Double_t *trackMultiplicity); // JB 2014/11/15
  
  void         ComputeStripPosition( Int_t mapping, Int_t col, Int_t lin, Double_t &u, Double_t &v, Double_t &w, Int_t stripsNu, Int_t stripsNv, Double_t pitchU, Double_t pitchV, Double_t pitchW); // JB 2015/04/01
  void         ComputeStripPosition_UVToColRow( Int_t mapping, Double_t u, Double_t v, double &col, double &lin, Int_t stripsNu, Int_t stripsNv, Double_t pitchU, Double_t pitchV);  // JB 2015/04/01

  char*        LocalizeDirName(const char *inputString); // JB 2011/07/07
  void         LocalizeDirName( TString *inputString); // JB 2011/07/07
  
  double       BuildVertex( double *point11, double *point12, double *point21, double *point22, double *vertex); // JB 2011/07/26
  Double_t     BuildVertex( DR3 &pointA1, DR3 &pointA2, DR3 &pointB1, DR3 &pointB2, DR3 &intersection); // VR 2014/06/29 
  
  bool        (*VetoPixel)( int chipNb, int row, int col); // JB 2012/03/11
  void         SetVetoPixel( int noiseRun);

  //Functions used for multiple scattering, both simulation and track fitting
  double     scatteringAngle(string particle = "proton",
			     double momentum  = 120.0,
			     string material  = "silicon",
			     double thickness = 50.0,
			     bool verbose     = true);  //AP 2015/03/11
  double     Getb(string particle = "proton",
		  string material = "silicon",
		  double momentum = 120.0,
		  double thickness = 50.0); //AP 2015/04/24
  double     GetBfromb(double b); //AP 2015/04/24
  double     Getf0(double Theta); //AP 2015/04/24
  void       Getf1Andf2(double Theta,
			double& f1,
			double& f2); //AP 2015/04/24
  double     GetDistribution(double Theta,
			     double B); //AP 2015/04/24
  double     GetMass(string particle); // QL 2016/05/26
  int        GetCharge(string particle); // QL 2016/05/26
  double     GetX0(string material); // QL 2016/05/26
  
  int        RoundOff(double a);
  
  double fpeaks(double *x, double *par, int npeaks);
  double CBfunction(double x, double mean, double sigma, double alpha, double n);
  double BifurcatedCBfunction(double x, double mean, double sigmaL, double sigmaR, double alpha, double n);
  double DoublePeakCBFitFunction55Fe(double x,
                                     double mean1,
                                     double mean2,
                                     double sigma1,
                                     double sigma2,
                                     double alpha1,
                                     double alpha2,
                                     double n1,
                                     double n2,
                                     double N1,
                                     double N2);
  double DoublePeakBifurcatedCBFitFunction55Fe(double x,
                                               double mean1,
                                               double mean2,
                                               double sigmaL1,
                                               double sigmaL2,
                                               double sigmaR1,
                                               double sigmaR2,
                                               double alpha1,
                                               double alpha2,
                                               double n1,
                                               double n2,
                                               double N1,
                                               double N2);
  double fpeaksCB55Fe(double *x, double *par);
  double fpeaksBifurcatedCB55Fe(double *x, double *par);
  

  static  DGlobalTools*& Instance() { 
    if (!fgInstance)  fgInstance = new  DGlobalTools(); 
    return fgInstance; 
  }
  
  ClassDef(DGlobalTools,1)       // Collection of global tools
    
};

inline double DGlobalTools::GetMass(string particle){
  std::map<string, double>::iterator it = fMass.find(particle);
  if(it != fMass.end())
       return it->second;
  else
       return -1;
}

inline int DGlobalTools::GetCharge(string particle){
  std::map<string, int>::iterator it = fCharge.find(particle);
  if(it != fCharge.end())
       return it->second;
  else
       return 0;
}

inline double DGlobalTools::GetX0(string material){
  std::map<string, double>::iterator it = fX0.find(material);
  if(it != fX0.end())
       return it->second;
  else
       return 0.;
}

#endif
