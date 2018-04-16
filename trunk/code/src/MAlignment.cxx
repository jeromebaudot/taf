////////////////////////////////////////////////////////////
//                                                        //
// Class Description of MAlignment                        //
//                                                        //   
////////////////////////////////////////////////////////////


#include "MAlignment.h"
#include "DTrack.h"
//#include "DCluster.h"
#include "DHit.h"
#include "DLine.h"
#include "DPlane.h"
#include "DR3.h"
#include "MMillepede.h"

#include "TMath.h"
#include "TMatrixDSym.h"
#include "TSystem.h"

/// \cond CLASSIMP
ClassImp(MAlignment)
/// \endcond

//_________________________________________________________________
MAlignment::MAlignment() 
 : TObject(),
   fStartFac(16.), 
   fResCutInitial(100.), 
   fResCut(100.),
   fMillepede(0x0),
   fTrack(0),
   fHit(0),
   fNGlobal(0),
   fNLocal(0),
   fNStdDev(3),
   fPhi(0.),
   fCosPhi(1.),
   fSinPhi(0.),
   fDebugLevel(0)
{
   /// Default constructor, setting define alignment parameters
   fSigma[0] = 10;
   fSigma[1] = 10;
   
   fDoF[0] = kTRUE;  fDoF[1] = kTRUE;  fDoF[2] = kTRUE;
   fAllowVar[0] = 500;  fAllowVar[1] = 500;  fAllowVar[2] = 0.1;
   
   fMillepede = new MMillepede();
}

//_________________________________________________________________
MAlignment::~MAlignment() 
{
   /// Destructor
   delete fMillepede;
}

//_________________________________________________________________
void MAlignment::SetIterations(Int_t iter)
{
   fMillepede->SetIterations(iter);
}

//_________________________________________________________________
void MAlignment::Init(Int_t nGlobal,  /* number of global paramers */
					  Int_t nLocal,   /* number of local parameters */
					  Int_t nStdDev,  /* std dev cut */ 
					  Int_t nPlanes   /* number of planes */)
{
   /// Initialization of AliMillepede. Fix parameters, define constraints ...
   fMillepede->InitMille(nGlobal,nLocal,nStdDev,fResCut,fResCutInitial);
   fNGlobal   = nGlobal;
   fNLocal    = nLocal;
   fNStdDev   = nStdDev;
   fNPlanes   = nPlanes;
   fNParPlane = nGlobal/nPlanes;
   
   // Define global constrains to be applied
   // X, Y, P, XvsZ, YvsZ, PvsZ, XvsY, YvsY, PvsY
   //Bool_t bVarXYT[9] = {kTRUE,kTRUE,kTRUE,kTRUE,kTRUE,kTRUE,kTRUE,kTRUE,kTRUE};
   //Bool_t bDetTLBR[4] = {kFALSE,kTRUE,kFALSE,kTRUE};
   
   // Other possible way to add constrains
   //bVarXYT[0] = kFALSE; bVarXYT[1] = kFALSE; bVarXYT[2] = kTRUE;
   //bDetTLBR[0] = kFALSE; bDetTLBR[1] = kTRUE; bDetTLBR[2] = kFALSE; bDetTLBR[3] = kFALSE;
   
   //bVarXYT[0] = kTRUE; bVarXYT[1] = kTRUE; bVarXYT[2] = kFALSE;
   
   // Set iterations
   if (fStartFac > 1) fMillepede->SetIterations(fStartFac);  
   
   printf("fNPlanes: %i\t fNGlobal: %i\t fNLocal: %i\n", fNPlanes, fNGlobal, fNLocal);

   ResetLocalEquation();
   printf("Parameters initialized to zero\n");

}

//_________________________________________________________________
void MAlignment::SetNonLinear(Bool_t *lChOnOff,Bool_t *lVarXYT)
{
   /// Set non linear parameter flag selected chambers and degrees of freedom
   for (Int_t i = 0; i < fNPlanes; i++){    
	  if (lChOnOff[i]){
		 if (lVarXYT[0]) { // X constraint
			std::cout<<"1"<<std::endl;
         SetNonLinear(i*fNParPlane+0);
		 }
		 if (lVarXYT[1]) { // Y constraint
         std::cout<<"2"<<std::endl;
			SetNonLinear(i*fNParPlane+1);
		 }
		 if (lVarXYT[2]) { // T constraint
			std::cout<<"3"<<std::endl;
         SetNonLinear(i*fNParPlane+2);
		 }
	  }
   }
}

