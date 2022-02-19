// @(#)maf/maf:$Name:  $:$Id: MPost.cxx v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 
// Last Modified: JB 2009/09/07 for User's plots
// Last Modified: JB 2009/09/10 for Resolution plots
// Last Modified: JB 2009/09/15 for Resolution plots
// Last Modified: JB 2010/07/22 for new Resolution plots
// Last Modified: JB 2010/07/27 for new FakeRate plots & PreparePost
// Last Modified: JB 2010/07/27 for new Calibration plots
// Last Modified: JB 2010/10/06 for new HitMap plots
// Last Modified: JB 2011/07/01 FakeRate, StoreHitMap
// Last modified: JB 2011/07/07 to localize path names
// Last Modified: MG 2011/07/08 InspectEfficiency
// Last Modified: JB 2011/07/21 introduce fSession member pointer
// Last Modified: SS 2011/12/05 Some results for the resolution are saved to the CSV file.
// Last Modified: JB 2011/07/21 MimosaClusterCharge Checkreferencetracks
// Last Modified: SS 2013/06/22 Correction of trackLimit print (%d -> %.0f)
// Last Modified: JB 2013/07/16 CheckMimosaAlign
// Last Modified: JB 2013/09/07 ClusterShape, ClusterCharge
// Last Modified: JB 2013/09/17 MimosaClusterCharge
// Last Modified: JB 2013/10/29 MimosaClusterCharge
// Last Modified: JB 2013/11/08 CheckClusters
// Last Modified: JB 2013/11/13 MimosaBinaryplots
// Last Modified: JB 2014/05/22 CheckClusters
// Last Modified: JH 2014/06/16 CheckImaging
// Last Modified: AP 2015/03/16 Added ChickMCSimulation function
//                              Used to display plots of MC simulation for evaluating telescope resolution
// Last Modified: BB 2015/12/17 MiniVectors: change Char_t Header[100] and sprintf to stringstream in a way to avoid segmentation fault

/////////////////////////////////////////////////////////////
//                                                         //
//  Contains all methods to display analysis results.      
//                                                         
//  Those are accessible at the end of the main analysis methods,
//   through a graphical menu displayed by a call to PreparePost().
//
////////////////////////////////////////////////////////////

#include "MAnalysis.h"
#ifdef UseROOFIT
  #include "DXRay2DPdf.h"
  #include "RooArgSet.h"
  #include "RooRealVar.h"
  #include "RooDataSet.h"
  #include "RooArgSet.h"
  #include "RooGenericPdf.h"
  #include "RooProdPdf.h"
  #include "RooUniform.h"
  #include "RooAddPdf.h"
  #include "RooFitResult.h"
  #include "RooPlot.h"
  #include "RooHist.h"
  #include "RooRealConstant.h"
#endif
#include "TVector2.h"
#include "TEllipse.h"

ClassImp(MimosaAnalysis) 


//_______________________________________________________________________________________
//
void   MimosaAnalysis::PreparePost()
{

  // Menu with all methods to plot results
  // Shall be call after an "Analysis" method
  //
  // Modified: JB 2010/07/26 to allow call from several analysis method
  // Modified: JB 2013/05/01 condition for MimosaPro2Planes added
  // Modified: AP 2015/03/09 adding summary pdf file with metadata and plots

  if( !fMimosaProDone && !fMimosaFakerateDone && !fMimosaCalibrationDone && !fMimosaMiniVectorsDone && !fMimosaPro2PlanesDone && !fMimosaImagingDone ) return;
  
  // Flag that the general analysis on Mimosa is done
  fMimosaDone = kTRUE; // JB 2010/09/10

 if (!gROOT->IsBatch())
   {
     bar2  = new TControlBar("vertical","Display");
     // Menu whatever the analysis was 
     bar2->AddButton("Cluster charge (Landau)","gTAF->MimosaClusterCharge()", "Main distribution of cluster charge & SNR");
     bar2->AddButton("Cluster charge (Gauss)","gTAF->MimosaClusterCharge(\"gaus\")", "Main distribution of cluster charge & SNR with gaussians");
     bar2->AddButton("Binary output Plots","gTAF->MimosaBinaryplots()","Plot for binary readout");
     bar2->AddButton("Check asymmetry","gTAF->CheckAsymmetry()"," ");
     bar2->AddButton("baseline","gTAF->CompareBadGoodRaw()","compare the baseline of good events and bad events");
     bar2->AddButton("Charge in each pixel","gTAF->pixelcharge()","Charge distributions in each pixel ordered by charge");
     bar2->AddButton("Signal/Noise in each pixel","gTAF->MimosaSN()","S/N distribution in each pixel ordered by charge");
     bar2->AddButton("Cluster shape","gTAF->ClusterShape()","Study cluster shape and multiplicity");
     bar2->AddButton("Hit map","gTAF->HitMap()","Display map of hits");
     if( fIfFake ) {
       bar2->AddButton("Optimize fake rate","gTAF->MimosaOptimize()","Optimize cuts to minimize fake hit rate");
       bar2->AddButton("Fake Rate","gTAF->FakeRate()","Plot experimental fake rate");
     }
     if( !fMimosaProDone && !fMimosaPro2PlanesDone ) bar2->AddButton("Check Clusters","gTAF->CheckClusters()","Selected cluster properties");
     
     // Menu when tracking (mimosapro)
     if( fMimosaProDone || fMimosaPro2PlanesDone) {
       bar2->AddButton("Resolution","gTAF->MimosaResolution()", "...  >>Resolution<<");
       bar2->AddButton("Check alignment","gTAF->CheckMimosaAlign()"," ");
       bar2->AddButton("Check eff in the run","gTAF->InspectEfficiency()","check how evolves eff during the run");
       bar2->AddButton("Check Ref.tracks","gTAF->Checkreferencetracks()","Track and cluster properties");
       bar2->AddButton("Check Telescope","gTAF->checkRefPlane()","check Telescope properties.");
       bar2->AddButton("Pixel response homogeneity","gTAF->MimosaPixelHomogeneity()",
		       "Check pixel homogeneity depending on real track position");
       if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
	 bar2->AddButton("Check MC simulation","gTAF->CheckMCSimulation()",
			 "Check MC simulation for evaluating telescope resolution");
       }
     }

     // Menu when calibration
     else if( fMimosaCalibrationDone ) {
       bar2->AddButton("Calibration","gTAF->Calibration()","Fit the calibration peak.");
     }

     // Menu when minivectors
     else if( fMimosaMiniVectorsDone ) {
       bar2->AddButton("Check eff in the run","gTAF->InspectEfficiency()","check how evolves eff during the run");
       bar2->AddButton("MiniVectors","gTAF->MiniVectors()","Track to vector comparison.");
     }
     
     // Again generic end menu

     bar2->AddButton("User's analysis","gTAF->UserAnalysis()","Perform user's analysis");
     bar2->AddButton("SAVE GIF/EPS files","gTAF->SaveGifFile()","Save in GIF and EPS files, in Results/#/ directory");
     bar2->AddButton("Clear / Close","gTAF->Clear()","Clear everything to start over");
     bar2->Show();

   }
  
 for(Int_t iu=0;iu<50;iu++){ UsedMacro[iu]= 0; }
 
 gStyle->SetOptStat(1);
 gStyle->SetOptFit(1);
 gStyle->SetPalette(1); // JB 2010/07/26
 gROOT->SaveContext();
 
 gStyle->SetPalette(1);
 gStyle->SetLabelSize(0.06); // JB 2011/11/01
 gStyle->SetTitleSize(0.06);
 gStyle->SetPadLeftMargin(0.15);
 gStyle->SetTitleH(.085);

 MHist::Dir();

}

//_______________________________________________________________________________________
//
int MimosaAnalysis::ResultsStrore()
{  
  if(!CheckIfDone("mimosall") ) return 0;
  
  // collect and store all important results
  
  cout<<"  ==> Storing Results ..."<<endl;
  
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  
  char fOutName[50];
  sprintf(fOutName,"Results_%d_%d_%d.root",fSession->GetRunNumber(),fSession->GetPlaneNumber(),ThesubmatrixNumber);
  sprintf(fOutName,"%s", fTool.LocalizeDirName( fOutName)); // JB 2011/07/07
  TFile fOut(fOutName,"RECREATE");
  
  TNtuple Ntp("Results","Mimosa Analysis Results","MimosaType:ChipNumber:RunNumber:Temperature:IrradiationDose:Angle:MatrixNumber:SubMatrixNumber:AvSN:MPVSN:Noise:NoiseError:Efficiency:EfficiencyError:ResidualU:ResolutionU:ResidualV:ResolutionV:AvChargeSeed:AvCharge2x2:AvCharge3x3:AvCharge5x5:MPVChargeSeed:MPVCharge2x2:MPVCharge3x3:MPVCharge5x5:SN_Seed_CUT:SN_Neighbours_CUT:ClusteSize:Pixelsize:DiodeSize");
  
  //---ADC
  
  //---ADC 
  tVect[0] = (float)MimosaType;
  tVect[1] = (float)3; //ChipNumber;
  tVect[2] = (float)RunNumber;
  float Temperature=20;
  if (RunNumber ==  9581 || RunNumber ==  9582)Temperature=-20;  ; //Temperature;
  if (RunNumber ==  9629 || RunNumber ==  9630 || RunNumber ==  9583)Temperature=20; //Temperature;
  tVect[3] = (float)Temperature; //Temperature;
  tVect[4] = (float)0; //IrradiationDose;
  tVect[5] = (float)0; //Angle;
  
  tVect[6] = (float) ThePlaneNumber; //MatrixNumber
  tVect[7] = (float) ThesubmatrixNumber; //SubMatrixNumber 
  tVect[8] = (float) hsnc->GetMean(); //AvSN
  tVect[9] = (float) (hsnc->GetFunction("landau"))->GetParameter(1); //MPVSN 
  tVect[10] = (float) hRealTrackNoise->GetMean(); //Noise
  tVect[11] = (float) hRealTrackNoise->GetRMS(); //NoiseError
  tVect[12] = (float) MimosaEfficiency; //Efficiency
  tVect[13] = (float) MimosaEfficiency_ERR; //EfficiencyError
  tVect[14] = (float) ResolutionPoint[4]; // Residual[0];
  tVect[15] = (float) sqrt(ResolutionPoint[4]*ResolutionPoint[4]-1.0);  //Resolution[0]
  tVect[16] = (float) ResolutionPoint[5];// Residual[1]
  tVect[17] = (float) sqrt(ResolutionPoint[5]*ResolutionPoint[5]-1.0);// Resolution[1]
  
  tVect[18] = (float) hqcn[0]->GetMean(); // AvChargeSeed
  tVect[19] = (float) hqcn[3]->GetMean(); //AvCharge2x2
  tVect[20] = (float) hqcn[8]->GetMean(); // AvCharge3x3
  tVect[21] = (float) hqcn[24]->GetMean(); //AvCharge5x5
  
  tVect[22] = (float)  (hqcn[0]->GetFunction("landau"))->GetParameter(1);//PVChargeSeed
  tVect[23] = (float)  (hqcn[3]->GetFunction("landau"))->GetParameter(1); //MPVCharge2x2
  tVect[24] = (float)  (hqcn[8]->GetFunction("landau"))->GetParameter(1); //MPVCharge3x3
  tVect[25] = (float)  (hqcn[24]->GetFunction("landau"))->GetParameter(1); //MPVCharge5x5
  
  tVect[26] = (float) CUT_S2N_seed; //SN_Seed_CUT
  tVect[27] = (float) CUT_S2N_neighbour; //SN_Neighbours_CUT
  tVect[28] = (float) ChargeSpread->GetX()[TMath::LocMax(ChargeSpread->GetN(), ChargeSpread->GetY())]; //clustesize
  tVect[29] = (float) PixelSize;//pitch 
  
  float diode_size = 0;
  if((RunNumber ==  9581 || RunNumber ==  9630) && ThesubmatrixNumber==1) diode_size = 36;
  if((RunNumber ==  9581 || RunNumber ==  9630) && ThesubmatrixNumber==2) diode_size = 11.56;
  if((RunNumber ==  9582 || RunNumber ==  9583) && ThesubmatrixNumber==1) diode_size = 11.56;
  if((RunNumber ==  9582 || RunNumber ==  9583) && ThesubmatrixNumber==2) diode_size = 25;
  tVect[30] = (float) diode_size; //diode_size 
  
  // ->etudier les parametres clefs: efficacite, resolution, N, S/N, Charge
  // 1,4,9,25, (MPV plutot que valeur moyenne) "taille du cluster", en fonction
  // de la temperature, des tailles des diodes et du pitch. Dans un premier
  // temps tu peux faire un tableau resumant toutes ces valeurs.
  
  
  Ntp.Fill(tVect);
  Ntp.Write();
  Ntp.Show(0);
  fOut.Close();
  
  return 0;
}


//____________________________________________________________________________________
//
void MimosaAnalysis::CheckMimosaAlign()
{
  // the the quality of the Mimosa chip alignement
  // Modified JB 2010/03/12 for new histograms
  // Modified JB 2012/11/22 for new histograms and savings
  // Modified JB 2013/07/16 new alignment check histograms

 if(!CheckIfDone("mimosapro")) return;

  char fOutName[200];
  sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //sprintf(tmpFile,"%s",fTool.LocalizeDirName(tmpFile));
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");

  gStyle->SetPalette(1,0);
  UsedMacro[2] = 1;
  c2 = new TCanvas("calign","Check mimosa align",550,10,750,550);
  c2->SetFillColor(0);
  c2->Divide(2,4);
  c2->cd(1);
  hAllHuvsAllTu->SetStats(1);
  AutoZoom(hAllHuvsAllTu)->Draw("colz");
  c2->cd(2);
  hAllHvvsAllTv->SetStats(1);
  AutoZoom(hAllHvvsAllTv)->Draw("colz");
  c2->cd(3);
  hAlignHuTuvsTv->SetStats(1); // JB 2010/03/12
  hAlignHuTuvsTv->Draw("colz");
  c2->cd(4);
  hAlignHvTvvsTu->SetStats(1); // JB 2010/03/12
  hAlignHvTvvsTu->Draw("colz");
  c2->cd(5);
  hAlignHuTuvsTu->SetStats(1); // JB 2013/07/16
  hAlignHuTuvsTu->Draw("colz");
  c2->cd(6);
  hAlignHvTvvsTv->SetStats(1); // JB 2013/07/16
  hAlignHvTvvsTv->Draw("colz");
  c2->cd(7);
  //hAllHuvsAllTu1->SetStats(1);
  // AutoZoom(hAllHuvsAllTu1,"Sym")->Draw();
  hAlignHuTu->SetStats(1); // JB 2010/03/12
  AutoZoom(hAlignHuTu,"Sym")->Draw();
  c2->cd(8);
  //hAllHvvsAllTv1->SetStats(1);
  // AutoZoom(hAllHvvsAllTv1,"Sym")->Draw();
  hAlignHvTv->SetStats(1); // JB 2010/03/12
  AutoZoom(hAlignHvTv,"Sym")->Draw();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    c2->Print(EPSNameO.Data());
    c2->Print(EPSName.Data());
    c2->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // ------------------------
  // --- Save histos and canvas ---:
  
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2));
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");
  c2->Write();
  hAllHuvsAllTu->Write();
  hAllHvvsAllTv->Write();
  hAlignHuTuvsTv->Write();
  hAlignHvTvvsTu->Write();
  hAlignHuTuvsTu->Write();
  hAlignHvTvvsTv->Write();
  hAlignHuTuvsHv->Write();
  hAlignHvTvvsHu->Write();
  hAlignHuTu->Write();
  hAlignHvTv->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();
}

//____________________________________________________________________________________
//
void MimosaAnalysis::MimosaClusterCharge( TString fitOption)
{
  // Display the information relevant to charge collection, noise, S/N etc.
  // Option is "landau" or "gaus" depending on which charge distribution
  //  function is expected
  //
  //
  // Modified: JB 2010/04/28
  // Modified: JB 2012/08/19 cleaned
  // Modified: JB 2012/09/07 save noise histo
  // Modified: JB 2013/09/17 protection when fitting hSNNReal
  // Modified: JB 2013/10/29 allow two models for signal distri. Landau/Gauss
  
  if(!CheckIfDone("mimosall")) return;
  
  UsedMacro[1] = 1;
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleSize(0.06);
  gStyle->SetOptStat(1111);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetTitleH(.065);
  gStyle->SetTitleH(.085);
  gStyle->SetOptStat(1111);

  cClusterCharge = new TCanvas("cClusterCharge","Cluster charges summary",850,10,700,900);
  cClusterCharge->Draw();
  cClusterCharge->Clear();
  cClusterCharge->SetBorderMode(0);
  
  // ------------------------
  // --- build the screen ---:
  
  //---ab
  Char_t titleSN[20];
  sprintf(titleSN,"e=%f",MimosaEfficiency);
  
  //--ab
  cClusterCharge->Update();
  TPad* mpad1 = new TPad("mpad1","Total charge in N (charge ordered) pixels",0.67,0.02,0.99,0.5);
  mpad1->SetLeftMargin(0.2);
  mpad1->SetGrid(1);
  mpad1->Draw();
  TPad* mpad2 = new TPad("mpad2","Charge sum in the 3x3 highest pixels",
                         0.33,0.51,0.66,0.9);
  mpad2->SetGrid(1);
  mpad2->Draw();
  TPad* mpad3 = new TPad("mpad3","Signal/Noise in the central pixel",
                         0.33,0.02,0.66,0.5);
  mpad3->SetGrid(1);
  mpad3->Draw();
  TPad* mpad4 = new TPad("mpad4","Noise in the central pixel",
                         0.01,0.02,0.32,0.5);
  mpad4->SetGrid(1);
  mpad4->Draw();
  TPad* mpad6 = new TPad("mpad6","Charge in the central(seed) pixel",
                         0.01,0.51,0.32,0.9);
  mpad6->SetGrid(1);
  mpad6->Draw();
  TPad* mpad7 = new TPad("mpad7","Charge sum in the 5x5 highest pixels",
                         0.67,0.51,0.99,0.9);
  mpad7->Draw();
  
  TPad* mpad5 = new TPad("mpad5","MIMOSA cluster charge",
                         0.01,0.91,0.99,0.99);
  mpad5->Draw();
  mpad5->SetFillColor(19);
  mpad5->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(4);
  ttt->SetTextSize(.4);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  Char_t Header[100];
  //---ab
  sprintf(Header,"M%d ; run %d; Pl %d, sub %d, dist %.0f; Gain %3.2f; eff %3.3f +- %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,ThesubmatrixNumber,TrackToHitDistanceLimit,calibration,MimosaEfficiency,MimosaEfficiency_ERR,CUT_S2N_seed,CUT_S2N_neighbour);
  
  ttt->DrawText( 0.05,0.6,Header);
  TText *tt2 = new TText();
  tt2->SetTextFont(222);
  tt2->SetTextColor(1);
  tt2->SetTextSize(0.2);
  tt2->SetTextAlign(12);
  
  mpad1->SetFillColor(19);
  mpad2->SetFillColor(19);
  mpad3->SetFillColor(19);
  mpad4->SetFillColor(19);
  mpad6->SetFillColor(19);
  mpad7->SetFillColor(19);
  
  ttt->DrawText( 0.05,0.6,Header);
  tt2 = new TText();
  tt2->SetTextFont(222);
  tt2->SetTextColor(1);
  tt2->SetTextSize(0.2);
  tt2->SetTextAlign(12);
  
  mpad1->SetFillColor(19);
  mpad2->SetFillColor(19);
  mpad3->SetFillColor(19);
  mpad4->SetFillColor(19);
  mpad6->SetFillColor(19);
  mpad7->SetFillColor(19);
  
  //--- end of screen building ---
  
  
  // ------------------------
  // --- Draw simple histos ---:

  //- Signal/Noise in the central pixel
  mpad3->cd();
  if( hSNReal->Integral()>100 ) {
    hSNReal->Fit(fitOption.Data());
    (hSNReal->GetFunction(fitOption.Data()))->SetLineColor(2);
  }
  else {
    hSNReal->Draw();
  }
  mpad3->Update();
  
  //- Noise in the central pixel
  mpad4->cd();
  AutoZoom(hRealTrackNoise)->Draw();
  mpad4->Update();
  
  //- Charge in the central(seed) pixel
  mpad6->cd();
  hqcn[0]->Draw();
  //AutoZoom(hqcn[0],"all", hqcn[0]->GetMaximum()/2.)->Draw(); 
  mpad6->Update();
  TPaveStats *stats2 = (TPaveStats*)hqcn[0]->GetListOfFunctions()->FindObject("stats");
  if(stats2!=0){
    stats2->SetX1NDC(0.5);stats2->SetX2NDC(0.95);stats2->SetY1NDC(0.5);stats2->SetY2NDC(0.97);
    
    stats2->SetOptStat(111111);stats2->Draw();
  }
  
  //- Charge sum in the 3x3 highest pixels
  mpad2->cd();
  mpad2->GetFrame()->SetFillColor(0);
  
  //AutoZoom(hqcn[8],"all", hqcn[8]->GetMaximum()/1.5)->Draw(); 
  hqcn[8]->Draw();
  //hqcn[TMath::Min(NumOfPoint/2,8)]->Draw(); 
  
  mpad2->Update();
  TPaveStats *stats1 = (TPaveStats*)hqcn[8]->GetListOfFunctions()->FindObject("stats");
  if(stats1!=0){
    stats1->SetX1NDC(0.5);stats1->SetX2NDC(0.95);stats1->SetY1NDC(0.5);stats1->SetY2NDC(0.97);
    stats1->SetOptStat(111111);stats1->Draw();
  }
  
  //- Charge sum in the 5x5 highest pixels
  if( MaxNofPixelsInCluster>24 ) {
    mpad7->cd();
    
    hqcn[24]->Draw(); 
    //AutoZoom(hqcn[24],"all", hqcn[24]->GetMaximum()/1.3)->Draw(); 
    //hqcn[TMath::Min(NumOfPoint-1,24)]->Draw(); 
    
    mpad7->Update();
    TPaveStats *stats3 = (TPaveStats*)hqcn[24]->GetListOfFunctions()->FindObject("stats");
    if(stats3!=0){
      stats3->SetX1NDC(0.5);stats3->SetX2NDC(0.95);stats3->SetY1NDC(0.5);stats3->SetY2NDC(0.97);
      stats3->SetOptStat(111111);stats3->Draw();
    }
  }
  
  // ------------------------
  // --- Charge spread histo ---:
  
  //- Compute the average & MPV charge collected by a number of pixels
  // modified by JB 2010/03/12

  //mpad1->cd();
  TCanvas *cCharges = new TCanvas("cCharges","Cluster charges detail",350,10,1200,900);
  cCharges->Draw();
  cCharges->Clear();
  cCharges->SetBorderMode(0);
  Int_t ncols = (Int_t)(sqrt(MaxNofPixelsInCluster));
  Int_t nrows = ceil(MaxNofPixelsInCluster/ncols);
  cCharges->Divide( ncols, nrows ); 
  
  Float_t* NumberOfEntries = new Float_t[MaxNofPixelsInCluster]; 
  Float_t* Chi2 = new Float_t[MaxNofPixelsInCluster]; 
  Float_t* MPVerr = new Float_t[MaxNofPixelsInCluster]; 
  Float_t* Ndf = new Float_t[MaxNofPixelsInCluster]; 
  Float_t* ChargeOfPixelGroup = new Float_t[MaxNofPixelsInCluster];
  Float_t* RMSChargeOfPixelGroup = new Float_t[MaxNofPixelsInCluster];
  TF1 *fitf = 0;
  
  Int_t NumOfPoint = 0; // num of valid point
  TH1F *hToFit;
  for(Int_t i=0; i<MaxNofPixelsInCluster; i++){
    cCharges->cd(i+1);
    hToFit = (TH1F*)AutoZoom(hqcn[i],"all", hqcn[i]->GetMaximum()/1.3);
    NumberOfEntries[i]=hqcn[i]->Integral();
    //NumberOfEntries[i]=hqcn[i]->GetEntries();
    
    if ( NumberOfEntries[i]>200 ) { // check there is enough data points for a fit
      NumOfPoint++;
//      hToFit->Draw();
//      ChargeOfPixelGroup[i]=hToFit->GetMean();
//      MPVerr[i]=hToFit->GetRMS()/sqrt(NumberOfEntries[i]);
//      RMSChargeOfPixelGroup[i]=hToFit->GetRMS();
      //hqcn[i]->Fit(fitOption.Data(),"RMWQ");
      hToFit->Fit(fitOption.Data(),"MWQ");
      hqcn[i]->GetXaxis()->SetRange( 1, hqcn[i]->GetNbinsX());
      hqcn[i]->Draw(); // unzoom after the fit
      fitf = hqcn[i]->GetFunction(fitOption.Data());
      if(!(gMinuit->fCstatu).Contains("CONVERGED") || TMath::Abs(fitf->GetParameter(1) - (i>0)*ChargeOfPixelGroup[i-i]) > (100./i)*(i>0)+700*(i<=0))  {
        if(MimoDebug>2) {
          cout<<"BAD FIT !!! "<<i<<endl;
        } 
        //hqcn[i]->Fit(fitOption.Data(),"MWQ"); // why trying again ??
        ChargeOfPixelGroup[i]=hToFit->GetMean();
        MPVerr[i]=hToFit->GetRMS()/sqrt(NumberOfEntries[i]);
        RMSChargeOfPixelGroup[i]=hToFit->GetRMS();
      }
      else {
        fitf->SetLineWidth(2);
        fitf->SetLineColor(2);
        ChargeOfPixelGroup[i]=fitf->GetParameter(1);
        Chi2[i]=fitf->GetChisquare();
        Ndf[i]=fitf->GetNDF();
        MPVerr[i]=fitf->GetParError(1)*sqrt(Chi2[i]/Ndf[i]);
        RMSChargeOfPixelGroup[i]=hqcn[i]->GetRMS()/sqrt(NumberOfEntries[i]);
      }
      
      printf(" ClusterCharge: %d pixels, <MPV %s> = %f +- %f\n", i, fitOption.Data(), ChargeOfPixelGroup[i], MPVerr[i]);
    }
    
    else { // not enough data points
      hqcn[i]->Draw();
      if(i==0) { // for seed distribution, take histo mean and RMS
        ChargeOfPixelGroup[i]=hqcn[i]->GetMean();
        MPVerr[i]=hqcn[i]->GetRMS()/TMath::Max(1.,sqrt(hqcn[i]->GetEntries()));
        RMSChargeOfPixelGroup[i]=hqcn[i]->GetRMS();
      }
      else {  // simply use the previous value
        ChargeOfPixelGroup[i]=ChargeOfPixelGroup[i-1]; 
        MPVerr[i]=MPVerr[i-1];
        RMSChargeOfPixelGroup[i]=RMSChargeOfPixelGroup[i-1];
//        ChargeOfPixelGroup[i]=0.;
//        MPVerr[i]=0.;
//        RMSChargeOfPixelGroup[i]=0.;
      }
      printf(" ClusterCharge: %d pixels q distribution has %d points, which is not enough to fit -> keep previous fit value <MPV %s> = %f +- %f\n", i, (Int_t)NumberOfEntries[i], fitOption.Data(), ChargeOfPixelGroup[i], MPVerr[i]);
    }
    
    cCharges->Update();
  }
    
  cout << "# valid points " << NumOfPoint << endl;
  Float_t* NumOfPix = new Float_t[MaxNofPixelsInCluster];
  for(Int_t in=0; in<MaxNofPixelsInCluster; in++){
    NumOfPix[in]=1.*in+1.;
  }
  mpad1->cd();
  mpad1->Clear();
  Float_t* ErrorX = new Float_t[MaxNofPixelsInCluster];
  for (int i=0; i<MaxNofPixelsInCluster;i++) 
  {ErrorX[i]=0;}
  cout << "creating ChargeSpread" << endl;
  ChargeSpread = new TGraphErrors( MaxNofPixelsInCluster/*TMath::Min(NumOfPoint+3,MaxNofPixelsInCluster)*/,NumOfPix,ChargeOfPixelGroup,ErrorX,MPVerr);
  /*
   // this is in principle not useful anymore, fixed in the config file
  switch(MimosaType){
    case 2:ChargeSpread->SetMaximum(2500.); break;
    case 4:ChargeSpread->SetMaximum(10000.); break; // for may-03
    case 5:ChargeSpread->SetMaximum(5000.); break;
    default: ChargeSpread->SetMaximum(2500.);
  }
  */
  //--find the maximum:
  Int_t maximumYaxischarge = 50;
  Int_t estimatedClusterSize=0;
  cout << "finding ChargeSpread maximum" << endl;
  for(Int_t in=0; in<MaxNofPixelsInCluster; in++){
    if(maximumYaxischarge < ChargeOfPixelGroup[in]){
      maximumYaxischarge=(Int_t)ChargeOfPixelGroup[in];
    }
    if ( ChargeOfPixelGroup[in] > ChargeOfPixelGroup[MaxNofPixelsInCluster-1] ) {
      estimatedClusterSize = in;
    }
  }
  printf(" --> ClusterCharge: highestcharge is %.0f, charge of largest subarea is %d, estimated cluster size %d.\n", double(maximumYaxischarge), int(ChargeOfPixelGroup[MaxNofPixelsInCluster-1]), int(estimatedClusterSize));
  ChargeSpread->SetMinimum(0.);
  
  //--fit
  TF1* pol6 = new TF1("pol6","pol6"); // def pol6 comp
  // don't fit if first point is zero (crashes otherwise), JB 2010/0316)
  if( NumOfPoint>1 ) {
    cout << "fitting ChargeSpread" << endl;
    ChargeSpread->Fit("pol6","");
  }
  
  cout << "drawing ChargeSpread" << endl;
  ChargeSpread->Draw("AP");
  ChargeSpread->SetMarkerStyle(20);
  ChargeSpread->SetMarkerSize(1.);
  ChargeSpread->SetLineWidth(2);
  ChargeSpread->SetLineColor(1);
  ChargeSpread->SetName("chargespread");
  ChargeSpread->SetTitle("Total charge in N pixels");
  ChargeSpread->GetHistogram()->SetMaximum(maximumYaxischarge+50.0);
  ChargeSpread->GetHistogram()->SetXTitle("Number of pixels in a cluster");
  ChargeSpread->GetHistogram()->SetYTitle("Accumulated charge (electrons)");
  ChargeSpread->SetMarkerColor(kRed);
//  fitf->SetLineWidth(2);
//  fitf->SetLineColor(2);
  mpad1->Update();
  cout << "END fitting ChargeSpread" << endl;
  
  /*TPaveStats *stats = (TPaveStats*)ChargeSpread->FindObject("stats");
  if(stats!=0){
    stats->SetX1NDC(0.4);stats->SetX2NDC(0.95);stats->SetY1NDC(0.23);stats->SetY2NDC(0.56);
    stats->Draw();
  }
  //else{cout<<"test 01"<<endl;}
  mpad1->GetFrame()->SetFillColor(0);
  ChargeSpread->GetHistogram()->SetTitleOffset(1.4);
  ChargeSpread->GetHistogram()->SetTitleOffset(1.9,"Y");
  mpad1->Modified();
  */
  // --- end of Charge spread histo

  /*
   cClusterCharge->cd();
   for(int ipad=1;ipad<=6;ipad++)
   {
   cClusterCharge->cd(ipad);
   gPad->Modified();
   gPad->Update();
   }
   */
  
  // ------------------------
  // --- Save histos and canvas ---:
  
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");
  for(Int_t i=0; i<MaxNofPixelsInCluster; i++){ // Test by JB, Sept 2007
    hqcn[i]->Write();
    hqcngeom[i]->Write();
  }
  hSNReal->Write();
  hRealTrackNoise->Write(); // added, JB 2013/09/07
  ChargeSpread->Write();
  if( NumOfPoint>1 ) pol6->Write();
  cClusterCharge->Write();
  cCharges->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();
}
//________________________________________________________________________________________
//
void MimosaAnalysis::CheckClusters()
{
  // Part of the Checkreftracks method concerning only the clusters.
  // Called by Checkreftracks when there are tracks.
  //
  // Added: JB 2013/10/29 
  // Modified: JB 2013/11/08 new histos added (canvas 2)
  // Modified: JB 2013/05/22 new histos added (canvas 3)
  
  if(!CheckIfDone("mimosall")) return;

  char fOutName[200];
  sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //sprintf(tmpFile,"%s",fTool.LocalizeDirName(tmpFile));
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");
  
  UsedMacro[6] = 1;
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);
  gStyle->SetPalette(1,0);
  
  //--------------------------------
  //  Cluster properties 1
  //--------------------------------
  
  c4 = new TCanvas("cclusterproperties","Cluster properties 1",100,10,700,900);
  c4->Draw();
  c4->Clear();
  c4->SetBorderMode(0);
  
  // --- build the screen ---:
  gPad->cd();
  c4->Clear();
  
  TPad* mpad2_1 = new TPad("mpad2_1","",0.01,0.01,0.32,0.30);
  mpad2_1->SetGrid(1);
  mpad2_1->Draw();
  
  TPad* mpad2_2 = new TPad("mpad2_2","",0.33,0.01,0.66,0.30);
  mpad2_2->SetGrid(1);
  mpad2_2->Draw();
  
  TPad* mpad2_3 = new TPad("mpad2_3","title 1",0.67,0.01,0.99,0.30);
  mpad2_3->SetGrid(1);
  mpad2_3->Draw();
  
  TPad* mpad2_4 = new TPad("mpad2_4","",0.01,0.31,0.32,0.60);
  mpad2_4->SetGrid(1);
  mpad2_4->Draw();
  
  TPad* mpad2_5 = new TPad("mpad2_5","",0.33,0.31,0.66,0.60);
  mpad2_5->SetGrid(1);
  mpad2_5->Draw();
  
  TPad* mpad2_6 = new TPad("mpad2_6","",0.67,0.31,0.99,0.60);
  mpad2_6->SetGrid(1);
  mpad2_6->Draw();
  
  TPad* mpad2_7 = new TPad("mpad2_7","",0.01,0.61,0.32,0.9);
  mpad2_7->SetGrid(1);
  mpad2_7->Draw();
  
  TPad* mpad2_8 = new TPad("mpad2_8","",0.33,0.61,0.66,0.9);
  mpad2_8->SetGrid(1);
  mpad2_8->Draw();
  
  TPad* mpad2_9 = new TPad("mpad2_9","",0.67,0.61,0.99,0.9);
  mpad2_9->SetGrid(1);
  mpad2_9->Draw();
  
  TPad* mpad2_title = new TPad("mpad2_title","",0.10,0.91,0.85,0.99);
  mpad2_title->SetGrid(1);
  mpad2_title->SetFillColor(19);
  mpad2_title->Draw();
  mpad2_title->cd();
  TText *ttt2 = new TText();
  ttt2->SetTextFont(25);
  ttt2->SetTextColor(9);
  ttt2->SetTextSize(.3);
  ttt2->SetTextAlign(12);
  ttt2->SetTextFont(22);
  Char_t Header2[100];
  //---ab
  sprintf(Header2,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
  ttt2->DrawText( 0.05,0.6,Header2);
  
  mpad2_1->SetFillColor(19);
  mpad2_2->SetFillColor(19);
  mpad2_3->SetFillColor(19);
  mpad2_4->SetFillColor(19);
  mpad2_5->SetFillColor(19);
  mpad2_6->SetFillColor(19);
  mpad2_7->SetFillColor(19);
  mpad2_8->SetFillColor(19);
  mpad2_9->SetFillColor(19);
  
  //----------- Seed S/N, ALL HITS
  mpad2_1->cd();
  mpad2_1->SetLogy(0);
//  AutoZoom(hAllS2N,"Max")->Draw();
  mpad2_1->Update();
  //----------- SNR seed pixel
  mpad2_2->cd();
  mpad2_2->SetLogy(0);
  AutoZoom(hSNNReal,"Max")->Draw();
  mpad2_2->Update();
  //----------- Seed S/N vs 2nd highest S/N
  mpad2_3->cd();
  AutoZoom(hSN_vs_SNNReal)->Draw("colz");
  //AutoZoom(hPedestal)->Draw();
  mpad2_3->Update();
  //----------- SNR of sum of neighbour pixels without seed
  mpad2_4->cd();
  AutoZoom(hSNneighbour)->Draw();
  mpad2_4->Update();
  //----------- "# GOOD hits in plane when a GOOD track is in GEOMATRIX
  mpad2_5->cd();
  if( hnGOODhitwhentrack->GetEntries()>0 ) mpad2_5->SetLogy(1);
  AutoZoom(hnGOODhitwhentrack,"Max")->Draw();
  mpad2_5->Update();
  //----------- # GOOD hits in plane vs event number
  mpad2_6->cd();
  mpad2_6->SetLogy(0);
  //  hnahitievt->SetAxisRange(0,10100,"X");
  //hnahitievt->Draw("colz");
  AutoZoom(hnahitievt)->Draw("colz");
  mpad2_6->Update();
  //----------- Signal/noise in the seed pixel(0, ordered by S/N), MATCHED hits
  mpad2_7->cd();
  hsnn[0]->SetStats(1111);
  hsnn[0]->Draw();
  TPaveStats *st = (TPaveStats*)hsnn[0]->GetListOfFunctions()->FindObject("stats");
  if(st!=0){st->SetX1NDC(0.65) ; st->SetX2NDC(0.9) ; st->SetY1NDC(0.65) ; st->SetY2NDC(0.9);}
  hsnn[0]->SetFillColor(29);
  hsnn[0]->SetXTitle("Signal/Noise");
  hsnn[0]->SetYTitle("Counts");
  hsnn[0]->SetTitleOffset(1.4);
  TF1 *fitf2 = NULL;
  if (hsnn[0]->Integral()>100) { // if enough data
    hsnn[0]->Fit("landau");
    fitf2 = hsnn[0]->GetFunction("landau");	  
    fitf2->SetLineWidth(1);
    fitf2->SetLineColor(1);
  }
  else {
    hsnn[0]->Draw();
    cout << " Not enough data in seed SNR histo, integral is " << hsnn[0]->Integral() << endl;
  }
  //----------- nb of pixels in MATCHED hits
  mpad2_8->cd();
  mpad2_8->SetLogy(0);
  hnpix_c->Draw();
  mpad2_8->Update();
  //----------- Charge of neighbours (w/o seed)
  mpad2_9->cd();
  //hnhitievt->SetAxisRange(0,10000,"X");
  //AutoZoom(hnhitievt)->Draw("colz");
  AutoZoom(etal[1],"Max")->Draw();
  mpad2_9->Update();
  
  gPad->Update();
  
  c4->cd();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    c4->Print(EPSNameO.Data());
    c4->Print(EPSName.Data());
    c4->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }
  
  //--------------------------------
  //  Cluster properties 2
  //--------------------------------
  
  cClusterProperties2 = new TCanvas("cclusterproperties2","Cluster properties 2",100,15,700,900);
  cClusterProperties2->Draw();
  cClusterProperties2->Clear();
  cClusterProperties2->SetBorderMode(0);
  cClusterProperties2->cd();
  
  TPad* mpad3_title = new TPad("mpad3_title","",0.10,0.91,0.85,0.99);
  mpad3_title->SetGrid(1);
  mpad3_title->SetFillColor(19);
  mpad3_title->Draw();
  mpad3_title->cd();
  ttt2->DrawText( 0.05,0.6,Header2);
  
  cClusterProperties2->cd();
  TPad* mpad3_histo = new TPad("mpad3_histo","",0.01,0.01,0.99,0.90);
  mpad3_histo->SetGrid(1);
  mpad3_histo->Draw();
  mpad3_histo->cd();
  mpad3_histo->Divide(3,3);
  
  //-----------
  mpad3_histo->cd(1);
  AutoZoom(hnGOODhit,"Max")->Draw();
  //----------- Charge vs S/N seed, MATCHED hits
  mpad3_histo->cd(2);
  AutoZoom(hseedQvsS2NGood,"Max")->Draw("colz");
  //----------- S/N optimized, cluster restricted for highest S/N track-MATCHED
  mpad3_histo->cd(3);
  AutoZoom(hsnc,"Max")->Draw();
  //----------- Ucg-Udig
  mpad3_histo->cd(4);
  AutoZoom(hdCGDigU)->Draw();
  //----------- Vcg-Vdig
  mpad3_histo->cd(5);
  AutoZoom(hdCGDigV)->Draw();
  //----------- Neighbour SNR versus Seed SNR
  mpad3_histo->cd(6);
  AutoZoom(hSNseedvsSNneighbour)->Draw("colz");
  //----------- Neighbour charge versus Seed charge
  mpad3_histo->cd(7);
  AutoZoom(hQseedvsQneighbour)->Draw("colz");
  //----------- Seed noise
  mpad3_histo->cd(8);
  AutoZoom(hRealTrackNoise)->Draw();
  //----------- Cluster charge vs seed SNR
  mpad3_histo->cd(9);
  AutoZoom(hSNseedvsQcluster)->Draw("colz");
  
  cClusterProperties2->Update();
 
  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cClusterProperties2->Print(EPSNameO.Data());
    cClusterProperties2->Print(EPSName.Data());
    cClusterProperties2->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  //--------------------------------
  //  Cluster properties 3
  //--------------------------------
  
  cClusterProperties3 = new TCanvas("cClusterProperties3","Cluster properties 3",100,15,700,900);
  cClusterProperties3->Draw();
  cClusterProperties3->Clear();
  cClusterProperties3->SetBorderMode(0);
  cClusterProperties3->cd();
  
  TPad* mpad4_title = new TPad("mpad4_title","",0.10,0.91,0.85,0.99);
  mpad4_title->SetGrid(1);
  mpad4_title->SetFillColor(19);
  mpad4_title->Draw();
  mpad4_title->cd();
  ttt2->DrawText( 0.05,0.6,Header2);
  
  cClusterProperties3->cd();
  TPad* mpad4_histo = new TPad("mpad4_histo","",0.01,0.01,0.99,0.90);
  mpad4_histo->SetGrid(1);
  mpad4_histo->Draw();
  mpad4_histo->cd();
  mpad4_histo->Divide(3,3);
  
  //----------- Cluster charge vs seed charge for track-MATCHED hits
  mpad4_histo->cd(1);
  AutoZoom(hQseedvsQcluster)->Draw("colz");
  //----------- Charge in the 4 direct neighbours to the seed (cross) for MATCHED
  mpad4_histo->cd(2);
  AutoZoom(hChargeInCross,"Max")->Draw();
  //----------- Charge in the 4 corners to the seed (X) for MATCHED
  mpad4_histo->cd(3);
  AutoZoom(hChargeInX,"Max")->Draw();
  //-----------Charge in the first crown for MATCHED
  mpad4_histo->cd(4);
  AutoZoom(hChargeInCrown1)->Draw();
  //----------- Charge in the second crown for MATCHED
  mpad4_histo->cd(5);
  AutoZoom(hChargeInCrown2)->Draw();
  //----------- 
  mpad4_histo->cd(6);
  //AutoZoom(hSeedBetweenDist)->Draw();
  hMinDistance_vs_2ndDistance->Draw("colz");
  //----------- 
  mpad4_histo->cd(7);
  hdCGDigUV->Draw("colz");
  //-----------
  mpad4_histo->cd(8);
  AutoZoom(hTrackTo2ndclosestClusterDistance)->Draw();
  //-----------
  mpad4_histo->cd(9);
  AutoZoom(hWrongAssociationProba)->Draw();

  cClusterProperties3->Update();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cClusterProperties3->Print(EPSNameO.Data());
    cClusterProperties3->Print(EPSName.Data());
    cClusterProperties3->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }


  //--------------------------------
  //  Cluster properties 4
  //--------------------------------

  cClusterProperties4 = new TCanvas("cClusterProperties4","Cluster properties 3",100,15,700,900);
  cClusterProperties4->Draw();
  cClusterProperties4->Clear();
  cClusterProperties4->SetBorderMode(0);
  cClusterProperties4->cd();

  TPad* mpad5_title = new TPad("mpad5_title","",0.10,0.91,0.85,0.99);
  mpad5_title->SetGrid(1);
  mpad5_title->SetFillColor(19);
  mpad5_title->Draw();
  mpad5_title->cd();
  ttt2->DrawText( 0.05,0.6,Header2);
  
  cClusterProperties4->cd();
  TPad* mpad5_histo = new TPad("mpad5_histo","",0.01,0.01,0.99,0.90);
  mpad5_histo->SetGrid(1);
  mpad5_histo->Draw();
  mpad5_histo->cd();
  mpad5_histo->Divide(2,2);

  mpad5_histo->cd(1);
  mpad5_histo->cd(1)->SetTickx(1);
  mpad5_histo->cd(1)->SetTicky(1);
  hDuplicate_2DMult->Draw("colz");
  mpad5_histo->cd(2);
  mpad5_histo->cd(2)->SetTickx(1);
  mpad5_histo->cd(2)->SetTicky(1);
  hDuplicate_DeltaTS->Draw();
  mpad5_histo->cd(3);
  mpad5_histo->cd(3)->SetTickx(1);
  mpad5_histo->cd(3)->SetTicky(1);
  //hDuplicate_npixc->Scale(100.0/hnpix_c->GetEntries());
  //AutoZoom(hDuplicate_npixc)->Draw();
  for(int imult=0;imult<hnpix_c->GetXaxis()->GetNbins();imult++) {
    double Num = hDuplicate_npixc->GetBinContent(imult+1);
    double Den = hnpix_c->GetBinContent(imult+1);
    double Prob[2];
    if(Den > 0.0) {
      Prob[0] = Num/Den;
      Prob[1] = sqrt(Prob[0]*(1.0 - Prob[0])/Den);
    }
    else {
      Prob[0] = 0.0;
      Prob[1] = 1.0e-10;
    }
    Prob[0] *= 100.0;
    Prob[1] *= 100.0;
    hDuplicate_npixc->SetBinContent(imult+1,Prob[0]);
    hDuplicate_npixc->SetBinError(imult+1,Prob[1]);
  }
  hDuplicate_npixc->SetStats(false);
  hDuplicate_npixc->Draw();
  mpad5_histo->cd(4);
  mpad5_histo->cd(4)->SetTickx(1);
  mpad5_histo->cd(4)->SetTicky(1);
  //hDuplicate_npixc_vs_TrkDistToDiode->Scale(100.0/hnpix_c->GetEntries());
  for(int imult=0;imult<hnpixc_vs_TrkDistToDiode->GetXaxis()->GetNbins();imult++) {
    for(int itrkD=0;itrkD<hnpixc_vs_TrkDistToDiode->GetYaxis()->GetNbins();itrkD++) {
      double Num = hDuplicate_npixc_vs_TrkDistToDiode->GetBinContent(imult+1,itrkD+1);
      double Den = hnpixc_vs_TrkDistToDiode->GetBinContent(imult+1,itrkD+1);
      double Prob = 0.0;
      if(Den > 0.0) {
	Prob = Num/Den;
      }
      Prob *= 100.0;
      hDuplicate_npixc_vs_TrkDistToDiode->SetBinContent(imult+1,itrkD+1,Prob);
    }
  }
  hDuplicate_npixc_vs_TrkDistToDiode->SetStats(false);
  hDuplicate_npixc_vs_TrkDistToDiode->Draw("colz");
  cClusterProperties4->Update();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cClusterProperties4->Print(EPSNameO.Data());
    cClusterProperties4->Print(EPSName.Data());
    cClusterProperties4->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }
  
  //--------------------------------
  //--------------WRITE IN A ROOT FILE:
  
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileName[50];
  sprintf(ResultFileName,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName,"%s", fTool.LocalizeDirName( ResultFileName)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName,"UPDATE");

  //---histos: 
  hNorm->Write();
  h2DgoodSeedPixel->Write();
  hnhit->Write();
  //hgoodSN_vs_SNN->Write();
  //hallSN_vs_SNN->Write();
  //hallSN_minus_hgoodSN_vs_SNN->Write();
  hSNReal->Write();
  hSNNReal->Write();
  hSN_vs_SNNReal->Write();
  hnGOODhit->Write();
  hseedQvsS2NGood->Write();
  hsnc->Write();
  hSNseedvsSNneighbour->Write();
  hQseedvsQneighbour->Write();
  hSNseedvsQcluster->Write();
  hSeedBetweenDist->Write();

  // JB 2009/09/01
  hoptimalsize->Write();
  hnpix->Write();
  hnpix_c->Write();
  hnpix_nc->Write();
  
  hdCGDigUV->Write();
  hTrackTo2ndclosestClusterDistance->Write();

  hDuplicate_2DMult->Write();
  hDuplicate_DeltaTS->Write();
  hDuplicate_npixc->Write();
  hDuplicate_npixc_vs_TrkDistToDiode->Write();

  //canvas:
  c4->Write();
  cClusterProperties2->Write();
  cClusterProperties3->Write();
  cClusterProperties4->Write();
  
  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();
}

//________________________________________________________________________________________
//
void MimosaAnalysis::Checkreferencetracks()
{
  //  plot distribution on tracks position.
  // (Check the angle effect)
  // A. Besson november 2003
  // Modified: JB 2012/08/20 histos reshuffling
  // Modified: JB 2014/04/01 histos with #hits/track added
  
  if(!CheckIfDone("mimosapro")) return;
  
  char fOutName[200];
  sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //sprintf(tmpFile,"%s",fTool.LocalizeDirName(tmpFile));
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");

  UsedMacro[6] = 1;
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);
  gStyle->SetPalette(1,0);
  
  //--------------------------------
  // ---> first canvas.
  //  correlation between tracks and hits
  //--------------------------------  
  
  c3 = new TCanvas("ctrackandhit","Track and hit Correlation",850,10,700,900);
  c3->Draw();
  c3->Clear();
  c3->SetBorderMode(0);
  Int_t MimosaType=RunNumber/1000;
  
  // --- build the screen ---:
  gPad->cd();
  c3->Clear();
  
  TPad* mpad1 = new TPad("mpad1","",0.01,0.01,0.32,0.30);
  mpad1->SetGrid(0);
  mpad1->SetLeftMargin(0.2);
  mpad1->Draw();
  
  TPad* mpad2 = new TPad("mpad2","",0.33,0.01,0.66,0.30);
  mpad2->SetGrid(1);
  mpad2->Draw();
  
  TPad* mpad3 = new TPad("mpad3","title 1",0.67,0.01,0.99,0.30);
  mpad3->SetGrid(1);
  mpad3->Draw();
  
  TPad* mpad4 = new TPad("mpad4","",0.01,0.31,0.32,0.60);
  mpad4->SetGrid(1);
  mpad4->Draw();
  
  TPad* mpad5 = new TPad("mpad5","",0.33,0.31,0.66,0.60);
  mpad5->SetGrid(1);
  mpad5->Draw();
  
  TPad* mpad6 = new TPad("mpad6","",0.67,0.31,0.99,0.60);
  mpad6->SetGrid(1);
  mpad6->Draw();
  
  TPad* mpad7 = new TPad("mpad7","",0.01,0.61,0.32,0.9);
  mpad7->SetGrid(1);
  mpad7->Draw();
  
  TPad* mpad8 = new TPad("mpad8","",0.33,0.61,0.66,0.9);
  mpad8->SetGrid(1);
  mpad8->Draw();
  
  TPad* mpad9 = new TPad("mpad9","",0.67,0.61,0.99,0.9);
  mpad9->SetGrid(1);
  mpad9->Draw();
  
  TPad* mpadtitle = new TPad("mpadtitle","",0.01,0.91,0.99,0.99);
  mpadtitle->SetGrid(1);
  mpadtitle->SetFillColor(19);
  mpadtitle->Draw();
  mpadtitle->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.3);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  Char_t Header[100];
  //---ab
  sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
  
  ttt->DrawText( 0.05,0.6,Header);
  
  mpad1->SetFillColor(19);
  mpad2->SetFillColor(19);
  mpad3->SetFillColor(19);
  mpad4->SetFillColor(19);
  mpad5->SetFillColor(19);
  mpad6->SetFillColor(19);
  mpad7->SetFillColor(19);
  mpad8->SetFillColor(19);
  mpad9->SetFillColor(19);
  
  //----------- tracks unmatched with hits
  mpad1->cd();
  //  if(MimosaType==5){
  //    htuhtv->SetAxisRange(-7000,7000,"X");
  //    htuhtv->SetAxisRange(-7000,7000,"Y");
  //  }else{
  //    htuhtv->SetAxisRange(-10000,10000,"X");
  //    htuhtv->SetAxisRange(-10000,10000,"Y");
  //  }
  //  htuhtv->SetTitleOffset(1.4);
  //  htuhtv->SetTitleOffset(1.9,"Y");
  AutoZoom(htuhtv)->Draw("colz");
  mpad1->Update();
  //-----------
  mpad2->cd();
  //  if(MimosaType==5){
  //    htu->SetAxisRange(-8000,8000,"X");
  //  }else{
  //    htu->SetAxisRange(-10000,10000,"X");
  //  }
  //  htu->SetTitleOffset(1.4);
  AutoZoom(htu)->Draw();
  mpad2->Update();
  //-----------
  mpad3->cd();
  //  if(MimosaType==5){
  //    htv->SetAxisRange(-8000,8000,"X");
  //  }else{
  //    htv->SetAxisRange(-10000,10000,"X");
  //  }
  //  htv->SetTitleOffset(1.4);
  AutoZoom(htv)->Draw();
  mpad3->Update();
  //-----------
  mpad4->cd();
  //  h2dmatchedhits->SetAxisRange(-5000,5000,"X");
  //  h2dmatchedhits->SetAxisRange(-2500,2500,"Y");
  AutoZoom(h2dmatchedhits)->Draw("colz");
  mpad4->Update();
  //-----------
  mpad5->cd();
  AutoZoom(hnGOODhit,"Max")->Draw();
  
  mpad5->Update();
  //-----------
  mpad6->cd();
  AutoZoom(hNGoodGeomTracksPerEvent,"Max")->Draw();
  
  mpad6->Update();
  //-----------
  mpad7->cd();
  //  if(MimosaType==5){
  //    h2dgoodhits->SetAxisRange(-7000,7000,"X");
  //    h2dgoodhits->SetAxisRange(-7000,7000,"Y");
  //  }else{
  //    h2dgoodhits->SetAxisRange(-650,650,"X");
  //    h2dgoodhits->SetAxisRange(-700,60,"Y");
  //  }
  AutoZoom(h2dgoodhits)->Draw("colz");
  
  mpad7->Update();

  

  //-----------
  mpad8->cd();
  mpad8->SetLogy(0);
  //  if(MimosaType==5){
  //    vec->SetAxisRange(-30,30,"X");
  //    vec->SetAxisRange(-30,30,"Y");
  //  }else{
  //  }
  AutoZoom(vec)->Draw("colz");
  
  mpad8->Update();
  //-----------
  mpad9->cd();
  mpad9->SetLogy(1);
  //  if(MimosaType==5){
  //    DuvCG->SetAxisRange(0,40,"X");
  //  }else{
  //  }
  if(DuvCG->GetMaximum()<=0){mpad9->SetLogy(0);}
  AutoZoom(DuvCG,"Max")->Draw();
  AutoZoom(hTrackToClusterMinDistance,"Max")->Draw("same"); // JB 2013/08/24
  mpad9->Update();
  
  gPad->Update();
  
  c3->cd();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    c3->Print(EPSNameO.Data());
    c3->Print(EPSName.Data());
    c3->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  c3_2 = new TCanvas("ctrackDentistyQuantities","Track density quantities",850,10,700,900);
  c3_2->Draw();
  c3_2->Clear();
  c3_2->cd();
  c3_2->Divide(2,2);
  c3_2->cd(1);

  double Max_effic, Min_effic, porcent_effic,MaxRange;

  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  MaxRange = -999.0;
  for(int i=0;i<hnTracksinSensorPerEvent->GetXaxis()->GetNbins();i++) {
    int index = hnTracksinSensorPerEvent->GetXaxis()->GetNbins() - i;
    if(hnTracksinSensorPerEvent->GetBinContent(index) > 0.0) {
      MaxRange  = hnTracksinSensorPerEvent->GetBinCenter(index);
      MaxRange += 0.5*hnTracksinSensorPerEvent->GetBinWidth(index);
      break;
    }
  }
  MaxRange += 0.10*(MaxRange - hnTracksinSensorPerEvent->GetXaxis()->GetXmin());
  hnTracksinSensorPerEvent->SetAxisRange(hnTracksinSensorPerEvent->GetXaxis()->GetXmin(),
					 MaxRange,
					 "X");
  hnTracksinSensorPerEvent->Draw();
  c3_2->cd(2);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  MaxRange = -999.0;
  for(int i=0;i<hTrackDensity->GetXaxis()->GetNbins();i++) {
    int index = hTrackDensity->GetXaxis()->GetNbins() - i;
    if(hTrackDensity->GetBinContent(index) > 0.0) {
      MaxRange  = hTrackDensity->GetBinCenter(index);
      MaxRange += 0.5*hTrackDensity->GetBinWidth(index);
      break;
    }
  }
  MaxRange += 0.10*(MaxRange - hTrackDensity->GetXaxis()->GetXmin());
  hTrackDensity->SetAxisRange(hTrackDensity->GetXaxis()->GetXmin(),
			      MaxRange,
			      "X");
  hTrackDensity->Draw();
  c3_2->cd(3);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  MaxRange = -999.0;
  for(int i=0;i<hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();i++) {
    int index = hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins() - i;
    if(hnTracksInGeomatrixVsTrackPerEvent->GetBinContent(index) > 0.0) {
      MaxRange  = hnTracksInGeomatrixVsTrackPerEvent->GetBinCenter(index);
      MaxRange += 0.5*hnTracksInGeomatrixVsTrackPerEvent->GetBinWidth(index);
      break;
    }
  }
  for(int i=0;i<hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();i++) {
    int index = hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins() - i;
    if(hnGOODhitInGeomatrixVsTrackPerEvent->GetBinContent(index) > 0.0) {
      MaxRange  = hnGOODhitInGeomatrixVsTrackPerEvent->GetBinCenter(index);
      MaxRange += 0.5*hnGOODhitInGeomatrixVsTrackPerEvent->GetBinWidth(index);
      break;
    }
  }
  MaxRange += 0.10*(MaxRange - hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetXmin());
  hnTracksInGeomatrixVsTrackPerEvent->SetAxisRange(hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetXmin(),
						   MaxRange,"X");
  hnGOODhitInGeomatrixVsTrackPerEvent->SetAxisRange(hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetXmin(),
						    MaxRange,"X");
  Max_effic = -1.0e+20;
  for(int i=0;i<hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();i++) {
    if(Max_effic < hnTracksInGeomatrixVsTrackPerEvent->GetBinContent(i+1))  Max_effic = hnTracksInGeomatrixVsTrackPerEvent->GetBinContent(i+1);
    if(Max_effic < hnGOODhitInGeomatrixVsTrackPerEvent->GetBinContent(i+1)) Max_effic = hnGOODhitInGeomatrixVsTrackPerEvent->GetBinContent(i+1);
  }
  porcent_effic = 0.10;
  hnTracksInGeomatrixVsTrackPerEvent->SetMaximum(Max_effic*(1.0 + porcent_effic));
  hnGOODhitInGeomatrixVsTrackPerEvent->SetMaximum(Max_effic*(1.0 + porcent_effic));
  hnTracksInGeomatrixVsTrackPerEvent->SetLineColor(4);
  hnTracksInGeomatrixVsTrackPerEvent->SetLineWidth(2);
  hnGOODhitInGeomatrixVsTrackPerEvent->SetLineColor(2);
  hnGOODhitInGeomatrixVsTrackPerEvent->SetLineWidth(2);
  hnTracksInGeomatrixVsTrackPerEvent->Draw();
  hnGOODhitInGeomatrixVsTrackPerEvent->Draw("same");
  leg_TracksinSensorPerEvent = new TLegend(0.7,0.6,0.8,0.7);
  leg_TracksinSensorPerEvent->SetFillColor(10);
  leg_TracksinSensorPerEvent->AddEntry(hnTracksInGeomatrixVsTrackPerEvent, "Tracks",   "l");
  leg_TracksinSensorPerEvent->AddEntry(hnGOODhitInGeomatrixVsTrackPerEvent,"GOOD hits","l");
  leg_TracksinSensorPerEvent->Draw("same");
  c3_2->cd(4);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  hEfficiencyInGeomatrixVsTrackPerEvent->SetAxisRange(hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetXmin(),
						      MaxRange,"X");
  Max_effic = -1.0e+20;
  Min_effic = +1.0e+20;
  for(int i=0;i<hEfficiencyInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();i++) {
    double v = hEfficiencyInGeomatrixVsTrackPerEvent->GetBinContent(i+1);
    double e = hEfficiencyInGeomatrixVsTrackPerEvent->GetBinError(i+1);
    if(v < 0.0) continue;
    if(Max_effic < v+e) Max_effic = v+e;
    if(Min_effic > v-e) Min_effic = v-e;
    
  }
  porcent_effic = 0.10;
  hEfficiencyInGeomatrixVsTrackPerEvent->SetMaximum(Max_effic + porcent_effic*(Max_effic-Min_effic));
  hEfficiencyInGeomatrixVsTrackPerEvent->SetMinimum(Min_effic - porcent_effic*(Max_effic-Min_effic));
  hEfficiencyInGeomatrixVsTrackPerEvent->Draw();
  l_average_Effic = new TLine(hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetXmin(),AverageEffic[0],
			      MaxRange,AverageEffic[0]);
  l_average_Effic->SetLineColor(2);
  l_average_Effic->SetLineWidth(2);
  l_average_Effic->SetLineStyle(1);
  l_average_Effic_PErr = new TLine(hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetXmin(),AverageEffic[0] + AverageEffic[1],
				   MaxRange,AverageEffic[0] + AverageEffic[1]);
  l_average_Effic_PErr->SetLineColor(2);
  l_average_Effic_PErr->SetLineWidth(2);
  l_average_Effic_PErr->SetLineStyle(2);
  l_average_Effic_MErr = new TLine(hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetXmin(),AverageEffic[0] - AverageEffic[1],
				   MaxRange,AverageEffic[0] - AverageEffic[1]);
  l_average_Effic_MErr->SetLineColor(2);
  l_average_Effic_MErr->SetLineWidth(2);
  l_average_Effic_MErr->SetLineStyle(2);

  l_average_Effic->Draw();
  l_average_Effic_PErr->Draw();
  l_average_Effic_MErr->Draw();
  
  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    c3_2->Print(EPSNameO.Data());
    c3_2->Print(EPSName.Data());
    c3_2->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }


  //--------------------------------
  // ---> 3rd canvas.
  //  Track properties
  //--------------------------------
  
  c4_2 = new TCanvas("ctrackproperties","Track properties",100,10,700,900);
  c4_2->Draw();
  c4_2->Clear();
  c4_2->SetBorderMode(0);
  
  // --- build the screen ---:
  gPad->cd();
  c4_2->Clear();
  
  
  TPad* mpad3_1 = new TPad("mpad3_1","",0.01,0.01,0.32,0.30);
  mpad3_1->SetGrid(1);
  mpad3_1->Draw();
  
  TPad* mpad3_2 = new TPad("mpad3_2","",0.33,0.01,0.66,0.30);
  mpad3_2->SetGrid(1);
  mpad3_2->Draw();
  
  TPad* mpad3_3 = new TPad("mpad3_3","title 1",0.67,0.01,0.99,0.30);
  mpad3_3->SetGrid(1);
  mpad3_3->Draw();
  
  TPad* mpad3_4 = new TPad("mpad3_4","",0.01,0.31,0.32,0.60);
  mpad3_4->SetGrid(1);
  mpad3_4->Draw();
  
  TPad* mpad3_5 = new TPad("mpad3_5","",0.33,0.31,0.66,0.60);
  mpad3_5->SetGrid(1);
  mpad3_5->Draw();
  
  TPad* mpad3_6 = new TPad("mpad3_6","",0.67,0.31,0.99,0.60);
  mpad3_6->SetGrid(1);
  mpad3_6->Draw();
  
  TPad* mpad3_7 = new TPad("mpad3_7","",0.01,0.61,0.32,0.9);
  mpad3_7->SetGrid(1);
  mpad3_7->Draw();
  
  TPad* mpad3_8 = new TPad("mpad3_8","",0.33,0.61,0.66,0.9);
  mpad3_8->SetGrid(1);
  mpad3_8->Draw();
  
  TPad* mpad3_9 = new TPad("mpad3_9","",0.67,0.61,0.99,0.9);
  mpad3_9->SetGrid(1);
  mpad3_9->Draw();
  
  TPad* mpad3_title = new TPad("mpad3_title","",0.10,0.91,0.85,0.99);
  mpad3_title->SetGrid(1);
  mpad3_title->SetFillColor(19);
  mpad3_title->Draw();
  mpad3_title->cd();
  TText *ttt3 = new TText();
  ttt3->SetTextFont(25);
  ttt3->SetTextColor(9);
  ttt3->SetTextSize(.3);
  ttt3->SetTextAlign(12);
  ttt3->SetTextFont(22);
  Char_t Header3[100];
  //---ab
  sprintf(Header3,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
  ttt->DrawText( 0.05,0.6,Header3);
  
  mpad3_1->SetFillColor(19);
  mpad3_2->SetFillColor(19);
  mpad3_3->SetFillColor(19);
  mpad3_4->SetFillColor(19);
  mpad3_5->SetFillColor(19);
  mpad3_6->SetFillColor(19);
  mpad3_7->SetFillColor(19);
  mpad3_8->SetFillColor(19);
  mpad3_9->SetFillColor(19);
  
  
  // Formula for expected nb of tracks with a given nb of hits (x),
  // if all plane have the same det. eff.
  //  [0] = TOTAL nb of planes
  //  [1] = det. eff.
  //  [2] = total nb of tracks
  //TF1* fNhitsPerTrack = new TF1("fnhitspertrack","[2]*pow([1],x)*pow(1-[1],[0]-x)*TMath::Factorial([0])/TMath::Factorial(x)/TMath::Factorial([0]-x)",0, 11);
  //fNhitsPerTrack->SetParameters( 4, 1., hNhitsPerTrack_all->GetEntries());

  
  //-----------
  mpad3_1->cd();
  mpad3_1->SetLogy(0);
  //AutoZoom(hAllTvTu)->Draw("colz");
  hAllTvTu->Draw("colz");
  mpad3_1->Update();
  //-----------
  mpad3_2->cd();
  mpad3_2->SetLogy(0);
  //AutoZoom(hGoodChi2TvTu)->Draw("colz");
  hGoodChi2TvTu->Draw("colz");
  mpad3_2->Update();
  //-----------
  mpad3_3->cd();
  AutoZoom(hNTracksPerEvent)->Draw();
  mpad3_3->Update();
  //-----------
  mpad3_4->cd();
  AutoZoom(hchi2,"Max",80)->Draw();
  AutoZoom(hchi2_c,"Max")->Draw("same");
  mpad3_4->Update();
  //-----------
  mpad3_5->cd();
  //mpad3_5->SetLogy(0);
  AutoZoom(hchi2_nc,"Max")->Draw();
  mpad3_5->Update();
  //-----------
  mpad3_6->cd();
  hNhitsPerTrack_all->Draw(); // JB 2014/04/01
  hNhitsPerTrack_matched->Draw("same");
  mpad3_6->Update();
  //-----------
  mpad3_7->cd();
  mpad3_7->SetLogy(0);
  AutoZoom(hdistchi2)->Draw("colz");
  mpad3_7->Update();
  //-----------
  mpad3_8->cd();
  mpad3_8->SetLogy(0);
  AutoZoom(hGoodChi2AngleXZ)->Draw();
  mpad3_8->Update();
  //-----------
  mpad3_9->cd();
  mpad3_9->SetLogy(0);
  AutoZoom(hGoodChi2AngleYZ)->Draw();
  mpad3_9->Update();
  
  gPad->Update();
  
  c4_2->cd();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    c4_2->Print(EPSNameO.Data());
    c4_2->Print(EPSName.Data());
    c4_2->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }
  
  //--------------------------------
  //--------------WRITE IN A ROOT FILE:
  
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileName[50];
  sprintf(ResultFileName,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName,"%s", fTool.LocalizeDirName( ResultFileName)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName,"UPDATE");
  //---histos:
  htuhtv->Write();
  htu->Write();
  htv->Write();
  huvBad->Write();
  FalseHitMap->Write();
  hAllHitsInPixel->Write();
  hNGoodGeomTracksPerEvent->Write();
  
  h2dmatchedhits->Write();
  h2DgoodSeedPixel->Write();
  vec->Write();
  DuvCG->Write();
  hAllTvTu->Write();
  hAllTu->Write();
  hAllTv->Write();
  hGoodChi2TvTu->Write();
  hnhit->Write();
  //hgoodSN_vs_SNN->Write();
  //hallSN_vs_SNN->Write();
  //hallSN_minus_hgoodSN_vs_SNN->Write();
  hSNReal->Write();
  hSNNReal->Write();
  hSN_vs_SNNReal->Write();
  
  // JB 2009/09/01
  hoptimalsize->Write();
  hnpix->Write();
  hnpix_c->Write();
  hnpix_nc->Write();
  hnWindows_c->Write();
  
  hNhitsPerTrack_all->Write(); // JB 2014/04/01
  hNhitsPerTrack_good->Write();
  hNhitsPerTrack_matched->Write();

  hnTracksinSensorPerEvent->Write();
  hTrackDensity->Write();
  hnTracksInGeomatrixVsTrackPerEvent->Write();
  hnGOODhitInGeomatrixVsTrackPerEvent->Write();
  hEfficiencyInGeomatrixVsTrackPerEvent->Write();
  
  //canvas:
  c3->Write();
  c3_2->Write();
  c4_2->Write();
  
  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();
  
  CheckClusters(); // replace part on cluster properties, JB 2013/10/29
  
}

//________________________________________________________________________________________
//
void MimosaAnalysis::InspectEfficiency()
{
  //inspect Efficiency by samples of N events.
  //
  // Modified: MG 2011/07/08 for efficiency map
  // Modified: JB 2011/11/23 add MiniVector condition
  
  if(!CheckIfDone("mimosapro") && !CheckIfDone("mimosaminivectors")) return;
  
  char fOutName[200];
  sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //sprintf(tmpFile,"%s",fTool.LocalizeDirName(tmpFile));
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");

  UsedMacro[4] = 1;
  ceffi = new TCanvas("ceffi","Inspect efficiency",550,10,750,550);
  ceffi->Draw();
  //ceffi->Clear();
  ceffi->Divide(2,2);
  //_1
  ceffi->cd(1);
  if(!greff->GetN()) {Warning("InspectEfficiency()","No entry in greff, did you run over enough events?"); return;}
  greff->Draw("ALP");
  
  //_2
  ceffi->cd(2);
  grnum->SetLineColor(2);
  grnum->SetTitle("Num of tracks");
  grnum->SetFillStyle(0);
  grnum->Draw("ALP");
  grevt->SetLineColor(4);
  grevt->Draw("same");
  
  //_3
  ceffi->cd(3);
  AutoZoom(effimap)->Draw("colz");
  effimap->SetMinimum(MimosaEfficiency-50*MimosaEfficiency_ERR); //MG 2011/07/08
  
  //_4
  ceffi->cd(4);
  ceffi->cd(4)->SetTickx(1);
  ceffi->cd(4)->SetTicky(1);
  double Maximum_effi = -1.0e+20;
  double Minimum_effi = +1.0e+20;
  for(int i=0;i<effi_vs_TrkHitDist->GetXaxis()->GetNbins();i++) {
    double v = effi_vs_TrkHitDist->GetBinContent(i+1);
    double e = effi_vs_TrkHitDist->GetBinError(i+1);
    if(Maximum_effi < v+e) Maximum_effi = v+e;
    if(Minimum_effi > v-e) Minimum_effi = v-e;

    v = effiCorr_vs_TrkHitDist->GetBinContent(i+1);
    e = effiCorr_vs_TrkHitDist->GetBinError(i+1);
    if(Maximum_effi < v+e) Maximum_effi = v+e;
    if(Minimum_effi > v-e) Minimum_effi = v-e;
  }
  effi_vs_TrkHitDist->SetMaximum(Maximum_effi + 0.10*(Maximum_effi - Minimum_effi));
  effi_vs_TrkHitDist->SetMinimum(Minimum_effi - 0.10*(Maximum_effi - Minimum_effi));
  TLine* l_effi_vs_TrKHitDist_1 = new TLine(TrackToHitDistanceLimit,effi_vs_TrkHitDist->GetBinContent(effi_vs_TrkHitDist->FindBin(TrackToHitDistanceLimit)),
					    TrackToHitDistanceLimit,effi_vs_TrkHitDist->GetMinimum());
  l_effi_vs_TrKHitDist_1->SetLineColor(2);
  l_effi_vs_TrKHitDist_1->SetLineWidth(2);
  l_effi_vs_TrKHitDist_1->SetLineStyle(2);
  TLine* l_effi_vs_TrKHitDist_2 = new TLine(effi_vs_TrkHitDist->GetXaxis()->GetXmin(),effi_vs_TrkHitDist->GetBinContent(effi_vs_TrkHitDist->FindBin(TrackToHitDistanceLimit)),
					    TrackToHitDistanceLimit,effi_vs_TrkHitDist->GetBinContent(effi_vs_TrkHitDist->FindBin(TrackToHitDistanceLimit)));
  l_effi_vs_TrKHitDist_2->SetLineColor(2);
  l_effi_vs_TrKHitDist_2->SetLineWidth(2);
  l_effi_vs_TrKHitDist_2->SetLineStyle(2);
  effi_vs_TrkHitDist->Draw(); //AP, 2015/01/13
  effiCorr_vs_TrkHitDist->Draw("same"); //AP, 2015/01/13
  l_effi_vs_TrKHitDist_1->Draw();
  l_effi_vs_TrKHitDist_2->Draw();

  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    ceffi->Print(EPSNameO.Data());
    ceffi->Print(EPSName.Data());
    ceffi->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");
  greff->Write();
  grnum->Write();
  grevt->Write();
  effimap->Write();
  goodtracks->Write();  
  ceffi->Write();
  effi_vs_TrkHitDist->Write();
  effiCorr_vs_TrkHitDist->Write();
  hNomEffic->Write();

  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;

  MHist::Dir();
}


//_________________________________________________________________________________________
//
void MimosaAnalysis::MimosaResolution()
{

  // display the spacial resolution of a given chip
  //
  // Modified JB 2009/09/14  to show all the fits
  // Modified JB 2010/07/22  add AHT algo
  // Modified JB 2010/09/10  add 2nd digital algo and CG full-cluster
  // Modified SS 2011/12/05 Results of the huCG and huCG5 methods are saved to the CSV file opened by MimosaPro.	

  if(!CheckIfDone("mimosapro")) return;

  bool SaveAllMethods = false;

  char fOutName[200];
  sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //sprintf(tmpFile,"%s",fTool.LocalizeDirName(tmpFile));
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");

  UsedMacro[0] = 1;

  Int_t iPointNumber=0;
  Int_t NumberOfResolutionPoints=30;
  ResolutionPoint = new Float_t[NumberOfResolutionPoints];
  Float_t* ErrorOnResolutionPoint = new Float_t[NumberOfResolutionPoints];
  Char_t **algorithmName = new Char_t*[NumberOfResolutionPoints];
  for( Int_t iRes=0; iRes<NumberOfResolutionPoints; iRes++) {
    algorithmName[iRes] = new Char_t[50];
  }
  cresfit = new TCanvas*[NumberOfResolutionPoints];
  Char_t cname[20];
  Char_t ctitle[100];

  gStyle->SetOptFit(1);


  // Method 1
  //if(MimoDebug) cout<<" resolution method : "<<->GetTitle()<<endl;
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hAllHuvsAllTu1->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  AutoZoom(hAllHuvsAllTu1,"All")->Draw();
  sprintf( algorithmName[iPointNumber], "%s", hAllHuvsAllTu1->GetTitle());
  ResolutionPoint[iPointNumber]=hAllHuvsAllTu1->GetRMS();
  ErrorOnResolutionPoint[iPointNumber]=hAllHuvsAllTu1->GetRMSError();
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  AutoZoom(hAllHvvsAllTv1,"All")->Draw();
  sprintf( algorithmName[iPointNumber], "%s", hAllHvvsAllTv1->GetTitle());
  ResolutionPoint[iPointNumber]=hAllHvvsAllTv1->GetRMS();
  ErrorOnResolutionPoint[iPointNumber]=hAllHvvsAllTv1->GetRMSError();
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 2
  // JB 2010/09/10
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,huCGtu2->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  huCGtu2->SetFillColor(19);
  huCGtu2->SetLineWidth(2);
  huCGtu2->SetLineColor(1);
  huCGtu2->Draw();
  if( huCGtu2->Integral()>10. ) {
    huCGtu2->Fit("gaus");
    TF1 *fitgs2 =huCGtu2->GetFunction("gaus"); // Take parameters of the fit 
    fitgs2->SetLineColor(3);
    sprintf( algorithmName[iPointNumber], "%s", huCGtu2->GetTitle());
    ResolutionPoint[iPointNumber]=fitgs2->GetParameter(2);
    ErrorOnResolutionPoint[iPointNumber]=fitgs2->GetParError(2);    
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hvCGtv2->SetFillColor(19);
  hvCGtv2->SetLineWidth(2);
  hvCGtv2->SetLineColor(1);
  hvCGtv2->Draw();
  if( hvCGtv2->Integral()>10. ) {
    hvCGtv2->Fit("gaus");
    TF1 *fitg2 =hvCGtv2->GetFunction("gaus"); // Take parameters of the fit 
    fitg2->SetLineColor(3);
    sprintf( algorithmName[iPointNumber], "%s", hvCGtv2->GetTitle());
    ResolutionPoint[iPointNumber]=fitg2->GetParameter(2);
    ErrorOnResolutionPoint[iPointNumber]=fitg2->GetParError(2);
  }    
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 3
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,huCGtu1->GetTitle());
  int TheCGPointNumber = iPointNumber;
  double RangeAxis_Residuals = 50.0;
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  cresfit[iPointNumber]->cd(1)->SetTickx(1);
  cresfit[iPointNumber]->cd(1)->SetTicky(1);
  huCGtu1->SetFillColor(19);
  huCGtu1->SetLineWidth(2);
  huCGtu1->SetLineColor(4);
  huCGtu1->SetLineWidth(2);
  huCGtu1->SetMarkerColor(4);
  huCGtu1->SetMarkerSize(0.5);
  huCGtu1->SetMarkerStyle(20);
  huCGtu1->SetAxisRange(-RangeAxis_Residuals,+RangeAxis_Residuals,"X");
  huCGtu1->Sumw2();
  huCGtu1->Draw();
  if(huCGtu1->Integral()>10.) {
    if(!fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
      huCGtu1->Fit("gaus");
    }
    TF1 *fitgs =huCGtu1->GetFunction("gaus"); // Take parameters of the fit 
    fitgs->SetLineColor(2);
    sprintf( algorithmName[iPointNumber], "%s", huCGtu1->GetTitle());
    ResolutionPoint[iPointNumber]=fitgs->GetParameter(2);
    ErrorOnResolutionPoint[iPointNumber]=fitgs->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  cresfit[iPointNumber-1]->cd(2)->SetTickx(1);
  cresfit[iPointNumber-1]->cd(2)->SetTicky(1);
  hvCGtv1->SetFillColor(19);
  hvCGtv1->SetLineWidth(2);
  hvCGtv1->SetLineColor(4);
  hvCGtv1->SetLineWidth(2);
  hvCGtv1->SetMarkerColor(4);
  hvCGtv1->SetMarkerSize(0.5);
  hvCGtv1->SetMarkerStyle(20);
  hvCGtv1->SetAxisRange(-RangeAxis_Residuals,+RangeAxis_Residuals,"X");
  hvCGtv1->Sumw2();
  hvCGtv1->Draw();
  if(hvCGtv1->Integral()>10.) {
    if(!fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
      hvCGtv1->Fit("gaus");
    }
    TF1 *fitg =hvCGtv1->GetFunction("gaus"); // Take parameters of the fit 
    fitg->SetLineColor(2);
    sprintf( algorithmName[iPointNumber], "%s", hvCGtv1->GetTitle());
    ResolutionPoint[iPointNumber]=fitg->GetParameter(2);
    ErrorOnResolutionPoint[iPointNumber]=fitg->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  double ResolutionPointU_CGDSF[huCGwidth_vs_Mult->GetXaxis()->GetNbins()];
  double ErrResolutionPointU_CGDSF[huCGwidth_vs_Mult->GetXaxis()->GetNbins()];
  double ResolutionPointV_CGDSF[huCGwidth_vs_Mult->GetXaxis()->GetNbins()];
  double ErrResolutionPointV_CGDSF[huCGwidth_vs_Mult->GetXaxis()->GetNbins()];

  //AP, 2014/10/23
  //Plotting the residual for the CG(DSF) method for different multiplicities
  cresfit_CG_vs_Mult = new TCanvas*[huCGwidth_vs_Mult->GetXaxis()->GetNbins()];
  for(int imult=0;imult<huCGwidth_vs_Mult->GetXaxis()->GetNbins();imult++) {
    TString CanvasName,CanvasTitle;
    CanvasName   = TString("cresfit_CGDSF_mult") + long(imult+1);
    CanvasTitle  = TString("Run ") + long(RunNumber) + TString(" Plane ") + long(ThePlaneNumber);
    CanvasTitle += TString(" - Method ") + long(iPointNumber/2+1) + TString(" - ") + TString(huCGtu1->GetTitle());
    CanvasTitle += TString(", for mult = ") + long(mult+1);
    cresfit_CG_vs_Mult[imult] = new TCanvas(CanvasName.Data(),CanvasTitle.Data(),400,400);
    cresfit_CG_vs_Mult[imult]->Divide(2,1);

    cresfit_CG_vs_Mult[imult]->cd(1);
    cresfit_CG_vs_Mult[imult]->cd(1)->SetTickx(1);
    cresfit_CG_vs_Mult[imult]->cd(1)->SetTicky(1);
    huCGtu1_vs_Mult[imult]->SetFillColor(19);
    huCGtu1_vs_Mult[imult]->SetLineWidth(2);
    huCGtu1_vs_Mult[imult]->SetLineColor(4);
    huCGtu1_vs_Mult[imult]->SetLineWidth(2);
    huCGtu1_vs_Mult[imult]->SetMarkerColor(4);
    huCGtu1_vs_Mult[imult]->SetMarkerSize(0.5);
    huCGtu1_vs_Mult[imult]->SetMarkerStyle(20);
    huCGtu1_vs_Mult[imult]->SetAxisRange(-RangeAxis_Residuals,+RangeAxis_Residuals,"X");
    huCGtu1_vs_Mult[imult]->Sumw2();
    huCGtu1_vs_Mult[imult]->Draw();
    if( huCGtu1_vs_Mult[imult]->Integral()>10. ) {
      huCGtu1_vs_Mult[imult]->Fit("gaus");
      TF1 *fitg =huCGtu1_vs_Mult[imult]->GetFunction("gaus"); // Take parameters of the fit 
      fitg->SetLineColor(2);

      huCGmean_vs_Mult->SetBinContent(imult+1,huCGtu1_vs_Mult[imult]->GetMean());
      huCGmean_vs_Mult->SetBinError(imult+1,  huCGtu1_vs_Mult[imult]->GetMeanError());

      ResolutionPointU_CGDSF[imult]    = fitg->GetParameter(2);
      ErrResolutionPointU_CGDSF[imult] = fitg->GetParError(2);

      if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
	ResolutionPointU_CGDSF[imult]     = sqrt(pow(fitg->GetParameter(2),2) - pow(Tel_resolutionU_CGDSF[0],2));
	ErrResolutionPointU_CGDSF[imult]  = pow(fitg->GetParameter(2)*fitg->GetParError(2),2);
	ErrResolutionPointU_CGDSF[imult] += pow(Tel_resolutionU_CGDSF[0]*Tel_resolutionU_CGDSF[1],2);
	ErrResolutionPointU_CGDSF[imult]  = sqrt(ErrResolutionPointU_CGDSF[imult])/ResolutionPointU_CGDSF[imult];
      }

      huCGwidth_vs_Mult->SetBinContent(imult+1,ResolutionPointU_CGDSF[imult]);
      huCGwidth_vs_Mult->SetBinError(imult+1,  ErrResolutionPointU_CGDSF[imult]);
    }
    else {
      huCGwidth_vs_Mult->SetBinContent(imult+1,-999.0);
      huCGwidth_vs_Mult->SetBinError(imult+1,1.0e-6);
      huCGmean_vs_Mult->SetBinContent(imult+1,-999.0);
      huCGmean_vs_Mult->SetBinError(imult+1,1.0e-6);

      ResolutionPointU_CGDSF[imult]    = 0.0;
      ErrResolutionPointU_CGDSF[imult] = 0.0;
    }
    //
    cresfit_CG_vs_Mult[imult]->cd(2);
    cresfit_CG_vs_Mult[imult]->cd(2)->SetTickx(1);
    cresfit_CG_vs_Mult[imult]->cd(2)->SetTicky(1);
    hvCGtv1_vs_Mult[imult]->SetFillColor(19);
    hvCGtv1_vs_Mult[imult]->SetLineWidth(2);
    hvCGtv1_vs_Mult[imult]->SetLineColor(4);
    hvCGtv1_vs_Mult[imult]->SetLineWidth(2);
    hvCGtv1_vs_Mult[imult]->SetMarkerColor(4);
    hvCGtv1_vs_Mult[imult]->SetMarkerSize(0.5);
    hvCGtv1_vs_Mult[imult]->SetMarkerStyle(20);
    hvCGtv1_vs_Mult[imult]->SetAxisRange(-RangeAxis_Residuals,+RangeAxis_Residuals,"X");
    hvCGtv1_vs_Mult[imult]->Sumw2();
    hvCGtv1_vs_Mult[imult]->Draw();
    if( hvCGtv1_vs_Mult[imult]->Integral()>10. ) {
      hvCGtv1_vs_Mult[imult]->Fit("gaus");
      TF1 *fitg =hvCGtv1_vs_Mult[imult]->GetFunction("gaus"); // Take parameters of the fit 
      fitg->SetLineColor(2);
      hvCGmean_vs_Mult->SetBinContent(imult+1,hvCGtv1_vs_Mult[imult]->GetMean());
      hvCGmean_vs_Mult->SetBinError(imult+1,  hvCGtv1_vs_Mult[imult]->GetMeanError());

      ResolutionPointV_CGDSF[imult]    = fitg->GetParameter(2);
      ErrResolutionPointV_CGDSF[imult] = fitg->GetParError(2);

      if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
	ResolutionPointV_CGDSF[imult]     = sqrt(pow(fitg->GetParameter(2),2) - pow(Tel_resolutionV_CGDSF[0],2));
	ErrResolutionPointV_CGDSF[imult]  = pow(fitg->GetParameter(2)*fitg->GetParError(2),2);
	ErrResolutionPointV_CGDSF[imult] += pow(Tel_resolutionV_CGDSF[0]*Tel_resolutionV_CGDSF[1],2);
	ErrResolutionPointV_CGDSF[imult]  = sqrt(ErrResolutionPointV_CGDSF[imult])/ResolutionPointV_CGDSF[imult];
      }

      hvCGwidth_vs_Mult->SetBinContent(imult+1,ResolutionPointV_CGDSF[imult]);
      hvCGwidth_vs_Mult->SetBinError(imult+1,  ErrResolutionPointV_CGDSF[imult]);
    }
    else {
      hvCGwidth_vs_Mult->SetBinContent(imult+1,-999.0);
      hvCGwidth_vs_Mult->SetBinError(imult+1,   1.0e-6);
      hvCGmean_vs_Mult->SetBinContent(imult+1, -999.0);
      hvCGmean_vs_Mult->SetBinError(imult+1,    1.0e-6);

      ResolutionPointV_CGDSF[imult]    = 0.0;
      ErrResolutionPointV_CGDSF[imult] = 0.0;
    }

    if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
      NPages++;
      TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
      TString EPSNameO = EPSName + TString("[");
      TString EPSNameC = EPSName + TString("]");
    
      cresfit_CG_vs_Mult[imult]->Print(EPSNameO.Data());
      cresfit_CG_vs_Mult[imult]->Print(EPSName.Data());
      cresfit_CG_vs_Mult[imult]->Print(EPSNameC.Data());
      //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
      //gSystem->Exec(command.Data());
      //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
      //gSystem->Exec(command.Data());
      //command = TString("rm -rf ") + EPSName;
      //gSystem->Exec(command.Data());
    }
  }

  cresfit_CGWidth_vs_Mult = new TCanvas("cresfit_CGWidth_vs_Mult","Width of residual of CG(DSF) mwthod vs cluster multiplicity",400,400);
  cresfit_CGWidth_vs_Mult->SetTickx(1);
  cresfit_CGWidth_vs_Mult->SetTicky(1);
  cresfit_CGWidth_vs_Mult->SetGridx(1);
  cresfit_CGWidth_vs_Mult->SetGridy(1);
  huCGwidth_vs_Mult->SetLineColor(4);
  huCGwidth_vs_Mult->SetLineWidth(2);
  huCGwidth_vs_Mult->SetMarkerStyle(20);
  huCGwidth_vs_Mult->SetMarkerSize(0.7);
  huCGwidth_vs_Mult->SetMarkerColor(4);
  hvCGwidth_vs_Mult->SetLineColor(2);
  hvCGwidth_vs_Mult->SetLineWidth(2);
  hvCGwidth_vs_Mult->SetMarkerStyle(20);
  hvCGwidth_vs_Mult->SetMarkerSize(0.7);
  hvCGwidth_vs_Mult->SetMarkerColor(2);
  if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
    huCGwidth_vs_Mult->SetTitle("#sigma_{sp} from CG(DSF) vs cluster multiplicity");
    hvCGwidth_vs_Mult->SetTitle("#sigma_{sp} from CG(DSF) vs cluster multiplicity");

    huCGwidth_vs_Mult->SetYTitle("#sigma_{sp}(#mum)");
    hvCGwidth_vs_Mult->SetYTitle("#sigma_{sp}(#mum)");
  }
  double Maximum = -1.0e+20;
  double Minimum = +1.0e+20;
  for(int imult=0;imult<huCGwidth_vs_Mult->GetXaxis()->GetNbins();imult++) {
    double v,e;

    v = huCGwidth_vs_Mult->GetBinContent(imult+1);
    e = huCGwidth_vs_Mult->GetBinError(imult+1);
    if(v > 0.0) {
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }

    v = hvCGwidth_vs_Mult->GetBinContent(imult+1);
    e = hvCGwidth_vs_Mult->GetBinError(imult+1);
    if(v > 0.0) {
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }
    
  }
  double porcent = 0.30;
  huCGwidth_vs_Mult->SetMaximum(Maximum + porcent*(Maximum-Minimum));
  hvCGwidth_vs_Mult->SetMaximum(Maximum + porcent*(Maximum-Minimum));
  porcent = 0.10;
  huCGwidth_vs_Mult->SetMinimum(Minimum - porcent*(Maximum-Minimum));
  hvCGwidth_vs_Mult->SetMinimum(Minimum - porcent*(Maximum-Minimum));
  huCGwidth_vs_Mult->Draw();
  hvCGwidth_vs_Mult->Draw("same");
  leg_CGWidth_vs_Mult = new TLegend(0.15,0.75,0.35,0.9);
  leg_CGWidth_vs_Mult->SetFillColor(10);
  leg_CGWidth_vs_Mult->AddEntry(huCGwidth_vs_Mult,"U direction","l");
  leg_CGWidth_vs_Mult->AddEntry(hvCGwidth_vs_Mult,"V direction","l");
  leg_CGWidth_vs_Mult->Draw("same");

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cresfit_CGWidth_vs_Mult->Print(EPSNameO.Data());
    cresfit_CGWidth_vs_Mult->Print(EPSName.Data());
    cresfit_CGWidth_vs_Mult->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }


  cresfit_CGMean_vs_Mult = new TCanvas("cresfit_CGMean_vs_Mult","Mean of residual of CG(DSF) mwthod vs cluster multiplicity",400,400);
  cresfit_CGMean_vs_Mult->SetTickx(1);
  cresfit_CGMean_vs_Mult->SetTicky(1);
  cresfit_CGMean_vs_Mult->SetGridx(1);
  cresfit_CGMean_vs_Mult->SetGridy(1);
  huCGmean_vs_Mult->SetLineColor(4);
  huCGmean_vs_Mult->SetLineWidth(2);
  huCGmean_vs_Mult->SetMarkerStyle(20);
  huCGmean_vs_Mult->SetMarkerSize(0.7);
  huCGmean_vs_Mult->SetMarkerColor(4);
  hvCGmean_vs_Mult->SetLineColor(2);
  hvCGmean_vs_Mult->SetLineWidth(2);
  hvCGmean_vs_Mult->SetMarkerStyle(20);
  hvCGmean_vs_Mult->SetMarkerSize(0.7);
  hvCGmean_vs_Mult->SetMarkerColor(2);
  Maximum = -1.0e+20;
  Minimum = +1.0e+20;
  for(int imult=0;imult<huCGmean_vs_Mult->GetXaxis()->GetNbins();imult++) {
    double v,e;

    v = huCGmean_vs_Mult->GetBinContent(imult+1);
    e = huCGmean_vs_Mult->GetBinError(imult+1);
    if(v > -999.0) {
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }

    v = hvCGmean_vs_Mult->GetBinContent(imult+1);
    e = hvCGmean_vs_Mult->GetBinError(imult+1);
    if(v > -999.0) {
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }
    
  }
  porcent = 0.30;
  huCGmean_vs_Mult->SetMaximum(Maximum + porcent*(Maximum-Minimum));
  hvCGmean_vs_Mult->SetMaximum(Maximum + porcent*(Maximum-Minimum));
  porcent = 0.10;
  huCGmean_vs_Mult->SetMinimum(Minimum - porcent*(Maximum-Minimum));
  hvCGmean_vs_Mult->SetMinimum(Minimum - porcent*(Maximum-Minimum));
  huCGmean_vs_Mult->Draw();
  hvCGmean_vs_Mult->Draw("same");
  leg_CGWidth_vs_Mult->Draw("same");

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cresfit_CGMean_vs_Mult->Print(EPSNameO.Data());
    cresfit_CGMean_vs_Mult->Print(EPSName.Data());
    cresfit_CGMean_vs_Mult->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

#if 0
  //AP, 2014/10/31
  //Plotting the residue for the CG(DSF) method for different values of tracks/event in sensor
  cresfit_CG_vs_TracksPerEvent = new TCanvas*[hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins()];
  for(int itrks=0;itrks<hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();itrks++) {
    if(hnTracksInGeomatrixVsTrackPerEvent->GetBinContent(itrks+1) < 100) continue;


    TString CanvasName,CanvasTitle;
    CanvasName   = TString("cresfit_CGDSF_TracksPerEvent") + long(hnTracksInGeomatrixVsTrackPerEvent->GetBinCenter(itrks+1));
    CanvasTitle  = TString("Run ") + long(RunNumber) + TString(" Plane ") + long(ThePlaneNumber);
    CanvasTitle += TString(" - Method ") + long(iPointNumber/2+1) + TString(" - ") + TString(huCGtu1->GetTitle());
    CanvasTitle += TString(", for mult = ") + long(mult+1);
    cresfit_CG_vs_TracksPerEvent[itrks] = new TCanvas(CanvasName.Data(),CanvasTitle.Data(),400,400);
    cresfit_CG_vs_TracksPerEvent[itrks]->Divide(2,1);

    cresfit_CG_vs_TracksPerEvent[itrks]->cd(1);
    cresfit_CG_vs_TracksPerEvent[itrks]->cd(1)->SetTickx(1);
    cresfit_CG_vs_TracksPerEvent[itrks]->cd(1)->SetTicky(1);
    huCGtu1_vs_TracksPerEvent[itrks]->SetFillColor(19);
    huCGtu1_vs_TracksPerEvent[itrks]->SetLineWidth(2);
    huCGtu1_vs_TracksPerEvent[itrks]->SetLineColor(4);
    huCGtu1_vs_TracksPerEvent[itrks]->SetLineWidth(2);
    huCGtu1_vs_TracksPerEvent[itrks]->SetMarkerColor(4);
    huCGtu1_vs_TracksPerEvent[itrks]->SetMarkerSize(0.5);
    huCGtu1_vs_TracksPerEvent[itrks]->SetMarkerStyle(20);
    huCGtu1_vs_TracksPerEvent[itrks]->SetAxisRange(-RangeAxis_Residuals,+RangeAxis_Residuals,"X");
    huCGtu1_vs_TracksPerEvent[itrks]->Sumw2();
    huCGtu1_vs_TracksPerEvent[itrks]->Draw();
    if( huCGtu1_vs_TracksPerEvent[itrks]->Integral()>100. ) {
      huCGtu1_vs_TracksPerEvent[itrks]->Fit("gaus");
      TF1 *fitgs =huCGtu1_vs_TracksPerEvent[itrks]->GetFunction("gaus"); // Take parameters of the fit 
      fitgs->SetLineColor(2);

      hResidueUInGeomatrixVsTrackPerEvent->SetBinContent(itrks+1,fitgs->GetParameter(2));
      hResidueUInGeomatrixVsTrackPerEvent->SetBinError(itrks+1,  fitgs->GetParError(2));
    }
    else {
      hResidueUInGeomatrixVsTrackPerEvent->SetBinContent(itrks+1,-999.0);
      hResidueUInGeomatrixVsTrackPerEvent->SetBinError(itrks+1,  1.0e-6);
    }
    //
    cresfit_CG_vs_TracksPerEvent[itrks]->cd(2);
    cresfit_CG_vs_TracksPerEvent[itrks]->cd(2)->SetTickx(1);
    cresfit_CG_vs_TracksPerEvent[itrks]->cd(2)->SetTicky(1);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetFillColor(19);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetLineWidth(2);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetLineColor(4);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetLineWidth(2);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetMarkerColor(4);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetMarkerSize(0.5);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetMarkerStyle(20);
    hvCGtv1_vs_TracksPerEvent[itrks]->SetAxisRange(-RangeAxis_Residuals,+RangeAxis_Residuals,"X");
    hvCGtv1_vs_TracksPerEvent[itrks]->Sumw2();
    hvCGtv1_vs_TracksPerEvent[itrks]->Draw();
    if( hvCGtv1_vs_TracksPerEvent[itrks]->Integral()>100. ) {
      hvCGtv1_vs_TracksPerEvent[itrks]->Fit("gaus");
      TF1 *fitg =hvCGtv1_vs_TracksPerEvent[itrks]->GetFunction("gaus"); // Take parameters of the fit 
      fitg->SetLineColor(2);

      hResidueVInGeomatrixVsTrackPerEvent->SetBinContent(itrks+1,fitg->GetParameter(2));
      hResidueVInGeomatrixVsTrackPerEvent->SetBinError(itrks+1,  fitg->GetParError(2));
    }
    else {
      hResidueVInGeomatrixVsTrackPerEvent->SetBinContent(itrks+1,-999.0);
      hResidueVInGeomatrixVsTrackPerEvent->SetBinError(itrks+1,  1.0e-6);
    }

    if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
      NPages++;
      TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
      TString EPSNameO = EPSName + TString("[");
      TString EPSNameC = EPSName + TString("]");
    
      cresfit_CG_vs_TracksPerEvent[itrks]->Print(EPSNameO.Data());
      cresfit_CG_vs_TracksPerEvent[itrks]->Print(EPSName.Data());
      cresfit_CG_vs_TracksPerEvent[itrks]->Print(EPSNameC.Data());
      //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
      //gSystem->Exec(command.Data());
      //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
      //gSystem->Exec(command.Data());
      //command = TString("rm -rf ") + EPSName;
      //gSystem->Exec(command.Data());
    }
  }


  cresfit_CGWidth_vs_TracksPerEvent = new TCanvas("cresfit_CGWidth_vs_TracksPerEvent",
						  "Width of residual of CG(DSF) method vs Tracks / event",400,400);
  cresfit_CGWidth_vs_TracksPerEvent->SetTickx(1);
  cresfit_CGWidth_vs_TracksPerEvent->SetTicky(1);
  cresfit_CGWidth_vs_TracksPerEvent->SetGridx(1);
  cresfit_CGWidth_vs_TracksPerEvent->SetGridy(1);
  hResidueUInGeomatrixVsTrackPerEvent->SetLineColor(4);
  hResidueUInGeomatrixVsTrackPerEvent->SetLineWidth(2);
  hResidueUInGeomatrixVsTrackPerEvent->SetMarkerStyle(20);
  hResidueUInGeomatrixVsTrackPerEvent->SetMarkerColor(4);
  hResidueVInGeomatrixVsTrackPerEvent->SetLineColor(2);
  hResidueVInGeomatrixVsTrackPerEvent->SetLineWidth(2);
  hResidueVInGeomatrixVsTrackPerEvent->SetMarkerStyle(20);
  hResidueVInGeomatrixVsTrackPerEvent->SetMarkerColor(2);
  Maximum = -1.0e+20;
  Minimum = +1.0e+20;
  for(int itrks=0;itrks<hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();itrks++) {
    double v,e;

    v = hResidueUInGeomatrixVsTrackPerEvent->GetBinContent(itrks+1);
    e = hResidueUInGeomatrixVsTrackPerEvent->GetBinError(itrks+1);
    if(v > 0.0) {
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }

    v = hResidueVInGeomatrixVsTrackPerEvent->GetBinContent(itrks+1);
    e = hResidueVInGeomatrixVsTrackPerEvent->GetBinError(itrks+1);
    if(v > 0.0) {
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }
    
  }
  porcent = 0.30;
  hResidueUInGeomatrixVsTrackPerEvent->SetMaximum(Maximum + porcent*(Maximum-Minimum));
  hResidueVInGeomatrixVsTrackPerEvent->SetMaximum(Maximum + porcent*(Maximum-Minimum));
  porcent = 0.10;
  hResidueUInGeomatrixVsTrackPerEvent->SetMinimum(Minimum - porcent*(Maximum-Minimum));
  hResidueVInGeomatrixVsTrackPerEvent->SetMinimum(Minimum - porcent*(Maximum-Minimum));

  double MyMaxRange = -999.0;
  for(int i=0;i<hnTracksinSensorPerEvent->GetXaxis()->GetNbins();i++) {
    int index = hnTracksinSensorPerEvent->GetXaxis()->GetNbins() - i;
    if(hnTracksinSensorPerEvent->GetBinContent(index) > 0.0) {
      MyMaxRange  = hnTracksinSensorPerEvent->GetBinCenter(index);
      MyMaxRange += 0.5*hnTracksinSensorPerEvent->GetBinWidth(index);
      break;
    }
  }
  MyMaxRange += 0.10*(MyMaxRange - hnTracksinSensorPerEvent->GetXaxis()->GetXmin());
  hResidueUInGeomatrixVsTrackPerEvent->SetAxisRange(hnTracksinSensorPerEvent->GetXaxis()->GetXmin(),
						    MyMaxRange,
						    "X");
  hResidueUInGeomatrixVsTrackPerEvent->Draw();
  hResidueVInGeomatrixVsTrackPerEvent->Draw("same");
  leg_CGWidth_vs_Mult->Draw("same");

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cresfit_CGWidth_vs_TracksPerEvent->Print(EPSNameO.Data());
    cresfit_CGWidth_vs_TracksPerEvent->Print(EPSName.Data());
    cresfit_CGWidth_vs_TracksPerEvent->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }
#endif

  // Method 4
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hCG2x2tu1->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hCG2x2tu1->SetFillColor(19);
  hCG2x2tu1->SetLineWidth(2);
  hCG2x2tu1->SetLineColor(kRed);
  hCG2x2tu1->Draw();
  if( hCG2x2tu1->Integral()>10. ) {
  hCG2x2tu1->Fit("gaus");
  TF1 *fitgCGU2x2 =hCG2x2tu1->GetFunction("gaus"); // Take parameters of the fit 
  fitgCGU2x2->SetLineColor(4);
  sprintf( algorithmName[iPointNumber], "%s", hCG2x2tu1->GetTitle());
  ResolutionPoint[iPointNumber]=fitgCGU2x2->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgCGU2x2->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hCG2x2tv1->SetFillColor(19);
  hCG2x2tv1->SetLineWidth(2);
  hCG2x2tv1->SetLineColor(kRed);
  hCG2x2tv1->Draw();
  if( hCG2x2tv1->Integral()>10. ) {
  hCG2x2tv1->Fit("gaus");
  TF1 *fitgCGV2x2 =hCG2x2tv1->GetFunction("gaus"); // Take parameters of the fit 
  fitgCGV2x2->SetLineColor(4);
  sprintf( algorithmName[iPointNumber], "%s", hCG2x2tv1->GetTitle());
  ResolutionPoint[iPointNumber]=fitgCGV2x2->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgCGV2x2->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 5
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hCG5URes->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hCG5URes->SetFillColor(19);
  hCG5URes->SetLineWidth(2);
  hCG5URes->SetLineColor(kRed);
  hCG5URes->Draw();
  if( hCG5URes->Integral()>10. ) {
  hCG5URes->Fit("gaus");
  TF1 *fitgCGU5 =hCG5URes->GetFunction("gaus"); // Take parameters of the fit 
  fitgCGU5->SetLineColor(4);
  sprintf( algorithmName[iPointNumber], "%s", hCG5URes->GetTitle());
  ResolutionPoint[iPointNumber]=fitgCGU5->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgCGU5->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hCG5VRes->SetFillColor(19);
  hCG5VRes->SetLineWidth(2);
  hCG5VRes->SetLineColor(kRed);
  hCG5VRes->Draw();
  if( hCG5VRes->Integral()>10. ) {
  hCG5VRes->Fit("gaus");
  TF1 *fitgCGV5 =hCG5VRes->GetFunction("gaus"); // Take parameters of the fit 
  fitgCGV5->SetLineColor(4);
  sprintf( algorithmName[iPointNumber], "%s", hCG5VRes->GetTitle());
  ResolutionPoint[iPointNumber]=fitgCGV5->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgCGV5->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 6
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hTuHuCorr->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hTuHuCorr->SetFillColor(19);
  hTuHuCorr->SetLineWidth(2);
  hTuHuCorr->SetLineColor(kRed);
  hTuHuCorr->Draw();
  if( hTuHuCorr->Integral()>10. ) {
  hTuHuCorr->Fit("gaus");
  TF1 *fitgCGUCorr =hTuHuCorr->GetFunction("gaus"); // Take parameters of the fit 
  fitgCGUCorr->SetLineColor(4);
  sprintf( algorithmName[iPointNumber], "%s", hTuHuCorr->GetTitle());
  ResolutionPoint[iPointNumber]=fitgCGUCorr->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgCGUCorr->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hTvHvCorr->SetFillColor(19);
  hTvHvCorr->SetLineWidth(2);
  hTvHvCorr->SetLineColor(kRed);
  hTvHvCorr->Draw();
  if( hTvHvCorr->Integral()>10. ) {
  hTvHvCorr->Fit("gaus");
  TF1 *fitgCGVCorr =hTvHvCorr->GetFunction("gaus"); // Take parameters of the fit 
  fitgCGVCorr->SetLineColor(4);
  sprintf( algorithmName[iPointNumber], "%s", hTvHvCorr->GetTitle());
  ResolutionPoint[iPointNumber]=fitgCGVCorr->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgCGVCorr->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 7
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hEtaURes->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hEtaURes->SetStats(1111);
  hEtaURes->SetLineWidth(2);
  hEtaURes->SetLineColor(kBlue);
  //----CDr,Nov 2007: fit range for residuals: mean+-2*sigma----------
  Double_t  hEtaUResRMS  = hEtaURes->GetRMS();
  Double_t  hEtaUResMEAN = hEtaURes->GetMean();
  Double_t  minU_my      =  hEtaUResMEAN-2*hEtaUResRMS;
  Double_t  maxU_my      =  hEtaUResMEAN+2*hEtaUResRMS;
  hEtaURes->Draw();
  if( hEtaURes->Integral()>10. ) {
  hEtaURes->Fit("gaus","","", minU_my, maxU_my);
  TF1 *fitg4 =hEtaURes->GetFunction("gaus"); // Take parameters of the fit 
  fitg4->SetLineColor(8);
  sprintf( algorithmName[iPointNumber], "%s", hEtaURes->GetTitle());
  ResolutionPoint[iPointNumber]=fitg4->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitg4->GetParError(2);
  }  
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hEtaVRes->SetStats(1111);
  //----CDr,Nov 2007: fit range for residuals: mean+-2*sigma----------
  Double_t  hEtaVResRMS  = hEtaVRes->GetRMS();
  Double_t  hEtaVResMEAN = hEtaVRes->GetMean();
  Double_t  minV_my      =  hEtaVResMEAN-2*hEtaVResRMS;
  Double_t  maxV_my      =  hEtaVResMEAN+2*hEtaVResRMS;
  hEtaVRes->SetLineWidth(2);
  hEtaVRes->SetLineColor(kBlue);
  hEtaVRes->Draw();
  if( hEtaVRes->Integral()>10. ) {
  hEtaVRes->Fit("gaus","","",minV_my, maxV_my);
  TF1 *fitg5 =hEtaVRes->GetFunction("gaus"); // Take parameters of the fit 
  fitg5->SetLineColor(8);
  sprintf( algorithmName[iPointNumber], "%s", hEtaVRes->GetTitle());
  ResolutionPoint[iPointNumber]=fitg5->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitg5->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 8
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hEta2x2tu1->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hEta2x2tu1->SetFillColor(19);
  hEta2x2tu1->SetLineWidth(2);
  hEta2x2tu1->SetLineColor(kBlue);
  hEta2x2tu1->SetXTitle("Residual (#mum)");
  hEta2x2tu1->SetYTitle("Events");
  hEta2x2tu1->Draw("");
  if( hEta2x2tu1->Integral()>10. ) {
  hEta2x2tu1->Fit("gaus");
  TF1 *fitge2 =hEta2x2tu1->GetFunction("gaus"); // Take parameters of the fit 
  fitge2->SetLineColor(6);
  sprintf( algorithmName[iPointNumber], "%s", hEta2x2tu1->GetTitle());
  ResolutionPoint[iPointNumber]=fitge2->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitge2->GetParError(2);
  fitge2->SetLineWidth(4);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hEta2x2tv1->SetFillColor(19);
  hEta2x2tv1->SetLineWidth(2);
  hEta2x2tv1->SetLineColor(kBlue);
  hEta2x2tv1->SetXTitle("Residual (#mum)");
  hEta2x2tv1->SetYTitle("Events");
  hEta2x2tv1->Draw();
  if( hEta2x2tv1->Integral()>10. ) {
  hEta2x2tv1->Fit("gaus");
  TF1 *fitge3 =hEta2x2tv1->GetFunction("gaus"); // Take parameters of the fit 
  fitge3->SetLineColor(6);
  sprintf( algorithmName[iPointNumber], "%s", hEta2x2tv1->GetTitle());
  ResolutionPoint[iPointNumber]=fitge3->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitge3->GetParError(2);
  fitge3->SetLineWidth(4);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 9
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hEtaU_2x2Res->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hEtaU_2x2Res->Draw("");
  if( hEtaU_2x2Res->Integral()>10. ) {
  hEtaU_2x2Res->Fit("gaus");
  TF1 *fitg6 =hEtaU_2x2Res->GetFunction("gaus"); // Take parameters of the fit 
  fitg6->SetLineColor(28);
  sprintf( algorithmName[iPointNumber], "%s", hEtaU_2x2Res->GetTitle());
  ResolutionPoint[iPointNumber]=fitg6->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitg6->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hEtaV_2x2Res->Draw("");
  if( hEtaV_2x2Res->Integral()>10. ) {
  hEtaV_2x2Res->Fit("gaus");
  TF1 *fitg7 =hEtaV_2x2Res->GetFunction("gaus"); // Take parameters of the fit 
  fitg7->SetLineColor(28);
  sprintf( algorithmName[iPointNumber], "%s", hEtaV_2x2Res->GetTitle());
  ResolutionPoint[iPointNumber]=fitg7->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitg7->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 10
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hEtaU_5x5Res->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hEtaU_5x5Res->Draw("");
  if( hEtaU_5x5Res->Integral()>10. ) {
  hEtaU_5x5Res->Fit("gaus");
  TF1 *fitg8 =hEtaU_5x5Res->GetFunction("gaus"); // Take parameters of the fit 
  fitg8->SetLineColor(38);
  sprintf( algorithmName[iPointNumber], "%s", hEtaU_5x5Res->GetTitle());
  ResolutionPoint[iPointNumber]=fitg8->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitg8->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hEtaV_5x5Res->Draw("");
  if( hEtaV_5x5Res->Integral()>10. ) {
  hEtaV_5x5Res->Fit("gaus");
  TF1 *fitg9 =hEtaV_5x5Res->GetFunction("gaus"); // Take parameters of the fit 
  fitg9->SetLineColor(38);
  sprintf( algorithmName[iPointNumber], "%s", hEtaV_5x5Res->GetTitle());
  ResolutionPoint[iPointNumber]=fitg9->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitg9->GetParError(2);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 11
  // JB 2010/07/22
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hEta3URes->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hEta3URes->SetFillColor(19);
  hEta3URes->SetLineWidth(2);
  hEta3URes->SetLineColor(kBlue);
  hEta3URes->SetXTitle("Residual (#mum)");
  hEta3URes->SetYTitle("Events");
  hEta3URes->Draw("");
  if( hEta3URes->Integral()>10. ) {
  hEta3URes->Fit("gaus");
  TF1 *fitgUEta3 =hEta3URes->GetFunction("gaus"); // Take parameters of the fit 
  fitgUEta3->SetLineColor(6);
  sprintf( algorithmName[iPointNumber], "%s", hEta3URes->GetTitle());
  ResolutionPoint[iPointNumber]=fitgUEta3->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgUEta3->GetParError(2);
  fitgUEta3->SetLineWidth(4);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hEta3VRes->SetFillColor(19);
  hEta3VRes->SetLineWidth(2);
  hEta3VRes->SetLineColor(kBlue);
  hEta3VRes->SetXTitle("Residual (#mum)");
  hEta3VRes->SetYTitle("Events");
  hEta3VRes->Draw("");
  if( hEta3VRes->Integral()>10. ) {
  hEta3VRes->Fit("gaus");
  TF1 *fitgVEta3 =hEta3VRes->GetFunction("gaus"); // Take parameters of the fit 
  fitgVEta3->SetLineColor(6);
  sprintf( algorithmName[iPointNumber], "%s", hEta3VRes->GetTitle());
  ResolutionPoint[iPointNumber]=fitgVEta3->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgVEta3->GetParError(2);
  fitgVEta3->SetLineWidth(4);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    //cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    //cresfit[iPointNumber-2]->Print(EPSName.Data());
    //cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Method 12
  // JB 2010/07/22
  sprintf( cname, "cresfit%d", iPointNumber);
  sprintf( ctitle, "Run %d Plane %d - Meth %d - %s",RunNumber,ThePlaneNumber,iPointNumber/2+1,hAHTURes->GetTitle());
  cresfit[iPointNumber] = new TCanvas( cname, ctitle, 400, 400);
  cresfit[iPointNumber]->Divide(2,1);
  cresfit[iPointNumber]->cd(1);
  hAHTURes->SetFillColor(19);
  hAHTURes->SetLineWidth(2);
  hAHTURes->SetLineColor(kBlue);
  hAHTURes->SetXTitle("Residual (#mum)");
  hAHTURes->SetYTitle("Events");
  hAHTURes->Draw("");
  if( hAHTURes->Integral()>10. ) {
  hAHTURes->Fit("gaus");
  TF1 *fitgUAHT =hAHTURes->GetFunction("gaus"); // Take parameters of the fit 
  fitgUAHT->SetLineColor(6);
  sprintf( algorithmName[iPointNumber], "%s", hAHTURes->GetTitle());
  ResolutionPoint[iPointNumber]=fitgUAHT->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgUAHT->GetParError(2);
  fitgUAHT->SetLineWidth(4);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;
  //
  cresfit[iPointNumber-1]->cd(2);
  hAHTVRes->SetFillColor(19);
  hAHTVRes->SetLineWidth(2);
  hAHTVRes->SetLineColor(kBlue);
  hAHTVRes->SetXTitle("Residual (#mum)");
  hAHTVRes->SetYTitle("Events");
  hAHTVRes->Draw("");
  if( hAHTVRes->Integral()>10. ) {
  hAHTVRes->Fit("gaus");
  TF1 *fitgVAHT =hAHTVRes->GetFunction("gaus"); // Take parameters of the fit 
  fitgVAHT->SetLineColor(6);
  sprintf( algorithmName[iPointNumber], "%s", hAHTVRes->GetTitle());
  ResolutionPoint[iPointNumber]=fitgVAHT->GetParameter(2);
  ErrorOnResolutionPoint[iPointNumber]=fitgVAHT->GetParError(2);
  fitgVAHT->SetLineWidth(4);
  }
  else { ResolutionPoint[iPointNumber]=ErrorOnResolutionPoint[iPointNumber]=0.;}
  iPointNumber++;

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots
     && SaveAllMethods
     ) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
  
    cresfit[iPointNumber-2]->Print(EPSNameO.Data());
    cresfit[iPointNumber-2]->Print(EPSName.Data());
    cresfit[iPointNumber-2]->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  //----------------------- summary canvas

  cres = new TCanvas("cres","Spatial Resolution",150,10,800,600);
  cres->Draw();
  cres->Clear();
  cres->SetBorderMode(0);

  Char_t RealTitle[20];
  sprintf(RealTitle,"Run %d Plane %d",RunNumber,ThePlaneNumber);
  gStyle->SetOptTitle(1);

  TPad* mpadrestitle = new TPad("mpadrestitle","",0.55,0.80,0.95,0.90);
  mpadrestitle->SetGrid(1);
  mpadrestitle->SetFillColor(10);
  mpadrestitle->Draw();
  mpadrestitle->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.6);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  ttt->DrawText( 0.05,0.6,RealTitle);
  cres->cd();

  TPad* my3 = new TPad("my3","",0.1,0.1,0.95,0.75);
  my3->SetFillColor(10);
  my3->Draw();
  my3->cd();
  my3->Clear();

  Float_t ResMax = 1.0;
  for ( Int_t ll=0 ; ll<iPointNumber ; ll++ ) {
    if(ResolutionPoint[ll]>ResMax){ResMax=ResolutionPoint[ll];}
  }

  TH1F *hresdummy = new TH1F( "hresdummy", "Resolution: different methods", iPointNumber+1, 0, iPointNumber/2+1);
  hresdummy->SetMinimum(0);
  hresdummy->SetMaximum(TMath::Min(ResMax, PixelSize));
  hresdummy->SetXTitle("Method");
  hresdummy->SetYTitle("Residue #sigma (#mum)");
  hresdummy->Draw();

  TLegend *leg2 = new TLegend(0.2,0.5,0.5,0.85);
  leg2->SetHeader("Method");

  TGraphErrors **gr=new TGraphErrors*[iPointNumber];
  Float_t n[2], en[2];
  Float_t reso[2], ereso[2];
  for( Short_t iRes=0; iRes<iPointNumber; iRes+=2) {
    if( ResolutionPoint[iRes]>0. || ResolutionPoint[iRes+1]>0. ) {
      n[0] = 1+iRes/2;
      n[1] = 1+iRes/2+0.2;
      en[0] = 0.;
      en[1] = 0.;
      reso[0] = ResolutionPoint[iRes];
      reso[1] = ResolutionPoint[iRes+1];
      ereso[0] = ErrorOnResolutionPoint[iRes];
      ereso[1] = ErrorOnResolutionPoint[iRes+1];
      gr[iRes] = new TGraphErrors(2, n, reso, en, ereso);
      gr[iRes]->SetMarkerColor(1+iRes/2);
      gr[iRes]->SetMarkerStyle(20+iRes/2);
      gr[iRes]->SetMarkerSize(1.);
      gr[iRes]->Draw("P");
      leg2->AddEntry(gr[iRes],algorithmName[iRes],"p");
    }
  }

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cres->Print(EPSNameO.Data());
    cres->Print(EPSName.Data());
    cres->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  /*
  Float_t *nnn = new Float_t[NumberOfResolutionPoints];
  Float_t *ennn = new Float_t[NumberOfResolutionPoints];
  for( Short_t i=0; i<NumberOfResolutionPoints; i++) {
    nnn[i] = 1+i/2+0.2*(i%2);
    ennn[i] = 0.;
  }

  TGraphErrors* gr=new TGraphErrors(iPointNumber,nnn,ResolutionPoint,ennn,ErrorOnResolutionPoint);
  gr->SetTitle("Resolution: different methods");

  Float_t Reso1[2];
  Float_t Reso2[2];
  Float_t Reso3[2];
  Float_t Reso4[2];
  Float_t Reso5[2];
  Float_t Reso6[2];
  Float_t Reso7[2];

  Float_t Reso1E[2];
  Float_t Reso2E[2];
  Float_t Reso3E[2];
  Float_t Reso4E[2];
  Float_t Reso5E[2];
  Float_t Reso6E[2];
  Float_t Reso7E[2];

  Float_t nnn1[2]={1,1.2};
  Float_t nnn2[2]={3,3.2};
  Float_t nnn3[2]={5,5.2};
  Float_t nnn4[2]={7,7.2};
  Float_t nnn5[2]={9,9.2};
  Float_t nnn6[2]={11,11.2};
  Float_t nnn7[2]={13,13.2};

  Float_t nnn1E[2]={0,0};
  Float_t nnn2E[2]={0,0};
  Float_t nnn3E[2]={0,0};
  Float_t nnn4E[2]={0,0};
  Float_t nnn5E[2]={0,0};
  Float_t nnn6E[2]={0,0};
  Float_t nnn7E[2]={0,0};
 
  Reso1[0]= ResolutionPoint[0];
  Reso1[1]= ResolutionPoint[1];
  Reso2[0]= ResolutionPoint[2];
  Reso2[1]= ResolutionPoint[3];
  Reso3[0]= ResolutionPoint[4];
  Reso3[1]= ResolutionPoint[5];
  Reso4[0]= ResolutionPoint[6];
  Reso4[1]= ResolutionPoint[7];
  Reso5[0]= ResolutionPoint[8];
  Reso5[1]= ResolutionPoint[9];
  Reso6[0]= ResolutionPoint[10];
  Reso6[1]= ResolutionPoint[11];
  Reso7[0]= ResolutionPoint[12];
  Reso7[1]= ResolutionPoint[13];

  Reso1E[0]=ErrorOnResolutionPoint[0];
  Reso1E[1]=ErrorOnResolutionPoint[1];
  Reso2E[0]=ErrorOnResolutionPoint[2];
  Reso2E[1]=ErrorOnResolutionPoint[3];
  Reso3E[0]=ErrorOnResolutionPoint[4];
  Reso3E[1]=ErrorOnResolutionPoint[5];
  Reso4E[0]=ErrorOnResolutionPoint[6];
  Reso4E[1]=ErrorOnResolutionPoint[7];
  Reso5E[0]=ErrorOnResolutionPoint[8];
  Reso5E[1]=ErrorOnResolutionPoint[9];
  Reso6E[0]=ErrorOnResolutionPoint[10];
  Reso6E[1]=ErrorOnResolutionPoint[11];
  Reso7E[0]=ErrorOnResolutionPoint[12];
  Reso7E[1]=ErrorOnResolutionPoint[13];

  TGraphErrors* gr1=new TGraphErrors(2,nnn1,Reso1,nnn1E,Reso1E);
  TGraphErrors* gr2=new TGraphErrors(2,nnn2,Reso2,nnn2E,Reso2E);
  TGraphErrors* gr3=new TGraphErrors(2,nnn3,Reso3,nnn3E,Reso3E);
  TGraphErrors* gr4=new TGraphErrors(2,nnn4,Reso4,nnn4E,Reso4E);
  TGraphErrors* gr5=new TGraphErrors(2,nnn5,Reso5,nnn5E,Reso5E);
  TGraphErrors* gr6=new TGraphErrors(2,nnn6,Reso6,nnn6E,Reso6E);
  TGraphErrors* gr7=new TGraphErrors(2,nnn7,Reso7,nnn7E,Reso7E);

  gr1->SetMarkerColor(2);
  gr2->SetMarkerColor(3);
  gr3->SetMarkerColor(4);
  gr4->SetMarkerColor(6);
  gr5->SetMarkerColor(8);
  gr6->SetMarkerColor(28);
  gr7->SetMarkerColor(38);

  gr1->SetMarkerStyle(8);
  gr2->SetMarkerStyle(21);
  gr3->SetMarkerStyle(22);
  gr4->SetMarkerStyle(23);
  gr5->SetMarkerStyle(28);
  gr6->SetMarkerStyle(29);
  gr7->SetMarkerStyle(27);

  gr1->SetMarkerSize(1.);
  gr2->SetMarkerSize(1.);
  gr3->SetMarkerSize(1.);
  gr4->SetMarkerSize(1.);
  gr5->SetMarkerSize(1.);
  gr6->SetMarkerSize(1.);
  gr7->SetMarkerSize(1.);

  gr->SetMarkerColor(10);
  gr->Draw("AP");
  gr->GetXaxis()->SetTitle("Method");
  gr->GetYaxis()->SetTitle("Residue #sigma (#mum)");
  gPad->Update();

  gr1->Draw("P");
  gr2->Draw("P");
  gr3->Draw("P");
  gr4->Draw("P");
  gr5->Draw("P");
  gr6->Draw("P");
  gr7->Draw("P");

  leg2->AddEntry(gr1,algorithmName[0],"p");
  leg2->AddEntry(gr2,algorithmName[2],"p");
  leg2->AddEntry(gr3,algorithmName[4],"p");
  leg2->AddEntry(gr4,algorithmName[6],"p");
  leg2->AddEntry(gr5,algorithmName[8],"p");
  leg2->AddEntry(gr6,algorithmName[10],"p");
  leg2->AddEntry(gr7,algorithmName[12],"p");
  */

  leg2->SetFillStyle(0);
  leg2->Draw();

  for ( Int_t ll=0; ll<iPointNumber ; ll++ ) {
    if(ll%2==0) cout << " Method " << ll/2+1 << endl;
    cout<< "residue from "<<algorithmName[ll]<<" algo("<<ll<<") "<<ResolutionPoint[ll]<<" +- "<<ErrorOnResolutionPoint[ll]<<" um"<<endl;

    if(ll%2==0 && ll/2+1==3) csvfile << ResolutionPoint[ll]<<","<<ErrorOnResolutionPoint[ll]<<","<<ResolutionPoint[ll+1]<<","<<ErrorOnResolutionPoint[ll+1]<<",";
    if(ll%2==0 && ll/2+1==5) csvfile << ResolutionPoint[ll]<<","<<ErrorOnResolutionPoint[ll]<<","<<ResolutionPoint[ll+1]<<","<<ErrorOnResolutionPoint[ll+1]<<",";
  }
  cout << endl;
  for(int imult=0;imult<huCGwidth_vs_Mult->GetXaxis()->GetNbins();imult++) {
    if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
      cout<< "resolution from Tu-huCG(DSF) algo for mult = " << imult+1 << " is " << ResolutionPointU_CGDSF[imult] << " +- " << ErrResolutionPointU_CGDSF[imult] << " um" << endl;
      cout<< "resolution from Tv-hvCG(DSF) algo for mult = " << imult+1 << " is " << ResolutionPointV_CGDSF[imult] << " +- " << ErrResolutionPointV_CGDSF[imult] << " um" << endl;
    }
    else {
      cout<< "residue from Tu-huCG(DSF) algo for mult = " << imult+1 << " is " << ResolutionPointU_CGDSF[imult] << " +- " << ErrResolutionPointU_CGDSF[imult] << " um" << endl;
      cout<< "residue from Tv-hvCG(DSF) algo for mult = " << imult+1 << " is " << ResolutionPointV_CGDSF[imult] << " +- " << ErrResolutionPointV_CGDSF[imult] << " um" << endl;
    }
    cout << endl;
  }
  cout << endl;
  
  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");

    TString command;
    
    TCanvas c1("c1","c1",400,400);
    c1.SetFrameFillColor(10);
    c1.SetFillColor(10);
    c1.SetLeftMargin(0.05);
    c1.SetRightMargin(0.05);
    c1.Print(EPSNameO.Data());
    
    double initX = 0.05;
    double initY = 0.05;
    double X = initX;
    double Y = 1.0 - initY;
    char ytitle[100];
    TLatex* latex = new TLatex();

    c1.Clear();
    latex->SetTextAlign(12);
    latex->SetTextSize(0.04);
    latex->SetTextColor(kBlack);
    latex->DrawLatex(0.25,Y,"Residuals for different methods");
    latex->SetTextSize(0.02);
    latex->SetTextColor(kBlue);

    int counter_methods = 0;
    for ( Int_t ll=0; ll<iPointNumber ; ll++ ) {
      if(ll%2==0) {
	counter_methods++;
	Y -= initY;
	command = TString(" Method ") + long(ll/2+1);
	latex->DrawLatex(X,Y,command.Data());
      }
      Y -= 0.02;
      sprintf(ytitle,"%.4f",ResolutionPoint[ll]);
      command  = TString("residue from ") + TString(algorithmName[ll]) + TString(" algo(") + long(ll) + TString(") = (") + TString(ytitle) + TString(" #pm ");
      sprintf(ytitle,"%.4f",ErrorOnResolutionPoint[ll]);
      command += TString(ytitle) + TString(")#mum");
      latex->DrawLatex(X,Y,command.Data());
      
      if(counter_methods == (iPointNumber/4) && 
	 ll+1 == counter_methods*2) {
	c1.Print(EPSName.Data());
	
	c1.Clear();
	Y = 1.0 - initY;
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	latex->SetTextColor(kBlack);
	latex->DrawLatex(0.25,Y,"Residuals for different methods");
	latex->SetTextSize(0.02);
	latex->SetTextColor(kBlue);
      }
    }
    c1.Print(EPSName.Data());

    c1.Clear();
    Y = 1.0 - initY;
    latex->SetTextAlign(12);
    latex->SetTextSize(0.04);
    latex->SetTextColor(kBlack);
    if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
      latex->DrawLatex(0.25,Y,"#sigma_{sp} vs cluster multiplicity");
    }
    else {
      latex->DrawLatex(0.25,Y,"Residuals vs cluster multiplicity");
    }
    latex->SetTextSize(0.02);
    latex->SetTextColor(kBlue);
    Y -= 0.04;
    command  = TString("");
    latex->DrawLatex(X,Y,command.Data());
    if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
      Y -= 0.02;
      sprintf(ytitle,"%.4f",DUT_resolutionU_CGDSF[0]);
      command  = TString("resolution from Tu-huCG(DSF) algo for all multiplicities = ") + TString(ytitle) + TString(" #pm ");
      sprintf(ytitle,"%.4f",DUT_resolutionU_CGDSF[1]);
      command += TString(ytitle) + TString(")#mum");
      latex->DrawLatex(X,Y,command.Data());
      Y -= 0.02;
      sprintf(ytitle,"%.4f",DUT_resolutionV_CGDSF[0]);
      command  = TString("resolution from Tv-hvCG(DSF) algo for all multiplicities = ") + TString(ytitle) + TString(" #pm ");
      sprintf(ytitle,"%.4f",DUT_resolutionV_CGDSF[1]);
      command += TString(ytitle) + TString(")#mum");
      latex->DrawLatex(X,Y,command.Data());
      Y -= 0.02;
      command  = TString("");
      latex->DrawLatex(X,Y,command.Data());
    }
    for(int imult=0;imult<huCGwidth_vs_Mult->GetXaxis()->GetNbins();imult++) {
      Y -= 0.02;
      sprintf(ytitle,"%.4f",ResolutionPointU_CGDSF[imult]);
      command  = TString("residue from Tu-huCG(DSF) algo for mult = ") + long(imult+1) + TString(" = (") + TString(ytitle) + TString(" #pm ");
      sprintf(ytitle,"%.4f",ErrResolutionPointU_CGDSF[imult]);
      command += TString(ytitle) + TString(")#mum");
      latex->DrawLatex(X,Y,command.Data());
      Y -= 0.02;
      sprintf(ytitle,"%.4f",ResolutionPointV_CGDSF[imult]);
      command  = TString("residue from Tv-hvCG(DSF) algo for mult = ") + long(imult+1) + TString(" = (") + TString(ytitle) + TString(" #pm ");
      sprintf(ytitle,"%.4f",ErrResolutionPointV_CGDSF[imult]);
      command += TString(ytitle) + TString(")#mum");
      latex->DrawLatex(X,Y,command.Data());
      Y -= 0.02;
      command  = TString("");
      latex->DrawLatex(X,Y,command.Data());
    }
    c1.Print(EPSName.Data());
    
    c1.Print(EPSNameC.Data());

    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  /*
    gr->SetMarkerColor(2);
    gr->SetMarkerStyle(8);
    gr->SetMarkerSize(1.5);
    TLatex *tex1 = new TLatex(0.719397,2.3,"C0G3x3+corr");
    tex1->SetTextSize(0.03);
    tex1->SetLineStyle(0);
    tex1->SetLineWidth(2);
    TLatex* tex = new TLatex(2.64741,2.3,"#eta 2x2");
    tex->SetTextSize(0.03);
    tex->SetLineStyle(0);
    tex->SetLineWidth(2);
    TLatex* tex2 = new TLatex(4.79267,2.3,"#eta 3x3");
    tex2->SetTextSize(0.03);
    tex2->SetLineStyle(0);
    tex2->SetLineWidth(2);
    TLatex* tex3 = new TLatex(1.53405,2.4,"Resolution: Different methods");
    tex3->SetTextSize(0.07);
    tex3->SetLineStyle(0);
    tex3->SetLineWidth(2);
    gr->Draw("AP");
    gPad->Update();
  */
  //--------------WRITE IN A ROOT FILE

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;


  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");

  cres->Write();
  cresfit[TheCGPointNumber]->SetName("cresfit_CGDSF");
  cresfit[TheCGPointNumber]->Write();
  cresfit_CGWidth_vs_Mult->Write();
  cresfit_CGMean_vs_Mult->Write();
  hAllHuvsAllTu1->Write();
  hAllHvvsAllTv1->Write();
  hAllHuvsAllTu2->Write();
  hAllHvvsAllTv2->Write();
  huCGtu1->Write();
  hvCGtv1->Write();
  for(int imult=0;imult<huCGwidth_vs_Mult->GetXaxis()->GetNbins();imult++) {
    cresfit_CG_vs_Mult[imult]->Write();
    huCGtu1_vs_Mult[imult]->Write();
    hvCGtv1_vs_Mult[imult]->Write();
  }
  huCGwidth_vs_Mult->Write();
  hvCGwidth_vs_Mult->Write();
  huCGmean_vs_Mult->Write();
  hvCGmean_vs_Mult->Write();
#if 0
  for(int itrks=0;itrks<hnTracksInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();itrks++) {
    if(hnTracksInGeomatrixVsTrackPerEvent->GetBinContent(itrks+1) < 100) continue;
    cresfit_CG_vs_TracksPerEvent[itrks]->Write();
    huCGtu1_vs_TracksPerEvent[itrks]->Write();
    hvCGtv1_vs_TracksPerEvent[itrks]->Write();
  }
  cresfit_CGWidth_vs_TracksPerEvent->Write();
  hResidueUInGeomatrixVsTrackPerEvent->Write();
  hResidueVInGeomatrixVsTrackPerEvent->Write();
#endif
  hCG2x2tu1->Write();
  hCG2x2tv1->Write();
  hCG5URes->Write();
  hCG5VRes->Write();
  hTuHuCorr->Write();
  hTvHvCorr->Write();
  hEtaURes->Write();
  hEtaVRes->Write();
  hEta2x2tu1->Write();
  hEta2x2tv1->Write();
  hEtaU_2x2Res->Write();
  hEtaV_2x2Res->Write();
  hEtaU_5x5Res->Write();
  hEtaV_5x5Res->Write();
  hEta3URes->Write();
  hEta3VRes->Write();
  hAHTURes->Write();
  hAHTVRes->Write();


  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;

  MHist::Dir();

}

//__________________________________________________________________________________________
//
void MimosaAnalysis::CheckMCSimulation(bool StandAlone)
{

  // Display plots of MC simulation for evaluating the telescope resolution
  //
  // Modified AP 2015/03/16. Creation

  if(!CheckIfDone("MCGeneration")) return;

  char fOutName[200];
  if(StandAlone) sprintf(fOutName,"MCStandAlone_run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  else           sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //if(!StandAlone) sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //else {
  //sprintf(tmpFile,"MCStandAlone_run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //TString command = TString("rm ") + TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");
  //gSystem->Exec(command.Data());
  //}
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");

  UsedMacro[0] = 1;

  gStyle->SetOptFit(1);
  
  Int_t nPlanes = fSession->GetTracker()->GetPlanesN();

  if(hTrackChi2_MC->GetEntries()>0) {
    double Max = -1.0e+20;
    if(hTrackChi2_MC->GetBinContent(hTrackChi2_MC->GetXaxis()->GetNbins()) > 0.0) {
      Max  = hTrackChi2_MC->GetBinCenter(hTrackChi2_MC->GetXaxis()->GetNbins());
      Max += hTrackChi2_MC->GetBinWidth(hTrackChi2_MC->GetXaxis()->GetNbins())*0.48;
    }
    else {
      for(int ichi2=0;ichi2<hTrackChi2_MC->GetXaxis()->GetNbins();ichi2++) {
	int idx = hTrackChi2_MC->GetXaxis()->GetNbins() - ichi2;
	if(hTrackChi2_MC->GetBinContent(idx) > 0.0) {
	  Max  = hTrackChi2_MC->GetBinCenter(idx);
	  Max += hTrackChi2_MC->GetBinWidth(idx)*0.48;
	  break;
	}
      }
    }
    hTrackChi2_MC->SetAxisRange(hTrackChi2_MC->GetXaxis()->GetXmin(),
				Max,"X");
  }
  
  TF1 *fitgs = NULL;
  DUT_residualU_CGDSF[0] = -1.0;
  DUT_residualU_CGDSF[1] =  1.0e-6;
  if(huCGtu1 != NULL) {
    if(huCGtu1->Integral() > 10.) {
      huCGtu1->Fit("gaus");
      fitgs = huCGtu1->GetFunction("gaus");
      DUT_residualU_CGDSF[0] = fitgs->GetParameter(2);
      DUT_residualU_CGDSF[1] = fitgs->GetParError(2);
    }
  }
  DUT_residualV_CGDSF[0] = -1.0;
  DUT_residualV_CGDSF[1] =  1.0e-6;
  if(hvCGtv1 != NULL) {
    if(hvCGtv1->Integral() > 10.) {
      hvCGtv1->Fit("gaus");
      fitgs = hvCGtv1->GetFunction("gaus");
      DUT_residualV_CGDSF[0] = fitgs->GetParameter(2);
      DUT_residualV_CGDSF[1] = fitgs->GetParError(2);
    }
  }
  
  if(DUT_residualU_CGDSF[0] > 0.0 && 
     DUT_residualV_CGDSF[0] > 0.0) {
    if(UsingTrackerResolution) {
      //Estimation of U-resolution:
      if(DUT_residualU_CGDSF[0] < h_ResidualAtDUT_Scan_U->GetBinContent(1) || 
	 DUT_residualU_CGDSF[0] > h_ResidualAtDUT_Scan_U->GetBinContent(h_ResidualAtDUT_Scan_U->GetXaxis()->GetNbins())) {
	cout << endl;
	cout << "MimosaGeneration:: U-residual=" << DUT_residualU_CGDSF[0] << "um is outside scanned U-Range (" << h_ResidualAtDUT_Scan_U->GetBinContent(1) << "," 
	     << h_ResidualAtDUT_Scan_U->GetBinContent(h_ResidualAtDUT_Scan_U->GetXaxis()->GetNbins())
	     << ")um. Setting U-resolution to stupid value -999.0!" << endl;
	cout << endl;
	DUT_resolutionU_CGDSF[0] = -999.0;
	DUT_resolutionU_CGDSF[1] = 1.0e-6;
	Tel_resolutionU_CGDSF[0] = -999.0;
	Tel_resolutionU_CGDSF[1] = 1.0e-6;
      }
      else {
	int idx = -999;
	for(Int_t istep=0;istep<Bins_GlobalResolution-1;istep++) {
	  double x1  = h_ResidualAtDUT_Scan_U->GetBinCenter(istep+1);
	  double x2  = h_ResidualAtDUT_Scan_U->GetBinCenter(istep+2);
	  double y1  = h_ResidualAtDUT_Scan_U->GetBinContent(istep+1);
	  double y2  = h_ResidualAtDUT_Scan_U->GetBinContent(istep+2);
	  double y1e = h_ResidualAtDUT_Scan_U->GetBinError(istep+1);
	  double y2e = h_ResidualAtDUT_Scan_U->GetBinError(istep+2);
	  
	  if((DUT_residualU_CGDSF[0] >= y1 && 
	      DUT_residualU_CGDSF[0] <  y2) || 
	     (DUT_residualU_CGDSF[0] >= y2 && 
	      DUT_residualU_CGDSF[0] <  y1)) {
	    
	    idx = istep+1;
	    
	    double a = (y2-y1)/(x2-x1);
	    double b = y2 - a*x2;
	    
	    if(TMath::Abs(a) > 1.0e-6) {
	      DUT_resolutionU_CGDSF[0]  = (DUT_residualU_CGDSF[0] - b)/a;
	      DUT_resolutionU_CGDSF[1]  = pow(DUT_residualU_CGDSF[1],2);
	      DUT_resolutionU_CGDSF[1] += pow(y1e*(DUT_residualU_CGDSF[0] - y2)/(y2-y1),2);
	      DUT_resolutionU_CGDSF[1] += pow(y2e*(DUT_residualU_CGDSF[0] - y1)/(y2-y1),2);
	      DUT_resolutionU_CGDSF[1] /= pow(a,2);
	      DUT_resolutionU_CGDSF[1]  = sqrt(DUT_resolutionU_CGDSF[1]);
	    }
	    else {
	      DUT_resolutionU_CGDSF[0] = 0.5*(x2 + x1);
	      DUT_resolutionU_CGDSF[1] = sqrt(pow(y2e,2) + pow(DUT_residualU_CGDSF[1],2));
	    }
	    
	    y1  = h_TelescopeResolution_Scan_U->GetBinContent(istep+1);
	    y2  = h_TelescopeResolution_Scan_U->GetBinContent(istep+2);
	    y1e = h_TelescopeResolution_Scan_U->GetBinError(istep+1);
	    y2e = h_TelescopeResolution_Scan_U->GetBinError(istep+2);
	    a = (y2-y1)/(x2-x1);
	    b = y2 - a*x2;
	    if(TMath::Abs(a) > 1.0e-6) {
	      Tel_resolutionU_CGDSF[0] = DUT_resolutionU_CGDSF[0]*a + b;
	      Tel_resolutionU_CGDSF[1] = 0.5*(pow(y1e,2) + pow(y2e,2));
	    }
	    else {
	      Tel_resolutionU_CGDSF[0] = y1;
	      Tel_resolutionU_CGDSF[1] = y1e;
	    }
	    
	    break;
	  }
	}
	cout << endl;
	cout << "MimosaGeneration:: U-residual = (" << DUT_residualU_CGDSF[0] << " +/- " << DUT_residualU_CGDSF[1] << ")um and estimated telescope U-resolution = " 
	     << h_TelescopeResolution_Scan_U->GetBinContent(idx) << " +/- " << h_TelescopeResolution_Scan_U->GetBinError(idx)
	     << "um. U-resolution = (" << DUT_resolutionU_CGDSF[0] << " +/- " << DUT_resolutionU_CGDSF[1] << ")um." << endl;
	cout << endl;
	
      }

      //Estimation of V-resolution:
      if(DUT_residualV_CGDSF[0] < h_ResidualAtDUT_Scan_V->GetBinContent(1) || 
	 DUT_residualV_CGDSF[0] > h_ResidualAtDUT_Scan_V->GetBinContent(h_ResidualAtDUT_Scan_V->GetXaxis()->GetNbins())) {
	cout << endl;
	cout << "MimosaGeneration:: V-residual=" << DUT_residualV_CGDSF[0] << "um is outside scanned V-Range (" << h_ResidualAtDUT_Scan_V->GetBinContent(1) << "," 
	     << h_ResidualAtDUT_Scan_V->GetBinContent(h_ResidualAtDUT_Scan_V->GetXaxis()->GetNbins())
	     << ")um. Setting U-resolution to stupid value -999.0!" << endl;
	cout << endl;
	DUT_resolutionV_CGDSF[0] = -999.0;
	DUT_resolutionV_CGDSF[1] = 1.0e-6;
      }
      else {
	int idx = -999;
	for(Int_t istep=0;istep<Bins_GlobalResolution-1;istep++) {
	  double x1  = h_ResidualAtDUT_Scan_V->GetBinCenter(istep+1);
	  double x2  = h_ResidualAtDUT_Scan_V->GetBinCenter(istep+2);
	  double y1  = h_ResidualAtDUT_Scan_V->GetBinContent(istep+1);
	  double y2  = h_ResidualAtDUT_Scan_V->GetBinContent(istep+2);
	  double y1e = h_ResidualAtDUT_Scan_V->GetBinError(istep+1);
	  double y2e = h_ResidualAtDUT_Scan_V->GetBinError(istep+2);
	  
	  if((DUT_residualV_CGDSF[0] >= y1 && 
	      DUT_residualV_CGDSF[0] <  y2) || 
	     (DUT_residualV_CGDSF[0] >= y2 && 
	      DUT_residualV_CGDSF[0] <  y1)) {
	    
	    idx = istep+1;
	    
	    double a = (y2-y1)/(x2-x1);
	    double b = y2 - a*x2;
	    
	    if(TMath::Abs(a) > 1.0e-6) {
	      DUT_resolutionV_CGDSF[0]  = (DUT_residualV_CGDSF[0] - b)/a;
	      DUT_resolutionV_CGDSF[1]  = pow(DUT_residualV_CGDSF[1],2);
	      DUT_resolutionV_CGDSF[1] += pow(y1e*(DUT_residualV_CGDSF[0] - y2)/(y2-y1),2);
	      DUT_resolutionV_CGDSF[1] += pow(y2e*(DUT_residualV_CGDSF[0] - y1)/(y2-y1),2);
	      DUT_resolutionV_CGDSF[1] /= pow(a,2);
	      DUT_resolutionV_CGDSF[1]  = sqrt(DUT_resolutionV_CGDSF[1]);
	    }
	    else {
	      DUT_resolutionV_CGDSF[0] = 0.5*(x2 + x1);
	      DUT_resolutionV_CGDSF[1] = sqrt(pow(y2e,2) + pow(DUT_residualV_CGDSF[1],2));
	    }
	    
	    y1  = h_TelescopeResolution_Scan_V->GetBinContent(istep+1);
	    y2  = h_TelescopeResolution_Scan_V->GetBinContent(istep+2);
	    y1e = h_TelescopeResolution_Scan_V->GetBinError(istep+1);
	    y2e = h_TelescopeResolution_Scan_V->GetBinError(istep+2);
	    a = (y2-y1)/(x2-x1);
	    b = y2 - a*x2;
	    if(TMath::Abs(a) > 1.0e-6) {
	      Tel_resolutionV_CGDSF[0] = DUT_resolutionV_CGDSF[0]*a + b;
	      Tel_resolutionV_CGDSF[1] = 0.5*(pow(y1e,2) + pow(y2e,2));
	    }
	    else {
	      Tel_resolutionV_CGDSF[0] = y1;
	      Tel_resolutionV_CGDSF[1] = y1e;
	    }
	    
	    break;
	  }
	}
	cout << endl;
	cout << "MimosaGeneration:: V-residual = (" << DUT_residualV_CGDSF[0] << " +/- " << DUT_residualV_CGDSF[1] << ")um and estimated telescope V-resolution = " 
	     << h_TelescopeResolution_Scan_V->GetBinContent(idx) << " +/- " << h_TelescopeResolution_Scan_V->GetBinError(idx)
	     << "um. U-resolution = (" << DUT_resolutionV_CGDSF[0] << " +/- " << DUT_resolutionV_CGDSF[1] << ")um." << endl;
	cout << endl;
	
      }
      
    }
    else {
      //fitgs = hTrackResidualAtDUT_U[0]->GetFunction("gaus");
      //if(fitgs->GetParameter(2) > DUT_residualU_CGDSF[0]) {
      if(hTrackResidualAtDUT_U[0]->GetRMS() > DUT_residualU_CGDSF[0]) {
	cout << endl;
	cout << "MimosaGeneration:: U-residual=" << DUT_residualU_CGDSF[0] << "um is smaller than estimated telescope U-resolution = " << hTrackResidualAtDUT_U[0]->GetRMS()
	     << "um. Setting U-resolution to stupid value -999.0!" << endl;
	cout << endl;
	DUT_resolutionU_CGDSF[0] = -999.0;
	DUT_resolutionU_CGDSF[1] = 1.0e-6;
	Tel_resolutionU_CGDSF[0] = hTrackResidualAtDUT_U[0]->GetRMS();
	Tel_resolutionU_CGDSF[1] = hTrackResidualAtDUT_U[0]->GetRMSError();
      }
      else {
	DUT_resolutionU_CGDSF[0]  = sqrt(pow(DUT_residualU_CGDSF[0],2) - pow(hTrackResidualAtDUT_U[0]->GetRMS(),2));
	DUT_resolutionU_CGDSF[1]  = pow(DUT_residualU_CGDSF[0]*DUT_residualU_CGDSF[1],2);
	DUT_resolutionU_CGDSF[1] += pow(hTrackResidualAtDUT_U[0]->GetRMS()*hTrackResidualAtDUT_U[0]->GetRMSError(),2);
	DUT_resolutionU_CGDSF[1]  = sqrt(DUT_resolutionU_CGDSF[1])/DUT_resolutionU_CGDSF[0];
	cout << endl;
	cout << "MimosaGeneration:: U-residual = (" << DUT_residualU_CGDSF[0] << " +/- " << DUT_residualU_CGDSF[1] << ")um and estimated telescope U-resolution = " 
	     << hTrackResidualAtDUT_U[0]->GetRMS() << " +/- " << hTrackResidualAtDUT_U[0]->GetRMSError()
	     << "um. U-resolution = (" << DUT_resolutionU_CGDSF[0] << " +/- " << DUT_resolutionU_CGDSF[1] << ")um." << endl;
	cout << endl;
	Tel_resolutionU_CGDSF[0] = hTrackResidualAtDUT_U[0]->GetRMS();
	Tel_resolutionU_CGDSF[1] = hTrackResidualAtDUT_U[0]->GetRMSError();
      }
      
      //fitgs = hTrackResidualAtDUT_V[0]->GetFunction("gaus");
      //if(fitgs->GetParameter(2) > DUT_residualV_CGDSF[0]) {
      if(hTrackResidualAtDUT_V[0]->GetRMS() > DUT_residualV_CGDSF[0]) {
	cout << endl;
	cout << "MimosaGeneration:: V-residual=" << DUT_residualV_CGDSF[0] << "um is smaller than estimated telescope U-resolution = " << hTrackResidualAtDUT_V[0]->GetRMS() 
	     << "um. Setting V-resolution to stupid value -999.0!" << endl;
	cout << endl;
	DUT_resolutionV_CGDSF[0] = -999.0;
	DUT_resolutionV_CGDSF[1] = 1.0e-6;
	Tel_resolutionV_CGDSF[0] = hTrackResidualAtDUT_V[0]->GetRMS();
	Tel_resolutionV_CGDSF[1] = hTrackResidualAtDUT_V[0]->GetRMSError();
      }
      else {
	DUT_resolutionV_CGDSF[0]  = sqrt(pow(DUT_residualV_CGDSF[0],2) - pow(hTrackResidualAtDUT_V[0]->GetRMS(),2));
	DUT_resolutionV_CGDSF[1]  = pow(DUT_residualV_CGDSF[0]*DUT_residualV_CGDSF[1],2);
	DUT_resolutionV_CGDSF[1] += pow(hTrackResidualAtDUT_V[0]->GetRMS()*hTrackResidualAtDUT_V[0]->GetRMSError(),2);
	DUT_resolutionV_CGDSF[1]  = sqrt(DUT_resolutionV_CGDSF[1])/DUT_resolutionV_CGDSF[0];
	cout << endl;
	cout << "MimosaGeneration:: V-residual = (" << DUT_residualV_CGDSF[0] << " +/- " << DUT_residualV_CGDSF[1] << ")um and estimated telescope U-resolution = " 
	     << hTrackResidualAtDUT_V[0]->GetRMS() << " +/- " << hTrackResidualAtDUT_V[0]->GetRMSError()
	     << "um. V-resolution = (" << DUT_resolutionV_CGDSF[0] << " +/- " << DUT_resolutionV_CGDSF[1] << ")um." << endl;
	cout << endl;
	Tel_resolutionV_CGDSF[0] = hTrackResidualAtDUT_V[0]->GetRMS();
	Tel_resolutionV_CGDSF[1] = hTrackResidualAtDUT_V[0]->GetRMSError();
      }
    }
  }
  
  if(!UsingTrackerResolution) {
    TObject* g;
    // Canvases for track properties
    g = gROOT->FindObject("ccumultrack3") ;
    if(g) ccumultrack3 = (TCanvas*)g;
    else  ccumultrack3 = new TCanvas("ccumultrack3", "Cumulate tracks Properties", 50, 50,900,700);
    ccumultrack3->Clear();
    ccumultrack3->UseCurrentStyle();
    ccumultrack3->Divide(3,2);
  
    g = gROOT->FindObject("ccumultrack4") ;
    if(g) ccumultrack4 = (TCanvas*)g;
    else  ccumultrack4 = new TCanvas("ccumultrack4", "Cumulate tracks Residual-U", 75, 75,800,700);
    ccumultrack4->Clear();
    ccumultrack4->UseCurrentStyle();
    ccumultrack4->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

    g = gROOT->FindObject("ccumultrack5") ;
    if(g) ccumultrack5 = (TCanvas*)g;
    else  ccumultrack5 = new TCanvas("ccumultrack5", "Cumulate tracks Residual-V", 100, 100,800,700);
    ccumultrack5->Clear();
    ccumultrack5->UseCurrentStyle();
    ccumultrack5->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

    // track properties
    ccumultrack3->cd(1);
    if(hTrackSlopeX_MC->GetEntries()>0) {
      TF1* fTrackSlopeX = new TF1("fTrackSlopeX",
				  "[2]*(1.0/(sqrt(2.0*TMath::Pi())*[1]))*TMath::Exp(-0.5*TMath::Power((x-[0])/[1],2))",
				  hTrackSlopeX_MC->GetMean() - 3.0*hTrackSlopeX_MC->GetRMS(),
				  hTrackSlopeX_MC->GetMean() + 3.0*hTrackSlopeX_MC->GetRMS());
      fTrackSlopeX->SetParameter(0,hTrackSlopeX_MC->GetMean());
      fTrackSlopeX->SetParameter(1,hTrackSlopeX_MC->GetRMS());
      fTrackSlopeX->SetParameter(2,hTrackSlopeX_MC->GetEntries());
      fTrackSlopeX->SetParName(0,"#mu");
      fTrackSlopeX->SetParName(1,"#sigma");
      fTrackSlopeX->SetParName(2,"Yield");
      fTrackSlopeX->SetLineColor(2);

      hTrackSlopeX_MC->GetXaxis()->SetNdivisions(605);
      hTrackSlopeX_MC->Fit(fTrackSlopeX,"R");
      hTrackSlopeX_MC->SetLineColor(4);
      hTrackSlopeX_MC->SetLineWidth(2);
      hTrackSlopeX_MC->Draw();
    }
    ccumultrack3->cd(2);
    if(hTrackSlopeY_MC->GetEntries()>0) {
      TF1* fTrackSlopeY = new TF1("fTrackSlopeY",
				  "[2]*(1.0/(sqrt(2.0*TMath::Pi())*[1]))*TMath::Exp(-0.5*TMath::Power((x-[0])/[1],2))",
				  hTrackSlopeY_MC->GetMean() - 3.0*hTrackSlopeY_MC->GetRMS(),
				  hTrackSlopeY_MC->GetMean() + 3.0*hTrackSlopeY_MC->GetRMS());
      fTrackSlopeY->SetParameter(0,hTrackSlopeY_MC->GetMean());
      fTrackSlopeY->SetParameter(1,hTrackSlopeY_MC->GetRMS());
      fTrackSlopeY->SetParameter(2,hTrackSlopeY_MC->GetEntries());
      fTrackSlopeY->SetParName(0,"#mu");
      fTrackSlopeY->SetParName(1,"#sigma");
      fTrackSlopeY->SetParName(2,"Yield");
      fTrackSlopeY->SetLineColor(2);

      hTrackSlopeY_MC->GetXaxis()->SetNdivisions(605);
      hTrackSlopeY_MC->Fit(fTrackSlopeY,"R");
      hTrackSlopeY_MC->SetLineColor(4);
      hTrackSlopeY_MC->SetLineWidth(2);
      hTrackSlopeY_MC->Draw();
    }
    ccumultrack3->cd(4);
    if(hTrackNHits_MC->GetEntries()>0) {
      hTrackNHits_MC->Draw();
      gPad->SetGridx();
    }
    ccumultrack3->cd(5);
    if(hTrackPlanesUsed_MC->GetEntries()>0) {
      hTrackPlanesUsed_MC->Draw();
      gPad->SetGridx();
    }
    ccumultrack3->cd(6);
    if(hTrackChi2_MC->GetEntries()>0) {
      hTrackChi2_MC->SetLineColor(4);
      hTrackChi2_MC->SetLineWidth(2);
      hTrackChi2_MC->Draw();
      TLine* l_Chi2Cut = new TLine(TrackChi2Limit,hTrackChi2_MC->GetMinimum(),
				   TrackChi2Limit,hTrackChi2_MC->GetMaximum());
      l_Chi2Cut->SetLineColor(2);
      l_Chi2Cut->SetLineWidth(2);
      l_Chi2Cut->SetLineStyle(2);
      l_Chi2Cut->Draw();
    }
    
    for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { 
      ccumultrack4->cd(iPlane);
      if(hResidualU_MC[iPlane-1]->GetEntries()>0) hResidualU_MC[iPlane-1]->Draw();
      ccumultrack5->cd(iPlane);
      if(hResidualV_MC[iPlane-1]->GetEntries()>0) hResidualV_MC[iPlane-1]->Draw();    
    }
    
    ccumultrack3->Update();
    if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
      NPages++;
      TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
      TString EPSNameO = EPSName + TString("[");
      TString EPSNameC = EPSName + TString("]");
    
      ccumultrack3->Print(EPSNameO.Data());
      ccumultrack3->Print(EPSName.Data());
      ccumultrack3->Print(EPSNameC.Data());

      //if(!StandAlone) {
	//command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
	//gSystem->Exec(command.Data());
	//command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
	//gSystem->Exec(command.Data());
	//command = TString("rm -rf ") + EPSName;
	//gSystem->Exec(command.Data());
      //}
      //else {
	//command = TString("mv ") + EPSName + TString(" ") + EPSName_final;
	//gSystem->Exec(command.Data());
      //}
    }

    ccumultrack4->Update();
    if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
      NPages++;
      TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
      TString EPSNameO = EPSName + TString("[");
      TString EPSNameC = EPSName + TString("]");
    
      ccumultrack4->Print(EPSNameO.Data());
      ccumultrack4->Print(EPSName.Data());
      ccumultrack4->Print(EPSNameC.Data());
      //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
      //gSystem->Exec(command.Data());
      //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
      //gSystem->Exec(command.Data());
      //command = TString("rm -rf ") + EPSName;
      //gSystem->Exec(command.Data());
    }

    ccumultrack5->Update();
    if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
      NPages++;
      TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
      TString EPSNameO = EPSName + TString("[");
      TString EPSNameC = EPSName + TString("]");
    
      ccumultrack5->Print(EPSNameO.Data());
      ccumultrack5->Print(EPSName.Data());
      ccumultrack5->Print(EPSNameC.Data());
      //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
      //gSystem->Exec(command.Data());
      //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
      //gSystem->Exec(command.Data());
      //command = TString("rm -rf ") + EPSName;
      //gSystem->Exec(command.Data());
    }
  }

  Char_t name[50], title[100];
  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    if(!UsingTrackerResolution) {
      sprintf(name,  "cresAtDUT_Pl%d",ThePlaneNumber);
      sprintf(title, "Hit-track residual at DUT, plane %d",ThePlaneNumber);
    }
    else {
      sprintf(name,  "cresAtDUT_Pl%d_step%d",ThePlaneNumber,istep+1);
      sprintf(title, "Hit-track residual at DUT, plane %d, for step %d",ThePlaneNumber,istep+1);
    }

    cresAtDUT[istep] = new TCanvas(name,title);
    cresAtDUT[istep]->Divide(2,1);
    cresAtDUT[istep]->cd(1);
    gPad->SetFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    if(hTrackResidualAtDUT_U[istep]->Integral() > 10) {
      TF1* fitgsU = hTrackResidualAtDUT_U[istep]->GetFunction("gaus");
      if(fitgsU != NULL) fitgsU->SetLineColor(2);
    }
    hTrackResidualAtDUT_U[istep]->Draw();
    cresAtDUT[istep]->cd(2);
    gPad->SetFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    if(hTrackResidualAtDUT_V[istep]->Integral() > 10) {
      TF1* fitgsV = hTrackResidualAtDUT_V[istep]->GetFunction("gaus");
      if(fitgsV != NULL) fitgsV->SetLineColor(2);
    }
    hTrackResidualAtDUT_V[istep]->Draw();

    if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
      NPages++;
      TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
      TString EPSNameO = EPSName + TString("[");
      TString EPSNameC = EPSName + TString("]");
    
      cresAtDUT[istep]->Print(EPSNameO.Data());
      cresAtDUT[istep]->Print(EPSName.Data());
      cresAtDUT[istep]->Print(EPSNameC.Data());
      //if(istep == 0 && 
	//StandAlone && 
	//UsingTrackerResolution
      //) {
	//command = TString("mv ") + EPSName + TString("  ") + EPSName_final;
	//gSystem->Exec(command.Data());
      //}
      //else {
        //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
        //gSystem->Exec(command.Data());
        //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
        //gSystem->Exec(command.Data());
        //command = TString("rm -rf ") + EPSName;
        //gSystem->Exec(command.Data());
      //}
      
    }
  }

  
  if(UsingTrackerResolution) {
    leg_TelResMC = new TLegend(0.20,0.75,0.45,0.88);
    leg_TelResMC->SetFillColor(10);
    leg_TelResMC->AddEntry(h_TelescopeResolution_Scan_U,"#sigma_{Tel}",                             "l");
    leg_TelResMC->AddEntry(h_ResidualAtDUT_Scan_U,      "#sqrt{#sigma^{2}_{Tel} + #sigma^{2}_{sp}}","l");
    cresAtDUT_vs_SensorSP = new TCanvas("cresAtDUT_vs_SensorSP","Telescope resolution vs Sensors Spatial resolution");
    cresAtDUT_vs_SensorSP->Divide(2,1);
    cresAtDUT_vs_SensorSP->cd(1);
    gPad->SetFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    double Maximum = -1.0e+20;
    double Minimum = +1.0e+20;
    double porcent = 0.10;
    for(int i=0;i<h_TelescopeResolution_Scan_U->GetXaxis()->GetNbins();i++) {
      double v,e;
      v = h_TelescopeResolution_Scan_U->GetBinContent(i+1);
      e = h_TelescopeResolution_Scan_U->GetBinError(i+1);
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
      
      v = h_ResidualAtDUT_Scan_U->GetBinContent(i+1);
      e = h_ResidualAtDUT_Scan_U->GetBinError(i+1);
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }
    h_TelescopeResolution_Scan_U->SetMaximum(Maximum + porcent*(Maximum - Minimum));
    h_TelescopeResolution_Scan_U->SetMinimum(Minimum - porcent*(Maximum - Minimum));
    h_ResidualAtDUT_Scan_U->SetMaximum(Maximum + porcent*(Maximum - Minimum));
    h_ResidualAtDUT_Scan_U->SetMinimum(Minimum - porcent*(Maximum - Minimum));
    h_TelescopeResolution_Scan_U->Draw();
    h_ResidualAtDUT_Scan_U->Draw("same");
    leg_TelResMC->Draw("same");

    lSolResolutionU1 = new TLine(h_ResidualAtDUT_Scan_U->GetXaxis()->GetXmin(),DUT_residualU_CGDSF[0],
	  			 DUT_resolutionU_CGDSF[0],DUT_residualU_CGDSF[0]);
    lSolResolutionU1->SetLineColor(h_ResidualAtDUT_Scan_U->GetLineColor());
    lSolResolutionU1->SetLineWidth(2);
    lSolResolutionU1->SetLineStyle(2);
    lSolResolutionU2 = new TLine(DUT_resolutionU_CGDSF[0],h_ResidualAtDUT_Scan_U->GetMinimum(),
				 DUT_resolutionU_CGDSF[0],DUT_residualU_CGDSF[0]);
    lSolResolutionU2->SetLineColor(h_ResidualAtDUT_Scan_U->GetLineColor());
    lSolResolutionU2->SetLineWidth(2);
    lSolResolutionU2->SetLineStyle(2);
    lSolResolutionU3 = new TLine(h_ResidualAtDUT_Scan_U->GetXaxis()->GetXmin(),Tel_resolutionU_CGDSF[0],
  			         DUT_resolutionU_CGDSF[0],                     Tel_resolutionU_CGDSF[0]);
    lSolResolutionU3->SetLineColor(h_TelescopeResolution_Scan_U->GetLineColor());
    lSolResolutionU3->SetLineWidth(2);
    lSolResolutionU3->SetLineStyle(2);

    if(DUT_resolutionU_CGDSF[0] > 0) {
      lSolResolutionU1->Draw();
      lSolResolutionU2->Draw();
      lSolResolutionU3->Draw();
    }

    cresAtDUT_vs_SensorSP->cd(2);
    gPad->SetFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_TelescopeResolution_Scan_V->Draw();
    Maximum = -1.0e+20;
    Minimum = +1.0e+20;
    porcent = 0.10;
    for(int i=0;i<h_TelescopeResolution_Scan_V->GetXaxis()->GetNbins();i++) {
      double v,e;
      v = h_TelescopeResolution_Scan_V->GetBinContent(i+1);
      e = h_TelescopeResolution_Scan_V->GetBinError(i+1);
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
      
      v = h_ResidualAtDUT_Scan_V->GetBinContent(i+1);
      e = h_ResidualAtDUT_Scan_V->GetBinError(i+1);
      if(Maximum < v+e) Maximum = v+e;
      if(Minimum > v-e) Minimum = v-e;
    }
    h_TelescopeResolution_Scan_V->SetMaximum(Maximum + porcent*(Maximum - Minimum));
    h_TelescopeResolution_Scan_V->SetMinimum(Minimum - porcent*(Maximum - Minimum));
    h_ResidualAtDUT_Scan_V->SetMaximum(Maximum + porcent*(Maximum - Minimum));
    h_ResidualAtDUT_Scan_V->SetMinimum(Minimum - porcent*(Maximum - Minimum));
    h_ResidualAtDUT_Scan_V->Draw("same");

    lSolResolutionV1 = new TLine(h_ResidualAtDUT_Scan_V->GetXaxis()->GetXmin(),DUT_residualV_CGDSF[0],
				 DUT_resolutionV_CGDSF[0],DUT_residualV_CGDSF[0]);
    lSolResolutionV1->SetLineColor(h_ResidualAtDUT_Scan_V->GetLineColor());
    lSolResolutionV1->SetLineWidth(2);
    lSolResolutionV1->SetLineStyle(2);
    lSolResolutionV2 = new TLine(DUT_resolutionV_CGDSF[0],h_ResidualAtDUT_Scan_V->GetMinimum(),
				 DUT_resolutionV_CGDSF[0],DUT_residualV_CGDSF[0]);
    lSolResolutionV2->SetLineColor(h_ResidualAtDUT_Scan_V->GetLineColor());
    lSolResolutionV2->SetLineWidth(2);
    lSolResolutionV2->SetLineStyle(2);
    lSolResolutionV3 = new TLine(h_ResidualAtDUT_Scan_V->GetXaxis()->GetXmin(),Tel_resolutionV_CGDSF[0],
				 DUT_resolutionV_CGDSF[0],                     Tel_resolutionV_CGDSF[0]);
    lSolResolutionV3->SetLineColor(h_TelescopeResolution_Scan_V->GetLineColor());
    lSolResolutionV3->SetLineWidth(2);
    lSolResolutionV3->SetLineStyle(2);

    if(DUT_resolutionV_CGDSF[0] > 0) {
      lSolResolutionV1->Draw();
      lSolResolutionV2->Draw();
      lSolResolutionV3->Draw();
    }

    if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
      NPages++;
      TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
      TString EPSNameO = EPSName + TString("[");
      TString EPSNameC = EPSName + TString("]");
    
      cresAtDUT_vs_SensorSP->Print(EPSNameO.Data());
      cresAtDUT_vs_SensorSP->Print(EPSName.Data());
      cresAtDUT_vs_SensorSP->Print(EPSNameC.Data());
      //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
      //gSystem->Exec(command.Data());
      //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
      //gSystem->Exec(command.Data());
      //command = TString("rm -rf ") + EPSName;
      //gSystem->Exec(command.Data());
    }
  }


  if(StandAlone && fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    sprintf(fOutName,"MCStandAlone_run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
    sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
    TString EPSName_final = TString(CreateGlobalResultDir()) + TString(fOutName) + TString(".pdf");
 
    TString command;
    TString ListOfFiles("");
    for(int i=0;i<NPages;i++) {
      TString TmpFileName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(i+1) + (".pdf");
      ListOfFiles += TmpFileName + TString("   ");
    }
    command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_final + TString(" -dBATCH ");
    command += ListOfFiles;
    gSystem->Exec(command.Data());
    command = TString("rm ");
    command += ListOfFiles;
    gSystem->Exec(command.Data());
  }

  //--------------WRITE IN A ROOT FILE
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

  Char_t ResultFileName2[50];
  if(!StandAlone) sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  else            sprintf(ResultFileName2,"MCStandAlone_run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);;
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");

  if(!UsingTrackerResolution) {
    ccumultrack3->Write();
    ccumultrack4->Write();
    ccumultrack5->Write();

    hTrackSlopeX_MC->Write();
    hTrackSlopeY_MC->Write();
    hTrackNHits_MC->Write();
    hTrackPlanesUsed_MC->Write();
    hTrackChi2_MC->Write();

    for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { 
      if(hResidualU_MC[iPlane-1]->GetEntries()>0) hResidualU_MC[iPlane-1]->Write();
      if(hResidualV_MC[iPlane-1]->GetEntries()>0) hResidualV_MC[iPlane-1]->Write();    
    }
  }

  for(int istep=0;istep<Bins_GlobalResolution;istep++) {
    cresAtDUT[istep]->Write();
    hTrackResidualAtDUT_U[istep]->Write();
    hTrackResidualAtDUT_V[istep]->Write();
  }

  if(UsingTrackerResolution) {
    cresAtDUT_vs_SensorSP->Write();
    h_TelescopeResolution_Scan_U->Write();
    h_TelescopeResolution_Scan_V->Write();
    h_ResidualAtDUT_Scan_U->Write();
    h_ResidualAtDUT_Scan_V->Write();
  }

  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;

  MHist::Dir();

}
//__________________________________________________________________________________________
//
void MimosaAnalysis::CompareBadGoodRaw()
{
// compare the baseline of good events and bad events 
//(i.e., with or without a hit where the track position is)

  if(!CheckIfDone("mimosall")) return;

UsedMacro[5] = 1;
ccomp = new TCanvas("ccomp","Comparebadgoodraw",450,10,800,900);
ccomp->Divide(2,4);

ccomp->cd(1);
//hraw1goodone->SetStats(1111);
//AutoZoom(hraw1goodone,"All")->Draw();
AutoZoom(hraw1Noise,"All")->Draw();

ccomp->cd(2);
//hraw1badone->SetStats(1111);
//AutoZoom(hraw1badone,"All")->Draw();
AutoZoom(hraw1Pedestal,"All")->Draw();

ccomp->cd(3);
//AutoZoom(hraw1goodone_time,"All")->Draw("colz");
 if(hraw1CDS->GetEntries()!=0){gPad->SetLogy(1);}
AutoZoom(hraw1CDS,"All")->Draw();

ccomp->cd(4);
 if(hraw1Signal->GetEntries()!=0){gPad->SetLogy(1);}
//AutoZoom(hraw1badone_time,"All")->Draw("colz");
AutoZoom(hraw1Signal,"All")->Draw();

ccomp->cd(5);
gPad->SetLogy(0);
//AutoZoom(hraw1goodoneNoise_time,"All")->Draw("colz");
AutoZoom(hraw1Noise_time,"All")->Draw("colz");

ccomp->cd(6);
//AutoZoom(hraw1badoneNoise_time,"All")->Draw("colz");
AutoZoom(hraw1Pedestal_time,"All")->Draw("colz");

ccomp->cd(7);
//AutoZoom(hRealTrackNoise_time,"All")->Draw("colz");
AutoZoom(hraw1CDS_time,"All")->Draw("colz");

ccomp->cd(8);
AutoZoom(hraw1Signal_time,"All")->Draw("colz");


ccomp2 = new TCanvas("ccomp2","Comparebadgoodraw 2",450,10,800,900);
ccomp2->Divide(2,4);

ccomp2->cd(1);
 if(hraw1PFrfr1GOOD->GetEntries()!=0){gPad->SetLogy(1);}
AutoZoom(hraw1PFrfr1GOOD,"All")->Draw();

ccomp2->cd(2);
 if(hraw1PFrfr1BAD->GetEntries()!=0){gPad->SetLogy(1);}
AutoZoom(hraw1PFrfr1BAD,"All")->Draw();

ccomp2->cd(3);
 if(hraw1PFrfr2GOOD->GetEntries()!=0){gPad->SetLogy(1);}
AutoZoom(hraw1PFrfr2GOOD,"All")->Draw();

ccomp2->cd(4);
 if(hraw1PFrfr2BAD->GetEntries()!=0){gPad->SetLogy(1);}
AutoZoom(hraw1PFrfr2BAD,"All")->Draw();

ccomp2->cd(5);
gPad->SetLogy(0);
AutoZoom(hraw1PFrfr1GOOD_time,"All")->Draw("colz");

ccomp2->cd(6);
AutoZoom(hraw1PFrfr1BAD_time,"All")->Draw("colz");

ccomp2->cd(7);
AutoZoom(hraw1NoiseGOOD_time,"All")->Draw("colz");

ccomp2->cd(8);
AutoZoom(hraw1NoiseBAD_time,"All")->Draw("colz");

ccomp3 = new TCanvas("ccomp3","Comparebadgoodraw 3",450,10,800,900);
ccomp3->Divide(2,4);

//cd to result dir
 gSystem->cd(CreateGlobalResultDir());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

MHist::Dir();

}


//__________________________________________________________________________________________
void MimosaAnalysis::CheckAsymmetry()
{
  //checks assysmetry 

if(!CheckIfDone("mimosall")) return;

UsedMacro[3] = 1;
casym = new TCanvas("casym","Check Asymmetry",550,10,750,550);
casym->cd();
casym->Clear();
casym->SetTitle("Asymmetry");
casym->Divide(2,2);
casym->cd(1);
hQ3x345->SetFillColor(42);
hQ3x327->SetFillColor(42);
hQ3x345r->SetFillColor(42);
hQ3x327r->SetFillColor(42);

//set stat 11111
hQ3x345->SetStats(1111);
hQ3x327->SetStats(1111);
hQ3x345r->SetStats(1111);
hQ3x327r->SetStats(1111);
hQ3x345->Draw();
casym->cd(2);
hQ3x327->Draw();
casym->cd(3);
hQ3x345r->Draw();
casym->cd(4);
hQ3x327r->Draw();
//cd to result dir
 gSystem->cd(CreateGlobalResultDir());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

MHist::Dir();

}

//________________________________________________________________________________________
//
void MimosaAnalysis::MimosaSN() 
{

  // plot distribution Signal over Noise in the different pixels.
  // A. Besson september 2004
  // Modified: JB 2012/08/21 histo & canvas saved in file
  
  if(!CheckIfDone("mimosall")) return;
  
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleSize(0.06);
  gStyle->SetOptStat(1111);
  gStyle->SetPadLeftMargin(0.15);
  
  gStyle->SetTitleH(.065);
  gStyle->SetTitleH(.085);
  
  UsedMacro[9] = 1;
  
  Int_t MimosaType=RunNumber/1000;
  
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);
  
  
  
  // ---> 1st canvas.
  // --- Make CANVAS
  
  Char_t nomcanvas[50]; 
  Char_t titlecanvas[80]; 
  Char_t nomPadsn[80]; 
  Char_t nomPadsntitle[50]; 
  Char_t Headersn[120];
  
  for (Int_t ii=0 ; ii<numcanvasSN; ii++){
    sprintf(nomcanvas,"cSN%d",ii+1); 
    sprintf(titlecanvas,"Check Signal / Noise (%d)",ii+1);
    cSN[ii] = new TCanvas(nomcanvas,titlecanvas,500,10,900,1000);
    cSN[ii]->Draw();
    cSN[ii]->Clear();
    cSN[ii]->SetBorderMode(0); //ligne de bordure sur le canvas.
  }
  
  // --- ALL PADS 
  cout<<"MimosaSN()  make all pads..."<<endl;
  const Int_t numpad1X = 3;
  const Int_t numpad1Y = 3;// = NPlane per canvas
  const Int_t numtotpad1 = numpad1X*numpad1Y*numcanvasSN;
  TPad *mpadSN[numtotpad1];
  TPad *mpadSNtitle[numcanvasSN];
  
  Float_t pad_Xwidth1 = 1 / float(numpad1X); 
  Float_t pad_Ywidth1 = 0.94 / float(numpad1Y); 
  
  for (Int_t ii=0 ; ii<numcanvasSN; ii++){
    cSN[ii]->cd();
    for (Int_t iy=0 ; iy<numpad1Y; iy++){
      for (Int_t ix=0 ; ix<numpad1X; ix++){
        Int_t iloop =ix+iy*numpad1X+ii*numpad1X*numpad1Y;
        
        sprintf(nomPadsn,"mpadSN%d_%d_%d",ii,ix,iy); 
        mpadSN[iloop] = new TPad(nomPadsn,"",(ix*pad_Xwidth1)+0.01,(iy*pad_Ywidth1)+0.01,(ix+1)*(pad_Xwidth1)-0.0001,(iy+1)*(pad_Ywidth1));
        mpadSN[iloop]->Draw();
      }
    }
    cSN[ii]->cd();
    //-------Draw title:
    sprintf(nomPadsntitle,"mpadtitle%d",ii); 
    mpadSNtitle[ii] = new TPad(nomPadsntitle,"",0.05,0.95,0.95,0.99);
    mpadSNtitle[ii]->Draw();
    mpadSNtitle[ii]->cd();
    TText *ttt = new TText();
    ttt->SetTextFont(25);
    ttt->SetTextColor(9);
    ttt->SetTextSize(.4);
    ttt->SetTextAlign(12);
    ttt->SetTextFont(22);
    sprintf(Headersn,"Signal / Noise (%d) M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",
            ii+1,MimosaType,RunNumber,
            ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
    ttt->DrawText( 0.05,0.6,Headersn);
  }
  
#if 0
  // --- PLOTS
  Float_t Xmin1,Xmax1;
  Float_t Ymin1,Ymax1;
  switch(MimosaType){
    case 1:
      Xmin1 = -4000.0;
      Xmax1 = +4000.0;
      Ymin1 = -10.0;
      Ymax1 = 10.0;
      break;
    case 2:
      Xmin1 = -4000.0;
      Xmax1 = +4000.0;
      Ymin1 = -10.0;
      Ymax1 = 10.0;
      break;
    case 5:
      Xmin1 = -4000.0;
      Xmax1 = +4000.0;
      Ymin1 = -10.0;
      Ymax1 = 10.0;
      break;
    case 9:
      Xmin1 = -4000.0;
      Xmax1 = +4000.0;
      Ymin1 = -10.0;
      Ymax1 = 10.0;
      break;
    default:
      Xmin1 = -4000.0;
      Xmax1 = +4000.0;
      Ymin1 = -10.0;
      Ymax1 = 10.0;
  }
#endif
  Int_t drawn = 0;
  
  //----------set axis range for all the 2D plots.
  Float_t Xaxismaxrange = 140.0;
  Float_t Yaxismaxrange = 140.0;
  for (Int_t ii=0 ; ii<jpixmax; ii++){
    hsn_pix_0[ii]->SetAxisRange(0,Xaxismaxrange,"X");
    hsn_pix_1[ii]->SetAxisRange(0,Xaxismaxrange,"X");
    hsn_pix_0[ii]->SetAxisRange(0,Yaxismaxrange,"Y");
    hsn_pix_1[ii]->SetAxisRange(0,Yaxismaxrange,"Y");
  }
  
  cout<<"Check Signal / Noise: plots..."<<endl;
  for (Int_t ii=0 ; ii<numcanvasSN; ii++){
    for (Int_t iy=0 ; iy<numpad1Y; iy++){
      for (Int_t ix=0 ; ix<numpad1X; ix++){
        drawn = 0;
        Int_t iloop =ix+iy*numpad1X+ii*numpad1X*numpad1Y;
        mpadSN[iloop]->cd();
        if(ii==0){
          if((iloop<=8)&&(iloop>=6)){hsnn[iloop]->SetAxisRange(0,20,"X");}
          if((iloop<=5)&&(iloop>=3)){hsnn[iloop]->SetAxisRange(0,40,"X");}
          if((iloop<=2)&&(iloop>=0)){hsnn[iloop]->SetAxisRange(0,140,"X");}
          hsnn[iloop]->Draw();drawn = 1;
        }
        if(ii==1){
          if(iloop == (0 + ii*numpad1X*numpad1Y)){hsn_pix_0[0]->Draw();drawn = 1;}
          else if(iloop == (1 + ii*numpad1X*numpad1Y)){hsn_pix_0[1]->Draw();drawn = 1;}
          else if(iloop == (2 + ii*numpad1X*numpad1Y)){hsn_pix_0[2]->Draw();drawn = 1;}
          else if(iloop == (3 + ii*numpad1X*numpad1Y)){hsn_pix_0[3]->Draw();drawn = 1;}
          else if(iloop == (4 + ii*numpad1X*numpad1Y)){hsn_pix_0[4]->Draw();drawn = 1;}
          else if(iloop == (5 + ii*numpad1X*numpad1Y)){hsn_pix_0[5]->Draw();drawn = 1;}
          else if(iloop == (6 + ii*numpad1X*numpad1Y)){hsn_pix_0[6]->Draw();drawn = 1;}
          else if(iloop == (7 + ii*numpad1X*numpad1Y)){hsn_pix_0[7]->Draw();drawn = 1;}
          else if(iloop == (8 + ii*numpad1X*numpad1Y)){hsnc->Draw();drawn = 1;}		
          else {hdummy->Draw();}
        }
        if(ii==2){
          if(iloop == (0 + ii*numpad1X*numpad1Y)){hsn_pix_1[0]->Draw();drawn = 1;}
          else if(iloop == (1 + ii*numpad1X*numpad1Y)){hsn_pix_1[1]->Draw();drawn = 1;}
          else if(iloop == (2 + ii*numpad1X*numpad1Y)){hsn_pix_1[2]->Draw();drawn = 1;}
          else if(iloop == (3 + ii*numpad1X*numpad1Y)){hsn_pix_1[3]->Draw();drawn = 1;}
          else if(iloop == (4 + ii*numpad1X*numpad1Y)){hsn_pix_1[4]->Draw();drawn = 1;}
          else if(iloop == (5 + ii*numpad1X*numpad1Y)){hsn_pix_1[5]->Draw();drawn = 1;}
          else if(iloop == (6 + ii*numpad1X*numpad1Y)){hsn_pix_1[6]->Draw();drawn = 1;}
          else if(iloop == (7 + ii*numpad1X*numpad1Y)){hsn_pix_1[7]->Draw();drawn = 1;}
          else {hdummy->Draw();}
        }
        if(ii==3){
          if(iloop == (0 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[0]->Draw("colz");drawn = 1;}
          else if(iloop == (1 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[1]->Draw("colz");drawn = 1;}
          else if(iloop == (2 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[2]->Draw("colz");drawn = 1;}
          else if(iloop == (3 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[3]->Draw("colz");drawn = 1;}
          else if(iloop == (4 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[4]->Draw("colz");drawn = 1;}
          else if(iloop == (5 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[5]->Draw("colz");drawn = 1;}
          else if(iloop == (6 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[6]->Draw("colz");drawn = 1;}
          else if(iloop == (7 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_0[7]->Draw("colz");drawn = 1;}
          else {hdummy->Draw();}
        }
        if(ii==4){
          if(iloop == (0 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[0]->Draw("colz");drawn = 1;}
          else if(iloop == (1 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[1]->Draw("colz");drawn = 1;}
          else if(iloop == (2 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[2]->Draw("colz");drawn = 1;}
          else if(iloop == (3 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[3]->Draw("colz");drawn = 1;}
          else if(iloop == (4 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[4]->Draw("colz");drawn = 1;}
          else if(iloop == (5 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[5]->Draw("colz");drawn = 1;}
          else if(iloop == (6 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[6]->Draw("colz");drawn = 1;}
          else if(iloop == (7 + ii*numpad1X*numpad1Y)){hsn_seed_vs_pix_1[7]->Draw("colz");drawn = 1;}
          else {hdummy->Draw();}
        }
        if(ii==5){
          if(iloop ==      (0 + ii*numpad1X*numpad1Y)){hindivq[0]->Fit("landau");hindivq[0]->Draw();drawn = 1;}
          else if(iloop == (1 + ii*numpad1X*numpad1Y)){hindivq[1]->Fit("landau");hindivq[1]->Draw();drawn = 1;}
          else if(iloop == (2 + ii*numpad1X*numpad1Y)){hindivq[2]->Fit("landau");hindivq[2]->Draw();drawn = 1;}
          else if(iloop == (3 + ii*numpad1X*numpad1Y)){hindivq[3]->Fit("landau");hindivq[3]->Draw();drawn = 1;}
          else if(iloop == (4 + ii*numpad1X*numpad1Y)){hindivq[4]->Fit("landau");hindivq[4]->Draw();drawn = 1;}
          else if(iloop == (5 + ii*numpad1X*numpad1Y)){hindivq[5]->Fit("landau");hindivq[5]->Draw();drawn = 1;}
          else if(iloop == (6 + ii*numpad1X*numpad1Y)){hindivq[6]->Fit("landau");hindivq[6]->Draw();drawn = 1;}
          else if(iloop == (7 + ii*numpad1X*numpad1Y)){hindivq[7]->Fit("landau");hindivq[7]->Draw();drawn = 1;}
          else if(iloop == (8 + ii*numpad1X*numpad1Y)){hindivq[8]->Fit("landau");hindivq[8]->Draw();drawn = 1;}
          else {hdummy->Draw();}
        }
        mpadSN[iloop]->Update();
        if(drawn==1){
          mpadSN[iloop]->cd();
          if((ii==0)||(ii==1)||(ii==2)){
          }else{
          }
        }
      }
    }
  }
  gPad->Update();
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  //--------------------------------
  //--------------WRITE IN A ROOT FILE:
  // JB 2012/08/21
  
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileName[50];
  sprintf(ResultFileName,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName,"%s", fTool.LocalizeDirName( ResultFileName));
  TFile *ResultRootFile=new TFile(ResultFileName,"UPDATE");
  //---histos:
  for( Int_t i=0; i<MaxNofPixelsInCluster; i++ ) {
    hindivq[i]->Write();
    hsnn[i]->Write();    
  }
  
  //canvas:
  for (Int_t ii=0 ; ii<numcanvasSN; ii++){
    cSN[ii]->Write();
  }
  
  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  
  MHist::Dir();

}

//_____________________________________________________________________________________________
//

void MimosaAnalysis::checkRefPlane()
{
// Macro to plot distribution about reference planes. 
// A. Besson august 2004

if(!CheckIfDone("mimosapro")) return;

UsedMacro[8] = 1;

Int_t MimosaType=RunNumber/1000;

gStyle->SetTitleOffset(1.);
gStyle->SetOptTitle(1);
gStyle->SetOptStat(1111);

// ---> 1st canvas.
// --- Make CANVAS


Char_t nomcanvas[50]; 
Char_t titlecanvas[80]; 

for (Int_t ii=0 ; ii<numcanvas; ii++){
  sprintf(nomcanvas,"cRef%d",ii); 
  sprintf(titlecanvas,"Check Reference Planes (%d)",ii);
  cRef[ii] = new TCanvas(nomcanvas,titlecanvas,500,10,900,1000);
  cRef[ii]->Draw();
  cRef[ii]->Clear();
  cRef[ii]->SetBorderMode(0); //ligne de bordure sur le canvas.
}
 

//---------------------------
// --- ALL PADS 
//---------------------------
cout<<"checkeRefPlane: make all pads..."<<endl;
const Int_t numpad1X = 4;
const Int_t numpad1Y = 4;// = NPlane per canvas
const Int_t numtotpad1 = numpad1X*numpad1Y*numcanvas;
TPad *mpadref[numtotpad1];
TPad *mpadreftitle[numcanvas];

Float_t pad_Xwidth1 = 1 / float(numpad1X); 
Float_t pad_Ywidth1 = 0.94 / float(numpad1Y); 

for (Int_t ii=0 ; ii<numcanvas; ii++){
  cRef[ii]->cd();
  for (Int_t iy=0 ; iy<numpad1Y; iy++){
    for (Int_t ix=0 ; ix<numpad1X; ix++){
      Int_t iloop =ix+iy*numpad1X+ii*numpad1X*numpad1Y;
      Char_t nomPad[50]; 
      sprintf(nomPad,"mpadref%d_%d_%d",ii,ix,iy); 
      mpadref[iloop] = new TPad(nomPad,"",(ix*pad_Xwidth1)+0.01,(iy*pad_Ywidth1)+0.01,(ix+1)*(pad_Xwidth1)-0.0001,(iy+1)*(pad_Ywidth1));
      mpadref[iloop]->Draw();
    }
  }
  cRef[ii]->cd();
  //-------Draw title:
  Char_t nomPadtitle[50]; 
  sprintf(nomPadtitle,"mpadtitle%d",ii); 
  mpadreftitle[ii] = new TPad(nomPadtitle,"",0.01,0.95,0.99,0.99);
  mpadreftitle[ii]->Draw();
  mpadreftitle[ii]->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.4);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  Char_t HeadercRS[100];
  sprintf(HeadercRS,"Ref Plane (1) M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,
	  ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
  ttt->DrawText( 0.05,0.6,HeadercRS);
}

// --- PLOTS
Float_t Xmin1,Xmax1;
Float_t Ymin1,Ymax1;

Float_t Xmin2 = 0.0;
Float_t Xmax2 = 30.0;

Float_t Ymin2 = -15.0;
Float_t Ymax2 = 15.0 ;

Float_t Xmin3 = 0.0;
Float_t Xmax3 = 256.0;

switch(MimosaType){
	case 1:
	  Xmin1 = -4000.0;
	  Xmax1 = +4000.0;
	  Ymin1 = -10.0;
	  Ymax1 = 10.0;
	  break;
	case 2:
	  Xmin1 = -4000.0;
	  Xmax1 = +4000.0;
	  Ymin1 = -10.0;
	  Ymax1 = 10.0;
	  break;
	case 5:
	  Xmin1 = -4000.0;
	  Xmax1 = +4000.0;
	  Ymin1 = -10.0;
	  Ymax1 = 10.0;
	  break;
	case 9:
	  Xmin1 = -4000.0;
	  Xmax1 = +4000.0;
	  Ymin1 = -10.0;
	  Ymax1 = 10.0;
	  break;
	default:
	  Xmin1 = -4000.0;
	  Xmax1 = +4000.0;
	  Ymin1 = -10.0;
	  Ymax1 = 10.0;
}

cout<<"checkeRefPlane: plots..."<<endl;
for (Int_t ii=0 ; ii<numcanvas; ii++){
  for (Int_t iy=0 ; iy<numpad1Y; iy++){
    for (Int_t ix=0 ; ix<numpad1X; ix++){
      Int_t iloop =ix+iy*numpad1X+ii*numpad1X*numpad1Y;
      mpadref[iloop]->cd();
      if((ii==0) || (ii==1)){
	switch(ix){
	case 0:
	  hRef_Tud_vs_Tv[iy+ii*numpad1Y]->SetAxisRange(Xmin1,Xmax1,"X");
	  hRef_Tud_vs_Tv[iy+ii*numpad1Y]->SetAxisRange(Ymin1,Ymax1,"Y");
	  hRef_Tud_vs_Tv[iy+ii*numpad1Y]->Draw("colz");
	  break;
	case 1:
	  hRef_Tud_vs_Tu[iy+ii*numpad1Y]->SetAxisRange(Xmin1,Xmax1,"X");
	  hRef_Tud_vs_Tu[iy+ii*numpad1Y]->SetAxisRange(Ymin1,Ymax1,"Y");
	  hRef_Tud_vs_Tu[iy+ii*numpad1Y]->Draw("colz");
	  break;
	case 2:
	  hRef_Tud_vs_Tu1[iy+ii*numpad1Y]->SetAxisRange(Xmin1,Xmax1,"X");
	  hRef_Tud_vs_Tu1[iy+ii*numpad1Y]->SetAxisRange(Ymin1,Ymax1,"Y");
	  hRef_Tud_vs_Tu1[iy+ii*numpad1Y]->Draw("colz");
	  break;
	case 3: 
	  hRef_Tud_vs_Tk1[iy+ii*numpad1Y]->SetAxisRange(Xmin3,Xmax3,"X");
	  hRef_Tud_vs_Tk1[iy+ii*numpad1Y]->SetAxisRange(Ymin1,Ymax1,"Y");
	  hRef_Tud_vs_Tk1[iy+ii*numpad1Y]->Draw("colz");
	  break;
	}
      }
      if((ii==2) || (ii==3)){
	switch(ix){
	case 0:
	  hRef_Tud_vs_Chi2[iy+(ii-2)*numpad1Y]->SetAxisRange(Xmin2,Xmax2,"X");
	  hRef_Tud_vs_Chi2[iy+(ii-2)*numpad1Y]->SetAxisRange(Ymin2,Ymax2,"Y");
	  hRef_Tud_vs_Chi2[iy+(ii-2)*numpad1Y]->Draw("colz");
	  break;
	}
      }
      mpadref[ix+iy*numpad1X]->Update();
    }
  }
}
gPad->Update();

//cd to result dir
 gSystem->cd(CreateGlobalResultDir());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
MHist::Dir();

}

//_________________________________________________________________________________________
//
void MimosaAnalysis::pixelcharge()
{
  //---- Macro to plot charge distribution in each pixel
  // A. Besson June 2004
  //
  // Modified: JB 2013/11/18 to add histos for all pixels

  if(!CheckIfDone("mimosall")) return;
  
  UsedMacro[7] = 1;
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);
  
  Int_t MimosaType=RunNumber/1000;
    
  Char_t Header[100];
  sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);

  //-----------------------------------
  // ---> canvas for 9 first pixels
  
  c5 = new TCanvas("c5","Pixel charge studies (1)",850,10,700,900);
  c5->Draw();
  c5->Clear();
  c5->SetBorderMode(0);
  c5->Clear();
  
  c5->cd();
  TPad* mpadtitle = new TPad("mpadtitle","",0.10,0.91,0.90,0.99);
  mpadtitle->SetGrid(1);
  mpadtitle->SetFillColor(19);
  mpadtitle->Draw();
  mpadtitle->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.3);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  ttt->DrawText( 0.05,0.6,Header);

  c5->cd();
  TPad* mpadhisto = new TPad("mpad1","",0.01,0.01,0.99,0.90);
  mpadhisto->SetGrid(1);
  mpadhisto->SetLeftMargin(0.2);
  mpadhisto->Draw();
  mpadhisto->Divide(3,3);
  
  for (Int_t i=0 ; i<9 && i<MaxNofPixelsInCluster; i++) {
    mpadhisto->cd(i+1);
    hindivq[i]->Draw();
  }
  mpadhisto->Update();


  //-----------------------------------
  // ---> canvas for pixels 10 to 25
  c6 = new TCanvas("c6","Pixel charge studies (2)",850,10,700,900);
  c6->Draw();
  c6->Clear();
  c6->SetBorderMode(0);
  c6->Clear();

  c6->cd();
  TPad* mpad2_title = new TPad("mpad2_title","",0.10,0.91,0.90,0.99);
  mpad2_title->SetGrid(1);
  mpad2_title->SetFillColor(19);
  mpad2_title->Draw();
  mpad2_title->cd();
  ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.3);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  ttt->DrawText( 0.05,0.6,Header);

  c6->cd();
  TPad* mpadhisto2 = new TPad("mpadhisto2","",0.01,0.01,0.99,0.90);
  mpadhisto2->SetGrid(1);
  mpadhisto2->SetLeftMargin(0.2);
  mpadhisto2->Draw();
  mpadhisto2->Divide(3,3);
  
  for (Int_t i=9 ; i<25 && i<MaxNofPixelsInCluster; i++) {
    mpadhisto2->cd(i-8);
    hindivq[i]->Draw();
  }
  mpadhisto2->Update();



  //-----------------------------------
  // ---> canvas for ratios and correlations

  c7 = new TCanvas("c7","Pixel charge studies (3)",850,10,700,900);
  c7->Draw();
  c7->Clear();
  c7->SetBorderMode(0);
  c7->cd();
  c7->Clear();
  
  TPad* mpad3_1 = new TPad("mpad3_1","",0.01,0.01,0.32,0.30);
  mpad3_1->SetGrid(1);
  mpad3_1->Draw();
  
  TPad* mpad3_2 = new TPad("mpad3_2","",0.33,0.01,0.66,0.30);
  mpad3_2->SetGrid(1);
  mpad3_2->Draw();
  
  TPad* mpad3_3 = new TPad("mpad3_3","title 1",0.67,0.01,0.99,0.30);
  mpad3_3->SetGrid(1);
  mpad3_3->Draw();

  TPad* mpad3_4 = new TPad("mpad3_4","",0.01,0.31,0.32,0.60);
  mpad3_4->SetGrid(1);
  mpad3_4->Draw();
  
  TPad* mpad3_5 = new TPad("mpad3_5","",0.33,0.31,0.66,0.60);
  mpad3_5->SetGrid(1);
  mpad3_5->Draw();
  
  TPad* mpad3_6 = new TPad("mpad3_6","",0.67,0.31,0.99,0.60);
  mpad3_6->SetGrid(1);
  mpad3_6->Draw();
  
  TPad* mpad3_7 = new TPad("mpad3_7","",0.01,0.61,0.32,0.9);
  mpad3_7->SetGrid(1);
  mpad3_7->Draw();
  
  TPad* mpad3_8 = new TPad("mpad3_8","",0.33,0.61,0.66,0.9);
  mpad3_8->SetGrid(1);
  mpad3_8->Draw();
  
  TPad* mpad3_9 = new TPad("mpad3_9","",0.67,0.61,0.99,0.9);
  mpad3_9->SetGrid(1);
  mpad3_9->Draw();
  
  TPad* mpad3_title = new TPad("mpad3_title","",0.10,0.91,0.85,0.99);
  mpad3_title->SetGrid(1);
  mpad3_title->SetFillColor(19);
  mpad3_title->Draw();
  mpad3_title->cd();
  ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.3);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  ttt->DrawText( 0.05,0.6,Header);
  
  mpad3_1->SetFillColor(19);
  mpad3_2->SetFillColor(19);
  mpad3_3->SetFillColor(19);
  mpad3_4->SetFillColor(19);
  mpad3_5->SetFillColor(19);
  mpad3_6->SetFillColor(19);
  mpad3_7->SetFillColor(19);
  mpad3_8->SetFillColor(19);
  mpad3_9->SetFillColor(19);
  
  //-----------------------------------
  mpad3_7->cd();
  mpad3_7->SetLogy(1);
  AutoZoom(hChargeRap_1_over_2,"Max")->Draw();
  mpad3_7->Update();
  //-----------------------------------
  mpad3_8->cd();
  mpad3_8->SetLogy(1);
  AutoZoom(hChargeRap_1_over_3,"Max")->Draw();
  mpad3_8->Update();
  //-----------------------------------
  mpad3_9->cd();
  mpad3_9->SetLogy(1);
  AutoZoom(hChargeRap_1_over_4,"Max")->Draw();
  mpad3_9->Update();
  //-----------------------------------
  mpad3_4->cd();
  mpad3_4->SetLogy(1);
  AutoZoom(hChargeRap_2_over_3,"Max")->Draw();
  mpad3_4->Update();
  //-----------------------------------
  mpad3_5->cd();
  mpad3_5->SetLogy(0);
  AutoZoom(hChargeCor_1_2,"Max")->Draw("colz");
  mpad3_5->Update();
  //-----------------------------------
  mpad3_6->cd();
  mpad3_6->SetLogy(0);
  AutoZoom(hChargeCor_1_2,"Max")->Draw("colz");
  mpad3_6->Update();
  //-----------------------------------
  mpad3_1->cd();
  mpad3_1->SetLogy(0);
  AutoZoom(hChargeCor_1_3,"Max")->Draw("colz");
  mpad3_1->Update();
  //-----------------------------------
  mpad3_2->cd();
  mpad3_2->SetLogy(0);
  AutoZoom(hChargeCor_2_3,"Max")->Draw("colz");
  mpad3_2->Update();
  //-----------------------------------
  mpad3_3->cd();
  mpad3_3->SetLogy(1);
  mpad3_3->Update();

//--------------WRITE IN A ROOT FILE:
/*
//cd to result dir
 gSystem->cd(CreateGlobalResultDir());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

Char_t ResultFileName[50];
sprintf(ResultFileName,"results_ana_M%d/run%dPl%d_ClCharge.root",MimosaType,RunNumber,ThePlaneNumber);
sprintf(ResultFileName,"%s", fTool.LocalizeDirName( ResultFileName)); // JB 2011/07/07
TFile *ResultRootFile=new TFile(ResultFileName,"UPDATE");

hChargeOrder1->Write();
hChargeOrder2->Write();
hChargeOrder3->Write();
hChargeOrder4->Write();
hChargeOrder5->Write();

hChargeRap_1_over_2->Write();
hChargeRap_1_over_3->Write();
hChargeRap_1_over_4->Write();
hChargeRap_1_over_3->Write();

hGOODChargeRap_1_over_2->Write();
hGOODChargeRap_1_over_3->Write();
hGOODChargeRap_1_over_4->Write();
hGOODChargeRap_2_over_3->Write();
hGOODChargeSum_4->Write();

hGOODChargeCor_1_2->Write();
hGOODChargeCor_1_3->Write();
hGOODChargeCor_1_4->Write();
hGOODChargeCor_2_3->Write();

c5->Write();
c6->Write();
c7->Write();
*/
/*
  ResultRootFile->Write();
  ResultRootFile->Close();
  delete ResultRootFile;
*/
MHist::Dir();

}

//____________________________________________________________________________________________
//
Double_t Gaussrandom()
{
  // Random gaussian value
  Double_t x = gRandom->Gaus(0,1); //mean, sigma
  return x;
}

//____________________________________________________________________________________________
//
Double_t SignalGauss(Double_t *x, Double_t *par)
{
  // The signal function: a gaussian

   Double_t arg = 0;
   if (par[2]) arg = (x[0] - par[1])/par[2];

   Double_t sig = par[0]*TMath::Exp(-0.5*arg*arg);
   return sig;
}


//____________________________________________________________________________________________
//
Double_t FComputeFakeRAte(Float_t  CUT_S2N_seed, Float_t CUT_S2N_neighbour, Int_t NumNeighbours)
{
  Double_t FakeRate;
  Double_t Pi = 3.141592653;
  Float_t gaussSigma = 1.0;
  Float_t gaussNorm = 1/(gaussSigma * sqrt(2*Pi));
  Float_t gaussMu = 0.0;

  Int_t npar2 = 3;
  TF1 *tempgauss = new TF1("tempgauss",SignalGauss,-100,100,npar2);
  tempgauss->SetParameter(0,gaussNorm);
  tempgauss->SetParameter(1,gaussMu);
  tempgauss->SetParameter(2,gaussSigma);
  
  Double_t proba1,proba2;
  proba1 =  tempgauss->Integral(CUT_S2N_seed,CUT_S2N_seed+50.0);
  proba2 =  tempgauss->Integral(CUT_S2N_neighbour,CUT_S2N_neighbour+50.0);
  FakeRate = proba1 * proba2;
  return FakeRate;
}


//__________________________________________________________________________________________
//
void MimosaAnalysis::MimosaOptimize()
{
  //fakes  studies

if(!CheckIfDone("mimosapro") && !CheckIfDone("mimosafakerate")) return;

UsedMacro[10] = 1;

gStyle->SetTitleOffset(1.);
gStyle->SetOptTitle(1);
gStyle->SetOptStat(1111);

//-----------VARIABLES
Int_t NumpixelsU = 512;
Int_t NumpixelsV = 512;
Float_t pitch = 30.0;

Float_t Surface = NumpixelsU * pitch * NumpixelsV * pitch * 0.001 * 0.001; //mm^2
Int_t Ntotpixel = NumpixelsU * NumpixelsV ;


Int_t Nummaxpixels = 8;
Int_t NumLoop = 100;
Int_t Numfake = 0;
Double_t SN[9];
Double_t SNN;
Float_t  CUT_S2N_seedvar;
Float_t  CUT_S2N_neighbourvar;
Int_t printrate = 100;

//----not standalone:
NumpixelsU = NofPixelInRaw;
NumpixelsV = NofPixelInColumn;
//pitch = PixelSize; // commented becasue not used, JB 2012/09/05 

//fake rate vs efficiency variables:
const Int_t Num_seedscan = 9;
const Int_t Num_SNNscan = 10;
Float_t CUT_S2N_seed_var2[Num_seedscan] = {3, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0};
Float_t efficiencyarray[Num_seedscan][Num_SNNscan];

//histograms
TH1F *SNseed  = new TH1F("SNseed","SN seed",200,-10.,10.);
TH1F *SNNeighbours  = new TH1F("SNNeighbours","SN Neighbours",200,-10.,10.);

TH1F *SNseedfake  = new TH1F("SNseedfake","SN seed fake",200,-10.,10.);
TH1F *SNNeighboursfake  = new TH1F("SNNeighboursfake","SN Neighbours fake",200,-10.,10.);

TH1F *SNseedSNok  = new TH1F("SNseedSNok","SN seed SNok",200,-10.,10.);
TH1F *SNNeighboursSNok  = new TH1F("SNNeighboursSNok","SN Neighbours SNok",200,-10.,10.);

TH1F *SNseedSNNok  = new TH1F("SNseedSNNok","SN seed SNNok",200,-10.,10.);
TH1F *SNNeighboursSNNok  = new TH1F("SNNeighboursSNNok","SN Neighbours SNNok",200,-10.,10.);

Int_t nbinfake = 16;
Axis_t maxfake = 8.0;
TH2F *fakerate_SN_SNN = new TH2F("fakerate_SN_SNN","Fake rate in the SN vs SNN plane",nbinfake,0.,maxfake,nbinfake,0.,maxfake);

Char_t nomfake[50],titlefake[50];

TH1F *fakerate_SN[Num_SNNscan];
for (Int_t j=0 ; j<Num_SNNscan ; j++) {    
sprintf(titlefake,"Fake rate vs SN with SNN=%3.1f",float(j)/2.0);// <---------------------------------------------------------
sprintf(nomfake,"Fakerate_vs_SN_%3.1f",float(j)/2.0);  // <---------------------------------------------------------
fakerate_SN[j] = new TH1F(nomfake,titlefake,nbinfake,0,maxfake);

}

TH2F *fakerate_SN_SNN_surface = new TH2F("fakerate_SN_SNN_surface","Fake rate in the SN vs SNN / mm^2 ",nbinfake,0.,maxfake,nbinfake,0.,maxfake);
TH2F *fakerate_SN_SNN_matrix = new TH2F("fakerate_SN_SNN_matrix","Fake rate in the SN vs SNN / matrix ",nbinfake,0.,maxfake,nbinfake,0.,maxfake);

// pour chaque set de coupure SN SNN1...SNN8


//---MAIN LOOP

for (Int_t nl=0 ; nl<NumLoop; nl++){

  //seed:
  SN[0] = Gaussrandom();
  SNN = 0.0;
  //loop over neighbours
  for (Int_t ii=0 ; ii<Nummaxpixels; ii++){
    SN[ii+1] = Gaussrandom();        
    SNN += SN[ii+1];
  }
  SNN = SNN / sqrt(8.0);
  //fill histo:
  SNseed->Fill(SN[0]);
  SNNeighbours->Fill(SNN);

  if(nl/printrate*printrate == nl){
    cout<<"nl SN SNN "<<nl<<" "<<SN[0]<<" "<<SN[1]<<" "<<SN[2]<<" "<<SN[3]<<" "<<SN[4]<<" "<<SN[5]<<" "<<SN[6]<<" "<<SN[7]<<" "<<SN[8]<<" "<<SNN<<endl;
  }
  if((SN[0] > CUT_S2N_seed)&&(SNN > CUT_S2N_neighbour)){
    Numfake++;
    SNseedfake->Fill(SN[0]);
    SNNeighboursfake->Fill(SNN);    
  }
  if(SN[0] > CUT_S2N_seed){
    SNseedSNok->Fill(SN[0]);
    SNNeighboursSNok->Fill(SNN);    
  }
  if(SNN > CUT_S2N_neighbour){
    SNseedSNNok->Fill(SN[0]);
    SNNeighboursSNNok->Fill(SNN); 
  }   
}
 cout<<" fake / num events = rate : "<<Numfake <<" / "<<NumLoop <<" = "<<float(Numfake)/float(NumLoop)<<endl;
//-----------------END OF MAIN LOOP

//   fake rate map
//----------------

Double_t Pi = 3.141592653;
Float_t gaussSigma = 1.0;
Float_t gaussNorm = 1/(gaussSigma * sqrt(2*Pi));
Float_t gaussMu = 0.0;
Int_t npar = 3;
TF1 *fgauss = new TF1("fgauss",SignalGauss,-20,20,npar);
fgauss->SetParameter(0,gaussNorm);
fgauss->SetParameter(1,gaussMu);
fgauss->SetParameter(2,gaussSigma);



CUT_S2N_seedvar = 0.0;
CUT_S2N_neighbourvar = 0.0;
for (Int_t i=0 ; i<nbinfake; i++){

  CUT_S2N_neighbourvar = 0.0;

  for (Int_t j=0 ; j<nbinfake; j++){

    Double_t tempfake=0.;
 
    fakerate_SN_SNN->Fill(CUT_S2N_neighbourvar*1.01,CUT_S2N_seedvar*1.01,tempfake);
    fakerate_SN_SNN_surface->Fill(CUT_S2N_neighbourvar*1.01,CUT_S2N_seedvar*1.01,tempfake*Ntotpixel/Surface);
    fakerate_SN_SNN_matrix->Fill(CUT_S2N_neighbourvar*1.01,CUT_S2N_seedvar*1.01,tempfake*Ntotpixel);
    CUT_S2N_neighbourvar += maxfake/float(nbinfake);

  }
  CUT_S2N_seedvar += maxfake/float(nbinfake);
}
//---compute fake rate for the different crowns

Int_t ii = 0;
Bool_t minfoundSN = kFALSE ;
Float_t BestcutSN = 0;
while(minfoundSN == kFALSE) {
  if(hsnc->GetBinContent(ii)>0.0){ 
    minfoundSN = kTRUE ;
    BestcutSN = hsnc->GetBinLowEdge(ii);
  }
  ii++;
}

Bool_t minfoundSNN[8];
Float_t BestcutSNN[8];
for(Int_t jj=0 ; jj<jpixmax ; jj++){
  minfoundSNN[jj] = kFALSE;
}
for(Int_t jj=0 ; jj<jpixmax ; jj++){
  ii=0;
  while(minfoundSNN[jj] == kFALSE) {
    if(hsn_pix_1[jj]->GetBinContent(ii)>0.0){ 
      minfoundSNN[jj] = kTRUE ;
      BestcutSNN[jj] = hsn_pix_1[jj]->GetBinLowEdge(ii);
    }
    ii++;
  }
}

 cout<<"++++++++++++++++++++++++++++++++++++++++++++"<<endl;
 cout<<"BestcutSN = "<< BestcutSN<<" [1] = "<<BestcutSNN[0] <<" [2] = "<<BestcutSNN[1]<<" [3] = "<<BestcutSNN[2]
<<" [4] = "<<BestcutSNN[3]<<" [5] = "<<BestcutSNN[4]<<" [6] = "<<BestcutSNN[5]<<" [7] = "<<BestcutSNN[6]
     <<" [8] = "<<BestcutSNN[7]<<endl;
 cout<<" fakerate SN Only = "<< BestcutSN<<"  ;  "<<FComputeFakeRAte(BestcutSN,-40.0,1)<<endl;
 cout<<" fakerate SN [1] = "<< BestcutSNN[0]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[0],1)<<endl;
 cout<<" fakerate SN [2] = "<< BestcutSNN[1]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[1],1)<<endl;
 cout<<" fakerate SN [3] = "<< BestcutSNN[2]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[2],1)<<endl;
 cout<<" fakerate SN [4] = "<< BestcutSNN[3]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[3],1)<<endl;
 cout<<" fakerate SN [5] = "<< BestcutSNN[4]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[4],1)<<endl;
 cout<<" fakerate SN [6] = "<< BestcutSNN[5]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[5],1)<<endl;
 cout<<" fakerate SN [7] = "<< BestcutSNN[6]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[6],1)<<endl;
 cout<<" fakerate SN [8] = "<< BestcutSNN[7]<<"  ;  "<<FComputeFakeRAte(BestcutSN,BestcutSNN[7],1)<<endl;
 cout<<"++++++++++++++++++++++++++++++++++++++++++++"<<endl;

//---compute fake rate vs data efficiency:

for(Int_t jj=0 ; jj<Num_seedscan ; jj++){
  cout<<CUT_S2N_seed_var2[jj]<<" ";
  for(Int_t ii=0 ; ii< Num_SNNscan ; ii++){
    Float_t SNNthreshold = float(ii/2.0);
    

    efficiencyarray[jj][ii] = hsn_seed_vs_pix_1[7]->Integral((int)SNNthreshold*2+1,281,(int)CUT_S2N_seed_var2[jj]*2,281);
    if(NtrkInMimo > 0){
      efficiencyarray[jj][ii] = efficiencyarray[jj][ii] / NtrkInMimo;
    }
    cout<<SNNthreshold<<" > "<<efficiencyarray[jj][ii]<<" ";
  }
  cout<<endl;
}


// pour chaque set de coupure SN SNN1...SNN8
// optimiser les coupures pour une efficacite donnee.
//en deduire le taux de fake.
// si un hit (SN 3 SNN 0) remplace un vrai hit qui est plus loin ?

// ---> 1st canvas.
// --- Make CANVAS

Char_t nomcanvasopt[50]; 
Char_t titlecanvasopt[80]; 

for (Int_t ii=0 ; ii<numcanvasOptimize; ii++){
  sprintf(nomcanvasopt,"cOptimize%d",ii+1); 
  sprintf(titlecanvasopt,"Mimosa Optimize (%d)",ii+1);
  cOptimize[ii] = new TCanvas(nomcanvasopt,titlecanvasopt,500,10,900,1000);
  cOptimize[ii]->Draw();
  cOptimize[ii]->Clear();
  cOptimize[ii]->SetBorderMode(0); //ligne de bordure sur le canvas.
}

// --- ALL PADS 
cout<<"MimosaSN(): make all pads..."<<endl;

// FIRST CANVAS :
cOptimize[0]->Divide(2,2);
 if(!fakerate_SN_SNN->GetMaximum() || !fakerate_SN_SNN_surface->GetMaximum() || !fakerate_SN_SNN_matrix->GetMaximum()) {Warning("InspectFake()","One TH2 has Max=0! , Investigate!"); return;}

//-------
 cOptimize[0]->cd(1);
 //cOptimize1_1
gPad->SetLogz(1);
 fakerate_SN_SNN->Draw("colz");
 //-------
 cOptimize[0]->cd(2);
 // cOptimize1_2
gPad->SetLogz(1);
 fakerate_SN_SNN_surface->Draw("colz");
 //-------
 cOptimize[0]->cd(3);

 // cOptimize1_3->
gPad->SetLogz(1);
 fakerate_SN_SNN_matrix->Draw("colz");
 //-------
 cOptimize[0]->cd(4);

 //cOptimize1_4
gPad->SetLogz(1);
 fakerate_SN[0]->Draw();
 for (Int_t j=0 ; j<Num_SNNscan ; j++) {    
   fakerate_SN[j]->Draw("same");
 }
 
//cd to result dir
 gSystem->cd(CreateGlobalResultDir());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

MHist::Dir();
}

//__________________________________________________________________________________________
//
void MimosaAnalysis::SaveGifFile()
{
// saves open canvaces to GIF and EPS files.
// A. Besson  June 2004

if(!CheckIfDone("mimosall")) return;

 Bool_t ProdGif=kTRUE;

//cd to result dir
 gSystem->cd(CreateGlobalResultDir());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;


Char_t HeaderSave[200];

//----MimosaResolution
if(UsedMacro[0] == 1){
sprintf(HeaderSave,"resolution_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) cres->Print(HeaderSave,"gif");
sprintf(HeaderSave,"resolution_%d_%d.eps",RunNumber,ThePlaneNumber);
cres->Print(HeaderSave,"eps");
}
//----MimosaClusterCharge
if(UsedMacro[1] == 1){
sprintf(HeaderSave,"clustercharge_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) MainCanvas->Print(HeaderSave,"gif");
sprintf(HeaderSave,"clustercharge_%d_%d.eps",RunNumber,ThePlaneNumber);
MainCanvas->Print(HeaderSave,"eps");
}
//----CheckMimosaAlign
if(UsedMacro[2] == 1){
sprintf(HeaderSave,"mimosaalignement_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) c2->Print(HeaderSave,"gif");
sprintf(HeaderSave,"mimosaalignement_%d_%d.eps",RunNumber,ThePlaneNumber);
c2->Print(HeaderSave,"eps");
}
//----CheckAsymmetry
if(UsedMacro[3] == 1){
sprintf(HeaderSave,"asymmetry_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) casym->Print(HeaderSave,"gif");
sprintf(HeaderSave,"asymmetry_%d_%d.eps",RunNumber,ThePlaneNumber);
casym->Print(HeaderSave,"eps");
}
//----InspectEfficiency
if(UsedMacro[4] == 1){
sprintf(HeaderSave,"efficiency_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) ceffi->Print(HeaderSave,"gif");
sprintf(HeaderSave,"efficiency_%d_%d.eps",RunNumber,ThePlaneNumber);
ceffi->Print(HeaderSave,"eps");
}
//----Comparebadgoodraw
if(UsedMacro[5] == 1){
sprintf(HeaderSave,"compare_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) ccomp->Print(HeaderSave,"gif");
sprintf(HeaderSave,"compare_%d_%d.eps",RunNumber,ThePlaneNumber);
ccomp->Print(HeaderSave,"eps");
sprintf(HeaderSave,"compare_2_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) ccomp2->Print(HeaderSave,"gif");
sprintf(HeaderSave,"compare_2_%d_%d.eps",RunNumber,ThePlaneNumber);
ccomp2->Print(HeaderSave,"eps");
sprintf(HeaderSave,"compare_3_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) ccomp3->Print(HeaderSave,"gif");
sprintf(HeaderSave,"compare_3_%d_%d.eps",RunNumber,ThePlaneNumber);
ccomp3->Print(HeaderSave,"eps");
}
//----checkreferencetracks
if(UsedMacro[6] == 1){
sprintf(HeaderSave,"referencetrack_1_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) c3->Print(HeaderSave,"gif");
sprintf(HeaderSave,"referencetrack_1_%d_%d.eps",RunNumber,ThePlaneNumber);
c3->Print(HeaderSave,"eps");
sprintf(HeaderSave,"referencetrack_2_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) c4->Print(HeaderSave,"gif");
sprintf(HeaderSave,"referencetrack_2_%d_%d.eps",RunNumber,ThePlaneNumber);
c4->Print(HeaderSave,"eps");
sprintf(HeaderSave,"referencetrack_3_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) c4_2->Print(HeaderSave,"gif");
sprintf(HeaderSave,"referencetrack_3_%d_%d.eps",RunNumber,ThePlaneNumber);
c4_2->Print(HeaderSave,"eps");
}
//----pixelcharge
if(UsedMacro[7] == 1){

sprintf(HeaderSave,"pixel_1_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) c5->Print(HeaderSave,"gif");
sprintf(HeaderSave,"pixel_2_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) c6->Print(HeaderSave,"gif");
sprintf(HeaderSave,"pixel_3_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) c7->Print(HeaderSave,"gif");
sprintf(HeaderSave,"pixel_1_%d_%d.eps",RunNumber,ThePlaneNumber);
c5->Print(HeaderSave,"eps");
sprintf(HeaderSave,"pixel_2_%d_%d.eps",RunNumber,ThePlaneNumber);
c6->Print(HeaderSave,"eps");
sprintf(HeaderSave,"pixel_3_%d_%d.eps",RunNumber,ThePlaneNumber);
c7->Print(HeaderSave,"eps");

}
//----checkRefPlane
if(UsedMacro[8] == 1){
  for(Int_t ii=0 ; ii<numcanvas; ii++){
    sprintf(HeaderSave,"telescope_%d_%d_%d.gif",ii,RunNumber,ThePlaneNumber);
      if(ProdGif) cRef[ii]->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"telescope_%d_%d_%d.eps",ii,RunNumber,ThePlaneNumber);
     cRef[ii]->Print(HeaderSave,"eps");
  }
}
//----MimosaSN
if(UsedMacro[9] == 1){
  for(Int_t ii=0 ; ii<numcanvasSN; ii++){
    sprintf(HeaderSave,"signalovernoise_%d_%d_%d.gif",ii,RunNumber,ThePlaneNumber);
     if(ProdGif) cSN[ii]->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"signalovernoise_%d_%d_%d.eps",ii,RunNumber,ThePlaneNumber);
    cSN[ii]->Print(HeaderSave,"eps");
  }
}

//----
if(UsedMacro[10] == 1){
  for(Int_t ii=0 ; ii<numcanvasOptimize; ii++){
    sprintf(HeaderSave,"Optimize_%d_%d_%d.gif",ii,RunNumber,ThePlaneNumber);
     if(ProdGif) cOptimize[ii]->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"Optimize_%d_%d_%d.eps",ii,RunNumber,ThePlaneNumber);
    cOptimize[ii]->Print(HeaderSave,"eps");
  }
}

//----MimosaBinaryplots()
 if(UsedMacro[11] == 1){
  sprintf(HeaderSave,"MimosaBinaryplots_%d_%d.gif",RunNumber,ThePlaneNumber);
  if(ProdGif) cM8->Print(HeaderSave,"gif");
  sprintf(HeaderSave,"MimosaBinaryplots_%d_%d.eps",RunNumber,ThePlaneNumber);
  cM8->Print(HeaderSave,"eps");
}

//----ADC
//---FakeRate()
if(UsedMacro[12] == 1){
sprintf(HeaderSave,"fakerate_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) cfake->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"fakerate_%d_%d.eps",RunNumber,ThePlaneNumber);
cfake->Print(HeaderSave,"eps");
}
//----ADC

//---MimosaPixelHomogeneity
if(UsedMacro[13] == 1){
sprintf(HeaderSave,"PixHom_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) PixHom->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"PixHom_%d_%d.eps",RunNumber,ThePlaneNumber);
PixHom->Print(HeaderSave,"eps");
sprintf(HeaderSave,"PixHom2_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) PixHom2->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"PixHom2_%d_%d.eps",RunNumber,ThePlaneNumber);
PixHom2->Print(HeaderSave,"eps");
sprintf(HeaderSave,"PixHom3_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) PixHom3->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"PixHom3_%d_%d.eps",RunNumber,ThePlaneNumber);
PixHom3->Print(HeaderSave,"eps");
sprintf(HeaderSave,"PixHom4_%d_%d.gif",RunNumber,ThePlaneNumber);
 if(ProdGif) PixHom4->Print(HeaderSave,"gif");
    sprintf(HeaderSave,"PixHom4_%d_%d.eps",RunNumber,ThePlaneNumber);
PixHom4->Print(HeaderSave,"eps");

}

// save all open canvas in a root file 
 sprintf(HeaderSave,"AllPlots_%d_%d.root",RunNumber,ThePlaneNumber);
 sprintf(HeaderSave,"%s", fTool.LocalizeDirName( HeaderSave)); // JB 2011/07/07
 TFile* AllPlots = new TFile(HeaderSave,"RECREATE");
 gROOT->GetListOfCanvases()->Write();
 AllPlots->Close(); delete AllPlots;
  printf("\n All plots saved in %s\n", AllPlots->GetName());


MHist::Dir();

}



//___________________________________________________________________________________________
//
void MimosaAnalysis::MimosaBinaryplots()
{
  //---- Macro to plot results for mimosa with binary output
  //
  // A. Besson september 2005
  // Modified: JB 2011/11/13 new histo npixCumul
  
  
  if(!CheckIfDone("mimosall")) return;
  
  UsedMacro[11] = 1;
  
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetLabelSize(0.06);
  gStyle->SetTitleSize(0.06);
  gStyle->SetOptStat(1111);
  gStyle->SetPadLeftMargin(0.15);  
  gStyle->SetTitleH(.065);
  gStyle->SetTitleH(.085);
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);
 
  // Generate cumulated ditribution of pixel multiplicity
  //  from histogram hnpix_c
  
  for (Int_t ibin=1; ibin<=hnpix_c->GetNbinsX(); ibin++) {
    hnpixCumul_c->SetBinContent( ibin, hnpix_c->Integral( ibin, hnpix_c->GetNbinsX()));
  }
  if( hnpix_c->Integral()>0. ) {
    hnpixCumul_c->Scale( 1./hnpix_c->Integral() );
  }

  char fOutName[200];
  sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //sprintf(tmpFile,"%s",fTool.LocalizeDirName(tmpFile));
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");

  // ---> 1st canvas.
  // --- Make CANVAS
  cM8 = new TCanvas("cM8","Binary ouput sensor",550,10,750,850);
  cM8->Draw();
  cM8->Clear();
  cM8->SetFillColor(0);
  cM8->SetBorderMode(0);
  
  gPad->cd();
 
  TPad* mpad1 = new TPad("mpad1","",0.01,0.01,0.32,0.30);
  mpad1->SetGrid(0);
  mpad1->SetLeftMargin(0.2);
  mpad1->Draw();
 
  TPad* mpad2 = new TPad("mpad2","",0.33,0.01,0.66,0.30);
  mpad2->SetGrid(1);
  mpad2->Draw();
  
  TPad* mpad3 = new TPad("mpad3","title 1",0.67,0.01,0.99,0.30);
  mpad3->SetGrid(1);
  mpad3->Draw();
  
  TPad* mpad4 = new TPad("mpad4","",0.01,0.31,0.32,0.60);
  mpad4->SetGrid(1);
  mpad4->Draw();
  
  TPad* mpad5 = new TPad("mpad5","",0.33,0.31,0.66,0.60);
  mpad5->SetGrid(1);
  mpad5->Draw();
  
  TPad* mpad6 = new TPad("mpad6","",0.67,0.31,0.99,0.60);
  mpad6->SetGrid(1);
  mpad6->Draw();
  
  TPad* mpad7 = new TPad("mpad7","",0.01,0.61,0.32,0.9);
  mpad7->SetGrid(1);
  mpad7->Draw();
  
  TPad* mpad8 = new TPad("mpad8","",0.33,0.61,0.66,0.9);
  mpad8->SetGrid(1);
  mpad8->Draw();
  
  TPad* mpad9 = new TPad("mpad9","",0.67,0.61,0.99,0.9);
  mpad9->SetGrid(1);
  mpad9->Draw();
  
  TPad* mpadtitle = new TPad("mpadtitle","",0.01,0.91,0.99,0.99);
  mpadtitle->SetGrid(1);
  mpadtitle->SetFillColor(19);
  mpadtitle->Draw();
  mpadtitle->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.3);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  Char_t Header[100];
  //---ab
  sprintf(Header,"M%d ; run %d; Pl %d, sub %d, dist %.0f; Gain %3.2f; eff %3.3f +- %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,ThesubmatrixNumber,TrackToHitDistanceLimit,calibration,MimosaEfficiency,MimosaEfficiency_ERR,CUT_S2N_seed,CUT_S2N_neighbour);
  //sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit);
  ttt->DrawText( 0.05,0.6,Header);
  
  mpad1->SetFillColor(19);
  mpad2->SetFillColor(19);
  mpad3->SetFillColor(19);
  mpad4->SetFillColor(19);
  mpad5->SetFillColor(19);
  mpad6->SetFillColor(19);
  mpad7->SetFillColor(19);
  mpad8->SetFillColor(19);
  mpad9->SetFillColor(19);

  mpad1->cd();
  AutoZoom(hBinary_NumberOf_1_submatrix,"Max")->Draw();
  mpad1->Update();
   
  mpad2->cd();
  AutoZoom(hBinary_NumberOf_1_ALL,"Max")->Draw();
  mpad2->Update();
 
  mpad3->cd();
  AutoZoom(hBinary_NumberOf_1_goodhit,"Max")->Draw();
  mpad3->Update();
  
  gStyle->SetOptStat(111111);  
  
  mpad4->cd();
  if(hBinary_Nhitperpixel_submatrix->GetEntries()!=0){ mpad4->SetLogy(1);}
  AutoZoom(hBinary_Nhitperpixel_submatrix,"Max")->Draw();
  mpad4->Update();
  
  mpad5->cd();
  if(hBinary_NhitRateperpixel_submatrix->GetEntries()!=0){ mpad5->SetLogy(1);}
  AutoZoom(hBinary_NhitRateperpixel_submatrix,"Max")->Draw();
  mpad5->Update();
  
  mpad6->cd();
  //if(hnpix_c->GetEntries()!=0){ mpad6->SetLogy(1);}
  AutoZoom(hnpix_c,"Max")->Draw();
  mpad6->Update();
  
  mpad7->cd();
  AutoZoom(hnpixCumul_c,"Max")->Draw();
  mpad7->Update();
  
  mpad8->cd();
  AutoZoom(hClusterMeanForm,"Max")->Draw("colz");
  mpad8->Update();
  
  mpad9->cd();
  //AutoZoom(hnWindows_c,"Max")->Draw();
  hnWindows_c->Draw();
  mpad9->Update();

  gStyle->SetOptStat(1111);  

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cM8->Print(EPSNameO.Data());
    cM8->Print(EPSName.Data());
    cM8->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // Save plots to ROOT file
  
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileName2[150];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");
  
  cM8->Write();
  
  hBinary_NumberOf_1_submatrix->Write();
  hBinary_NumberOf_1_ALL->Write();
  hBinary_NumberOf_1_goodhit->Write();
  hBinary_Nhitperpixel_submatrix->Write();
  hBinary_NhitRateperpixel_submatrix->Write();
  hnpix_c->Write();
  hnpixCumul_c->Write();
  hClusterMeanForm->Write();
  
  
  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();

}


//___________________________________________________________________________________________
//
void MimosaAnalysis::FakeRate()
{
  //---- Macro to plot experimental fake rate.
  // Originaly only valid if the Geomatrix window = 5. and when tracking,
  // but could also be used without tracking if no real impact is expected on the matrix.
  // Saves open canvas to GIF and EPS files.
  //
  // A. Besson february 2006.
  // Modified: JB 2010/07/27 to display more results
  // Modified: JB 2011/07/01 to write one more histo and suppress endesirable log scale

  if( !CheckIfDone("mimosapro") && !CheckIfDone("mimosafakerate") ) return;


  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

  UsedMacro[12] = 1;

  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetLabelSize(0.06);
  gStyle->SetTitleSize(0.06);
  gStyle->SetOptStat(1111);
  gStyle->SetPadLeftMargin(0.15);  
  gStyle->SetTitleH(.065);
  gStyle->SetTitleH(.085);
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);

  // ---> 1st canvas.
  // --- Make CANVAS

  cfake = new TCanvas("cfake","Experimental Fake rate",550,10,750,850);
  cfake->Draw();
  cfake->Clear();
  cfake->SetFillColor(0);
  cfake->SetBorderMode(0);

  gPad->cd();

  TPad* mpad0 = new TPad("mpad0","title 1",0.01,0.96,0.99,0.99);
  mpad0->SetGrid(1);
  mpad0->Draw();

  TPad* mpad00 = new TPad("mpad00","title 1",0.01,0.91,0.99,0.95);
  mpad00->SetGrid(1);
  mpad00->Draw();

  TPad* mpad1 = new TPad("mpad1","",0.01,0.01,0.32,0.30);
  mpad1->SetGrid(0);
  mpad1->SetLeftMargin(0.2);
  mpad1->Draw();

  TPad* mpad2 = new TPad("mpad2","",0.33,0.01,0.66,0.30);
  mpad2->SetGrid(1);
  mpad2->Draw();

  TPad* mpad3 = new TPad("mpad3","title 1",0.67,0.01,0.99,0.30);
  mpad3->SetGrid(1);
  mpad3->Draw();

  TPad* mpad4 = new TPad("mpad4","",0.01,0.31,0.32,0.60);
  mpad4->SetGrid(1);
  mpad4->Draw();

  TPad* mpad5 = new TPad("mpad5","",0.33,0.31,0.66,0.60);
  mpad5->SetGrid(1);
  mpad5->Draw();

  TPad* mpad6 = new TPad("mpad6","",0.67,0.31,0.99,0.60);
  mpad6->SetGrid(1);
  mpad6->Draw();

  TPad* mpad7 = new TPad("mpad7","",0.01,0.61,0.32,0.9);
  mpad7->SetGrid(1);
  mpad7->Draw();

  TPad* mpad8 = new TPad("mpad8","",0.33,0.61,0.66,0.9);
  mpad8->SetGrid(1);
  mpad8->Draw();

  TPad* mpad9 = new TPad("mpad9","",0.67,0.61,0.99,0.9);
  mpad9->SetGrid(1);
  mpad9->Draw();


  mpad1->SetFillColor(19);
  mpad2->SetFillColor(19);
  mpad3->SetFillColor(19);
  mpad4->SetFillColor(19);
  mpad5->SetFillColor(19);
  mpad6->SetFillColor(19);
  mpad7->SetFillColor(19);
  mpad8->SetFillColor(19);
  mpad9->SetFillColor(19);

  TLine *line = new TLine;
  line->SetLineColor(2);
  line->SetLineWidth(2);

  gStyle->SetOptStat(111111);  


  //-------------- Compute fake hit rate from histograms

  Float_t confidence = 0.;
  Float_t confidenceLevel = 6.;
  Float_t Fake_Ntot; //number of events to normalise to
  Float_t Fake_Nent = 0.; //number of reconstructed hits
  Float_t Fake_Rate = hNhitRateperpixel->GetMean();
  Float_t Fake_Err = 0.;
  Char_t Header1[100], Header2[100];

  cout<<endl<<"---------------------------------------------------------------------------------------------"<<endl;
  cout<<" CUT seed / CUT neighbours = "<< CUT_S2N_seed<<" / "<<CUT_S2N_neighbour<<endl;

  if( !CheckIfDone("mimosapro") ) { // If the analysis was done with MimosaPro (tracks)

    Fake_Ntot = float(NtrkInMimo * hNhitperpixel->GetEntries()); // number of events x number of pixels

    Fake_Nent = hNhitperpixel->GetMean()* hNhitperpixel->GetEntries(); // total number of fake hits

  /*
    for(Int_t fakei=1 ; fakei<= Fake_Nbin_sub1 ; fakei++){
    Fake_Nent += hNhitperpixel->GetBinContent(fakei) ;
    cout<<" Fake_Nent "<< Fake_Nent<<endl;
    }
  */

    Fake_Err = sqrt( ((1.0*Fake_Nent)/(1.*Fake_Ntot))* (1.0 - ( (1.0*Fake_Nent)/(1.*Fake_Ntot) )  ) / Fake_Ntot );

    sprintf(Header1,"M%d ; run %d; Pl %d, sub %d, dist %.0f; Gain %3.2f; eff %3.3f +- %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,ThesubmatrixNumber,TrackToHitDistanceLimit,calibration,MimosaEfficiency,MimosaEfficiency_ERR,CUT_S2N_seed,CUT_S2N_neighbour);

    if(hNhitRateperpixel->GetEntries()>0){
      confidence = float(hsnc->Integral(0,int(confidenceLevel*CUT_S2N_seed))) / float(hsnc->GetEntries());
    }
    sprintf(Header2,"Fake rate: %3.2e; correction: %3.2f (< %.1f x seed cut)", Fake_Rate, confidence, confidenceLevel);

    cout<<"---> WARNING : the fake rate is only valid for a Geomatrix = 5 (tracks not passing trough the detector)"<<endl;
    cout<<"N tracks = "<<NtrkInMimo<<endl;
    cout<<" fake hits = "<< Fake_Nent <<" / "<<Fake_Ntot<<" = "<<((Fake_Ntot>0)?Fake_Nent/Fake_Ntot:0.)<<endl;
    cout<<"  correction: (< "<<confidenceLevel<<" x seed cut) = "<<confidence<<endl;

  } // end if analysis with mimosapro (tracks)

  else { // analysis without tracks
    if( hNhitRateperpixel->GetEntries() != 0 ) Fake_Err = hNhitRateperpixel->GetRMS() / sqrt( hNhitRateperpixel->GetEntries() );

    sprintf(Header1,"M%d ; run %d; Pl %d, sub %d; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,ThesubmatrixNumber,CUT_S2N_seed,CUT_S2N_neighbour);
    sprintf(Header2,"Fake rate: %3.2e +/- %3.2f", Fake_Rate, Fake_Err);

  } // end if analysis without tracks

  cout<<" Fake rate:  "<<Fake_Rate<<" +/- "<<Fake_Err<<endl;
  cout<<"---------------------------------------------------------------------------------------------"<<endl;


  //-------------- Display headers

  mpad0->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.4);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  ttt->DrawText( 0.05,0.6,Header1);
  mpad0->Update();

  mpad00->cd();
  TText *ttt2 = new TText();
  ttt2->SetTextFont(25);
  ttt2->SetTextColor(9);
  ttt2->SetTextSize(.4);
  ttt2->SetTextAlign(12);
  ttt2->SetTextFont(22);
  ttt2->DrawText( 0.05,0.6,Header2);
  mpad00->Update();


  //-------------- plot histograms

  mpad1->cd();
  if(hNhitperpixel->GetEntries()!=0){ mpad1->SetLogy(1);}
  AutoZoom(hNhitperpixel,"Max")->Draw();
  mpad1->Update();
   
  mpad2->cd();
  if(hNhitRateperpixel->GetEntries()!=0){ mpad2->SetLogy(1);}
  AutoZoom(hNhitRateperpixel,"Max")->Draw();
  line->DrawLine(Fake_Rate, hNhitRateperpixel->GetMinimum(), Fake_Rate, hNhitRateperpixel->GetMaximum());
  mpad2->Update(); 
  
  mpad3->cd();
  mpad3->SetLogy(1);
  AutoZoom(hnGOODhit,"Max")->Draw("colz");
  mpad2->Update(); 
  
  mpad4->cd();
  AutoZoom(h2dgoodhits,"Max")->Draw("colz");
  mpad4->Update(); 
  
  mpad5->cd();
  AutoZoom(h2DgoodSeedPixel,"Max")->Draw("colz");
  mpad5->Update(); 
  
  mpad6->cd();
  if(hPixelsPerFakeRate->GetEntries()!=0){ mpad6->SetLogy(1);}
  AutoZoom(hPixelsPerFakeRate,"Max")->Draw();
  mpad6->Update(); 
  
  mpad7->cd();
  if(hnpix->GetEntries()!=0){ mpad7->SetLogy(1);}
  AutoZoom(hnpix,"Max")->Draw();
  mpad7->Update(); 
  
  mpad8->cd();
  if(hnGOODhit->GetEntries()!=0){ mpad8->SetLogy(1);}
  AutoZoom(hnGOODhit,"Max")->Draw();
  mpad8->Update(); 
  
  /*
  mpad9->cd();
  if(hnGOODhit->GetEntries()!=0){ mpad9->SetLogy(1);}
  AutoZoom(hnGOODhit,"Max")->Draw();
  mpad9->Update(); 
  */


  //--------------WRITE IN A ROOT FILE

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;


  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");

  hNhitperpixel->Write();
  hNhitRateperpixel->Write();
  hPixelsPerFakeRate->Write(); // JB 2011/07/01
  
  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;

  MHist::Dir();


}




//____________________________________________________________________________________
//
void MimosaAnalysis::MimosaPixelHomogeneity()
{
if(!CheckIfDone("mimosall")) return;


//cd to result dir
 gSystem->cd(CreateGlobalResultDir());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

UsedMacro[13] = 1;

gStyle->SetTitleOffset(1.);
gStyle->SetOptTitle(1);
gStyle->SetLabelSize(0.06);
gStyle->SetTitleSize(0.06);
gStyle->SetOptStat(1111);
gStyle->SetPadLeftMargin(0.15);  
gStyle->SetTitleH(.065);
gStyle->SetTitleH(.085);
gStyle->SetTitleOffset(1.);
gStyle->SetOptTitle(1);
gStyle->SetOptStat(1111);

//-----------------------------------
// CANVAS 1
//-----------------------------------
PixHom = new TCanvas("PixHom","Pixel Homogeneity",550,10,750,850);
PixHom->Draw();
PixHom->Clear();
PixHom->SetFillColor(0);
PixHom->SetBorderMode(0);

gPad->cd();

TPad* mpad1_1 = new TPad("mpad1_1","",0.01,0.01,0.32,0.30);
mpad1_1->SetGrid(1);
mpad1_1->Draw();

TPad* mpad1_2 = new TPad("mpad1_2","",0.33,0.01,0.66,0.30);
mpad1_2->SetGrid(1);
mpad1_2->Draw();

TPad* mpad1_3 = new TPad("mpad1_3","title 1",0.67,0.01,0.99,0.30);
mpad1_3->SetGrid(1);
mpad1_3->Draw();

TPad* mpad1_4 = new TPad("mpad1_4","",0.01,0.31,0.32,0.60);
mpad1_4->SetGrid(1);
mpad1_4->Draw();

TPad* mpad1_5 = new TPad("mpad1_5","",0.33,0.31,0.66,0.60);
mpad1_5->SetGrid(1);
mpad1_5->Draw();

TPad* mpad1_6 = new TPad("mpad1_6","",0.67,0.31,0.99,0.60);
mpad1_6->SetGrid(1);
mpad1_6->Draw();

TPad* mpad1_7 = new TPad("mpad1_7","",0.01,0.61,0.32,0.9);
mpad1_7->SetGrid(1);
mpad1_7->Draw();

TPad* mpad1_8 = new TPad("mpad1_8","",0.33,0.61,0.66,0.9);
mpad1_8->SetGrid(1);
mpad1_8->Draw();

TPad* mpad1_9 = new TPad("mpad1_9","",0.67,0.61,0.99,0.9);
mpad1_9->SetGrid(1);
mpad1_9->Draw();

TPad* mpad1_title = new TPad("mpad1_title","",0.10,0.91,0.85,0.99);
mpad1_title->SetGrid(1);
mpad1_title->SetFillColor(19);
mpad1_title->Draw();
mpad1_title->cd();

TText *ttt = new TText();
ttt = new TText();
ttt->SetTextFont(25);
ttt->SetTextColor(9);
ttt->SetTextSize(.3);
ttt->SetTextAlign(12);
ttt->SetTextFont(22);

//---ab
Char_t Header[100];
sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
ttt->DrawText( 0.05,0.6,Header);

mpad1_1->SetFillColor(19);
mpad1_2->SetFillColor(19);
mpad1_3->SetFillColor(19);
mpad1_4->SetFillColor(19);
mpad1_5->SetFillColor(19);
mpad1_6->SetFillColor(19);
mpad1_7->SetFillColor(19);
mpad1_8->SetFillColor(19);
mpad1_9->SetFillColor(19);



//-----------------------------------
mpad1_1->cd();
//mpad1_1->SetLogy(1);
hHOM_tu_tv_modulo->Draw("colz");
mpad1_1->Update();

//-----------------------------------
mpad1_2->cd();
//mpad1_1->SetLogy(1);
hHOM_ResU_tu->Draw("colz");
mpad1_2->Update();

//-----------------------------------
mpad1_3->cd();
//mpad1_1->SetLogy(1);
hHOM_ResV_tv->Draw("colz");
mpad1_3->Update();

//-----------------------------------
mpad1_4->cd();
//mpad1_1->SetLogy(1);
ProfHOM_ResU_tu->Draw();
mpad1_4->Update();

//-----------------------------------
mpad1_5->cd();
//mpad1_1->SetLogy(1);
ProfHOM_ResV_tv->Draw();
mpad1_5->Update();

//-----------------------------------
mpad1_6->cd();
//mpad1_1->SetLogy(1);
hHOM_Charge_diodedist->Draw("colz");
mpad1_6->Update();

//-----------------------------------
mpad1_7->cd();
//mpad1_1->SetLogy(1);
ProfHOM_Charge_diodedist->Draw();
mpad1_7->Update();

//-----------------------------------
mpad1_8->cd();
//mpad1_1->SetLogy(1);
hHOM_SNseed_diodedist->Draw("colz");
mpad1_8->Update();

//-----------------------------------
mpad1_9->cd();
//mpad1_1->SetLogy(1);
ProfHOM_SNseed_diodedist->Draw();
mpad1_9->Update();






//-----------------------------------
// CANVAS 2
//-----------------------------------
PixHom2 = new TCanvas("PixHom2","Pixel Homogeneity (2)",550,10,750,850);
PixHom2->Draw();
PixHom2->Clear();
PixHom2->SetFillColor(0);
PixHom2->SetBorderMode(0);

gPad->cd();

TPad* mpad2_1 = new TPad("mpad2_1","",0.01,0.01,0.32,0.30);
mpad2_1->SetGrid(1);
mpad2_1->Draw();

TPad* mpad2_2 = new TPad("mpad2_2","",0.33,0.01,0.66,0.30);
mpad2_2->SetGrid(1);
mpad2_2->Draw();

TPad* mpad2_3 = new TPad("mpad2_3","title 1",0.67,0.01,0.99,0.30);
mpad2_3->SetGrid(1);
mpad2_3->Draw();

TPad* mpad2_4 = new TPad("mpad2_4","",0.01,0.31,0.32,0.60);
mpad2_4->SetGrid(1);
mpad2_4->Draw();

TPad* mpad2_5 = new TPad("mpad2_5","",0.33,0.31,0.66,0.60);
mpad2_5->SetGrid(1);
mpad2_5->Draw();

TPad* mpad2_6 = new TPad("mpad2_6","",0.67,0.31,0.99,0.60);
mpad2_6->SetGrid(1);
mpad2_6->Draw();

TPad* mpad2_7 = new TPad("mpad2_7","",0.01,0.61,0.32,0.9);
mpad2_7->SetGrid(1);
mpad2_7->Draw();

TPad* mpad2_8 = new TPad("mpad2_8","",0.33,0.61,0.66,0.9);
mpad2_8->SetGrid(1);
mpad2_8->Draw();

TPad* mpad2_9 = new TPad("mpad2_9","",0.67,0.61,0.99,0.9);
mpad2_9->SetGrid(1);
mpad2_9->Draw();

TPad* mpad2_title = new TPad("mpad2_title","",0.10,0.91,0.85,0.99);
mpad2_title->SetGrid(1);
mpad2_title->SetFillColor(19);
mpad2_title->Draw();
mpad2_title->cd();

//---ab
sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
ttt->DrawText( 0.05,0.6,Header);

mpad2_1->SetFillColor(19);
mpad2_2->SetFillColor(19);
mpad2_3->SetFillColor(19);
mpad2_4->SetFillColor(19);
mpad2_5->SetFillColor(19);
mpad2_6->SetFillColor(19);
mpad2_7->SetFillColor(19);
mpad2_8->SetFillColor(19);
mpad2_9->SetFillColor(19);


//-----------------------------------
mpad2_1->cd();
//mpad2_1->SetLogy(1);
mpad2_1->Update();
hEta3DigU->Draw();
//-----------------------------------
mpad2_2->cd();
//mpad2_2->SetLogy(1);
hEta3DigV->Draw();
mpad2_2->Update();

//-----------------------------------
mpad2_3->cd();
//mpad2_3->SetLogy(1);
//hEta3UInt->Draw();

  hHOM_DU_Nevent->Draw("colz");
mpad2_3->Update();

//-----------------------------------
mpad2_4->cd();
//mpad2_4->SetLogy(1);
//hEta3VInt->Draw();
 hHOM_DV_Nevent->Draw("colz");
mpad2_4->Update();


//-----------------------------------
mpad2_5->cd();
//mpad2_5->SetLogy(1);
hChargeVsPosition->Draw("colz");
mpad2_5->Update();


//-----------------------------------
mpad2_6->cd();
//mpad2_5->SetLogy(1);
AutoZoom(hHOM_modUeta3_modUCG)->Draw("colz");
			   
//ProfHOM_Charge4_diodedist->Draw();
mpad2_6->Update();

//-----------------------------------
mpad2_7->cd();
//mpad2_5->SetLogy(1);
AutoZoom(hHOM_modVeta3_modVCG)->Draw("colz");
mpad2_7->Update();

//-----------------------------------
mpad2_8->cd();
//mpad2_5->SetLogy(1);
  hChargeVsDistance->Draw();
mpad2_8->Update();

//-----------------------------------
mpad2_9->cd();
//mpad2_5->SetLogy(1);
hHOM_Noise_diodedist->Draw("colz");
mpad2_9->Update();



//-----------------------------------
// CANVAS 3
//-----------------------------------
PixHom3 = new TCanvas("PixHom3","Pixel Homogeneity (3)",550,10,750,850);
PixHom3->Draw();
PixHom3->Clear();
PixHom3->SetFillColor(0);
PixHom3->SetBorderMode(0);

gPad->cd();

TPad* mpad3_1 = new TPad("mpad3_1","",0.01,0.01,0.32,0.30);
mpad3_1->SetGrid(1);
mpad3_1->Draw();

TPad* mpad3_2 = new TPad("mpad3_2","",0.33,0.01,0.66,0.30);
mpad3_2->SetGrid(1);
mpad3_2->Draw();

TPad* mpad3_3 = new TPad("mpad3_3","title 1",0.67,0.01,0.99,0.30);
mpad3_3->SetGrid(1);
mpad3_3->Draw();

TPad* mpad3_4 = new TPad("mpad3_4","",0.01,0.31,0.32,0.60);
mpad3_4->SetGrid(1);
mpad3_4->Draw();

TPad* mpad3_5 = new TPad("mpad3_5","",0.33,0.31,0.66,0.60);
mpad3_5->SetGrid(1);
mpad3_5->Draw();

TPad* mpad3_6 = new TPad("mpad3_6","",0.67,0.31,0.99,0.60);
mpad3_6->SetGrid(1);
mpad3_6->Draw();

TPad* mpad3_7 = new TPad("mpad3_7","",0.01,0.61,0.32,0.9);
mpad3_7->SetGrid(1);
mpad3_7->Draw();

TPad* mpad3_8 = new TPad("mpad3_8","",0.33,0.61,0.66,0.9);
mpad3_8->SetGrid(1);
mpad3_8->Draw();

TPad* mpad3_9 = new TPad("mpad3_9","",0.67,0.61,0.99,0.9);
mpad3_9->SetGrid(1);
mpad3_9->Draw();

TPad* mpad3_title = new TPad("mpad3_title","",0.10,0.91,0.85,0.99);
mpad3_title->SetGrid(1);
mpad3_title->SetFillColor(19);
mpad3_title->Draw();
mpad3_title->cd();

//---ab
sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
ttt->DrawText( 0.05,0.6,Header);

mpad3_1->SetFillColor(19);
mpad3_2->SetFillColor(19);
mpad3_3->SetFillColor(19);
mpad3_4->SetFillColor(19);
mpad3_5->SetFillColor(19);
mpad3_6->SetFillColor(19);
mpad3_7->SetFillColor(19);
mpad3_8->SetFillColor(19);
mpad3_9->SetFillColor(19);


//-----------------------------------
mpad3_1->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_00_10->Draw();
hHOM_Charge_diodedist_00_10->Fit("landau");
TPaveStats *stats1 = (TPaveStats*)hHOM_Charge_diodedist_00_10->GetListOfFunctions()->FindObject("stats");
if(stats1!=0){
stats1->SetX1NDC(0.5);stats1->SetX2NDC(0.95);stats1->SetY1NDC(0.5);stats1->SetY2NDC(0.97);
stats1->SetOptStat(111111);stats1->Draw();
}
 hHOM_modtu_Nevent->Draw("colz");
mpad3_1->Update();

//-----------------------------------
mpad3_2->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_10_20->Draw();
hHOM_Charge_diodedist_10_20->Fit("landau");
TPaveStats *stats2 = (TPaveStats*)hHOM_Charge_diodedist_10_20->GetListOfFunctions()->FindObject("stats");
if(stats2!=0){
stats2->SetX1NDC(0.5);stats2->SetX2NDC(0.95);stats2->SetY1NDC(0.5);stats2->SetY2NDC(0.97);
stats2->SetOptStat(111111);stats2->Draw();
}
 hHOM_modtv_Nevent->Draw("colz");
mpad3_2->Update();

//-----------------------------------
mpad3_3->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_20_30->Draw();
 hHOM_Charge_diodedist_20_30->Fit("landau");
TPaveStats *stats3 = (TPaveStats*)hHOM_Charge_diodedist_20_30->GetListOfFunctions()->FindObject("stats");
if(stats3!=0){
stats3->SetX1NDC(0.5);stats3->SetX2NDC(0.95);stats3->SetY1NDC(0.5);stats3->SetY2NDC(0.97);
stats3->SetOptStat(111111);stats3->Draw();
}
 hHOM_modUCG_Nevent->Draw("colz");
mpad3_3->Update();

//-----------------------------------
mpad3_4->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_30_40->Draw();
 hHOM_Charge_diodedist_30_40->Fit("landau");
TPaveStats *stats4 = (TPaveStats*)hHOM_Charge_diodedist_30_40->GetListOfFunctions()->FindObject("stats");
if(stats4!=0){
stats4->SetX1NDC(0.5);stats4->SetX2NDC(0.95);stats4->SetY1NDC(0.5);stats4->SetY2NDC(0.97);
stats4->SetOptStat(111111);stats4->Draw();
}
 hHOM_modVCG_Nevent->Draw("colz");
mpad3_4->Update();

//-----------------------------------
mpad3_5->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_40_50->Draw();
 hHOM_Charge_diodedist_40_50->Fit("landau");
TPaveStats *stats5 = (TPaveStats*)hHOM_Charge_diodedist_40_50->GetListOfFunctions()->FindObject("stats");
if(stats5!=0){
stats5->SetX1NDC(0.5);stats5->SetX2NDC(0.95);stats5->SetY1NDC(0.5);stats5->SetY2NDC(0.97);
stats5->SetOptStat(111111);stats5->Draw();
}
hEtaU_2x2Res->Draw();
hEtaU_2x2Res->Fit("gaus");
mpad3_5->Update();

//-----------------------------------
mpad3_6->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_50_60->Draw();
 hHOM_Charge_diodedist_50_60->Fit("landau");
TPaveStats *stats6 = (TPaveStats*)hHOM_Charge_diodedist_50_60->GetListOfFunctions()->FindObject("stats");
if(stats6!=0){
stats6->SetX1NDC(0.5);stats6->SetX2NDC(0.95);stats6->SetY1NDC(0.5);stats6->SetY2NDC(0.97);
stats6->SetOptStat(111111);stats6->Draw();
}
hEtaV_2x2Res->Draw();
hEtaV_2x2Res->Fit("gaus");
mpad3_6->Update();

//-----------------------------------
mpad3_7->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_60_70->Draw();
 hHOM_Charge_diodedist_60_70->Fit("landau");
TPaveStats *stats7 = (TPaveStats*)hHOM_Charge_diodedist_60_70->GetListOfFunctions()->FindObject("stats");
if(stats7!=0){
stats7->SetX1NDC(0.5);stats7->SetX2NDC(0.95);stats7->SetY1NDC(0.5);stats7->SetY2NDC(0.97);
stats7->SetOptStat(111111);stats7->Draw();
}

hEtaU_5x5Res->Draw();
hEtaU_5x5Res->Fit("gaus");
mpad3_7->Update();

//-----------------------------------
mpad3_8->cd();
//mpad2_1->SetLogy(1);
hHOM_Charge_diodedist_90_inf->Draw();
hHOM_Charge_diodedist_90_inf->Fit("landau");
TPaveStats *stats10 = (TPaveStats*)hHOM_Charge_diodedist_80_90->GetListOfFunctions()->FindObject("stats");
if(stats10!=0){
stats10->SetX1NDC(0.5);stats10->SetX2NDC(0.95);stats10->SetY1NDC(0.5);stats10->SetY2NDC(0.97);
stats10->SetOptStat(111111);stats10->Draw();
}

hHOM_Charge_diodedist_80_90->Draw();
hHOM_Charge_diodedist_80_90->Fit("landau");
TPaveStats *stats9 = (TPaveStats*)hHOM_Charge_diodedist_90_inf->GetListOfFunctions()->FindObject("stats");
if(stats9!=0){
stats9->SetX1NDC(0.5);stats9->SetX2NDC(0.95);stats9->SetY1NDC(0.5);stats9->SetY2NDC(0.97);
stats9->SetOptStat(111111);stats9->Draw();
}

hHOM_Charge_diodedist_70_80->Draw();
hHOM_Charge_diodedist_70_80->Fit("landau");
TPaveStats *stats8 = (TPaveStats*)hHOM_Charge_diodedist_70_80->GetListOfFunctions()->FindObject("stats");
if(stats8!=0){
stats8->SetX1NDC(0.5);stats8->SetX2NDC(0.95);stats8->SetY1NDC(0.5);stats8->SetY2NDC(0.97);
stats8->SetOptStat(111111);stats8->Draw();
}

hEtaV_5x5Res->Draw();
hEtaV_5x5Res->Fit("gaus");
mpad3_8->Update();


//-----------------------------------
mpad3_9->cd();
//mpad2_1->SetLogy(1);

 
const Int_t nn(10);
Double_t x[nn] = {5,15,25,35,45,55,65,75,85,95};
Double_t ex[nn] = {5,5,5,5,5,5,5,5,5,5};
Double_t y[nn] = {
  float(hHOM_Charge_diodedist_00_10->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_10_20->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_20_30->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_30_40->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_40_50->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_50_60->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_60_70->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_70_80->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_80_90->GetFunction("landau")->GetParameter(1)),
  float(hHOM_Charge_diodedist_90_inf->GetFunction("landau")->GetParameter(1))
};

Double_t ey[nn] = {
  float(hHOM_Charge_diodedist_00_10->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_10_20->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_20_30->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_30_40->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_40_50->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_50_60->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_60_70->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_70_80->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_80_90->GetFunction("landau")->GetParError(1)),
  float(hHOM_Charge_diodedist_90_inf->GetFunction("landau")->GetParError(1))
};

TGraphErrors* grCharge_diodedist = new TGraphErrors(nn,x,y,ex,ey);
grCharge_diodedist->Draw("ALP");
TF1* polCh_diodedist = new TF1("polCh_diodedist","pol3");
grCharge_diodedist->Fit("polCh_diodedist");

mpad3_9->Update();

//-----------------------------------
// CANVAS 4
//-----------------------------------
PixHom4 = new TCanvas("PixHom4","Pixel Homogeneity (4)",550,10,750,850);
PixHom4->Draw();
PixHom4->Clear();
PixHom4->SetFillColor(0);
PixHom4->SetBorderMode(0);

gPad->cd();

TPad* mpad4_1 = new TPad("mpad4_1","",0.01,0.01,0.32,0.30);
mpad4_1->SetGrid(1);
mpad4_1->Draw();

TPad* mpad4_2 = new TPad("mpad4_2","",0.33,0.01,0.66,0.30);
mpad4_2->SetGrid(1);
mpad4_2->Draw();

TPad* mpad4_3 = new TPad("mpad4_3","title 1",0.67,0.01,0.99,0.30);
mpad4_3->SetGrid(1);
mpad4_3->Draw();

TPad* mpad4_4 = new TPad("mpad4_4","",0.01,0.31,0.32,0.60);
mpad4_4->SetGrid(1);
mpad4_4->Draw();

TPad* mpad4_5 = new TPad("mpad4_5","",0.33,0.31,0.66,0.60);
mpad4_5->SetGrid(1);
mpad4_5->Draw();

TPad* mpad4_6 = new TPad("mpad4_6","",0.67,0.31,0.99,0.60);
mpad4_6->SetGrid(1);
mpad4_6->Draw();

TPad* mpad4_7 = new TPad("mpad4_7","",0.01,0.61,0.32,0.9);
mpad4_7->SetGrid(1);
mpad4_7->Draw();

TPad* mpad4_8 = new TPad("mpad4_8","",0.33,0.61,0.66,0.9);
mpad4_8->SetGrid(1);
mpad4_8->Draw();

TPad* mpad4_9 = new TPad("mpad4_9","",0.67,0.61,0.99,0.9);
mpad4_9->SetGrid(1);
mpad4_9->Draw();

TPad* mpad4_title = new TPad("mpad4_title","",0.10,0.91,0.85,0.99);
mpad4_title->SetGrid(1);
mpad4_title->SetFillColor(19);
mpad4_title->Draw();
mpad4_title->cd();

//---ab
sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
ttt->DrawText( 0.05,0.6,Header);

mpad4_1->SetFillColor(19);
mpad4_2->SetFillColor(19);
mpad4_3->SetFillColor(19);
mpad4_4->SetFillColor(19);
mpad4_5->SetFillColor(19);
mpad4_6->SetFillColor(19);
mpad4_7->SetFillColor(19);
mpad4_8->SetFillColor(19);
mpad4_9->SetFillColor(19);


//-----------------------------------
mpad4_1->cd();
AutoZoom(hHOM_modUeta3_modtu)->Draw("colz");
mpad4_1->Update();

//-----------------------------------
mpad4_2->cd();
AutoZoom(hHOM_modVeta3_modtv)->Draw("colz");
mpad4_2->Update();

//-----------------------------------
mpad4_3->cd();
AutoZoom(hHOM_modUeta3_realtu)->Draw("colz");
mpad4_3->Update();

//-----------------------------------
mpad4_4->cd();
AutoZoom(hHOM_modVeta3_realtv)->Draw("colz");
mpad4_4->Update();

//-----------------------------------
mpad4_5->cd();
AutoZoom(hHOM_modUCG_realtu)->Draw("colz");
mpad4_5->Update();

//-----------------------------------
mpad4_6->cd();
AutoZoom(hHOM_modUeta3_Eta3U)->Draw("colz");
mpad4_6->Update();

//-----------------------------------
mpad4_7->cd();
AutoZoom(hHOM_modVeta3_Eta3V)->Draw("colz");
mpad4_7->Update();

//-----------------------------------
mpad4_8->cd();
hHOM_modUeta3_modVeta3->Draw("colz");
mpad4_8->Update();

//-----------------------------------
mpad4_9->cd();
hHOM_modUCG_modVCG->Draw("colz");
mpad4_9->Update();





  //-----------------------------------
  // CANVAS 5
  //-----------------------------------
  PixHom5 = new TCanvas("PixHom5","Pixel Homogeneity (5)",550,10,750,850);
  PixHom5->Draw();
  PixHom5->Clear();
  PixHom5->SetFillColor(0);
  PixHom5->SetBorderMode(0);
  
  PixHom5->cd();
  
  TPad* mpad5_title = new TPad("mpad5_title","",0.10,0.91,0.85,0.99);
  mpad5_title->SetGrid(1);
  mpad5_title->SetFillColor(19);
  mpad5_title->Draw();
  mpad5_title->cd();
  
  sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
  ttt->DrawText( 0.05,0.6,Header);

  PixHom5->cd();
  
  TPad* mpad5_1 = new TPad("mpad5_1","",0.01,0.01,0.99,0.90);
  mpad5_1->Draw();
  mpad5_1->Divide(3,3);
  
  //-----------------------------------
  mpad5_1->cd(1);
  ProfHOM_Charge_diodedist_alg->Draw();
  
  //-----------------------------------
  mpad5_1->cd(2);
  ProfHOM_Charge_diodedist_alg_u->Draw();
  
  //-----------------------------------
  mpad5_1->cd(3);
  ProfHOM_Charge_diodedist_alg_v->Draw();
  
  //-----------------------------------
  mpad5_1->cd(4);
  hHOM_Charge_diodedist_alg->Draw("colz");
  
  //-----------------------------------
  mpad5_1->cd(5);
  hHOM_Charge_diodedist_alg_u->Draw("colz");
  
  //-----------------------------------
  mpad5_1->cd(6);
  hHOM_Charge_diodedist_alg_v->Draw("colz");
  
  
  mpad5_1->Update();
  
  
    
  
  //-----------------------------------
  // CANVAS 6
  //-----------------------------------
  PixHom6 = new TCanvas("PixHom6","Pixel Homogeneity (6)",550,10,750,850);
  PixHom6->Draw();
  PixHom6->Clear();
  PixHom6->SetFillColor(0);
  PixHom6->SetBorderMode(0);
  
  PixHom6->cd();
  
  TPad* mpad6_title = new TPad("mpad6_title","",0.10,0.91,0.85,0.99);
  mpad6_title->SetGrid(1);
  mpad6_title->SetFillColor(19);
  mpad6_title->Draw();
  mpad6_title->cd();
  
  sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f; eff %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit,MimosaEfficiency,CUT_S2N_seed,CUT_S2N_neighbour);
  ttt->DrawText( 0.05,0.6,Header);
  
  PixHom6->cd();
  
  TPad* mpad6_1 = new TPad("mpad6_1","",0.01,0.01,0.99,0.90);
  mpad6_1->Draw();
  mpad6_1->Divide(3,4);
  
  //-----------------------------------
  mpad6_1->cd(1);
  hHOM_Charge2_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(2);
  ProfHOM_Charge2_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(3);
  hHOM_Charge2_diodedist3D->Draw();
  
  //-----------------------------------
  mpad6_1->cd(4);
  hHOM_Charge4_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(5);
  ProfHOM_Charge4_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(6);
  hHOM_Charge4_diodedist3D->Draw();
  
  //-----------------------------------
  mpad6_1->cd(7);
  hHOM_Charge9_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(8);
  ProfHOM_Charge9_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(9);
  hHOM_Charge9_diodedist3D->Draw();
  
  //-----------------------------------
  mpad6_1->cd(10);
  hHOM_Charge25_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(11);
  ProfHOM_Charge25_diodedist->Draw();
  
  //-----------------------------------
  mpad6_1->cd(12);
  hHOM_Charge25_diodedist3D->Draw();
  
  mpad6_1->Update();

  
  
  
  
//------------------------------------------------------------------------------
// Print some results
//------------------------------------------------------------------------------

cout<<"PAR 00 10 ="<< hHOM_Charge_diodedist_00_10->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 10 20 ="<< hHOM_Charge_diodedist_10_20->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 20 30 ="<< hHOM_Charge_diodedist_20_30->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 30 40 ="<< hHOM_Charge_diodedist_30_40->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 40 50 ="<< hHOM_Charge_diodedist_40_50->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 50 60 ="<< hHOM_Charge_diodedist_50_60->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 60 70 ="<< hHOM_Charge_diodedist_60_70->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 70 80 ="<< hHOM_Charge_diodedist_70_80->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 80 90 ="<< hHOM_Charge_diodedist_80_90->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"PAR 90 inf ="<< hHOM_Charge_diodedist_90_inf->GetFunction("landau")->GetParameter(1)<<endl;
cout<<"RATIO MAX / MIN (MPV)"<<endl;
if( hHOM_Charge_diodedist_90_inf->GetFunction("landau")->GetParameter(1) != 0){
  cout<< (hHOM_Charge_diodedist_00_10->GetFunction("landau")->GetParameter(1))
    /( hHOM_Charge_diodedist_90_inf->GetFunction("landau")->GetParameter(1) )<<endl;
}
cout<<"RATIO MAX / MIN (FIT)"<<endl;
if( polCh_diodedist->Eval(95) != 0.0){
  cout<<polCh_diodedist->Eval(15) / polCh_diodedist->Eval(95)<<endl;
}


  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");

  hChargeVsPosition->Write();
  hChargeVsDistance->Write();

  hHOM_Charge_diodedist_alg->Write(); // JB 2010/03/19
  hHOM_Charge_diodedist_alg_u->Write(); //clm
  hHOM_Charge_diodedist_alg_v->Write(); //clm
  hHOM_Charge_diodedist->Write();
  ProfHOM_Charge_diodedist_alg->Write(); // JB 2010/03/11
  ProfHOM_Charge_diodedist_alg_u->Write(); //clm
  ProfHOM_Charge_diodedist_alg_v->Write(); //clm
  ProfHOM_Charge_diodedist->Write(); // JB 2010/03/11
  hHOM_tu_tv_modulo->Write(); // JB 2010/03/11
  polCh_diodedist->Write();

  hHOM_Charge_diodedist3D  ->Write(); //clm 2013/01/23  
  hHOM_Charge2_diodedist3D ->Write(); //clm 2013/01/23  
  hHOM_Charge4_diodedist3D ->Write(); //clm 2013/01/23  
  hHOM_Charge9_diodedist3D ->Write(); //clm 2013/01/23  
  hHOM_Charge25_diodedist3D->Write(); //clm 2013/01/23  
  ProfhGOODCharge_Charge_DiodePosition->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePositionSeedQLT300->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePositionSeedQGT2000->Write();//clm 2013/01/23

  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow->Write();//clm 2013/01/23
  
  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_seed->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_seed->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_seed->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_seed->Write();//clm 2013/01/23
  
  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_1stcrown->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_1stcrown->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_1stcrown->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_1stcrown->Write();//clm 2013/01/23
  
  ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_2ndcrown->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_2ndcrown->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_2ndcrown->Write();//clm 2013/01/23
  ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_2ndcrown->Write();//clm 2013/01/23
  
  hDistVSeedOtherOldCalc->Write();
  hDistVSeedOtherNewCalc->Write();
  
  h2dCharge_Charge_DiodePosition_Track->Write();//clm 2013/01/23
  h2dCharge_Charge_DiodePosition_CluSize->Write();//clm 2013/01/23
  hNpixInClu->Write();//clm 2013/01/23
  hQpixInClu->Write();//clm 2013/01/23
  hHOM_Noise_diodedist->Write();//clm 2013/01/23
    
  ProfhGOODCharge_Charge_DiodePositionSimpDist->Write();//clm 2013/01/23   

  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();


   /*
   Float_t x[n]  = {-0.22, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};
   Float_t y[n]  = {1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};
   Float_t ex[n] = {.05,.1,.07,.07,.04,.05,.06,.07,.08,.05};
   Float_t ey[n] = {.8,.7,.6,.5,.4,.4,.5,.6,.7,.8};
   gr = new TGraphErrors(n,x,y,ex,ey);
   gr->SetTitle("TGraphErrors Example");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");

       TF1 *f1 = new TF1("f1","gaus",1,3);
        graph->Fit("f1","R");



*/

}

//________________________________________________________________________________________
//
void MimosaAnalysis::UserAnalysis()
{
  // Plot histograms from user analysis
  // JB 2009/09/07

  if(!CheckIfDone("mimosall")) return;

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

  UsedMacro[49] = 1;

  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetLabelSize(0.06);
  gStyle->SetTitleSize(0.06);
  gStyle->SetOptStat(1111);
  gStyle->SetPadLeftMargin(0.15);  
  gStyle->SetTitleH(.065);
  gStyle->SetTitleH(.085);
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);

  // ---> 1st canvas.
  // --- Make CANVAS

  cUser = new TCanvas("cUser","user analysis",550,10,750,850);
  cUser->Draw();
  cUser->Clear();
  cUser->SetFillColor(0);
  cUser->SetBorderMode(0);

  gPad->cd();

  TPad* mpad0 = new TPad("mpad0","title 1",0.01,0.95,0.99,0.99);
  mpad0->SetGrid(1);
  mpad0->Draw();

  TPad* mpad1 = new TPad("mpad1","",0.01,0.01,0.99,0.94);
  mpad1->SetGrid(1);
  mpad1->Divide(2,3);
  mpad1->Draw();



  mpad0->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.4);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  Char_t Header[100];
  //---ab
  sprintf(Header,"M%d ; run %d; Pl %d, sub %d, dist %.0f; Gain %3.2f; eff %3.3f +- %3.3f; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,ThesubmatrixNumber,TrackToHitDistanceLimit,calibration,MimosaEfficiency,MimosaEfficiency_ERR,CUT_S2N_seed,CUT_S2N_neighbour);
//sprintf(Header,"M%d ; run %d; Pl %d; dist %.0f",MimosaType,RunNumber,ThePlaneNumber,TrackToHitDistanceLimit);
  ttt->DrawText( 0.05,0.6,Header);
  mpad0->Update();


  //mpad1->SetFillColor(19);

  gStyle->SetOptStat(111111);  

  mpad1->cd(1);  
  AutoZoom(hUserHitCorrelationLine)->Draw();

  mpad1->cd(2);  
  AutoZoom(hUserHitCorrelationCol)->Draw();

  //--------------WRITE IN A ROOT FILE

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  

  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");
  
  hUserHitCorrelationCol->Write();
  hUserHitCorrelationLine->Write();
  
  
  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  
  MHist::Dir();

}

//________________________________________________________________________________________
void MimosaAnalysis::ClusterShape()
{

  // Study the clusters shape
  // usefull for angular study but not only
  //
  // JB 2010/04/13 based on cdritsa february 2008
  // Modified CLM 2013/08/25 new plots hMultVsFormFactor1D
  // Modified JB 2013/09/07 save in same files _ClCharge as all histos

  if(!CheckIfDone("mimosall")) return;
  
  UsedMacro[14] = 1;
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);
  gStyle->SetPalette(1,0);
  gStyle->SetPaintTextFormat(".1f");
  
  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
  
  Char_t ResultFileNameAng[50];
  sprintf(ResultFileNameAng,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileNameAng,"%s", fTool.LocalizeDirName( ResultFileNameAng)); // JB 2011/07/07
  TFile *ResultRootFileAng=new TFile(ResultFileNameAng,"UPDATE");
  
  /** ? **/
  ang1 = new TCanvas("ang1","Angular Study Plots (1)",50,10,1000,500);
  ang1->Draw();
  ang1->Clear();
  ang1->SetBorderMode(0);
  ang1->cd();
  ang1->Divide((Int_t)ceil(nThresholds/2.),2);
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    ang1->cd(ithres+1);
    hMultVsFormFactor[ithres]->Draw("colz");
  }
  
  /** 2D map of percentage of pix over SNR threshold **/
  ang2 = new TCanvas("ang2","Angular Study Plots (2)",100,10,1000,500);
  ang2->Draw();
  ang2->Clear();
  ang2->SetBorderMode(0);
  ang2->cd();
  ang2->Divide((Int_t)ceil(nThresholds/2.),2);
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    ang2->cd(ithres+1);
    hPixelsOverSNR[ithres]->Draw("text,colz");
  }
  
  /** 2D map of percentage of pix over Q threshold **/
  ang3 = new TCanvas("ang3","Angular Study Plots (3)",100,10,1000,500);
  ang3->Draw();
  ang3->Clear();
  ang3->SetBorderMode(0);
  ang3->cd();
  ang3->Divide((Int_t)ceil(nThresholds/2.),2);
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    ang3->cd(ithres+1);
    hPixelsOverCharge[ithres]->Draw("text,colz");
  }
  
  /** Pixel multiplicity over threshold **/
  ang4 = new TCanvas("ang4","Angular Study Plots (4)",100,10,1000,500);
  ang4->Draw();
  ang4->Clear();
  ang4->SetBorderMode(0);
  ang4->cd();
  ang4->Divide((Int_t)ceil(nThresholds/2.),2);
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    ang4->cd(ithres+1);
    hCountPixels[ithres]->Draw();
  }
  
  
  /** ? **/
  ang5 = new TCanvas("ang5","Angular Study Plots (5)",100,10,1000,500);
  ang5->Draw();
  ang5->Clear();
  ang5->SetBorderMode(0);
  ang5->cd();
  ang5->Divide(3,4);
  
  ang5->cd(1);
  hChargeCoG_Correl->Draw();
  ang5->cd(2);
  hChargeCoG_Correl2->Draw();
//  h_SNRratioL->Draw();
  ang5->cd(3);
  hClusterTypes->Draw();
  ang5->cd(4);
  hClusterTest1->Draw();
  ang5->cd(5);
  hClusterTest2->Draw();
  ang5->cd(7);
  hprojLForm->Draw();
  ang5->cd(8);
  hprojCForm->Draw();
  ang5->cd(9);
  hClusterTypesBeyond4->Draw();
  ang5->cd(10);
  hClusterSizeInLines->Draw();
  ang5->cd(11);
  hClusterSizeInColumns->Draw();

  ang5v2 = new TCanvas("ang5v2","Angular Study Plots (5) v2",100,10,1000,500);
  ang5v2->Draw();
  ang5v2->Clear();
  ang5v2->SetFillColor(10);
  ang5v2->SetFrameFillColor(10);
  ang5v2->SetTickx(1);
  ang5v2->SetTicky(1);
  hClusterSize2DInLineAndColumns->Draw("colz");

  //clm added back 1D clu mult. 2013.08.25
  ang6 = new TCanvas("ang6","Angular Study Plots (6)",50,10,1000,500);
  ang6->Draw();
  ang6->Clear();
  ang6->SetBorderMode(0);
  ang6->cd();
  ang6->Divide((Int_t)ceil(nThresholds/2.),2);
  
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    ang6->cd(ithres+1);
    hMultVsFormFactor1D[ithres]->Draw("be");
  }
  
  /** The average cluster shape **/
  shape = new TCanvas("shape","Average Cluster Shape",100,10,700,700);
  shape->cd();
  hClusterMeanForm->Draw("text,colz");
  
  
  /** A few individual clusters **/
  solo = new TCanvas("clust","Individual clusters",100,10,1000,500);
  solo->Divide(2,3);
  
  for(Int_t i=0; i<6; i++) {
    solo->cd(i+1);
    Cluster[i]->Draw("text,colz");
  }
  
  /** Projection of a line **/
  projL = new TCanvas("projCanvas","Percentage of pixels above (charge) threshold for the central row",100,10,1000,500);
  projL->Divide((Int_t)ceil(nThresholds/2.),2);
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    projL->cd(ithres+1)->SetGrid(1,1);;
    hprojL[ithres]->Draw();
  }
  
  /** Projection of a column **/
  projC = new TCanvas("projCCanvas","Percentage of pixels above (charge) threshold for the central column",100,10,1000,500);
  projC->Divide((Int_t)ceil(nThresholds/2.),2);
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    projC->cd(ithres+1)->SetGrid(1,1);;
    hprojC[ithres]->Draw();
  }
  
  // Saving histos & canvas to file
  
  ang1->Write();
  ang2->Write();
  ang3->Write();
  ang4->Write();
  ang5->Write();
  shape->Write();
  solo->Write();
  projL->Write();
  projC->Write();
  
  for( Int_t ithres=0; ithres<nThresholds; ithres++) {
    hMultVsFormFactor[ithres]->Write();
    hMultVsFormFactor1D[ithres]->Write(); //clm added back 1D clu mult. 2013.08.25
    hPixelsOverSNR[ithres]->Write();
    hPixelsOverCharge[ithres]->Write();
    
    hCountPixels[ithres]->Write();
    
    hprojL[ithres]->Write();
    hprojC[ithres]->Write();
  }
  
  hChargeCoG_Correl->Write();
  hChargeCoG_Correl2->Write();
  h_SNRratioL->Write();
  
  hClusterTest1->Write();
  hClusterTest2->Write();
  
  hClusterMeanForm->Write();
  hprojLForm->Write();
  hprojCForm->Write();
  
  hClusterSizeInLines->Write();
  hClusterSizeInColumns->Write();
  hClusterSize2DInLineAndColumns->Write();
  hClusterTypes->Write();
  hClusterTypesBeyond4->Write();
  
  hChargeDistrIn3rdLeftNeigh->Write();
  hChargeDistrIn2ndLeftNeigh->Write();
  hChargeDistrIn1stLeftNeigh->Write();
  hChargeDistrInSeed->Write();
  hChargeDistrIn1stRightNeigh->Write();
  hChargeDistrIn2ndRightNeigh->Write();
  hChargeDistrIn3rdRightNeigh->Write();
  
  for(Int_t i=0; i<6; i++) {
    Cluster[i]->Write();
  }
  
  ResultRootFileAng->Write();
  ResultRootFileAng->Close();
  
  printf("\n All plots saved in %s\n", ResultRootFileAng->GetName());
  delete ResultRootFileAng;
  
  MHist::Dir();

}

//________________________________________________________________________________________
void MimosaAnalysis::HitMap()
{

  // Plots a map of the reconstructed hits
  //  and comparison between different position calculations
  //
  // JB 2010/10/06
  // Modified: JB 2013/05/01 hit map in tracker frame added
  
  if(!CheckIfDone("mimosa")) return;
  
  UsedMacro[17] = 1;
  
  char fOutName[200];
  sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
  //TString command;
  //char tmpFile[200];
  //sprintf(tmpFile,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
  //sprintf(tmpFile,"%s",fTool.LocalizeDirName(tmpFile));
  //TString EPSName = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_tmp.pdf");
  //TString EPSNameO = EPSName + TString("[");
  //TString EPSNameC = EPSName + TString("]");
  //TString EPSName_test  = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString("_v2.pdf");
  //TString EPSName_final = TString(CreateGlobalResultDir()) + TString(tmpFile) + TString(".pdf");

  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(11111);
  gStyle->SetPalette(1,0);

  //------------
  //--- Hit map
  cHitMap = new TCanvas("hitmap","Hit map",550,10,750,850);
  cHitMap->Draw();
  cHitMap->Clear();
  cHitMap->SetFillColor(0);
  cHitMap->SetBorderMode(0);

  TPad* mpad0 = new TPad("mpad0","title",0.01,0.95,0.99,0.99);
  mpad0->SetGrid(1);
  mpad0->Draw();

  TPad* mpad1 = new TPad("mpad1","plots",0.01,0.01,0.99,0.94);
  mpad1->SetGrid(1);
  mpad1->Draw();

  mpad0->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.4);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  Char_t Header[100];
  sprintf(Header,"M%d ; run %d; Pl %d, sub %d; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,ThesubmatrixNumber,CUT_S2N_seed,CUT_S2N_neighbour);
  ttt->DrawText( 0.05,0.6,Header);
  mpad0->Update();

  mpad1->cd();
  gStyle->SetOptStat(111111);  
  mpad1->Divide(3,2);
  
  mpad1->cd(1);
  AutoZoom(huv)->Draw("colz");
  mpad1->cd(2);
  AutoZoom(hhu)->Draw();
  mpad1->cd(3);
  AutoZoom(hhv)->Draw();
  mpad1->cd(4);
  AutoZoom(hxy)->Draw("colz"); // JB 2013/05/01
  mpad1->cd(5);
  AutoZoom(hhx)->Draw();
  mpad1->cd(6);
  AutoZoom(hhy)->Draw();
  
  mpad1->Update();
  //------------

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cHitMap->Print(EPSNameO.Data());
    cHitMap->Print(EPSName.Data());
    cHitMap->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }
  

  //------------
  //--- Hit map 2
  cHitMap2 = new TCanvas("hitmap2","Hit map 2",550,10,750,850);
  cHitMap2->Draw();
  cHitMap2->Clear();
  cHitMap2->SetFillColor(10);
  cHitMap2->SetFrameFillColor(10);
  cHitMap2->SetTickx(1);
  cHitMap2->SetTicky(1);
  cHitMap2->SetLeftMargin(0.15);
  cHitMap2->SetBottomMargin(0.15);
  cHitMap2->SetRightMargin(0.15);
  huv_rate->Draw("colz");
  cHitMap2->Update();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cHitMap2->Print(EPSNameO.Data());
    cHitMap2->Print(EPSName.Data());
    cHitMap2->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  //------------
  //--- Hit map 2
  cPixEvent = new TCanvas("cPixEvent","Fired pixels per event",550,10,750,850);
  cPixEvent->Draw();
  cPixEvent->Clear();
  cPixEvent->SetFillColor(10);
  cPixEvent->SetFrameFillColor(10);
  cPixEvent->SetTickx(1);
  cPixEvent->SetTicky(1);
  cPixEvent->SetLeftMargin(0.15);
  cPixEvent->SetBottomMargin(0.15);
  cPixEvent->SetRightMargin(0.15);
  h_pixels_event->Draw();
  cPixEvent->Update();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cPixEvent->Print(EPSNameO.Data());
    cPixEvent->Print(EPSName.Data());
    cPixEvent->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  //------------
  //---details pads:

  sprintf( nom, "positionStudy1_P%dS%d", ThePlaneNumber, ThesubmatrixNumber);
  sprintf( titre, "Study 1 of position calculation Pl %d Sub %d", ThePlaneNumber, ThesubmatrixNumber);
  cPosStudy1 = new TCanvas( nom, titre, 900, 900);
  cPosStudy1->SetBorderMode(0);
  cPosStudy1->Divide(3,3);

  cPosStudy1->cd(1);
  AutoZoom(h2dmatchedhits)->Draw("colz");
  cPosStudy1->cd(2);
  AutoZoom(hhuS)->Draw();
  cPosStudy1->cd(3);
  AutoZoom(hhvS)->Draw();
  cPosStudy1->cd(4);
  AutoZoom(h2dgoodhits)->Draw("colz");
  cPosStudy1->cd(5);
//  AutoZoom(h2DgoodSeedPixel)->Draw("colz");
  h2DgoodSeedPixel->Draw("colz");
  cPosStudy1->cd(6);
  AutoZoom(hgoodSeedPixel)->Draw();
  cPosStudy1->cd(7);
  h2DpictureMatched->Draw("colz");
  cPosStudy1->cd(8);
  //AutoZoom()->Draw();
  cPosStudy1->cd(9);
  //AutoZoom()->Draw();

  cPosStudy1->Update();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cPosStudy1->Print(EPSNameO.Data());
    cPosStudy1->Print(EPSName.Data());
    cPosStudy1->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  sprintf( nom, "positionStudy2_P%dS%d", ThePlaneNumber, ThesubmatrixNumber);
  sprintf( titre, "Study 2 of position calculation Pl %d Sub %d", ThePlaneNumber, ThesubmatrixNumber);
  cPosStudy2 = new TCanvas( nom, titre, 900, 900);
  cPosStudy2->SetBorderMode(0);
  cPosStudy2->Divide(3,4);

  cPosStudy2->cd(1);
  AutoZoom(hdCGDigU)->Draw();
  cPosStudy2->cd(2);
  AutoZoom(hdCGDigV)->Draw();
  cPosStudy2->cd(3);
  AutoZoom(hEta3DigU)->Draw();
  cPosStudy2->cd(4);
  AutoZoom(hEta3DigV)->Draw();
  cPosStudy2->cd(5);
  AutoZoom(hdCGEtaU)->Draw();
  cPosStudy2->cd(6);
  AutoZoom(hdCGEtaV)->Draw();

  cPosStudy2->Update();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cPosStudy2->Print(EPSNameO.Data());
    cPosStudy2->Print(EPSName.Data());
    cPosStudy2->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  int NPadsX = 3;
  int NPadsY = 3;
  TLine* lh_TrackhitPos_vs_Mult = new TLine(huvCGtuv_vs_Mult[0]->GetXaxis()->GetXmax()*0.5,huvCGtuv_vs_Mult[0]->GetYaxis()->GetXmin(),
					    huvCGtuv_vs_Mult[0]->GetXaxis()->GetXmax()*0.5,huvCGtuv_vs_Mult[0]->GetYaxis()->GetXmax());
  lh_TrackhitPos_vs_Mult->SetLineColor(2);
  lh_TrackhitPos_vs_Mult->SetLineWidth(3);
  lh_TrackhitPos_vs_Mult->SetLineStyle(2);
  double rell_u = (huvCGtuv_vs_Mult[0]->GetXaxis()->GetXmax() - huvCGtuv_vs_Mult[0]->GetXaxis()->GetXmin())*0.03;
  double rell_v = (huvCGtuv_vs_Mult[0]->GetYaxis()->GetXmax() - huvCGtuv_vs_Mult[0]->GetYaxis()->GetXmin())*0.03;
  Diodes = new TEllipse*[4];
  int col,lin;
  float u,v;
  col = 0; lin = 0;
  ComputePixelPosition(col,lin,u,v);
  u = (u + 0.5*NofPixelInRaw    * PixelSizeU)/(2.0*PixelSizeU);
  u = (u - int(u))*2.0*PixelSizeU;
  v = (v + 0.5*NofPixelInColumn * PixelSizeV)/(2.0*PixelSizeV);
  v = (v - int(v))*2.0*PixelSizeV;
  Diodes[0] = new TEllipse(u,v,rell_u,rell_v);
  Diodes[0]->SetLineColor(2);
  Diodes[0]->SetLineWidth(2);
  Diodes[0]->SetFillStyle(3000);

  col = 0; lin = 1;
  ComputePixelPosition(col,lin,u,v);
  u = (u + 0.5*NofPixelInRaw    * PixelSizeU)/(2.0*PixelSizeU);
  u = (u - int(u))*2.0*PixelSizeU;
  v = (v + 0.5*NofPixelInColumn * PixelSizeV)/(2.0*PixelSizeV);
  v = (v - int(v))*2.0*PixelSizeV;
  Diodes[1] = new TEllipse(u,v,rell_u,rell_v);
  Diodes[1]->SetLineColor(2);
  Diodes[1]->SetLineWidth(2);
  Diodes[1]->SetFillStyle(3000);

  col = 1; lin = 0;
  ComputePixelPosition(col,lin,u,v);
  u = (u + 0.5*NofPixelInRaw    * PixelSizeU)/(2.0*PixelSizeU);
  u = (u - int(u))*2.0*PixelSizeU;
  v = (v + 0.5*NofPixelInColumn * PixelSizeV)/(2.0*PixelSizeV);
  v = (v - int(v))*2.0*PixelSizeV;
  Diodes[2] = new TEllipse(u,v,rell_u,rell_v);
  Diodes[2]->SetLineColor(2);
  Diodes[2]->SetLineWidth(2);
  Diodes[2]->SetFillStyle(3000);

  col = 1; lin = 1;
  ComputePixelPosition(col,lin,u,v);
  u = (u + 0.5*NofPixelInRaw    * PixelSizeU)/(2.0*PixelSizeU);
  u = (u - int(u))*2.0*PixelSizeU;
  v = (v + 0.5*NofPixelInColumn * PixelSizeV)/(2.0*PixelSizeV);
  v = (v - int(v))*2.0*PixelSizeV;
  Diodes[3] = new TEllipse(u,v,rell_u,rell_v);
  Diodes[3]->SetLineColor(2);
  Diodes[3]->SetLineWidth(2);
  Diodes[3]->SetFillStyle(3000);

  cPosStudy_TrackhitPos_vs_Mult = new TCanvas("cPosStudy_TrackhitPos_vs_Mult",
					      "Track hit position vs diodes location",
					      900,900);
  cPosStudy_TrackhitPos_vs_Mult->Divide(NPadsX,NPadsY);
  int counter_mult_canvas = 0;
  for(int imult=0;imult<NPadsX*NPadsY;imult++) {
    if(imult+1 > 6) continue;
    counter_mult_canvas++;
    cPosStudy_TrackhitPos_vs_Mult->cd(imult+1);
    cPosStudy_TrackhitPos_vs_Mult->cd(imult+1)->SetFillColor(10);
    cPosStudy_TrackhitPos_vs_Mult->cd(imult+1)->SetTickx(1);
    cPosStudy_TrackhitPos_vs_Mult->cd(imult+1)->SetTicky(1);
    if(imult+1 <= huCGwidth_vs_Mult->GetXaxis()->GetNbins()) {
      huvCGtuv_vs_Mult[imult]->Draw("colz");
      lh_TrackhitPos_vs_Mult->Draw();
      Diodes[0]->Draw();
      Diodes[1]->Draw();
      Diodes[2]->Draw();
      Diodes[3]->Draw();
    }
  }
  counter_mult_canvas++;
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetFillColor(10);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetTickx(1);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetTicky(1);
  huvCGtuv->Draw("colz");
  lh_TrackhitPos_vs_Mult->Draw();
  Diodes[0]->Draw();
  Diodes[1]->Draw();
  Diodes[2]->Draw();
  Diodes[3]->Draw();
  counter_mult_canvas++;
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetFillColor(10);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetTickx(1);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetTicky(1);
  huvCGtuv_NoAsso->Draw("colz");
  lh_TrackhitPos_vs_Mult->Draw();
  Diodes[0]->Draw();
  Diodes[1]->Draw();
  Diodes[2]->Draw();
  Diodes[3]->Draw();
  cPosStudy_TrackhitPos_vs_Mult->Update();
  counter_mult_canvas++;
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetFillColor(10);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetTickx(1);
  cPosStudy_TrackhitPos_vs_Mult->cd(counter_mult_canvas)->SetTicky(1);
  hnpixc_vs_TrkDistToDiode->Draw("colz");

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cPosStudy_TrackhitPos_vs_Mult->Print(EPSNameO.Data());
    cPosStudy_TrackhitPos_vs_Mult->Print(EPSName.Data());
    cPosStudy_TrackhitPos_vs_Mult->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }
  
  cPosStudy_TrackhitPos_vs_Mult2 = new TCanvas("cPosStudy_TrackhitPos_vs_Mult2",
					       "Track hit position vs diodes location 2",
					       900,900);
  cPosStudy_TrackhitPos_vs_Mult2->Divide(NPadsX,NPadsY);
  counter_mult_canvas = 0;
  
  for(int imult=0;imult<NPadsX*NPadsY;imult++) {
    if(imult+1 > 6) continue;
    counter_mult_canvas++;
    cPosStudy_TrackhitPos_vs_Mult2->cd(imult+1);
    cPosStudy_TrackhitPos_vs_Mult2->cd(imult+1)->SetFillColor(10);
    cPosStudy_TrackhitPos_vs_Mult2->cd(imult+1)->SetTickx(1);
    cPosStudy_TrackhitPos_vs_Mult2->cd(imult+1)->SetTicky(1);
    if(imult+1 <= huCGwidth_vs_Mult->GetXaxis()->GetNbins()) {
      hDist_Trck_Diode_Asso_vs_Mult[imult]->Sumw2();
      hDist_Trck_Diode_Asso_vs_Mult[imult]->Draw();
    }
  }
  counter_mult_canvas++;
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetFillColor(10);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetTickx(1);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetTicky(1);
  hDist_Trck_Diode_Asso->Sumw2();
  hDist_Trck_Diode_Asso->Draw();
  counter_mult_canvas++;
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetFillColor(10);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetTickx(1);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetTicky(1);
  hDist_Trck_Diode_NoAsso->Sumw2();
  hDist_Trck_Diode_NoAsso->Draw();
  
  counter_mult_canvas++;
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetFillColor(10);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetTickx(1);
  cPosStudy_TrackhitPos_vs_Mult2->cd(counter_mult_canvas)->SetTicky(1);
  //if(hDist_Trck_Diode_NoAsso->Integral() > 0.0) hDist_Trck_Diode_NoAsso->Scale(1.0/hDist_Trck_Diode_NoAsso->Integral("width"));
  //if(hDist_Trck_Diode_Asso->Integral()   > 0.0) hDist_Trck_Diode_Asso->Scale(1.0/hDist_Trck_Diode_Asso->Integral("width"));
  //hDist_Trck_Diode_NoAsso->SetStats(false);
  //hDist_Trck_Diode_Asso->SetStats(false);
  //double Maximum_TrkDiodeDist = TMath::Max(hDist_Trck_Diode_NoAsso->GetMaximum(),
  //					   hDist_Trck_Diode_Asso->GetMaximum());
  //hDist_Trck_Diode_NoAsso->SetMaximum(Maximum_TrkDiodeDist*(1.0 + 0.40));
  //hDist_Trck_Diode_Asso->SetMaximum(  Maximum_TrkDiodeDist*(1.0 + 0.40));
  //hDist_Trck_Diode_NoAsso->Draw();
  //hDist_Trck_Diode_Asso->Draw("same");
  //TLegend* leg_TrkDiodeDist = new TLegend(0.18,0.7,0.4,0.88);
  //leg_TrkDiodeDist->SetFillColor(10);
  //leg_TrkDiodeDist->AddEntry(hDist_Trck_Diode_Asso,  "Asso.",    "l");
  //leg_TrkDiodeDist->AddEntry(hDist_Trck_Diode_NoAsso,"Non-Asso.","l");
  //leg_TrkDiodeDist->Draw("same");

  double R_effic_TrkDiodeDist[2];
  R_effic_TrkDiodeDist[0] = +1.0e+20;
  R_effic_TrkDiodeDist[1] = -1.0e+20;
  for(int ItrkDiode=0;ItrkDiode<hDist_Trck_Diode_NoAsso->GetXaxis()->GetNbins();ItrkDiode++) {
    double v,e;
    v = hEffic_vs_Dist_Trck_Diode->GetBinContent(ItrkDiode+1);
    e = hEffic_vs_Dist_Trck_Diode->GetBinError(ItrkDiode+1);
    if(v < 0.0) continue;
    if(R_effic_TrkDiodeDist[0] > v-e) R_effic_TrkDiodeDist[0] = v-e;
    if(R_effic_TrkDiodeDist[1] < v+e) R_effic_TrkDiodeDist[1] = v+e;
  }
  double Delta_Effic = R_effic_TrkDiodeDist[1] - R_effic_TrkDiodeDist[0];
  if(Delta_Effic == 0.0) {
    R_effic_TrkDiodeDist[0] =   0.0;
    R_effic_TrkDiodeDist[1] = 100.0;
  }
  else {
    R_effic_TrkDiodeDist[0] -= Delta_Effic*0.10;
    R_effic_TrkDiodeDist[1] += Delta_Effic*0.10;
  }
  //cout << R_effic_TrkDiodeDist[0] << "  " << R_effic_TrkDiodeDist[1] << endl;
  //TGaxis *axiseffic = new TGaxis(hDist_Trck_Diode_NoAsso->GetXaxis()->GetXmax(),
  //				 hDist_Trck_Diode_NoAsso->GetMinimum(),
  //                               hDist_Trck_Diode_NoAsso->GetXaxis()->GetXmax(),
  //                               hDist_Trck_Diode_NoAsso->GetMaximum(),
  //                               R_effic_TrkDiodeDist[0],R_effic_TrkDiodeDist[1],510,"+L");
  //axiseffic->SetLineColor(hEffic_vs_Dist_Trck_Diode->GetLineColor());
  //axiseffic->SetLabelColor(hEffic_vs_Dist_Trck_Diode->GetLineColor());
  //axiseffic->SetTitleColor(hEffic_vs_Dist_Trck_Diode->GetLineColor());
  //axiseffic->SetTitle("Efficiency (%)");
  //axiseffic->Draw("same");
  //
  //double a_Effic_TrkDiodeDist = (hDist_Trck_Diode_NoAsso->GetMaximum() - hDist_Trck_Diode_NoAsso->GetMinimum())/(R_effic_TrkDiodeDist[1] - R_effic_TrkDiodeDist[0]);
  //double b_Effic_TrkDiodeDist =  hDist_Trck_Diode_NoAsso->GetMinimum() - R_effic_TrkDiodeDist[0]*a_Effic_TrkDiodeDist;
  //for(int ItrkDiode=0;ItrkDiode<hDist_Trck_Diode_NoAsso->GetXaxis()->GetNbins();ItrkDiode++) {
  //  double effic     = hEffic_vs_Dist_Trck_Diode->GetBinContent(ItrkDiode+1);
  //  double effic_err = hEffic_vs_Dist_Trck_Diode->GetBinError(ItrkDiode+1);
  //  hEffic_vs_Dist_Trck_Diode->SetBinContent(ItrkDiode+1,effic*a_Effic_TrkDiodeDist + b_Effic_TrkDiodeDist);
  //  hEffic_vs_Dist_Trck_Diode->SetBinError(ItrkDiode+1,effic_err*a_Effic_TrkDiodeDist);
  //}
  //hEffic_vs_Dist_Trck_Diode->Draw("same");
  hEffic_vs_Dist_Trck_Diode->SetYTitle("Efficiency (%)");
  hEffic_vs_Dist_Trck_Diode->SetStats(false);
  hEffic_vs_Dist_Trck_Diode->SetMinimum(R_effic_TrkDiodeDist[0]);
  hEffic_vs_Dist_Trck_Diode->SetMaximum(R_effic_TrkDiodeDist[1]);
  hEffic_vs_Dist_Trck_Diode->Draw();

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    
    cPosStudy_TrackhitPos_vs_Mult2->Print(EPSNameO.Data());
    cPosStudy_TrackhitPos_vs_Mult2->Print(EPSName.Data());
    cPosStudy_TrackhitPos_vs_Mult2->Print(EPSNameC.Data());
    //command = TString("gs -dQUIET -dNOPAUSE -sDEVICE=pdfwrite -sOUTPUTFILE=") + EPSName_test + TString(" -dBATCH ") + EPSName_final + TString("   ") + EPSName;
    //gSystem->Exec(command.Data());
    //command = TString("mv ") + EPSName_test + TString("  ") + EPSName_final;
    //gSystem->Exec(command.Data());
    //command = TString("rm -rf ") + EPSName;
    //gSystem->Exec(command.Data());
  }

  // == Now save histograms

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
 
  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");
  h2dmatchedhits->Write();
  hhuS->Write();
  hhvS->Write();
  huv->Write();
  huv_rate->Write();
  h_pixels_event->Write();
  hhu->Write();
  hhv->Write();
  hxy->Write();
  hhx->Write();
  hhy->Write();
  h2dgoodhits->Write();
  h2DgoodSeedPixel->Write();
  hgoodSeedPixel->Write();
  hdCGDigU->Write();
  hdCGDigV->Write();
  hEta3DigU->Write();
  hEta3DigV->Write();
  hdCGEtaU->Write();
  hdCGEtaV->Write();

  cPosStudy_TrackhitPos_vs_Mult->Write();
  for(int imult=0;imult<huCGwidth_vs_Mult->GetXaxis()->GetNbins();imult++) {
    huvCGtuv_vs_Mult[imult]->Write();
    hDist_Trck_Diode_Asso_vs_Mult[imult]->Write();
  }
  huvCGtuv_NoAsso->Write();
  hDist_Trck_Diode_Asso->Write();
  hDist_Trck_Diode_NoAsso->Write();
  hnpixc_vs_TrkDistToDiode->Write();
  hEffic_vs_Dist_Trck_Diode->Write();

  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();

}


//________________________________________________________________________________________
void MimosaAnalysis::Calibration()
{

  // Plots all the usefull histograms for calibration
  //
  // JB 2010/07/27 based on previous MCalib class
  
  if(!CheckIfDone("mimosacalibration")) return;
  if(!fIfCalibration) return; // JB 2014/01/11
  
  UsedMacro[15] = 1;
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(11111);
  gStyle->SetPalette(1,0);

  //------------
  //---main canvas:
  cCalibration = new TCanvas("calibration","Calibration",550,10,750,850);
  cCalibration->Draw();
  cCalibration->Clear();
  cCalibration->SetFillColor(0);
  cCalibration->SetBorderMode(0);

  TPad* mpad0 = new TPad("mpad0","title",0.01,0.95,0.99,0.99);
  mpad0->SetGrid(1);
  mpad0->Draw();

  TPad* mpad1 = new TPad("mpad1","plots",0.01,0.01,0.99,0.94);
  mpad1->SetBottomMargin(4.);
  mpad1->SetGrid(1);
  mpad1->Draw();

  mpad0->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.9);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  Char_t Header[100];
  sprintf(Header,"M%d ; run %d; Pl %d, sub %d; Seed %3.1f; Neigh %3.1f",MimosaType,RunNumber,ThePlaneNumber,ThesubmatrixNumber,CUT_S2N_seed,CUT_S2N_neighbour);
  ttt->DrawText( 0.05,0.6,Header);
  mpad0->Update();


  mpad1->cd();
  //mpad1->SetFillColor(19);
  gStyle->SetOptStat(111111);  
  PlotCalibration(AutoZoom(hqSeedCalibration,"calib",2), 0)->Draw();
  mpad1->Update();

  //cd to result dir to save pad picture
  gSystem->cd(CreateGlobalResultDir());
  Int_t rn = fSession->GetRunNumber();
  Char_t CalibMainPs[50];
  sprintf(CalibMainPs,"Calib_main_%d.eps",rn);
  cCalibration->SaveAs(CalibMainPs); 
 
  printf("--Calib zoom drawn\n\n");
  //------------

  //------------
  //---details pads:

  sprintf( nom, "calib_distr1_P%dS%d", ThePlaneNumber, ThesubmatrixNumber);
  sprintf( titre, "Distributions 1 for calibration Pl %d Sub %d", ThePlaneNumber, ThesubmatrixNumber);
  Calib_distr1 = new TCanvas( nom, titre, 900, 900);
  Calib_distr1->SetBorderMode(0);
  Calib_distr1->Divide(2,2);

  Calib_distr1->cd(1);
  AutoZoom(hqNeighbourCalibration)->Draw();
  Calib_distr1->cd(2);
  AutoZoom(hqSeedVsNeighbourCalibration)->Draw();
  Calib_distr1->cd(3);
  AutoZoom(hSeedBetweenDist)->Draw();

  Calib_distr1->Update();


  // == Now save histograms

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
 
  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_ClCharge.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");
  hqSeedCalibration->Write();
  hqNeighbourCalibration->Write();
  hqSeedVsNeighbourCalibration->Write();
  hSeedBetweenDist->Write();

  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  delete ResultRootFile;
  MHist::Dir();

}


//_______________________________________________________________________________________
//
TPad* MimosaAnalysis::PlotCalibration(TH1F* h, Int_t manualFitRange)
{

  // Plot and fit an histogram which is expected to be
  // the charge distribution of the seed pixel.
  // The fit function is a gaussian(signal) + exponential(tail)
  // Returns a canvas with the fit result.
  //
  
  TPad* Pad = (TPad*)gPad; 

  // Fits calibration peaks

  Float_t param[4][4]; // [Pad number] [parameter number]: 0=constant, 1=Mean, 2=sigma  
  Float_t param2[4][4]; // Same for second peak.


  manualFitRange = 0;
  

  /*Float_t FitIntegral[4] = {0.,0.,0.,0.};
  Float_t FitIntegralb[4]= {0.,0.,0.,0.};*/

  Float_t Peak1 =1640.0;
  //Float_t Peak2 =1804.0;

  
  //TF1* Fit = new TF1("fit","gaus",h->GetMean()-2*h->GetRMS(),h->GetMean()+2*h->GetRMS());  
  TF1* Fit = new TF1("fit","gaus(0)+expo(3)",h->GetMean()-2*h->GetRMS(),h->GetMean()+2*h->GetRMS());  
  Fit->SetParName(0,"Norm (gaus)");
  Fit->SetParName(1,"Mean");
  Fit->SetParName(2,"Sigma");
  Fit->SetParName(3,"Norm (exp)");
  Fit->SetParName(4,"Slope");

  Fit->SetParLimits(0,0.,1e10);
  Fit->SetParLimits(1,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  Fit->SetParLimits(2,0,10*h->GetRMS());
  Fit->SetParLimits(3,0.,1e10);
  Fit->SetParLimits(4,-1e-10,0.);

  Fit->SetParameter(2,5); // width
  Fit->SetParameter(1, h->GetMaximumBin());   // peak
 
  
  
   h->Fit("fit","WME");
   param[0][0]=Fit->GetParameter(0); //constant.
   param[0][1]=Fit->GetParameter(1); //Mean.
   param[0][2]=Fit->GetParameter(2); //sigma.
   param[0][3]=Peak1/(Fit->GetParameter(1));
   
   TString status;
   if( gMinuit->fCstatu.Contains("SUCCESS"))
     { status = TString("GOOD");}
   else 
     { status = TString("BAD");}
   
  /*
   TF1* Fit2 = new TF1("fit2","gaus"); 
   Fit2->FixParameter(2,.3); // width
   Fit2->SetParameter(1, h->GetMaximumBin()*Peak2/Peak1);   // peak
  //Fit2->SetParameter(1, GetMax(h)*Peak2/Peak1);   // peak

     
   h->Fit("fit2","MER+");
   
   param2[0][0]=Fit2->GetParameter(0); //constant.
   param2[0][1]=Fit2->GetParameter(1); //Mean.
   param2[0][2]=Fit2->GetParameter(2); //sigma.
   param2[0][3]=Peak2/(Fit2->GetParameter(1));
   */
   
   int i =0;
   
   cout<<"\n*****************************************"<<endl;
   cout<<"*      Fits results  "<<RunNumber<<"          *"<<endl;
   cout<<"*****************************************\n"<<endl;
   
   cout<<"constant ** Mean ** sigma ** Gain (e- per ADC unit)"<<endl;
   cout<<param[i][0]<<" ** "<<param[i][1]<<" ** "<<param[i][2]<<" ** "<<param[i][3]<<endl;
   cout<<param2[i][0]<<" ** "<<param2[i][1]<<" ** "<<param2[i][2]<<" ** "<<param2[i][3]<<endl;
  
   
   Char_t calib_string[50];
      
   sprintf(calib_string,"Gain= %3.2f e-/ADCu, fit status= %s",param[0][3],status.Data());
   
   gPad->SetBottomMargin(.15);
   cout<<calib_string<<endl;
   TText* inforeso = new TText(0.02,0.02,calib_string);
   inforeso->SetNDC(kTRUE);
   inforeso->SetTextSize(0.04);
   inforeso->SetTextColor(2);
   inforeso->Draw();
   
   
   
   cout<<"END...."<<endl;
   


return Pad;

}

//________________________________________________________________________________________
//
void MimosaAnalysis::MiniVectors()
{
  // Plot histograms from minivectors study
  // NCS 2010/01/21

  if(!CheckIfDone("mimosaminivectors")) return;

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

  UsedMacro[16] = 1;

  TStyle* plain = new TStyle("Plain","Plain Style"); //
  plain->SetCanvasBorderMode(0);
  plain->SetPadColor(0);
  plain->SetCanvasColor(0);
  //plain->SetTitleColor(0);
  plain->SetStatColor(0);
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(0111);
  gStyle->SetStatX(1.0);
  gStyle->SetStatY(1.0);	
  gStyle->SetStatW(0.18);
  gStyle->SetStatH(0.15);
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetLabelSize(0.06);
  gStyle->SetTitleSize(0.06);
  gStyle->SetPadLeftMargin(0.15);  
  gStyle->SetTitleH(.065);
  gStyle->SetTitleH(.085);
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(111111);  
  gErrorIgnoreLevel=2007;
	
  // ---> 1st canvas.
  // --- Make CANVAS

  cMiniVec = new TCanvas("cMiniVec","MiniVectors analysis - 1",550,10,750,850);
  cMiniVec->Draw();
  cMiniVec->Clear();
  cMiniVec->SetFillColor(0);
  cMiniVec->SetBorderMode(0);

  cMiniVec->cd();
  TPad* mpad0 = new TPad("mpad0","title 1",0.01,0.95,0.99,0.99);
  mpad0->SetGrid(1);
  mpad0->Draw();
  mpad0->cd();
  TText *ttt = new TText();
  ttt->SetTextFont(25);
  ttt->SetTextColor(9);
  ttt->SetTextSize(.4);
  ttt->SetTextAlign(12);
  ttt->SetTextFont(22);
  std::stringstream HeaderStream;
  HeaderStream << "Mimosa " << MimosaType<< " - MINIVECTOR ; RUN " << RunNumber << "; first PL" << ThePlaneNumber << " & PL" << ThePlaneNumber2 << ", sub "<< ThesubmatrixNumber << ", dist " << TrackToHitDistanceLimit << "; Gain " << calibration << "; EFF " << MimosaEfficiency << "+- " << MimosaEfficiency_ERR << "; Seed "<< CUT_S2N_seed << "; Neigh " << CUT_S2N_neighbour;
  std::string HeaderString = HeaderStream.str();
  const char* Header = HeaderString.c_str();
  ttt->DrawText( 0.05,0.6,Header);
  mpad0->Update();

  cMiniVec->cd();
  TPad* mpad1 = new TPad("mpad1","",0.01,0.01,0.99,0.94);
  mpad1->SetGrid(1);
  mpad1->Divide(3,2);
  mpad1->Draw();

  TF1 *mygaus_posX = new TF1("mygaus_posX","gaus",-30.,30.);
  mygaus_posX->SetLineColor(3);
  mygaus_posX->SetLineStyle(2);
  
  TF1 *mygaus_posY = new TF1("mygaus_posY","gaus",-30.,30.);
  mygaus_posY->SetLineColor(3);
  mygaus_posY->SetLineStyle(2);
  
  TF1 *mygaus_angleX = new TF1("mygaus_angleX","gaus",-2.,1.5);
  mygaus_angleX->SetLineColor(3);
  mygaus_angleX->SetLineStyle(2);

  TF1 *mygaus_angleY = new TF1("mygaus_angleY","gaus",-1.5,2.0);
  mygaus_angleY->SetLineColor(3);
  mygaus_angleY->SetLineStyle(2);

  //TPaveStats *st;
  
  mpad1->cd(1);  //gPad->SetLogy();
  hDiffPosX->SetLineWidth(2);
  hDiffPosX->SetLineColor(2);
  hDiffPosX->Fit("mygaus_posX","");
  //AutoZoom(hUserHitCorrelationLine)->Draw();
  //AutoZoom(hDiffPosU)->Draw();
  hxtxPL3->Fit("gaus",""); 
  Double_t resXpl3 = hxtxPL3->GetFunction("gaus")->GetParameter(2);
  hxtxPL3->GetFunction("gaus")->SetLineColor(1);
  hxtxPL3->GetFunction("gaus")->SetLineStyle(2);
  hxtxPL4->Fit("gaus","");
  Double_t resXpl4 = hxtxPL4->GetFunction("gaus")->GetParameter(2);
  hxtxPL4->GetFunction("gaus")->SetLineColor(4);
  hxtxPL4->GetFunction("gaus")->SetLineStyle(2);
  hDiffPosX->Draw();
  hxtxPL3->Draw("sames");
  //TPaveStats *st = (TPaveStats*)hxtxPL3->FindObject("stats");
  //st->SetX1NDC(.0);
  //st->SetX2NDC(.2);
  //st->SetTextColor(3);
  hxtxPL4->Draw("sames");
  //st = (TPaveStats*)hxtxPL4->FindObject("stats");
  //st->SetX1NDC(.2);
  //st->SetX2NDC(.4);
  //st->SetTextColor(1);
  
  mpad1->cd(2);   //gPad->SetLogy();   
  hDiffPosY->SetLineColor(2);
  hDiffPosY->SetLineWidth(2);
  hDiffPosY->Fit("mygaus_posY","");
  //AutoZoom(hUserHitCorrelationCol)->Draw();
  //AutoZoom(hDiffPosV)->Draw();
  hytyPL3->Fit("gaus",""); 
  Double_t resYpl3 = hytyPL3->GetFunction("gaus")->GetParameter(2);
  hytyPL3->GetFunction("gaus")->SetLineColor(1);
  hytyPL3->GetFunction("gaus")->SetLineStyle(2);
  hytyPL4->Fit("gaus","");
  Double_t resYpl4 = hytyPL4->GetFunction("gaus")->GetParameter(2);
  hytyPL4->GetFunction("gaus")->SetLineColor(4);
  hytyPL4->GetFunction("gaus")->SetLineStyle(2);
  hDiffPosY->Draw();
  hytyPL3->Draw("sames");  
  hytyPL4->Draw("sames");
  
  mpad1->cd(3);  //gPad->SetLogy();
  hDiffAngleX->SetLineColor(2);
  hDiffAngleX->SetLineWidth(2);
  hDiffAngleX->Fit("mygaus_angleX","");
  //AutoZoom(hUserHitCorrelationLine)->Draw();
  //AutoZoom(hDiffAngleU)->Draw();
  hDiffAngleX->Draw();

  mpad1->cd(6);  //gPad->SetLogy();  
  printf("\nY-Angle resolution:\n");
  hDiffAngleY->SetLineColor(2);
  hDiffAngleY->SetLineWidth(2);
  hDiffAngleY->Fit("mygaus_angleY","");
  printf( "  mini-vector: %.1f degrees\n", mygaus_angleY->GetParameter(2));
  //AutoZoom(hUserHitCorrelationCol)->Draw();
  //AutoZoom(hDiffAngleV)->Draw();
  hDiffAngleY->Draw();
  mpad1->Update();
  
  mpad1->cd(4);
  hutuPL3->Fit("gaus",""); 
  Double_t resUpl3 = hutuPL3->GetFunction("gaus")->GetParameter(2);
  hutuPL4->Fit("gaus","");
  Double_t resUpl4 = hutuPL4->GetFunction("gaus")->GetParameter(2);
  hutuPL3->Draw();
  hutuPL4->Draw("sames");
  
  mpad1->cd(5);
  hvtvPL3->Fit("gaus",""); 
  Double_t resVpl3 = hvtvPL3->GetFunction("gaus")->GetParameter(2);
  hvtvPL4->Fit("gaus","");
  Double_t resVpl4 = hvtvPL4->GetFunction("gaus")->GetParameter(2);
  hvtvPL3->Draw();
  hvtvPL4->Draw("sames");

  // ---> 2nd canvas.
  
  cMiniVec1 = new TCanvas("cMiniVec1","MiniVectors analysis - 2",560,20,750,850);
  cMiniVec1->Draw();
  cMiniVec1->Clear();
  cMiniVec1->SetFillColor(0);
  cMiniVec1->SetBorderMode(0);  

  cMiniVec1->cd();
  mpad0 = new TPad("mpad10","title 1",0.01,0.95,0.99,0.99);
  mpad0->SetGrid(1);
  mpad0->Draw();
  mpad0->cd();
  ttt->DrawText( 0.05,0.6,Header);
  mpad0->Update();
  
  cMiniVec1->cd();
  mpad1 = new TPad("mpad11","",0.01,0.01,0.99,0.94);
  mpad1->SetGrid(1);
  mpad1->Divide(3,5);
  mpad1->Draw();

  mpad1->cd(1);
  hdiffydiffx11->Draw("colz");
  mpad1->cd(2);
  hDiffAngleX11->Draw();
  mpad1->cd(3);
  hDiffAngleY11->Draw();

  mpad1->cd(4);
  hdiffydiffx12->Draw("colz");
  mpad1->cd(5);
  hDiffAngleX12->Draw();
  mpad1->cd(6);
  hDiffAngleY12->Draw();
  
  mpad1->cd(7);
  hdiffydiffx21->Draw("colz");
  mpad1->cd(8);
  hDiffAngleX21->Draw();
  mpad1->cd(9);
  hDiffAngleY21->Draw();

  mpad1->cd(10);
  hdiffydiffx22->Draw("colz");
  mpad1->cd(11);
  hDiffAngleX22->Draw();
  mpad1->cd(12);
  hDiffAngleY22->Draw();

  mpad1->cd(13);
  hdiffydiffxg1g1->Draw("colz");
  mpad1->cd(14);
  hDiffAngleXg1g1->Draw();
  mpad1->cd(15);
  hDiffAngleYg1g1->Draw();

  
  //--------------PRINTOUT
  
  printf("Mini-vector results with planes %d and %d\n\n", ThePlaneNumber, ThePlaneNumber2);
	 
  printf("X-Position resolution:\n");
  printf( "  mini-vector: %.1f um\n", mygaus_posX->GetParameter(2));
  printf( "  first  point: %.1f um\n", resXpl3);
  printf( "  second point: %.1f um\n", resXpl4);
  
  printf("Y-Position resolution:\n");
  printf( "  mini-vector: %.1f um\n", mygaus_posY->GetParameter(2));
  printf( "  first  point: %.1f um\n", resYpl3);
  printf( "  second point: %.1f um\n", resYpl4);
  
  printf("U-Position resolution in each plane:\n");
  printf( "  first  point: %.1f um\n", resUpl3);
  printf( "  second point: %.1f um\n", resUpl4);
  
  printf("V-Position resolution in each plane:\n");
  printf( "  first  point: %.1f um\n", resVpl3);
  printf( "  second point: %.1f um\n", resVpl4);
  
  printf("\nX-Angle resolution:\n");
  printf( "  mini-vector: %.1f degrees\n", mygaus_angleX->GetParameter(2));

  printf("\nY-Angle resolution:\n");
  printf( "  mini-vector: %.1f degrees\n", mygaus_angleY->GetParameter(2));

  //--------------WRITE IN A ROOT FILE

  //cd to result dir
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;
 
  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d-%d_MiniVectors.root",RunNumber,ThePlaneNumber,ThePlaneNumber2);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2)); // JB 2011/07/07
  TFile *ResultRootFile=new TFile(ResultFileName2,"UPDATE");

  //hUserHitCorrelationCol->Write();
  //hUserHitCorrelationLine->Write();
  hDiffPosX->Write();
  hDiffPosY->Write();
  hDiffAngleX->Write();
  hDiffAngleY->Write();  
  
  hxtxPL3->Write();
  hytyPL3->Write();
  hxtxPL4->Write();
  hytyPL4->Write();

  hutuPL3->Write();
  hvtvPL3->Write();
  hutuPL4->Write();
  hvtvPL4->Write();

  hdiffydiffx->Write();
  hdiffydiffx11->Write();
  hDiffAngleX11->Write();
  hDiffAngleY11->Write();
  hdiffydiffx12->Write();
  hDiffAngleX12->Write();
  hDiffAngleY12->Write();
  hdiffydiffx21->Write();
  hDiffAngleX21->Write();
  hDiffAngleY21->Write();
  hdiffydiffx22->Write();
  hDiffAngleX22->Write();
  hDiffAngleY22->Write();
  hdiffydiffxg1g1->Write();
  hDiffAngleXg1g1->Write();
  hDiffAngleYg1g1->Write();
  
  //tree_plume->Write();
  cMiniVec->Write();
  cMiniVec1->Write();

  // Good reference tracks
  hGoodChi2Tx->Write();
  hGoodChi2Ty->Write();
  hGoodChi2AngleXZ->Write();
  hGoodChi2AngleYZ->Write();
  
  ResultRootFile->Write();
  ResultRootFile->Close();
  printf("\n All plots saved in %s\n", ResultRootFile->GetName());
  
  //delete ResultRootFile;

  MHist::Dir();

}

//-----------------------------------------------------------------------------------------------



//________________________________________________________________________________________
//
void MimosaAnalysis::CheckImaging(Double_t theta_init, Int_t Npeak, Double_t S, Double_t W, Bool_t FirstLoop, Bool_t chooseFit)
{
  // -- Called by MimosaAnalysis::MimosaImaging (MCommands.cxx)
  //
  // Plot and fit histograms to evaluate the spatial resolution of sensor using parallel bands chart.
  //    - h2dgoodhits:         2D histogram displaying hits in the geomatrix
  //    - h1RmsOnTheta:        1D histogram displaying the RMS vs theta
  //    - h1ProjectionOnY:     1D histogram displaying the hit projection on the y-axis of the frame parallel to the bands
  //    - h1ProjectionOnX:     1D histogram displaying the hit projection on the x-axis of the frame parallel to the bands
  //    - h1ProjectionOnYMult: same histogram as h1ProjectionOnY but depending on the clusters multiplicity  
  //    - h1ProjectionOnXMult: same histogram as h1ProjectionOnX but depending on the clusters multiplicity
  //
  // h1ProjectionOnX and h1ProjectionOnXMult are fitted to evaluate the spatial resolution (parameter sigma).
  //
  // If FirstLoop=kTRUE then the all histograms and canvas are saved
  // If FirstLoop=kFALSE then all histograms and canvas are saved in a .root file except the 2 first.
  //
  // JH 2014/06/16

#ifdef UseROOFIT
  
  double TickLength         = 0.05;
  double TitleOffSet        = 0.50;
  double TheSize            = 0.07;
  double TheSize2           = 0.10;
  double TheSize3           = 0.20;
  double TheBottomMargin_Dn = 0.35;
  double Fraction_Pad       = 0.30;
  
  hClusterSizeInLines->SetDirectory(0);
  hClusterSizeInColumns->SetDirectory(0);
  hClusterSize2DInLineAndColumns->SetDirectory(0);
  
  h2dgoodhits->SetDirectory(0);
  h1RmsOnTheta->SetDirectory(0);
  h2RmsOnThetaScanVsThetaCut->SetDirectory(0);
  h2DXprimeVsYprime->SetDirectory(0);
  h1ProjectionOnY->SetDirectory(0);
  h1ProjectionOnX->SetDirectory(0);
  for(Int_t i=0; i<4; i++) {  
    h1ProjectionOnYMult[i]->SetDirectory(0);
    h1ProjectionOnXMult[i]->SetDirectory(0);
  }
  h1Sigma->SetDirectory(0);
  h1NumberOfHitsMult->SetDirectory(0);
  for(Int_t i_mult=0; i_mult<4; i_mult++) {
    h2GoodHitsMult[i_mult]->SetDirectory(0);
  }
  hnpix_c->SetDirectory(0);
  for(Int_t i_mult=0; i_mult<6; i_mult++) {
    hdCGDigUVMult[i_mult]->SetDirectory(0);
  }
  gFile->Close();

  TString TheOutFileName = TString(fWorkingDirectory) + TString("file_out_91893.out");


  // **********************************************************************************
  // ************************************* PLOTS **************************************
  //
  // Plots: h2dgoodhits, h1RmsOnTheta, h1ProjectionOnY, h1ProjectionOnYMult
  // **********************************************************************************
  gStyle->SetTitleOffset(1.);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1111);
  gStyle->SetPalette(1,0);
  

  //---------------------------------------------------------------
  // Evaluate the axis range of 2D histograms (depending on the bands angle)
  Double_t Xrange[2], Yrange[2];
  if(theta_init >=0) { 
    Xrange[0] = geomUmin*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmax*TMath::Sin(theta_init*TMath::Pi()/180.0);
    Xrange[1] = geomUmax*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmin*TMath::Sin(theta_init*TMath::Pi()/180.0);
    Yrange[0] = geomUmin*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmin*TMath::Cos(theta_init*TMath::Pi()/180.0);
    Yrange[1] = geomUmax*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmax*TMath::Cos(theta_init*TMath::Pi()/180.0);
  }
  else {
    Xrange[1] = geomUmin*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmax*TMath::Sin(theta_init*TMath::Pi()/180.0);
    Xrange[0] = geomUmax*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmin*TMath::Sin(theta_init*TMath::Pi()/180.0);
    Yrange[1] = geomUmin*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmin*TMath::Cos(theta_init*TMath::Pi()/180.0);
    Yrange[0] = geomUmax*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmax*TMath::Cos(theta_init*TMath::Pi()/180.0);
  }

  if ( Xrange[1]-Xrange[0] > Yrange[1]-Yrange[0] ) {
    
    Double_t RangeDifference = (Xrange[1]-Xrange[0]) - (Yrange[1]-Yrange[0]);
    Yrange[0] -= 0.5 * RangeDifference;
    Yrange[1] += 0.5 * RangeDifference;
    
  }
  else {
    
    Double_t RangeDifference = (Yrange[1]-Yrange[0]) - (Xrange[1]-Xrange[0]);
    Xrange[0] -= 0.5 * RangeDifference;
    Xrange[1] += 0.5 * RangeDifference;  
    
  }
  
  double delta = Xrange[1] - Xrange[0];
  Xrange[0] -= 0.1*delta;
  Xrange[1] += 0.1*delta;

  delta = Yrange[1] - Yrange[0];
  Yrange[0] -= 0.1*delta;
  Yrange[1] += 0.1*delta;
  
  
  //---------------------------------------------------------------
  // Then plot
  TCanvas *cImaging1 = new TCanvas("cImaging1","Features on matrix",1000,1000);
  cImaging1->Draw();
  cImaging1->Clear();
  cImaging1->SetBorderMode(0);
  cImaging1->SetTickx(1);
  cImaging1->SetTicky(1);    
  h2dgoodhits->SetAxisRange(Xrange[0], Xrange[1],"X");
  h2dgoodhits->SetAxisRange(Yrange[0], Yrange[1],"Y");
  h2dgoodhits->SetStats(false);
  h2dgoodhits->Draw("colz");

  double Points_X[4];
  double Points_Y[4];
  Points_X[0] =  geomUmin*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmin*TMath::Sin(theta_init*TMath::Pi()/180.0);
  Points_Y[0] =  geomUmin*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmin*TMath::Cos(theta_init*TMath::Pi()/180.0);
  Points_X[1] =  geomUmax*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmin*TMath::Sin(theta_init*TMath::Pi()/180.0);
  Points_Y[1] =  geomUmax*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmin*TMath::Cos(theta_init*TMath::Pi()/180.0);
  Points_X[2] =  geomUmax*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmax*TMath::Sin(theta_init*TMath::Pi()/180.0);
  Points_Y[2] =  geomUmax*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmax*TMath::Cos(theta_init*TMath::Pi()/180.0);
  Points_X[3] =  geomUmin*TMath::Cos(theta_init*TMath::Pi()/180.0) - geomVmax*TMath::Sin(theta_init*TMath::Pi()/180.0);
  Points_Y[3] =  geomUmin*TMath::Sin(theta_init*TMath::Pi()/180.0) + geomVmax*TMath::Cos(theta_init*TMath::Pi()/180.0);

  TLine* TheZone[4];
  TheZone[0] = new TLine(Points_X[0],Points_Y[0],
			 Points_X[1],Points_Y[1]);
  TheZone[0]->SetLineColor(1);
  TheZone[0]->SetLineWidth(2);
  TheZone[0]->SetLineStyle(1);
  TheZone[1] = new TLine(Points_X[1],Points_Y[1],
			 Points_X[2],Points_Y[2]);
  TheZone[1]->SetLineColor(1);
  TheZone[1]->SetLineWidth(2);
  TheZone[1]->SetLineStyle(1);
  TheZone[2] = new TLine(Points_X[2],Points_Y[2],
			 Points_X[3],Points_Y[3]);
  TheZone[2]->SetLineColor(1);
  TheZone[2]->SetLineWidth(2);
  TheZone[2]->SetLineStyle(1);
  TheZone[3] = new TLine(Points_X[3],Points_Y[3],
			 Points_X[0],Points_Y[0]);
  TheZone[3]->SetLineColor(1);
  TheZone[3]->SetLineWidth(2);
  TheZone[3]->SetLineStyle(1);

  TheZone[0]->Draw();
  TheZone[1]->Draw();
  TheZone[2]->Draw();
  TheZone[3]->Draw();

  TCanvas *cImaging2 = NULL;
  
  if ( FirstLoop ) { // if the angle theta has been evaluated

    Double_t RMSmin = 1.0e+20;
    Double_t ThetaScanMin = -999;
    Double_t ThetaCutMin = -999;
    
    for (Int_t i_thetaCut=0; i_thetaCut<h2RmsOnThetaScanVsThetaCut->GetYaxis()->GetNbins(); i_thetaCut++) {
      for (Int_t i_thetaScan=0; i_thetaScan<h2RmsOnThetaScanVsThetaCut->GetXaxis()->GetNbins(); i_thetaScan++) {
        
        if (h2RmsOnThetaScanVsThetaCut->GetBinContent(i_thetaScan+1, i_thetaCut+1) < RMSmin ) {
          RMSmin = h2RmsOnThetaScanVsThetaCut->GetBinContent(i_thetaScan+1, i_thetaCut+1);
          ThetaScanMin = h2RmsOnThetaScanVsThetaCut->GetXaxis()->GetBinCenter(i_thetaScan+1);
          ThetaCutMin = h2RmsOnThetaScanVsThetaCut->GetYaxis()->GetBinCenter(i_thetaCut+1);
        }
      }
    }
    
    TLine* l_ThetaMin_V = new TLine(ThetaScanMin, h2RmsOnThetaScanVsThetaCut->GetYaxis()->GetXmin(),
                                    ThetaScanMin, h2RmsOnThetaScanVsThetaCut->GetYaxis()->GetXmax());
    l_ThetaMin_V->SetLineColor(1);
    l_ThetaMin_V->SetLineWidth(2);
    l_ThetaMin_V->SetLineStyle(2);
  
    TLine* l_ThetaMin_H = new TLine(h2RmsOnThetaScanVsThetaCut->GetXaxis()->GetXmin(), ThetaCutMin,
                                    h2RmsOnThetaScanVsThetaCut->GetXaxis()->GetXmax(), ThetaCutMin);
    l_ThetaMin_H->SetLineColor(1);
    l_ThetaMin_H->SetLineWidth(2);
    l_ThetaMin_H->SetLineStyle(2);
  
    
    cImaging2 = new TCanvas("cImaging2","RMS vs Theta",1);
    cImaging2->Draw();
    cImaging2->Clear();
    cImaging2->SetTickx(1);
    cImaging2->SetTicky(1);
    cImaging2->SetBorderMode(0);
    //h1RmsOnTheta->SetStats(false);
    //h1RmsOnTheta->SetAxisRange(0,90,"X");
    //h1RmsOnTheta->Draw();
    h2RmsOnThetaScanVsThetaCut->SetContour(100);
    h2RmsOnThetaScanVsThetaCut->SetStats(false);
    h2RmsOnThetaScanVsThetaCut->Draw("CONT1Z");
    l_ThetaMin_H->Draw();
    l_ThetaMin_V->Draw();
    
  } // end if FirstLoop
 
  // **********************************************************************************
  // ******************************** PLOTS AND FITS **********************************
  //
  // Fit of the data projection on x, depending on different multiplicities:
  // h1ProjectionOnX, h1ProjectionOnXMult
  // **********************************************************************************
  
  // **************************************************************
  // PREPARATION OF THE FIT
  
  //---------------------------------------------------------------
  // Search for the position of the first peak center: x0
  Bool_t condition = kTRUE;
  Double_t binmax = 0;
  Double_t bini;
  Int_t i_max, i_rangemax;
  
  Int_t i = 1;

  do {
    
    bini = h1ProjectionOnX->GetBinContent(i);
    
    if (bini < binmax-0.8*binmax && bini > 0) {
        condition=kFALSE;
        i_rangemax=i;
    }
    
    if (bini > binmax) {
      binmax = bini;
      i_max = i;
    }
    i++;  
    
  }
  while (condition);

  Double_t x0 = h1ProjectionOnX->GetBinCenter(i_max);


  //---------------------------------------------------------------
  // Search for the range of the first peak
  Double_t rangex0[2];
  rangex0[0] = 1.0e20;
  rangex0[1] = -1.0e20;
  
  for (Int_t i=0; i<=i_rangemax; i++) {
  
    bini = h1ProjectionOnX->GetBinContent(i);
    
    if (TMath::Abs(bini- binmax) < 0.5*binmax) { 
      Double_t center = h1ProjectionOnX->GetBinCenter(i+1);
      Double_t width = 0.5*h1ProjectionOnX->GetBinWidth(i+1);
      
      if (rangex0[0] > center-width) rangex0[0] = center-width;
      if (rangex0[1] < center+width) rangex0[1] = center+width;
    }
  }
  
  cout << "\n*------------------------------------*" << endl;
  cout << "Expected position of the 1st peak:  x0 = " << x0 << "   N0 = " << h1ProjectionOnX->GetBinContent(i_max) <<endl;
  cout << "Expected range of the 1st peak: xRange = (" << rangex0[0] << ", " << rangex0[1] << ")" << endl;
  cout << "Expected W = " << (rangex0[1] - rangex0[0]) << endl;
  cout << "*------------------------------------*\n" << endl;  
  

  //---------------------------------------------------------------
  // Fit function
  //
  // The fit function is:
  //    Sum_{i=0}^{N-1}  0.5 * N_{i} * ( Erfc( x_low / (sqrt(2)*sigma) ) - Erfc( x_high / (sqrt(2)*sigma) ) )
  //
  // where:     N_{i}:  the height of the peak number i
  //            x_low:  x0 + i*S + (i+0.5)*W
  //            x_high: x0 + i*S + (i-0.5)*W
  //            S:      the distance between bands
  //            W:      the bands width
  //            Erfc:   the complementary error function defined in ROOT
  //
  // Two types of fit are possible:
  // - chooseFit=true: where the parameters S and W are the same for all bands
  //    [0] = x0        [1] = S         [2] = W         [3] = sigma     [i] (i>4) = N_i  
  // - chooseFit=false: where the parameters S and W depend on bands.
  //    [0] = x0        [1] = sigma     [2] = N_1       [3] = W_1       [4+(i-1)*3] = W_i       [5+(i-1)*3] = N_{i+1}       [6+(i-1)*3] = W_{i+1}

  //Declaration of the variables to be included in the dataset
  //- X' and the cluster multiplicity
  

  double DeltaX0 = rangex0[1]-rangex0[0];

  //Declaration of the variables to be included in the dataset
  //- X' and the cluster multiplicity
  RooRealVar* Xprime       = new RooRealVar("Xprime","Xprime",geomUmin,geomUmax,"#mum");                        //AP 2014/07/24
  RooRealVar* Yprime       = new RooRealVar("Yprime","Yprime",geomVmin,geomVmax,"#mum");                        //AP 2014/07/24
  RooRealVar* Multiplicity = new RooRealVar("Multiplicity","Multiplicity",0.0,20.0);                            //AP 2014/07/24

  RooRealVar* X0           = new RooRealVar("X0",   "X_{0}",   x0,     rangex0[0],  rangex0[1],"#mum");         //AP 2014/07/24
  RooRealVar* sigma        = new RooRealVar("sigma","#sigma", 3.5,     0.5,         20.0,      "#mum");         //AP 2014/07/24
  RooRealVar* beta         = new RooRealVar("beta", "#beta",  1.0e-2,  0.0,          1.0,       "");            //AP 2014/07/24
  RooRealVar* alpha        = new RooRealVar("alpha","#alpha", 0.0,  -180.0,        180.0,       "degrees");     //AP 2014/07/24
  RooRealVar* phi          = new RooRealVar("phi",  "#phi",   0.0,    -5.0,          5.0,       "degrees");     //AP 2014/07/24

  //RooRealVar* W1           = new RooRealVar("W1",   "W_{1}",  DeltaX0, 0.1*DeltaX0,  10.0*DeltaX0,"#mum");       //AP 2014/07/24
  //RooRealVar* W2           = new RooRealVar("W2",   "W_{2}",  DeltaX0, 0.1*DeltaX0,  10.0*DeltaX0,"#mum");       //AP 2014/07/24
  //RooRealVar* W3           = new RooRealVar("W3",   "W_{3}",  DeltaX0, 0.1*DeltaX0,  10.0*DeltaX0,"#mum");       //AP 2014/07/24
  //RooRealVar* W4           = new RooRealVar("W4",   "W_{4}",  DeltaX0, 0.1*DeltaX0,  10.0*DeltaX0,"#mum");       //AP 2014/07/24
  //RooRealVar* W5           = new RooRealVar("W5",   "W_{5}",  DeltaX0, 0.1*DeltaX0,  10.0*DeltaX0,"#mum");       //AP 2014/07/24

  RooRealVar* W1           = new RooRealVar("W1",   "W_{1}",        W, 0.1*W,        2.0*W,      "#mum");       //AP 2014/07/24
  RooRealVar* W2           = new RooRealVar("W2",   "W_{2}",        W, 0.1*W,        2.0*W,      "#mum");       //AP 2014/07/24
  RooRealVar* W3           = new RooRealVar("W3",   "W_{3}",        W, 0.1*W,        2.0*W,      "#mum");       //AP 2014/07/24
  RooRealVar* W4           = new RooRealVar("W4",   "W_{4}",        W, 0.1*W,        2.0*W,      "#mum");       //AP 2014/07/24
  RooRealVar* W5           = new RooRealVar("W5",   "W_{5}",        W, 0.1*W,        2.0*W,      "#mum");       //AP 2014/07/24

  RooRealVar* S2           = new RooRealVar("S2",   "S_{2}",        S, 0.1*S,        5.0*S,      "#mum");       //AP 2014/07/24
  RooRealVar* S3           = new RooRealVar("S3",   "S_{3}",        S, 0.1*S,        5.0*S,      "#mum");       //AP 2014/07/24
  RooRealVar* S4           = new RooRealVar("S4",   "S_{4}",        S, 0.1*S,        5.0*S,      "#mum");       //AP 2014/07/24
  RooRealVar* S5           = new RooRealVar("S5",   "S_{5}",        S, 0.1*S,        5.0*S,      "#mum");       //AP 2014/07/24

  DXRay2DPdf* TheSignalPdf = NULL;

  if(Npeak <= 0 || Npeak > 5) {
    cout << endl;
    cout << "Npeak has values out of range (1,5). If needs higher than 5 need to implement it in DXRay2DPdf!!!" << endl;
    cout << "Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }
  
  if(chooseFit) { // if chooseFit (S and W unchanging)    
    cout << "\nFit function with S and W the same for all bands" << endl;
    
    // *********************************
    // FIT OF THE GLOBAL HISTOGRAM
    
    W1->SetName("W");
    W1->SetTitle("W");

    S2->SetName("S");
    S2->SetTitle("S");

    if(Npeak == 1) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 2) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W1,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    *S2,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 3) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W1,
				    *W1,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    *S2,
				    *S2,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 4) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W1,
				    *W1,
				    *W1,
				    RooRealConstant::value(0.0),
				    *S2,
				    *S2,
				    *S2,
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 5) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W1,
				    *W1,
				    *W1,
				    *W1,
				    *S2,
				    *S2,
				    *S2,
				    *S2);
    }
  } // end if chooseFit (S and W unchanging)
  else { // if chooseFit=false, (S and W are considered as depending on bands)
    cout << "\nFit function with S and W different for all bands" << endl;

    if(Npeak == 1) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 2) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W2,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    *S2,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 3) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W2,
				    *W3,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0),
				    *S2,
				    *S3,
				    RooRealConstant::value(0.0),
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 4) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W2,
				    *W3,
				    *W4,
				    RooRealConstant::value(0.0),
				    *S2,
				    *S3,
				    *S4,
				    RooRealConstant::value(0.0));
    }
    else if(Npeak == 5) {
      TheSignalPdf = new DXRay2DPdf("TheSignalPdf","Signal Pdf", 
				    *Xprime,*Yprime,
				    *X0,
				    *sigma,
				    *beta,
				    *alpha,
				    *phi,
				    *W1,
				    *W2,
				    *W3,
				    *W4,
				    *W5,
				    *S2,
				    *S3,
				    *S4,
				    *S5);
    }
  } // end if chooseFit=false

  //---------------------------------------------------------------
  // Save the different values of sigma before plot them    
  //   h1Sigma->SetBinContent(1, fitFunc->GetParameter(1));
  //   h1Sigma->SetBinError(1, fitFunc->GetParError(1));
  //   for (Int_t i_mult=0; i_mult<4; i_mult++) h1Sigma->SetBinContent(i_mult+2, fitFuncMult[i_mult]->GetParameter(1));
  //   for (Int_t i_mult=0; i_mult<4; i_mult++) h1Sigma->SetBinError(i_mult+2, fitFuncMult[i_mult]->GetParError(1));

  RooUniform  TheNoisePdf_Xprime("TheNoisePdf_Xprime","Noise Pdf X'",RooArgSet(*Xprime));
  RooUniform  TheNoisePdf_Yprime("TheNoisePdf_Yprime","Noise Pdf Y'",RooArgSet(*Yprime));
  RooProdPdf  TheNoisePdf("TheNoisePdf","Noise Pdf",RooArgSet(TheNoisePdf_Xprime,TheNoisePdf_Yprime));

  RooRealVar Nsig("Nsig","Signal Yield",
		  h1ProjectionOnX->Integral(),
		  0.1*h1ProjectionOnX->Integral(),
		  10.0*h1ProjectionOnX->Integral());
  RooRealVar Nnoise("Nnoise","Noise Yield",
		    1.0e-4*h1ProjectionOnX->Integral(),
		    0.0,
		    0.20*h1ProjectionOnX->Integral());

  RooAddPdf TheTotalPdf("TheTotalPdf","The Total Pdf",
			RooArgSet(*TheSignalPdf,TheNoisePdf),
			RooArgSet(Nsig,Nnoise));
  Nsig.setConstant(false);
  Nnoise.setConstant(false);

  TH1F* h1ProjectionOnX_clone = new TH1F(*h1ProjectionOnX);
  h1ProjectionOnX_clone->SetYTitle("Pulls");
  h1ProjectionOnX_clone->SetStats(false);
  h1ProjectionOnX_clone->SetMaximum( 6.5);
  h1ProjectionOnX_clone->SetMinimum(-6.5);
  h1ProjectionOnX_clone->GetXaxis()->SetTitleSize(TheSize2);
  h1ProjectionOnX_clone->GetXaxis()->SetLabelSize(TheSize2);
  h1ProjectionOnX_clone->GetYaxis()->SetTitleSize(TheSize2);
  h1ProjectionOnX_clone->GetYaxis()->SetLabelSize(TheSize2);
  h1ProjectionOnX_clone->GetYaxis()->SetTitleOffset(TitleOffSet);
  h1ProjectionOnX_clone->GetXaxis()->SetTickLength(TickLength);
  h1ProjectionOnX_clone->SetDirectory(0);

  TLine* l_Xprime_pull_0 = new TLine(h1ProjectionOnX->GetXaxis()->GetXmin(),0.0,
                                     h1ProjectionOnX->GetXaxis()->GetXmax(),0.0);
  l_Xprime_pull_0->SetLineColor(1);
  l_Xprime_pull_0->SetLineWidth(1);
  l_Xprime_pull_0->SetLineStyle(1);
  TLine* l_Xprime_pull_1 = new TLine(h1ProjectionOnX->GetXaxis()->GetXmin(),2.0,
                                     h1ProjectionOnX->GetXaxis()->GetXmax(),2.0);
  l_Xprime_pull_1->SetLineColor(2);
  l_Xprime_pull_1->SetLineWidth(2);
  l_Xprime_pull_1->SetLineStyle(2);
  TLine* l_Xprime_pull_2 = new TLine(h1ProjectionOnX->GetXaxis()->GetXmin(),-2.0,
                                     h1ProjectionOnX->GetXaxis()->GetXmax(),-2.0);
  l_Xprime_pull_2->SetLineColor(2);
  l_Xprime_pull_2->SetLineWidth(2);
  l_Xprime_pull_2->SetLineStyle(2);  

  TH1F* h1ProjectionOnY_clone = new TH1F(*h1ProjectionOnY);
  h1ProjectionOnY_clone->SetYTitle("Pulls");
  h1ProjectionOnY_clone->SetStats(false);
  h1ProjectionOnY_clone->SetMaximum( 6.5);
  h1ProjectionOnY_clone->SetMinimum(-6.5);
  h1ProjectionOnY_clone->GetXaxis()->SetTitleSize(TheSize2);
  h1ProjectionOnY_clone->GetXaxis()->SetLabelSize(TheSize2);
  h1ProjectionOnY_clone->GetYaxis()->SetTitleSize(TheSize2);
  h1ProjectionOnY_clone->GetYaxis()->SetLabelSize(TheSize2);
  h1ProjectionOnY_clone->GetYaxis()->SetTitleOffset(TitleOffSet);
  h1ProjectionOnY_clone->GetXaxis()->SetTickLength(TickLength);
  h1ProjectionOnY_clone->SetDirectory(0);

  TLine* l_Yprime_pull_0 = new TLine(h1ProjectionOnY->GetXaxis()->GetXmin(),0.0,
                                     h1ProjectionOnY->GetXaxis()->GetXmax(),0.0);
  l_Yprime_pull_0->SetLineColor(1);
  l_Yprime_pull_0->SetLineWidth(1);
  l_Yprime_pull_0->SetLineStyle(1);
  TLine* l_Yprime_pull_1 = new TLine(h1ProjectionOnY->GetXaxis()->GetXmin(),2.0,
                                     h1ProjectionOnY->GetXaxis()->GetXmax(),2.0);
  l_Yprime_pull_1->SetLineColor(2);
  l_Yprime_pull_1->SetLineWidth(2);
  l_Yprime_pull_1->SetLineStyle(2);
  TLine* l_Yprime_pull_2 = new TLine(h1ProjectionOnY->GetXaxis()->GetXmin(),-2.0,
                                     h1ProjectionOnY->GetXaxis()->GetXmax(),-2.0);
  l_Yprime_pull_2->SetLineColor(2);
  l_Yprime_pull_2->SetLineWidth(2);
  l_Yprime_pull_2->SetLineStyle(2);  


  RooDataSet* TheDataSet = RooDataSet::read(TheOutFileName.Data(),RooArgList(*Xprime,*Yprime,*Multiplicity),"Q");  //AP 2014/07/24

  X0->setVal(x0);
  sigma->setVal(3.5);
  beta->setVal(0.5);
  alpha->setVal(0.0);
  phi->setVal(0.0);
  W1->setVal(DeltaX0);
  W2->setVal(DeltaX0);
  W3->setVal(DeltaX0);
  W4->setVal(DeltaX0);
  W5->setVal(DeltaX0);
  S2->setVal(S);
  S3->setVal(S);
  S4->setVal(S);
  S5->setVal(S);

  RooFitResult* resFit_all = TheTotalPdf.fitTo(*TheDataSet,RooFit::Save());
  resFit_all->SetName("resFit_all");
  resFit_all->SetTitle("RooFitResult for X'projection for all multiplicities");
  resFit_all->Print("v");

  // END OF PREPARATION OF THE FIT
  // **************************************************************  


  // **********************************************************************************
  // ************************** SPATIAL RESOLUTION RESULTS ****************************
  //
  // Display the spatial resultion (parameter sigma) of the data projection on x',
  // depending on clusters multiplicity.
  // **********************************************************************************

  //X' vs Y' plot:
  TCanvas *cImaging13 = new TCanvas("cImaging13", "X' and Y' 2D", 1);
  cImaging13->Clear();
  cImaging13->SetFillColor(10);
  cImaging13->SetFrameFillColor(10);
  cImaging13->SetTickx(1);
  cImaging13->SetTicky(1);
  h2DXprimeVsYprime->SetStats(false);
  h2DXprimeVsYprime->Draw("colz");
  double Yave_tmp = 0.5*(h2DXprimeVsYprime->GetYaxis()->GetXmin() + h2DXprimeVsYprime->GetYaxis()->GetXmax());
  double Xtmp,Ytmp;
  double thetan = TMath::Tan(0.5*TMath::Pi() + phi->getVal()*(TMath::Pi()/180.0));
  Ytmp = h2DXprimeVsYprime->GetYaxis()->GetXmin();
  double PosX1 = ((Ytmp - Yave_tmp)/thetan) + X0->getVal();
  double PosY1 = Ytmp;
  Ytmp = h2DXprimeVsYprime->GetYaxis()->GetXmax();
  double PosX2 = ((Ytmp - Yave_tmp)/thetan) + X0->getVal();
  double PosY2 = Ytmp;
  TLine* line_axis_band = new TLine(PosX1,PosY1,
				    PosX2,PosY2);
  line_axis_band->SetLineColor(1);
  line_axis_band->SetLineWidth(2);
  line_axis_band->SetLineStyle(1);
  line_axis_band->Draw();

  TH2F* Pull2DXprimeVsYprime = new TH2F(*h2DXprimeVsYprime);
  Pull2DXprimeVsYprime->SetName("Chi2DXprimeVsYprime");
  Pull2DXprimeVsYprime->SetTitle("Pull vs X' and Y' for all multiplicities");
  Pull2DXprimeVsYprime->SetZTitle("Pull");
  Pull2DXprimeVsYprime->SetDirectory(0);
  
  for(int ixprime=0;ixprime<h2DXprimeVsYprime->GetXaxis()->GetNbins();ixprime++) {
    double cx = h2DXprimeVsYprime->GetXaxis()->GetBinCenter(ixprime+1);
    double wx = h2DXprimeVsYprime->GetXaxis()->GetBinWidth(ixprime+1)*0.5;
    double Xrange_tmp[2];
    Xrange_tmp[0] = cx - wx;
    Xrange_tmp[1] = cx + wx;
    Xprime->setVal(cx);

    for(int iyprime=0;iyprime<h2DXprimeVsYprime->GetYaxis()->GetNbins();iyprime++) {
      double cy = h2DXprimeVsYprime->GetYaxis()->GetBinCenter(iyprime+1);
      double wy = h2DXprimeVsYprime->GetYaxis()->GetBinWidth(iyprime+1)*0.5;
       double Yrange_tmp[2];
      Yrange_tmp[0] = cy - wy;
      Yrange_tmp[1] = cy + wy;
      Yprime->setVal(cy);

      if(h2DXprimeVsYprime->GetBinContent(ixprime+1,iyprime+1) == 0) continue;
      
      double val = TheSignalPdf->getVal(RooArgSet(*Xprime,*Yprime))*Nsig.getVal();
      val += TheNoisePdf.getVal(RooArgSet(*Xprime,*Yprime))*Nnoise.getVal();
      val *= (Xrange_tmp[1]-Xrange_tmp[0])*(Yrange_tmp[1]-Yrange_tmp[0]);
      val = h2DXprimeVsYprime->GetBinContent(ixprime+1,iyprime+1) - val;
      val /= sqrt(h2DXprimeVsYprime->GetBinContent(ixprime+1,iyprime+1));

      Pull2DXprimeVsYprime->SetBinContent(ixprime+1,iyprime+1,val);

    }
  }

  //X' vs Y' plot:
  TCanvas *cImaging14 = new TCanvas("cImaging14", "Pull of X' and Y' 2D", 1);
  cImaging14->Clear();
  cImaging14->SetFillColor(10);
  cImaging14->SetFrameFillColor(10);
  cImaging14->SetTickx(1);
  cImaging14->SetTicky(1);
  Pull2DXprimeVsYprime->SetStats(false);
  Pull2DXprimeVsYprime->Draw("colz");

  TCanvas *cImaging15 = new TCanvas("cImaging15", "Cluster size in #line and #columns", 1);
  cImaging15->Clear();
  cImaging15->SetFillColor(10);
  cImaging15->SetFrameFillColor(10);
  cImaging15->SetTickx(1);
  cImaging15->SetTicky(1);
  cImaging15->Divide(2,2);
  cImaging15->cd(1);
  cImaging15->cd(1)->SetFillColor(10);
  cImaging15->cd(1)->SetFrameFillColor(10);
  cImaging15->cd(1)->SetTickx(1);
  cImaging15->cd(1)->SetTicky(1);
  hClusterSize2DInLineAndColumns->Draw("colz");
  cImaging15->cd(2);
  cImaging15->cd(2)->SetFillColor(10);
  cImaging15->cd(2)->SetFrameFillColor(10);
  cImaging15->cd(2)->SetTickx(1);
  cImaging15->cd(2)->SetTicky(1);
  hClusterSizeInColumns->Draw();
  cImaging15->cd(3);
  cImaging15->cd(3)->SetFillColor(10);
  cImaging15->cd(3)->SetFrameFillColor(10);
  cImaging15->cd(3)->SetTickx(1);
  cImaging15->cd(3)->SetTicky(1);
  hClusterSizeInLines->Draw();

  // **********************************************************************************
  // ********************************* MULTIPLICITY ***********************************
  //
  // Display the hits rate and the hit map (2D) depending on clusters multiplicity.
  // + Number of hits per event
  // **********************************************************************************
  TCanvas *cImaging8 = new TCanvas("cImaging8", "Multiplicity: Hits rate", 1);
  cImaging8->Draw();
  cImaging8->Clear();
  cImaging8->SetBorderMode(0);
  cImaging8->SetTickx(1);
  cImaging8->SetTicky(1);
  h1NumberOfHitsMult->SetStats(false);
  h1NumberOfHitsMult->SetLineColor(kBlue);
  h1NumberOfHitsMult->SetMarkerColor(kBlue);
  h1NumberOfHitsMult->SetMarkerStyle(20);
  h1NumberOfHitsMult->Draw();


  TCanvas *cImaging10 = new TCanvas("cImaging10", "Multiplicity: pixels in cluster", 1);
  cImaging10->Draw();
  cImaging10->Clear();
  cImaging10->SetBorderMode(0);
  cImaging10->SetTickx(1);
  cImaging10->SetTicky(1);
  hnpix_c->GetXaxis()->SetTitle("Pixels number in clusters");
  hnpix_c->GetYaxis()->SetTitle("Hits number");
  hnpix_c->Draw();


  TCanvas *cImaging11 = new TCanvas("cImaging11", "Multiplicity: Cluster shape", 1);
  cImaging11->Divide(3,2);
  for (Int_t i_mult=0; i_mult<6; i_mult++) {
    cImaging11->cd(i_mult+1);
    gPad->SetBorderMode(0);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    hdCGDigUVMult[i_mult]->SetAxisRange(-h1ProjectionOnX->GetXaxis()->GetBinWidth(5), h1ProjectionOnX->GetXaxis()->GetBinWidth(5), "X");
    hdCGDigUVMult[i_mult]->SetAxisRange(-h1ProjectionOnX->GetXaxis()->GetBinWidth(5), h1ProjectionOnX->GetXaxis()->GetBinWidth(5), "Y");
    hdCGDigUVMult[i_mult]->Draw("colz");
  }


  //Projection of fit results on X' direction:
  RooPlot *Xprimeframe_all = Xprime->frame(h1ProjectionOnX->GetXaxis()->GetXmin(),
					   h1ProjectionOnX->GetXaxis()->GetXmax(),
					   h1ProjectionOnX->GetXaxis()->GetNbins());
  Xprimeframe_all->SetTitle("X' projection for all multiplicitues");
  TheDataSet->plotOn(Xprimeframe_all);
  TheTotalPdf.plotOn(Xprimeframe_all,RooFit::LineColor(kRed));
  RooHist* hpull_Xprime_all  = Xprimeframe_all->pullHist();
  TheTotalPdf.plotOn(Xprimeframe_all,RooFit::Components(TheNoisePdf), RooFit::LineStyle(kDashed),RooFit::LineColor(kBlue));
  TheTotalPdf.paramOn(Xprimeframe_all,TheDataSet);
  //TheDataSet->statOn(Xprimeframe_all);


  RooPlot *Yprimeframe_all = Yprime->frame(h1ProjectionOnY->GetXaxis()->GetXmin(),
					   h1ProjectionOnY->GetXaxis()->GetXmax(),
					   h1ProjectionOnY->GetXaxis()->GetNbins());
  Yprimeframe_all->SetTitle("X' projection for all multiplicitues");
  TheDataSet->plotOn(Yprimeframe_all);
  TheTotalPdf.plotOn(Yprimeframe_all,RooFit::LineColor(kRed));
  RooHist* hpull_Yprime_all  = Yprimeframe_all->pullHist();
  TheTotalPdf.plotOn(Yprimeframe_all,RooFit::Components(TheNoisePdf), RooFit::LineStyle(kDashed),RooFit::LineColor(kBlue));
  //TheTotalPdf.paramOn(Yprimeframe_all,TheDataSet);
  //TheDataSet->statOn(Yprimeframe_all);

  TCanvas *cImaging12 = new TCanvas("cImaging12", "X' and Y' projection and fit", 1);
  cImaging12->Clear();
  cImaging12->Divide(2,1);
  cImaging12->cd(1);
  TPad* all_Xprime_pad_Up = new TPad("all_Xprime_pad_Up","X' Pad all UP",0.0,Fraction_Pad,1.0,1.0);
  all_Xprime_pad_Up->SetFillColor(10);
  all_Xprime_pad_Up->SetFrameFillColor(10);
  all_Xprime_pad_Up->SetTickx(1);
  all_Xprime_pad_Up->SetTicky(1);
  all_Xprime_pad_Up->SetBottomMargin(0.0);
  all_Xprime_pad_Up->SetLeftMargin(0.15);
  all_Xprime_pad_Up->Draw();
  all_Xprime_pad_Up->cd();
  Xprimeframe_all->GetXaxis()->CenterTitle(true);
  Xprimeframe_all->GetYaxis()->CenterTitle(true);
  Xprimeframe_all->GetXaxis()->SetTitleSize(TheSize);
  Xprimeframe_all->GetXaxis()->SetLabelSize(TheSize);
  Xprimeframe_all->GetYaxis()->SetTitleSize(TheSize);
  Xprimeframe_all->GetYaxis()->SetLabelSize(TheSize);
  Xprimeframe_all->Draw();
  cImaging12->cd(1);
  TPad* all_Xprime_pad_Dn = new TPad("all_Xprime_pad_Dn","X' Pad all DOWN",0.0,0.0,1.0,Fraction_Pad);
  all_Xprime_pad_Dn->SetFillColor(10);
  all_Xprime_pad_Dn->SetFrameFillColor(10);
  all_Xprime_pad_Dn->SetTickx(1);
  all_Xprime_pad_Dn->SetTicky(1);
  all_Xprime_pad_Dn->SetTopMargin(0.0);
  all_Xprime_pad_Dn->SetBottomMargin(TheBottomMargin_Dn);
  all_Xprime_pad_Dn->SetLeftMargin(0.15);
  all_Xprime_pad_Dn->Draw();
  all_Xprime_pad_Dn->cd();
  h1ProjectionOnX_clone->Draw("AXIS");
  l_Xprime_pull_0->Draw();
  l_Xprime_pull_1->Draw();
  l_Xprime_pull_2->Draw();
  hpull_Xprime_all->Draw("P");
  cImaging12->cd(2);
  TPad* all_Yprime_pad_Up = new TPad("all_Yprime_pad_Up","Y' Pad all UP",0.0,Fraction_Pad,1.0,1.0);
  all_Yprime_pad_Up->SetFillColor(10);
  all_Yprime_pad_Up->SetFrameFillColor(10);
  all_Yprime_pad_Up->SetTickx(1);
  all_Yprime_pad_Up->SetTicky(1);
  all_Yprime_pad_Up->SetBottomMargin(0.0);
  all_Yprime_pad_Up->SetLeftMargin(0.15);
  all_Yprime_pad_Up->Draw();
  all_Yprime_pad_Up->cd();
  Yprimeframe_all->GetXaxis()->CenterTitle(true);
  Yprimeframe_all->GetYaxis()->CenterTitle(true);
  Yprimeframe_all->GetXaxis()->SetTitleSize(TheSize);
  Yprimeframe_all->GetXaxis()->SetLabelSize(TheSize);
  Yprimeframe_all->GetYaxis()->SetTitleSize(TheSize);
  Yprimeframe_all->GetYaxis()->SetLabelSize(TheSize);
  Yprimeframe_all->Draw();
  cImaging12->cd(2);
  TPad* all_Yprime_pad_Dn = new TPad("all_Yprime_pad_Dn","Y' Pad all DOWN",0.0,0.0,1.0,Fraction_Pad);
  all_Yprime_pad_Dn->SetFillColor(10);
  all_Yprime_pad_Dn->SetFrameFillColor(10);
  all_Yprime_pad_Dn->SetTickx(1);
  all_Yprime_pad_Dn->SetTicky(1);
  all_Yprime_pad_Dn->SetTopMargin(0.0);
  all_Yprime_pad_Dn->SetBottomMargin(TheBottomMargin_Dn);
  all_Yprime_pad_Dn->SetLeftMargin(0.15);
  all_Yprime_pad_Dn->Draw();
  all_Yprime_pad_Dn->cd();
  h1ProjectionOnY_clone->Draw("AXIS");
  l_Yprime_pull_0->Draw();
  l_Yprime_pull_1->Draw();
  l_Yprime_pull_2->Draw();
  hpull_Yprime_all->Draw("P");


  // **********************************************************************************
  // **************************** SAVE HISTO AND CANVAS *******************************
  // **********************************************************************************
  gSystem->cd(CreateGlobalResultDir());
  if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

  Char_t ResultFileName2[50];
  sprintf(ResultFileName2,"run%dPl%d_Imaging.root",RunNumber,ThePlaneNumber);
  sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2 ));

  TFile *ResultRootFile = new TFile(ResultFileName2,"recreate");

  cImaging1->Write();
  cImaging8->Write();
  cImaging10->Write();
  cImaging11->Write();
  cImaging12->Write();
  cImaging13->Write();
  cImaging14->Write();
  cImaging15->Write();

  resFit_all->Write();

  h2dgoodhits->Write();
  //h1ProjectionOnY->Write();
  //h1ProjectionOnX->Write();
  h2DXprimeVsYprime->Write();
  //h1Sigma->Write();
  //for (Int_t i_mult=0; i_mult<4; i_mult++) h1ProjectionOnYMult[i_mult]->Write();
  //for (Int_t i_mult=0; i_mult<4; i_mult++) h1ProjectionOnXMult[i_mult]->Write();
  //for (Int_t i_mult=0; i_mult<4; i_mult++) h2GoodHitsMult[i_mult]->Write();
  h1NumberOfHitsMult->Write();

  hnpix_c->Write();

  hClusterSize2DInLineAndColumns->Write();
  hClusterSizeInColumns->Write();
  hClusterSizeInLines->Write();

  ResultRootFile->Close();

  if(FirstLoop) {
    sprintf(ResultFileName2,"run%dPl%d_Imaging_ThetaScan.root",RunNumber,ThePlaneNumber);
    sprintf(ResultFileName2,"%s", fTool.LocalizeDirName( ResultFileName2 ));
    TFile *ResultRootFile2 = new TFile(ResultFileName2,"recreate");
    cImaging2->Write();
    ResultRootFile2->Close();
  }
  
  cout << "\n All plots saved in " << ResultRootFile->GetName() << endl;
  delete ResultRootFile;
  //MHist::Dir();

  TString command = TString("rm -f ") + TheOutFileName;
  gSystem->Exec(command.Data());

#endif  
  
}

