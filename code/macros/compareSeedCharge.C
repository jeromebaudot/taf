{
  // Compare the same histo from different "" files

  const Int_t nFiles = 2;

  Char_t fileNames[nFiles][50] ={
          "HitMap_run4182430_nogaincorrection.root",
				  "HitMap_run4182430_withgaincorrection.root"
          };
  Char_t legendText[nFiles][50] ={
           "no correction",
				   "pixel gain correction"
          };

  Char_t *dirName = "Results/4182430/";

  Char_t *histoName = "hhitseedq1"; 

  Short_t colors[nFiles] = { 1, 2, 3, 4};
  Short_t styles[nFiles] = { 2, 1, 1, 2};

  TFile *files = new TFile[nFiles];
  TH1F *histos = new TH1F[nFiles];

  Char_t name[200];
  for( Short_t if=0; if<nFiles; if++) {
    sprintf( name, "%s/%s", dirName, fileNames[if]);
    cout << "getting file " << name << endl;
    files[if] = new TFile( name, "read");
    cout << "getting histo " << if << endl;
    histos[if] = (TH1F*)files[if]->Get(histoName);
    //histos[if]->Sumw2();
    cout << "scaling with " << 1./histos[if]->Integral() << endl;
    histos[if]->Scale( 1./histos[if]->Integral() ); // normalize to 1
    histos[if]->SetTitle("Pixel multiplicity in cluster");
    histos[if]->SetXTitle("# pixels");
    histos[if]->SetLineColor(colors[if]);
    histos[if]->SetLineStyle(styles[if]);
    histos[if]->SetLineWidth(3.);
    histos[if]->SetStats(0);
  }

  TCanvas ccomp("ccomp", "Comparison", 600,600);

  TLegend l(0.55,0.65,0.90,0.90, "Mimosa 26 thresholds:");
  l.SetFillStyle(0);
  l.SetBorderSize(0);
  l.SetTextSize(.04);
  
  // Set the maximum to highest histo
  Float_t max = 0.;
  for( Short_t if=1; if<nFiles; if++) {
    if( histos[if]->GetMaximum() > max ) {
      max = histos[if]->GetMaximum();
    }
  }
  histos[0]->SetMaximum( max*1.2);

  // Then draw all other histos on top
  for( Short_t if=0; if<nFiles; if++) {
    histos[if]->Draw(if?"same":"");
    l.AddEntry( &(histos[if]), legendText[if], "l");
  }
  l.Draw();

}
