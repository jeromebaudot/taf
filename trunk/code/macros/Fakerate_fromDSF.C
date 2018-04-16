{
  // This macros computes the fake rate from the DSF file
  // in the folowing conditions:
  // 1) NO BEAM
  //
  // JB 2009/09/21
  // Modified JB 2009/09/30 for additional plots

  Int_t planeNumber = 4;
  Int_t nPixelsU = 1152;
  Int_t nPixelsV = 576;
  Float_t pitch = 18.4;
  Int_t nPixels = nPixelsU*nPixelsV;
  Int_t maxMult = 100;
  Int_t maxPixMult = 10;

  Char_t filename[200];
  sprintf( filename, "datDSF/run26902_01.root");
  TFile DSFile( filename);
  TTree *T = (TTree*)DSFile.Get("T");
  cout << "-- " << filename << " --" << endl;

  Int_t nEvents = T->GetEntries();

  gStyle->SetPalette(1);
  gStyle->SetOptStat(111111);

//   TCanvas cfake("cfake","Fake rate study with NO BEAM -1/2",900,800);
//   cfake.Divide(3,2);

//   TCanvas cfake2("cfake2","Fake rate study with NO BEAM -2/2",900,800);
//   cfake2.Divide(3,2);

  TLine l;
  l.SetLineColor(2);
  l.SetLineWidth(2.);
  TLatex t;
  Char_t string[500];

  //-- fake rate computing
  //cfake.cd(1);
  TH1F hhitmult( "hhitmult", "Hit multiplicity per event", maxMult, 0, maxMult);
  hhitmult.SetFillColor(5);
  hhitmult.SetFillStyle(1001);
  TCanvas cfake1("cfake1",hhitmult.GetTitle(),600,600);

  Char_t cut[500];
  sprintf( cut, "Ppk==%d", planeNumber);
  T->Draw("PhN>>hhitmult",cut);
  hhitmult.Fit("gaus","Q");
  TF1 *ff = (TF1*)hhitmult.GetFunction("gaus");
  ff->SetLineWidth(1.);
  ff->SetLineColor(2);
  ff->Draw("same");
  gStyle->SetOptFit(1);
  gPad->SetLogy();

  const Short_t nquantiles = 5;
  Double_t proba[nquantiles] = {.6827, .95, .9545, .99, .9973};
  Double_t quantile[nquantiles];

  hhitmult.GetQuantiles( nquantiles, quantile, proba);

  Double_t averageFakeRate = hhitmult.GetMean() / (Double_t)nPixels;
  Double_t rmsFakeRate = hhitmult.GetRMS()/sqrt(hhitmult.GetEntries()) / (Double_t)nPixels;

  // draw vertical line at average
  l.DrawLine(averageFakeRate*nPixels, hhitmult.GetMinimum(), averageFakeRate*nPixels, hhitmult.GetMaximum());
  sprintf( string, "average");
  t.SetTextAngle(90.);
  t.SetTextColor(2);
  t.DrawLatex( averageFakeRate*nPixels, (hhitmult.GetMaximum()-hhitmult.GetMinimum())/30., string);

  // draw vertical line at 95% quantile
  l.DrawLine(quantile[1], hhitmult.GetMinimum(), quantile[1], hhitmult.GetMaximum()/20.);
  sprintf( string, "0.95 coverage");
  t.SetTextAngle(90.);
  t.SetTextColor(2);
  t.DrawLatex( quantile[1], (hhitmult.GetMaximum()/2.-hhitmult.GetMinimum())/30., string);


  //cfake.cd(4); // draw without logY option
  hhitmult.Draw();
  // draw vertical line at average
  l.DrawLine(averageFakeRate*nPixels, hhitmult.GetMinimum(), averageFakeRate*nPixels, hhitmult.GetMaximum());
  sprintf( string, "average");
  t.SetTextAngle(90.);
  t.SetTextColor(2);
  t.DrawLatex( averageFakeRate*nPixels, (hhitmult.GetMaximum()-hhitmult.GetMinimum())/3., string);
  // draw vertical line at 95% quantile
  l.DrawLine(quantile[1], hhitmult.GetMinimum(), quantile[1], hhitmult.GetMaximum()/2.);
  sprintf( string, "0.95 coverage");
  t.SetTextAngle(90.);
  t.SetTextColor(2);
  t.DrawLatex( quantile[1], (hhitmult.GetMaximum()/2.-hhitmult.GetMinimum())/3., string);

  Int_t nHits = 0;
  for( Short_t ib=1; ib<=hhitmult.GetNbinsX(); ib++) {
    nHits += hhitmult.GetBinContent(ib) * hhitmult.GetBinLowEdge(ib);
    //cout << ib << " nh=" << nHits << endl;
  } 
  Double_t countingFakeRate = nHits / (Double_t)nEvents / (Double_t)nPixels;
  Double_t poissonUncertainty = sqrt(nHits) / (Double_t)nEvents / (Double_t)nPixels;

  cout << "---- Fake rate - Plane " << planeNumber << " -----" << endl;
  cout << "Computed over " << nEvents << " events and for " << nPixels << " pixels" << endl;
  cout << "average fake rate/pix = " << averageFakeRate << " or " << countingFakeRate << endl;
  cout << "  uncertainty from RMS = " << rmsFakeRate << endl;
  cout << "  uncertainty from Poisson = " << poissonUncertainty << endl;
  cout << "Max rate for a given occurance" << endl;
  for( Short_t iq=0; iq<nquantiles; iq++) {
    cout << quantile[iq]/(Double_t)nPixels << " for " << proba[iq]*100 << " %" << endl;
  }
  cout << endl;
  

  //-- fake rate in geomatrix
  Float_t umin = -1400., umax=+1500.;
  Float_t vmin = -1900., vmax=+500.;
  Int_t nPixelsGeom = ceil( (vmax-vmin)/pitch * (umax-umin)/pitch );

  //cfake.cd(2);
  TH2F hhitmultgeom( "hhitmultgeom", "Hits position in geomatrix", 50, umin, umax, 50, vmin, vmax);
  TCanvas cfake2("cfake2",hhitmultgeom.GetTitle(),600,600);
  hhitmultgeom.SetYTitle("row");
  hhitmultgeom.SetXTitle("column");
  hhitmultgeom.SetStats(0);
  sprintf( cut, "Hpk==%d && %f<=Hu && Hu<=%f && %f<=Hv && Hv<=%f", planeNumber, umin, umax, vmin, vmax);
  T->Draw("Hv:Hu>>hhitmultgeom",cut,"colz");

  Double_t averageFakeRateGeom = hhitmultgeom.GetEntries() / (Double_t)nPixelsGeom / (Double_t)nEvents;
  cout << " in area (" << umin << ", " << umax << ") x (" << vmin << ", " << vmax << ") with " << nPixelsGeom << " pixels" << endl;
  cout << "      average fake rate/pix = " << averageFakeRateGeom << endl;


  //-- fake rate computing per submatrix
  // (Hu/18.4)%1152+576 is the column number

  //cfake.cd(3);
  sprintf( cut, "Hpk==%d", planeNumber);
  TH1F hhitmultsub( "hhitmultsub", "Hit multiplicity per event and per submatrix", 4, 1, 5);
  TCanvas cfake3("cfake3",hhitmultsub.GetTitle(),600,600);
  hhitmultsub.SetXTitle("submatrix number");
  hhitmultsub.SetFillColor(5);
  hhitmultsub.SetFillStyle(1001);
  hhitmultsub.SetStats(0);

  T->Draw("(((Hsu/18.4)%1152+576)/(1152/4))+1>>hhitmultsub",cut);
  hhitmultsub.Scale(1. / (Double_t)nEvents / (Double_t)nPixels * 4);

  for( Int_t isub=0; isub<4; isub++) {
    cout << "Submatrix " << isub+1 << ": average fake rate/pix = " << hhitmultsub->GetBinContent(isub+1) << endl;
  }
  cout << endl;


  //-- fake rate per pixel, line and columns
  // to search for "hot" pixels
  //cfake2.cd(1);
  TH2D hhitmap( "hhitmap", "Hit map", nPixelsU, 0, nPixelsU, nPixelsV, 0, nPixelsV);
  TCanvas cfake4("cfake4",hhitmap.GetTitle(),600,600);
  T->Draw("(Hsv/18.4)%576+288:(Hsu/18.4)%1152+576>>hhitmap", cut, "colz");
  hhitmap.SetXTitle("column number");
  hhitmap.SetYTitle("line number");
  hhitmap.SetStats(0);

  //cfake2.cd(2);
  TH1D *hhitmaplin = hhitmap->ProjectionY();
  TCanvas cfake5("cfake5",hhitmaplin.GetTitle(),600,600);
  hhitmaplin->SetTitle("# hits per event and per lines distribution");
  hhitmaplin->SetXTitle("line number");
  hhitmaplin->Draw();
  hhitmaplin.SetStats(0);
  l.DrawLine(0, averageFakeRate*nEvents*nPixelsU, nPixelsV, averageFakeRate*nEvents*nPixelsU);
  
  //cfake2.cd(4);
  TH1D *hhitmapcol = hhitmap->ProjectionX();
  TCanvas cfake6("cfake6",hhitmapcol.GetTitle(),600,600);
  hhitmapcol->SetTitle("# hits per event and per columns distribution");
  hhitmapcol->SetXTitle("column number");
  hhitmapcol->Draw();
  hhitmapcol.SetStats(0);
  l.DrawLine(0, averageFakeRate*nEvents*nPixelsV, nPixelsU, averageFakeRate*nEvents*nPixelsV);

  TH1F hhitpix( "hhitpix", "Fake rate distribution over pixels", 500, averageFakeRate/100, 1e3*averageFakeRate);
  hhitpix.SetXTitle("Fake rate per pixel");
  hhitpix.SetFillColor(5);
  hhitpix.SetFillStyle(1001);

  const Int_t nbins = 10;
  Double_t lowBinEdge[nbins+1];
  lowBinEdge[0]     = averageFakeRate*1e-1;
  lowBinEdge[nbins] = averageFakeRate*1e+3;
  Double_t logBinWidth = TMath::Log10(lowBinEdge[nbins]/lowBinEdge[0])/nbins;
  for( Short_t ib=1; ib<nbins; ib++) {
    //lowBinEdge[ib] = lowBinEdge[ib-1]+pow(10,ib/10)*averageFakeRate;
    lowBinEdge[ib] = lowBinEdge[ib-1] * TMath::Power(10,logBinWidth);
    //cout << "bin " << ib << " lowEdge = " << lowBinEdge[ib] << ", ";
  }

  TH1F hhitpixv( "hhitpixv", "Fake rate distribution over pixels", nbins, lowBinEdge);
  hhitpixv.SetXTitle("Fake rate per pixel");
  hhitpixv.SetFillColor(5);
  hhitpixv.SetFillStyle(1001);

  Double_t lowBinEdge2[nbins+1];
  for( Short_t ib=0; ib<nbins+1; ib++) {
    lowBinEdge2[ib] = lowBinEdge[ib]/averageFakeRate;
    //cout << "bin " << ib << " lowEdge = " << lowBinEdge2[ib] << ", ";
  }
  TH1F hhitpixcum( "hhitpixcum", "Proportion of pixels above a given fake rate", nbins, lowBinEdge2);
  hhitpixcum.SetXTitle("Fake rate per pixel relative to average rate");
  hhitpixcum.SetLineWidth(2.);
  hhitpixcum.SetStats(0);

  // Fill the histos with special trick to avoid underflow
  for( Int_t ibx=1; ibx<=nPixelsU; ibx++) {
    for( Int_t iby=1; iby<=nPixelsV; iby++) {
      if( hhitmap.GetBinContent( ibx, iby)/(Double_t)nEvents < hhitpix.GetBinLowEdge(1) ) {
	hhitpix.Fill( hhitpix.GetBinCenter(1));
      }
      else {
	hhitpix.Fill( hhitmap.GetBinContent( ibx, iby) / (Double_t)nEvents);
      }
      if( hhitmap.GetBinContent( ibx, iby)/(Double_t)nEvents < hhitpixv.GetBinLowEdge(1) ) {
	hhitpixv.Fill( hhitpixv.GetBinCenter(1) );
      }
      else {
	hhitpixv.Fill( hhitmap.GetBinContent( ibx, iby) / (Double_t)nEvents);
      }
    }
  }
  hhitpixv.Scale( 1./hhitpixv.GetEntries() );

  // Fill the cumulative distribution
  for( Short_t ib=1; ib<=nbins; ib++) {
    for( Short_t jb=1; jb<=ib; jb++) {
      hhitpixcum.AddBinContent( ib, hhitpixv.GetBinContent( jb) ); 
    }
  }

  cout << "average fake rate = " << hhitpix.GetMean() <<  " with RMS over pixels = " << hhitpix.GetRMS() << endl;



  TH1F hhitlin( "hhitlin", "Distribution over lines", 500, averageFakeRate/10, 10*averageFakeRate);
  hhitlin.SetXTitle("Fake rate per pixel");
  hhitlin.SetFillColor(5);
  hhitlin.SetFillStyle(1001);
  for( Int_t ibx=1; ibx<nPixelsU; ibx++) {
    hhitlin.Fill( hhitmaplin.GetBinContent( ibx) / (Double_t)nPixelsU / (Double_t)nEvents);
  }
  cout << "average fake rate = " << hhitlin.GetMean() <<  " with RMS over lines = " << hhitlin.GetRMS() << endl;

  TH1F hhitcol( "hhitcol", "Distribution over columns", 500, averageFakeRate/1000, 10*averageFakeRate);
  hhitcol.SetXTitle("Fake rate per pixel");
  hhitcol.SetFillColor(5);
  hhitcol.SetFillStyle(1001);
  for( Int_t iby=1; iby<nPixelsV; iby++) {
    hhitcol.Fill( hhitmapcol.GetBinContent( iby) / (Double_t)nPixelsV / (Double_t)nEvents);
  }
  cout << "average fake rate = " << hhitcol.GetMean() <<  " with RMS over columns = " << hhitcol.GetRMS() << endl;

  //cfake2.cd(6);
  TCanvas cfake6("cfake6",hhitpixv.GetTitle(),600,600);
  hhitpixv.Draw();
  gPad->SetLogy();
  gPad->SetLogx();
  l.DrawLine(averageFakeRate, hhitpixv.GetMinimum(), averageFakeRate, hhitpixv.GetMaximum());
  sprintf( string, "average");
  t.SetTextAngle(90.);
  t.SetTextColor(2);
  t.DrawLatex( averageFakeRate, hhitpixv.GetMaximum()/30., string);

  //cfake.cd(5);
  TCanvas cfake7("cfake7",hhitpixcum.GetTitle(),600,600);
  hhitpixcum.Draw("L");
  gPad->SetLogx();
  gPad->SetGrid(1,1);

  //cfake2.cd(3);
  TCanvas cfake8("cfake8",hhitlin.GetTitle(),600,600);
  hhitlin.Draw();
  gPad->SetLogy();
  gPad->SetLogx();
  l.DrawLine(averageFakeRate, hhitlin.GetMinimum(), averageFakeRate, hhitlin.GetMaximum());


  //cfake2.cd(5);
  TCanvas cfake9("cfake9",hhitcol.GetTitle(),600,600);
  hhitcol.Draw();
  gPad->SetLogy();
  gPad->SetLogx();
  l.DrawLine(averageFakeRate, hhitcol.GetMinimum(), averageFakeRate, hhitcol.GetMaximum());


  //-- Pixel multiplicity in hits
  //cfake.cd(6);
  TH1F hpixmult( "hpixmult", "Pixel multiplicity in hits", maxPixMult, 0, maxPixMult);
  TCanvas cfake10("cfake10",hpixmult.GetTitle(),600,600);
  hpixmult.SetXTitle("# pixels per hit");
  hpixmult.SetStats(1);

  T->Draw("HNNS>>hpixmult",cut);
  hpixmult.SetFillColor(5);
  hpixmult.SetFillStyle(1001);
  gPad->SetLogy();

  for( Short_t iq=0; iq<nquantiles; iq++) { quantile[iq]=0.; }
  hpixmult.GetQuantiles( nquantiles, quantile, proba);

  Double_t averagePixMult = hpixmult.GetMean();
  Double_t rmsPixMult = hpixmult.GetRMS();

  cout << "---- Pixel multiplicity in hits - Plane " << planeNumber << " -----" << endl;
  cout << "Computed over " << nEvents << " events and for " << nPixels << " pixels" << endl;
  cout << "average pixel multiplicity per hit = " << averagePixMult <<  endl;
  cout << "  uncertainty from RMS = " << rmsPixMult << endl;
  cout << "Max multiplicity for a given occurance" << endl;
  for( Short_t iq=0; iq<nquantiles; iq++) {
    cout << quantile[iq] << " for " << proba[iq]*100 << " %" << endl;
  }
  cout << endl;

}
