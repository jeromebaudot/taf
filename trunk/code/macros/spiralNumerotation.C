{
  // Test the algorithm for the geometrical numerotation of pixel
  // in a spiral way
  //
  // JB 2010/07/28

  Int_t matrixWidth = 6;
  Double_t ac, as, dist, angle;
  Int_t n, firstIndex, nofPixels, spiralIndex;
  Double_t pi = TMath::Pi();

  TH2F *f2map = new TH2F("f2map", "Spiral map of index", 2*matrixWidth+1, -matrixWidth-.5, matrixWidth+.5, 2*matrixWidth+1, -matrixWidth-.5, matrixWidth+.5);
  f2map->SetStats(0);

  for( Int_t ix=-matrixWidth; ix<=matrixWidth; ix++) {
    for( Int_t iy=-matrixWidth; iy<=matrixWidth; iy++) {

      // center pixel is a special case
      if( ix==0 && iy==0 ) {spiralIndex=0;}

      // pixels on crowns
      else {

	dist = sqrt( ix*ix + iy*iy );
	n = TMath::Max( abs(ix), abs(iy)); // crown number
	firstIndex = 1+4*n*(n-1); // always odd
	nofPixels = 8*n; // always even
	angle = acos(ix/dist); // 0->pi
	as = asin(iy/dist); // -pi/2->pi/2
	// build an angle from 0->2pi
	if( as<0 ) { angle = 2.*pi-angle; }

        //printf("     dist=%6.1f n=%2d first=%3d pixels=%3d (%5.1f %5.1f) angle=%6.2f i=%6.2f\n", dist, n, firstIndex, nofPixels, ac, as, angle, nofPixels*angle/2./pi);
	spiralIndex = firstIndex + floor(nofPixels*angle/2./pi+0.5); // +.5 is a trick to round to nearest int
      }

      f2map->Fill( ix, iy, spiralIndex);
      printf(" [ %2d, %2d] -> %3d\n", ix, iy, spiralIndex);

    }
  }

  f2map->Draw("text");
  //gPad->SetGrid(1,1);

}
