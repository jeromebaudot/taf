{
// a.besson march 2006
// .x code/macros/scan.C

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CHECK READOUT = 5 !!!!!!!! -> NON ?
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// refaire reco pour coupure a 6.

// verifier la version run9552_04.root

// fichier config.

//ABSOLUTE PATH dans la macro: CHANGER A 2 ENDROITS !!!!



// MimosaPro(Int_t MaxEvt, Int_t TrackHitDist, Float_t S2N_seed, Float_t S2N_neighbour  , 
//           Int_t submatrix , Int_t GeoMatrix ,Option_t*  SaveAlign,  Int_t UseHitMap)
  Int_t RunChosen = 9552;
  Int_t PlaneChosen = 9; //A0
  Int_t SubmtxChosen = 1; //sub1
  Int_t Nevent_scan = 150000;
  Int_t thdist_true = 60 ;
  
  const Int_t NSeedChargeCut = 7;
  Float_t tab_SeedChargeCut[NSeedChargeCut] = {6,7,8,9,10,11,12};
  Float_t tab_SeedChargeCutERR[NSeedChargeCut] = {0,0,0,0,0,0,0};
  const Int_t NNeighboursChargeCut = 6;
  Float_t tab_NeighboursChargeCut[NNeighboursChargeCut] = {0,3,4,9,13,17};
  
  /*
  const Int_t NSeedChargeCut = 2;
  Float_t tab_SeedChargeCut[NSeedChargeCut] = {8,9};
  Float_t tab_SeedChargeCutERR[NSeedChargeCut] = {0.01,0.01};
  const Int_t NNeighboursChargeCut = 2;
  Float_t tab_NeighboursChargeCut[NNeighboursChargeCut] = {13,17};
  */

  Float_t tab_resolX[NSeedChargeCut][NNeighboursChargeCut];
  Float_t tab_resolY[NSeedChargeCut][NNeighboursChargeCut];
  Float_t tab_eff[NSeedChargeCut][NNeighboursChargeCut];
  Float_t tab_effERR[NSeedChargeCut][NNeighboursChargeCut];
  Float_t tab_fakerate_sub1[NSeedChargeCut][NNeighboursChargeCut];
  Float_t tab_fakerateERR_sub1[NSeedChargeCut][NNeighboursChargeCut];
  Float_t tab_fakerate_sub2[NSeedChargeCut][NNeighboursChargeCut];
  Float_t tab_fakerateERR_sub2[NSeedChargeCut][NNeighboursChargeCut];

  Int_t Ntrack_total_sub1 = 0;
  Int_t Ntrack_total_sub2 = 0;
  

  cout<<"Curent Dir : "<<gSystem->pwd()<<endl; 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// MAIN LOOP
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){	  
      if((iseed==0)&&(ineig==0)){
	gMAF->InitSession(RunChosen,PlaneChosen);
      }
      
      Char_t CorFileName[150];
   //      sprintf(CorFileName,"/rawcmos8/daq/MAF/maf/Results/%d_pl%d_Sub%d_Seed%d_Neig%d_geomat3_T2h%d_HitMap0/Results_%d_%d_1.root",
      sprintf(CorFileName,"/rawcmos9/abesson/maf/Results/%d_pl%d_Sub%d_Seed%d_Neig%d_geomat3_T2h%d_HitMap0/Results_%d_%d_1.root",
	      RunChosen,PlaneChosen,SubmtxChosen,tab_SeedChargeCut[iseed],tab_NeighboursChargeCut[ineig],thdist_true,RunChosen,PlaneChosen);
      if (!gSystem->AccessPathName(CorFileName))
	{
	  cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	  cout<<"             "<<CorFileName<<" IS READ "<<endl;
	  cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	  TFile *CorFileNameResult=new TFile(CorFileName,"read");
	  TNtuple *Ntp = (TNtuple*)CorFileNameResult.Get("Results");
	  Float_t MimosaType;
	  Float_t ChipNumber;
	  Float_t RunNumber;
	  Float_t Temperature;
	  Float_t IrradiationDose;
	  Float_t Angle;
	  Float_t MatrixNumber;
	  Float_t SubMatrixNumber;
	  Float_t AvSN;
	  Float_t MPVSN;
	  Float_t Noise;
	  Float_t NoiseError;
	  Float_t Efficiency;
	  Float_t EfficiencyError;
	  Float_t ResidualU;
	  Float_t ResolutionU;
	  Float_t ResidualV;
	  Float_t ResolutionV;
	  Float_t AvChargeSeed;
	  Float_t AvCharge2x2;
	  Float_t AvCharge3x3;
	  Float_t AvCharge5x5;
	  Float_t MPVChargeSeed;
	  Float_t MPVCharge2x2;
	  Float_t MPVCharge3x3;
	  Float_t MPVCharge5x5;
	  Float_t SN_Seed_CUT;
	  Float_t SN_Neighbours_CUT;
	  Float_t ClusteSize;
	  Float_t Pixelsize;
	  Float_t DiodeSize;
	  Ntp->SetBranchAddress("RunNumber",&RunNumber);
	  Ntp->SetBranchAddress("RunNumber",&RunNumber);
	  Ntp->SetBranchAddress("ResolutionU",&ResolutionU);
	  Ntp->SetBranchAddress("ResolutionV",&ResolutionV);
	  Ntp->SetBranchAddress("Efficiency",&Efficiency);
	  Ntp->SetBranchAddress("EfficiencyError",&EfficiencyError);
	  Ntp->GetEntry(0);
	  
	  tab_eff[iseed][ineig] = Efficiency;
	  tab_effERR[iseed][ineig] = EfficiencyError;
	  tab_resolX[iseed][ineig] = ResolutionU;
	  tab_resolY[iseed][ineig] = ResolutionV;
	  //	  cout<<" TTTTTTTTTTTTTTTTTTTTTTTTTTTTTT "<<Efficiency<<" "<<EfficiencyError<<" "<<ResolutionU<<" "<<ResolutionV<<endl;
	  
	}else
	{   //process mimosapro if the results do not exist.
	  cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	  cout<<"             "<<CorFileName<<" PROCESS MIMOSA PRO "<<endl;
	  cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	  gMAF->MimosaPro(Nevent_scan,thdist_true,tab_SeedChargeCut[iseed],tab_NeighboursChargeCut[ineig],1,3,"no",0);
	  gMAF->MimosaClusterCharge();
	  gMAF->MimosaResolution();
	  gMAF->SaveGifFile();
	  gMAF->ResultsStrore();
	  Float_t *tabresults = gMAF->GettVect();
	  
	  tab_eff[iseed][ineig] = tabresults[12];
	  tab_effERR[iseed][ineig] = tabresults[13];
	  tab_resolX[iseed][ineig] = tabresults[15];
	  tab_resolY[iseed][ineig] = tabresults[17];
	  gMAF->Clear();
	}
      
      //this part only reads results, It doesn't process.
      
      Char_t  ResultFileName[150];
      sprintf(ResultFileName,"/rawcmos9/abesson/maf/Results/%d_pl%d_Sub%d_Seed%d_Neig%d_geomat5_T2h600000_HitMap0/AllPlots_%d_%d.root",
	      RunChosen,PlaneChosen,SubmtxChosen,tab_SeedChargeCut[iseed],tab_NeighboursChargeCut[ineig],RunChosen,PlaneChosen);
      //sprintf(ResultFileName,"AllPlots_%d_%d.root",RunChosen,PlaneChosen);
      //	  cout<<"3Curent Dir : "<<gSystem->pwd()<<endl;
      TFile *ResultRootFile=new TFile(ResultFileName,"read");
      TCanvas *cfake2 = (TCanvas*)ResultRootFile.Get("cfake");
      TPad *mpad1 = (TPad*)cfake2->GetPrimitive("mpad1");
      TPad *mpad2 = (TPad*)cfake2->GetPrimitive("mpad2");
      TPad *mpad3 = (TPad*)cfake2->GetPrimitive("mpad3");
      TPad *mpad4 = (TPad*)cfake2->GetPrimitive("mpad4");

      TCanvas *c4= (TCanvas*)ResultRootFile.Get("c4");
      TPad *mpad2_4 = (TPad*)c4->GetPrimitive("mpad2_4");

      //	  cout<<"4Curent Dir : "<<gSystem->pwd()<<endl;
      
      tab_fakerate_sub1[iseed][ineig] = (((TH1F*)mpad2->GetPrimitive("ADC_sub1_hNhitRateperpixel"))->GetMean());
      tab_fakerate_sub2[iseed][ineig] = (((TH1F*)mpad4->GetPrimitive("ADC_sub2_hNhitRateperpixel"))->GetMean());
      //-------------
      Ntrack_total_sub1 = 0;
      Ntrack_total_sub2 = 0;
      Ntrack_total_sub1 = ((TH1F*)mpad2_4->GetPrimitive("hnGOODhit"))->GetEntries();
      Ntrack_total_sub2 = ((TH1F*)mpad2_4->GetPrimitive("hnGOODhit"))->GetEntries();
      //-------------

      Float_t Fake_Err_sub1 = 0.0;
      Float_t Fake_Err_sub2 = 0.0;
      Float_t Fake_Ntot_sub1 = float(Ntrack_total_sub1 * (((TH1F*)mpad1->GetPrimitive("ADC_sub1_hNhitperpixel"))->GetEntries()) );
      Float_t Fake_Ntot_sub2 = float(Ntrack_total_sub2 * (((TH1F*)mpad3->GetPrimitive("ADC_sub2_hNhitperpixel"))->GetEntries()) ); 
      Float_t Fake_Nent_sub1 = 0.0; //number of reconstructed hits
      Float_t Fake_Nent_sub2 = 0.0;
      Fake_Nent_sub1 = float(((TH1F*)mpad1->GetPrimitive("ADC_sub1_hNhitperpixel"))->GetMean() 
			     * ((TH1F*)mpad1->GetPrimitive("ADC_sub1_hNhitperpixel"))->GetEntries() );
      Fake_Nent_sub2 =  float(((TH1F*)mpad3->GetPrimitive("ADC_sub2_hNhitperpixel"))->GetMean() 
			      * ((TH1F*)mpad3->GetPrimitive("ADC_sub2_hNhitperpixel"))->GetEntries() );
      
      
      if(Ntrack_total_sub1!=0){
	Fake_Err_sub1 = sqrt( ((1.0*Fake_Nent_sub1)/(1.*Fake_Ntot_sub1))* (1.0 - ( (1.0*Fake_Nent_sub1)/(1.*Fake_Ntot_sub1) )  ) / Fake_Ntot_sub1 );
	tab_fakerateERR_sub1[iseed][ineig] = Fake_Err_sub1;
      }else{
	tab_fakerateERR_sub1[iseed][ineig] = 0;
      }
      if(Ntrack_total_sub2!=0){
	Fake_Err_sub2 = sqrt( ((1.0*Fake_Nent_sub2)/(1.*Fake_Ntot_sub2))* (1.0 - ( (1.0*Fake_Nent_sub2)/(1.*Fake_Ntot_sub2) )  ) / Fake_Ntot_sub2 ); 
	tab_fakerateERR_sub2[iseed][ineig] = Fake_Err_sub2;
      }else{
	tab_fakerateERR_sub2[iseed][ineig] = 0;
      }

      //---------------------------------------------------------------------------------------------
      //---------------------------------------------------------------------------------------------





       //---------------------------------------------------------------------------------------------
     //---------------------------------------------------------------------------------------------

      cout<<gMAF->GetNtrkInMimo()<<" ---------> "<<tab_fakerateERR_sub1[iseed][ineig]<<endl;
      //	  cout<<"5Curent Dir : "<<gSystem->pwd()<<endl;
      
      
      ResultRootFile.Close();
      delete ResultRootFile;
      
      
      //	  cout<<"6Curent Dir : "<<gSystem->pwd()<<endl;
      gMAF->Clear();
    } // end for(Int_t iseed=0; iseed<1; iseed++)
  }// end for(Int_t ineig=0; ineig<1; ineig++)
      
  
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // PRINT PART
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){	  
      cout<<tab_SeedChargeCut[iseed]<<" "<<tab_NeighboursChargeCut[ineig]<<" "<<tab_eff[iseed][ineig]<<" "<<tab_effERR[iseed][ineig]
	  <<" "<<tab_resolX[iseed][ineig]<<" "<<tab_resolY[iseed][ineig]
	  <<" "<<tab_fakerate_sub1[iseed][ineig] <<" "<< tab_fakerateERR_sub1[iseed][ineig]
	  <<" "<< tab_fakerate_sub2[iseed][ineig]<<" "<<tab_fakerateERR_sub2[iseed][ineig]
	  <<endl;
    } // end for(Int_t iseed=0; iseed<1; iseed++)
  } // end for(Int_t ineig=0; ineig<1; ineig++)
 
  
  

  
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // DRAW PART
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  ccomp = new TCanvas("ccomp","Mimosa 8 digital",550,10,700,800);
  ccomp->Draw();
  ccomp->Clear();
  ccomp->SetFillColor(0);
  ccomp->SetBorderMode(0);
  ccomp->Divide(2,2);
  
  //------------------------------------------------------------
  // PLOT 1
  //------------------------------------------------------------
   ccomp->cd(1);
  ccomp_1->SetLeftMargin(0.14);
  //gStyle->SetLabelOffset(0.1,"Y");
  gStyle->SetTitleYSize(0.05);
  ccomp_1->SetGridy(0);
  ccomp_1->SetGridx(0);
  
  TH2F *heffFrame = new TH2F("heffFrame","Mimosa 9. Efficiency (%) vs SEED CUT",180,0,18,300,96.2,100.1);
  heffFrame->SetStats(0000000);
  heffFrame->SetXTitle("Seed cut");
  heffFrame->SetYTitle("Efficiency (%)");
  heffFrame->Draw();
  
  Float_t tab_eff_graph[NSeedChargeCut];
  Float_t tab_effERR_graph[NSeedChargeCut];
  Float_t tab_fakerate_sub1_graph[NSeedChargeCut];
  Float_t tab_fakerateERR_sub1_graph[NSeedChargeCut];
  
  Float_t tab_SHIFT_SeedChargeCut[NSeedChargeCut];
  Float_t MarkerSize = 0.5;
  
  //-----------------------------------{0,3,4,9,13,17};
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][0];
    tab_effERR_graph[iseed] = tab_effERR[iseed][0];
    tab_SHIFT_SeedChargeCut[iseed] = tab_SeedChargeCut[iseed];
  }
  cout<<" GRAPH "<<NSeedChargeCut<<" ; "<<tab_SeedChargeCut[0]<<" ; "<<tab_eff_graph[0]<<" ; "<<tab_SeedChargeCutERR[0]<<" ; "<<tab_effERR_graph[0]<<endl;
  gr00 = new TGraphErrors(NSeedChargeCut,tab_SHIFT_SeedChargeCut,tab_eff_graph,tab_SeedChargeCutERR,tab_effERR_graph);
  gr00->SetMarkerColor(4);
  gr00->SetMarkerStyle(21);
  gr00->SetMarkerSize(MarkerSize);
  gr00->SetLineColor(4);
  gr00->Draw("LP");
  //-----------------------------------
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][1];
    tab_effERR_graph[iseed] = tab_effERR[iseed][1];
    tab_SHIFT_SeedChargeCut[iseed] += 0.1;
  } 
  gr03 = new TGraphErrors(NSeedChargeCut,tab_SHIFT_SeedChargeCut,tab_eff_graph,tab_SeedChargeCutERR,tab_effERR_graph);
  gr03->SetMarkerColor(1);
  gr03->SetMarkerStyle(21);
  gr03->SetMarkerSize(MarkerSize);
  gr03->SetLineColor(1);
  gr03->Draw("LP");
  //-----------------------------------
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][2];
    tab_effERR_graph[iseed] = tab_effERR[iseed][2];
    tab_SHIFT_SeedChargeCut[iseed] += 0.1;
  } 
  gr04 = new TGraphErrors(NSeedChargeCut,tab_SHIFT_SeedChargeCut,tab_eff_graph,tab_SeedChargeCutERR,tab_effERR_graph);
  gr04->SetMarkerColor(2);
  gr04->SetMarkerStyle(21);
  gr04->SetMarkerSize(MarkerSize);
  gr04->SetLineColor(2);
  gr04->Draw("LP");
  //-----------------------------------
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][3];
    tab_effERR_graph[iseed] = tab_effERR[iseed][3];
  } 
  gr09 = new TGraphErrors(NSeedChargeCut,tab_SeedChargeCut,tab_eff_graph,tab_SeedChargeCutERR,tab_effERR_graph);
  gr09->SetMarkerColor(3);
  gr09->SetMarkerStyle(21);
  gr09->SetMarkerSize(MarkerSize);
  gr09->SetLineColor(3);
  gr09->Draw("LP");
  //----------------------------------- 
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][4];
    tab_effERR_graph[iseed] = tab_effERR[iseed][4];
  } 
  gr13 = new TGraphErrors(NSeedChargeCut,tab_SeedChargeCut,tab_eff_graph,tab_SeedChargeCutERR,tab_effERR_graph);
  gr13->SetMarkerColor(5);
  gr13->SetMarkerStyle(21);
  gr13->SetMarkerSize(MarkerSize);
  gr13->SetLineColor(5);
  gr13->Draw("LP");
  //-----------------------------------
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][5];
    tab_effERR_graph[iseed] = tab_effERR[iseed][5];
  } 
  gr17 = new TGraphErrors(NSeedChargeCut,tab_SeedChargeCut,tab_eff_graph,tab_SeedChargeCutERR,tab_effERR_graph);
  gr17->SetMarkerColor(6);
  gr17->SetMarkerStyle(21);
  gr17->SetMarkerSize(MarkerSize);
  gr17->SetLineColor(6);
  gr17->Draw("LP");
  
  //------------------------------------------------------------
  // PLOT 2
  //------------------------------------------------------------

  ccomp->cd(2);
  ccomp_2->SetLeftMargin(0.14);
  //gStyle->SetLabelOffset(0.1,"Y");
  gStyle->SetTitleYSize(0.05);
  ccomp_2->SetGridy(0);
  ccomp_2->SetGridx(0);
  //  ccomp_2->SetLogx(1);
  TH2F *heffFrame2 = new TH2F("heffFrame2","Mimosa 9. Efficiency VS Fake",5000,0.000001,0.02,300,96.2,100.1);
  heffFrame2->SetStats(0000000);
  heffFrame2->SetXTitle("Fake ");
  heffFrame2->SetYTitle("Efficiency (%) ");
  heffFrame2->Draw();
  
  //-----------------------------------{0,3,4,9,13,17};
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][0];
    tab_effERR_graph[iseed] = tab_effERR[iseed][0];
    tab_fakerate_sub1_graph[iseed] = tab_fakerate_sub1[iseed][0];
    tab_fakerateERR_sub1_graph[iseed] = tab_fakerateERR_sub1[iseed][0];
  }
  grb00 = new TGraphErrors(NSeedChargeCut,tab_fakerate_sub1_graph,tab_eff_graph,tab_fakerateERR_sub1_graph,tab_effERR_graph);
  grb00->SetMarkerColor(4);
  grb00->SetMarkerStyle(21);
  grb00->SetMarkerSize(MarkerSize);
  grb00->SetLineColor(4);
  grb00->Draw("LP");
  //-----------------------------------
  //-----------------------------------{0,3,4,9,13,17};
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][1];
    tab_effERR_graph[iseed] = tab_effERR[iseed][1];
    tab_fakerate_sub1_graph[iseed] = tab_fakerate_sub1[iseed][1];
    tab_fakerateERR_sub1_graph[iseed] = tab_fakerateERR_sub1[iseed][1];
  }
  grb03 = new TGraphErrors(NSeedChargeCut,tab_fakerate_sub1_graph,tab_eff_graph,tab_fakerateERR_sub1_graph,tab_effERR_graph);
  grb03->SetMarkerColor(1);
  grb03->SetMarkerStyle(21);
  grb03->SetMarkerSize(MarkerSize);
  grb03->SetLineColor(1);
  grb03->Draw("LP");
  //-----------------------------------
  //-----------------------------------{0,3,4,9,13,17};
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][2];
    tab_effERR_graph[iseed] = tab_effERR[iseed][2];
    tab_fakerate_sub1_graph[iseed] = tab_fakerate_sub1[iseed][2];
    tab_fakerateERR_sub1_graph[iseed] = tab_fakerateERR_sub1[iseed][2];
  }
  grb04 = new TGraphErrors(NSeedChargeCut,tab_fakerate_sub1_graph,tab_eff_graph,tab_fakerateERR_sub1_graph,tab_effERR_graph);
  grb04->SetMarkerColor(2);
  grb04->SetMarkerStyle(21);
  grb04->SetMarkerSize(MarkerSize);
  grb04->SetLineColor(2);
  grb04->Draw("LP");
  //-----------------------------------
  //-----------------------------------{0,3,4,9,13,17};
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][3];
    tab_effERR_graph[iseed] = tab_effERR[iseed][3];
    tab_fakerate_sub1_graph[iseed] = tab_fakerate_sub1[iseed][3];
    tab_fakerateERR_sub1_graph[iseed] = tab_fakerateERR_sub1[iseed][3];
  }
  grb09 = new TGraphErrors(NSeedChargeCut,tab_fakerate_sub1_graph,tab_eff_graph,tab_fakerateERR_sub1_graph,tab_effERR_graph);
  grb09->SetMarkerColor(3);
  grb09->SetMarkerStyle(21);
  grb09->SetMarkerSize(MarkerSize);
  grb09->SetLineColor(3);
  grb09->Draw("LP");
  //-----------------------------------
  //-----------------------------------{0,3,4,9,13,17};
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][4];
    tab_effERR_graph[iseed] = tab_effERR[iseed][4];
    tab_fakerate_sub1_graph[iseed] = tab_fakerate_sub1[iseed][4];
    tab_fakerateERR_sub1_graph[iseed] = tab_fakerateERR_sub1[iseed][4];
  }
  grb13 = new TGraphErrors(NSeedChargeCut,tab_fakerate_sub1_graph,tab_eff_graph,tab_fakerateERR_sub1_graph,tab_effERR_graph);
  grb13->SetMarkerColor(5);
  grb13->SetMarkerStyle(21);
  grb13->SetMarkerSize(MarkerSize);
  grb13->SetLineColor(5);
  grb13->Draw("LP");
  //-----------------------------------
  //-----------------------------------{0,3,4,9,13,17};
  for(Int_t iseed=0; iseed<NSeedChargeCut; iseed++){
    tab_eff_graph[iseed] = tab_eff[iseed][5];
    tab_effERR_graph[iseed] = tab_effERR[iseed][5];
    tab_fakerate_sub1_graph[iseed] = tab_fakerate_sub1[iseed][5];
    tab_fakerateERR_sub1_graph[iseed] = tab_fakerateERR_sub1[iseed][5];
  }
  grb17 = new TGraphErrors(NSeedChargeCut,tab_fakerate_sub1_graph,tab_eff_graph,tab_fakerateERR_sub1_graph,tab_effERR_graph);
  grb17->SetMarkerColor(6);
  grb17->SetMarkerStyle(21);
  grb17->SetMarkerSize(MarkerSize);
  grb17->SetLineColor(6);
  grb17->Draw("LP");
  //-----------------------------------
  
  //------------------------------------------------------------
  // PLOT 3
  //------------------------------------------------------------
  ccomp->cd(3);
  ccomp_3->SetLeftMargin(0.14);
  //gStyle->SetLabelOffset(0.1,"Y");
  gStyle->SetTitleYSize(0.05);
  ccomp_3->SetGridy(0);
  ccomp_3->SetGridx(0);
  ccomp_3->SetLogx(1);
  TH2F *heffFrame3 = new TH2F("heffFrame3","Mimosa 9. Efficiency VS Fake",5000,0.000001,0.02,300,96.2,100.1);
  heffFrame2->SetStats(0000000);
  heffFrame2->SetXTitle("Fake rate per pixel ");
  heffFrame2->SetYTitle("Detection efficiency (%) ");
  heffFrame2->Draw();

  Float_t tab_eff_graph_neig[NNeighboursChargeCut];
  Float_t tab_effERR_graph_neig[NNeighboursChargeCut];
  Float_t tab_fakerate_sub1_graph_neig[NNeighboursChargeCut];
  Float_t tab_fakerateERR_sub1_graph_neig[NNeighboursChargeCut];
  Float_t tab_SHIFT_SeedChargeCut_neig[NNeighboursChargeCut];


     
  //-----------------------------------{6,7,8,9,10,11,12}{0,3,4,9,13,17};
  for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){
    tab_eff_graph_neig[ineig] = tab_eff[0][ineig];
    tab_effERR_graph_neig[ineig] = tab_effERR[0][ineig];
    tab_fakerate_sub1_graph_neig[ineig] = tab_fakerate_sub1[0][ineig];
    tab_fakerateERR_sub1_graph_neig[ineig] = tab_fakerateERR_sub1[0][ineig];
  }

  grb06_neig = new TGraphErrors(NNeighboursChargeCut,tab_fakerate_sub1_graph_neig,tab_eff_graph_neig,
				tab_fakerateERR_sub1_graph_neig,tab_effERR_graph_neig);

  grb06_neig->SetMarkerColor(4);
  grb06_neig->SetMarkerStyle(21);
  grb06_neig->SetMarkerSize(MarkerSize);
  grb06_neig->SetLineColor(4);
  grb06_neig->Draw("LP");
  //-----------------------------------
  //-----------------------------------{6,7,8,9,10,11,12}{0,3,4,9,13,17};
  for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){
    tab_eff_graph_neig[ineig] = tab_eff[1][ineig];
    tab_effERR_graph_neig[ineig] = tab_effERR[1][ineig];
    tab_fakerate_sub1_graph_neig[ineig] = tab_fakerate_sub1[1][ineig];
    tab_fakerateERR_sub1_graph_neig[ineig] = tab_fakerateERR_sub1[1][ineig];
  }

  grb07_neig = new TGraphErrors(NNeighboursChargeCut,tab_fakerate_sub1_graph_neig,tab_eff_graph_neig,
				tab_fakerateERR_sub1_graph_neig,tab_effERR_graph_neig);

  grb07_neig->SetMarkerColor(1);
  grb07_neig->SetMarkerStyle(21);
  grb07_neig->SetMarkerSize(MarkerSize);
  grb07_neig->SetLineColor(1);
  grb07_neig->Draw("LP");
  //-----------------------------------
  //-----------------------------------{6,7,8,9,10,11,12}{0,3,4,9,13,17};
  for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){
    tab_eff_graph_neig[ineig] = tab_eff[2][ineig];
    tab_effERR_graph_neig[ineig] = tab_effERR[2][ineig];
    tab_fakerate_sub1_graph_neig[ineig] = tab_fakerate_sub1[2][ineig];
    tab_fakerateERR_sub1_graph_neig[ineig] = tab_fakerateERR_sub1[2][ineig];
  }

  grb08_neig = new TGraphErrors(NNeighboursChargeCut,tab_fakerate_sub1_graph_neig,tab_eff_graph_neig,
				tab_fakerateERR_sub1_graph_neig,tab_effERR_graph_neig);

  grb08_neig->SetMarkerColor(2);
  grb08_neig->SetMarkerStyle(21);
  grb08_neig->SetMarkerSize(MarkerSize);
  grb08_neig->SetLineColor(2);
  grb08_neig->Draw("LP");
  //-----------------------------------
  //-----------------------------------{6,7,8,9,10,11,12}{0,3,4,9,13,17};
  for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){
    tab_eff_graph_neig[ineig] = tab_eff[3][ineig];
    tab_effERR_graph_neig[ineig] = tab_effERR[3][ineig];
    tab_fakerate_sub1_graph_neig[ineig] = tab_fakerate_sub1[3][ineig];
    tab_fakerateERR_sub1_graph_neig[ineig] = tab_fakerateERR_sub1[3][ineig];
  }

  grb09_neig = new TGraphErrors(NNeighboursChargeCut,tab_fakerate_sub1_graph_neig,tab_eff_graph_neig,
				tab_fakerateERR_sub1_graph_neig,tab_effERR_graph_neig);

  grb09_neig->SetMarkerColor(3);
  grb09_neig->SetMarkerStyle(21);
  grb09_neig->SetMarkerSize(MarkerSize);
  grb09_neig->SetLineColor(3);
  grb09_neig->Draw("LP");
  //-----------------------------------
  //-----------------------------------{6,7,8,9,10,11,12}{0,3,4,9,13,17};
  for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){
    tab_eff_graph_neig[ineig] = tab_eff[4][ineig];
    tab_effERR_graph_neig[ineig] = tab_effERR[4][ineig];
    tab_fakerate_sub1_graph_neig[ineig] = tab_fakerate_sub1[4][ineig];
    tab_fakerateERR_sub1_graph_neig[ineig] = tab_fakerateERR_sub1[4][ineig];
  }

  grb10_neig = new TGraphErrors(NNeighboursChargeCut,tab_fakerate_sub1_graph_neig,tab_eff_graph_neig,
				tab_fakerateERR_sub1_graph_neig,tab_effERR_graph_neig);

  grb10_neig->SetMarkerColor(44);
  grb10_neig->SetMarkerStyle(21);
  grb10_neig->SetMarkerSize(MarkerSize);
  grb10_neig->SetLineColor(44);
  grb10_neig->Draw("LP");
  //-----------------------------------
  //-----------------------------------{6,7,8,9,10,11,12}{0,3,4,9,13,17};
  for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){
    tab_eff_graph_neig[ineig] = tab_eff[5][ineig];
    tab_effERR_graph_neig[ineig] = tab_effERR[5][ineig];
    tab_fakerate_sub1_graph_neig[ineig] = tab_fakerate_sub1[5][ineig];
    tab_fakerateERR_sub1_graph_neig[ineig] = tab_fakerateERR_sub1[5][ineig];
  }

  grb11_neig = new TGraphErrors(NNeighboursChargeCut,tab_fakerate_sub1_graph_neig,tab_eff_graph_neig,
				tab_fakerateERR_sub1_graph_neig,tab_effERR_graph_neig);

  grb11_neig->SetMarkerColor(6);
  grb11_neig->SetMarkerStyle(21);
  grb11_neig->SetMarkerSize(MarkerSize);
  grb11_neig->SetLineColor(6);
  grb11_neig->Draw("LP");
  //-----------------------------------
  //-----------------------------------{6,7,8,9,10,11,12}{0,3,4,9,13,17};
  for(Int_t ineig=0; ineig<NNeighboursChargeCut; ineig++){
    tab_eff_graph_neig[ineig] = tab_eff[6][ineig];
    tab_effERR_graph_neig[ineig] = tab_effERR[6][ineig];
    tab_fakerate_sub1_graph_neig[ineig] = tab_fakerate_sub1[6][ineig];
    tab_fakerateERR_sub1_graph_neig[ineig] = tab_fakerateERR_sub1[6][ineig];
  }

  grb12_neig = new TGraphErrors(NNeighboursChargeCut,tab_fakerate_sub1_graph_neig,tab_eff_graph_neig,
				tab_fakerateERR_sub1_graph_neig,tab_effERR_graph_neig);

  grb12_neig->SetMarkerColor(28);
  grb12_neig->SetMarkerStyle(21);
  grb12_neig->SetMarkerSize(MarkerSize);
  grb12_neig->SetLineColor(28);
  grb12_neig->Draw("LP");
  //-----------------------------------




  // 4 1 2 3 44 6 28


