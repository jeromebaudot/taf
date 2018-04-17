void Fakerate_fromRawdata( const Char_t *fileName, Int_t nEvents, const Char_t *histoName) {
  // This macros computes the fake rate from the histograms (hrdmappl6) produced by 
  // gTAF->GetRaw()->DisplayCumulatedRawData2D()
  // This histo is assumed to have 1 bin per pixel and contains the #hits per pixel.
  //
  // JB 2011/07/04
  
  //------------------------
  // inputs
  
  // enter file and histogram names
  TFile inputFile(fileName,"read");
  TH2F *h2firedpixels = (TH2F*)inputFile.Get(histoName);
  h2firedpixels->Draw("colz");
  
  // enter pitch and matrix size
  Double_t pitch = 20.7; // microns, (M26)18.4, (M28)20.7
  Int_t nPixelsX = 960; // (M26)1152, (M28)960
  Int_t nPixelsY = 928; // (M26)576, (M28)928
  
  // enter geomatrix limits (microns) here
  // format xmin, xmax, ymin, ymax
  const Int_t nGeom = 3;
  Double_t geomatrix[nGeom][4] = {
/* MIMOSA 26 
    {1500,  7500, -1000,  4000},
    {-10000,  10000, -10000,  10000},
*/
    /* MIMOSA 28 */
    {-3000,  3000, -3000,  4000},
    {-3000,  0, -3000,  4000},
    {0,  3000, -3000,  4000},

  };
  
  
  //------------------------
  // histo analysis
  
  Int_t nPixels[nGeom+1] = {0};
  Double_t nFired[nGeom+1] = {0};
  Double_t nHits, x, y;
  Double_t centerX = nPixelsX/2.*pitch;
  Double_t centerY = nPixelsY/2.*pitch;
  
  
  // loop on input histogram bins
  for( Int_t xbin=1; xbin<=h2firedpixels->GetNbinsX(); xbin++ ) {
    for( Int_t ybin=1; ybin<=h2firedpixels->GetNbinsY(); ybin++ ) {

      nHits = h2firedpixels->GetBinContent( xbin, ybin);
      x = xbin*pitch-centerX;
      y = ybin*pitch-centerY;
      
      // whole sensor
      nPixels[nGeom]++;
      if( nHits > 0 ) { //if pixel fired
	nFired[nGeom] += nHits;
      } // end if fired pixels
      
      for( Int_t iGeom=0; iGeom<nGeom; iGeom++) { // loop on geomatrices
	if( geomatrix[iGeom][0]<=x && x<=geomatrix[iGeom][1] && geomatrix[iGeom][2]<=y && y<=geomatrix[iGeom][3] ) {
	  nPixels[iGeom]++;
	  if( nHits > 0 ) { //if pixel fired
	    nFired[iGeom] += nHits;
	  } // end if fired pixels
	}
      } // end loop on geomatrices
      
    }    
  } //end loop on bins
  
  
  //------------------------
  // outputs
  
  cout << "---- Fake rate - Matrix " << nPixelsX << "x" << nPixelsY << " with " << pitch << " um pitch." << endl;
  cout << " " << nEvents << " events" << endl;
  cout << "Whole sensor: (" << nPixels[nGeom] << " pixels)" << endl;
  cout << "    " << nFired[nGeom] << " counts" << endl;
  cout << "    " << nFired[nGeom]/nPixels[nGeom]/nEvents << " +/- " << sqrt(nFired[nGeom])/nPixels[nGeom]/nEvents << endl;
  for( Int_t iGeom=0; iGeom<nGeom; iGeom++) { // loop on geomatrices
    cout << "In: x[" << geomatrix[iGeom][0] << ", " << geomatrix[iGeom][1] << "]; y[" << geomatrix[iGeom][2] << ", " << geomatrix[iGeom][3]<< "] - with " << nPixels[iGeom] << " pixels:" << endl;
    cout << "    " << nFired[iGeom] << " counts" << endl;
    if( nPixels[iGeom]>0 ) {
      cout << "   " << nFired[iGeom]/nPixels[iGeom]/nEvents << " +/- " << sqrt(nFired[iGeom])/nPixels[iGeom]/nEvents << endl;
    }
    else {
      cout << "   no pixels" << endl;
    }
  }  
}