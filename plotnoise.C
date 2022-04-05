#include "TCanvas.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
void plotnoise(	TString resultsPath = "./Results/1",
 	            	TString pngName = "noise_pic"
		          )
{
 //hnoisepl1
 TH2D* hnoisepl1;
	TFile *file  = new TFile(resultsPath + "/Noise_run1.root");
	hnoisepl1	=  ( (TH2D*)file -> Get("hnoisepl1"));
 TCanvas *c_test = new TCanvas();
 	c_test -> SetCanvasSize(2500, 1500);
 c_test ->Divide(2, 2);
 c_test -> cd(1);
 hnoisepl1 -> Draw("colz");
 
 TH1D* hnoise_AC_amp = new TH1D("hnoise_AC_amp", "hnoise_AC_amp; noise[ADU];#", 1000, 0, 1000 );
 TH1D* hnoise_DC_amp = new TH1D("hnoise_DC_amp", "hnoise_DC_amp; noise[ADU];#", 1000, 0, 1000 );
 TH1D* hnoise_DC_SF = new TH1D("hnoise_DC_SF", "hnoise_DC_SF; noise[ADU];#", 1000, 0, 1000 );
 
		for(int xPix = 0 ; xPix < 64 ; xPix++) {
		for(int yPix = 0 ; yPix < 32 ; yPix++) {
			
		
			if (xPix < 21 ) 		  {hnoise_AC_amp	-> Fill( hnoisepl1 -> GetBinContent(xPix+1, yPix+1) ); 	}
			else if (xPix < 42 ) 	{hnoise_DC_amp	-> Fill( hnoisepl1 -> GetBinContent(xPix+1, yPix+1) );	}
			else 			 	        	{hnoise_DC_SF	  -> Fill( hnoisepl1 -> GetBinContent(xPix+1, yPix+1) );	}
	
			
		}
		}
 c_test -> cd(2);
 hnoise_AC_amp -> Draw("");
  c_test -> cd(3);
 hnoise_DC_amp -> Draw("");
  c_test -> cd(4);
 hnoise_DC_SF -> Draw("");
    
	c_test -> Print(resultsPath + "/" + pngName+".png");
}	