leg2 = new TLegend(0.5,0.25,0.85,0.55);
leg2->SetHeader(" Seed Charge Cut (ADC)");
leg2->AddEntry(grb06_neig," Seed > 6","p");
leg2->AddEntry(grb07_neig," Seed > 7","p");
leg2->AddEntry(grb08_neig," Seed > 8","p");
leg2->AddEntry(grb09_neig," Seed > 9","p");
leg2->AddEntry(grb10_neig," Seed > 10","p");
leg2->AddEntry(grb11_neig," Seed > 11","p");
leg2->AddEntry(grb12_neig," Seed > 12","p");

leg2->SetFillStyle(0);
leg2->Draw();





  
}


      /*

 MimosaType      = 9
 ChipNumber      = 3
 RunNumber       = 9552
 Temperature     = 20
 IrradiationDose = 0
 Angle           = 0
 MatrixNumber    = 9
 SubMatrixNumber = 1
 AvSN            = 60.2803
 MPVSN           = 58.2201
 Noise           = 1.5
 NoiseError      = 0
 Efficiency      = 100
 EfficiencyError = 0.692029
 ResidualU       = 1.96546
 ResolutionU     = 1.69205
 ResidualV       = 1.93833
 ResolutionV     = 1.66046
 AvChargeSeed    = 79.7917
 AvCharge2x2     = 162.056
 AvCharge3x3     = 208.09
 AvCharge5x5     = 229.196
 MPVChargeSeed   = 44.1263
 MPVCharge2x2    = 98.4501
 MPVCharge3x3    = 127.035
 MPVCharge5x5    = 135.157
 SN_Seed_CUT     = 12
 SN_Neighbours_CUT = 17
 ClusteSize      = 15
 Pixelsize       = 20
 DiodeSize       = 0

      */
/*
  TFile *f = new TFile("tree1.root");
   TTree *t1 = (TTree*)f->Get("t1");
   Float_t px, py, pz;
   Double_t random;
   Int_t ev;
   t1->SetBranchAddress("px",&px);
   t1->SetBranchAddress("py",&py);
   t1->SetBranchAddress("pz",&pz);
   t1->SetBranchAddress("random",&random);
   t1->SetBranchAddress("ev",&ev);

   //create two histograms
   TH1F *hpx   = new TH1F("hpx","px distribution",100,-3,3);
   TH2F *hpxpy = new TH2F("hpxpy","py vs px",30,-3,3,30,-3,3);

   //read all entries and fill the histograms
   Int_t nentries = (Int_t)t1->GetEntries();
   for (Int_t i=0;i<nentries;i++) {
     t1->GetEntry(i);
     hpx->Fill(px);
     hpxpy->Fill(px,py);
  }

*/
