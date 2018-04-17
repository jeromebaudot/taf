/***************************************************************************//**
 * SVN File:    $Id$
 *
 * Project:     MLeastChiSquare applications
 *
 * @class:      MLeastChiSquare
 *
 * @brief:      
 *
 * Description:	A macro to estimate tracking performance with MLeastChiSquare class (a Generalized Least Squares implementation)
 *
 *
 * @createdby:  LIU Qingyuan <liuqingyuan678@gmail.com> at 2016-05-20 11:19:00
 * @copyright:  (c)2016 IPHC & HEPG - Shandong University. All Rights Reserved.
 *
 * @lastchange: $Revision$
 *              $Author$
 *              $Date$
 *
 *******************************************************************************/
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include <TH2D.h>
#include <vector>
#include "MTools.C"
#include "MInputs.C"// inputs of this macro.

//_______________________Temporary variables_________________________
// probabilities to be calculated in each layer
Double_t GprobArr[100];
Int_t GnProb = 0;

class MLeastChiSquare;
// get a GLS fitter with estimated track parameters and covariances
MLeastChiSquare getFitter(Double_t mass, Double_t momentum, Int_t nLayers, Double_t layerArr[][kNProperty], Double_t matRatio = 1.0){//matRatio will be multiplied to x/x0 on each layer
     MLeastChiSquare myFitter;
     std::vector<Double_t> *tMeasArr = myFitter.GetPtrMeasArray(); // Array of measurments
     std::vector<Double_t> *tSigmaArr = myFitter.GetPtrSigmaArray(); // Array of layer spatial resolution
     std::vector<Double_t> *tZArr = myFitter.GetPtrZArray(); // Array of the positions of the detection layers
     std::vector<Double_t> *tSigmaMSArr = myFitter.GetPtrSigmaMSArray(); // Array of multiple scattering angle 
     std::vector<Double_t> *tZScatterArr = myFitter.GetPtrZScatterArray(); // Array of layer positions

     for (unsigned i = 0; i < nLayers; i++){
	  tZScatterArr->push_back(layerArr[i][kZPos]);
	  tSigmaMSArr ->push_back(calculateSigmaMS(mass, sqrt(mass*mass+momentum*momentum), layerArr[i][kXOverX0] * matRatio));
	  if(layerArr[i][kTrackingLayer]){
	       tMeasArr ->push_back(0); // dummy hit, no measurements!
	       tSigmaArr->push_back(layerArr[i][kSigmaLayer]);
	       tZArr    ->push_back(layerArr[i][kZPos]);
	  }
     }
     myFitter.Estimate(); // Estimate track parameters
     return myFitter;

}
// seudo-efficiency calculation
Double_t calSeudoEff(MLeastChiSquare& myFitter, Double_t rho1stLayer, Int_t nLayers, Double_t layerArr[][kNProperty], Int_t &nProb = GnProb, Double_t probArr[] = NULL, Int_t debug = 0){
     // Set probArr to store efficiencies on each detecting layer
     // If probArr == NULL, nProb is not used!
     Double_t pseudoEff = 1;
     if(probArr!=NULL) nProb = 0;
     //for (int j = nLayers-1; j >= 0; j--){
     for (int j = 0; j <nLayers; j++){
          if(layerArr[j][kTrackingLayer]){
               Double_t zLayer = layerArr[j][kZPos];
               Double_t resoTracker = myFitter.GetSigmaAtZ(zLayer);
               Double_t resoLayer = layerArr[j][kSigmaLayer];
               Double_t resoCurr = sqrt(resoLayer*resoLayer + resoTracker*resoTracker);
               Double_t rhoCurr = rho1stLayer*(rRef*rRef)/(zLayer*zLayer);
               Double_t prob = probCorrectHit(rhoCurr, resoCurr, resoCurr);
	       if(probArr!=NULL){
		    probArr[nProb] = prob;
                    nProb++;
	       }
               pseudoEff *= prob;

               //probPre = prob;
               if(debug>1){
           	 printf("Check Eff:\n");
           	 printf("Predicted residual width: %f, with noise hit density %e at %f \n", resoCurr, rhoCurr,  zLayer);
           	 //printf("Pseudo-Efficiency: %f\n", pseudoEff);
               }
          }
     }
     return pseudoEff;

}

