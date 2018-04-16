#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <assert.h>

#include <TROOT.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TFile.h>
#include <TMinuit.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TMatrixTSym.h>
#include <TMatrixDSymEigen.h>
#include <TLine.h>

using namespace std;

TH2D* h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom    = NULL;
TH2D* h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom    = NULL;

//=========================================================================================================
double GetResidue(double sigma_sp,
		  double E,
		  TH1F** hMC,
		  std::vector<double> _EValList)
{
  
  int Nbins_E = _EValList.size();
  double RE[2];
  RE[0] = _EValList[0];
  RE[1] = _EValList[Nbins_E-1];
  
  if(E < RE[0] || E > RE[1]) {
    cout << endl;
    cout << "The energy value " << E << " is outside the MC data range (" << RE[0] << "," << RE[1] << ")" << endl;
    cout << endl;
    assert(false);
  }
  
  int idx_E = -999;
  for(int iE=0;iE<Nbins_E-1;iE++) {
    if(E >= _EValList[iE] && E < _EValList[iE+1]) {
      idx_E = iE;
      break;
    }
  }
  
  double Rsigma[2];
  Rsigma[0] = TMath::Max(hMC[idx_E]->GetBinCenter(1),
			 hMC[idx_E+1]->GetBinCenter(1));
  Rsigma[1] = TMath::Min(hMC[idx_E]->GetBinCenter(hMC[idx_E]->GetXaxis()->GetNbins()),
			 hMC[idx_E]->GetBinCenter(hMC[idx_E+1]->GetXaxis()->GetNbins()));

  if(sigma_sp < Rsigma[0] || sigma_sp > Rsigma[1]) {
    cout << endl;
    cout << "The sigma_sp value " << sigma_sp << " is outside the MC data range (" << Rsigma[0] << "," << Rsigma[1] << ")" << endl;
    cout << endl;
    assert(false);
  }

  int idx_Sigma   = -999;
  for(int isigma=0;isigma<hMC[idx_E]->GetXaxis()->GetNbins()-1;isigma++) {
    double x1 = hMC[idx_E]->GetBinCenter(isigma+1);
    double x2 = hMC[idx_E]->GetBinCenter(isigma+2);
    if(sigma_sp >= x1 && sigma_sp < x2) {
      idx_Sigma = isigma+1;
      break;
    }
  }
  int idx_Sigmap1 = -999;
  for(int isigma=0;isigma<hMC[idx_E+1]->GetXaxis()->GetNbins()-1;isigma++) {
    double x1 = hMC[idx_E+1]->GetBinCenter(isigma+1);
    double x2 = hMC[idx_E+1]->GetBinCenter(isigma+2);
    if(sigma_sp >= x1 && sigma_sp < x2) {
      idx_Sigmap1 = isigma+1;
      break;
    }
  }
  
  TVector2 ThePoint(E,sigma_sp);
  TVector3 ClosePoints[4];
  ClosePoints[0].SetXYZ(_EValList[idx_E],  hMC[idx_E]->GetBinCenter(idx_Sigma),      hMC[idx_E]->GetBinContent(idx_Sigma));
  ClosePoints[1].SetXYZ(_EValList[idx_E],  hMC[idx_E]->GetBinCenter(idx_Sigma+1),    hMC[idx_E]->GetBinContent(idx_Sigma+1));
  ClosePoints[2].SetXYZ(_EValList[idx_E+1],hMC[idx_E+1]->GetBinCenter(idx_Sigmap1),  hMC[idx_E+1]->GetBinContent(idx_Sigmap1));
  ClosePoints[3].SetXYZ(_EValList[idx_E+1],hMC[idx_E+1]->GetBinCenter(idx_Sigmap1+1),hMC[idx_E+1]->GetBinContent(idx_Sigmap1+1));
  
  //Ordering the 4 closest w.r.t to distance
  for(int iii=2;iii<=4;iii++) {
    for(int jjj=0;jjj<=4-iii;jjj++) {
      TVector2 vect_jjj   = ThePoint - TVector2(ClosePoints[jjj].X(),  ClosePoints[jjj].Y());
      TVector2 vect_jjjp1 = ThePoint - TVector2(ClosePoints[jjj+1].X(),ClosePoints[jjj+1].Y());
      if(vect_jjj.Mod() > vect_jjjp1.Mod()) {
	TVector3 point_aux  = ClosePoints[jjj];
	ClosePoints[jjj]    = ClosePoints[jjj+1];
	ClosePoints[jjj+1]  = point_aux;
      }
    }
  }
  
  TVector3 Vect1      = ClosePoints[0] - ClosePoints[1];
  TVector3 Vect2      = ClosePoints[0] - ClosePoints[2];
  TVector3 NormalVect = Vect1.Cross(Vect2);
  NormalVect          = NormalVect.Unit();
  
  double residue;
  if(TMath::Abs(NormalVect.Z()) < 1.0e-6) {
    residue = 0.0;
    for(int i=0;i<3;i++) residue += ClosePoints[i].Z();
    residue /= 3;
  }
  else {
    residue  = NormalVect.Dot(ClosePoints[0]);
    residue -= NormalVect.X()*E;
    residue -= NormalVect.Y()*sigma_sp;
    residue /= NormalVect.Z();
  }

  return residue;
  
}
//=========================================================================================================
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{

  //par[0] -> sigma_sp(U)
  //par[1] -> sigma_sp(V)
  //par[2] -> beam energy

  if(par[0] < h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin() || par[0] > h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmax()) {
    cout << endl;
    cout << "In fcn function: the parameters 0 <-> sigma_sp(U) has value " << par[0] << " um outside limits (" 
         <<  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin() << "," << h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmax() << ") um."
	 << ". Exiting now!!!"
         << endl;
    cout << endl;
    
    assert(false);
  }
  if(par[1] < h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin() || par[1] > h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmax()) {
    cout << endl;
    cout << "In fcn function: the parameters 1 <-> sigma_sp(V) has value " << par[1] << " um outside limits (" 
         <<  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin() << "," << h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmax() << ") um."
	 << ". Exiting now!!!"
         << endl;
    cout << endl;
    
    assert(false);
  }
  if(par[2] < h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmin() || par[2] > h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmax()) {
    cout << endl;
    cout << "In fcn function: the parameters 2 <-> Ebeam has value " << par[2] << " GeV outside limits (" 
         <<  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmin() << "," << h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmax() << ") GeV."
	 << ". Exiting now!!!"
         << endl;
    cout << endl;
    
    assert(false);
  }
  
  int idx_sigmaU = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->FindBin(par[0]);
  int idx_sigmaV = h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->FindBin(par[1]);
  int idx_Ebeam  = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->FindBin(par[2]);
  
  double chi2U = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetBinContent(idx_sigmaU,idx_Ebeam);
  double chi2V = h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetBinContent(idx_sigmaV,idx_Ebeam);
  double chisq = chi2U + chi2V;
  
  f = chisq;
  
  return;
  
}
//=========================================================================================================
void Spatial_Resolution_BeamEScan(const char* DataCard)
{

  std::vector<int>     _PlNb;
  std::vector<double>  _residueU_data;
  std::vector<double>  _residueU_err_data;
  std::vector<double>  _residueV_data;
  std::vector<double>  _residueV_err_data;

  const int MaxPlanes(30);
  const int MaxEPoints(60);
  std::vector<TString> _MCFilesList[MaxPlanes];
  std::vector<double>  _EnergyValues[MaxPlanes];
  TH1F* h_TelescopeResolution_Scan_U[MaxPlanes][MaxEPoints];
  TH1F* h_TelescopeResolution_Scan_V[MaxPlanes][MaxEPoints];
  TH1F* h_ResidualAtDUT_Scan_U[MaxPlanes][MaxEPoints];
  TH1F* h_ResidualAtDUT_Scan_V[MaxPlanes][MaxEPoints];
  
  //Set starting values and step sizes for parameters
  static double vstart[3] = {4.50,4.50,4.00};
  static double step[3]   = {0.01,0.01,0.01};

  TLatex* latex = new TLatex();
  latex->SetTextAlign(12);
  
  double Systematic         = 0.05;  //um
  double Systematic_Residue = 0.05;  //um
  double Systematic_E       = -999.9;
  
  gStyle->SetOptFit(1);

  TString tmp_file("tmp_38994.txt");
  TString tmp_file2("tmp_66666.txt");
  TString command;
  TString HistName;
  TString HistTitle;
  TString file_out;
  TString MCFileList;
  TString MiName;

  //int colors[5] = {1,2,4,kGreen+2,kOrange};

  char ccc1[500];
  char ccc2[500];
  int    PlNb;
  double PlPos;

  //Pixel Characteristics:
  double Diode_Surf;
  double Diode_FP;
  double Ampl1;
  double Ampl2;
  TString CapType;
  double PitchU;
  double PitchV;

  std::vector<double>  _PlLocation;
  std::vector<TString> _DataFile;
  _PlNb.clear();
  _PlLocation.clear();
  _DataFile.clear();

  //Getting the Data:
  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep Data: > ") + tmp_file;
  gSystem->Exec(command.Data());
  
  ifstream fp1(tmp_file.Data());
  while(fp1
	>> ccc1
	>> PlNb
	>> PlPos
	>> ccc2
	) {
    _PlNb.push_back(PlNb);
    _PlLocation.push_back(PlPos);
    _DataFile.push_back(TString(ccc2));
  }
  fp1.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());
  
  
  //Ordering files w.r.t plane position
  for(int iii=2;iii<=int(_PlLocation.size());iii++) {
    for(int jjj=0;jjj<=int(_PlLocation.size())-iii;jjj++) {
      double Z_jjj   = _PlLocation[jjj];
      double Z_jjjp1 = _PlLocation[jjj+1];
      
      if(Z_jjj > Z_jjjp1) {
	double Z_aux       = _PlLocation[jjj];
        _PlLocation[jjj]   = _PlLocation[jjj+1];
        _PlLocation[jjj+1] = Z_aux;
	  
	int Nb_aux   = _PlNb[jjj];
        _PlNb[jjj]   = _PlNb[jjj+1];
        _PlNb[jjj+1] = Nb_aux;
	  
	TString File_aux = _DataFile[jjj];
        _DataFile[jjj]   = _DataFile[jjj+1];
        _DataFile[jjj+1] = File_aux;
      }
	
    }
  }

  const int N(_PlNb.size());
  TH1F* huCGtu1[N];
  TH1F* hvCGtu1[N];
  for(int i=0;i<N;i++) {
    huCGtu1[i] = NULL;
    hvCGtu1[i] = NULL;

    cout << i+1 << "  " << _PlNb[i] << "  " << _DataFile[i].Data() << "  location = " << _PlLocation[i] << endl;
    TFile ftmp_data(_DataFile[i].Data());
    HistName  = TString("huCGtu1");
    huCGtu1[i] = (TH1F*)ftmp_data.Get(HistName.Data());
    HistName += TString("_Pl") + long(_PlNb[i]);
    huCGtu1[i]->SetName(HistName.Data());
    HistTitle = TString("Tu - huCG for Plane ") + long(_PlNb[i]);
    huCGtu1[i]->SetTitle(HistTitle.Data());
    huCGtu1[i]->SetDirectory(0);
    
    HistName  = TString("hvCGtu1");
    hvCGtu1[i] = (TH1F*)ftmp_data.Get(HistName.Data());
    HistName += TString("_Pl") + long(_PlNb[i]);
    hvCGtu1[i]->SetName(HistName.Data());
    HistTitle = TString("Tv - hvCG for Plane ") + long(_PlNb[i]);
    hvCGtu1[i]->SetTitle(HistTitle.Data());
    hvCGtu1[i]->SetDirectory(0);
    ftmp_data.Close();
  }

  //Getting the Pixels characteristics:
  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep Diode: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp2(tmp_file.Data());
  while(fp2 >> ccc1 >> Diode_Surf >> Diode_FP) {}
  fp2.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());

  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep Ampl: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp3(tmp_file.Data());
  while(fp3 >> ccc1 >> Ampl1 >> Ampl2) {}
  fp3.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());

  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep Cap: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp4(tmp_file.Data());
  while(fp4 >> ccc1 >> ccc2) {
    CapType = TString(ccc2);
  }
  fp4.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());

  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep Pitch: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp5(tmp_file.Data());
  while(fp5 >> ccc1 >> PitchU >> PitchV) {}
  fp5.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());

  cout << endl;
  cout << "Pixel Characteristics:" << endl;
  cout << "Diode (Surf/Footh-Print)(um2): " << Diode_Surf << "/" << Diode_FP << endl;
  cout << "Ampli (um)                   : " << Ampl1      << "/" << Ampl2    << endl; 
  cout << "Cap                          : " << CapType.Data() << endl;
  cout << "Pitch (um)                   : " << PitchU     << "x" << PitchV   << endl;
  
  //Getting output file:
  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep Output: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp6(tmp_file.Data());
  while(fp6 >> ccc1 >> ccc2) {
    file_out = TString(ccc2);
  }
  fp6.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());

  cout << endl;
  cout << "Output file: " << file_out.Data() << endl;

  //Getting Sensor Name file:
  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep MimosaName: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp7(tmp_file.Data());
  while(fp7 >> ccc1 >> ccc2) {
    MiName = TString(ccc2);
  }
  fp7.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());

  cout << endl;
  cout << "Sensor Name: " << MiName.Data() << endl;

  //Getting MC files list:
  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep MC: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp8(tmp_file.Data());
  while(fp8 >> ccc1 >> ccc2) {
    MCFileList = TString(ccc2);
  }
  fp8.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());
  
  cout << endl;
  cout << "MC File List: " << MCFileList.Data() << endl;

  //Getting The systematic error:
  command = TString("cat ") + TString(DataCard) + TString(" | grep -v // | grep Syst: > ") + tmp_file;
  gSystem->Exec(command.Data());
  ifstream fp81(tmp_file.Data());
  while(fp81 >> ccc1 >> Systematic) {}
  fp81.close();
  command = TString("rm ") + tmp_file;
  gSystem->Exec(command.Data());
  
  cout << endl;
  cout << "Systematic error: " << Systematic << " um" << endl;

  cout << endl;
  cout << endl;

  double Eval;
  ifstream fp9(MCFileList.Data());
  while(fp9 >> ccc1 >> Eval >> PlNb) {
    for(int i=0;i<N;i++) {
      if(PlNb == _PlNb[i]) {
	_MCFilesList[i].push_back(TString(ccc1));
	_EnergyValues[i].push_back(Eval);
	break;
      }
    }
  }
  fp9.close();

  for(int i=0;i<N;i++) {
    cout << "The MC File for Pl" << _PlNb[i] << endl;
    for(int k=0;k<int(_MCFilesList[i].size());k++) {
      cout << " " << _MCFilesList[i][k].Data() << "  Eval = " << _EnergyValues[i][k] << " GeV" << endl;

      h_TelescopeResolution_Scan_U[i][k] = NULL;
      h_TelescopeResolution_Scan_V[i][k] = NULL;
      h_ResidualAtDUT_Scan_U[i][k]       = NULL;
      h_ResidualAtDUT_Scan_V[i][k]       = NULL;
      
      TFile ftmp_mc(_MCFilesList[i][k].Data());
      HistName  = TString("h_TelescopeResolution_Scan_U");
      h_TelescopeResolution_Scan_U[i][k] = (TH1F*)ftmp_mc.Get(HistName.Data());
      HistName += TString("_G4_Pl") + long(_PlNb[i]) + TString("Estep") + long(k+1);
      h_TelescopeResolution_Scan_U[i][k]->SetName(HistName.Data());
      sprintf(ccc1,"%.1f",_EnergyValues[i][k]);
      HistTitle = TString("#sigma_{Tel}(U) vs #sigma_{sp} for Plane ") + long(_PlNb[i]) + TString(" and Beam Energy ") + TString(ccc1) + TString("GeV");
      h_TelescopeResolution_Scan_U[i][k]->SetTitle(HistTitle.Data());
      h_TelescopeResolution_Scan_U[i][k]->SetDirectory(0);
      
      HistName  = TString("h_TelescopeResolution_Scan_V");
      h_TelescopeResolution_Scan_V[i][k] = (TH1F*)ftmp_mc.Get(HistName.Data());
      HistName += TString("_G4_Pl") + long(_PlNb[i]) + TString("Estep") + long(k+1);
      h_TelescopeResolution_Scan_V[i][k]->SetName(HistName.Data());
      sprintf(ccc1,"%.1f",_EnergyValues[i][k]);
      HistTitle = TString("#sigma_{Tel}(V) vs #sigma_{sp} for Plane ") + long(_PlNb[i]) + TString(" and Beam Energy ") + TString(ccc1) + TString("GeV");
      h_TelescopeResolution_Scan_V[i][k]->SetTitle(HistTitle.Data());
      h_TelescopeResolution_Scan_V[i][k]->SetDirectory(0);
      
      HistName  = TString("h_ResidualAtDUT_Scan_U");
      h_ResidualAtDUT_Scan_U[i][k] = (TH1F*)ftmp_mc.Get(HistName.Data());
      HistName += TString("_G4_Pl") + long(_PlNb[i]) + TString("Estep") + long(k+1);
      h_ResidualAtDUT_Scan_U[i][k]->SetName(HistName.Data());
      sprintf(ccc1,"%.1f",_EnergyValues[i][k]);
      HistTitle = TString("#sigma_{residue}(U) vs #sigma_{sp} for Plane ") + long(_PlNb[i]) + TString(" and Beam Energy ") + TString(ccc1) + TString("GeV");
      h_ResidualAtDUT_Scan_U[i][k]->SetTitle(HistTitle.Data());
      h_ResidualAtDUT_Scan_U[i][k]->SetDirectory(0);
      
      HistName  = TString("h_ResidualAtDUT_Scan_V");
      h_ResidualAtDUT_Scan_V[i][k] = (TH1F*)ftmp_mc.Get(HistName.Data());
      HistName += TString("_G4_Pl") + long(_PlNb[i]) + TString("Estep") + long(k+1);
      h_ResidualAtDUT_Scan_V[i][k]->SetName(HistName.Data());
      sprintf(ccc1,"%.1f",_EnergyValues[i][k]);
      HistTitle = TString("#sigma_{residue}(U) vs #sigma_{sp} for Plane ") + long(_PlNb[i]) + TString(" and Beam Energy ") + TString(ccc1) + TString("GeV");
      h_ResidualAtDUT_Scan_V[i][k]->SetTitle(HistTitle.Data());
      h_ResidualAtDUT_Scan_V[i][k]->SetDirectory(0);
      ftmp_mc.Close();

    }
    cout << endl;
  }
  cout << endl;

  //Ordering the MC files w.r.t. energy
  for(int i=0;i<N;i++) {
    for(int iii=2;iii<=int(_EnergyValues[i].size());iii++) {
      for(int jjj=0;jjj<=int(_EnergyValues[i].size())-iii;jjj++) {
	double E_jjj   = _EnergyValues[i][jjj];
        double E_jjjp1 = _EnergyValues[i][jjj+1];
	
	if(E_jjj > E_jjjp1) {
	  double E_aux            = _EnergyValues[i][jjj];
          _EnergyValues[i][jjj]   = _EnergyValues[i][jjj+1];
          _EnergyValues[i][jjj+1] = E_aux;
	  
	  TH1F* h_tmp                            = h_TelescopeResolution_Scan_U[i][jjj];
	  h_TelescopeResolution_Scan_U[i][jjj]   = h_TelescopeResolution_Scan_U[i][jjj+1];
	  h_TelescopeResolution_Scan_U[i][jjj+1] = h_tmp;
	  
	  h_tmp                                  = h_TelescopeResolution_Scan_V[i][jjj];
	  h_TelescopeResolution_Scan_V[i][jjj]   = h_TelescopeResolution_Scan_V[i][jjj+1];
	  h_TelescopeResolution_Scan_V[i][jjj+1] = h_tmp;
	  
	  h_tmp                                  = h_ResidualAtDUT_Scan_U[i][jjj];
	  h_ResidualAtDUT_Scan_U[i][jjj]         = h_ResidualAtDUT_Scan_U[i][jjj+1];
	  h_ResidualAtDUT_Scan_U[i][jjj+1]       = h_tmp;
	  
	  h_tmp                                  = h_ResidualAtDUT_Scan_V[i][jjj];
	  h_ResidualAtDUT_Scan_V[i][jjj]         = h_ResidualAtDUT_Scan_V[i][jjj+1];
	  h_ResidualAtDUT_Scan_V[i][jjj+1]       = h_tmp;
	}
	
      }
    }
  }
  
  //Define systematic in energy as the smaller step of the MC simulation files
  Systematic_E = 1.0e+20;
  for(int i=0;i<N;i++) {
    for(int iii=0;iii<int(_EnergyValues[i].size()-1);iii++) {
      double E_step = _EnergyValues[i][iii+1] - _EnergyValues[i][iii];
      if(Systematic_E > E_step) Systematic_E = E_step;
    }
  }
  cout << endl;
  cout << "Syst. error resolution         = " << Systematic              << " um" << endl;
  cout << "Syst. error residue            = " << Systematic_Residue      << " um" << endl;
  cout << "Syst. error Ebeam              = " << Systematic_E            << " GeV" << endl;
  cout << "Syst. error Ebeam (1/sqrt(12)) = " << Systematic_E/sqrt(12.0) << " GeV" << endl;
  cout << endl;
  Systematic_E /= sqrt(12.0);
  
  
  double Rsigma[2];
  Rsigma[0] = -1.0e+20;
  Rsigma[1] = +1.0e+20;
  double RE[2];
  RE[0] = +1.0e+20;
  RE[1] = -1.0e+20;
  for(int i=0;i<N;i++) {
    for(int k=1;k<int(_EnergyValues[i].size()-1);k++) {
      if(RE[0] > _EnergyValues[i][k]) RE[0] = _EnergyValues[i][k];
      if(RE[1] < _EnergyValues[i][k]) RE[1] = _EnergyValues[i][k];
      
      double Mymin = h_ResidualAtDUT_Scan_U[i][k]->GetBinCenter(1);
      double Mymax = h_ResidualAtDUT_Scan_U[i][k]->GetBinCenter(h_ResidualAtDUT_Scan_U[i][k]->GetXaxis()->GetNbins());
      
      if(Rsigma[0] < Mymin) Rsigma[0] = Mymin;
      if(Rsigma[1] > Mymin) Rsigma[1] = Mymax;
    }
  }
  
  cout << endl;
  cout << "E-range        = (" << RE[0]     << "," << RE[1]     << ") GeV" << endl;
  cout << "sigma_sp-range = (" << Rsigma[0] << "," << Rsigma[1] << ") um"  << endl;
  cout << endl;

  //Set starting values and step sizes for parameters
  vstart[0] = 0.5*(Rsigma[0] + Rsigma[1]);
  vstart[1] = 0.5*(Rsigma[0] + Rsigma[1]);
  vstart[2] = 0.5*(RE[0]     + RE[1]);
  
  //Defining Pl location range:
  double RPlLocation[2];
  RPlLocation[0] = +1.0e+20;
  RPlLocation[1] = -1.0e+20;
  for(int i=0;i<N;i++) {
    if(RPlLocation[0] > _PlLocation[i]) RPlLocation[0] = _PlLocation[i];
    if(RPlLocation[1] < _PlLocation[i]) RPlLocation[1] = _PlLocation[i];
  }
  double porcent_Range = 0.10;
  double delta = RPlLocation[1] - RPlLocation[0];
  RPlLocation[0] -= delta*porcent_Range;
  RPlLocation[1] += delta*porcent_Range;

  cout << endl;
  cout << "Plane location Range = (" << RPlLocation[0] << "," << RPlLocation[1] << ") cm" << endl;
  cout << endl;

  double TheSize = 0.05;

  int color_data    = kBlack;
  int color_mcG4    = kGreen+2;
  int fillstyle     = 3013;
  
  //Booking the histos and TGraphErrors

  //The Frame histo to draw the plots:
  sprintf(ccc1,"%.1f",Diode_Surf);
  HistTitle  = MiName + TString(", Diode: ") + TString(ccc1) + TString("/");
  sprintf(ccc1,"%.1f",Diode_FP);
  HistTitle += TString(ccc1) + TString("#mum^{2}, Amp: ");
  sprintf(ccc1,"%.2f",Ampl1);
  HistTitle += TString(ccc1) + TString("/");
  sprintf(ccc1,"%.1f",Ampl2);
  HistTitle += TString(ccc1) + TString("#mum, Cap: ") + CapType + TString(", Pitch: ");
  sprintf(ccc1,"%.1f",PitchU);
  HistTitle += TString(ccc1) + TString("#times");
  sprintf(ccc1,"%.1f",PitchV);
  HistTitle += TString(ccc1) + TString("#mum^{2}");
  TH1D h_REF("h_REF",HistTitle.Data(),100,RPlLocation[0],RPlLocation[1]);
  h_REF.SetXTitle("Plane Position (cm)");
  h_REF.GetXaxis()->CenterTitle(true);
  h_REF.SetYTitle("#sigma_{residue} (#mum)");
  h_REF.GetYaxis()->CenterTitle(true);
  h_REF.SetLineColor(1);
  h_REF.GetXaxis()->SetTitleSize(TheSize);
  h_REF.GetXaxis()->SetLabelSize(TheSize);
  h_REF.GetYaxis()->SetTitleSize(TheSize);
  h_REF.GetYaxis()->SetLabelSize(TheSize);
  h_REF.SetStats(false);
  
  //Experimental residue
  TGraphErrors gresidueU_data(N);
  gresidueU_data.SetName("gresidueU_data");
  gresidueU_data.SetTitle(h_REF.GetTitle());
  gresidueU_data.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueU_data.GetXaxis()->CenterTitle(true);
  gresidueU_data.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueU_data.GetYaxis()->CenterTitle(true);
  gresidueU_data.SetLineColor(color_data);
  gresidueU_data.SetMarkerColor(color_data);
  gresidueU_data.SetMarkerStyle(20);
  gresidueU_data.SetLineWidth(2);
  gresidueU_data.SetLineStyle(1);
  gresidueU_data.GetXaxis()->SetTitleSize(TheSize);
  gresidueU_data.GetXaxis()->SetLabelSize(TheSize);
  gresidueU_data.GetYaxis()->SetTitleSize(TheSize);
  gresidueU_data.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueV_data(N);
  gresidueV_data.SetName("gresidueV_data");
  gresidueV_data.SetTitle(h_REF.GetTitle());
  gresidueV_data.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueV_data.GetXaxis()->CenterTitle(true);
  gresidueV_data.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueV_data.GetYaxis()->CenterTitle(true);
  gresidueV_data.SetLineColor(color_data);
  gresidueV_data.SetMarkerColor(color_data);
  gresidueV_data.SetMarkerStyle(20);
  gresidueV_data.SetLineWidth(2);
  gresidueV_data.SetLineStyle(2);
  gresidueV_data.GetXaxis()->SetTitleSize(TheSize);
  gresidueV_data.GetXaxis()->SetLabelSize(TheSize);
  gresidueV_data.GetYaxis()->SetTitleSize(TheSize);
  gresidueV_data.GetYaxis()->SetLabelSize(TheSize);

  TGraphErrors gresidueU_G4_Global(N);
  gresidueU_G4_Global.SetName("gresidueU_G4_Global");
  gresidueU_G4_Global.SetTitle(h_REF.GetTitle());
  gresidueU_G4_Global.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueU_G4_Global.GetXaxis()->CenterTitle(true);
  gresidueU_G4_Global.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueU_G4_Global.GetYaxis()->CenterTitle(true);
  gresidueU_G4_Global.SetLineColor(color_mcG4);
  gresidueU_G4_Global.SetMarkerColor(color_mcG4);
  gresidueU_G4_Global.SetMarkerStyle(20);
  gresidueU_G4_Global.SetLineWidth(2);
  gresidueU_G4_Global.SetLineStyle(1);
  gresidueU_G4_Global.GetXaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global.GetXaxis()->SetLabelSize(TheSize);
  gresidueU_G4_Global.GetYaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueU_G4_Global_Shade(2*N);
  gresidueU_G4_Global_Shade.SetName("gresidueU_G4_Global_Shade");
  gresidueU_G4_Global_Shade.SetTitle(h_REF.GetTitle());
  gresidueU_G4_Global_Shade.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueU_G4_Global_Shade.GetXaxis()->CenterTitle(true);
  gresidueU_G4_Global_Shade.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueU_G4_Global_Shade.GetYaxis()->CenterTitle(true);
  gresidueU_G4_Global_Shade.SetLineColor(color_mcG4);
  gresidueU_G4_Global_Shade.SetMarkerColor(color_mcG4);
  gresidueU_G4_Global_Shade.SetFillColor(color_mcG4);
  gresidueU_G4_Global_Shade.SetFillStyle(fillstyle);
  //gresidueU_G4_Global_Shade.SetMarkerStyle(20);
  //gresidueU_G4_Global_Shade.SetLineWidth(2);
  //gresidueU_G4_Global_Shade.SetLineStyle(1);
  gresidueU_G4_Global_Shade.GetXaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global_Shade.GetXaxis()->SetLabelSize(TheSize);
  gresidueU_G4_Global_Shade.GetYaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global_Shade.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueU_G4_Global_max(N);
  gresidueU_G4_Global_max.SetName("gresidueU_G4_Global_max");
  gresidueU_G4_Global_max.SetTitle(h_REF.GetTitle());
  gresidueU_G4_Global_max.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueU_G4_Global_max.GetXaxis()->CenterTitle(true);
  gresidueU_G4_Global_max.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueU_G4_Global_max.GetYaxis()->CenterTitle(true);
  gresidueU_G4_Global_max.SetLineColor(color_mcG4);
  gresidueU_G4_Global_max.SetMarkerColor(color_mcG4);
  //gresidueU_G4_Global_max.SetMarkerStyle(20);
  //gresidueU_G4_Global_max.SetLineWidth(2);
  gresidueU_G4_Global_max.SetLineStyle(1);
  gresidueU_G4_Global_max.GetXaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global_max.GetXaxis()->SetLabelSize(TheSize);
  gresidueU_G4_Global_max.GetYaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global_max.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueU_G4_Global_min(N);
  gresidueU_G4_Global_min.SetName("gresidueU_G4_Global_min");
  gresidueU_G4_Global_min.SetTitle(h_REF.GetTitle());
  gresidueU_G4_Global_min.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueU_G4_Global_min.GetXaxis()->CenterTitle(true);
  gresidueU_G4_Global_min.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueU_G4_Global_min.GetYaxis()->CenterTitle(true);
  gresidueU_G4_Global_min.SetLineColor(color_mcG4);
  gresidueU_G4_Global_min.SetMarkerColor(color_mcG4);
  //gresidueU_G4_Global_min.SetMarkerStyle(20);
  //gresidueU_G4_Global_min.SetLineWidth(2);
  gresidueU_G4_Global_min.SetLineStyle(1);
  gresidueU_G4_Global_min.GetXaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global_min.GetXaxis()->SetLabelSize(TheSize);
  gresidueU_G4_Global_min.GetYaxis()->SetTitleSize(TheSize);
  gresidueU_G4_Global_min.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueV_G4_Global(N);
  gresidueV_G4_Global.SetName("gresidueV_G4_Global");
  gresidueV_G4_Global.SetTitle(h_REF.GetTitle());
  gresidueV_G4_Global.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueV_G4_Global.GetXaxis()->CenterTitle(true);
  gresidueV_G4_Global.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueV_G4_Global.GetYaxis()->CenterTitle(true);
  gresidueV_G4_Global.SetLineColor(color_mcG4);
  gresidueV_G4_Global.SetMarkerColor(color_mcG4);
  gresidueV_G4_Global.SetMarkerStyle(20);
  gresidueV_G4_Global.SetLineWidth(2);
  gresidueV_G4_Global.SetLineStyle(2);
  gresidueV_G4_Global.GetXaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global.GetXaxis()->SetLabelSize(TheSize);
  gresidueV_G4_Global.GetYaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueV_G4_Global_Shade(2*N);
  gresidueV_G4_Global_Shade.SetName("gresidueV_G4_Global_Shade");
  gresidueV_G4_Global_Shade.SetTitle(h_REF.GetTitle());
  gresidueV_G4_Global_Shade.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueV_G4_Global_Shade.GetXaxis()->CenterTitle(true);
  gresidueV_G4_Global_Shade.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueV_G4_Global_Shade.GetYaxis()->CenterTitle(true);
  gresidueV_G4_Global_Shade.SetLineColor(color_mcG4);
  gresidueV_G4_Global_Shade.SetMarkerColor(color_mcG4);
  gresidueV_G4_Global_Shade.SetFillColor(color_mcG4);
  gresidueV_G4_Global_Shade.SetFillStyle(fillstyle);
  //gresidueV_G4_Global_Shade.SetMarkerStyle(20);
  //gresidueV_G4_Global_Shade.SetLineWidth(2);
  //gresidueV_G4_Global_Shade.SetLineStyle(1);
  gresidueV_G4_Global_Shade.GetXaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global_Shade.GetXaxis()->SetLabelSize(TheSize);
  gresidueV_G4_Global_Shade.GetYaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global_Shade.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueV_G4_Global_max(N);
  gresidueV_G4_Global_max.SetName("gresidueV_G4_Global_max");
  gresidueV_G4_Global_max.SetTitle(h_REF.GetTitle());
  gresidueV_G4_Global_max.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueV_G4_Global_max.GetXaxis()->CenterTitle(true);
  gresidueV_G4_Global_max.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueV_G4_Global_max.GetYaxis()->CenterTitle(true);
  gresidueV_G4_Global_max.SetLineColor(color_mcG4);
  gresidueV_G4_Global_max.SetMarkerColor(color_mcG4);
  //gresidueV_G4_Global_max.SetMarkerStyle(20);
  //gresidueV_G4_Global_max.SetLineWidth(2);
  gresidueV_G4_Global_max.SetLineStyle(2);
  gresidueV_G4_Global_max.GetXaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global_max.GetXaxis()->SetLabelSize(TheSize);
  gresidueV_G4_Global_max.GetYaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global_max.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gresidueV_G4_Global_min(N);
  gresidueV_G4_Global_min.SetName("gresidueV_G4_Global_min");
  gresidueV_G4_Global_min.SetTitle(h_REF.GetTitle());
  gresidueV_G4_Global_min.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gresidueV_G4_Global_min.GetXaxis()->CenterTitle(true);
  gresidueV_G4_Global_min.GetYaxis()->SetTitle("#sigma_{residude} (#mum)");
  gresidueV_G4_Global_min.GetYaxis()->CenterTitle(true);
  gresidueV_G4_Global_min.SetLineColor(color_mcG4);
  gresidueV_G4_Global_min.SetMarkerColor(color_mcG4);
  //gresidueV_G4_Global_min.SetMarkerStyle(20);
  //gresidueV_G4_Global_min.SetLineWidth(2);
  gresidueV_G4_Global_min.SetLineStyle(2);
  gresidueV_G4_Global_min.GetXaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global_min.GetXaxis()->SetLabelSize(TheSize);
  gresidueV_G4_Global_min.GetYaxis()->SetTitleSize(TheSize);
  gresidueV_G4_Global_min.GetYaxis()->SetLabelSize(TheSize);

  //Telescope resolutio at DUT position from MC  
  TGraphErrors gTelresoU_G4_Global(N);
  gTelresoU_G4_Global.SetName("gTelresoU_G4_Global");
  gTelresoU_G4_Global.SetTitle(h_REF.GetTitle());
  gTelresoU_G4_Global.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoU_G4_Global.GetXaxis()->CenterTitle(true);
  gTelresoU_G4_Global.GetYaxis()->SetTitle("#sigma_{Tel}(U) (#mum)");
  gTelresoU_G4_Global.GetYaxis()->CenterTitle(true);
  gTelresoU_G4_Global.SetLineColor(color_mcG4);
  gTelresoU_G4_Global.SetMarkerColor(color_mcG4);
  gTelresoU_G4_Global.SetMarkerStyle(20);
  gTelresoU_G4_Global.SetLineWidth(2);
  gTelresoU_G4_Global.SetLineStyle(1);
  gTelresoU_G4_Global.GetXaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global.GetXaxis()->SetLabelSize(TheSize);
  gTelresoU_G4_Global.GetYaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gTelresoU_G4_Global_Shade(2*N);
  gTelresoU_G4_Global_Shade.SetName("gTelresoU_G4_Global_Shade");
  gTelresoU_G4_Global_Shade.SetTitle(h_REF.GetTitle());
  gTelresoU_G4_Global_Shade.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoU_G4_Global_Shade.GetXaxis()->CenterTitle(true);
  gTelresoU_G4_Global_Shade.GetYaxis()->SetTitle("#sigma_{Tel}(U) (#mum)");
  gTelresoU_G4_Global_Shade.GetYaxis()->CenterTitle(true);  
  gTelresoU_G4_Global_Shade.SetLineColor(color_mcG4);
  gTelresoU_G4_Global_Shade.SetMarkerColor(color_mcG4);
  gTelresoU_G4_Global_Shade.SetFillColor(color_mcG4);
  gTelresoU_G4_Global_Shade.SetFillStyle(fillstyle);
  //gTelresoU_G4_Global_Shade.SetMarkerStyle(20);
  //gTelresoU_G4_Global_Shade.SetLineWidth(2);
  //gTelresoU_G4_Global_Shade.SetLineStyle(1);
  gTelresoU_G4_Global_Shade.GetXaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global_Shade.GetXaxis()->SetLabelSize(TheSize);
  gTelresoU_G4_Global_Shade.GetYaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global_Shade.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gTelresoU_G4_Global_max(N);
  gTelresoU_G4_Global_max.SetName("gTelresoU_G4_Global_max");
  gTelresoU_G4_Global_max.SetTitle(h_REF.GetTitle());
  gTelresoU_G4_Global_max.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoU_G4_Global_max.GetXaxis()->CenterTitle(true);
  gTelresoU_G4_Global_max.GetYaxis()->SetTitle("#sigma_{Tel}(U) (#mum)");
  gTelresoU_G4_Global_max.GetYaxis()->CenterTitle(true);
  gTelresoU_G4_Global_max.SetLineColor(color_mcG4);
  gTelresoU_G4_Global_max.SetMarkerColor(color_mcG4);
  //gTelresoU_G4_Global_max.SetMarkerStyle(20);
  //gTelresoU_G4_Global_max.SetLineWidth(2);
  gTelresoU_G4_Global_max.SetLineStyle(1);
  gTelresoU_G4_Global_max.GetXaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global_max.GetXaxis()->SetLabelSize(TheSize);
  gTelresoU_G4_Global_max.GetYaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global_max.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gTelresoU_G4_Global_min(N);
  gTelresoU_G4_Global_min.SetName("gTelresoU_G4_Global_min");
  gTelresoU_G4_Global_min.SetTitle(h_REF.GetTitle());
  gTelresoU_G4_Global_min.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoU_G4_Global_min.GetXaxis()->CenterTitle(true);
  gTelresoU_G4_Global_min.GetYaxis()->SetTitle("#sigma_{Tel}(U) (#mum)");
  gTelresoU_G4_Global_min.GetYaxis()->CenterTitle(true);
  gTelresoU_G4_Global_min.SetLineColor(color_mcG4);
  gTelresoU_G4_Global_min.SetMarkerColor(color_mcG4);
  //gTelresoU_G4_Global_min.SetMarkerStyle(20);
  //gTelresoU_G4_Global_min.SetLineWidth(2);
  gTelresoU_G4_Global_min.SetLineStyle(1);
  gTelresoU_G4_Global_min.GetXaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global_min.GetXaxis()->SetLabelSize(TheSize);
  gTelresoU_G4_Global_min.GetYaxis()->SetTitleSize(TheSize);
  gTelresoU_G4_Global_min.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gTelresoV_G4_Global(N);
  gTelresoV_G4_Global.SetName("gTelresoV_G4_Global");
  gTelresoV_G4_Global.SetTitle(h_REF.GetTitle());
  gTelresoV_G4_Global.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoV_G4_Global.GetXaxis()->CenterTitle(true);
  gTelresoV_G4_Global.GetYaxis()->SetTitle("#sigma_{Tel}(V) (#mum)");
  gTelresoV_G4_Global.GetYaxis()->CenterTitle(true);
  gTelresoV_G4_Global.SetLineColor(color_mcG4);
  gTelresoV_G4_Global.SetMarkerColor(color_mcG4);
  gTelresoV_G4_Global.SetMarkerStyle(20);
  gTelresoV_G4_Global.SetLineWidth(2);
  gTelresoV_G4_Global.SetLineStyle(2);
  gTelresoV_G4_Global.GetXaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global.GetXaxis()->SetLabelSize(TheSize);
  gTelresoV_G4_Global.GetYaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gTelresoV_G4_Global_Shade(2*N);
  gTelresoV_G4_Global_Shade.SetName("gTelresoV_G4_Global_Shade");
  gTelresoV_G4_Global_Shade.SetTitle(h_REF.GetTitle());
  gTelresoV_G4_Global_Shade.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoV_G4_Global_Shade.GetXaxis()->CenterTitle(true);
  gTelresoV_G4_Global_Shade.GetYaxis()->SetTitle("#sigma_{Tel}(V) (#mum)");
  gTelresoV_G4_Global_Shade.GetYaxis()->CenterTitle(true);  
  gTelresoV_G4_Global_Shade.SetLineColor(color_mcG4);
  gTelresoV_G4_Global_Shade.SetMarkerColor(color_mcG4);
  gTelresoV_G4_Global_Shade.SetFillColor(color_mcG4);
  gTelresoV_G4_Global_Shade.SetFillStyle(fillstyle);
  //gTelresoV_G4_Global_Shade.SetMarkerStyle(20);
  //gTelresoV_G4_Global_Shade.SetLineWidth(2);
  //gTelresoV_G4_Global_Shade.SetLineStyle(2);
  gTelresoV_G4_Global_Shade.GetXaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global_Shade.GetXaxis()->SetLabelSize(TheSize);
  gTelresoV_G4_Global_Shade.GetYaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global_Shade.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gTelresoV_G4_Global_max(N);
  gTelresoV_G4_Global_max.SetName("gTelresoV_G4_Global_max");
  gTelresoV_G4_Global_max.SetTitle(h_REF.GetTitle());
  gTelresoV_G4_Global_max.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoV_G4_Global_max.GetXaxis()->CenterTitle(true);
  gTelresoV_G4_Global_max.GetYaxis()->SetTitle("#sigma_{Tel}(V) (#mum)");
  gTelresoV_G4_Global_max.GetYaxis()->CenterTitle(true);
  gTelresoV_G4_Global_max.SetLineColor(color_mcG4);
  gTelresoV_G4_Global_max.SetMarkerColor(color_mcG4);
  //gTelresoV_G4_Global_max.SetMarkerStyle(20);
  //gTelresoV_G4_Global_max.SetLineWidth(2);
  gTelresoV_G4_Global_max.SetLineStyle(2);
  gTelresoV_G4_Global_max.GetXaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global_max.GetXaxis()->SetLabelSize(TheSize);
  gTelresoV_G4_Global_max.GetYaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global_max.GetYaxis()->SetLabelSize(TheSize);
  
  TGraphErrors gTelresoV_G4_Global_min(N);
  gTelresoV_G4_Global_min.SetName("gTelresoV_G4_Global_min");
  gTelresoV_G4_Global_min.SetTitle(h_REF.GetTitle());
  gTelresoV_G4_Global_min.GetXaxis()->SetTitle(h_REF.GetXaxis()->GetTitle());
  gTelresoV_G4_Global_min.GetXaxis()->CenterTitle(true);
  gTelresoV_G4_Global_min.GetYaxis()->SetTitle("#sigma_{Tel}(V) (#mum)");
  gTelresoV_G4_Global_min.GetYaxis()->CenterTitle(true);
  gTelresoV_G4_Global_min.SetLineColor(color_mcG4);
  gTelresoV_G4_Global_min.SetMarkerColor(color_mcG4);
  //gTelresoV_G4_Global_min.SetMarkerStyle(20);
  //gTelresoV_G4_Global_min.SetLineWidth(2);
  gTelresoV_G4_Global_min.SetLineStyle(2);
  gTelresoV_G4_Global_min.GetXaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global_min.GetXaxis()->SetLabelSize(TheSize);
  gTelresoV_G4_Global_min.GetYaxis()->SetTitleSize(TheSize);
  gTelresoV_G4_Global_min.GetYaxis()->SetLabelSize(TheSize);
  
  _residueU_data.clear();
  _residueU_err_data.clear();
  _residueV_data.clear();
  _residueV_err_data.clear();
  
  for(int i=0;i<N;i++) {
    double v,e;
    double c,w;
    //double sigma_res[2];
    c = _PlLocation[i];
    w = 1.0e-8;

    //The data residues
    v = huCGtu1[i]->GetRMS();
    e = huCGtu1[i]->GetRMSError();
    e = pow(e,2);
    e += pow(Systematic_Residue,2);
    e = sqrt(e);
    TF1* funcU = NULL;
    funcU = huCGtu1[i]->GetFunction("gaus");
    v = funcU->GetParameter(2);
    e = funcU->GetParError(2);
    e = pow(e,2);
    e += pow(Systematic_Residue,2);
    e = sqrt(e);
    gresidueU_data.SetPoint(i,c,v);
    gresidueU_data.SetPointError(i,w,e);
    _residueU_data.push_back(v);
    _residueU_err_data.push_back(e);
    
    v = hvCGtu1[i]->GetRMS();
    e = hvCGtu1[i]->GetRMSError();
    e = pow(e,2);
    e += pow(Systematic_Residue,2);
    e = sqrt(e);
    TF1* funcV = NULL;
    funcV = hvCGtu1[i]->GetFunction("gaus");
    v = funcV->GetParameter(2);
    e = funcV->GetParError(2);
    e = pow(e,2);
    e += pow(Systematic_Residue,2);
    e = sqrt(e);
    gresidueV_data.SetPoint(i,c,v);
    gresidueV_data.SetPointError(i,w,e);
    _residueV_data.push_back(v);
    _residueV_err_data.push_back(e);
  }
  
  cout << endl;
  TheSize = 0.06;
  int Nbins_Sigma = 300;
  int Nbins_Ebeam = 300;
  TH2D* h_residueU_vs_SigmaSP_Ebeam[N];
  TH2D* h_residueV_vs_SigmaSP_Ebeam[N];
  TH2D* h_Chi2U_vs_SigmaSP_Ebeam[N];
  TH2D* h_Chi2V_vs_SigmaSP_Ebeam[N];
  TH2D* h_Chi2U_vs_SigmaSP_Ebeam_Tot = NULL;
  TH2D* h_Chi2V_vs_SigmaSP_Ebeam_Tot = NULL;
  TH2D* h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2 = NULL;
  TH2D* h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2 = NULL;
  for(int i=0;i<N;i++) {
    
    cout << "Building residue functions for Plane " << _PlNb[i] << endl;
    
    HistName  = TString("h_residueU_vs_SigmaSP_Ebeam_Pl") + long(_PlNb[i]);
    HistTitle = TString("#sigma_{residue}(U) vs #sigma_{sp}(U) and E_{beam} for Pl") + long(_PlNb[i]);
    h_residueU_vs_SigmaSP_Ebeam[i] = new TH2D(HistName.Data(),
                                              HistTitle.Data(),
					      Nbins_Sigma,Rsigma[0],Rsigma[1],
					      Nbins_Ebeam,RE[0],RE[1]);
    h_residueU_vs_SigmaSP_Ebeam[i]->SetXTitle("#sigma_{sp}(U) (#mum)");
    h_residueU_vs_SigmaSP_Ebeam[i]->GetXaxis()->CenterTitle(true);
    h_residueU_vs_SigmaSP_Ebeam[i]->SetYTitle("E_{beam} (GeV)");
    h_residueU_vs_SigmaSP_Ebeam[i]->GetYaxis()->CenterTitle(true);
    h_residueU_vs_SigmaSP_Ebeam[i]->SetZTitle("#sigma_{residue}(U) (#mum)");
    h_residueU_vs_SigmaSP_Ebeam[i]->GetZaxis()->CenterTitle(true);
    h_residueU_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetTitleSize(TheSize);
    h_residueU_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetLabelSize(TheSize);
    h_residueU_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetTitleSize(TheSize);
    h_residueU_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetLabelSize(TheSize);
    h_residueU_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetTitleSize(TheSize);
    h_residueU_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetLabelSize(TheSize);
    h_residueU_vs_SigmaSP_Ebeam[i]->SetStats(false);
    
    
    HistName  = TString("h_residueV_vs_SigmaSP_Ebeam_Pl") + long(_PlNb[i]);
    HistTitle = TString("#sigma_{residue}(V) vs #sigma_{sp}(V) and E_{beam} for Pl") + long(_PlNb[i]);
    h_residueV_vs_SigmaSP_Ebeam[i] = new TH2D(HistName.Data(),
                                              HistTitle.Data(),
					      Nbins_Sigma,Rsigma[0],Rsigma[1],
					      Nbins_Ebeam,RE[0],RE[1]);
    h_residueV_vs_SigmaSP_Ebeam[i]->SetXTitle("#sigma_{sp}(V) (#mum)");
    h_residueV_vs_SigmaSP_Ebeam[i]->GetXaxis()->CenterTitle(true);
    h_residueV_vs_SigmaSP_Ebeam[i]->SetYTitle("E_{beam} (GeV)");
    h_residueV_vs_SigmaSP_Ebeam[i]->GetYaxis()->CenterTitle(true);
    h_residueV_vs_SigmaSP_Ebeam[i]->SetZTitle("#sigma_{residue}(V) (#mum)");
    h_residueV_vs_SigmaSP_Ebeam[i]->GetZaxis()->CenterTitle(true);
    h_residueV_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetTitleSize(TheSize);
    h_residueV_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetLabelSize(TheSize);
    h_residueV_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetTitleSize(TheSize);
    h_residueV_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetLabelSize(TheSize);
    h_residueV_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetTitleSize(TheSize);
    h_residueV_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetLabelSize(TheSize);
    h_residueV_vs_SigmaSP_Ebeam[i]->SetStats(false);
    
    HistName  = TString("h_Chi2U_vs_SigmaSP_Ebeam_Pl") + long(_PlNb[i]);
    HistTitle = TString("#chi^{2}(U) vs #sigma_{sp}(U) and E_{beam} for Pl") + long(_PlNb[i]);
    h_Chi2U_vs_SigmaSP_Ebeam[i] = new TH2D(HistName.Data(),
                                              HistTitle.Data(),
					      Nbins_Sigma,Rsigma[0],Rsigma[1],
					      Nbins_Ebeam,RE[0],RE[1]);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->SetXTitle("#sigma_{sp}(U) (#mum)");
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetXaxis()->CenterTitle(true);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->SetYTitle("E_{beam} (GeV)");
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetYaxis()->CenterTitle(true);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->SetZTitle("#chi^{2}(U)");
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetZaxis()->CenterTitle(true);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetTitleSize(TheSize);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetLabelSize(TheSize);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetTitleSize(TheSize);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetLabelSize(TheSize);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetTitleSize(TheSize);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetLabelSize(TheSize);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->SetStats(false);
    
    
    HistName  = TString("h_Chi2V_vs_SigmaSP_Ebeam_Pl") + long(_PlNb[i]);
    HistTitle = TString("#chi^{2}(V) vs #sigma_{sp}(V) and E_{beam} for Pl") + long(_PlNb[i]);
    h_Chi2V_vs_SigmaSP_Ebeam[i] = new TH2D(HistName.Data(),
                                           HistTitle.Data(),
					   Nbins_Sigma,Rsigma[0],Rsigma[1],
					   Nbins_Ebeam,RE[0],RE[1]);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->SetXTitle("#sigma_{sp}(V) (#mum)");
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetXaxis()->CenterTitle(true);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->SetYTitle("E_{beam} (GeV)");
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetYaxis()->CenterTitle(true);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->SetZTitle("#chi^{2}(V)");
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetZaxis()->CenterTitle(true);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetTitleSize(TheSize);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetXaxis()->SetLabelSize(TheSize);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetTitleSize(TheSize);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetYaxis()->SetLabelSize(TheSize);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetTitleSize(TheSize);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->GetZaxis()->SetLabelSize(TheSize);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->SetStats(false);
    
    if(i==0) {
      HistName  = TString("h_Chi2U_vs_SigmaSP_Ebeam_Tot");
      HistTitle = TString("#chi^{2}_{tot}(U) vs #sigma_{sp}(U) and E_{beam}");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot = new TH2D(HistName.Data(),
                                             HistTitle.Data(),
	  				     Nbins_Sigma,Rsigma[0],Rsigma[1],
					     Nbins_Ebeam,RE[0],RE[1]);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->SetXTitle("#sigma_{sp}(U) (#mum)");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetXaxis()->CenterTitle(true);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->SetYTitle("E_{beam} (GeV)");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetYaxis()->CenterTitle(true);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->SetZTitle("#chi^{2}_{tot}(U)");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetZaxis()->CenterTitle(true);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetXaxis()->SetTitleSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetXaxis()->SetLabelSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetYaxis()->SetTitleSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetYaxis()->SetLabelSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetZaxis()->SetTitleSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetZaxis()->SetLabelSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->SetStats(false);
      
      HistName  = TString("h_Chi2V_vs_SigmaSP_Ebeam_Tot");
      HistTitle = TString("#chi^{2}_{tot}(V) vs #sigma_{sp}(V) and E_{beam}");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot = new TH2D(HistName.Data(),
                                             HistTitle.Data(),
	  				     Nbins_Sigma,Rsigma[0],Rsigma[1],
					     Nbins_Ebeam,RE[0],RE[1]);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->SetXTitle("#sigma_{sp}(V) (#mum)");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetXaxis()->CenterTitle(true);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->SetYTitle("E_{beam} (GeV)");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetYaxis()->CenterTitle(true);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->SetZTitle("#chi^{2}_{tot}(V)");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetZaxis()->CenterTitle(true);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetXaxis()->SetTitleSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetXaxis()->SetLabelSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetYaxis()->SetTitleSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetYaxis()->SetLabelSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetZaxis()->SetTitleSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetZaxis()->SetLabelSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->SetStats(false);
      
      HistName  = TString("h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2");
      HistTitle = TString("#chi^{2}_{tot}(U) vs #sigma_{sp}(U) and E_{beam}");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2 = new TH2D(HistName.Data(),
                                             HistTitle.Data(),
	  				     Nbins_Sigma,Rsigma[0],Rsigma[1],
					     Nbins_Ebeam,RE[0],RE[1]);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->SetXTitle("#sigma_{sp}(U) (#mum)");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->CenterTitle(true);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->SetYTitle("E_{beam} (GeV)");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->CenterTitle(true);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->SetZTitle("#chi^{2}_{tot}(U)");
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetZaxis()->CenterTitle(true);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->SetTitleSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->SetLabelSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->SetTitleSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->SetLabelSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetZaxis()->SetTitleSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetZaxis()->SetLabelSize(TheSize);
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->SetStats(false);
      
      HistName  = TString("h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2");
      HistTitle = TString("#chi^{2}_{tot}(V) vs #sigma_{sp}(V) and E_{beam}");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2 = new TH2D(HistName.Data(),
                                             HistTitle.Data(),
	  				     Nbins_Sigma,Rsigma[0],Rsigma[1],
					     Nbins_Ebeam,RE[0],RE[1]);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->SetXTitle("#sigma_{sp}(V) (#mum)");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->CenterTitle(true);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->SetYTitle("E_{beam} (GeV)");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->CenterTitle(true);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->SetZTitle("#chi^{2}_{tot}(V)");
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetZaxis()->CenterTitle(true);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->SetTitleSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->SetLabelSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->SetTitleSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->SetLabelSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetZaxis()->SetTitleSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetZaxis()->SetLabelSize(TheSize);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->SetStats(false);
    }
    
    for(int isigma=0;isigma<Nbins_Sigma;isigma++) {
      double sigma_sp = h_residueU_vs_SigmaSP_Ebeam[i]->GetXaxis()->GetBinCenter(isigma+1);
      for(int iE=0;iE<Nbins_Ebeam;iE++) {
	double Ebeam = h_residueU_vs_SigmaSP_Ebeam[i]->GetYaxis()->GetBinCenter(iE+1);
	
	double residueU = GetResidue(sigma_sp,Ebeam,h_ResidualAtDUT_Scan_U[i],_EnergyValues[i]);
	double residueV = GetResidue(sigma_sp,Ebeam,h_ResidualAtDUT_Scan_V[i],_EnergyValues[i]);
	
	double chi2U = pow((residueU - _residueU_data[i])/_residueU_err_data[i],2);
	double chi2V = pow((residueV - _residueV_data[i])/_residueV_err_data[i],2);
	
	h_residueU_vs_SigmaSP_Ebeam[i]->SetBinContent(isigma+1,iE+1,residueU);
	h_residueV_vs_SigmaSP_Ebeam[i]->SetBinContent(isigma+1,iE+1,residueV);
	
	h_Chi2U_vs_SigmaSP_Ebeam[i]->SetBinContent(isigma+1,iE+1,chi2U);
	h_Chi2V_vs_SigmaSP_Ebeam[i]->SetBinContent(isigma+1,iE+1,chi2V);
      }
    }
  }
  cout << endl;
  
  double chi2U_min = +1.0e+20;
  double chi2V_min = +1.0e+20;
  for(int isigma=0;isigma<Nbins_Sigma;isigma++) {
    for(int iE=0;iE<Nbins_Ebeam;iE++) {
      double chi2U = 0.0;
      double chi2V = 0.0;
      for(int i=0;i<N;i++) {
	chi2U += h_Chi2U_vs_SigmaSP_Ebeam[i]->GetBinContent(isigma+1,iE+1);
	chi2V += h_Chi2V_vs_SigmaSP_Ebeam[i]->GetBinContent(isigma+1,iE+1);
      }
      
      if(chi2U_min > chi2U) chi2U_min = chi2U;
      if(chi2V_min > chi2V) chi2V_min = chi2V;
      
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->SetBinContent(isigma+1,iE+1,chi2U);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->SetBinContent(isigma+1,iE+1,chi2V);
    }
  }
  
  double REv2[2];
  double Rsigmav2[2];
  REv2[0] = Rsigmav2[0] = +1.0e+20;
  REv2[1] = Rsigmav2[1] = -1.0e+20;
  
  double Chi2_Limit = 100.0;
  for(int isigma=0;isigma<Nbins_Sigma;isigma++) {
    for(int iE=0;iE<Nbins_Ebeam;iE++) {
      double chi2U = h_Chi2U_vs_SigmaSP_Ebeam_Tot->GetBinContent(isigma+1,iE+1) - chi2U_min;
      double chi2V = h_Chi2V_vs_SigmaSP_Ebeam_Tot->GetBinContent(isigma+1,iE+1) - chi2V_min;
      
      h_Chi2U_vs_SigmaSP_Ebeam_Tot->SetBinContent(isigma+1,iE+1,chi2U);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot->SetBinContent(isigma+1,iE+1,chi2V);
      
      if(chi2U > Chi2_Limit) h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->SetBinContent(isigma+1,iE+1,-999);
      else {
	double sigma = h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->GetBinCenter(isigma+1);
	double Eval  = h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->GetBinCenter(iE+1);
	if(REv2[0]     > Eval)  REv2[0]     = Eval;
	if(REv2[1]     < Eval)  REv2[1]     = Eval;
	if(Rsigmav2[0] > sigma) Rsigmav2[0] = sigma;
	if(Rsigmav2[1] < sigma) Rsigmav2[1] = sigma;
	h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->SetBinContent(isigma+1,iE+1,chi2U);
      }
      
      if(chi2V > Chi2_Limit) h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->SetBinContent(isigma+1,iE+1,-999);
      else {
	double sigma = h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetXaxis()->GetBinCenter(isigma+1);
	double Eval  = h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->GetYaxis()->GetBinCenter(iE+1);
	if(REv2[0]     > Eval)  REv2[0]     = Eval;
	if(REv2[1]     < Eval)  REv2[1]     = Eval;
	if(Rsigmav2[0] > sigma) Rsigmav2[0] = sigma;
	if(Rsigmav2[1] < sigma) Rsigmav2[1] = sigma;
	h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->SetBinContent(isigma+1,iE+1,chi2V);
      }
    }
  }
  
  double RangeFactorSigma = 100.0/100.0;
  double RangeFactorE     =   0.0/100.0;
  double delta_range;
  delta_range = Rsigmav2[1] - Rsigmav2[0];
  Rsigmav2[0] -= delta_range*RangeFactorSigma;
  Rsigmav2[1] += delta_range*RangeFactorSigma;
  delta_range = REv2[1] - REv2[0];
  REv2[0] -= delta_range*RangeFactorE;
  REv2[1] += delta_range*RangeFactorE;
  
  if(REv2[0]     < RE[0])     REv2[0]     = RE[0];
  if(REv2[1]     > RE[1])     REv2[1]     = RE[1];
  if(Rsigmav2[0] < Rsigma[0]) Rsigmav2[0] = Rsigma[0];
  if(Rsigmav2[1] > Rsigma[1]) Rsigmav2[1] = Rsigma[1];
  
  cout << endl;
  cout << "E-range        (v2) = (" << REv2[0]     << "," << REv2[1]     << ") GeV" << endl;
  cout << "sigma_sp-range (v2) = (" << Rsigmav2[0] << "," << Rsigmav2[1] << ") um"  << endl;
  cout << endl;
  
  int Nbins_SigmaV2 = 400;
  int Nbins_EbeamV2 = 400;
  //TH2D* h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom    = NULL;
  //TH2D* h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom    = NULL;
  TH2D* h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2 = NULL;
  TH2D* h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2 = NULL;
  
  HistName  = TString("h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom");
  HistTitle = TString("#chi^{2}_{tot}(U) vs #sigma_{sp}(U) and E_{beam} (Zoom)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom = new TH2D(HistName.Data(),
                                               HistTitle.Data(),
	  				       Nbins_SigmaV2,Rsigmav2[0],Rsigmav2[1],
					       Nbins_EbeamV2,REv2[0],REv2[1]);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->SetXTitle("#sigma_{sp}(U) (#mum)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->CenterTitle(true);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->SetYTitle("E_{beam} (GeV)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->CenterTitle(true);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->SetZTitle("#chi^{2}_{tot}(U)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetZaxis()->CenterTitle(true);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->SetTitleSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->SetLabelSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->SetTitleSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->SetLabelSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetZaxis()->SetTitleSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetZaxis()->SetLabelSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->SetStats(false);
      
  HistName  = TString("h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom");
  HistTitle = TString("#chi^{2}_{tot}(V) vs #sigma_{sp}(V) and E_{beam} (Zoom)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom = new TH2D(HistName.Data(),
                                               HistTitle.Data(),
					       Nbins_SigmaV2,Rsigmav2[0],Rsigmav2[1],
					       Nbins_EbeamV2,REv2[0],REv2[1]);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->SetXTitle("#sigma_{sp}(V) (#mum)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->CenterTitle(true);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->SetYTitle("E_{beam} (GeV)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->CenterTitle(true);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->SetZTitle("#chi^{2}_{tot}(V)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetZaxis()->CenterTitle(true);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->SetTitleSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->SetLabelSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->SetTitleSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->SetLabelSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetZaxis()->SetTitleSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetZaxis()->SetLabelSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->SetStats(false);
      
  HistName  = TString("h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2");
  HistTitle = TString("#chi^{2}_{tot}(U) vs #sigma_{sp}(U) and E_{beam} (Zoom)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2 = new TH2D(HistName.Data(),
						  HistTitle.Data(),
						  Nbins_SigmaV2,Rsigmav2[0],Rsigmav2[1],
						  Nbins_EbeamV2,REv2[0],REv2[1]);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetXTitle("#sigma_{sp}(U) (#mum)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetXaxis()->CenterTitle(true);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetYTitle("E_{beam} (GeV)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetYaxis()->CenterTitle(true);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetZTitle("#chi^{2}_{tot}(U)");
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetZaxis()->CenterTitle(true);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetXaxis()->SetTitleSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetXaxis()->SetLabelSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetYaxis()->SetTitleSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetYaxis()->SetLabelSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetZaxis()->SetTitleSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetZaxis()->SetLabelSize(TheSize);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetStats(false);
      
  HistName  = TString("h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2");
  HistTitle = TString("#chi^{2}_{tot}(V) vs #sigma_{sp}(V) and E_{beam} (Zoom)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2 = new TH2D(HistName.Data(),
						  HistTitle.Data(),
						  Nbins_SigmaV2,Rsigmav2[0],Rsigmav2[1],
						  Nbins_EbeamV2,REv2[0],REv2[1]);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetXTitle("#sigma_{sp}(V) (#mum)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetXaxis()->CenterTitle(true);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetYTitle("E_{beam} (GeV)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetYaxis()->CenterTitle(true);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetZTitle("#chi^{2}_{tot}(V)");
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetZaxis()->CenterTitle(true);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetXaxis()->SetTitleSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetXaxis()->SetLabelSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetYaxis()->SetTitleSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetYaxis()->SetLabelSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetZaxis()->SetTitleSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->GetZaxis()->SetLabelSize(TheSize);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetStats(false);
  
  
  chi2U_min = +1.0e+20;
  chi2V_min = +1.0e+20;
  double sigma_sp_min_U = -999.0;
  double sigma_sp_min_V = -999.0;
  double Ebeam_min_U = -999.0;
  double Ebeam_min_V = -999.0;
  for(int isigma=0;isigma<Nbins_SigmaV2;isigma++) {
    double sigma_sp = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetBinCenter(isigma+1);
    for(int iE=0;iE<Nbins_EbeamV2;iE++) {
      double Ebeam = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetBinCenter(iE+1);
      
      double chi2U = 0.0;
      double chi2V = 0.0;
      
      for(int i=0;i<N;i++) {
	double residueU = GetResidue(sigma_sp,Ebeam,h_ResidualAtDUT_Scan_U[i],_EnergyValues[i]);
	double residueV = GetResidue(sigma_sp,Ebeam,h_ResidualAtDUT_Scan_V[i],_EnergyValues[i]);
	chi2U += pow((residueU - _residueU_data[i])/_residueU_err_data[i],2);
	chi2V += pow((residueV - _residueV_data[i])/_residueV_err_data[i],2);
      }
      
      h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->SetBinContent(isigma+1,iE+1,chi2U);
      h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->SetBinContent(isigma+1,iE+1,chi2V);
      
      if(chi2U_min > chi2U) {
	chi2U_min = chi2U;
	sigma_sp_min_U = sigma_sp;
	Ebeam_min_U    = Ebeam;
      }
      if(chi2V_min > chi2V) {
	chi2V_min = chi2V;
	sigma_sp_min_V = sigma_sp;
	Ebeam_min_V    = Ebeam;
      }
    }
  }
  
  double REv3[2];
  double Rsigmav3[2];
  REv3[0] = Rsigmav3[0] = +1.0e+20;
  REv3[1] = Rsigmav3[1] = -1.0e+20;
  double Chi2_Limit_v3 = 25.0;
  Chi2_Limit = 9.0;
  for(int isigma=0;isigma<Nbins_SigmaV2;isigma++) {
    for(int iE=0;iE<Nbins_EbeamV2;iE++) {
      double chi2U = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetBinContent(isigma+1,iE+1) - chi2U_min;
      double chi2V = h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetBinContent(isigma+1,iE+1) - chi2V_min;
      
      if(chi2U > Chi2_Limit) h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetBinContent(isigma+1,iE+1,-999);
      else                   h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetBinContent(isigma+1,iE+1,chi2U);
      
      if(chi2V > Chi2_Limit) h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetBinContent(isigma+1,iE+1,-999);
      else                   h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetBinContent(isigma+1,iE+1,chi2V);

      if(chi2U < Chi2_Limit_v3) {
	double sigma = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetBinCenter(isigma+1);
	double Eval  = h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetBinCenter(iE+1);
	if(REv3[0]     > Eval)  REv3[0]     = Eval;
	if(REv3[1]     < Eval)  REv3[1]     = Eval;
	if(Rsigmav3[0] > sigma) Rsigmav3[0] = sigma;
	if(Rsigmav3[1] < sigma) Rsigmav3[1] = sigma;
      }
      if(chi2V < Chi2_Limit_v3) {
	double sigma = h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetBinCenter(isigma+1);
	double Eval  = h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetBinCenter(iE+1);
	if(REv3[0]     > Eval)  REv3[0]     = Eval;
	if(REv3[1]     < Eval)  REv3[1]     = Eval;
	if(Rsigmav3[0] > sigma) Rsigmav3[0] = sigma;
	if(Rsigmav3[1] < sigma) Rsigmav3[1] = sigma;
      }
    }
  }
  
  cout << endl;
  cout << "Chi2(U) min at (sigma_sp(U),Ebeam) = (" << sigma_sp_min_U << " um, " << Ebeam_min_U << " GeV)" << endl;
  cout << "Chi2(V) min at (sigma_sp(V),Ebeam) = (" << sigma_sp_min_V << " um, " << Ebeam_min_V << " GeV)" << endl;
  cout << "Average Ebeam @ min = " <<  0.5*(Ebeam_min_U + Ebeam_min_V) << " GeV" << endl;
  cout << endl;
  
  cout << endl;
  cout << "E-range        (v3) = (" << REv3[0]     << "," << REv3[1]     << ") GeV" << endl;
  cout << "sigma_sp-range (v3) = (" << Rsigmav3[0] << "," << Rsigmav3[1] << ") um"  << endl;
  cout << endl;

  TMinuit *gMinuit = new TMinuit(5);
  gMinuit->SetFCN(fcn);
  double arglist[10];
  int    ierflg = 0;
  
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR",arglist,1,ierflg);
  
  vstart[0] = sigma_sp_min_U;                   //sigma_sp_U
  vstart[1] = sigma_sp_min_V;                   //sigma_sp_V
  vstart[2] = 0.5*(Ebeam_min_U + Ebeam_min_V);  //Ebeam
  step[0]   = (Rsigmav3[1] - Rsigmav3[0])/1000.0;
  step[1]   = (Rsigmav3[1] - Rsigmav3[0])/1000.0;
  step[2]   = (REv3[1]     - REv3[0])    /1000.0;
  
  gMinuit->mnparm(0, "#sigma_{sp}(U)", vstart[0], step[0], Rsigmav3[0], Rsigmav3[1], ierflg);
  gMinuit->mnparm(1, "#sigma_{sp}(V)", vstart[1], step[1], Rsigmav3[0], Rsigmav3[1], ierflg);
  gMinuit->mnparm(2, "E_{beam}",       vstart[2], step[2], REv3[0],     REv3[1],     ierflg);
  
  //Now ready for minimization step
  arglist[0] = 100000;
  arglist[1] = 1.;
  gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
  gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
  gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
  gMinuit->mnexcm("HESSE",  arglist, 2, ierflg);

  //Print results
  double amin,edm,errdef;
  int nvpar,nparx,icstat;
  gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  
  double sigma_sp_U[2],sigma_sp_V[2], BeamEnergy[2];
  double correlation_matrix[3][3];
  gMinuit->GetParameter(0,sigma_sp_U[0],sigma_sp_U[1]);
  gMinuit->GetParameter(1,sigma_sp_V[0],sigma_sp_V[1]);
  gMinuit->GetParameter(2,BeamEnergy[0],BeamEnergy[1]);
  gMinuit->mnemat(&correlation_matrix[0][0],3);
  
  cout << endl;  
  cout << "  Minimum chi square                           = " << amin << endl;
  cout << "  Estimated vert. distance to min.             = " << edm << endl;
  cout << "  Number of variable parameters                = " << nvpar << endl;
  cout << "  Highest number of parameters defined by user = " << nparx << endl;
  cout << "  Status of covariance matrix                  = " << icstat << endl;
  cout << endl;
  cout << "  sigma_sp(U) = (" << sigma_sp_U[0] << " +/- " << sigma_sp_U[1] << ") um" << endl;
  cout << "  sigma_sp(V) = (" << sigma_sp_V[0] << " +/- " << sigma_sp_V[1] << ") um" << endl;
  cout << "  BeamEnergy  = (" << BeamEnergy[0] << " +/- " << BeamEnergy[1] << ") GeV" << endl;
  cout << "  corr. sigma_sp(U) vs sigma_sp(V) = " << correlation_matrix[0][1]/sqrt(correlation_matrix[0][0]*correlation_matrix[1][1])*100.0 << " %" << endl;
  cout << "  corr. sigma_sp(U) vs BeamEnergy  = " << correlation_matrix[0][2]/sqrt(correlation_matrix[0][0]*correlation_matrix[2][2])*100.0 << " %" << endl;
  cout << "  corr. sigma_sp(V) vs BeamEnergy  = " << correlation_matrix[1][2]/sqrt(correlation_matrix[1][1]*correlation_matrix[2][2])*100.0 << " %" << endl;
  cout << endl;
  
  
  const int Nvariables = 3;
  TString Variable_Names[Nvariables];
  Variable_Names[0] = TString("sigma_sp(U)");
  Variable_Names[1] = TString("sigma_sp(V)");
  Variable_Names[2] = TString("BeamEnergy");

  double Cvals[Nvariables];
  //double Evals[Nvariables];
  TMatrixTSym<double> _CovMatrix;
  _CovMatrix.ResizeTo(TMatrixTSym<double>(3));
  Cvals[0] = sigma_sp_U[0];
  //Evals[0] = sigma_sp_U[1];
  Cvals[1] = sigma_sp_V[0];
  //Evals[1] = sigma_sp_V[1];
  Cvals[2] = BeamEnergy[0];
  //Evals[2] = BeamEnergy[1];
  for(int i=0;i<Nvariables;i++) {
    for(int j=0;j<Nvariables;j++) {
      _CovMatrix(i,j) = correlation_matrix[i][j];
    }
  }
  cout << "Cov-matrix:" << endl;
  _CovMatrix.Print();

  TMatrixTSym<double> _AMatrix(_CovMatrix);
  _AMatrix.Invert();

  TMatrixDSymEigen _AMatrix_eigen(_AMatrix);
  TVectorD _Eigen_values      = _AMatrix_eigen.GetEigenValues();
  TMatrixD _Eigen_vectors     = _AMatrix_eigen.GetEigenVectors();
  TMatrixD _Eigen_vectors_Inv(_Eigen_vectors);
  _Eigen_vectors_Inv.Invert();
  cout << "Eigen-values:" << endl;
  _Eigen_values.Print();
  cout << "Eigen-vectors:" << endl;
  _Eigen_vectors.Print();
  cout << "Eigen-vectors(Inv):" << endl;
  _Eigen_vectors_Inv.Print();

  TMatrixTSym<double> _Tmp_unit;
  _Tmp_unit.ResizeTo(TMatrixTSym<double>(3));
  for(int i=0;i<Nvariables;i++) {
    for(int j=0;j<Nvariables;j++) {
      _Tmp_unit(i,j) = 0.0;
      for(int k=0;k<Nvariables;k++) {
	_Tmp_unit(i,j) += _Eigen_vectors_Inv(i,k)*_Eigen_vectors(k,j);
      }
    }
  }
  cout << "Unit-matrix:" << endl;
  _Tmp_unit.Print();

  TMatrixTSym<double> _Diag;
  _Diag.ResizeTo(TMatrixTSym<double>(3));
  for(int i=0;i<Nvariables;i++) {
    for(int j=0;j<Nvariables;j++) {
      _Diag(i,j) = 0.0;
      for(int k=0;k<Nvariables;k++) {
	for(int l=0;l<Nvariables;l++) {
	  _Diag(i,j) += _Eigen_vectors_Inv(i,k)*_AMatrix(k,l)*_Eigen_vectors(l,j);
	}
      }
    }
  }
  cout << "Diag-matrix:" << endl;
  _Diag.Print();

  double Cvals_p[Nvariables];
  double Evals_p[Nvariables];
  for(int i=0;i<Nvariables;i++) {
    Cvals_p[i] = 0.0;
    for(int k=0;k<Nvariables;k++) {
      Cvals_p[i] += _Eigen_vectors_Inv(i,k)*Cvals[k];
    }
    Evals_p[i] = 1.0/sqrt(_Diag(i,i));
  }
  for(int i=0;i<Nvariables;i++) {
    cout << Variable_Names[i] << "_prime = " 
	 << Cvals_p[i] << " +/- " << Evals_p[i]
	 << endl;
  }
  cout << endl;

  double Cvals_p_tmp[Nvariables];
  double Cvals_tmp[Nvariables];
  std::vector<double> _sigma_sp_U_values_p;
  std::vector<double> _sigma_sp_V_values_p;
  std::vector<double> _BeamEnergy_values_p;
  std::vector<double> _sigma_sp_U_values_m;
  std::vector<double> _sigma_sp_V_values_m;
  std::vector<double> _BeamEnergy_values_m;
  _sigma_sp_U_values_p.clear();
  _sigma_sp_V_values_p.clear();
  _BeamEnergy_values_p.clear();
  _sigma_sp_U_values_m.clear();
  _sigma_sp_V_values_m.clear();
  _BeamEnergy_values_m.clear();
  for(int i=0;i<Nvariables;i++) {
    for(int j=0;j<Nvariables;j++) {
      Cvals_p_tmp[j] = Cvals_p[j];
    }
    Cvals_p_tmp[i] = Cvals_p[i] + Evals_p[i];
    for(int j=0;j<Nvariables;j++) {
      Cvals_tmp[j] = 0.0;
      for(int k=0;k<Nvariables;k++) {
	Cvals_tmp[j] += _Eigen_vectors(j,k)*Cvals_p_tmp[k];
      }
    }
    _sigma_sp_U_values_p.push_back(Cvals_tmp[0]);
    _sigma_sp_V_values_p.push_back(Cvals_tmp[1]);
    _BeamEnergy_values_p.push_back(Cvals_tmp[2]);

    for(int j=0;j<Nvariables;j++) {
      Cvals_p_tmp[j] = Cvals_p[j];
    }
    Cvals_p_tmp[i] = Cvals_p[i] - Evals_p[i];
    for(int j=0;j<Nvariables;j++) {
      Cvals_tmp[j] = 0.0;
      for(int k=0;k<Nvariables;k++) {
	Cvals_tmp[j] += _Eigen_vectors(j,k)*Cvals_p_tmp[k];
      }
    }
    _sigma_sp_U_values_m.push_back(Cvals_tmp[0]);
    _sigma_sp_V_values_m.push_back(Cvals_tmp[1]);
    _BeamEnergy_values_m.push_back(Cvals_tmp[2]);
  }
  
  cout << Variable_Names[0].Data() << " = " << Cvals[0] << endl;
  for(int i=0;i<Nvariables;i++) {
    cout << "("
	 << _sigma_sp_U_values_m[i] << ","
	 << _sigma_sp_U_values_p[i] << ")"
	 << endl;
  }
  cout << endl;
  cout << Variable_Names[1].Data() << " = " << Cvals[1] << endl;
  for(int i=0;i<Nvariables;i++) {
    cout << "("
    	 << _sigma_sp_V_values_m[i] << ","
	 << _sigma_sp_V_values_p[i] << ")"
	 << endl;
  }
  cout << endl;
  cout << Variable_Names[2].Data() << " = " << Cvals[2] << endl;
  for(int i=0;i<Nvariables;i++) {
    cout << "("
	 << _BeamEnergy_values_m[i] << ","
	 << _BeamEnergy_values_p[i] << ")"
	 << endl;
  }
  cout << endl;

  for(int i=0;i<N;i++) {
    double v,e;
    double c,w;
    c = _PlLocation[i];
    w = 1.0e-8;
    double residueU,residueU_err;
    double residueV,residueV_err;
    
    const int Npar(_sigma_sp_U_values_m.size());
    double residueU_p[Npar],residueU_m[Npar],residueU_delta[Npar];
    double residueV_p[Npar],residueV_m[Npar],residueV_delta[Npar];
        
    residueU = GetResidue(sigma_sp_U[0],
		 	  BeamEnergy[0],
		          h_ResidualAtDUT_Scan_U[i],
			  _EnergyValues[i]);
    
    residueV = GetResidue(sigma_sp_V[0],
			  BeamEnergy[0],
		          h_ResidualAtDUT_Scan_V[i],
			  _EnergyValues[i]);
    
    residueU_err = residueV_err = 0.0;

    //propagate error on the fitted parameters to the predicted residues
    for(int k=0;k<Npar;k++) {
      residueU_p[k] = GetResidue(_sigma_sp_U_values_p[k],
				 _BeamEnergy_values_p[k],
				 h_ResidualAtDUT_Scan_U[i],
				 _EnergyValues[i]);
      residueU_p[k] = residueU_p[k] - residueU;
      residueU_m[k] = GetResidue(_sigma_sp_U_values_m[k],
				 _BeamEnergy_values_m[k],
				 h_ResidualAtDUT_Scan_U[i],
				 _EnergyValues[i]);
      residueU_m[k] = residueU - residueU_m[k];
      residueU_delta[k] = 0.5*(residueU_m[k] + residueU_p[k]);
      residueU_err += pow(residueU_delta[k],2);
      
      
      residueV_p[k] = GetResidue(_sigma_sp_V_values_p[k],
				 _BeamEnergy_values_p[k],
				 h_ResidualAtDUT_Scan_V[i],
				 _EnergyValues[i]);
      residueV_p[k] = residueV_p[k] - residueV;
      residueV_m[k] = GetResidue(_sigma_sp_V_values_m[k],
				 _BeamEnergy_values_m[k],
				 h_ResidualAtDUT_Scan_V[i],
				 _EnergyValues[i]);
      residueV_m[k] = residueV - residueV_m[k];
      residueV_delta[k] = 0.5*(residueV_m[k] + residueV_p[k]);
      residueV_err += pow(residueV_delta[k],2);
    }

    //Include systematic set by hand
    residueU_p[0] = GetResidue(sigma_sp_U[0] + Systematic,
			       BeamEnergy[0],
			       h_ResidualAtDUT_Scan_U[i],
			       _EnergyValues[i]);
    residueU_p[0] = residueU_p[0] - residueU;
    residueU_m[0] = GetResidue(sigma_sp_U[0] - Systematic,
			       BeamEnergy[0],
			       h_ResidualAtDUT_Scan_U[i],
			       _EnergyValues[i]);
    residueU_m[0] = residueU - residueU_m[0];
    residueU_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueU_err += pow(residueU_delta[0],2);

    residueV_p[0] = GetResidue(sigma_sp_V[0] + Systematic,
			       BeamEnergy[0],
			       h_ResidualAtDUT_Scan_V[i],
			       _EnergyValues[i]);
    residueV_p[0] = residueV_p[0] - residueV;
    residueV_m[0] = GetResidue(sigma_sp_V[0] - Systematic,
			       BeamEnergy[0],
			       h_ResidualAtDUT_Scan_V[i],
			       _EnergyValues[i]);
    residueV_m[0] = residueV - residueV_m[0];
    residueV_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueV_err += pow(residueU_delta[0],2);
    
    //Include systematic on Ebeam
    residueU_p[0] = GetResidue(sigma_sp_U[0],
			       BeamEnergy[0] + Systematic_E,
			       h_ResidualAtDUT_Scan_U[i],
			       _EnergyValues[i]);
    residueU_p[0] = residueU_p[0] - residueU;
    residueU_m[0] = GetResidue(sigma_sp_U[0],
			       BeamEnergy[0] - Systematic_E,
			       h_ResidualAtDUT_Scan_U[i],
			       _EnergyValues[i]);
    residueU_m[0] = residueU - residueU_m[0];
    residueU_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueU_err += pow(residueU_delta[0],2);

    residueV_p[0] = GetResidue(sigma_sp_V[0],
			       BeamEnergy[0] + Systematic_E,
			       h_ResidualAtDUT_Scan_V[i],
			       _EnergyValues[i]);
    residueV_p[0] = residueV_p[0] - residueV;
    residueV_m[0] = GetResidue(sigma_sp_V[0],
			       BeamEnergy[0] - Systematic_E,
			       h_ResidualAtDUT_Scan_V[i],
			       _EnergyValues[i]);
    residueV_m[0] = residueV - residueV_m[0];
    residueV_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueV_err += pow(residueU_delta[0],2);

    residueU_err = sqrt(residueU_err);
    residueV_err = sqrt(residueV_err);
    
    v = residueU;
    e = 1.0e-6;
    gresidueU_G4_Global.SetPoint(i,c,v);
    gresidueU_G4_Global.SetPointError(i,w,e);

    v = residueU + residueU_err;
    e = 1.0e-6;
    gresidueU_G4_Global_max.SetPoint(i,c,v);
    gresidueU_G4_Global_max.SetPointError(i,w,e);
    v = residueU - residueU_err;
    e = 1.0e-6;
    gresidueU_G4_Global_min.SetPoint(i,c,v);
    gresidueU_G4_Global_min.SetPointError(i,w,e);

    v = residueV;
    e = 1.0e-6;
    gresidueV_G4_Global.SetPoint(i,c,v);
    gresidueV_G4_Global.SetPointError(i,w,e);
    v = residueV + residueV_err;
    e = 1.0e-6;
    gresidueV_G4_Global_max.SetPoint(i,c,v);
    gresidueV_G4_Global_max.SetPointError(i,w,e);
    v = residueV - residueV_err;
    e = 1.0e-6;
    gresidueV_G4_Global_min.SetPoint(i,c,v);
    gresidueV_G4_Global_min.SetPointError(i,w,e);
  }

  for(int i=0;i<N;i++) {
    double v,e;
    double c,w;
    c = _PlLocation[i];
    w = 1.0e-8;
    //double residue,residue_err;
    
    gresidueU_G4_Global_max.GetPoint(i,c,v);
    e = 1.0e-6;
    gresidueU_G4_Global_Shade.SetPoint(i,c,v);
    gresidueU_G4_Global_Shade.SetPointError(i,w,e);
    gresidueU_G4_Global_min.GetPoint(N - i - 1,c,v);
    e = 1.0e-6;
    gresidueU_G4_Global_Shade.SetPoint(N+i,c,v);
    gresidueU_G4_Global_Shade.SetPointError(N+i,w,e);
    
    gresidueV_G4_Global_max.GetPoint(i,c,v);
    e = 1.0e-6;
    gresidueV_G4_Global_Shade.SetPoint(i,c,v);
    gresidueV_G4_Global_Shade.SetPointError(i,w,e);
    gresidueV_G4_Global_min.GetPoint(N - i - 1,c,v);
    e = 1.0e-6;
    gresidueV_G4_Global_Shade.SetPoint(N+i,c,v);
    gresidueV_G4_Global_Shade.SetPointError(N+i,w,e);
  }


  for(int i=0;i<N;i++) {
    double v,e;
    double c,w;
    c = _PlLocation[i];
    w = 1.0e-8;
    double residueU,residueU_err;
    double residueV,residueV_err;
    
    const int Npar(_sigma_sp_U_values_m.size());
    double residueU_p[Npar],residueU_m[Npar],residueU_delta[Npar];
    double residueV_p[Npar],residueV_m[Npar],residueV_delta[Npar];
        
    residueU = GetResidue(sigma_sp_U[0],
		 	  BeamEnergy[0],
		          h_TelescopeResolution_Scan_U[i],
			  _EnergyValues[i]);
    
    residueV = GetResidue(sigma_sp_V[0],
			  BeamEnergy[0],
		          h_TelescopeResolution_Scan_V[i],
			  _EnergyValues[i]);
    
    residueU_err = residueV_err = 0.0;

    //propagate error on the fitted parameters to the predicted residues
    for(int k=0;k<Npar;k++) {
      residueU_p[k] = GetResidue(_sigma_sp_U_values_p[k],
				 _BeamEnergy_values_p[k],
				 h_TelescopeResolution_Scan_U[i],
				 _EnergyValues[i]);
      residueU_p[k] = residueU_p[k] - residueU;
      residueU_m[k] = GetResidue(_sigma_sp_U_values_m[k],
				 _BeamEnergy_values_m[k],
				 h_TelescopeResolution_Scan_U[i],
				 _EnergyValues[i]);
      residueU_m[k] = residueU - residueU_m[k];
      residueU_delta[k] = 0.5*(residueU_m[k] + residueU_p[k]);
      residueU_err += pow(residueU_delta[k],2);
      
      
      residueV_p[k] = GetResidue(_sigma_sp_V_values_p[k],
				 _BeamEnergy_values_p[k],
				 h_TelescopeResolution_Scan_V[i],
				 _EnergyValues[i]);
      residueV_p[k] = residueV_p[k] - residueV;
      residueV_m[k] = GetResidue(_sigma_sp_V_values_m[k],
				 _BeamEnergy_values_m[k],
				 h_TelescopeResolution_Scan_V[i],
				 _EnergyValues[i]);
      residueV_m[k] = residueV - residueV_m[k];
      residueV_delta[k] = 0.5*(residueV_m[k] + residueV_p[k]);
      residueV_err += pow(residueV_delta[k],2);
    }

    //Include systematic set by hand
    residueU_p[0] = GetResidue(sigma_sp_U[0] + Systematic,
			       BeamEnergy[0],
			       h_TelescopeResolution_Scan_U[i],
			       _EnergyValues[i]);
    residueU_p[0] = residueU_p[0] - residueU;
    residueU_m[0] = GetResidue(sigma_sp_U[0] - Systematic,
			       BeamEnergy[0],
			       h_TelescopeResolution_Scan_U[i],
			       _EnergyValues[i]);
    residueU_m[0] = residueU - residueU_m[0];
    residueU_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueU_err += pow(residueU_delta[0],2);

    residueV_p[0] = GetResidue(sigma_sp_V[0] + Systematic,
			       BeamEnergy[0],
			       h_TelescopeResolution_Scan_V[i],
			       _EnergyValues[i]);
    residueV_p[0] = residueV_p[0] - residueV;
    residueV_m[0] = GetResidue(sigma_sp_V[0] - Systematic,
			       BeamEnergy[0],
			       h_TelescopeResolution_Scan_V[i],
			       _EnergyValues[i]);
    residueV_m[0] = residueV - residueV_m[0];
    residueV_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueV_err += pow(residueU_delta[0],2);
    
    //Include systematic on Ebeam
    residueU_p[0] = GetResidue(sigma_sp_U[0],
			       BeamEnergy[0] + Systematic_E,
			       h_TelescopeResolution_Scan_U[i],
			       _EnergyValues[i]);
    residueU_p[0] = residueU_p[0] - residueU;
    residueU_m[0] = GetResidue(sigma_sp_U[0],
			       BeamEnergy[0] - Systematic_E,
			       h_TelescopeResolution_Scan_U[i],
			       _EnergyValues[i]);
    residueU_m[0] = residueU - residueU_m[0];
    residueU_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueU_err += pow(residueU_delta[0],2);

    residueV_p[0] = GetResidue(sigma_sp_V[0],
			       BeamEnergy[0] + Systematic_E,
			       h_TelescopeResolution_Scan_V[i],
			       _EnergyValues[i]);
    residueV_p[0] = residueV_p[0] - residueV;
    residueV_m[0] = GetResidue(sigma_sp_V[0],
			       BeamEnergy[0] - Systematic_E,
			       h_TelescopeResolution_Scan_V[i],
			       _EnergyValues[i]);
    residueV_m[0] = residueV - residueV_m[0];
    residueV_delta[0] = 0.5*(residueU_m[0] + residueU_p[0]);
    residueV_err += pow(residueU_delta[0],2);

    residueU_err = sqrt(residueU_err);
    residueV_err = sqrt(residueV_err);
    
    v = residueU;
    e = 1.0e-6;
    gTelresoU_G4_Global.SetPoint(i,c,v);
    gTelresoU_G4_Global.SetPointError(i,w,e);
    v = residueU + residueU_err;
    e = 1.0e-6;
    gTelresoU_G4_Global_max.SetPoint(i,c,v);
    gTelresoU_G4_Global_max.SetPointError(i,w,e);
    v = residueU - residueU_err;
    e = 1.0e-6;
    gTelresoU_G4_Global_min.SetPoint(i,c,v);
    gTelresoU_G4_Global_min.SetPointError(i,w,e);

    v = residueV;
    e = 1.0e-6;
    gTelresoV_G4_Global.SetPoint(i,c,v);
    gTelresoV_G4_Global.SetPointError(i,w,e);
    v = residueV + residueV_err;
    e = 1.0e-6;
    gTelresoV_G4_Global_max.SetPoint(i,c,v);
    gTelresoV_G4_Global_max.SetPointError(i,w,e);
    v = residueV - residueV_err;
    e = 1.0e-6;
    gTelresoV_G4_Global_min.SetPoint(i,c,v);
    gTelresoV_G4_Global_min.SetPointError(i,w,e);
  }

  for(int i=0;i<N;i++) {
    double v,e;
    double c,w;
    c = _PlLocation[i];
    w = 1.0e-8;
    //double residue,residue_err;
    
    gTelresoU_G4_Global_max.GetPoint(i,c,v);
    e = 1.0e-6;
    gTelresoU_G4_Global_Shade.SetPoint(i,c,v);
    gTelresoU_G4_Global_Shade.SetPointError(i,w,e);
    gTelresoU_G4_Global_min.GetPoint(N - i - 1,c,v);
    e = 1.0e-6;
    gTelresoU_G4_Global_Shade.SetPoint(N+i,c,v);
    gTelresoU_G4_Global_Shade.SetPointError(N+i,w,e);
    
    gTelresoV_G4_Global_max.GetPoint(i,c,v);
    e = 1.0e-6;
    gTelresoV_G4_Global_Shade.SetPoint(i,c,v);
    gTelresoV_G4_Global_Shade.SetPointError(i,w,e);
    gTelresoV_G4_Global_min.GetPoint(N - i - 1,c,v);
    e = 1.0e-6;
    gTelresoV_G4_Global_Shade.SetPoint(N+i,c,v);
    gTelresoV_G4_Global_Shade.SetPointError(N+i,w,e);
  }

  
  double porcent = 0.10;
  double Maximum;
  double Minimum;
  TString EPSName;
  EPSName = TString(file_out) + TString(".eps");
  TString EPSNameO = EPSName + TString("[");
  TString EPSNameC = EPSName + TString("]");

  TCanvas* cSummary = new TCanvas("cSummary","cSummary");
  cSummary->SetFillColor(10);
  cSummary->SetFrameFillColor(10);
  cSummary->SetTickx(1);
  //cSummary->SetTicky(1);
  cSummary->SetGridx(1);
  cSummary->SetGridy(1);
  cSummary->SetRightMargin(0.15);

  cSummary->Print(EPSNameO.Data());
  
  for(int i=0;i<N;i++) {
    cSummary->Clear();
    cSummary->Divide(2,1);
    cSummary->cd(1);
    gPad->SetFillColor(10);
    gPad->SetFrameFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    huCGtu1[i]->Draw();
    cSummary->cd(2);
    gPad->SetFillColor(10);
    gPad->SetFrameFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    hvCGtu1[i]->Draw();
    cSummary->Print(EPSName.Data());
  }
  
  cSummary->Clear();
  Maximum = -1.0e+20;
  Minimum = +1.0e+20;
  for(int i=0;i<N;i++) {
    double v,e,c;
    gresidueU_data.GetPoint(i,c,v);
    e = gresidueU_data.GetErrorY(i);
    if(Maximum < v+e) Maximum = v+e;
    if(Minimum > v-e) Minimum = v-e;
    
    gresidueV_data.GetPoint(i,c,v);
    e = gresidueV_data.GetErrorY(i);
    if(Maximum < v+e) Maximum = v+e;
    if(Minimum > v-e) Minimum = v-e;
  }
  porcent = 0.10;
  h_REF.SetMaximum(Maximum + porcent*(Maximum-Minimum));
  h_REF.SetMinimum(Minimum - porcent*(Maximum-Minimum));
  h_REF.Draw();
  gresidueU_data.Draw("PEL");
  gresidueV_data.Draw("PEL");
  cSummary->Print(EPSName.Data());

  for(int i=0;i<N;i++) {
    cSummary->Clear();
    cSummary->Divide(2,2);
    cSummary->cd(1);
    gPad->SetFillColor(10);
    gPad->SetFrameFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.2);
    h_residueU_vs_SigmaSP_Ebeam[i]->Draw("colz");
    cSummary->cd(2);
    gPad->SetFillColor(10);
    gPad->SetFrameFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.2);
    h_residueV_vs_SigmaSP_Ebeam[i]->Draw("colz");
    cSummary->cd(3);
    gPad->SetFillColor(10);
    gPad->SetFrameFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.2);
    h_Chi2U_vs_SigmaSP_Ebeam[i]->Draw("colz");
    cSummary->cd(4);
    gPad->SetFillColor(10);
    gPad->SetFrameFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.2);
    h_Chi2V_vs_SigmaSP_Ebeam[i]->Draw("colz");
    cSummary->Print(EPSName.Data());
  }
  
  cSummary->Clear();
  cSummary->Divide(2,2);
  cSummary->cd(1);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot->Draw("colz");
  cSummary->cd(2);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot->Draw("colz");
  cSummary->cd(3);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->SetMinimum(0.0);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_V2->Draw("colz");
  cSummary->cd(4);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->SetMinimum(0.0);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_V2->Draw("colz");
  cSummary->Print(EPSName.Data());
  
  TLine* l_min_location_U_h = new TLine(h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin(),Ebeam_min_U,
                                        sigma_sp_min_U,                                          Ebeam_min_U);
  l_min_location_U_h->SetLineColor(1);
  l_min_location_U_h->SetLineWidth(2);
  l_min_location_U_h->SetLineStyle(2);
  TLine* l_min_location_U_v = new TLine(sigma_sp_min_U,h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmin(),
                                        sigma_sp_min_U,Ebeam_min_U);
  l_min_location_U_v->SetLineColor(1);
  l_min_location_U_v->SetLineWidth(2);
  l_min_location_U_v->SetLineStyle(2);
  
  TLine* l_min_location_V_h = new TLine(h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin(),Ebeam_min_V,
                                        sigma_sp_min_V,                                          Ebeam_min_V);
  l_min_location_V_h->SetLineColor(1);
  l_min_location_V_h->SetLineWidth(2);
  l_min_location_V_h->SetLineStyle(2);
  TLine* l_min_location_V_v = new TLine(sigma_sp_min_V,h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmin(),
                                        sigma_sp_min_V,Ebeam_min_V);
  l_min_location_V_v->SetLineColor(1);
  l_min_location_V_v->SetLineWidth(2);
  l_min_location_V_v->SetLineStyle(2);
  
  TLine* l_min_locationFit_U_h = new TLine(h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin(),BeamEnergy[0],
                                           sigma_sp_U[0],                                           BeamEnergy[0]);
  l_min_locationFit_U_h->SetLineColor(2);
  l_min_locationFit_U_h->SetLineWidth(2);
  l_min_locationFit_U_h->SetLineStyle(2);
  TLine* l_min_locationFit_U_v = new TLine(sigma_sp_U[0],h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmin(),
                                           sigma_sp_U[0],BeamEnergy[0]);
  l_min_locationFit_U_v->SetLineColor(2);
  l_min_locationFit_U_v->SetLineWidth(2);
  l_min_locationFit_U_v->SetLineStyle(2);
  
  TLine* l_min_locationFit_V_h = new TLine(h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetXaxis()->GetXmin(),BeamEnergy[0],
                                           sigma_sp_V[0],                                           BeamEnergy[0]);
  l_min_locationFit_V_h->SetLineColor(2);
  l_min_locationFit_V_h->SetLineWidth(2);
  l_min_locationFit_V_h->SetLineStyle(2);
  TLine* l_min_locationFit_V_v = new TLine(sigma_sp_V[0],h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->GetYaxis()->GetXmin(),
                                           sigma_sp_V[0],BeamEnergy[0]);
  l_min_locationFit_V_v->SetLineColor(2);
  l_min_locationFit_V_v->SetLineWidth(2);
  l_min_locationFit_V_v->SetLineStyle(2);

  
  cSummary->Clear();
  cSummary->Divide(2,2);
  cSummary->cd(1);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom->Draw("colz");
  cSummary->cd(2);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom->Draw("colz");
  cSummary->cd(3);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetMinimum(0.0);
  h_Chi2U_vs_SigmaSP_Ebeam_Tot_Zoom_V2->Draw("colz");
  l_min_location_U_h->Draw();
  l_min_location_U_v->Draw();
  l_min_locationFit_U_h->Draw();
  l_min_locationFit_U_v->Draw();
  cSummary->cd(4);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetRightMargin(0.2);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->SetMinimum(0.0);
  h_Chi2V_vs_SigmaSP_Ebeam_Tot_Zoom_V2->Draw("colz");
  l_min_location_V_h->Draw();
  l_min_location_V_v->Draw();
  l_min_locationFit_V_h->Draw();
  l_min_locationFit_V_v->Draw();
  cSummary->Print(EPSName.Data());

  Maximum = -1.0e+20;
  Minimum = +1.0e+20;
  for(int i=0;i<N;i++) {
    double v,e,c;
    gresidueU_data.GetPoint(i,c,v);
    e = gresidueU_data.GetErrorY(i);
    if(Maximum < v+e) Maximum = v+e;
    if(Minimum > v-e) Minimum = v-e;
    
    gresidueV_data.GetPoint(i,c,v);
    e = gresidueV_data.GetErrorY(i);
    if(Maximum < v+e) Maximum = v+e;
    if(Minimum > v-e) Minimum = v-e;

    gresidueU_G4_Global_max.GetPoint(i,c,v);
    e = 1.0e-6;
    if(Maximum < v+e) Maximum = v+e;
    gresidueU_G4_Global_min.GetPoint(i,c,v);
    e = 1.0e-6;
    if(Minimum > v-e) Minimum = v-e;
    
    gresidueV_G4_Global_max.GetPoint(i,c,v);
    e = 1.0e-6;
    if(Maximum < v+e) Maximum = v+e;
    gresidueV_G4_Global_min.GetPoint(i,c,v);
    e = 1.0e-6;
    if(Minimum > v-e) Minimum = v-e;
  }
  porcent = 0.10;
  h_REF.SetMaximum(Maximum + porcent*(Maximum-Minimum));
  h_REF.SetMinimum(Minimum - porcent*(Maximum-Minimum));

  double X,Y;
  
  cSummary->Clear();
  h_REF.SetYTitle("#sigma_{residue}(U) (#mum)");
  h_REF.Draw();
  gresidueU_data.Draw("PEL");
  
  gresidueU_G4_Global_Shade.Draw("f");
  gresidueU_G4_Global_max.Draw("l");
  gresidueU_G4_Global_min.Draw("l");
  gresidueU_G4_Global.Draw("PEL");
  cSummary->Print(EPSName.Data());
  
  cSummary->Clear();
  h_REF.Draw();
  gresidueU_data.Draw("PEL");
  
  gresidueU_G4_Global_Shade.Draw("f");
  gresidueU_G4_Global_max.Draw("l");
  gresidueU_G4_Global_min.Draw("l");
  gresidueU_G4_Global.Draw("PEL");
  
  porcent = 0.25;
  X = h_REF.GetXaxis()->GetXmin() + porcent*(h_REF.GetXaxis()->GetXmax() - h_REF.GetXaxis()->GetXmin());
  porcent = 0.10;
  Y = h_REF.GetMaximum() - porcent*(h_REF.GetMaximum() - h_REF.GetMinimum());
  latex->SetTextSize(0.035);
  latex->SetTextColor(color_mcG4);
  HistName  = TString("#sigma_{sp}(U) = (");
  sprintf(ccc1,"%.2f",sigma_sp_U[0]);
  HistName += TString(ccc1) + TString(" #pm ");
  sprintf(ccc1,"%.2f",sigma_sp_U[1]);
  HistName += TString(ccc1) + TString(" #pm ");
  sprintf(ccc1,"%.2f",Systematic);
  HistName += TString(ccc1) + TString(") #mum");
  latex->DrawLatex(X,Y,HistName.Data());
  
  porcent = 0.25;
  X = h_REF.GetXaxis()->GetXmin() + porcent*(h_REF.GetXaxis()->GetXmax() - h_REF.GetXaxis()->GetXmin());
  porcent = 0.15;
  Y = h_REF.GetMaximum() - porcent*(h_REF.GetMaximum() - h_REF.GetMinimum());
  latex->SetTextSize(0.035);
  latex->SetTextColor(color_mcG4);
  HistName  = TString("#sigma_{sp}(V) = (");
  sprintf(ccc1,"%.2f",sigma_sp_V[0]);
  HistName += TString(ccc1) + TString(" #pm ");
  sprintf(ccc1,"%.2f",sigma_sp_V[1]);
  HistName += TString(ccc1) + TString(" #pm ");
  sprintf(ccc1,"%.2f",Systematic);
  HistName += TString(ccc1) + TString(") #mum");
  latex->DrawLatex(X,Y,HistName.Data());
  
  porcent = 0.25;
  X = h_REF.GetXaxis()->GetXmin() + porcent*(h_REF.GetXaxis()->GetXmax() - h_REF.GetXaxis()->GetXmin());
  porcent = 0.20;
  Y = h_REF.GetMaximum() - porcent*(h_REF.GetMaximum() - h_REF.GetMinimum());
  latex->SetTextSize(0.035);
  latex->SetTextColor(color_mcG4);
  HistName  = TString("E_{beam} = (");
  sprintf(ccc1,"%.2f",BeamEnergy[0]);
  HistName += TString(ccc1) + TString(" #pm ");
  sprintf(ccc1,"%.2f",BeamEnergy[1]);
  HistName += TString(ccc1) + TString(" #pm ");
  sprintf(ccc1,"%.2f",Systematic_E);
  HistName += TString(ccc1) + TString(") GeV");
  latex->DrawLatex(X,Y,HistName.Data());
  cSummary->Print(EPSName.Data());
  
  cSummary->Clear();
  h_REF.SetYTitle("#sigma_{residue}(V) (#mum)");
  h_REF.Draw();
  gresidueV_data.Draw("PEL");
  
  gresidueV_G4_Global_Shade.Draw("f");
  gresidueV_G4_Global_max.Draw("l");
  gresidueV_G4_Global_min.Draw("l");
  gresidueV_G4_Global.Draw("PEL");
  cSummary->Print(EPSName.Data());

  Maximum = -1.0e+20;
  Minimum = +1.0e+20;
  for(int i=0;i<N;i++) {
    double v,c;
    gTelresoU_G4_Global_max.GetPoint(i,c,v);
    if(Maximum < v) Maximum = v;
    gTelresoU_G4_Global_min.GetPoint(i,c,v);
    if(Minimum > v) Minimum = v;
    
    gTelresoV_G4_Global_max.GetPoint(i,c,v);
    if(Maximum < v) Maximum = v;
    gTelresoV_G4_Global_min.GetPoint(i,c,v);
    if(Minimum > v) Minimum = v;
  }
  porcent = 0.10;
  h_REF.SetMaximum(Maximum + porcent*(Maximum-Minimum));
  h_REF.SetMinimum(Minimum - porcent*(Maximum-Minimum));
  
  cSummary->Clear();
  h_REF.SetYTitle("#sigma_{Tel}(U) (#mum)");
  h_REF.Draw();
  gTelresoU_G4_Global_Shade.Draw("f");
  gTelresoU_G4_Global_max.Draw("l");
  gTelresoU_G4_Global_min.Draw("l");
  gTelresoU_G4_Global.Draw("PEL");
  cSummary->Print(EPSName.Data());
  
  cSummary->Clear();
  h_REF.SetYTitle("#sigma_{Tel}(V) (#mum)");
  h_REF.Draw();
  gTelresoV_G4_Global_Shade.Draw("f");
  gTelresoV_G4_Global_max.Draw("l");
  gTelresoV_G4_Global_min.Draw("l");
  gTelresoV_G4_Global.Draw("PEL");
  cSummary->Print(EPSName.Data());

  cSummary->Print(EPSNameC.Data());

  //Creating a pdf file from the eps file
  TString PDFName = TString(file_out) + TString(".pdf");
  command = TString("ps2pdf ") + EPSName + TString("  ") + PDFName;
  gSystem->Exec(command.Data());
  command = TString("rm  ") + EPSName;
  gSystem->Exec(command.Data());
  command = TString("ls -tlr  ") + PDFName;
  gSystem->Exec(command.Data());

  
  return;

  
}
//=========================================================================================================


