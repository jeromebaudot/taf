// @(#)maf/maf:$Name:  $:$Id: MAnalysis.cxx v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Perez 2015/03/11
// Last Modified: AP 2015/07/08 Added MS air between the sensors
// Last Modified: AP 2015/07/08 Changed the name of the MimosaGeneration function to MimosaGeneration_ToyMC
// Last Modified: AP 2015/07/08 New function, MimosaGeneration_LoicG4Simu, for MC estimation of tel resolution which uses Geant4 transport of tracks (Loic's code)

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Defines some methods for MC generation for             //
  //  for purposes of Telescope resolution evelaution        //
  //                                                         //
  /////////////////////////////////////////////////////////////

#ifndef _MimosaAnalysis_included_
#include "MAnalysis.h"
#endif
#include "DSetup.h"
#include "DTrack.h"
#include <TLatex.h>
#include <TArrow.h>
#include <TRandom.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TGeoManager.h>
//#include <TGLViewer.h>
#include "DEventMC.h"

#include <assert.h>
#include <map>

ClassImp(MimosaAnalysis)

//______________________________________________________________________________
//
double MimosaAnalysis::DistanceOfPointToPlane(const int PlaneNumber,
                                              DR3 ThePoint)
{
  
  //This function returns the distance between plane PlaneNumber and ThePoint. The ThePoint coordinates are in the laboratory frame
  
  if(!(PlaneNumber >= 1 && 
       PlaneNumber <= fSession->GetTracker()->GetPlanesN())
    ) {
    cout << endl;
    cout << "The plane number " << PlaneNumber << " specified in function DistanceOfPointToPlane is ";
    if(PlaneNumber <= 0) cout << " <=0. ";
    else if(PlaneNumber > fSession->GetTracker()->GetPlanesN()) cout << " > total number of planes. ";
    cout << "Exiting now!!!" << endl;
    cout << endl;
    assert(false);   
  }
  
  DPlane* tPlane     = fSession->GetTracker()->GetPlane(PlaneNumber);
  DPrecAlign* tAlign = (DPrecAlign*)tPlane->GetPrecAlignment();

  double *RotMat = tAlign->GetTorationMatrix();
  DR3 normVectPlane(0.0,0.0,0.0);
  if(tAlign->GetDPrecAlignMethod() == 0) normVectPlane.SetValue(RotMat[6],RotMat[7],RotMat[8]);
  else                                   normVectPlane.SetValue(RotMat[2],RotMat[5],RotMat[8]);
  normVectPlane = normVectPlane*(1.0/normVectPlane.Length());
  DR3 PlanePosition = tPlane->GetPosition();
  
  double distance  = 0.0;
  for(int i=0;i<3;i++) {
    distance += (PlanePosition(i) - ThePoint(i))*normVectPlane(i); 
  }
  distance = TMath::Abs(distance);
  
  return distance;
  
}
//______________________________________________________________________________
//
DR3 MimosaAnalysis::ClosestPointInPlaneToPoint(const int PlaneNumber,
                                               DR3 ThePoint)
{
  
  //This function returns the closest point of the plane PlaneNumber to the point ThePoint. The ThePoint coordinates are in the laboratory frame. The returned position 
  //coordinates are in the laboratory frame as well
  
    if(!(PlaneNumber >= 1 && 
       PlaneNumber <= fSession->GetTracker()->GetPlanesN())
    ) {
    cout << endl;
    cout << "The plane number " << PlaneNumber << " specified in function DistanceOfPointToPlane is ";
    if(PlaneNumber <= 0) cout << " <=0. ";
    else if(PlaneNumber > fSession->GetTracker()->GetPlanesN()) cout << " > total number of planes. ";
    cout << "Exiting now!!!" << endl;
    cout << endl;
    assert(false);   
  }
  
  DPlane* tPlane     = fSession->GetTracker()->GetPlane(PlaneNumber);
  DPrecAlign* tAlign = (DPrecAlign*)tPlane->GetPrecAlignment();

  double *RotMat = tAlign->GetRotationMatrix();
  DR3 normVectPlane(0.0,0.0,0.0);
  if(tAlign->GetDPrecAlignMethod() == 0) normVectPlane.SetValue(RotMat[6],RotMat[7],RotMat[8]);
  else                                   normVectPlane.SetValue(RotMat[2],RotMat[5],RotMat[8]);
  normVectPlane = normVectPlane*(1.0/normVectPlane.Length());
  DR3 PlanePosition = tPlane->GetPosition();
  
  double DotProduct = 0.0;
  for(int i=0;i<3;i++) DotProduct += (ThePoint(i) - PlanePosition(i))*normVectPlane(i);
  DR3 TheClosestPosition(0.0,0.0,0.0);
  for(int i=0;i<3;i++) {
    TheClosestPosition(i) = ThePoint(i) - DotProduct*normVectPlane(i);
  }
  
  return TheClosestPosition;
  
}
//______________________________________________________________________________
//
void MimosaAnalysis::BookingMC(Int_t PlaneNumber,
	                       Int_t Submatrix,
			       Int_t Geomatrix,
			       bool CalledFromMimosaPro)
{

  From_mu_to_mm = 1.0e-03;
  _PrintFreq    = 5000;
  Nsigma        = 5.0;
  double momentum = fSession->GetSetup()->GetTrackerPar().BeamMomentum;
  
  if(!CalledFromMimosaPro) {
    ThesubmatrixNumber = Submatrix;
    Thegeomatrix       = Geomatrix;
    GetParameters();
    ThePlaneNumber = PlaneNumber;
  }
  
  //DTracker *tTracker = fSession->GetTracker();
  Int_t nPlanes = fSession->GetTracker()->GetPlanesN();
  DPrecAlign* tAlign;
  DPlane* tPlane;
  DR3 posInPlane, posInTracker;
  
  lineXZ = new TLine*[nPlanes];
  lineYZ = new TLine*[nPlanes];
  number = new TText();
  latex  = new TLatex();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);
  latex->SetTextColor(kBlue);
  
  // Book histograms
  int Nbins_Chi2 = 500;
  double R_Chi2[2];
  R_Chi2[0] =   0.0;
  R_Chi2[1] = 250.0;
  hTrackChi2_MC = new TH1F("htrackchi2_MC", "#chi2/ndof of MC generated tracks",Nbins_Chi2,R_Chi2[0],R_Chi2[1]);
  hTrackChi2_MC->SetXTitle("#chi2/ndof");

  hTrackSlopeX_MC = new TH1F( "htrackslopex_MC", "Slope X=f(Z) of MC generated tracks", 100, 0., 0.);
  hTrackSlopeX_MC->SetXTitle("slope");
  hTrackSlopeY_MC = new TH1F( "htrackslopey_MC", "Slope Y=f(Z) of MC generated tracks", 100, 0., 0.);
  hTrackSlopeY_MC->SetXTitle("slope");
  hTrackNHits_MC = new TH1F( "htacknhits_MC", "Track hit multiplicity", nPlanes+1, -.5, nPlanes+.5);
  hTrackNHits_MC->SetXTitle("# hits in track");
  hTrackPlanesUsed_MC = new TH1F( "htackplanesused_MC", "Planes used in MC generated track fit; Plane number; # tracks", nPlanes+1, -.5, nPlanes+.5);

  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    sprintf( name, "hresidualupl%d_MC", iPlane);
    sprintf( title, "Hit-track horizontal residual (MC), plane %d", iPlane);
    hResidualU_MC[iPlane-1] = new TH1F(name, title, 200, -50., +50.);
    hResidualU_MC[iPlane-1]->SetXTitle("U residue (#mum)");

    sprintf( name, "hresidualvpl%d_MC", iPlane);
    sprintf( title, "Hit-track vertical residual (MC), plane %d", iPlane);
    hResidualV_MC[iPlane-1] = new TH1F(name, title, 200, -50., +50.);
    hResidualV_MC[iPlane-1]->SetXTitle("V residue (#mum)");
  }
  
  _PlaneList.clear();
  _PlaneListZ.clear();
  _PlaneThickness.clear();
  _PlaneMaterial.clear();
  _RU_min.clear();
  _RU_max.clear();
  _RV_min.clear();
  _RV_max.clear();
  
  //-- Read plane positions 
  
  MyRX[0] = +1.0e+20;
  MyRX[1] = -1.0e+20;
  MyRY[0] = +1.0e+20;
  MyRY[1] = -1.0e+20;
  MyRZ[0] = +1.0e+20;
  MyRZ[1] = -1.0e+20;

  DUTGeomMatrixX[0] = +1.0e+20;
  DUTGeomMatrixY[0] = +1.0e+20;
  DUTGeomMatrixX[1] = -1.0e+20;
  DUTGeomMatrixY[1] = -1.0e+20;
  
  UsingTrackerResolution = true;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = fSession->GetTracker()->GetPlane(iPlane);
    color[iPlane-1] = (iPlane==10)?49:iPlane;
    tAlign = (DPrecAlign*)tPlane->GetPrecAlignment();
    
    if(!fSession->GetSetup()->GetPlanePar(iPlane).UsingTrackerResolution) {
      cout << "Plane " << iPlane << " is using user defined plane resolution." << endl;
      UsingTrackerResolution = false;
    }

    // Get the lab coordinates of the plane center;
    posInPlane.SetValue(0.0,0.0,0.0);
    posInTracker = tAlign->TransformHitToTracker(posInPlane);
    _PlaneList.push_back(iPlane);
    _PlaneListZ.push_back(posInTracker(2));
    _PlaneThickness.push_back(fSession->GetSetup()->GetPlanePar(iPlane).PlaneThickness);
    _PlaneMaterial.push_back(fSession->GetSetup()->GetPlanePar(iPlane).PlaneMaterial);

    int col,lin;
    double u,v,w;

    //Getting the U and V ranges of the planes:
    double RU[2],RV[2];
    col = 0;
    lin = 0;
    tPlane->ComputeStripPosition(col,lin,u,v,w);
    RU[0] = u;
    RV[0] = v;
    col = tPlane->GetStripsNu()-1;
    lin = tPlane->GetStripsNv()-1;
    tPlane->ComputeStripPosition(col,lin,u,v,w);
    RU[1] = u;
    RV[1] = v;
    if(RU[0] > RU[1]) {
      double Rtmp = RU[0];
      RU[0] = RU[1];
      RU[1] = Rtmp;
    }
    if(RV[0] > RV[1]) {
      double Rtmp = RV[0];
      RV[0] = RV[1];
      RV[1] = Rtmp;
    }
    _RU_min.push_back(RU[0]);
    _RU_max.push_back(RU[1]);
    _RV_min.push_back(RV[0]);
    _RV_max.push_back(RV[1]);
    
    double RXtmp[2];
    double RYtmp[2];
    double RZtmp[2];
    
    // -- line for the XZ top view
    lineXZ[iPlane-1] = new TLine();
    lineXZ[iPlane-1]->SetLineWidth(3);
    lineXZ[iPlane-1]->SetLineColor(color[iPlane-1]);

    col = 0;
    lin = tPlane->GetStripsNv()/2;
    tPlane->ComputeStripPosition(col,lin,u,v,w);
    posInPlane.SetValue(u,v,0.0);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    RXtmp[0] = posInTracker(0)*From_mu_to_mm;
    RYtmp[0] = posInTracker(1)*From_mu_to_mm;
    RZtmp[0] = posInTracker(2)*From_mu_to_mm;
    if(MyRX[0] > RXtmp[0]) MyRX[0] = RXtmp[0];
    if(MyRX[1] < RXtmp[0]) MyRX[1] = RXtmp[0];
    if(MyRY[0] > RYtmp[0]) MyRY[0] = RYtmp[0];
    if(MyRY[1] < RYtmp[0]) MyRY[1] = RYtmp[0];
    if(MyRZ[0] > RZtmp[0]) MyRZ[0] = RZtmp[0];
    if(MyRZ[1] < RZtmp[0]) MyRZ[1] = RZtmp[0];
    col = tPlane->GetStripsNu()-1;
    lin = tPlane->GetStripsNv()/2;
    tPlane->ComputeStripPosition(col,lin,u,v,w);
    posInPlane.SetValue(u,v,0.0);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    RXtmp[1] = posInTracker(0)*From_mu_to_mm;
    RYtmp[1] = posInTracker(1)*From_mu_to_mm;
    RZtmp[1] = posInTracker(2)*From_mu_to_mm;
    if(MyRX[0] > RXtmp[1]) MyRX[0] = RXtmp[1];
    if(MyRX[1] < RXtmp[1]) MyRX[1] = RXtmp[1];
    if(MyRY[0] > RYtmp[1]) MyRY[0] = RYtmp[1];
    if(MyRY[1] < RYtmp[1]) MyRY[1] = RYtmp[1];
    if(MyRZ[0] > RZtmp[1]) MyRZ[0] = RZtmp[1];
    if(MyRZ[1] < RZtmp[1]) MyRZ[1] = RZtmp[1];
    if(RXtmp[0] > RXtmp[1]) {
      double tmp = RXtmp[0];
      RXtmp[0]   = RXtmp[1];
      RXtmp[1]   = tmp;
    }
    if(RYtmp[0] > RYtmp[1]) {
      double tmp = RYtmp[0];
      RYtmp[0]   = RYtmp[1];
      RYtmp[1]   = tmp;
    }
    if(RZtmp[0] > RZtmp[1]) {
      double tmp = RZtmp[0];
      RZtmp[0]   = RZtmp[1];
      RZtmp[1]   = tmp;
    }
    // middle left corner
    lineXZ[iPlane-1]->SetX1(RZtmp[0]);
    lineXZ[iPlane-1]->SetY1(RXtmp[0]);
    // middle right corner
    lineXZ[iPlane-1]->SetX2(RZtmp[1]);
    lineXZ[iPlane-1]->SetY2(RXtmp[1]);
    //printf( " XZ-line: left (%.1f, %.1f), right (%.1f, %.1f)\n", lineXZ[iPlane-1]->GetY1(), lineXZ[iPlane-1]->GetX1(), lineXZ[iPlane-1]->GetY2(), lineXZ[iPlane-1]->GetX2() );
	   
    // -- line for the YZ top view
    lineYZ[iPlane-1] = new TLine();
    lineYZ[iPlane-1]->SetLineWidth(3);
    lineYZ[iPlane-1]->SetLineColor(color[iPlane-1]);
    col = tPlane->GetStripsNu()/2;
    lin = 0;
    tPlane->ComputeStripPosition(col,lin,u,v,w);
    posInPlane.SetValue(u,v,0.0);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    RXtmp[0] = posInTracker(0)*From_mu_to_mm;
    RYtmp[0] = posInTracker(1)*From_mu_to_mm;
    RZtmp[0] = posInTracker(2)*From_mu_to_mm;
    if(MyRX[0] > RXtmp[0]) MyRX[0] = RXtmp[0];
    if(MyRX[1] < RXtmp[0]) MyRX[1] = RXtmp[0];
    if(MyRY[0] > RYtmp[0]) MyRY[0] = RYtmp[0];
    if(MyRY[1] < RYtmp[0]) MyRY[1] = RYtmp[0];
    if(MyRZ[0] > RZtmp[0]) MyRZ[0] = RZtmp[0];
    if(MyRZ[1] < RZtmp[0]) MyRZ[1] = RZtmp[0];
    col = tPlane->GetStripsNu()/2;
    lin = tPlane->GetStripsNv()-1;
    tPlane->ComputeStripPosition(col,lin,u,v,w);
    posInPlane.SetValue(u,v,0.0);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    RXtmp[1] = posInTracker(0)*From_mu_to_mm;
    RYtmp[1] = posInTracker(1)*From_mu_to_mm;
    RZtmp[1] = posInTracker(2)*From_mu_to_mm;
    if(MyRX[0] > RXtmp[1]) MyRX[0] = RXtmp[1];
    if(MyRX[1] < RXtmp[1]) MyRX[1] = RXtmp[1];
    if(MyRY[0] > RYtmp[1]) MyRY[0] = RYtmp[1];
    if(MyRY[1] < RYtmp[1]) MyRY[1] = RYtmp[1];
    if(MyRZ[0] > RZtmp[1]) MyRZ[0] = RZtmp[1];
    if(MyRZ[1] < RZtmp[1]) MyRZ[1] = RZtmp[1];
    if(RXtmp[0] > RXtmp[1]) {
      double tmp = RXtmp[0];
      RXtmp[0]   = RXtmp[1];
      RXtmp[1]   = tmp;
    }
    if(RYtmp[0] > RYtmp[1]) {
      double tmp = RYtmp[0];
      RYtmp[0]   = RYtmp[1];
      RYtmp[1]   = tmp;
    }
    if(RZtmp[0] > RZtmp[1]) {
      double tmp = RZtmp[0];
      RZtmp[0]   = RZtmp[1];
      RZtmp[1]   = tmp;
    }
    // bottom middle corner
    lineYZ[iPlane-1]->SetX1(RZtmp[0]);
    lineYZ[iPlane-1]->SetY1(RYtmp[0]);
    // top middle corner
    lineYZ[iPlane-1]->SetX2(RZtmp[1]);
    lineYZ[iPlane-1]->SetY2(RYtmp[1]);

    if(iPlane == PlaneNumber) {

      Double_t gumin = fSession->GetSetup()->GetAnalysisPar().Umin[Submatrix][Geomatrix];
      Double_t gumax = fSession->GetSetup()->GetAnalysisPar().Umax[Submatrix][Geomatrix];
      Double_t gvmin = fSession->GetSetup()->GetAnalysisPar().Vmin[Submatrix][Geomatrix];
      Double_t gvmax = fSession->GetSetup()->GetAnalysisPar().Vmax[Submatrix][Geomatrix];

      posInPlane.SetValue(gumin,gvmin,0.0);
      posInTracker = tAlign->TransformHitToTracker(posInPlane);
      if(DUTGeomMatrixX[0] > posInTracker(0)) DUTGeomMatrixX[0] = posInTracker(0);
      if(DUTGeomMatrixX[1] < posInTracker(0)) DUTGeomMatrixX[1] = posInTracker(0);
      if(DUTGeomMatrixY[0] > posInTracker(1)) DUTGeomMatrixY[0] = posInTracker(1);
      if(DUTGeomMatrixY[1] < posInTracker(1)) DUTGeomMatrixY[1] = posInTracker(1);

      posInPlane.SetValue(gumin,gvmax,0.0);
      posInTracker = tAlign->TransformHitToTracker(posInPlane);
      if(DUTGeomMatrixX[0] > posInTracker(0)) DUTGeomMatrixX[0] = posInTracker(0);
      if(DUTGeomMatrixX[1] < posInTracker(0)) DUTGeomMatrixX[1] = posInTracker(0);
      if(DUTGeomMatrixY[0] > posInTracker(1)) DUTGeomMatrixY[0] = posInTracker(1);
      if(DUTGeomMatrixY[1] < posInTracker(1)) DUTGeomMatrixY[1] = posInTracker(1);

      posInPlane.SetValue(gumax,gvmin,0.0);
      posInTracker = tAlign->TransformHitToTracker(posInPlane);
      if(DUTGeomMatrixX[0] > posInTracker(0)) DUTGeomMatrixX[0] = posInTracker(0);
      if(DUTGeomMatrixX[1] < posInTracker(0)) DUTGeomMatrixX[1] = posInTracker(0);
      if(DUTGeomMatrixY[0] > posInTracker(1)) DUTGeomMatrixY[0] = posInTracker(1);
      if(DUTGeomMatrixY[1] < posInTracker(1)) DUTGeomMatrixY[1] = posInTracker(1);

      posInPlane.SetValue(gumax,gvmax,0.0);
      posInTracker = tAlign->TransformHitToTracker(posInPlane);
      if(DUTGeomMatrixX[0] > posInTracker(0)) DUTGeomMatrixX[0] = posInTracker(0);
      if(DUTGeomMatrixX[1] < posInTracker(0)) DUTGeomMatrixX[1] = posInTracker(0);
      if(DUTGeomMatrixY[0] > posInTracker(1)) DUTGeomMatrixY[0] = posInTracker(1);
      if(DUTGeomMatrixY[1] < posInTracker(1)) DUTGeomMatrixY[1] = posInTracker(1);
    }

  } // end loop on planes

  //Ordering the planes by Z coordinate, from lower to higher.
  for(int iii=2;iii<=int(_PlaneList.size());iii++) {
    for(int jjj=0;jjj<=int(_PlaneList.size())-iii;jjj++) {
      double z_jjj   = _PlaneListZ[jjj];
      double z_jjjp1 = _PlaneListZ[jjj+1];
      
      if(momentum >= 0.0) {
        if(z_jjj > z_jjjp1) {
	  double Z_aux       = _PlaneListZ[jjj];
	  _PlaneListZ[jjj]   = _PlaneListZ[jjj+1];
	  _PlaneListZ[jjj+1] = Z_aux;

	  int idx_aux        = _PlaneList[jjj];
	  _PlaneList[jjj]    = _PlaneList[jjj+1];
	  _PlaneList[jjj+1]  = idx_aux;

	  double Thickness_aux   = _PlaneThickness[jjj];
	  _PlaneThickness[jjj]   = _PlaneThickness[jjj+1];
	  _PlaneThickness[jjj+1] = Thickness_aux;

	  TString Material_aux  = _PlaneMaterial[jjj];
	  _PlaneMaterial[jjj]   = _PlaneMaterial[jjj+1];
	  _PlaneMaterial[jjj+1] = Material_aux;
	  
	  double range;
	  range  = _RU_min[jjj];
	  _RU_min[jjj]   = _RU_min[jjj+1];
	  _RU_min[jjj+1] = range;
	  range  = _RU_max[jjj];
	  _RU_max[jjj]   = _RU_max[jjj+1];
	  _RU_max[jjj+1] = range;
	  range  = _RV_min[jjj];
	  _RV_min[jjj]   = _RV_min[jjj+1];
	  _RV_min[jjj+1] = range;
	  range  = _RV_max[jjj];
	  _RV_max[jjj]   = _RV_max[jjj+1];
	  _RV_max[jjj+1] = range;
        }
      }
      else {
	if(z_jjj < z_jjjp1) {
	  double Z_aux       = _PlaneListZ[jjj];
	  _PlaneListZ[jjj]   = _PlaneListZ[jjj+1];
	  _PlaneListZ[jjj+1] = Z_aux;

	  int idx_aux        = _PlaneList[jjj];
	  _PlaneList[jjj]    = _PlaneList[jjj+1];
	  _PlaneList[jjj+1]  = idx_aux;

	  double Thickness_aux   = _PlaneThickness[jjj];
	  _PlaneThickness[jjj]   = _PlaneThickness[jjj+1];
	  _PlaneThickness[jjj+1] = Thickness_aux;

	  TString Material_aux  = _PlaneMaterial[jjj];
	  _PlaneMaterial[jjj]   = _PlaneMaterial[jjj+1];
	  _PlaneMaterial[jjj+1] = Material_aux;
	  
	  double range;
	  range  = _RU_min[jjj];
	  _RU_min[jjj]   = _RU_min[jjj+1];
	  _RU_min[jjj+1] = range;
	  range  = _RU_max[jjj];
	  _RU_max[jjj]   = _RU_max[jjj+1];
	  _RU_max[jjj+1] = range;
	  range  = _RV_min[jjj];
	  _RV_min[jjj]   = _RV_min[jjj+1];
	  _RV_min[jjj+1] = range;
	  range  = _RV_max[jjj];
	  _RV_max[jjj]   = _RV_max[jjj+1];
	  _RV_max[jjj+1] = range;
        }
      }
    }
  }

  cout << endl;
  for(int iii=0;iii<int(_PlaneList.size());iii++) {
    cout << iii+1 << " iPlane = " << _PlaneList[iii] 
	 << ", Z = " << _PlaneListZ[iii]*1.0e-4 << " cm" 
	 << ", thickness = " << _PlaneThickness[iii] << "um"
	 << ", material  = " << _PlaneMaterial[iii].Data()
	 << ", U range (" << _RU_min[iii] << "," << _RU_max[iii] << ")um"
	 << ", V range (" << _RV_min[iii] << "," << _RV_max[iii] << ")um"
	 << endl;
  }
  cout << endl;
  _PlaneListZ.clear();
  
  //-- histograms for reference frame

  RGlobalResolution[0] = fSession->GetSetup()->GetAnalysisPar().ResolutionScanInit;
  RGlobalResolution[1] = fSession->GetSetup()->GetAnalysisPar().ResolutionScanEnd;
  int NbinsTmp   = fSession->GetSetup()->GetAnalysisPar().ResolutionScanSteps;
  if(!UsingTrackerResolution) NbinsTmp = 1;
  Bins_GlobalResolution = NbinsTmp;
  
  if(Bins_GlobalResolution > MyBins_GlobalResolution) {
    cout << endl;
    cout << "MimosaGeneration:: number of steps " << Bins_GlobalResolution << " in resolution scan is higher than maximum " << MyBins_GlobalResolution 
	 << " defined in MHits.h. Check you inputs. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  if(UsingTrackerResolution) {
    cout << endl;
    cout << "MimosaAnalysis::MimosaGeneration:: Using global tracker resolution." << endl;
    cout << "MimosaAnalysis::MimosaGeneration:: Assume all planes are identical with the same spatial resolution." << endl;
    cout << "MimosaAnalysis::MimosaGeneration:: Doing scan of plane spatial resolution with " << Bins_GlobalResolution << " steps and in range (" 
	 << RGlobalResolution[0] << "," << RGlobalResolution[1] << ")um"
	 << endl;
    cout << endl;
  }
  
  int Nbins_res = 1600;
  double Rres[2];
  //Rres[0] = -50.0;
  //Rres[1] = +50.0;
  Rres[0] = -400.0;
  Rres[1] = +400.0;
  for(Int_t istep=0;istep<Bins_GlobalResolution;istep++) {
    double TheResolution = RGlobalResolution[0] + (istep+0.5)*(RGlobalResolution[1] - RGlobalResolution[0])/Bins_GlobalResolution;
    if(!UsingTrackerResolution) {
      sprintf(name,  "hTrackResidualAtDUT_U_Pl%d", PlaneNumber);
      sprintf(title, "Hit-track U-residual at DUT, plane %d", PlaneNumber);
    }
    else {
      sprintf(name,  "hTrackResidualAtDUT_U_Pl%d_step%d", PlaneNumber,istep+1);
      sprintf(title, "Hit-track U-residual at DUT, plane %d, for step %d (resolution = %.2f)",PlaneNumber,istep+1,TheResolution);
    }
    hTrackResidualAtDUT_U[istep] = new TH1F(name, title, Nbins_res, Rres[0], Rres[1]);
    hTrackResidualAtDUT_U[istep]->SetXTitle("U residual (#mum)");
    hTrackResidualAtDUT_U[istep]->SetLineWidth(2);
    hTrackResidualAtDUT_U[istep]->SetLineColor(4);
    hTrackResidualAtDUT_U[istep]->SetMarkerColor(4);
    hTrackResidualAtDUT_U[istep]->SetMarkerSize(0.5);
    hTrackResidualAtDUT_U[istep]->SetMarkerStyle(20);
    hTrackResidualAtDUT_U[istep]->Sumw2();

    if(!UsingTrackerResolution) {
      sprintf(name,  "hTrackResidualAtDUT_V_Pl%d", PlaneNumber);
      sprintf(title, "Hit-track V-residual at DUT, plane %d", PlaneNumber);
    }
    else {
      sprintf(name,  "hTrackResidualAtDUT_V_Pl%d_step%d", PlaneNumber,istep+1);
      sprintf(title, "Hit-track V-residual at DUT, plane %d, for step %d (resolution = %.2f)",PlaneNumber,istep+1,TheResolution);
    }
    hTrackResidualAtDUT_V[istep] = new TH1F(name, title, Nbins_res, Rres[0], Rres[1]);
    hTrackResidualAtDUT_V[istep]->SetXTitle("V residual (#mum)");
    hTrackResidualAtDUT_V[istep]->SetLineWidth(2);
    hTrackResidualAtDUT_V[istep]->SetLineColor(4);
    hTrackResidualAtDUT_V[istep]->SetMarkerColor(4);
    hTrackResidualAtDUT_V[istep]->SetMarkerSize(0.5);
    hTrackResidualAtDUT_V[istep]->SetMarkerStyle(20);
    hTrackResidualAtDUT_V[istep]->Sumw2();
  }

  h_TelescopeResolution_Scan_U = new TH1F("h_TelescopeResolution_Scan_U",
					  "#sigma_{Tel}(U) vs sensor #sigma_{sp}(U)",
					  Bins_GlobalResolution,RGlobalResolution[0],RGlobalResolution[1]);
  h_TelescopeResolution_Scan_U->SetXTitle("sensor #sigma_{sp}(U) (#mum)");
  h_TelescopeResolution_Scan_U->GetXaxis()->CenterTitle(true);
  h_TelescopeResolution_Scan_U->SetYTitle("#sigma_{Tel}(U) (#mum)");
  h_TelescopeResolution_Scan_U->GetYaxis()->CenterTitle(true);
  h_TelescopeResolution_Scan_U->SetLineWidth(2);
  h_TelescopeResolution_Scan_U->SetLineColor(4);
  h_TelescopeResolution_Scan_U->SetMarkerColor(4);
  h_TelescopeResolution_Scan_U->SetMarkerSize(0.5);
  h_TelescopeResolution_Scan_U->SetMarkerStyle(20);
  h_TelescopeResolution_Scan_U->SetStats(false);

  h_TelescopeResolution_Scan_V = new TH1F("h_TelescopeResolution_Scan_V",
					  "#sigma_{Tel}(V) vs sensor #sigma_{sp}(V)",
					  Bins_GlobalResolution,RGlobalResolution[0],RGlobalResolution[1]);
  h_TelescopeResolution_Scan_V->SetXTitle("sensor #sigma_{sp}(V) (#mum)");
  h_TelescopeResolution_Scan_V->GetXaxis()->CenterTitle(true);
  h_TelescopeResolution_Scan_V->SetYTitle("#sigma_{Tel}(V) (#mum)");
  h_TelescopeResolution_Scan_V->GetYaxis()->CenterTitle(true);
  h_TelescopeResolution_Scan_V->SetLineWidth(2);
  h_TelescopeResolution_Scan_V->SetLineColor(4);
  h_TelescopeResolution_Scan_V->SetMarkerColor(4);
  h_TelescopeResolution_Scan_V->SetMarkerSize(0.5);
  h_TelescopeResolution_Scan_V->SetMarkerStyle(20);
  h_TelescopeResolution_Scan_V->SetStats(false);

  h_ResidualAtDUT_Scan_U = new TH1F("h_ResidualAtDUT_Scan_U",
				    "#sqrt{#sigma^{2}_{Tel} + #sigma^{2}_{sp}}(U) vs sensor #sigma_{sp}(U)",
				    Bins_GlobalResolution,RGlobalResolution[0],RGlobalResolution[1]);
  h_ResidualAtDUT_Scan_U->SetXTitle("sensor #sigma_{sp}(U) (#mum)");
  h_ResidualAtDUT_Scan_U->GetXaxis()->CenterTitle(true);
  h_ResidualAtDUT_Scan_U->SetYTitle("#sqrt{#sigma^{2}_{Tel} + #sigma^{2}_{sp}}(U) (#mum)");
  h_ResidualAtDUT_Scan_U->GetYaxis()->CenterTitle(true);
  h_ResidualAtDUT_Scan_U->SetLineWidth(2);
  h_ResidualAtDUT_Scan_U->SetLineColor(2);
  h_ResidualAtDUT_Scan_U->SetMarkerColor(2);
  h_ResidualAtDUT_Scan_U->SetMarkerSize(0.5);
  h_ResidualAtDUT_Scan_U->SetMarkerStyle(20);
  h_ResidualAtDUT_Scan_U->SetStats(false);

  h_ResidualAtDUT_Scan_V = new TH1F("h_ResidualAtDUT_Scan_V",
				    "#sqrt{#sigma^{2}_{Tel} + #sigma^{2}_{sp}}(V) vs sensor #sigma_{sp}(V)",
				    Bins_GlobalResolution,RGlobalResolution[0],RGlobalResolution[1]);
  h_ResidualAtDUT_Scan_V->SetXTitle("sensor #sigma_{sp}(V) (#mum)");
  h_ResidualAtDUT_Scan_V->GetXaxis()->CenterTitle(true);
  h_ResidualAtDUT_Scan_V->SetYTitle("#sqrt{#sigma^{2}_{Tel} + #sigma^{2}_{sp}}(V) (#mum)");
  h_ResidualAtDUT_Scan_V->GetYaxis()->CenterTitle(true);
  h_ResidualAtDUT_Scan_V->SetLineWidth(2);
  h_ResidualAtDUT_Scan_V->SetLineColor(2);
  h_ResidualAtDUT_Scan_V->SetMarkerColor(2);
  h_ResidualAtDUT_Scan_V->SetMarkerSize(0.5);
  h_ResidualAtDUT_Scan_V->SetMarkerStyle(20);
  h_ResidualAtDUT_Scan_V->SetStats(false);
  
  double RX[2];
  double RY[2];
  double RZ[2];
  double delta;
  double porcent = 0.10;
  RX[0] = MyRX[0];
  RX[1] = MyRX[1];
  //RX[0] = DUTGeomMatrixX[0]*From_mu_to_mm;
  //RX[1] = DUTGeomMatrixX[1]*From_mu_to_mm;
  delta = RX[1] - RX[0];
  RX[0] -= porcent*delta;
  RX[1] += porcent*delta;
  RY[0] = MyRY[0];
  RY[1] = MyRY[1];
  //RY[0] = DUTGeomMatrixY[0]*From_mu_to_mm;
  //RY[1] = DUTGeomMatrixY[1]*From_mu_to_mm;
  delta = RY[1] - RY[0];
  RY[0] -= porcent*delta;
  RY[1] += porcent*delta;
  RZ[0] = MyRZ[0];
  RZ[1] = MyRZ[1];
  delta = RZ[1] - RZ[0];
  RZ[0] -= porcent*delta;
  RZ[1] += porcent*delta;

  int Nbins_viasual = 1000;
  hXZ_MC = new TH2F("hXZ_MC","X-Z projection - TOP VIEW",
			  Nbins_viasual, RZ[0], RZ[1],
			  Nbins_viasual, RX[0], RX[1]);
  hXZ_MC->SetXTitle("Z (mm)");
  hXZ_MC->GetXaxis()->CenterTitle(true);
  hXZ_MC->SetYTitle("X (mm)");
  hXZ_MC->GetYaxis()->CenterTitle(true);
  hXZ_MC->SetTitleSize( 0.05, "XY");
  hXZ_MC->SetLabelSize( 0.05, "XY");
  hXZ_MC->SetStats(0);
  hYZ_MC = new TH2F("hYZ_MC","Y-Z projection - SIDE VIEW",
			  Nbins_viasual, RZ[0], RZ[1],
			  Nbins_viasual, RY[0], RY[1]);
  hYZ_MC->SetXTitle("Z (mm)");
  hYZ_MC->GetXaxis()->CenterTitle(true);
  hYZ_MC->SetYTitle("Y (mm)");
  hYZ_MC->GetYaxis()->CenterTitle(true);
  hYZ_MC->SetTitleSize( 0.05, "XY");
  hYZ_MC->SetLabelSize( 0.05, "XY");
  hYZ_MC->SetStats(0);

  MyRX[0] /= From_mu_to_mm;
  MyRX[1] /= From_mu_to_mm;
  MyRY[0] /= From_mu_to_mm;
  MyRY[1] /= From_mu_to_mm;


  //if(CalledFromMimosaPro) {
  MyRX[0] = DUTGeomMatrixX[0];
  MyRX[1] = DUTGeomMatrixX[1];
  MyRY[0] = DUTGeomMatrixY[0];
  MyRY[1] = DUTGeomMatrixY[1];

  return;
  
}
//______________________________________________________________________________
//
void MimosaAnalysis::DoMCDisplay(int NDisplay,
                                 bool verbose)
{

  Char_t tnum[20];
  Int_t nPlanes = fSession->GetTracker()->GetPlanesN();
  double porcentX = 0.05;
  //double porcentY = 0.05;
  
  cgeometry = new TCanvas("cgeometryMC", "Telescope geometry");
  cgeometry->cd();
  cgeometry->SetLeftMargin(0.15);
  cgeometry->SetBottomMargin(0.15);
  cgeometry->SetRightMargin(0.15);
  cgeometry->Divide(1,2);
  cgeometry->cd(1);
  gPad->SetFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  //gPad->SetGridx(1);
  //gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  //gPad->SetRightMargin(0.15);
  hXZ_MC->Draw();
  TLine* lh_XZ = new TLine(hXZ_MC->GetXaxis()->GetXmin(),0.0,
	                   hXZ_MC->GetXaxis()->GetXmax(),0.0);
  lh_XZ->SetLineColor(1);
  lh_XZ->SetLineWidth(1);
  lh_XZ->SetLineStyle(2);
  lh_XZ->Draw();
  TLine* lv_XZ = new TLine(0.0,hXZ_MC->GetYaxis()->GetXmin(),
		           0.0,hXZ_MC->GetYaxis()->GetXmax());
  lv_XZ->SetLineColor(1);
  lv_XZ->SetLineWidth(1);
  lv_XZ->SetLineStyle(2);
  lv_XZ->Draw();
  for(int iPlane=0; iPlane<nPlanes; iPlane++) {
    lineXZ[iPlane]->Draw();
    sprintf( tnum, "%d", iPlane+1);
    number->SetTextColor(lineXZ[iPlane]->GetLineColor());
    porcentX = 0.05;
    number->DrawText(0.5*(lineXZ[iPlane]->GetX2() + lineXZ[iPlane]->GetX1()),
                    lineXZ[iPlane]->GetY2() + porcentX*(lineXZ[iPlane]->GetY2() - lineXZ[iPlane]->GetY1()),
		    tnum);
  }
  cgeometry->cd(2);
  gPad->SetFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  //gPad->SetGridx(1);
  //gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  //gPad->SetRightMargin(0.15);
  hYZ_MC->Draw();
  TLine* lh_YZ = new TLine(hYZ_MC->GetXaxis()->GetXmin(),0.0,
	                   hYZ_MC->GetXaxis()->GetXmax(),0.0);
  lh_YZ->SetLineColor(1);
  lh_YZ->SetLineWidth(1);
  lh_YZ->SetLineStyle(2);
  lh_YZ->Draw();
  TLine* lv_YZ = new TLine(0.0,hYZ_MC->GetYaxis()->GetXmin(),
	                   0.0,hYZ_MC->GetYaxis()->GetXmax());
  lv_YZ->SetLineColor(1);
  lv_YZ->SetLineWidth(1);
  lv_YZ->SetLineStyle(2);
  lv_YZ->Draw();
  for(int iPlane=0; iPlane<nPlanes; iPlane++) lineYZ[iPlane]->Draw();
  
  for(int ievt=0;ievt<NDisplay;ievt++) {
    for(int itrack=0;itrack<TracksPerEvt[ievt];itrack++) {
      if(verbose) {
	cout << "Evt = " << ievt+1 << ", Drawing track " << itrack+1 << ", color = " << gtracksXZ[ievt][itrack]->GetLineColor() << endl;
      }
      cgeometry->cd(1);
      gtracksXZ[ievt][itrack]->Draw("Pl");
      cgeometry->cd(2);
      gtracksYZ[ievt][itrack]->Draw("Pl");
    }
    if(verbose) cout << endl;

    if(!UsingTrackerResolution) {
      //Plotting reconstructed tracks:
      for(Int_t itrack=0;itrack<NRectracks[ievt];itrack++) {
	if(verbose) {
	  cout << "Evt = " << ievt+1 << ", Drawing rec track " << itrack+1 << endl;
	}
	cgeometry->cd(1);
	RectracksXZ[ievt][itrack]->Draw();
	cgeometry->cd(2);
	RectracksYZ[ievt][itrack]->Draw();
      }
      if(verbose) cout << endl;
    }
  }

  return;
  
}
//______________________________________________________________________________
//
void MimosaAnalysis::DoMCDisplay3D(int NDisplay,
                                   bool verbose,
				   const char* file_gdml)
{

  //Char_t tnum[20];
  //Int_t nPlanes = fSession->GetTracker()->GetPlanesN();
  //double porcentX = 0.05;
  //double porcentY = 0.05;
  
  cgeometry3D = new TCanvas("cgeometryMC3D", "Telescope geometry 3D (gdml)");
  cgeometry3D->cd();
  TGeoManager::Import(file_gdml);
  gGeoManager->GetTopVolume()->Draw("ogl");

  for(int ievt=0;ievt<NDisplay;ievt++) {
    for(int itrack=0;itrack<TracksPerEvt[ievt];itrack++) {
      if(verbose) cout << "Evt = " << ievt+1 << ", Drawing track3D " << itrack+1 << ", color = " << gtracksXYZ[ievt][itrack]->GetLineColor() << " and style = " << gtracksXYZ[ievt][itrack]->GetLineStyle() << endl;
      gtracksXYZ[ievt][itrack]->Draw("same");
      gtracksXYZ_mark[ievt][itrack]->Draw("same");
    }
    if(verbose) cout << endl;

    if(!UsingTrackerResolution) {
      //Plotting reconstructed tracks:
      for(Int_t itrack=0;itrack<NRectracks[ievt];itrack++) {
	if(verbose) {
	  cout << "Evt = " << ievt+1 << ", Drawing rec track3D " << itrack+1 << ", color " << RectracksXYZ[ievt][itrack]->GetLineColor() << " and style = " << RectracksXYZ[ievt][itrack]->GetLineStyle() << endl;
	}
	RectracksXYZ[ievt][itrack]->Draw();
      }
      if(verbose) cout << endl;
    }
  }

  return;
  
}
//______________________________________________________________________________
//
void MimosaAnalysis::MimosaGeneration_ToyMC(Int_t MaxEvt,
				            Int_t PlaneNumber,
				            Int_t Submatrix,
				            Int_t Geomatrix,
				            bool  DoGaussianMS,
				            Int_t seed,
				            bool  DoDisplay,
				            double XMeanDiv,
				            double XRMSDiv,
				            double YMeanDiv,
				            double YRMSDiv,
				            bool  CalledFromMimosaPro,
				            bool  verbose)
{

  // This function estimates the telescope resolution including multiple scattering effects with a Toy MC simulation. The procedure is as follows,
  //  - The experimental set-up geometry is read from the config file.
  //  - Particles are generated with the type (e.g. electron/pion) and momentum as specified in the config file.
  //  - The tracks are generated parallel to the Z-axis with a Gaussian angular dispersion in X and Y using the parameters XMeanDiv,XRMSDiv and YMeanDiv,YRMSDiv, respectively
  //    Defaul values are XMeanDiv = YMeanDiv = 0 and XRMSDiv = YRMSDiv = 1.0e-20, i.e. no angular dispersion.
  //  - The tracks are generated within the X and Y ranges defined by the Submatrix and Geomatrix.
  //  - Tracks transport is simulated using a toy theory of multiple scattering.
  //    * if DoGaussianMS is true   => gaussian model of the multiple scattering angle is used.
  //    * if DoGaussianMS is false, => non-gaussian tails of the multiple scattering angle are included using the Moliere theory.
  //  - No energy loss is simulated, only multiple scattering by the different materials (e.g. sensors (Si), medium (Air), and maybe other).
  //  - The real positions of the transported track intersection with the sensors are then smeared using the user defined PlaneResolution.
  //  - Hits are then generated and passed on to the tracker.
  //  - The track finding and tracking algorithms are the same as in the regular analysis. The same track quality cuts (Chi2 cut) as in regular analysis are also applied.
  //  - Displays the geometry defined in the config file, along with the generated and reconstructed tracks
  // AP 2015/03/11
  // Last Modified AP 2015/06/12: Possibility to have a negative momentum, meaning that the beam is in the negative Z direction. 
  //                              If momentum is positive then the beam is supposed to move in the positive Z direction.
  // Last Modified AP 2015/07/08: Added the possibility of adding a material to the medium containing the sensors.
  //                              Default value is Vacuum (no MS), a more reallistic value if DryAir
  
  BookingMC(PlaneNumber,Submatrix,Geomatrix,CalledFromMimosaPro);
  
  if(!DoGaussianMS) _PrintFreq = 500;
  
  string particle       = fSession->GetSetup()->GetTrackerPar().BeamType.Data();
  double momentum       = fSession->GetSetup()->GetTrackerPar().BeamMomentum;
  string MediumMaterial = fSession->GetSetup()->GetTrackerPar().MediumMaterial.Data();

  if(verbose) {
    TString Direction("");
    if(momentum > 0.0) Direction = TString("+Z");
    else               Direction = TString("-Z");
    cout << "MimosaGeneration:: Generating " << particle.data() << "'s with momentum " << momentum << " GeV/c" 
         << " and moving in " << Direction.Data()
         << endl;
  }

  DTracker *tTracker = fSession->GetTracker();
  const Int_t nPlanes      = fSession->GetTracker()->GetPlanesN(); // FIXME Antonin, 7 oct 2015

  if(nPlanes > MynPlanes) {
    cout << endl;
    cout << "MimosaGeneration:: number of planes " << nPlanes << " is higher than maximum " << MynPlanes << " defined in MHits.h. Check you inputs. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  DPlane* tPlane;
  DPrecAlign* tAlign;
  DR3 posInPlane, posInTracker;
  //Char_t tnum[20];

  //Generation of the tracks
  cout << endl;
  cout << "MimosaGeneration:: Generation Region:" << endl;
  cout << "MimosaGeneration:: X-range = (" << MyRX[0]*From_mu_to_mm << "," << MyRX[1]*From_mu_to_mm << ") mm" << endl;
  cout << "MimosaGeneration:: Y-range = (" << MyRY[0]*From_mu_to_mm << "," << MyRY[1]*From_mu_to_mm << ") mm" << endl;
  cout << endl;

  TVector3 TrackVect(0.0,0.0,0.0);
  TVector3 PlaneVect(0.0,0.0,0.0);
  TVector3 NormVect(0.0,0.0,0.0);

  //function of MS model which includes non-Gaussian tails
  TF1 f_MS_tails("f_MS_tails","x*DGlobalTools::GetDistribution(x,[0])",0.0,10.0);

  const int MaxEventDisplay(MyMaxEventDisplay);
  int NDisplay = MaxEvt;
  if(NDisplay > MaxEventDisplay) NDisplay = MaxEventDisplay;

  int NgoodGenTrk     = 0;
  int NgoodRecTrk     = 0;
  int NgoodRecTrk_Sel = 0;

  DR3 Origin(0.0,0.0,0.0);
  DR3 Slope(0.0,0.0,1.0);
  DTrack* aTrack = new DTrack(Origin,Slope);
  DTrack* aRecTrack = NULL;
  DHit*   aHit      = NULL;
  TRandom rand(seed);
  
  // FIXME Antonin, 7 oct 2015
  std::vector<DR3> TrueHitPosition;     TrueHitPosition.resize(nPlanes);
  std::vector<DR3> RecHitPosition ;     RecHitPosition. resize(nPlanes);
  
  for(Int_t iPlane=0; iPlane<nPlanes; iPlane++) {
    TrueHitPosition[iPlane] = DR3(0.0,0.0,0.0);
    RecHitPosition[iPlane]  = DR3(0.0,0.0,0.0);
  }
  NRectracks   = new int[MaxEvt];
  TracksPerEvt = new int[MaxEvt];
  std::vector<TVector3> _ListOfTrackPoints;

  //Track lines properties
  int LineStyle = 2;
  int LineWidth = 2;
  int RecLineStyle = 1;
  int RecLineWidth = 2;
  double Z1,Z2,X1,X2,Y1,Y2;
  
  int  Nevents = 0;
  bool HitDUT = false;
  DR3 HitPositionDUT(0.0,0.0,0.0);
  while(Nevents != MaxEvt) {
    double Xtmp = rand.Uniform(MyRX[0],MyRX[1]);
    double Ytmp = rand.Uniform(MyRY[0],MyRY[1]);

    bool GoodPoint = true;

    if(GoodPoint) {
      TracksPerEvt[Nevents] = 0;

      if(!((Nevents+1)%_PrintFreq)) cout << Nevents+1 << " events generated!!!" << endl;
      if(Nevents+1 == MaxEvt) {
        cout << endl;
        cout << Nevents+1 << " Total events generated!!!" << endl;
        cout << endl;
      }

      int mycolor = (Nevents+1==10)?49:(Nevents+1);

      if(verbose) {
	cout << "Evt " << Nevents+1 << ", Generating track at position (X,Y,Z) = (" 
	     << Xtmp*From_mu_to_mm << "," 
	     << Ytmp*From_mu_to_mm << ",";
	if(momentum > 0.0) cout << hXZ_MC->GetXaxis()->GetXmin();
	else               cout << hXZ_MC->GetXaxis()->GetXmax();
	cout << ") mm, color = " << mycolor;
        cout << endl;
      }

      TracksPerEvt[Nevents]++;
      _ListOfTrackPoints.clear();
      
      //Generate tracks with gaussian distribution of tilt w.r.t z-axis
      double XDiv = rand.Gaus(XMeanDiv,XRMSDiv);
      double YDiv = rand.Gaus(YMeanDiv,YRMSDiv);
      Origin.SetValue(Xtmp,Ytmp,0.0);
      Slope.SetValue(XDiv,YDiv,1.0);
      DTrack aTrack_init(Origin,Slope);
      aTrack->SetLinearFit2(aTrack_init.GetLinearFit2());

      double TheCurrentX = Xtmp;
      double TheCurrentY = Ytmp;
      double TheCurrentZ = hXZ_MC->GetXaxis()->GetXmin()/From_mu_to_mm;
      if(momentum < 0.0) TheCurrentZ = hXZ_MC->GetXaxis()->GetXmax()/From_mu_to_mm;
      double ThePreviousX = TheCurrentX;
      double ThePreviousY = TheCurrentY;
      double ThePreviousZ = TheCurrentZ;

      HitDUT = false;
      HitPositionDUT.SetValue(0.0,0.0,0.0);

      int NRefPlanesTouched = 0;
      bool GoodGenTrk = true;
      //Looping on the planes:
      for(int i=0;i<int(_PlaneList.size());i++) {
	int iPlane = _PlaneList[i];
	tPlane = fSession->GetTracker()->GetPlane(iPlane);
	tAlign = (DPrecAlign*)tPlane->GetPrecAlignment();

	if(TString(MediumMaterial.data()) != TString("Vacuum")) {
	  //1st simulated the interation of particle with the material of the medium containing the sensors (usually DryAir)
	  posInPlane     = tPlane->Intersection(aTrack);
	  posInTracker   = tAlign->TransformHitToTracker(posInPlane);
	  TVector3 NormVect_Medimum(aTrack->GetLinearFit().GetSlopeZ()(0),
	  		            aTrack->GetLinearFit().GetSlopeZ()(1),
			            aTrack->GetLinearFit().GetSlopeZ()(2));
	  NormVect_Medimum = NormVect_Medimum.Orthogonal();
	  NormVect_Medimum.Unit();
	  float MedimumThickness      = sqrt(pow(posInTracker(0) - ThePreviousX,2) + pow(posInTracker(1) - ThePreviousY,2) + pow(posInTracker(2) - ThePreviousZ,2));
	  double sigma_thetaMS_Medium = fTool.scatteringAngle(particle,TMath::Abs(momentum),MediumMaterial,MedimumThickness,verbose);
	
	  double thetaSM_Medium_gen,phi_Medium_gen,YMS_X1_gen,YMS_X2_gen;
	  if(DoGaussianMS) {
	    //Guassian model of MS angle
	    double z1_X1,z2_X1,z1_X2,z2_X2;
	    z1_X1 = rand.Gaus(0.0,1.0);
	    z2_X1 = rand.Gaus(0.0,1.0);
	    z1_X2 = rand.Gaus(0.0,1.0);
	    z2_X2 = rand.Gaus(0.0,1.0);
	  
	    double thetaSM_gen1 = z2_X1*sigma_thetaMS_Medium;
	    YMS_X1_gen   = z1_X1*MedimumThickness*(sigma_thetaMS_Medium/sqrt(12.0)) + z2_X1*MedimumThickness*(sigma_thetaMS_Medium/2.0);
	    double thetaSM_gen2 = z2_X2*sigma_thetaMS_Medium;
	    YMS_X2_gen   = z1_X2*MedimumThickness*(sigma_thetaMS_Medium/sqrt(12.0)) + z2_X2*MedimumThickness*(sigma_thetaMS_Medium/2.0);
	  
	    TVector2 tmp_vect(thetaSM_gen1,thetaSM_gen2);
	    thetaSM_Medium_gen = tmp_vect.Mod();
	    phi_Medium_gen     = tmp_vect.Phi();
	  }
	  else {
	    //MS model which includes non-Gaussian tails
	    double z1_X1,z2_X1,z1_X2,z2_X2;
	  
	    double B = fTool.GetBfromb(fTool.Getb(particle,MediumMaterial,TMath::Abs(momentum),MedimumThickness));
	    f_MS_tails.SetParameter(0,B);
	  
	    thetaSM_Medium_gen  = f_MS_tails.GetRandom();
	    thetaSM_Medium_gen *= sqrt(2.0)*sigma_thetaMS_Medium;
	    phi_Medium_gen      = rand.Uniform(-TMath::Pi(),TMath::Pi());
	  
	    double thetaSM_gen1 = thetaSM_Medium_gen*TMath::Cos(phi_Medium_gen);
	    double thetaSM_gen2 = thetaSM_Medium_gen*TMath::Sin(phi_Medium_gen);
	  
	    z1_X1 = rand.Gaus(0.0,1.0);
	    z2_X1 = thetaSM_gen1/sigma_thetaMS_Medium;
	    z1_X2 = rand.Gaus(0.0,1.0);
	    z2_X2 = thetaSM_gen2/sigma_thetaMS_Medium;
	  
	    YMS_X1_gen   = z1_X1*MedimumThickness*(sigma_thetaMS_Medium/sqrt(12.0)) + z2_X1*MedimumThickness*(sigma_thetaMS_Medium/2.0);
	    YMS_X2_gen   = z1_X2*MedimumThickness*(sigma_thetaMS_Medium/sqrt(12.0)) + z2_X2*MedimumThickness*(sigma_thetaMS_Medium/2.0);
	  }
	
	  TVector3 TrackVect_Medimum(aTrack->GetLinearFit().GetSlopeZ()(0),
				     aTrack->GetLinearFit().GetSlopeZ()(1),
				     aTrack->GetLinearFit().GetSlopeZ()(2));
          TVector3 Rot_TrackVect_Medimum = TrackVect_Medimum;
	  Rot_TrackVect_Medimum.Rotate(thetaSM_Medium_gen,NormVect_Medimum);
	  Rot_TrackVect_Medimum.Rotate(phi_Medium_gen,    TrackVect_Medimum);
	  DR3 Rot_DirectorVectTrack_Medimum(Rot_TrackVect_Medimum.X(),Rot_TrackVect_Medimum.Y(),Rot_TrackVect_Medimum.Z());
	  Rot_DirectorVectTrack_Medimum = Rot_DirectorVectTrack_Medimum*(1.0/Rot_DirectorVectTrack_Medimum(2));
	  TVector3 NormVect_Medimum2 = TrackVect_Medimum.Cross(NormVect_Medimum);
	  NormVect_Medimum2.Unit();
	  DR3 NewposInTracker(0.0,0.0,0.0);
	  NewposInTracker.SetValue(posInTracker(0) + YMS_X1_gen*NormVect_Medimum.X() + YMS_X2_gen*NormVect_Medimum2.X(),
			           posInTracker(1) + YMS_X1_gen*NormVect_Medimum.Y() + YMS_X2_gen*NormVect_Medimum2.Y(),
			           posInTracker(2) + YMS_X1_gen*NormVect_Medimum.Z() + YMS_X2_gen*NormVect_Medimum2.Z());
	  DR3 Origin_new_Medimum(0.0,0.0,0.0);
	  Origin_new_Medimum.SetValue(NewposInTracker(0),
			              NewposInTracker(1),
			              NewposInTracker(2));
	  DR3 Slope_new_Medimum = Rot_DirectorVectTrack_Medimum;
	  Origin_new_Medimum.SetValue(Origin_new_Medimum(0) - Slope_new_Medimum(0)*Origin_new_Medimum(2),
			              Origin_new_Medimum(1) - Slope_new_Medimum(1)*Origin_new_Medimum(2),
			              0.0);
	  DTrack aTrack_tmp_Medimum(Origin_new_Medimum,Slope_new_Medimum);
	  aTrack->SetLinearFit2(aTrack_tmp_Medimum.GetLinearFit2());
	
	  if(verbose) {
	    cout << "Evt = "    << Nevents+1 << ", " << "iPlane = " << iPlane << ". Multiple scattering with medium (" << MediumMaterial.data() << ")" << endl;
	    cout << "Intersection(U,V)             = (" 
	         << posInPlane(0)*From_mu_to_mm << "," 
	         << posInPlane(1)*From_mu_to_mm << "," 
	         << posInPlane(2)*From_mu_to_mm << ")mm" 
	         << endl;
	    cout << "Intersection(X,Y,Z)           = (" 
	         << posInTracker(0)*From_mu_to_mm << "," 
	         << posInTracker(1)*From_mu_to_mm << "," 
	         << posInTracker(2)*From_mu_to_mm << ")mm" 
	         << endl;
	    cout << "Origin_old                    = (" << Origin(0)*From_mu_to_mm   << "," << Origin(1)*From_mu_to_mm  << "," << Origin(2)*From_mu_to_mm << ")" << endl;
	    cout << "Track director vector         = (" << TrackVect_Medimum.X()     << "," << TrackVect_Medimum.Y()    << "," << TrackVect_Medimum.Z()   << ")" << endl;
	    cout << "Track othogonal vector        = (" << NormVect_Medimum.X()      << "," << NormVect_Medimum.Y()     << "," << NormVect_Medimum.Z()    << ")" << endl;
	    cout << "Medimum thickness             = " << MedimumThickness << "um" << endl;
	    cout << "Medimum material              = " << MediumMaterial.data() << endl;
	    cout << "medium sigma(theta0)          = " << sigma_thetaMS_Medium   << " rad, " << sigma_thetaMS_Medium*(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "medium theta_MS_gen_proj1     = " << thetaSM_Medium_gen*TMath::Cos(phi_Medium_gen) << " rad, " << thetaSM_Medium_gen*TMath::Cos(phi_Medium_gen)*(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "medium theta_MS_gen_proj2     = " << thetaSM_Medium_gen*TMath::Sin(phi_Medium_gen) << " rad, " << thetaSM_Medium_gen*TMath::Sin(phi_Medium_gen)*(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "medium theta_MS_gen           = " << thetaSM_Medium_gen     << " rad, " << thetaSM_Medium_gen  *(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "medium phi_gen                = " << phi_Medium_gen         << " rad, " << phi_Medium_gen      *(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "medium y_X1_gen               = " << YMS_X1_gen << "um" << endl;
	    cout << "medium y_X2_gen               = " << YMS_X2_gen << "um" << endl;
	    cout << "Rotated Track directot vector = (" << Rot_DirectorVectTrack_Medimum(0)  << "," << Rot_DirectorVectTrack_Medimum(1) << "," << Rot_DirectorVectTrack_Medimum(2) << ")" << endl;
	    cout << "Origin_new                    = (" 
	         << aTrack->GetLinearFit().GetOrigin()(0)*From_mu_to_mm << "," 
	         << aTrack->GetLinearFit().GetOrigin()(1)*From_mu_to_mm << "," 
	         << aTrack->GetLinearFit().GetOrigin()(2)*From_mu_to_mm << ")mm" << endl;
	    cout << "Slope_new                     = (" 
	         << aTrack->GetLinearFit().GetSlopeZ()(0) << "," 
	         << aTrack->GetLinearFit().GetSlopeZ()(1) << "," 
	         << aTrack->GetLinearFit().GetSlopeZ()(2) << ")" << endl;
	    cout << endl;
	  }
	}// end of if medium is different from vacuum
	  
	//Now check if trajectory scattered by the medium material hits a plane and then simulation the corresponding multiple scattering
	posInPlane     = tPlane->Intersection(aTrack);
	posInTracker   = tAlign->TransformHitToTracker(posInPlane);
	bool IsInPlane = false;
	ThePreviousX   = posInTracker(0);
	ThePreviousY   = posInTracker(1);
	ThePreviousZ   = posInTracker(2);
	
        DR3 Origin_new(0.0,0.0,0.0);
	
	if(verbose) {
	  cout << "Plane = " << iPlane << ", RU = (" << _RU_min[iPlane] << "," << _RU_max[iPlane] << ")mu"
	       << endl;
	  cout << "Plane = " << iPlane << ", RV = (" << _RV_min[iPlane] << "," << _RV_max[iPlane] << ")mu"
	       << endl;
	  cout << "Plane = " << iPlane << ", Intersection = (" << posInPlane(0) << "," << posInPlane(1) << ")um"
	       << endl;
	}

	if((posInPlane(0) >= _RU_min[i] && posInPlane(0) <= _RU_max[i]) && 
	   (posInPlane(1) >= _RV_min[i] && posInPlane(1) <= _RV_max[i])) IsInPlane = true;

	if(tPlane->GetStatus() < 3) {
	  if(!IsInPlane) NRefPlanesTouched++;
	}

	//Saving true hit position of track on DUT
	if(iPlane == PlaneNumber) {
	  if(IsInPlane) {
	    HitDUT = true;
	    HitPositionDUT.SetValue(posInPlane(0),posInPlane(1),posInPlane(2));
	  }
	  else HitPositionDUT.SetValue(-999.0,-999.0,-999.0);
	}

	double *RotMat = tAlign->GetRotationMatrix();
	DR3 normVectPlane(0.0,0.0,0.0);
	if(tAlign->GetDPrecAlignMethod() == 0) normVectPlane.SetValue(RotMat[6],RotMat[7],RotMat[8]);
	else                                   normVectPlane.SetValue(RotMat[2],RotMat[5],RotMat[8]);
	normVectPlane              = normVectPlane*(1.0/normVectPlane.Length());
	DR3 DirectorVectTrack      = aTrack->GetLinearFit().GetSlopeZ();
	DirectorVectTrack          = DirectorVectTrack*(1.0/DirectorVectTrack.Length());
	double CosTheta_TrackPlane = DirectorVectTrack.InnerProduct(normVectPlane);

	TrackVect.SetXYZ(DirectorVectTrack(0),DirectorVectTrack(1),DirectorVectTrack(2));
	PlaneVect.SetXYZ(normVectPlane(0),    normVectPlane(1),    normVectPlane(2));
	if(TMath::Abs(TMath::Abs(CosTheta_TrackPlane) - 1) < 1.0e-6) NormVect = TrackVect.Orthogonal();
	else                                                         NormVect = TrackVect.Cross(PlaneVect);
	NormVect = NormVect.Unit();
	DR3 NormalVector(NormVect.X(),NormVect.Y(),NormVect.Z());

	double Eff_Plane_thickness = _PlaneThickness[i]/TMath::Abs(CosTheta_TrackPlane);
	string Material            = _PlaneMaterial[i].Data();
	double sigma_thetaMS       = fTool.scatteringAngle(particle,TMath::Abs(momentum),Material,Eff_Plane_thickness,verbose&IsInPlane);

	double thetaSM_gen,phi_gen;
	if(DoGaussianMS) {
	  //Guassian model of MS angle
	  double thetaSM_gen1        = rand.Gaus(0.0,sigma_thetaMS);
	  double thetaSM_gen2        = rand.Gaus(0.0,sigma_thetaMS);
	  TVector2 tmp_vect(thetaSM_gen1,thetaSM_gen2);
	  thetaSM_gen                = tmp_vect.Mod();
	  phi_gen                    = tmp_vect.Phi();
	}
	else {
	  //MS model which includes non-Gaussian tails
	  double B = fTool.GetBfromb(fTool.Getb(particle,Material,TMath::Abs(momentum),Eff_Plane_thickness));
	  f_MS_tails.SetParameter(0,B);

	  thetaSM_gen  = f_MS_tails.GetRandom();
	  thetaSM_gen *= sqrt(2.0)*sigma_thetaMS;
	  phi_gen      = rand.Uniform(-TMath::Pi(),TMath::Pi());
	}
	
	if(!IsInPlane) {
	  thetaSM_gen = 1.0e-10;
	  phi_gen     = 1.0e-10;
	}

	TVector3 Rot_TrackVect = TrackVect;
	Rot_TrackVect.Rotate(thetaSM_gen,NormVect);
	Rot_TrackVect.Rotate(phi_gen,    TrackVect);
	DR3 Rot_DirectorVectTrack(Rot_TrackVect.X(),Rot_TrackVect.Y(),Rot_TrackVect.Z());
	Rot_DirectorVectTrack = Rot_DirectorVectTrack*(1.0/Rot_DirectorVectTrack(2));	  
	Origin_new.SetValue(posInTracker(0),
			    posInTracker(1),
			    posInTracker(2));
	DR3 Slope_new = Rot_DirectorVectTrack;
	Origin_new.SetValue(Origin_new(0) - Slope_new(0)*Origin_new(2),
			    Origin_new(1) - Slope_new(1)*Origin_new(2),
			    0.0);

	DTrack aTrack_tmp(Origin_new,Slope_new);
	aTrack->SetLinearFit2(aTrack_tmp.GetLinearFit2());

	if(IsInPlane) {
	  if(verbose) {
	    cout << "Evt = "    << Nevents+1 << ", " << "iPlane = " << iPlane  << endl;
	    cout << "Intersection(U,V)             = (" 
		 << posInPlane(0)*From_mu_to_mm << "," 
		 << posInPlane(1)*From_mu_to_mm << "," 
		 << posInPlane(2)*From_mu_to_mm << ")mm" 
		 << endl;
	    cout << "Intersection(X,Y,Z)           = (" 
		 << posInTracker(0)*From_mu_to_mm << "," 
		 << posInTracker(1)*From_mu_to_mm << "," 
		 << posInTracker(2)*From_mu_to_mm << ")mm" 
		 << endl;
	    cout << "Origin_old                    = (" << Origin(0)*From_mu_to_mm   << "," << Origin(1)*From_mu_to_mm   << "," << Origin(2)*From_mu_to_mm << ")" << endl;
	    cout << "Track director vector         = (" << DirectorVectTrack(0) << "," << DirectorVectTrack(1) << "," << DirectorVectTrack(2) << ")" << endl;
	    cout << "Plane normal   vector         = (" << normVectPlane(0)     << "," << normVectPlane(1)     << "," << normVectPlane(2) << ")" << endl;
	    cout << "Orthogonal     vector         = (" << NormalVector(0)      << "," << NormalVector(1)      << "," << NormalVector(2) << ")" << endl;
	    cout << "Cos(theta) of track and plane = " << CosTheta_TrackPlane << ", => theta = " << TMath::ACos(CosTheta_TrackPlane)*(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "Plane thickness               = " << _PlaneThickness[i] << "um, effective thickness view by track = " << Eff_Plane_thickness << "um" << endl;
	    cout << "Plane material                = " << Material.data() << endl;
	    cout << "sigma(theta0)                 = " << sigma_thetaMS   << " rad, " << sigma_thetaMS*(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "theta_MS_gen_proj1            = " << thetaSM_gen*TMath::Cos(phi_gen) << " rad, " << thetaSM_gen*TMath::Cos(phi_gen)*(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "theta_MS_gen_proj2            = " << thetaSM_gen*TMath::Sin(phi_gen) << " rad, " << thetaSM_gen*TMath::Sin(phi_gen)*(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "theta_MS_gen                  = " << thetaSM_gen     << " rad, " << thetaSM_gen  *(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "phi_gen                       = " << phi_gen         << " rad, " << phi_gen      *(180.0/TMath::Pi()) << " degrees" << endl;
	    cout << "Rotated Track directot vector = (" << Rot_DirectorVectTrack(0)      << "," << Rot_DirectorVectTrack(1)      << "," << Rot_DirectorVectTrack(2)    << ")" << endl;
	    cout << "Origin_new                    = (" 
		 << aTrack->GetLinearFit().GetOrigin()(0)*From_mu_to_mm << "," 
		 << aTrack->GetLinearFit().GetOrigin()(1)*From_mu_to_mm << "," 
 		 << aTrack->GetLinearFit().GetOrigin()(2)*From_mu_to_mm << ")mm" << endl;
	    cout << "Slope_new                     = (" 
		 << aTrack->GetLinearFit().GetSlopeZ()(0) << "," 
		 << aTrack->GetLinearFit().GetSlopeZ()(1) << "," 
		 << aTrack->GetLinearFit().GetSlopeZ()(2) << ")" << endl;
	    cout << endl;
	  }
	}
	else {
	  if(verbose) {
	    cout << "Evt = "    << Nevents+1 << ", " << "iPlane = " << iPlane  << endl;
	    cout << "No intersecction of track with plane" << endl;
	    cout << endl;
	  }
	}

	if(IsInPlane) {
	  TrueHitPosition[iPlane-1].SetValue(posInPlane(0),posInPlane(1),posInPlane(2));

	  X1 = TheCurrentX*From_mu_to_mm;
	  //X2 = posInTracker(0)*From_mu_to_mm;
	  Y1 = TheCurrentY*From_mu_to_mm;
	  //Y2 = posInTracker(1)*From_mu_to_mm;
	  Z1 = TheCurrentZ*From_mu_to_mm;
	  //Z2 = posInTracker(2)*From_mu_to_mm;
	  
	  if(Nevents+1 <= MaxEventDisplay) {
	    TVector3 tmp_PosVector(X1,Y1,Z1);
	    _ListOfTrackPoints.push_back(tmp_PosVector);
	  }

	  TheCurrentX = posInTracker(0);
	  TheCurrentY = posInTracker(1);
	  TheCurrentZ = posInTracker(2);

	  Origin = Origin_new;
	}
	else TrueHitPosition[iPlane-1].SetValue(-999.0,-999.0,-999.0);
      } // end of loop on the planes

      if(NRefPlanesTouched < fSession->GetSetup()->GetTrackerPar().PlanesForTrackMinimum) GoodGenTrk = false;
      if(GoodGenTrk) NgoodGenTrk++;

      //After the last plane:
      if(momentum > 0.0) {
        posInTracker.SetValue(aTrack->GetLinearFit().GetOrigin()(0) + aTrack->GetLinearFit().GetSlopeZ()(0)*(hXZ_MC->GetXaxis()->GetXmax()/From_mu_to_mm),
			      aTrack->GetLinearFit().GetOrigin()(1) + aTrack->GetLinearFit().GetSlopeZ()(1)*(hXZ_MC->GetXaxis()->GetXmax()/From_mu_to_mm),
			      aTrack->GetLinearFit().GetOrigin()(2) + aTrack->GetLinearFit().GetSlopeZ()(2)*(hXZ_MC->GetXaxis()->GetXmax()/From_mu_to_mm));
      }
      else {
	posInTracker.SetValue(aTrack->GetLinearFit().GetOrigin()(0) + aTrack->GetLinearFit().GetSlopeZ()(0)*(hXZ_MC->GetXaxis()->GetXmin()/From_mu_to_mm),
			      aTrack->GetLinearFit().GetOrigin()(1) + aTrack->GetLinearFit().GetSlopeZ()(1)*(hXZ_MC->GetXaxis()->GetXmin()/From_mu_to_mm),
			      aTrack->GetLinearFit().GetOrigin()(2) + aTrack->GetLinearFit().GetSlopeZ()(2)*(hXZ_MC->GetXaxis()->GetXmin()/From_mu_to_mm));
      }
      if(verbose) {
	cout << "Last position  = (" << posInTracker(0)*From_mu_to_mm << "," << posInTracker(1)*From_mu_to_mm << "," << posInTracker(2)*From_mu_to_mm << ")mm" << endl;
	cout << endl;
      }

      //X1 = TheCurrentX*From_mu_to_mm;
      X2 = posInTracker(0)*From_mu_to_mm;
      //Y1 = TheCurrentY*From_mu_to_mm;
      Y2 = posInTracker(1)*From_mu_to_mm;
      //Z1 = TheCurrentZ*From_mu_to_mm;
      Z2 = posInTracker(2)*From_mu_to_mm;

      if(Nevents+1 <= MaxEventDisplay) {
	TVector3 tmp_PosVector(X2,Y2,Z2);
        _ListOfTrackPoints.push_back(tmp_PosVector);
      }

      if(verbose) {
	cout << "Evt = " << Nevents+1 << ", number of track segments = " << _ListOfTrackPoints.size() << endl;
	cout << endl;
      }

      if(Nevents+1 <= MaxEventDisplay) {
        //Filling up the TGraph with the trajectories:
        TString TmpName;
        TmpName = TString("gtracksXZ_evt") + long(Nevents+1) + TString("_trk") + long(TracksPerEvt[Nevents]);
        gtracksXZ[Nevents][TracksPerEvt[Nevents]-1] = new TGraph(int(_ListOfTrackPoints.size()));
        gtracksXZ[Nevents][TracksPerEvt[Nevents]-1]->SetName(TmpName.Data());
        gtracksXZ[Nevents][TracksPerEvt[Nevents]-1]->SetLineColor(mycolor);
        gtracksXZ[Nevents][TracksPerEvt[Nevents]-1]->SetLineWidth(LineWidth);
        gtracksXZ[Nevents][TracksPerEvt[Nevents]-1]->SetLineStyle(LineStyle);
      
        TmpName = TString("gtracksYZ_evt") + long(Nevents+1) + TString("_trk") + long(TracksPerEvt[Nevents]);
        gtracksYZ[Nevents][TracksPerEvt[Nevents]-1] = new TGraph(int(_ListOfTrackPoints.size()));
        gtracksYZ[Nevents][TracksPerEvt[Nevents]-1]->SetName(TmpName.Data());
        gtracksYZ[Nevents][TracksPerEvt[Nevents]-1]->SetLineColor(mycolor);
        gtracksYZ[Nevents][TracksPerEvt[Nevents]-1]->SetLineWidth(LineWidth);
        gtracksYZ[Nevents][TracksPerEvt[Nevents]-1]->SetLineStyle(LineStyle);

        for(int ipoint=0;ipoint<int(_ListOfTrackPoints.size());ipoint++) {
	  gtracksXZ[Nevents][TracksPerEvt[Nevents]-1]->SetPoint(ipoint,_ListOfTrackPoints[ipoint].Z(),_ListOfTrackPoints[ipoint].X());
	  gtracksYZ[Nevents][TracksPerEvt[Nevents]-1]->SetPoint(ipoint,_ListOfTrackPoints[ipoint].Z(),_ListOfTrackPoints[ipoint].Y());
        }
      }
      
      if(!UsingTrackerResolution) {
	for(int i=0;i<int(_PlaneList.size());i++) {
	  int iPlane = _PlaneList[i];
	  tPlane = fSession->GetTracker()->GetPlane(iPlane);
	  
	  bool IsInPlane = true;
	  if(TrueHitPosition[iPlane-1](0) == -999.0 && 
	     TrueHitPosition[iPlane-1](1) == -999.0 && 
	     TrueHitPosition[iPlane-1](2) == -999.0) IsInPlane = false;
	  
	  if(IsInPlane) {
	    DR3 HitPosition(0.0,0.0,0.0);
	    DR3 HitResolution(0.0,0.0,0.0);
	    
	    //Setting hit resolution. Only for digitized output
	    double u,v,col,lin;
	    u = TrueHitPosition[iPlane-1](0);
	    v = TrueHitPosition[iPlane-1](1);
	    tPlane->ComputeStripPosition_UVToColRow(u,v,col,lin);
	
	    int ClusterMult = tPlane->GenerateMCMultiplicity(col,lin);
	    TVector2 MyHitResolution(0.0,0.0);
	    tPlane->GetHitResolution(col,lin,ClusterMult,MyHitResolution);
	    HitResolution.SetValue(MyHitResolution.X(),MyHitResolution.Y(),0.0);

	    RecHitPosition[iPlane-1].SetValue(rand.Gaus(TrueHitPosition[iPlane-1](0),HitResolution(0)),
					      rand.Gaus(TrueHitPosition[iPlane-1](1),HitResolution(1)),
					      0.0);
	    HitPosition.SetValue(RecHitPosition[iPlane-1](0),RecHitPosition[iPlane-1](1),RecHitPosition[iPlane-1](2));
	    
	    tPlane->SetHitsN(0);
	    tPlane->AddMCGeneratedHit(HitPosition,HitResolution);
	  }
	  else RecHitPosition[iPlane-1].SetValue(-999.0,-999.0,-999.0);
	}

	if(verbose) {
	  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
	    tPlane = fSession->GetTracker()->GetPlane(iPlane);
	    cout << "Evt = " << Nevents+1 << ",  "
		 << "Plane " << iPlane << " has " << tPlane->GetHitsN() << " hits"
		 << endl;
	    for(int iHit=1;iHit<=tPlane->GetHitsN();iHit++) {
	      aHit = (DHit*)tPlane->GetHit(iHit);
	      cout << "Hit = " << iHit << " is at position(U,V) = (" 
		   << aHit->GetPositionUhit() << ","
		   << aHit->GetPositionVhit() << ")um, and has resoluton(U,V) = ("
		   << aHit->GetResolutionUhit() << ","
		   << aHit->GetResolutionVhit() << ")um"
		   << endl;
	    }
	  }
	  cout << endl;
	}

	tTracker->UpdateMC();
	if(verbose) {
	  cout << "Evt = " << Nevents+1 << ", Tracker has reconstructed " << tTracker->GetTracksN() << " tracks" << endl;
	  cout << endl;
	}
	NRectracks[Nevents] = tTracker->GetTracksN();
	NgoodRecTrk += tTracker->GetTracksN();

	Z1 = hXZ_MC->GetXaxis()->GetXmin();
	Z2 = hXZ_MC->GetXaxis()->GetXmax();
	for(Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++) {// loop on reconstructed tracks
	  aRecTrack = tTracker->GetTrack(iTrack);
	  
	  X1 = aRecTrack->GetLinearFit().GetOrigin()(0) + aRecTrack->GetLinearFit().GetSlopeZ()(0)*(Z1/From_mu_to_mm);
	  X2 = aRecTrack->GetLinearFit().GetOrigin()(0) + aRecTrack->GetLinearFit().GetSlopeZ()(0)*(Z2/From_mu_to_mm);
	  Y1 = aRecTrack->GetLinearFit().GetOrigin()(1) + aRecTrack->GetLinearFit().GetSlopeZ()(1)*(Z1/From_mu_to_mm);
	  Y2 = aRecTrack->GetLinearFit().GetOrigin()(1) + aRecTrack->GetLinearFit().GetSlopeZ()(1)*(Z2/From_mu_to_mm);
	  
	  X1 *= From_mu_to_mm;
	  X2 *= From_mu_to_mm;
	  Y1 *= From_mu_to_mm;
	  Y2 *= From_mu_to_mm;
	  
	  if(Nevents+1 <= MaxEventDisplay) {
	    RectracksXZ[Nevents][iTrack-1] = new TLine(Z1,X1,Z2,X2);
	    RectracksXZ[Nevents][iTrack-1]->SetLineColor(mycolor);
	    RectracksXZ[Nevents][iTrack-1]->SetLineWidth(RecLineWidth);
	    RectracksXZ[Nevents][iTrack-1]->SetLineStyle(RecLineStyle);
	    
	    RectracksYZ[Nevents][iTrack-1] = new TLine(Z1,Y1,Z2,Y2);
	    RectracksYZ[Nevents][iTrack-1]->SetLineColor(mycolor);
	    RectracksYZ[Nevents][iTrack-1]->SetLineWidth(RecLineWidth);
	    RectracksYZ[Nevents][iTrack-1]->SetLineStyle(RecLineStyle);
	  }
	  
	  DR3 trackSlope  = aRecTrack->GetLinearFit().GetSlopeZ();
	  hTrackSlopeX_MC->Fill(trackSlope(0));
	  hTrackSlopeY_MC->Fill(trackSlope(1));
	  hTrackNHits_MC->Fill(aRecTrack->GetHitsNumber());
	  hTrackChi2_MC->Fill(aRecTrack->GetChiSquare());

	  if(aRecTrack->GetChiSquare() < TrackChi2Limit) NgoodRecTrk_Sel++;

	  if(HitDUT) {
	    tPlane     = fSession->GetTracker()->GetPlane(PlaneNumber);
	    posInPlane = tPlane->Intersection(aRecTrack);

	    if(verbose) {
	      cout << iTrack << " true-track intersection with DUT is (U,V) = (" << HitPositionDUT(0) << "," << HitPositionDUT(1) << ")um" << endl;
	      cout << iTrack << " rec-track  intersection with DUT is (U,V) = (" << posInPlane(0)     << "," << posInPlane(1) << ")um" << endl;
	      cout << iTrack << " residual with DUT hit (U,V)               = (" << posInPlane(0) - HitPositionDUT(0) << "," << posInPlane(1) - HitPositionDUT(1) << ")um" << endl;
	      cout << endl << endl;
	    }
	    if(aRecTrack->GetChiSquare() < TrackChi2Limit) {
	      hTrackResidualAtDUT_U[0]->Fill(posInPlane(0) - HitPositionDUT(0));
	      hTrackResidualAtDUT_V[0]->Fill(posInPlane(1) - HitPositionDUT(1));
	    }
	  }

	  for(Int_t iHit=0; iHit<aRecTrack->GetHitsNumber();iHit++ ) {
	    aHit   = (DHit*)aRecTrack->GetHit( iHit);
	    tPlane = (DPlane*)aHit->GetPlane();
	    hTrackPlanesUsed_MC->Fill(tPlane->GetPlaneNumber());
	    DR3 impactPosition = aRecTrack->Intersection(tPlane);
	    impactPosition -= *(aHit->GetPosition());
	    hResidualU_MC[tPlane->GetPlaneNumber()-1]->Fill( impactPosition(0));
	    hResidualV_MC[tPlane->GetPlaneNumber()-1]->Fill( impactPosition(1));
	  }
	} //end  loop on reconstructed tracks
      } //End of if UsingTrackerResolution
      else {
	for(int istep=0;istep<Bins_GlobalResolution;istep++) {
	  double TheResolution = RGlobalResolution[0] + (istep+0.5)*(RGlobalResolution[1] - RGlobalResolution[0])/Bins_GlobalResolution;

	  for(int i=0;i<int(_PlaneList.size());i++) {
	    int iPlane = _PlaneList[i];
	    tPlane = fSession->GetTracker()->GetPlane(iPlane);

	    bool IsInPlane = true;
	    if(TrueHitPosition[iPlane-1](0) == -999.0 && 
	       TrueHitPosition[iPlane-1](1) == -999.0 && 
	       TrueHitPosition[iPlane-1](2) == -999.0) IsInPlane = false;
	    
	    if(IsInPlane) {
	      DR3 HitPosition(0.0,0.0,0.0);
	      DR3 HitResolution(0.0,0.0,0.0);

	      //Setting hit resolution. Only for digitized output
	      double u,v,col,lin;
	      u = TrueHitPosition[iPlane-1](0);
	      v = TrueHitPosition[iPlane-1](1);
	      tPlane->ComputeStripPosition_UVToColRow(u,v,col,lin);

	      int ClusterMult = tPlane->GenerateMCMultiplicity(col,lin);
	      TVector2 MyHitResolution(0.0,0.0);
	      tPlane->GetHitResolution(col,lin,ClusterMult,MyHitResolution);
	      HitResolution.SetValue(MyHitResolution.X(),MyHitResolution.Y(),0.0);

	      RecHitPosition[iPlane-1].SetValue(rand.Gaus(TrueHitPosition[iPlane-1](0),TheResolution),
						rand.Gaus(TrueHitPosition[iPlane-1](1),TheResolution),
						0.0);
	      HitPosition.SetValue(RecHitPosition[iPlane-1](0),RecHitPosition[iPlane-1](1),RecHitPosition[iPlane-1](2));
	      
	      tPlane->SetHitsN(0);
	      tPlane->AddMCGeneratedHit(HitPosition,HitResolution);
	    }
	    else RecHitPosition[iPlane-1].SetValue(-999.0,-999.0,-999.0);
	  }

	  if(verbose) {
	    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
	      tPlane = fSession->GetTracker()->GetPlane(iPlane);
	      cout << "Evt = " << Nevents+1 << ",  "
		   << "Scan step = " << istep+1 << ",  "
		   << "Plane " << iPlane << " has " << tPlane->GetHitsN() << " hits"
		   << endl;
	      for(int iHit=1;iHit<=tPlane->GetHitsN();iHit++) {
		aHit = (DHit*)tPlane->GetHit(iHit);
		cout << "Hit = " << iHit << " is at position(U,V) = (" 
		     << aHit->GetPositionUhit() << ","
		     << aHit->GetPositionVhit() << ")um, and has resoluton(U,V) = ("
		     << aHit->GetResolutionUhit() << ","
		     << aHit->GetResolutionVhit() << ")um"
		     << endl;
	      }
	    }
	    cout << endl;
	  }

	  tTracker->UpdateMC();
	  if(verbose) {
	    cout << "Evt = " << Nevents+1 << ", Tracker has reconstructed " << tTracker->GetTracksN() << " tracks" << endl;
	    cout << endl;
	  }

	  for(Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++) {// loop on reconstructed tracks
	    aRecTrack = tTracker->GetTrack(iTrack);

	    if(HitDUT) {
	      tPlane     = fSession->GetTracker()->GetPlane(PlaneNumber);
	      posInPlane = tPlane->Intersection(aRecTrack);
	      if(aRecTrack->GetChiSquare() < TrackChi2Limit) {
		hTrackResidualAtDUT_U[istep]->Fill(posInPlane(0) - HitPositionDUT(0));
		hTrackResidualAtDUT_V[istep]->Fill(posInPlane(1) - HitPositionDUT(1));
	      }
	    }

	  } //end  loop on reconstructed tracks
	} // End of loop of steps for sensor spatial resolution scan
      }// End of else UsingTrackerResolution 
      Nevents++;
    } //End of if Good Point
  } // End of While

  double MaxRMS = -1.0e+20;
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    double TheRMS;
    if(hTrackResidualAtDUT_U[istep]->Integral() > 10.) TheRMS = hTrackResidualAtDUT_U[istep]->GetRMS();
    else                                               TheRMS = 1.0;
    if(MaxRMS < TheRMS) MaxRMS = TheRMS;

    if(hTrackResidualAtDUT_V[istep]->Integral() > 10.) TheRMS = hTrackResidualAtDUT_V[istep]->GetRMS();
    else                                               TheRMS = 1.0;
    if(MaxRMS < TheRMS) MaxRMS = TheRMS;
  }
  
  double TheRange = 50.0;
  if(TheRange < Nsigma*MaxRMS) TheRange = Nsigma*MaxRMS;
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    hTrackResidualAtDUT_U[istep]->SetAxisRange(-TheRange,+TheRange,"X");
    hTrackResidualAtDUT_V[istep]->SetAxisRange(-TheRange,+TheRange,"X");
  }
  
  if(!UsingTrackerResolution) {
    double TrkEffic[2];
    double TrkEffic_Sel[2];
    if(NgoodGenTrk > 0) {
      TrkEffic[0] = (double)NgoodRecTrk/NgoodGenTrk;
      TrkEffic[1] = sqrt(TrkEffic[0]*(1.0 - TrkEffic[0])/NgoodGenTrk);
      TrkEffic_Sel[0] = (double)NgoodRecTrk_Sel/NgoodGenTrk;
      TrkEffic_Sel[1] = sqrt(TrkEffic_Sel[0]*(1.0 - TrkEffic_Sel[0])/NgoodGenTrk);
    }
    else {
      TrkEffic[0] = -1.0;
      TrkEffic[1] =  0.0;
      TrkEffic_Sel[0] = -1.0;
      TrkEffic_Sel[1] =  0.0;
    }

    cout << endl;
    cout << endl;
    cout << "===================================================================" << endl;
    cout << "Tracking efficiency from MC simulation:  " << endl;
    cout << "IMPORTANT: no single hit detection efficiency included. Effect included:" << endl;
    cout << "           - Single point resolution," << endl;
    cout << "           - Multiple scattering," << endl;
    cout << "           - Track selection (Trk Chi2 cut = " << TrackChi2Limit << ")" << endl;
    cout << "-------------------------------------------------------------------" << endl;
    cout << "Nb of generated good tracks             =  " << NgoodGenTrk << ", out of " << MaxEvt << endl;
    cout << "Nb of reconstructed  tracks             =  " << NgoodRecTrk << endl;
    cout << "Nb of reconstructed and selected tracks =  " << NgoodRecTrk_Sel << endl;
    cout << "Tracking efficiency                     = (" << TrkEffic[0]*100.0 << " +/- " << TrkEffic[1]*100.0 << ")%" << endl;
    cout << "Tracking efficiency (with selection)    = (" << TrkEffic_Sel[0]*100.0 << " +/- " << TrkEffic_Sel[1]*100.0 << ")%" << endl;
    cout << "===================================================================" << endl;
    cout << "Estimated Track-hit resolution(1st DUT) from track Fitter:  " << endl;
    cout << "SigmaX:                                 = " << aRecTrack->GetDeltaOrigineX() << endl;
    cout << "SigmaY:                                 = " << aRecTrack->GetDeltaOrigineY() << endl;
    cout << "===================================================================" << endl;
    cout << endl;
    cout << endl;
  }

  TString TheDoubleGaussian("[0]*(1.0/sqrt(2*TMath::Pi()))*( [4]*(1.0/[2])*exp(-0.5*pow((x-[1])/[2],2)) +  (1.0 - [4])*(1.0/[3])*exp(-0.5*pow((x-[1])/[3],2)) )");
  
  //-- Draw
  TF1 *fitgs = NULL;
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    if(hTrackResidualAtDUT_U[istep]->Integral() > 10.) {
      cout << endl;
      cout << endl;
      cout << "Fitting Track U-residual at DUT position" << endl;
      if(DoGaussianMS) {
	hTrackResidualAtDUT_U[istep]->Fit("gaus");
	cout << endl;
	cout << endl;
	fitgs = hTrackResidualAtDUT_U[istep]->GetFunction("gaus");
	fitgs->SetParNames("N","#mu (#mum)","#sigma (#mum)");
	h_TelescopeResolution_Scan_U->SetBinContent(istep+1,fitgs->GetParameter(2));
	h_TelescopeResolution_Scan_U->SetBinError(istep+1,  fitgs->GetParError(2));
      }
      else {
	TF1* fres_U = new TF1("gaus",
			      TheDoubleGaussian.Data(),
			      -Nsigma*hTrackResidualAtDUT_U[istep]->GetRMS(),
			      +Nsigma*hTrackResidualAtDUT_U[istep]->GetRMS());
	fres_U->SetParNames("N","#mu (#mum)","#sigma_{1} (#mum)","#sigma_{2} (#mum)","f_{1}");
	fres_U->SetParameters(hTrackResidualAtDUT_U[istep]->Integral(),
			      0.0,
			      hTrackResidualAtDUT_U[istep]->GetRMS(),
			      2.0*hTrackResidualAtDUT_U[istep]->GetRMS(),
			      0.8);
	fres_U->SetParLimits(0,0.0,1.0*hTrackResidualAtDUT_U[istep]->Integral());
	fres_U->SetParLimits(1,-hTrackResidualAtDUT_U[istep]->GetRMS(),+hTrackResidualAtDUT_U[istep]->GetRMS());
	fres_U->SetParLimits(2,0.0,20.0*hTrackResidualAtDUT_U[istep]->GetRMS());
	fres_U->SetParLimits(3,0.0,20.0*hTrackResidualAtDUT_U[istep]->GetRMS());
	fres_U->SetParLimits(4,0.0,1.0);
	hTrackResidualAtDUT_U[istep]->Fit(fres_U,"R");
	h_TelescopeResolution_Scan_U->SetBinContent(istep+1,hTrackResidualAtDUT_U[istep]->GetRMS());
	h_TelescopeResolution_Scan_U->SetBinError(istep+1,  hTrackResidualAtDUT_U[istep]->GetRMSError());
      }

      double tot_res  = pow(h_TelescopeResolution_Scan_U->GetBinContent(istep+1),2);
      tot_res        += pow(h_TelescopeResolution_Scan_U->GetBinCenter(istep+1),2);
      tot_res         = sqrt(tot_res);
      h_ResidualAtDUT_Scan_U->SetBinContent(istep+1,tot_res);
      //h_ResidualAtDUT_Scan_U->SetBinError(istep+1,  fitgs->GetParError(2));
      h_ResidualAtDUT_Scan_U->SetBinError(istep+1, hTrackResidualAtDUT_U[istep]->GetRMSError());
    }

    if(hTrackResidualAtDUT_V[istep]->Integral() > 10.) {
      cout << endl;
      cout << endl;
      cout << "Fitting Track V-residual at DUT position" << endl;
      if(DoGaussianMS) {
	hTrackResidualAtDUT_V[istep]->Fit("gaus");
	cout << endl;
	cout << endl;
	fitgs = hTrackResidualAtDUT_V[istep]->GetFunction("gaus");
	fitgs->SetParNames("N","#mu (#mum)","#sigma (#mum)");
	h_TelescopeResolution_Scan_V->SetBinContent(istep+1,fitgs->GetParameter(2));
	h_TelescopeResolution_Scan_V->SetBinError(istep+1,fitgs->GetParError(2));
      }
      else {
	TF1* fres_V = new TF1("gaus",
			      TheDoubleGaussian.Data(),
			      -Nsigma*hTrackResidualAtDUT_V[istep]->GetRMS(),
			      +Nsigma*hTrackResidualAtDUT_V[istep]->GetRMS());
	fres_V->SetParNames("N","#mu (#mum)","#sigma_{1} (#mum)","#sigma_{2} (#mum)","f_{1}");
	fres_V->SetParameters(hTrackResidualAtDUT_V[istep]->Integral(),
			      0.0,
			      hTrackResidualAtDUT_V[istep]->GetRMS(),
			      2.0*hTrackResidualAtDUT_V[istep]->GetRMS(),
			      0.8);
	fres_V->SetParLimits(0,0.0,1.0*hTrackResidualAtDUT_V[istep]->Integral());
	fres_V->SetParLimits(1,-hTrackResidualAtDUT_V[istep]->GetRMS(),+hTrackResidualAtDUT_V[istep]->GetRMS());
	fres_V->SetParLimits(2,0.0,20.0*hTrackResidualAtDUT_V[istep]->GetRMS());
	fres_V->SetParLimits(3,0.0,20.0*hTrackResidualAtDUT_V[istep]->GetRMS());
	fres_V->SetParLimits(4,0.0,1.0);
	hTrackResidualAtDUT_V[istep]->Fit(fres_V,"R");
	h_TelescopeResolution_Scan_V->SetBinContent(istep+1,hTrackResidualAtDUT_V[istep]->GetRMS());
	h_TelescopeResolution_Scan_V->SetBinError(istep+1,  hTrackResidualAtDUT_V[istep]->GetRMSError());
      }
      
      double tot_res  = pow(h_TelescopeResolution_Scan_V->GetBinContent(istep+1),2);
      tot_res        += pow(h_TelescopeResolution_Scan_V->GetBinCenter(istep+1),2);
      tot_res         = sqrt(tot_res);
      h_ResidualAtDUT_Scan_V->SetBinContent(istep+1,tot_res);
      //h_ResidualAtDUT_Scan_V->SetBinError(istep+1,  fitgs->GetParError(2));
      h_ResidualAtDUT_Scan_V->SetBinError(istep+1,  hTrackResidualAtDUT_V[istep]->GetRMSError());
    }

    //if(hTrackResidualAtDUT_U[istep]->Integral() > 10.) hTrackResidualAtDUT_U[istep]->Fit("gaus");
    //if(hTrackResidualAtDUT_V[istep]->Integral() > 10.) hTrackResidualAtDUT_V[istep]->Fit("gaus");
  }
    
  if(CalledFromMimosaPro) hClusterMeanForm->Draw("lego2");
  
  if(DoDisplay) DoMCDisplay(NDisplay,verbose);

  fIfMCGeneration = kTRUE;

  if(!CalledFromMimosaPro) CheckMCSimulation(true);

  return;

}
//______________________________________________________________________________
//
void MimosaAnalysis::MimosaGeneration_APG4Simu(Int_t MaxEvt,
			                       Int_t PlaneNumber,
					       Int_t Submatrix,
					       Int_t Geomatrix,
					       Int_t seed,
					       bool DoDisplay,
					       bool verbose)
{

  // This function estimates the telescope resolution using a Geant4 simulation with the MimosaSimu code (Alejandro Perez).
  // The MimosaSimu code,
  //  - Builds the telescope geometry (including material) by reading a usual config file.
  //  - Defines the beam by reading the parameters on the config file.
  //  - No digitization of the hits is implemented.
  // 
  //  This function reads the config file and the MC output root file (TTree) produced by the MimosaSimu code (InputFileName.root). The function does the following,
  //  - The true intersections of the simulated particles with the sensors (planes) are then smeared using the PlaneResolution parameter defined in the config file
  //  - Hits are then generated and passed on to the tracker.
  //  - The track finding and tracking algorithms are the same as in the regular analysis. The same track quality cuts (Chi2 cut) as in regular analysis are also applied.
  //  - Displays the geometry defined in the config file, along with the generated and reconstructed tracks
  //  - Estimates the telecope resolution at the DUT position including plane single point resolution and the interacton of the beam particles with the material (multiple scattering and Bremsstrahlung)
  // AP 2015/07/29
  
  fSession->SetEvents(MaxEvt);

  double cm = 1.0e+4;
  double um = 1.0;
  double mm = 1.0e+3;
  
  BookingMC(PlaneNumber,Submatrix,Geomatrix,false);

  DTracker *tTracker     = fSession->GetTracker();
  DEventMC* MCInfoHolder = tTracker->GetMCInfoHolder();
  if(!MCInfoHolder) {
    cout << endl;
    cout << "MimosaGeneration:: the session was not started by reading out MC-data. For that need to used MCBoardReader decoder. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }
  Int_t nPlanes          = fSession->GetTracker()->GetPlanesN();

  if(nPlanes > MynPlanes) {
    cout << endl;
    cout << "MimosaGeneration:: number of planes " << nPlanes << " is higher than maximum " << MynPlanes << " defined in MHits.h. Check you inputs. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  DPlane* tPlane;
  //DPrecAlign* tAlign;
  DR3 posInPlane, posInTracker;
  //Char_t tnum[20];

  //Generation of the tracks
  cout << endl;
  cout << "MimosaGeneration:: Generation Region:" << endl;
  cout << "MimosaGeneration:: X-range = (" << MyRX[0]/mm << "," << MyRX[1]/mm << ") mm" << endl;
  cout << "MimosaGeneration:: Y-range = (" << MyRY[0]/mm << "," << MyRY[1]/mm << ") mm" << endl;
  cout << endl;
  
  TVector3 TrackVect(0.0,0.0,0.0);
  TVector3 PlaneVect(0.0,0.0,0.0);
  TVector3 NormVect(0.0,0.0,0.0);

  const int MaxEventDisplay(MyMaxEventDisplay);
  int NDisplay = MaxEvt;
  if(NDisplay > MaxEventDisplay) NDisplay = MaxEventDisplay;

  int NgoodGenTrk     = 0;
  int NgoodRecTrk     = 0;
  int NgoodRecTrk_Sel = 0;

  DR3 Origin(0.0,0.0,0.0);
  DR3 Slope(0.0,0.0,1.0);
  //DTrack* aTrack = new DTrack(Origin,Slope);
  DTrack* aRecTrack = NULL;
  DHit*   aHit      = NULL;
  TRandom rand(seed);

  if(DoDisplay) {
    NRectracks   = new int[MaxEvt];
    TracksPerEvt = new int[MaxEvt];
  }
  
  int  GlobalCounter = 0;
  bool HitDUT = false;
  DR3  HitPositionDUT(0.0,0.0,0.0);
 
  //Track lines properties
  int    LineStyle    = 2;
  int    LineWidth    = 2;
  int    MarkerStyle  = 20;
  Size_t MarkerSize   = 0.01;
  int    RecLineStyle = 1;
  int    RecLineWidth = 2;
  double Z1,Z2,X1,X2,Y1,Y2;
  
  std::map<int,int> _color_map;
  _color_map[11]   = kBlue;     //e-/+
  _color_map[22]   = kGreen+2;  //gamma
  _color_map[211]  = kRed;      //pion+/-
  _color_map[321]  = kOrange;   //kaon+/-
  _color_map[2212] = kMagenta;  //proton
  _color_map[-1]   = kYellow;   //other
 
  double Mean_U[Bins_GlobalResolution];
  double RMS_U[Bins_GlobalResolution];
  double RMS_U_err[Bins_GlobalResolution];
  double Mean_V[Bins_GlobalResolution];
  double RMS_V[Bins_GlobalResolution];
  double RMS_V_err[Bins_GlobalResolution];
  int    Nevts[Bins_GlobalResolution];
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    Mean_U[istep]    = 0.0;
    RMS_U[istep]     = 0.0;
    RMS_U_err[istep] = 0.0;
    Mean_V[istep]    = 0.0;
    RMS_V[istep]     = 0.0;
    RMS_V_err[istep] = 0.0;
    Nevts[istep]     = 0;
  }

  int    TrkID[MyMaxEventDisplay][MyMaxNumberOfTracks];
  
  DR3 StupidVector(-999.0,-999.0,-999.0);
  TLorentzVector StupidLorentzVector(-999.0,-999.0,-999.0,-999.0);
  for(Long64_t ievt=0; ievt<MaxEvt; ievt++) {
    if(!(fSession->NextRawEvent())) break;
    
    if(DoDisplay && ievt < MyMaxEventDisplay) {
      TracksPerEvt[ievt] = 0;
      //Looping over the particles in this event
      for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
        std::vector<TVector3> TrkPointList;
        TrkPointList.clear();
      
        if(!((MCInfoHolder->GetASimParticle(ipart).ProdVtx.X() >= MyRX[0] && MCInfoHolder->GetASimParticle(ipart).ProdVtx.X() <= MyRX[1]) &&  
             (MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y() >= MyRY[0] && MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y() <= MyRY[1]))) continue;
	
	if(TracksPerEvt[ievt] < MyMaxNumberOfTracks) TracksPerEvt[ievt]++;
	else                                         break;
      
        GlobalCounter++;
	
	int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
        //Looping over the hits of this particle
        for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++) {
	  double x = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosInXYZmm.X()*mm;
          double y = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosInXYZmm.Y()*mm;
	  double z = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosInXYZmm.Z()*mm;
	
	  TrkPointList.push_back(TVector3(x,y,z));
        }
        
        for(int ipart2=0;ipart2<MCInfoHolder->GetNNonSensitiveSimParticles();ipart2++) {
	  if(MCInfoHolder->GetANonSensitiveSimParticle(ipart2).SensitiveIdx == ipart) {
	    int FirstHitIdx2 = MCInfoHolder->GetANonSensitiveSimParticle(ipart).FirstHitIdx;
	    for(int ihit=0;ihit<MCInfoHolder->GetANonSensitiveSimParticle(ipart).NHits;ihit++) {
	      double x = MCInfoHolder->GetANonSensitiveSimHit(FirstHitIdx2 + ihit).PosInXYZmm.X()*mm;
              double y = MCInfoHolder->GetANonSensitiveSimHit(FirstHitIdx2 + ihit).PosInXYZmm.Y()*mm;
	      double z = MCInfoHolder->GetANonSensitiveSimHit(FirstHitIdx2 + ihit).PosInXYZmm.Z()*mm;
	      
	      TrkPointList.push_back(TVector3(x,y,z));
	    } // End of hit loop non-sensitive
	  
	    break;
	  } // End if good index
        } // End of particle loop non-sensitive
        
	TrkID[ievt][ipart] = ipart;
	
	int pdgcode = MCInfoHolder->GetASimParticle(ipart).pdgID;
	if(pdgcode != 11   &&
	   pdgcode != 22   &&
	   pdgcode != 211  &&
	   pdgcode != 321  && 
	   pdgcode != 2212
	) pdgcode = -1;
	
	int mycolor = (GlobalCounter+1==10)?49:(GlobalCounter+1);
	
	//Filling up the TGraph with the trajectories:
        TString TmpName;
        TmpName = TString("gtracksXZ_evt") + long(ievt+1) + TString("_trk") + long(ipart+1);
        gtracksXZ[ievt][ipart] = new TGraph(int(TrkPointList.size()));
        gtracksXZ[ievt][ipart]->SetName(TmpName.Data());
        gtracksXZ[ievt][ipart]->SetLineColor(mycolor);
        gtracksXZ[ievt][ipart]->SetLineWidth(LineWidth);
        gtracksXZ[ievt][ipart]->SetLineStyle(LineStyle);

        TmpName = TString("gtracksYZ_evt") + long(ievt+1) + TString("_trk") + long(ipart+1);
        gtracksYZ[ievt][ipart] = new TGraph(int(TrkPointList.size()));
        gtracksYZ[ievt][ipart]->SetName(TmpName.Data());
        gtracksYZ[ievt][ipart]->SetLineColor(mycolor);
        gtracksYZ[ievt][ipart]->SetLineWidth(LineWidth);
        gtracksYZ[ievt][ipart]->SetLineStyle(LineStyle);

        gtracksXYZ[ievt][ipart] = new TPolyLine3D(int(TrkPointList.size()),"");
        gtracksXYZ[ievt][ipart]->SetLineColor(_color_map[pdgcode]);
        gtracksXYZ[ievt][ipart]->SetLineWidth(LineWidth);
        gtracksXYZ[ievt][ipart]->SetLineStyle(LineStyle);
	
	gtracksXYZ_mark[ievt][ipart] = new TPolyMarker3D(int(TrkPointList.size()));
        gtracksXYZ_mark[ievt][ipart]->SetMarkerColor(_color_map[pdgcode]);
	gtracksXYZ_mark[ievt][ipart]->SetMarkerStyle(MarkerStyle);
	gtracksXYZ_mark[ievt][ipart]->SetMarkerSize(MarkerSize);

        for(int ipoint=0;ipoint<int(TrkPointList.size());ipoint++) {
	  gtracksXZ[ievt][ipart]->SetPoint(ipoint,TrkPointList[ipoint].Z()/mm,TrkPointList[ipoint].X()/mm);
	  gtracksYZ[ievt][ipart]->SetPoint(ipoint,TrkPointList[ipoint].Z()/mm,TrkPointList[ipoint].Y()/mm);
	  
	  gtracksXYZ[ievt][ipart]->SetPoint(ipoint,
					    TrkPointList[ipoint].X()/cm,
					    TrkPointList[ipoint].Y()/cm,
					    TrkPointList[ipoint].Z()/cm);  //Position in cm
	  
	  gtracksXYZ_mark[ievt][ipart]->SetPoint(ipoint,
					         TrkPointList[ipoint].X()/cm,
					         TrkPointList[ipoint].Y()/cm,
					         TrkPointList[ipoint].Z()/cm); //Position in cm
        }
	
      } //End of loop over particles
      
    }// End Do display
    
    if(!UsingTrackerResolution) {
      //Reseting the hits on the planes
      for(int iplane=0;iplane<nPlanes;iplane++) {
	if(fSession->GetTracker()->GetPlane(iplane+1)->GetReadout()     <= 0)            continue;  //Not considering non-sensitive planes
	if(fSession->GetTracker()->GetPlane(iplane+1)->GetPlaneNumber() == PlaneNumber)  continue;  //Only generate the hit for the tracking planes
	fSession->GetTracker()->GetPlane(iplane+1)->SetHitsN(0);
	if(verbose) cout << "Initializing plane " << iplane+1 << " with " << fSession->GetTracker()->GetPlane(iplane+1)->GetHitsN() << " hits" << endl;
      }
      
      //Filling up the hits on the planes
      for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
	if(!((MCInfoHolder->GetASimParticle(ipart).ProdVtx.X() >= MyRX[0] && MCInfoHolder->GetASimParticle(ipart).ProdVtx.X() <= MyRX[1]) &&  
             (MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y() >= MyRY[0] && MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y() <= MyRY[1]))) continue;
	int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
	//Looping over the hits of this particle
        for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++) {
	  tPlane = fSession->GetTracker()->GetPlane(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID+1);
	  
	  if(tPlane->GetReadout()     <=  0)            continue;  //Not considering non-sensitive planes
	  if(tPlane->GetPlaneNumber() ==  PlaneNumber)  continue;  //Only generate the hit for the tracking planes
	  
	  if(verbose) cout << "Looking at Plane" << tPlane->GetPlaneNumber() << ":" << endl;
	  
	  //Get the hit local position
	  double u,v,col,lin;
          u = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.X()*mm;
          v = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.Y()*mm;
	  tPlane->ComputeStripPosition_UVToColRow(u,v,col,lin);
	  
	  DR3 HitPosition(0.0,0.0,0.0);
	  DR3 HitResolution(0.0,0.0,0.0);
	  
	  int ClusterMult = tPlane->GenerateMCMultiplicity(col,lin);
	  TVector2 MyHitResolution(0.0,0.0);
	  tPlane->GetHitResolution(col,lin,ClusterMult,MyHitResolution);
	  HitResolution.SetValue(MyHitResolution.X()*um,
				 MyHitResolution.Y()*um,
				 0.0);
	  
	  HitPosition.SetValue(rand.Gaus(u,HitResolution(0)),
			       rand.Gaus(v,HitResolution(1)),
			       0.0);
	    
	  //Adding the hit to he List of hits
	  tPlane->AddMCGeneratedHit(HitPosition,HitResolution,FirstHitIdx + ihit);
	} // End of loop over hits
      } // End of loop over particles
      
      for(int iplane=0;iplane<nPlanes;iplane++) {
	if(fSession->GetTracker()->GetPlane(iplane+1)->GetReadout()     <= 0)            continue;  //Not considering non-sensitive planes
	if(fSession->GetTracker()->GetPlane(iplane+1)->GetPlaneNumber() == PlaneNumber)  continue;  //Only generate the hit for the tracking planes
	fSession->GetTracker()->GetPlane(iplane+1)->MCHitsTruthMatching();
      }
      
      tTracker->UpdateMC();
      tTracker->MCTracksTruthMatching();

      if(verbose) {
	cout << "Evt = " << ievt+1 << ", Tracker has reconstructed " << tTracker->GetTracksN() << " tracks" << endl;
	cout << endl;
      }
      
      if(DoDisplay && ievt < MyMaxEventDisplay) {
	NRectracks[ievt] = 0;
	for(Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++) {// loop over the reconstructed tracks
	  aRecTrack = tTracker->GetTrack(iTrack);
	  
	  if(NRectracks[ievt] < MyMaxNumberOfTracks) NRectracks[ievt]++;
          else                                       break;
	  
	  Z1 = hXZ_MC->GetXaxis()->GetXmin()*mm;
          Z2 = hXZ_MC->GetXaxis()->GetXmax()*mm;
	  
	  X1 = aRecTrack->GetLinearFit().GetOrigin()(0) + aRecTrack->GetLinearFit().GetSlopeZ()(0)*Z1;
	  X2 = aRecTrack->GetLinearFit().GetOrigin()(0) + aRecTrack->GetLinearFit().GetSlopeZ()(0)*Z2;
	  Y1 = aRecTrack->GetLinearFit().GetOrigin()(1) + aRecTrack->GetLinearFit().GetSlopeZ()(1)*Z1;
	  Y2 = aRecTrack->GetLinearFit().GetOrigin()(1) + aRecTrack->GetLinearFit().GetSlopeZ()(1)*Z2;
	  
	  int TheMotherTrackColor = 1;
	  int pdgcode = -1;
	  if(aRecTrack->GetMCPartID() > 0) {
	    pdgcode = MCInfoHolder->GetASimParticle(aRecTrack->GetMCPartID()).pdgID;
	    for(int iii=0;iii<TracksPerEvt[ievt];iii++) {
	      if(TrkID[ievt][iii] == aRecTrack->GetMCPartID()) {
		TheMotherTrackColor = gtracksXZ[ievt][iii]->GetLineColor();
		break;
	      }
	    }
	  }
	  if(pdgcode != 11   &&
	     pdgcode != 22   &&
	     pdgcode != 211  &&
	     pdgcode != 321  && 
	     pdgcode != 2212
	  ) pdgcode = -1;
	  
	  RectracksXZ[ievt][iTrack-1] = new TLine(Z1/mm,X1/mm,Z2/mm,X2/mm);
	  RectracksXZ[ievt][iTrack-1]->SetLineColor(TheMotherTrackColor);
	  RectracksXZ[ievt][iTrack-1]->SetLineWidth(RecLineWidth);
	  RectracksXZ[ievt][iTrack-1]->SetLineStyle(RecLineStyle);
	    
	  RectracksYZ[ievt][iTrack-1] = new TLine(Z1/mm,Y1/mm,Z2/mm,Y2/mm);
	  RectracksYZ[ievt][iTrack-1]->SetLineColor(TheMotherTrackColor);
	  RectracksYZ[ievt][iTrack-1]->SetLineWidth(RecLineWidth);
	  RectracksYZ[ievt][iTrack-1]->SetLineStyle(RecLineStyle);

	  RectracksXYZ[ievt][iTrack-1] = new TPolyLine3D(2,"");
	  RectracksXYZ[ievt][iTrack-1]->SetLineColor(kRed);
	  RectracksXYZ[ievt][iTrack-1]->SetLineWidth(6);
          RectracksXYZ[ievt][iTrack-1]->SetLineStyle(RecLineStyle);
	  RectracksXYZ[ievt][iTrack-1]->SetPoint(0,X1/cm,Y1/cm,Z1/cm);  //Position in cm
	  RectracksXYZ[ievt][iTrack-1]->SetPoint(1,X2/cm,Y2/cm,Z2/cm);  //Position in cm
	}  //end  loop on reconstructed tracks
      } // End of if Do display

      NgoodRecTrk      += tTracker->GetTracksN();
      for(Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++) {// loop over the reconstructed tracks
	aRecTrack = tTracker->GetTrack(iTrack);

	DR3 trackSlope  = aRecTrack->GetLinearFit().GetSlopeZ();
	hTrackSlopeX_MC->Fill(trackSlope(0));
	hTrackSlopeY_MC->Fill(trackSlope(1));
	hTrackNHits_MC->Fill(aRecTrack->GetHitsNumber());
	hTrackChi2_MC->Fill(aRecTrack->GetChiSquare());

	if(aRecTrack->GetChiSquare() < TrackChi2Limit) NgoodRecTrk_Sel++;

	HitDUT = false;
	if(aRecTrack->GetMCPartID() > 0) {
	  tPlane     = fSession->GetTracker()->GetPlane(PlaneNumber);
	  posInPlane = tPlane->Intersection(aRecTrack);
	  
	  int FirstHitIdx = MCInfoHolder->GetASimParticle(aRecTrack->GetMCPartID()).FirstHitIdx;
	  for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(aRecTrack->GetMCPartID()).NHits;ihit++) {
	    if(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID+1 == PlaneNumber) {
	      HitPositionDUT.SetValue(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.X()*mm,
		                      MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.Y()*mm,
				      0.0);
	      HitDUT = true; 
	      break;
	    }
	  }
	}
	  
	if(HitDUT) {
	  double DeltaU = posInPlane(0) - HitPositionDUT(0);
	  double DeltaV = posInPlane(1) - HitPositionDUT(1);

	  if(verbose) {
	    cout << iTrack << " true-track intersection with DUT is (U,V) = (" << HitPositionDUT(0) << "," << HitPositionDUT(1) << ") um" << endl;
	    cout << iTrack << " rec-track  intersection with DUT is (U,V) = (" << posInPlane(0)     << "," << posInPlane(1)     << ") um" << endl;
	    cout << iTrack << " residual with DUT hit               (U,V) = (" << DeltaU            << "," << DeltaV            << ") um" << endl;
	    cout << endl << endl;
	  }

	  if(aRecTrack->GetChiSquare() < TrackChi2Limit) {
	    hTrackResidualAtDUT_U[0]->Fill(DeltaU);
	    hTrackResidualAtDUT_V[0]->Fill(DeltaV);
	    
	    Nevts[0]++;
	      
	    Mean_U[0] += DeltaU;
            RMS_U[0]  += pow(DeltaU,2);
	      
	    Mean_V[0] += DeltaV;
            RMS_V[0]  += pow(DeltaV,2);
	  }

        }

	for(Int_t iHit=0; iHit<aRecTrack->GetHitsNumber();iHit++ ) {
	  aHit   = (DHit*)aRecTrack->GetHit( iHit);
	  tPlane = (DPlane*)aHit->GetPlane();
	  hTrackPlanesUsed_MC->Fill(tPlane->GetPlaneNumber());
	  DR3 impactPosition = aRecTrack->Intersection(tPlane);
	  impactPosition -= *(aHit->GetPosition());
	  hResidualU_MC[tPlane->GetPlaneNumber()-1]->Fill( impactPosition(0));
	  hResidualV_MC[tPlane->GetPlaneNumber()-1]->Fill( impactPosition(1));
	}

      } //end  loop on reconstructed tracks
    } //End of if UsingTrackerResolution
    else {
      for(int istep=0;istep<Bins_GlobalResolution;istep++) {
	double TheResolution = (RGlobalResolution[0] + (istep+0.5)*(RGlobalResolution[1] - RGlobalResolution[0])/Bins_GlobalResolution)*um;

	//Reseting the hits on the planes
        for(int iplane=0;iplane<nPlanes;iplane++) {
	  if(fSession->GetTracker()->GetPlane(iplane+1)->GetReadout()     <= 0)            continue;  //Not considering non-sensitive planes
	  if(fSession->GetTracker()->GetPlane(iplane+1)->GetPlaneNumber() == PlaneNumber)  continue;  //Only generate the hit for the tracking planes
	  fSession->GetTracker()->GetPlane(iplane+1)->SetHitsN(0);
	  if(verbose) cout << "Initializing plane " << iplane+1 << " with " << fSession->GetTracker()->GetPlane(iplane+1)->GetHitsN() << " hits" << endl;
        }
        
        //Filling up the hits on the planes
        for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
	  if(!((MCInfoHolder->GetASimParticle(ipart).ProdVtx.X() >= MyRX[0] && MCInfoHolder->GetASimParticle(ipart).ProdVtx.X() <= MyRX[1]) &&  
               (MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y() >= MyRY[0] && MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y() <= MyRY[1]))) continue;
	  int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
	  //Looping over the hits of this particle
          for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++) {
	    tPlane = fSession->GetTracker()->GetPlane(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID+1);

	    if(tPlane->GetReadout()     <=  0)            continue;  //Not considering non-sensitive planes
	    if(tPlane->GetPlaneNumber() ==  PlaneNumber)  continue;  //Only generate the hit for the tracking planes
	  
	    if(verbose) cout << "Looking at Plane" << tPlane->GetPlaneNumber() << ":" << endl;

	    //Get the hit local position
	    double u,v,col,lin;
            u = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.X()*mm;
            v = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.Y()*mm;
	    tPlane->ComputeStripPosition_UVToColRow(u,v,col,lin);

	    DR3 HitPosition(0.0,0.0,0.0);
	    DR3 HitResolution(0.0,0.0,0.0);
	      
	    int ClusterMult = tPlane->GenerateMCMultiplicity(col,lin);
	    TVector2 MyHitResolution(0.0,0.0);
	    tPlane->GetHitResolution(col,lin,ClusterMult,MyHitResolution);
	    HitResolution.SetValue(MyHitResolution.X()*um,MyHitResolution.Y()*um,0.0);
	  
	    DR3 TheRecPosition(rand.Gaus(u,TheResolution),
			       rand.Gaus(v,TheResolution),
			       0.0);
	  
	    HitPosition.SetValue(TheRecPosition(0),TheRecPosition(1),TheRecPosition(2));
	  
	    //Adding the hit to he List of hits
	    tPlane->AddMCGeneratedHit(HitPosition,HitResolution,FirstHitIdx + ihit);
	  } // End of loop over hits
        } // End of loop over particles

        for(int iplane=0;iplane<nPlanes;iplane++) {
	  if(fSession->GetTracker()->GetPlane(iplane+1)->GetReadout()     <= 0)            continue;  //Not considering non-sensitive planes
	  if(fSession->GetTracker()->GetPlane(iplane+1)->GetPlaneNumber() == PlaneNumber)  continue;  //Only generate the hit for the tracking planes
	  fSession->GetTracker()->GetPlane(iplane+1)->MCHitsTruthMatching();
        }
      
        tTracker->UpdateMC();
        tTracker->MCTracksTruthMatching();
      
        for(Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++) {// loop over the reconstructed tracks
	  aRecTrack = tTracker->GetTrack(iTrack);

	  DR3 trackSlope  = aRecTrack->GetLinearFit().GetSlopeZ();
	  hTrackSlopeX_MC->Fill(trackSlope(0));
	  hTrackSlopeY_MC->Fill(trackSlope(1));
	  hTrackNHits_MC->Fill(aRecTrack->GetHitsNumber());
	  hTrackChi2_MC->Fill(aRecTrack->GetChiSquare());

	  if(aRecTrack->GetChiSquare() < TrackChi2Limit) NgoodRecTrk_Sel++;

	  HitDUT = false;
	  if(aRecTrack->GetMCPartID() > 0) {
	    tPlane     = fSession->GetTracker()->GetPlane(PlaneNumber);
	    posInPlane = tPlane->Intersection(aRecTrack);
	  
	    int FirstHitIdx = MCInfoHolder->GetASimParticle(aRecTrack->GetMCPartID()).FirstHitIdx;
	    for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(aRecTrack->GetMCPartID()).NHits;ihit++) {
	      if(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID+1 == PlaneNumber) {
	        HitPositionDUT.SetValue(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.X()*mm,
		                        MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.Y()*mm,
				        0.0);
	        HitDUT = true; 
	        break;
	      }
	    }
	  }

	  if(HitDUT) {
	    double DeltaU = posInPlane(0) - HitPositionDUT(0);
	    double DeltaV = posInPlane(1) - HitPositionDUT(1);

	    if(verbose) {
	      cout << iTrack << " true-track intersection with DUT is (U,V) = (" << HitPositionDUT(0) << "," << HitPositionDUT(1) << ") um" << endl;
	      cout << iTrack << " rec-track  intersection with DUT is (U,V) = (" << posInPlane(0)     << "," << posInPlane(1)     << ") um" << endl;
	      cout << iTrack << " residual with DUT hit               (U,V) = (" << DeltaU            << "," << DeltaV            << ") um" << endl;
	      cout << endl << endl;
	    }

	    if(aRecTrack->GetChiSquare() < TrackChi2Limit) {
	      hTrackResidualAtDUT_U[istep]->Fill(DeltaU);
	      hTrackResidualAtDUT_V[istep]->Fill(DeltaV);
	    
	      Nevts[istep]++;
	      
	      Mean_U[istep] += DeltaU;
              RMS_U[istep]  += pow(DeltaU,2);
	      
	      Mean_V[istep] += DeltaV;
              RMS_V[istep]  += pow(DeltaV,2);
	    }
          }
	} //end  loop on reconstructed tracks

      } // End of loop of steps for sensor spatial resolution scan
    }// End of else UsingTrackerResolution
    
  } // End of loop over events

  double MaxRMS = -1.0e+20;
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    double TheRMS;
    if(hTrackResidualAtDUT_U[istep]->Integral() > 10.) TheRMS = hTrackResidualAtDUT_U[istep]->GetRMS();
    else                                               TheRMS = 1.0;
    if(MaxRMS < TheRMS) MaxRMS = TheRMS;

    if(hTrackResidualAtDUT_V[istep]->Integral() > 10.) TheRMS = hTrackResidualAtDUT_V[istep]->GetRMS();
    else                                               TheRMS = 1.0;
    if(MaxRMS < TheRMS) MaxRMS = TheRMS;
  }
  
  double TheRange = 50.0;
  if(TheRange < Nsigma*MaxRMS) TheRange = Nsigma*MaxRMS;
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    hTrackResidualAtDUT_U[istep]->SetAxisRange(-TheRange,+TheRange,"X");
    hTrackResidualAtDUT_V[istep]->SetAxisRange(-TheRange,+TheRange,"X");
  }

  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    double TheResolution = RGlobalResolution[0] + (istep+0.5)*(RGlobalResolution[1] - RGlobalResolution[0])/Bins_GlobalResolution;
    
    Mean_U[istep]   /= Nevts[istep];
    RMS_U[istep]    /= Nevts[istep];
    RMS_U[istep]    -= pow(Mean_U[istep],2);
    RMS_U[istep]     = sqrt(RMS_U[istep]);
    RMS_U_err[istep] = RMS_U[istep]/sqrt(2.0*Nevts[istep]);
    
    Mean_V[istep]   /= Nevts[istep];
    RMS_V[istep]    /= Nevts[istep];
    RMS_V[istep]    -= pow(Mean_V[istep],2);
    RMS_V[istep]     = sqrt(RMS_V[istep]);
    RMS_V_err[istep] = RMS_V[istep]/sqrt(2.0*Nevts[istep]);
    
    cout << istep+1 << "  Nentries = " << Nevts[istep] 
         << "  MeanU = " << Mean_U[istep] << " um"
         << "  RMS_U = (" << RMS_U[istep] << " +/- " << RMS_U_err[istep] << ") um"
	 << "  MeanV = " << Mean_V[istep] << " um"
         << "  RMS_V = (" << RMS_V[istep] << " +/- " << RMS_V_err[istep] << ") um"
	 << "  resolution = " << TheResolution << " um"
	 << "  ResidueU = "   << sqrt(pow(TheResolution,2) + pow(RMS_U[istep],2)) << " um"
	 << "  ResidueV = "   << sqrt(pow(TheResolution,2) + pow(RMS_V[istep],2)) << " um"
         << endl;
  }


  if(!UsingTrackerResolution) {
    double TrkEffic[2];
    double TrkEffic_Sel[2];
    if(NgoodGenTrk > 0) {
      TrkEffic[0]     = (double)NgoodRecTrk/NgoodGenTrk;
      TrkEffic[1]     = sqrt(TrkEffic[0]*(1.0 - TrkEffic[0])/NgoodGenTrk);
      TrkEffic_Sel[0] = (double)NgoodRecTrk_Sel/NgoodGenTrk;
      TrkEffic_Sel[1] = sqrt(TrkEffic_Sel[0]*(1.0 - TrkEffic_Sel[0])/NgoodGenTrk);
    }
    else {
      TrkEffic[0]     = -1.0;
      TrkEffic[1]     =  0.0;
      TrkEffic_Sel[0] = -1.0;
      TrkEffic_Sel[1] =  0.0;
    }

    cout << endl;
    cout << endl;
    cout << "===================================================================" << endl;
    cout << "Tracking efficiency from MC simulation:  " << endl;
    cout << "IMPORTANT: no single hit detection efficiency included. Effect included:" << endl;
    cout << "           - Single point resolution," << endl;
    cout << "           - Multiple scattering," << endl;
    cout << "           - Track selection (Trk Chi2 cut = " << TrackChi2Limit << ")" << endl;
    cout << "-------------------------------------------------------------------" << endl;
    cout << "Nb of generated good tracks             =  " << NgoodGenTrk << ", out of " << MaxEvt << endl;
    cout << "Nb of reconstructed  tracks             =  " << NgoodRecTrk << endl;
    cout << "Nb of reconstructed and selected tracks =  " << NgoodRecTrk_Sel << endl;
    cout << "Tracking efficiency                     = (" << TrkEffic[0]*100.0 << " +/- " << TrkEffic[1]*100.0 << ")%" << endl;
    cout << "Tracking efficiency (with selection)    = (" << TrkEffic_Sel[0]*100.0 << " +/- " << TrkEffic_Sel[1]*100.0 << ")%" << endl;
    cout << "===================================================================" << endl;
    cout << endl;
    cout << endl;
  }


  TString TheDoubleGaussian("[0]*(1.0/sqrt(2*TMath::Pi()))*(([4]/[2])*exp(-0.5*pow((x-[1])/[2],2)) +  ((1.0 - [4])/[3])*exp(-0.5*pow((x-[1])/[3],2)) )");
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    if(hTrackResidualAtDUT_U[istep]->Integral() > 10.) {
      cout << endl;
      cout << endl;
      cout << "Fitting Track U-residual at DUT position" << endl;
      cout << "step = " << istep+1 << "  Tel-resU = (" << hTrackResidualAtDUT_U[istep]->GetRMS() << " +/- " << hTrackResidualAtDUT_U[istep]->GetRMSError() << ") um" 
           << ",  integral = " << hTrackResidualAtDUT_U[istep]->Integral() << ", entries " << hTrackResidualAtDUT_U[istep]->GetEntries() << ", name = " << hTrackResidualAtDUT_U[istep]->GetName()
	   << ", My RMS_U = (" << RMS_U[istep] << " +/- " << RMS_U_err[istep] << ") um"
           << endl;
	   
      TF1* fres_U = new TF1("gaus",
			    TheDoubleGaussian.Data(),
			    -Nsigma*hTrackResidualAtDUT_U[istep]->GetRMS(),
			    +Nsigma*hTrackResidualAtDUT_U[istep]->GetRMS());
      fres_U->SetParNames("N","#mu (#mum)","#sigma_{1} (#mum)","#sigma_{2} (#mum)","f_{1}");
      fres_U->SetParameters(hTrackResidualAtDUT_U[istep]->Integral(),
			    0.0,
			    hTrackResidualAtDUT_U[istep]->GetRMS(),
			    2.0*hTrackResidualAtDUT_U[istep]->GetRMS(),
			    0.8);
      fres_U->SetParLimits(0,0.0,1.0*hTrackResidualAtDUT_U[istep]->Integral());
      fres_U->SetParLimits(1,-hTrackResidualAtDUT_U[istep]->GetRMS(),+hTrackResidualAtDUT_U[istep]->GetRMS());
      fres_U->SetParLimits(2,0.0,20.0*hTrackResidualAtDUT_U[istep]->GetRMS());
      fres_U->SetParLimits(3,0.0,20.0*hTrackResidualAtDUT_U[istep]->GetRMS());
      fres_U->SetParLimits(4,0.0,1.0);
      hTrackResidualAtDUT_U[istep]->Fit(fres_U,"R");
      h_TelescopeResolution_Scan_U->SetBinContent(istep+1,hTrackResidualAtDUT_U[istep]->GetRMS());
      h_TelescopeResolution_Scan_U->SetBinError(istep+1,  hTrackResidualAtDUT_U[istep]->GetRMSError());
      
      double tot_res  = pow(h_TelescopeResolution_Scan_U->GetBinContent(istep+1),2);
      tot_res        += pow(h_TelescopeResolution_Scan_U->GetBinCenter(istep+1),2);
      tot_res         = sqrt(tot_res);
      h_ResidualAtDUT_Scan_U->SetBinContent(istep+1,tot_res);
      h_ResidualAtDUT_Scan_U->SetBinError(istep+1, hTrackResidualAtDUT_U[istep]->GetRMSError());
      
      cout << "step = " << istep+1 << "  Tel-resU = (" << h_TelescopeResolution_Scan_U->GetBinContent(istep+1) << " +/- " << h_TelescopeResolution_Scan_U->GetBinError(istep+1) << ") um" 
           << ", ResidueU = (" << h_ResidualAtDUT_Scan_U->GetBinContent(istep+1) << " +/- " << h_ResidualAtDUT_Scan_U->GetBinError(istep+1) << ") um" 
           << endl;
      
    }

    if(hTrackResidualAtDUT_V[istep]->Integral() > 10.) {
      cout << endl;
      cout << endl;
      cout << "Fitting Track V-residual at DUT position" << endl;
      cout << "step = " << istep+1 << "  Tel-resV = (" << hTrackResidualAtDUT_V[istep]->GetRMS() << " +/- " << hTrackResidualAtDUT_V[istep]->GetRMSError() << ") um" 
           << ", My RMS_V = (" << RMS_V[istep] << " +/- " << RMS_V_err[istep] << ") um"
           << endl;

      TF1* fres_V = new TF1("gaus",
			    TheDoubleGaussian.Data(),
			    -Nsigma*hTrackResidualAtDUT_V[istep]->GetRMS(),
			    +Nsigma*hTrackResidualAtDUT_V[istep]->GetRMS());
      fres_V->SetParNames("N","#mu (#mum)","#sigma_{1} (#mum)","#sigma_{2} (#mum)","f_{1}");
      fres_V->SetParameters(hTrackResidualAtDUT_V[istep]->Integral(),
			    0.0,
			    hTrackResidualAtDUT_V[istep]->GetRMS(),
			    2.0*hTrackResidualAtDUT_V[istep]->GetRMS(),
			    0.8);
      fres_V->SetParLimits(0,0.0,1.0*hTrackResidualAtDUT_V[istep]->Integral());
      fres_V->SetParLimits(1,-hTrackResidualAtDUT_V[istep]->GetRMS(),+hTrackResidualAtDUT_V[istep]->GetRMS());
      fres_V->SetParLimits(2,0.0,20.0*hTrackResidualAtDUT_V[istep]->GetRMS());
      fres_V->SetParLimits(3,0.0,20.0*hTrackResidualAtDUT_V[istep]->GetRMS());
      fres_V->SetParLimits(4,0.0,1.0);
      hTrackResidualAtDUT_V[istep]->Fit(fres_V,"R");
      h_TelescopeResolution_Scan_V->SetBinContent(istep+1,hTrackResidualAtDUT_V[istep]->GetRMS());
      h_TelescopeResolution_Scan_V->SetBinError(istep+1,  hTrackResidualAtDUT_V[istep]->GetRMSError());
      
      double tot_res  = pow(h_TelescopeResolution_Scan_V->GetBinContent(istep+1),2);
      tot_res        += pow(h_TelescopeResolution_Scan_V->GetBinCenter(istep+1),2);
      tot_res         = sqrt(tot_res);
      h_ResidualAtDUT_Scan_V->SetBinContent(istep+1,tot_res);
      h_ResidualAtDUT_Scan_V->SetBinError(istep+1,  hTrackResidualAtDUT_V[istep]->GetRMSError());
      
      cout << "step = " << istep+1 << "  Tel-resV = (" << h_TelescopeResolution_Scan_V->GetBinContent(istep+1) << " +/- " << h_TelescopeResolution_Scan_V->GetBinError(istep+1) << ") um" 
           << ", ResidueV = (" << h_ResidualAtDUT_Scan_V->GetBinContent(istep+1) << " +/- " << h_ResidualAtDUT_Scan_V->GetBinError(istep+1) << ") um" 
           << endl;
    }
  }

  if(DoDisplay) {
    DoMCDisplay(NDisplay,verbose);
    
    //TString MyGdmlFile = TString(InputFileName) + TString(".gdml");
    //DoMCDisplay3D(NDisplay,verbose,MyGdmlFile.Data());
  }

  fIfMCGeneration = kTRUE;
  CheckMCSimulation(true);

  return;
  
}
//______________________________________________________________________________
//
void MimosaAnalysis::MimosaGeneration_LoicG4Simu(Int_t MaxEvt,
			                         Int_t PlaneNumber,
						 Int_t Submatrix,
						 Int_t Geomatrix,
						 Int_t seed,
						 bool DoDisplay,
						 bool verbose)
{
  
  // This function estimates the telescope resolution using a Geant4 simulation with the DigiCMOS code (Loic Cousin).
  // The DigiCMOS code has to be modified to
  //  - Implement the needed geometry, and the particle type and momentum to genrate the beam particles.
  //  - Avoid digitization of the hits.
  // 
  //  This function reads the config file and the MC output produced by the DigiCMOS code. The function does the following,
  //  - The true intersections of the simulated particles with the sensors (planes) are then smeared using the PlaneResolution parameter defined in the config file
  //  - Hits are then generated and passed on to the tracker.
  //  - The track finding and tracking algorithms are the same as in the regular analysis. The same track quality cuts (Chi2 cut) as in regular analysis are also applied.
  //  - Displays the geometry defined in the config file, along with the generated and reconstructed tracks
  // AP 2015/07/08
  
  return;
  
}
//______________________________________________________________________________
//