Double_t testLeastChiSquare(Double_t mass = 0.511e-3/*GeV*/, Double_t momentum = 450e-3/*GeV*/,  Int_t debug = 1, Double_t layerArr[][kNProperty]=Layers_3Pl, Int_t nLayers=nLayers_3Pl, Int_t iDUT = 0, Bool_t checkEff = kFALSE, Double_t rho1stLayer = noiseDens, Double_t matRatio = 1.0){

     Double_t result = 0;

     MLeastChiSquare myFitter = getFitter(mass, momentum, nLayers, layerArr, matRatio);

     if(debug>1) myFitter.Print();

     Double_t resultPar[4] = {1,1,1,1};
     resultPar[0] = myFitter.GetPar()[0];
     resultPar[1] = myFitter.GetPar()[1];
     Double_t sigmaX = myFitter.GetSigmaAtZ(layerArr[iDUT][kZPos]);
     if(checkEff){ // if checkEff, return pseudo efficiency; else return pointing resolution
	  Double_t pseudoEff = calSeudoEff(myFitter, rho1stLayer, nLayers, layerArr, GnProb, GprobArr, debug);// calculate pseudo eff and store good association probability of each layer into the array GprobArr!
	  if(GnProb>3){// 4 layers config
	       // pseudoEff = pseudoEff/GprobArr[0]/GprobArr[1] * (1 - (1-GprobArr[0])*(1-GprobArr[1]));// In this way, pseudoEff is overestimated?
	        Int_t nTBC = 2; // one of the two innermost layers will be disabled to simulate the situation that only 3 hits are found in a 4-layers tracker.
	        Int_t indexTBC[2];
	        Double_t pseudoEffArr[2];
	        // searching for the first 2 detecting layers
	        for(int j = 0; j < nLayers; j++){
	             if(layerArr[j][kTrackingLayer]){
	         	 indexTBC[2-nTBC] = j;
	         	 nTBC--;
	         	 if(nTBC == 0) break;
	             }
	        }
	        for(int j = 0; j < 2; j++){ // loops for the first 2 detecting layers
	             layerArr[(indexTBC[j])][kTrackingLayer] = 0; // disable one of the first two layers for tracking
	             MLeastChiSquare myFitter_tmp = getFitter(mass, momentum, nLayers, layerArr); // refit
	             //myFitter_tmp.Print();
	             pseudoEffArr[j] = calSeudoEff(myFitter_tmp, rho1stLayer, nLayers, layerArr, GnProb, NULL, debug); // calculate pseudo efficiency
		     // GnProb is not used when the the second last parameter is NULL
		     
	             layerArr[indexTBC[j]][kTrackingLayer] = 1; // enable that detecting layer again
	        }
	        pseudoEff = pseudoEff + (1-GprobArr[0])*pseudoEffArr[0] + (1-GprobArr[1])*pseudoEffArr[1];// The pseudo efficiency that at least one good association is required for the first two layers

	  }
	  if(debug){
	       printf("Pseudo-Efficiency: %e\n", pseudoEff);
	  }
	  result = pseudoEff;
     }
     else{
          if(debug){
            printf( "Track parameters: \n");
            printf( "X0: \t%f\nSlope: \t%f\n", resultPar[0], resultPar[1]);
            printf( "Predicted SigmaX(um): %f\n", sigmaX);
          }
	  result = sigmaX;
     }
     return result;
}