//_________________________________________________________________
void MAlignment::AddConstraint(Double_t *par, Double_t value) {
   /// Constrain equation defined by par to value
   fMillepede->SetGlobalConstraint(par, value);
   printf("Adding constraint\n");
}

//_________________________________________________________________
void MAlignment::InitGlobalParameters(Double_t *par) 
{
   /// Initialize global parameters with par array
   fMillepede->SetGlobalParameters(par);
   printf("Init Global Parameters\n");
}

//_________________________________________________________________
void MAlignment::FixPlane(Int_t iPlane)
{
   /// Fix all detection elements of chamber iCh
   std::cout<<"iPlane = "<<iPlane<<std::endl; 
   FixParameter(iPlane*fNParPlane+0, 0.0);
   FixParameter(iPlane*fNParPlane+1, 0.0);
   FixParameter(iPlane*fNParPlane+2, 0.0);
   
}

//_________________________________________________________________
void MAlignment::FixParameter(Int_t iPar, Double_t value) 
{
   /// Parameter iPar is encourage to vary in [-value;value]. 
   /// If value == 0, parameter is fixed
   std::cout<<"value = "<<value<<"  iPar = "<<iPar<<std::endl;
   fMillepede->SetParSigma(iPar, value);
   if (value==0) printf("Parameter %i Fixed\n", iPar);
}

//_________________________________________________________________
void MAlignment::ResetLocalEquation()
{
   /// Reset the derivative vectors
   for(int i=0; i<fNLocal; i++) {
	  fLocalDerivatives[i] = 0.0;
   }
   for(int i=0; i<fNGlobal; i++) {
	  fGlobalDerivatives[i] = 0.0;
   }
}

//_________________________________________________________________
void MAlignment::AllowVariations(Bool_t *bPlaneOnOff) 
{
   /// Set allowed variation for selected chambers based on fDoF and fAllowVar
   for (Int_t iPlane = 0; iPlane < fNPlanes; iPlane++) {
	  if (bPlaneOnOff[iPlane]) {
		 for (int i=0; i<fNParPlane; i++) {
			if (fDebugLevel)
			   printf("fDoF[%d]= %d",i,fDoF[i]);    
			if (fDoF[i]) {
			   FixParameter(iPlane*fNParPlane+i, fAllowVar[i]);
			   
			}
		 }
	  }
   }
}
//_________________________________________________________________
void MAlignment::SetNonLinear(Int_t iPar  /* set non linear flag */ ) {
   /// Set nonlinear flag for parameter iPar
   fMillepede->SetNonLinear(iPar);
   printf("Parameter %i set to non linear\n", iPar);
}

//_________________________________________________________________
void MAlignment::LocalEquationX(Double_t* param) 
{
   /// Define local equation for current track and cluster in x coor. measurement
   // set local derivatives

   //std::cout<<"LocalEquationX"<<std::endl;

   DPlane* plane     = fHit->GetPlane();
   Int_t planeNumber = plane->GetPlaneNumber();
   DR3 pos           = plane->GetPosition();

   SetLocalDerivative(0, 1.);
   SetLocalDerivative(1, pos(2));
   SetLocalDerivative(2, 0.);
   SetLocalDerivative(3, 0.);

   // set global derivatives
   if (param == 0x0 ) {
	  SetGlobalDerivative(planeNumber*fNParPlane+0, 0.);
	  SetGlobalDerivative(planeNumber*fNParPlane+1, 0.);
	  SetGlobalDerivative(planeNumber*fNParPlane+2, 0.);
   } else {
	  SetGlobalDerivative(planeNumber*fNParPlane+0, 1.);
	  SetGlobalDerivative(planeNumber*fNParPlane+1, 0.);
	  SetGlobalDerivative(planeNumber*fNParPlane+2, -(param[4]+param[6]*pos(2)));
   }

   fMillepede->SetLocalEquation(fGlobalDerivatives, fLocalDerivatives, fMeas[0], fSigma[0]);
}

//_________________________________________________________________
void MAlignment::LocalEquationY(Double_t* param) 
{
   /// Define local equation for current track and cluster in y coor. measurement
   // set local derivatives
   
   //std::cout<<"LocalEquationY"<<std::endl;

   DPlane* plane     = fHit->GetPlane();
   Int_t planeNumber = plane->GetPlaneNumber();
   DR3 pos           = plane->GetPosition();
   
   SetLocalDerivative(0, 0.);
   SetLocalDerivative(1, 0.);
   SetLocalDerivative(2, 1.);
   SetLocalDerivative(3, pos(2));

   // set global derivatives
   if (param == 0x0) {
	  SetGlobalDerivative(planeNumber*fNParPlane+0, 0.);
	  SetGlobalDerivative(planeNumber*fNParPlane+1, 0.);
	  SetGlobalDerivative(planeNumber*fNParPlane+2, 0.);
   } else {
	  SetGlobalDerivative(planeNumber*fNParPlane+0, 0.);
	  SetGlobalDerivative(planeNumber*fNParPlane+1, 1.);
	  SetGlobalDerivative(planeNumber*fNParPlane+2, -(param[0]+param[2]*pos(2)));
   }
   
   fMillepede->SetLocalEquation(fGlobalDerivatives, fLocalDerivatives, fMeas[1], fSigma[1]);
}

