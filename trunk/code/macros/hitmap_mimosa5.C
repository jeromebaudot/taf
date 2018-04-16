{

  // build an image from 4 Mimosa 5 matrices
  // assume the data are in the TAF Tree named T
  // The orientation of the sensor is as follows: pad ring on the bottom!

  Bool_t reverseVideo = kFALSE;

  Float_t snSeedCut = 2.;
  Float_t snClusterCut = 1.;
  Float_t snNeighbourCut = 3.;
  Int_t nPixelsCut = 2;
  Char_t additionalCut[200] = "Hn0<150 && Hq0<1000 && HhN<500"; // set to 1 if not needed
 printf("Additional cut: %s\n", additionalCut);

  Int_t nPixelsU = 512/2;
  Int_t nPixelsV = 512/2;
  Float_t pitch = 17.; // um
  Float_t shiftX, shiftY;

  TCanvas *c = new TCanvas("c","Hit map", 1);

  TH2F *hitmap = new TH2F("hitmap", "hit map", 100, -10000, 10000, 100, -10000, 10000);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  if( reverseVideo ) {
    c->SetFillColor(1);
    gStyle->SetMarkerColor(0);
    gStyle->SetLineColor(0);
    hitmap->GetXaxis()->SetLabelColor(0);
    hitmap->GetXaxis()->SetAxisColor(0);
    hitmap->GetXaxis()->SetTitleColor(0);
    hitmap->GetYaxis()->SetLabelColor(0);
    hitmap->GetYaxis()->SetAxisColor(0);
    hitmap->GetYaxis()->SetTitleColor(0);
  }

  hitmap->GetXaxis()->SetLabelSize(.03);
  hitmap->GetYaxis()->SetLabelSize(.03);
  hitmap->SetXTitle("#mum");
  hitmap->SetYTitle("#mum");
  hitmap->Draw();

// cuts
Char_t cutSignal[500], cut[1000], plot[200];
 sprintf( cutSignal, "Hq0/Hn0>%.2f && HSNc>%.2f && HSNneighbour>%.2f && HNNS>%d && %s", snSeedCut, snClusterCut, snNeighbourCut, nPixelsCut, additionalCut);
 printf("Cut signal: %s\n", cutSignal);

// matrix A0
sprintf( cut, "%s && Hpk==1", cutSignal);
shiftX = -nPixelsU*pitch;
shiftY = +nPixelsV*pitch;
sprintf( plot, "Hv+%.1f:Hu+%.1f", shiftX, shiftY);
 cout << "Plotting " << plot << " with " << cut << endl;
Int_t count1 = T->Draw( plot, cut, "same");
  if( reverseVideo ) Graph->SetMarkerColor(0);
 cout << "  " << count1 << " hits" << endl;

// matrix A1
sprintf( cut, "%s && Hpk==2", cutSignal);
shiftX = +nPixelsU*pitch;
shiftY = +nPixelsV*pitch;
sprintf( plot, "Hv+%.1f:Hu+%.1f", shiftX, shiftY);
 cout << "Plotting " << plot << " with " << cut << endl;
Int_t count2 = T->Draw( plot, cut, "same");
  if( reverseVideo ) Graph->SetMarkerColor(0);
 cout << "  " << count2 << " hits" << endl;

// matrix A2
sprintf( cut, "%s && Hpk==3", cutSignal);
shiftX = +nPixelsU*pitch;
shiftY = -nPixelsV*pitch;
sprintf( plot, "Hv+%.1f:Hu+%.1f", shiftX, shiftY);
 cout << "Plotting " << plot << " with " << cut << endl;
Int_t count3 =  T->Draw( plot, cut, "same");
  if( reverseVideo ) Graph->SetMarkerColor(0);
 cout << "  " << count3 << " hits" << endl;

// matrix A3
sprintf( cut, "%s && Hpk==4", cutSignal);
shiftX = -nPixelsU*pitch;
shiftY = -nPixelsV*pitch;
sprintf( plot, "Hv+%.1f:Hu+%.1f", shiftX, shiftY);
 cout << "Plotting " << plot << " with " << cut << endl;
Int_t count4 = T->Draw( plot, cut, "same");
  if( reverseVideo ) Graph->SetMarkerColor(0);
 cout << "  " << count4 << " hits" << endl;

 cout << endl << "Total hits drawn  " << count1+count2+count3+count4 << endl;

}