TGraph * gRes3Pls = NULL;
TGraph * gRes4Pls = NULL;
TCanvas * myCanvas = NULL;
TLegend * legend = NULL;
Int_t color_3Pls = kRed;//kBlue;
Int_t color_4Pls = kBlue;//kGreen;
void doEnergyScan(Double_t mass = Gmass, Double_t pMin = 200e-3, Double_t pMax = 2.2, Int_t nBins = 100, Int_t tel = 2, Bool_t checkEff = kFALSE, Double_t matRatio = 1.){
     TString optionDraw = "AC";
     if(gRes3Pls == NULL || gRes4Pls == NULL){
     }
     else{
	  optionDraw = "CSame";
     }

     gRes3Pls = new TGraph();
     gRes4Pls = new TGraph();
     TString title, titleY;
     if(checkEff){
	  title = "Pseudo-efficiency vs momentum";
	  titleY = "Pseudo-Efficiency";
	  gRes4Pls->SetMaximum(1.0);
	  gRes3Pls->SetMaximum(1.0);
     }
     else{
	  title = "Predicted resolution VS momentum";
	  titleY = "Resolution[#mum]";
     }

     gRes3Pls->SetTitle(title.Data());
     gRes3Pls->SetMarkerColor(color_3Pls);
     gRes3Pls->SetLineColor(color_3Pls);
     gRes4Pls->SetTitle(title.Data());
     gRes4Pls->SetMarkerColor(color_4Pls);
     gRes4Pls->SetLineColor(color_4Pls);

     Double_t momentum = 0;
     for (Int_t iP = 0; iP < nBins; iP++){
	  momentum = pMin + iP * (pMax-pMin)/(nBins-1);
	  if(tel > 1 || tel == 0)
	       gRes3Pls -> SetPoint(iP, momentum, testLeastChiSquare(mass, momentum, 0, Layers_3Pl, nLayers_3Pl, 0, checkEff, noiseDens, matRatio));
	  if(tel > 1 || tel == 1)
	       gRes4Pls -> SetPoint(iP, momentum, testLeastChiSquare(mass, momentum, 0, Layers_4Pl, nLayers_4Pl, 0, checkEff, noiseDens,  matRatio));
     }

     if(myCanvas==NULL) 
	  myCanvas = new TCanvas("myCanvas","Configs Comparison", 85,85, 800,800);
     myCanvas->SetMargin(0.13,1.,1.,1.);
     if(tel > 1 || tel == 1)
          //gRes4Pls->Draw("Csame");
          gRes4Pls->Draw(optionDraw.Data());
     if(tel > 1 || tel == 0)
	  //gRes3Pls->Draw(optionDraw.Data());
	  gRes3Pls->Draw("Csame");
     myCanvas->Update();
     gRes3Pls->GetXaxis()->SetTitle("Momentum[GeV/c]");
     gRes3Pls->GetYaxis()->SetTitleOffset(1.3);
     gRes3Pls->GetYaxis()->SetTitle(titleY.Data());
     gRes4Pls->GetXaxis()->SetTitle("Momentum[GeV/c]");
     gRes4Pls->GetYaxis()->SetTitleOffset(1.3);
     gRes4Pls->GetYaxis()->SetTitle(titleY.Data());
     if(legend == NULL){
	  legend = new TLegend(0.6,0.75,0.85,0.85);
          legend->AddEntry(gRes3Pls, "3Planes Config", "L");
          legend->AddEntry(gRes4Pls, "4Planes Config", "L");
     }
     legend->Draw();
     if(checkEff){
	  Double_t min = gRes4Pls->GetY()[0];
	  min = TMath::Min(min, gRes3Pls->GetY()[0]);
	  gRes3Pls->SetMinimum(min);
	  gRes4Pls->SetMinimum(min);
	  myCanvas->Update();
	  myCanvas->SetLogx();
     }
}

