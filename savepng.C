#include "TCanvas.h"
#include "TFile.h"
#include "TString.h"
void savepng(	TString resultsPath = "./Results/1",
 		TString pngName = "out_pic"
		)
{

	TFile *file  = new TFile(resultsPath + "/HitMap_run1.root");
	TCanvas *c_test	=  ( (TCanvas*)file -> Get("chitpl1"));
	c_test -> SetCanvasSize(2500, 1500);
	c_test -> Draw();
	c_test -> SetCanvasSize(2500, 1500);
	c_test -> Print(resultsPath + "/" + pngName+".png");
}	
