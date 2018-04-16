//**********************************************************************************//
//Autor: Daniel Cuesta
//Purpose of Class :
//    Holds all the modules needed to perform Helix fitting
//          - GetHelixParameter            : Perform Helix fitting with Hits give in Arguments
//            Sub modules of GetHelixParameter
//            -- GetResidual               : Give residual for the Chisquare minimization
//            -- BuiltCovarianceMatrix     : Built Covariance matrix
//            -- GetIntersectionHelixPlane : Get intersection of the helix with the sensor plane
//                                           for a given set of parameter
//***********************************************************************************//
//                      /!\ /!\ /!\ WORK IN PROGRESS/!\/!\ /!\*
//**********************************************************************************//

#include "DHelixFitter.h"
#include <TVector.h>
#include <TVector3.h>
#include <TMatrixD.h>
#include <TMatrix.h>
DHelixFitter* DHelixFitter::fgInstance = 0;
ClassImp(DHelixFitter)

DHelixFitter::DHelixFitter(DTracker *Tracker)
{
  tTracker=Tracker;
  fgInstance=this;
return;
}
DHelixFitter::~DHelixFitter(){
  return;
}
//===========================================================================================================
TVector3 DHelixFitter::GetHelixParameter(vector<TVector3> ListOfHits){
  // DHelixFitter *aHelixFitter = new DHelixFitter();
  std::cout << "GetHelixParameter" << '\n';
  aHelix = new DHelix();
  aHelix->SetMagneticField(1.5);
  ListOfHitsToFitted_1 = ListOfHits[0];
  ListOfHitsToFitted_2 = ListOfHits[1];
  SensorId_1           = ListOfHits[2].X();
  SensorId_2           = ListOfHits[2].Y();
  double c = 3e8;
  alpha    = 1./(0.3*1.5);
  dphi     = (TMath::Pi()/1000.);
  eps      = 1e-1;
  phi0     = 0;
  RootFind = 1e6;

  double Pi=TMath::Pi();

  TMinuit *ptMinuit = new TMinuit(5);  //initialize TMinuit
  ptMinuit->SetPrintLevel(0);
  // set the user function that calculates chi_square (the value to minimize)
  ptMinuit->SetFCN(FCNHelix_ChiSquare);

  Double_t arglist[10];
  Int_t ierflg = 0;

  arglist[0] = 1;
  ptMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
  double m_phi0=0.;
  double m_drho=0.;
  double m_dz=0.;
  // Set starting values and step sizes for parameters
  static Double_t vstart[5] = {53.22,m_phi0,0.36,m_drho,m_dz};
  // static Double_t vstart[5] = {53.,m_phi0,TMath::Tan((1-0.85)*Pi/2),m_drho,m_dz};
  static Double_t step[5] = {1.,0.01,TMath::Tan(0.01*Pi/2.),0.001,0.1};
  ptMinuit->mnparm(0, "k", vstart[0], step[0],10.,100.,ierflg);
  ptMinuit->mnparm(1, "phi0", vstart[1], step[1],0.,4*Pi,ierflg);
  ptMinuit->mnparm(2, "lambda", vstart[2], step[2],0.2,TMath::Tan(0.95*Pi/2),ierflg);
  // ptMinuit->mnparm(2, "lambda", vstart[2], step[2],TMath::Tan((1-0.99)*Pi/2),TMath::Tan((1-0.7)*Pi/2),ierflg);
  ptMinuit->mnparm(3, "drho", vstart[3], step[3], -2e-2,2e-2,ierflg);
  ptMinuit->mnparm(4, "dz", vstart[4], step[4], -0.4,0.4,ierflg);
  // ptMinuit->FixParameter(0);
  // ptMinuit->FixParameter(1);
  // ptMinuit->FixParameter(2);
  // ptMinuit->FixParameter(3);
  // ptMinuit->FixParameter(4);

  std::cout << "INSIDE DHelixFitter" << '\n';
  std::cout << "Position 1 X = "<< ListOfHits[0].X() << "[mm]"
            << ", Y = "<< ListOfHits[0].Y() << "[mm]"
            << ", Z = "<< ListOfHits[0].Z() << "[mm]" << '\n';
  std::cout << "Position 2 X = "<< ListOfHits[1].X() << "[mm]"
            << ", Y = "<< ListOfHits[1].Y() << "[mm]"
            << ", Z = "<< ListOfHits[1].Z() << "[mm]" << '\n';

  std::cout << "Sensor Id 1 = "<<ListOfHits[2].X() <<" Sensor Id 2 = "<< ListOfHits[2].Y() << '\n';
  // Now ready for minimization step
  arglist[0] = 500  ;
  arglist[1] = 0.1;
  // ptMinuit->SetMaxIteration(500);
  ptMinuit->mnexcm("MIGRAD", arglist ,1,ierflg);
  std::cout << "AFTER FIT" << '\n';
  // if you want to access to these parameters, use:
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  ptMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);

  cout << "\n";
  cout << " Minimum chi square = " << amin << "\n";
  // std::cout << "Probability = " << TMath::Prob(amin, 5)<<'\n';
  cout << " Estimated vert. distance to min. = " << edm << "\n";
  cout << " Number of variable parameters = " << nvpar << "\n";
  cout << " Highest number of parameters defined by user = " << nparx << "\n";
  cout << " Status of covariance matrix = " << icstat << "\n";

  double fParamValbis;
  double fParamErrbis;
  ptMinuit->GetParameter(0,fParamValbis,fParamErrbis);
  double k =fParamValbis;
  cout << "K =" << fParamValbis << "\n";
  std::cout << "Momemtum[GeV] = "<<1./k << '\n';
  TVector3 HelixParameter(1./k,amin,0.);

  return HelixParameter;
}
//===========================================================================================================
void FCNHelix_ChiSquare(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
  DHelixFitter::Instance()->Helix_ChiSquare(npar, gin, f, par , iflag);
  return;
}
//===========================================================================================================
void DHelixFitter::Helix_ChiSquare(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
  chisquare = 0.;
  TMatrixD CovarianceMatrix;
  TMatrixD Residual;
  Residual.ResizeTo(3,1);
  TMatrixD Residual_T;
  Residual_T.ResizeTo(1,3);
  TMatrixD StepMatrix;
  StepMatrix.ResizeTo(1,3);
  TMatrixD ChisquareMatrix;
  ChisquareMatrix.ResizeTo(3,3);
  int NbFittedPoints=3;
  for(int i=0;i<NbFittedPoints;i++){
    std::cout << "--------oooo00OO00oooo-----oooo00OO00oooo" << '\n';
    std::cout << "FITTED n°"<<i << '\n';
    std::cout << "--------oooo00OO00oooo-----oooo00OO00oooo" << '\n';

    Residual=GetResidual(par,i);
    std::cout << "Residual Matrix Print " << '\n';
    Residual.Print();
    if(Residual(0,0)==-999){
      // std::cout << "BAD" << '\n';
      chisquare += 1e6;
    }else{
      Residual_T.Transpose(Residual);
      Residual_T.Print();
      BuiltCovarianceMatrix(CovarianceMatrix,i);
      std::cout << "COVARIANCE MATRIX Print " << '\n';
      CovarianceMatrix.Print();
      StepMatrix = Residual_T*CovarianceMatrix;
      // ChisquareMatrix = StepMatrix*Residual;
      (StepMatrix*Residual).Print();
      std::cout << "chisquare for Fit point n°"<<i<<" = "<<(StepMatrix*Residual)(0,0) << '\n';
      chisquare += (StepMatrix*Residual)(0,0);
    }
  }
  f=chisquare;
  return;
}
//===========================================================================================================
TMatrixD DHelixFitter::GetResidual(Double_t *par, int MeasurmentIndex){
  std::cout << "--*-----*-**----- par[0]:k = "<< par[0] << '\n';
  std::cout << "--*-----*-**----- par[1]:phi0 = "<< par[1] << '\n';
  std::cout << "--*-----*-**----- par[2]:TanLambda = "<< par[2] << '\n';
  std::cout << "--*-----*-**----- par[3]:d0 = "<< par[3] << '\n';
  std::cout << "--*-----*-**----- par[4]:z0 = "<< par[4] << '\n';
  aHelix->SetAllParameters(par);
  // std::cout << "Test Get k = "<< 1./aHelix->GetTransverseMomentum() << '\n';
  // std::cout << "Test Get Phi0 = "<<   aHelix->GetPhi0()<< '\n';
  // std::cout << "C'est SetParameter" << '\n';
  int SensorId=0;
  double phi = 0;


  TMatrix  W;

  if(MeasurmentIndex == 0){
    x = 0.0;
    y = 0.0;
    z = 0.0;
  }
  if(MeasurmentIndex == 1){

    x        = ListOfHitsToFitted_1.X();
    y        = ListOfHitsToFitted_1.Y();
    z        = ListOfHitsToFitted_1.Z();
    SensorId = SensorId_1;
  }
  if(MeasurmentIndex == 2){
    x        = ListOfHitsToFitted_2.X();
    y        = ListOfHitsToFitted_2.Y();
    z        = ListOfHitsToFitted_2.Z();
    SensorId = SensorId_2;
  }
  Result.ResizeTo(3,1);
  if(MeasurmentIndex==0){
    // std::cout << "Test GetHelixPosition" << '\n';
    // std::cout << "Test Get Phi0" <<aHelix->GetPhi0()<< '\n';
    DR3 HelixPosXYZFitted = aHelix->GetHelixPosition(aHelix->GetPhi0());//careful now is meters
    // std::cout << "C'est pas Get HelixPosition " << '\n';
    Result(0,0) = x-(HelixPosXYZFitted)(0)/1000;
    Result(1,0) = y-(HelixPosXYZFitted)(1)/1000;
    Result(2,0) = z-(HelixPosXYZFitted)(2)/1000;
  }else{
    aPlane = tTracker->GetPlane(SensorId+1);
    Dprec  = aPlane->GetPrecAlignment();
    phi = GetIntersectionHelixPlane(aPlane,aHelix);
    if(phi<0) phi = 2*TMath::Pi()-abs(phi);
    std::cout << "Phi Inside GetResidual = "<<phi << '\n';
    // if(phi == -999){
    //   Result(0,0) = -999;
    //   Result(1,0) = -999;
    //   Result(2,0) = -999;
    // }else{

      DR3 HelixPosXYZFitted = aHelix->GetHelixPosition(phi);//careful now is meters
      (HelixPosXYZFitted)(0)=(HelixPosXYZFitted)(0)*1e6;
      (HelixPosXYZFitted)(1)=(HelixPosXYZFitted)(1)*1e6;
      (HelixPosXYZFitted)(2)=(HelixPosXYZFitted)(2)*1e6;//Now in um
      // std::cout << "--------oooo00OO00oooo-----oooo00OO00oooo" << '\n';
      // std::cout << "-----oo0OO0oo----POSITION MODELIZE-----oo0OO0oo----" << '\n';
      // std::cout << "SensorId = "<< SensorId << '\n';
      // std::cout << "Phi = "<<phi << '\n';
      // std::cout << "PosXYZ -> X = "<< (HelixPosXYZFitted)(0)
      //           << " PosXYZ -> Y = "<< (HelixPosXYZFitted)(1)
      //           << " PosXYZ -> Z = "<< (HelixPosXYZFitted)(2)<< '\n';

      DR3 HelixPosXYZMeasured(x*1000,y*1000,z*1000);//in um
      // DR3 HelixPosXYZMeasured(x*1000.,y*1000.,z*1000);//in micrometers
      // DR3 HelixPosXYZMeasured(x,y,z);//in micrometers

      //   std::cout << "-----oo0OO0oo----POSITION MEASURED-----oo0OO0oo----" << '\n';
      // std::cout << "PosXYZ -> X = "<< (HelixPosXYZMeasured)(0)
      //           << " PosXYZ -> Y = "<< (HelixPosXYZMeasured)(1)
      //           << " PosXYZ -> Z = "<< (HelixPosXYZMeasured)(2)<< '\n';
      DR3 HelixPosUVWFitted   = Dprec->TransformHitToPlane(HelixPosXYZFitted);//Return micrometers
      DR3 HelixPosUVWMeasured = Dprec->TransformHitToPlane(HelixPosXYZMeasured);//Return micrometers
      std::cout << "-----oo0OO0oo----LOCAL POSITION MODELIZE-----oo0OO0oo----" << '\n';
      std::cout << "PosUVW -> U = "<< (HelixPosUVWFitted)(0)
                << " PosUVW -> V = "<< (HelixPosUVWFitted)(1)
                << " PosUVW -> W = "<< (HelixPosUVWFitted)(2)<< '\n';
      std::cout << "-----oo0OO0oo----LOCAL POSITION MEASURED-----oo0OO0oo----" << '\n';
      std::cout << "PosUVW -> U = "<< (HelixPosUVWMeasured)(0)
                << " PosUVW -> V = "<< (HelixPosUVWMeasured)(1)
                << " PosUVW -> W = "<< (HelixPosUVWMeasured)(2)<< '\n';
      Result(0,0) = ((HelixPosUVWMeasured)(0)-(HelixPosUVWFitted)(0))/1000;//Result in millimeters
      Result(1,0) = ((HelixPosUVWMeasured)(1)-(HelixPosUVWFitted)(1))/1000;
      Result(2,0) = ((HelixPosUVWMeasured)(2)-(HelixPosUVWFitted)(2))/1000;
    // }
  }
  Result.Print();
  return Result;
}
//<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>>
double DHelixFitter::GetIntersectionHelixPlane(DPlane *aPlane, DHelix *aHelix)
{
  double PositionLimit = .1;
  double DeltaPhiLimit = 0.001;
  double phi  = 0.;
  Dprec       = aPlane->GetPrecAlignment();
  bool   HaveSolution = false;
  int RootFinderCounter = 0.;
  int RootFinderCounterLimit = 10;
  Dprec  = aPlane->GetPrecAlignment();
  while((HaveSolution == false) && RootFinderCounter<RootFinderCounterLimit){
    HelixPosXYZ      = aHelix->GetHelixPosition(phi);//Return meters
    (HelixPosXYZ)(0) = (HelixPosXYZ)(0)*1e6;
    (HelixPosXYZ)(1) = (HelixPosXYZ)(1)*1e6;
    (HelixPosXYZ)(2) = (HelixPosXYZ)(2)*1e6;
    HelixPosUVW      = Dprec->TransformHitToPlane(HelixPosXYZ);//Return micrometers
    double W         = (HelixPosUVW)(2)/1000;// in millimeters
    if(fabs(W)>PositionLimit){
      double Wprime  = GetWprime(phi,aHelix,aPlane);
      if(Wprime == 0) continue;
      phi -= W/(Wprime);
      RootFinderCounter++;
    }else{
    HaveSolution=true;
    std::cout << "Solution Founded" << '\n';
    // std::cout << "Phi founded = "<<phi << '\n';
    }
  }
  // if(!HaveSolution){
  //    phi = -999;
  //   //  std::cout << "Solution don't found" << '\n';
  //  }
  //  std::cout << "Phi returned ="<<phi << '\n';
  return phi;
}
//===========================================================================================================
double  DHelixFitter::GetWprime(double phi,DHelix *aHelix,DPlane *aPlane){
  double phi0      = aHelix->GetPhi0();
  double Alpha_k   = -1*aHelix->GetTransverseMomentum()/(0.3*aHelix->GetMagneticField());
  // double Alpha_k   = aHelix->GetTransverseMomentum()/(3e8*aHelix->GetMagneticField());
  double TanLambda = aHelix->GetTanLambda();

  DR3 Wprime(Alpha_k*sin(phi0+phi)*1e6,-Alpha_k*cos(phi0+phi)*1e6,-Alpha_k*TanLambda*1e6);
  // std::cout << "composante 1 = "<< (Wprime)(0) << '\n';
  // std::cout << "composante 2 = "<< (Wprime)(1) << '\n';
  // std::cout << "composante 3 = "<< (Wprime)(2) << '\n';
  Dprec  = aPlane->GetPrecAlignment();
  DR3 WprimeLocalFrame=Dprec->TransformHitToPlane(Wprime);//GOOD ONE// Returns micrometers
  // DR3 WprimeLocalFrame=Dprec->TransformHitToTracker(Wprime);//Returns micrometers

  // std::cout << "LOCAL" << '\n';
  // std::cout << "composante 1 = "<< (WprimeLocalFrame)(0) << '\n';
  // std::cout << "composante 2 = "<< (WprimeLocalFrame)(1) << '\n';
  // std::cout << "composante 3 = "<< (WprimeLocalFrame)(2) << '\n';
  return (WprimeLocalFrame)(2)/1000; //return millimeters
}
//===========================================================================================================
void DHelixFitter::BuiltCovarianceMatrix(TMatrixD &W,int MeasurmentIndex){
  // x/1000 test in meters
  SigmaPos_X = 2e-2;
  SigmaPos_Y = 1e-4;
  SigmaPos_Z = 4e-1;
//
  SigmaPos_U = 3e-3;
  SigmaPos_V = 3e-3;
  SigmaPos_W = 3e-3;

  if(MeasurmentIndex == 0){
    SigmaPos_1 = SigmaPos_X;
    SigmaPos_2 = SigmaPos_Y;
    SigmaPos_3 = SigmaPos_Z;
  }
  if(MeasurmentIndex == 1){
    SigmaPos_1 = SigmaPos_U;
    SigmaPos_2 = SigmaPos_V;
    SigmaPos_3 = SigmaPos_W;
  }
  if(MeasurmentIndex == 2){
    SigmaPos_1 = SigmaPos_U;
    SigmaPos_2 = SigmaPos_V;
    SigmaPos_3 = SigmaPos_W;
  }
  W.ResizeTo(3,3);

  W(0,0) =  1./(pow(SigmaPos_1,2));  W(0,1) =  0.0;                     W(0,2) =  0.0;
  W(1,0) =  0.0;                     W(1,1) =  1./(pow(SigmaPos_2,2));  W(1,2) =  0.0;
  W(2,0) =  0.0;                     W(2,1) = 0.0;                      W(2,2) =  1./(pow(SigmaPos_3,2));
  // W(0,0) =  1./SigmaPos_1;  W(0,1) =  0.0;                     W(0,2) =  0.0;
  // W(1,0) =  0.0;                     W(1,1) = 1./SigmaPos_2;  W(1,2) =  0.0;
  // W(2,0) =  0.0;                     W(2,1) = 0.0;                      W(2,2) = 1./SigmaPos_3;

  return;
}
//===========================================================================================================
TVector3 DHelixFitter::GetCircleParameters(TVector3 r1,TVector3 r2){
  double afit;
  double bfit;
  double rfit;
  ptMinuit = new TMinuit(9);
  ptMinuit->SetFCN(calc_chi_square_Circle);
  ptMinuit->SetPrintLevel(-1);

  Double_t Arglist[10];
  Int_t Ierflg = 0;

  Arglist[0] = 1.;
  ptMinuit->mnexcm("SET ERR", Arglist ,1,Ierflg);

  // Set starting values and step sizes for parameters
  static Double_t Vstart[9] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
  static Double_t Step[9] = {10,10,10,0.001,0.001,0.001,0.001,0.001,0.001};

  ptMinuit->mnparm(0, "R", Vstart[0], Step[0],-1e5,1e5,Ierflg);
  ptMinuit->mnparm(1, "a", Vstart[1], Step[1],-1e5,1e5,Ierflg);
  ptMinuit->mnparm(2, "b", Vstart[2], Step[2],-1e5,1e5,Ierflg);

  ptMinuit->mnparm(3, "X1", 0.0, Step[3],-1.,1.,Ierflg);
  ptMinuit->mnparm(4, "Y1", 0.0, Step[4],-1.,1.,Ierflg);

  ptMinuit->mnparm(5, "X2", r1.X(), Step[5],-1.,1.,Ierflg);
  ptMinuit->mnparm(6, "Y2", r1.Y(), Step[6],-1.,1.,Ierflg);

  ptMinuit->mnparm(7, "X3", r2.X(), Step[7],-1.,1.,Ierflg);
  ptMinuit->mnparm(8, "Y3", r2.Y(), Step[8],-1.,1.,Ierflg);

  for(int i = 3; i<9;i++){
    ptMinuit->FixParameter(i);
  }
  Arglist[0] = 500; //Max calls
  Arglist[1] = 1.; //TOLERANCE
  ptMinuit->mnexcm("MIGRAD", Arglist ,2,Ierflg);

  // Print results
  cout << "\nPrint results from minuit\n";
  double fParamValbis;
  double fParamErrbis;
  ptMinuit->GetParameter(0,fParamValbis,fParamErrbis);
  rfit=fParamValbis;
  cout << "R=" << fParamValbis << "\n";
  ptMinuit->GetParameter(1,fParamValbis,fParamErrbis);
  afit=fParamValbis;
  cout << "a =" << fParamValbis << "\n";
  ptMinuit->GetParameter(2,fParamValbis,fParamErrbis);
  bfit=fParamValbis;
  cout << "b =" << fParamValbis << "\n";
  TVector3 CircleParam(afit,bfit,rfit);

  return CircleParam;
}
//<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>>
Double_t Circle_Fit(double x,double y,double *par)
{
 double value=par[0]-sqrt( pow(x-par[1],2)+pow(y-par[2],2) );
 return value;
}
//<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>>
void calc_chi_square_Circle(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{

  double chisq = 0;
  double x,y,z;
  double errorz=0.001;
  int iNum=3;
  for (int i=0;i<iNum; i++) {
    if(i == 0){
      x=par[3];
      y=par[4];
    }
    if(i == 1){
      x=par[5];
      y=par[6];
    }
    if(i == 2){
      x=par[7];
      y=par[8];
    }
    // chi square is the quadratic sum of the distance from the point to the function weighted by its error
    double delta  = Circle_Fit(x,y,par)/errorz;
    chisq += delta*delta;
  }
  f = chisq;
  return;
}