//Main function 
//_______________________________________________________
//
void cal4BeamTestSim(Double_t mass = Gmass, Double_t p = Gp){
 
     gROOT->ProcessLine(".L MLeastChiSquare.cpp+");// load the GLS method
     printf("\n=============== 3Planes Telescope ===============\n");
     printf("Particle mass %.3e GeV, momentum %.3e GeV\n", mass, p);
     testLeastChiSquare(mass, p);
     testLeastChiSquare(mass, p, 1, Layers_3Pl, nLayers_3Pl, 0, kTRUE);
     printf("\n=============== 4Planes Telescope ===============\n");
     printf("Particle mass %.3e GeV, momentum %.3e GeV\n", mass, p);
     testLeastChiSquare(mass, p, 1, Layers_4Pl, nLayers_4Pl);
     testLeastChiSquare(mass, p, 1, Layers_4Pl, nLayers_4Pl, 0, kTRUE);
     //doEnergyScan(mass);
}

//________________________________________________________________________________
//Int_t GiLayersTBC[] = {7,8,9,10,11,12};
Int_t GiLayersTBC[] = {8,9,10,11,12,13};// layers with radii to be changed
Double_t Gtemp[100];
Int_t GnTBC = sizeof(GiLayersTBC)/sizeof(Int_t);// # of layers to be changed
void doRadiusScan(Int_t iLayersTBC[] = GiLayersTBC, Int_t nTBC = GnTBC, Double_t minus = -12000, Double_t plus = 12000, Int_t nBins = 100, Double_t layerArr[][kNProperty]=Layers_3Pl, const Int_t nLayers = nLayers_3Pl, Bool_t checkEff = kFALSE){

     TH2D *h2 = new TH2D("h2", "h2", nBins, 0.15, 2.2, nBins, minus, plus);
     // store the default layer radii
     Int_t tmpColor = color_3Pls;
     for(Int_t iLayer = 0; iLayer <nTBC; iLayer++){
          Gtemp[iLayer] = layerArr[(iLayersTBC[iLayer])][kZPos];
     }

     // change layer radii from r+minus to r+plus and doEnergyScan()
     for (Int_t i = 0; i < nBins; i++){
	  Double_t z1st;
          for(Int_t iLayer = 0; iLayer <nTBC; iLayer++){
               Double_t z = Gtemp[iLayer] + i* (plus - minus)/(nBins-1) + minus;
	       layerArr[(iLayersTBC[iLayer])][kZPos] = z;
	       if(iLayer == 0) z1st = z;
          }
	  color_3Pls = GetColorFromPalete(z1st, Gtemp[0] + minus, Gtemp[0] + plus, nBins);
          doEnergyScan(Gmass, 0.05, 3, 100/*nBins*/, 2, checkEff);
	  //myCanvas->SaveAs("scan.gif+2");
	  //if(i == nBins-1) myCanvas->SaveAs("scan.gif++200++");// delay 2s for next loop

     }

     // restore the default layer radii
     for(Int_t iLayer = 0; iLayer <nTBC; iLayer++){
          layerArr[(iLayersTBC[iLayer])][kZPos] = Gtemp[iLayer];
     }
     color_3Pls = tmpColor;

}

