#ifndef _DHelixFitter_included_
#define _DHelixFitter_included_

// #include <THStack.h>
// #include <TH3.h>
#include "TLegend.h"
#include "TObject.h"
#include <TROOT.h>
#include <TMinuit.h>
#include <TRandom.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH2.h>
#include <TArc.h>
#include <TEllipse.h>
#include <TVector.h>
#include <TVector3.h>
#include <TMatrix.h>
#include <TMatrixD.h>

//TAF Include
// #include "DTracker.h"
#include "DTracker.h"
#include "DLadder.h"
#include "DPlane.h"
#include "DAlign.h"
#include "DHit.h"
#include "DHelix.h"
#include "DR3.h"
// #include "DBeaster.h"

using namespace std;
class DHelix;
class DHelixFitter: public TObject {
private:
static DHelixFitter* fgInstance;
public:
  DHelixFitter (DTracker *Tracker);
  DLadder          *aLadder;
  DPlane           *aPlane;
  DHit             *aHit;
  DTracker         *tTracker;
  DEventMC         *MCInfoHolder;
  DSession         *fSession;
  DPrecAlign       *Dprec;
  DHelix           *aHelix;
  TMatrix          Result;
  TVector3         ListOfHitsToFitted_1;
  int              SensorId_1;
  TVector3         ListOfHitsToFitted_2;
  int              SensorId_2;
  double           SigmaPos_X;
  double           SigmaPos_Y;
  double           SigmaPos_Z;
  double           SigmaPos_U;
  double           SigmaPos_V;
  double           SigmaPos_W;
  double           SigmaPos_1;
  double           SigmaPos_2;
  double           SigmaPos_3;
  DR3              HelixPosXYZ;
  DR3              HelixPosUVW;
  double x;
  double y;
  double z;
  double phi;
  double xfit;
  double yfit;
  double zfit;
  double alpha;
  double dphi;
  double eps;
  double phi0;
  double RootFind;
  double chisquare;

  int NbFittedPoints;
  TMinuit *ptMinuit;
  int iNum;

  virtual  ~DHelixFitter ();
  virtual TVector3 GetHelixParameter(vector<TVector3> ListOfHits);
  virtual TVector3 GetCircleParameters(TVector3 r1, TVector3 r2);
  virtual void     Helix_ChiSquare(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
  virtual TMatrixD GetResidual(Double_t *par,int MeasurmentIndex );
  virtual DTracker *GetTracker(){return tTracker;};
  virtual void     BuiltCovarianceMatrix(TMatrixD &W,int MeasurmentIndex);
  // virtual TMatrixD HelixEquation(Double_t *par,int MeasurmentIndex );
  virtual double   GetWprime(double phi,DHelix *aHelix,DPlane *aPlane);
  static  DHelixFitter*& Instance() {

    return fgInstance;
  }
  virtual double   GetIntersectionHelixPlane(DPlane *aPlane, DHelix *aHelix);
ClassDef(DHelixFitter,1)
};

void     calc_chi_square_Circle(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void     FCNHelix_ChiSquare(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
Double_t Circle_Fit(double x,double y,double *par);

#endif