//_________________________________________________________________
void MAlignment::ProcessTrack(DTrack * track, Double_t* param) 
{

   //std::cout<<"ProcessTrack"<<std::endl;

   /// Process track; Loop over clusters and set local equations
   fTrack = track;
   fTrackParam = &fTrack->GetLinearFit(); //GetTrackLine();
   // get size of arrays
   Int_t nHits = fTrack->GetHitsNumber(); // GetHitsNumber ? ou autre fonction ??

   if (fDebugLevel) printf("Number of track param entries : %i ", nHits);
   
	  for(Int_t iHit = 0; iHit < nHits; iHit++) {
		 fHit = fTrack->GetHit(iHit);
		 if (!fHit) continue;
		 // fill local variables for this position --> one measurement
		 
		 fMeas[0]  = fHit->GetPositionUhitCG(); // Centre de gravite.
		 fMeas[1]  = fHit->GetPositionVhitCG();
       //std::cout<<"fHit->GetPositionUhitCG() = "<<fHit->GetPositionUhitCG()<<"  fHit->GetPositionVhitCG() = "<<fHit->GetPositionVhitCG()<<std::endl;
		 fSigma[0] = 20; // fHit->GetPosError()(0); // 20 um par exemple, la vraie valeur n'est pas calculee :)
		 fSigma[1] = 20; // fHit->GetPosError()(1); // a calculer :)
	
		 if (fDebugLevel > 0)
			printf("fMeas[0]: %f\t fMeas[1]: %f\t fSigma[0]: %f\t fSigma[1]: %f\n", fMeas[0], fMeas[1], fSigma[0], fSigma[1]);    
		
		 // Set local equations
		 LocalEquationY(param);
		 LocalEquationX(param);

  //     if(param != 0) {
  //     std::cout<<"param[0] = "<<param[0]<<std::endl;
  //     std::cout<<"param[1] = "<<param[1]<<std::endl;
  //     std::cout<<"param[2] = "<<param[2]<<std::endl;
  //     }
	  }
  }

//_________________________________________________________________
void MAlignment::LocalFit(Int_t iTrack, Double_t *lTrackParam, Int_t lSingleFit) 
{

   //std::cout<<"LocalFit"<<std::endl;
   /// Call local fit for this tracks
   Int_t iRes = fMillepede->LocalFit(iTrack,lTrackParam,lSingleFit);
   if (iRes && !lSingleFit) {
	  fMillepede->SetNLocalEquations(fMillepede->GetNLocalEquations()+1);
//     std::cout<<"LocaFit : fMillepede->GetNLocalEquations()+1 = "<< fMillepede->GetNLocalEquations()+1<<std::endl;
      
   }
}


//_________________________________________________________________
void MAlignment::GlobalFit(Double_t *parameters,Double_t *errors,Double_t *pulls) 
{

   //std::cout<<"GlobalFit"<<std::endl;

   /// Call global fit; Global parameters are stored in parameters
   fMillepede->GlobalFit(parameters,errors,pulls);
   
   printf("Done fitting global parameters!\n");
   for (int i = 0; i < fNPlanes; i++){
	  printf("%d\t %8.3f (%8.3f)\t %8.3f (%8.3f)\t %7.4f (%7.4f)\n",i+1,
			 parameters[i*fNParPlane+0], errors[i*fNParPlane+0],
			 parameters[i*fNParPlane+1], errors[i*fNParPlane+1],
			 parameters[i*fNParPlane+2]*TMath::RadToDeg(), errors[i*fNParPlane+2]*TMath::RadToDeg());
   }
}

//_________________________________________________________________
Double_t MAlignment::GetParError(Int_t iPar) 
{
   /// Get error of parameter iPar
   Double_t lErr = fMillepede->GetParError(iPar);
   return lErr;
}

//_________________________________________________________________
void MAlignment::PrintGlobalParameters() 
{
   /// Print global parameters
   fMillepede->PrintGlobalParameters();
}