//________________________________________________________________________________
Double_t ptArr[] = {0.1, 0.2, 1.0}; // GeV // draw eff vs noise density graph for each pt in this array
Int_t nPtNoiseScan = sizeof(ptArr)/sizeof(Double_t);
void doNoiseScan(Double_t lowNoise = noiseDens/2., Double_t highNoise = noiseDens/2.*4, Int_t nBins = 30)
{
     //using namespace std;
     std::vector<TGraph*> graphs3Pls;
     std::vector<TGraph*> graphs4Pls;
     for(Int_t i = 0; i < nPtNoiseScan; i++){// fill graphs
	  TGraph *g3Pls = new TGraph();
	  TGraph *g4Pls = new TGraph();
	  Double_t pt = ptArr[i];
	  for(Int_t j = 0; j< nBins; j++){
	       Double_t tmp_noiseDens = lowNoise + j * (highNoise-lowNoise)/(nBins-1);
	       Double_t tmpNoiseDensPermm2PerSec = tmp_noiseDens*1.e6/timeRes;
	       g3Pls->SetPoint(j, tmpNoiseDensPermm2PerSec, testLeastChiSquare(Gmass, pt, 0, Layers_3Pl, nLayers_3Pl, 0, kTRUE, tmp_noiseDens));
	       g4Pls->SetPoint(j, tmpNoiseDensPermm2PerSec, testLeastChiSquare(Gmass, pt, 0, Layers_4Pl, nLayers_4Pl, 0, kTRUE, tmp_noiseDens));
	       g3Pls->SetMarkerColor(color_3Pls);
	       g4Pls->SetMarkerColor(color_4Pls);
	       g3Pls->SetMarkerStyle(i+2);
	       g4Pls->SetMarkerStyle(i+2);
	  }
	  graphs3Pls.push_back(g3Pls);
	  graphs4Pls.push_back(g4Pls);
     }
     cout << "Predicted Noise density at 1st layer: " << noiseDens*1.e6/timeRes << " Hz/mm^2" << endl;
     
     //draw plots
     TCanvas *cNoiseScan = new TCanvas("cNoiseScan","Noise Scan", 100, 100, 800,800);
     //TLegend *leg = new TLegend(0.75,0.75,0.92,0.92);
     TLegend *leg = new TLegend(0.15,0.15,0.32,0.32);
     TString drawOpt = "AP";
     graphs3Pls[0]->SetMinimum(0.3);
     graphs3Pls[0]->SetMaximum(1.03);

     for(size_t it = 0; it < graphs3Pls.size(); it++){
	  graphs3Pls[it]->Draw(drawOpt.Data());
	  if(it==0) drawOpt = "PSame";
	  graphs4Pls[it]->Draw(drawOpt.Data());
	  char str3pls[100];
	  sprintf(str3pls, "3Pls, %.1f GeV", ptArr[it]); 
	  char str4pls[100];
	  sprintf(str4pls, "4Pls, %.1f GeV", ptArr[it]); 

	  leg->AddEntry((TGraph*)graphs3Pls[it], str3pls, "lp");
	  leg->AddEntry((TGraph*)graphs4Pls[it], str4pls, "lp");
     }
     graphs3Pls[0]->GetXaxis()->SetTitle("Noise Density at 1st layer[ mm^{-2} s^{-1} ]");
     graphs3Pls[0]->GetYaxis()->SetTitleOffset(1.3);
     graphs3Pls[0]->GetYaxis()->SetTitle("Pseudo Efficiency");
     
     leg->Draw();
}

