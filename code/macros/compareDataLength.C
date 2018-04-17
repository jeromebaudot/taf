{
  // Compare the same histo from different "" files

  const Int_t nFiles = 4;

  Char_t fileNames[nFiles][50] ={ "26901/PxiStats_50000evts_DataLength_pl3.root",
				  "26509/PxiStats_100000evts_DataLength_pl3.root",
				  "26905/PxiStats_100000evts_DataLength_pl3.root",
				  "26512/PxiStats_100000evts_DataLength_pl3.root"};
  Char_t legendText[nFiles][50] ={ "5 #times noise - no beam",
				   "5 #times noise - beam",
				   "10 #times noise - no beam",
				   "10 #times noise - beam"};

  Char_t *dirName = "Results";

  //Char_t *histoName = "h1DataLength"; 
  Char_t *histoName = "h1PixelOccupancy"; 

  Short_t colors[nFiles] = { 1, 2, 3, 4};
  Short_t styles[nFiles] = { 1, 2, 1, 2};

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
    //histos[if]->SetTitle("Data length per frame");
    //histos[if]->SetXTitle("# 16 bits words");
    histos[if]->SetTitle("Pixel occupancy per frame");
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

  ccomp->SetLogy();
  ccomp->SetGrid(1,1);

}
