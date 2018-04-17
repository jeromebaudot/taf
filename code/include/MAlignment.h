#ifndef _MAlignment_included_
#define _MAlignment_included_

#include <TObject.h>

/**
 MAlignment class for alignment of CMOS planes
 4 local parameters and 3 global parameters for maximum of 10 planes
 
 Alignment class which interface to AliMillepede.   
 for each track ProcessTrack calculates the local and global derivatives
 at each cluster and fill the corresponding local equations. Provide methods
 for fixing or constraining detection planes for best results. 
 
 \author Bruce Becker, Javier Castillo & Ch. Finck (transcription) 
 */

class MMillepede;
class DTrack;
class DHit;
class DLine;

class MAlignment:public TObject
{
   
public:
   MAlignment();
   virtual ~MAlignment();
   
   void ProcessTrack(DTrack *track, Double_t* param = 0x0);
   
   void AllowVariations(Bool_t *bPlaneOnOff);
   void SetNonLinear(Bool_t *bPlaneOnOff, Bool_t *bVarXYT);
   
   void FixPlane(Int_t iPlane);
   void FixParameter(Int_t param, Double_t value);
   void SetNonLinear(Int_t param);
   void AddConstraint(Double_t *factor, Double_t value );
   void InitGlobalParameters(Double_t *par);   
   void Init(Int_t nGlobal, Int_t nLocal, Int_t nStdDev, Int_t nPlanes);
   void SetIterations(Int_t iter);

   //! Set array of local derivatives
   void SetLocalDerivative(Int_t index, Double_t value) {      
	  fLocalDerivatives[index] = value;
   }
   
   //! Set array of global derivatives
   void SetGlobalDerivative(Int_t index, Double_t value) {
	  fGlobalDerivatives[index] = value;
   }  
   
   void LocalFit(Int_t iTrack, Double_t *lTrackParam, Int_t lSingleFit);
   void GlobalFit(Double_t *parameters,Double_t *errors,Double_t *pulls);
   void PrintGlobalParameters();
   Double_t GetParError(Int_t iPar);
   
   Int_t  GetNParPlane() const {return fNParPlane;}
      
private:
   /// Not implemented
   MAlignment(const MAlignment& right);
   /// Not implemented
   MAlignment&  operator = (const MAlignment& right);
   
   void ResetLocalEquation();
   void LocalEquationX(Double_t* param = 0x0);
   void LocalEquationY(Double_t* param = 0x0);
   
private:
   
   Bool_t   fDoF[3];         // Flags degrees of freedom to align (x,y,phi)
   Double_t fAllowVar[3];    // "Encouraged" variation for degrees of freedom 
   Double_t fStartFac;       // Initial value for chi2 cut 
   // if > 1 Iterations in AliMillepede are turned on
   Double_t fResCutInitial;  // Cut on residual for first iteration
   Double_t fResCut;         // Cut on residual for other iterations 
   
   MMillepede *fMillepede;   // Detector independent alignment class
   
   DTrack   *fTrack;         // AliMUONTrack 
   DLine    *fTrackParam;    // AliMUONVHit
   DHit     *fHit;           // AliMUONVHit
   
   Int_t    fNGlobal;        // Number of global parameters
   Int_t    fNLocal;         // Number of local parameters
   Int_t    fNStdDev;        // Number of standard deviations for chi2 cut
   Int_t    fNParPlane;      // Number of degrees of freedom per chamber
   Int_t    fNPlanes;        // Total number of detection elements
   
   Double_t fMeas[2];        // Current measurement (depend on B field On/Off)  
   Double_t fSigma[2];       // Estimated resolution on measurement
   
   Double_t fGlobalDerivatives[30]; // Array of global derivatives
   Double_t fLocalDerivatives[4];   // Array of local derivatives
   
   
   Double_t fPhi;            // Azimuthal tilt of detection element 
   Double_t fCosPhi;         // Cosine of fPhi
   Double_t fSinPhi;         // Sine of fPhi
   Int_t    fDebugLevel;     // debug level
   
   ClassDef(MAlignment, 0) //Class for alignment of muon spectrometer
};

#endif