//________________________________________________________________________________
Int_t GlayerIndexMTBC = 2; // Before this layer, material will not be changed!(Keep the same beam pipe!)
Double_t Gtemp_3Pls[100];
Double_t Gtemp_4Pls[100];
TGraph *gInter, *gInter_other = NULL;
TGraph *gInterMatVSP, *gInterMatVSP_other, *gInterShade = NULL;
TGraph *gRes3Pls_first, *gRes4Pls_first = NULL;
//TGraph2D* gInter2D;
void doMaterialScan(Double_t pMin = 0.2, Double_t pMax = 4.5, Double_t nPBins = 100, Double_t mMin = 0.3, Double_t mMax = 1., Int_t nBins = 50){
     // pMin should be large enough to make sure that betta is > 0.9
     // mMin & mMax should be chosen to make sure 1 > X/X0 > 0.001 where the PDG MS equation is accurate.

     Bool_t LowerLimitFound = kFALSE;
     gInter = new TGraph();
     gInter->SetTitle("Material scan");
     gInter->SetMarkerColor(kBlack);
     gInter->SetMarkerStyle(20); // bullet
     gInter->SetMarkerSize(0.8);
     gInterMatVSP = new TGraph();
     gInterMatVSP->SetTitle("Intersection P VS Material budget");
     gInterMatVSP->SetMarkerColor(kBlack);
     gInterMatVSP->SetMarkerStyle(20);
     gInterMatVSP->SetMarkerSize(0.8);
     
     gInter_other = new TGraph();
     gInter_other->SetMarkerColor(kBlack);
     gInter_other->SetMarkerStyle(24);// circle
     gInter_other->SetMarkerSize(0.8);
     gInterMatVSP_other = new TGraph();
     gInterMatVSP_other->SetTitle("Intersection P VS Material budget");
     gInterMatVSP_other->SetMarkerColor(kBlack);
     gInterMatVSP_other->SetMarkerStyle(24);
     gInterMatVSP_other->SetMarkerSize(0.8);
     //gInter2D = new TGraph2D();
     
     // store default material budget
     for(Int_t iLayer = GlayerIndexMTBC; iLayer <nLayers_3Pl; iLayer++){
          Gtemp_3Pls[iLayer] = Layers_3Pl[iLayer][kXOverX0];
     }
     for(Int_t iLayer = GlayerIndexMTBC; iLayer <nLayers_4Pl; iLayer++){
          Gtemp_4Pls[iLayer] = Layers_4Pl[iLayer][kXOverX0];
     }

     // change x/x0 of the tracker layers and doEnergyScan()
     for (Int_t i = 0; i < nBins; i++){
          for(Int_t iLayer = GlayerIndexMTBC; iLayer <nLayers_3Pl; iLayer++){
               Layers_3Pl[iLayer][kXOverX0] = Gtemp_3Pls[iLayer]*(mMin + double(i)/(nBins-1)*(mMax-mMin));
          }
          for(Int_t iLayer = GlayerIndexMTBC; iLayer <nLayers_4Pl; iLayer++){
               Layers_4Pl[iLayer][kXOverX0] = Gtemp_4Pls[iLayer]*(mMin + double(i)/(nBins-1)*(mMax-mMin));
          }

          doEnergyScan(Gmass, pMin, pMax, nPBins);
	  // save the resolution graphs at the first iteration to draw them
	  if (i == 0){
	       gRes3Pls_first = gRes3Pls;
	       gRes4Pls_first = gRes4Pls;
	  }
	  TGraph* tmpInters = GetIntersections(*gRes3Pls, *gRes4Pls);
	  //if(tmpInters->GetN()>0) tmpInters->Draw("Psame");
	  //if(tmpInters->GetN()==2){
	  if(tmpInters->GetN()>0){
	       Double_t x,y;
	       tmpInters->GetPoint(tmpInters->GetN()-1, x, y);
	       gInter->SetPoint(gInter->GetN(), x, y);
	       gInterMatVSP->SetPoint(gInterMatVSP->GetN(), mMin*100 + double(mMax-mMin)*100.*i/(nBins-1), x);
	       if(tmpInters->GetN()==2){
		    LowerLimitFound = kTRUE; // intersections at low Pt range! < 0.4 GeV
		    tmpInters->GetPoint(0, x, y);
	            gInter_other->SetPoint(gInter_other->GetN(), x, y);
	            gInterMatVSP_other->SetPoint(gInterMatVSP_other->GetN(), mMin*100 + double(mMax-mMin)*100.*i/(nBins-1), x);
	       }
	       //gInter2D->SetPoint(gInter2D->GetN(), i/(nBins-1)*(mMax-mMin), x, y);
	       //if (tmpInters->GetN()==1)
	       //    cout << "Momentum with only one intersection: " << x << endl;
	  }
	  //myCanvas->SaveAs("scan.gif+2");
	  //if(i == nBins-1) myCanvas->SaveAs("scan.gif++200++");// delay 2s for next loop

     }
     gInter->Draw("P");
     Int_t nPoints = gInterMatVSP->GetN();
     if(LowerLimitFound){
          gInterMatVSP->SetMinimum(0.);
	  gInter_other->Draw("Psame");
          gInter->SetMaximum(gInter_other->GetY()[0]);
          // Create a shade graph
          gInterShade = new TGraph(2*nPoints);
          for(Int_t i = 0; i < nPoints; i++){
               Double_t x, ymax;
               gInterMatVSP->GetPoint(i, x, ymax);
               gInterShade->SetPoint(i, x, ymax);
               Double_t xAft, ymin;
               gInterMatVSP_other->GetPoint(nPoints-1-i, xAft, ymin);
               gInterShade->SetPoint(nPoints + i, xAft, ymin);
          }
          gInterShade->SetFillStyle(3013);
          gInterShade->SetFillColor(16);
     }
     
     //gInter->Print();

     for(Int_t iLayer = GlayerIndexMTBC; iLayer <nLayers_3Pl; iLayer++){
          Layers_3Pl[iLayer][kXOverX0] = Gtemp_3Pls[iLayer];
     }
     for(Int_t iLayer = GlayerIndexMTBC; iLayer <nLayers_4Pl; iLayer++){
          Layers_4Pl[iLayer][kXOverX0] = Gtemp_4Pls[iLayer];
     }
     TCanvas *c1 = new TCanvas("c1","Material Scan", 95, 95, 1000, 600);
     c1->Divide(2,1);
     c1->cd(1);
     //c1->cd(1)->SetLogx();
     c1->cd(1)->SetLogy();
     gInter->Draw("APC");
     gInter->GetXaxis()->SetTitle("Momentum[GeV/c]");
     if(LowerLimitFound) gInter->GetXaxis()->SetLimits(pMin,gInter->GetX()[nPoints-1]);
     gInter->GetXaxis()->SetRangeUser(pMin,pMax);
     gInter->GetYaxis()->SetTitleOffset(1.3);
     gInter->GetYaxis()->SetTitle("Resolution[#mum]");
     gInter->Draw("APC");
     c1->cd(1)->Update();
     if(LowerLimitFound) {
	  gInter_other->Draw("PCsame");
          legend->AddEntry(gInter_other, "1stInterceptions", "lp");
     }
     gRes3Pls->Draw("PCsame");
     gRes4Pls->Draw("PCsame");
     //gRes3Pls_first->Draw("PCsame");
     //gRes4Pls_first->Draw("PCsame");
     legend->AddEntry(gInter, "Interceptions", "lp");
     legend->Draw();
     c1->cd(2);
     gInterMatVSP->Draw("APC");
     gInterMatVSP->GetXaxis()->SetTitle("Material Budget/default[%]");
     //gInterMatVSP->GetXaxis()->SetLimits(mMin*100, mMin*100);
     //gInterMatVSP->Draw("APC");
     //c1->cd(2)->Update();
     gInterMatVSP->GetYaxis()->SetTitleOffset(1.3);
     gInterMatVSP->GetYaxis()->SetTitle("Intersection Momentum[GeV/c]");
     if(LowerLimitFound){
          gInterMatVSP_other->Draw("PCsame");
          gInterShade->Draw("fsame");
     }
     TText *label3Pl = new TText();
     TText *label4Pl = new TText();
     label3Pl-> SetNDC();
     label4Pl-> SetNDC();
     label3Pl -> SetTextFont(1);
     label3Pl -> SetTextColor(color_3Pls);
     label3Pl -> SetTextSize(0.04);
     label3Pl -> SetTextAlign(22);
     label3Pl -> SetTextAngle(45);
     label3Pl -> DrawText(0.6, 0.4, "3Planes config wins");
     label4Pl -> SetTextFont(1);
     label4Pl -> SetTextColor(color_4Pls);
     label4Pl -> SetTextSize(0.04);
     label4Pl -> SetTextAlign(22);
     label4Pl -> SetTextAngle(45);
     label4Pl -> DrawText(0.35, 0.75, "4Planes config wins");

}
