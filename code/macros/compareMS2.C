{
// a.besson november 2005.
// .x code/macros/compareMS2.C
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const Int_t Nrun = 4;
const Int_t Nsub = 2;

 Int_t RunArray_chip2_freq10[Nrun]               = { 14506, 14500 ,  14502, 14504};
 Int_t ChipArray_chip2_freq10[Nrun]              = {     2,     2 ,      2,     2};
 Float_t TCArray_chip2_freq10[Nrun]              = {    -1,    19 ,     29,    39};
 Float_t TCerrArray_chip2_freq10[Nrun]           = {     0,     0 ,      0,     0};
 Float_t FreqArray_chip2_freq10[Nrun]            = {    10,    10 ,     10,    10};
 Float_t EfficiencyArray_pl9_chip2_freq10[Nrun] =  {99.942,99.949 , 99.984,99.965};
 Float_t EffErrorArray_pl9_chip2_freq10[Nrun]  =  {  0.026,0.016  ,  0.009, 0.014};
 Float_t EfficiencyArray_pl10_chip2_freq10[Nrun]= { 99.975,100.0  , 99.979,99.994};
 Float_t EffErrorArray_pl10_chip2_freq10[Nrun]  = {  0.018,0.00005,  0.011, 0.006};


 Int_t RunArray_chip2_freq2[Nrun]               = { 14507,  14501, 14503, 14505};
 Int_t ChipArray_chip2_freq2[Nrun]              = {     2,      2,     2,     2};
 Float_t TCArray_chip2_freq2[Nrun]              = {    -1,     19,    29,    39};
 Float_t TCerrArray_chip2_freq2[Nrun]           = {     0,      0,     0,     0};
 Float_t FreqArray_chip2_freq2[Nrun]            = {     2,      2,     2,     2};
 Float_t EfficiencyArray_pl9_chip2_freq2[Nrun]  = {99.863, 99.912,99.689,98.451};
 Float_t EffErrorArray_pl9_chip2_freq2[Nrun]  =  {  0.043,  0.013, 0.043, 0.128};
 Float_t EfficiencyArray_pl10_chip2_freq2[Nrun] = {100.00, 99.984,99.994,99.927};
 Float_t EffErrorArray_pl10_chip2_freq2[Nrun]   = {0.0001, 0.006 , 0.006, 0.028};


 Int_t RunArray_chip4_freq10[Nrun]               = { 14514, 14512 ,  14510, 14508};
 Int_t ChipArray_chip4_freq10[Nrun]              = {     4,     4 ,      4,     4};
 Float_t TCArray_chip4_freq10[Nrun]              = {     0,    20 ,     30,    40};
 Float_t TCerrArray_chip4_freq10[Nrun]           = {     0,     0 ,      0,     0};
 Float_t FreqArray_chip4_freq10[Nrun]            = {    10,    10 ,     10,    10};
 Float_t EfficiencyArray_pl9_chip4_freq10[Nrun] =  { 99.946, 99.985, 99.986, 99.974};
 Float_t EffErrorArray_pl9_chip4_freq10[Nrun]  =  {  0.027, 0.015, 0.014,  0.018};
 Float_t EfficiencyArray_pl10_chip4_freq10[Nrun]= {  100.0, 100.0, 99.886, 99.963};
 Float_t EffErrorArray_pl10_chip4_freq10[Nrun]  = {  0.0002, 0.0002, 0.014,  0.022};

 Int_t RunArray_chip4_freq2[Nrun]               = { 14515, 14513 ,  14511, 14509};
 Int_t ChipArray_chip4_freq2[Nrun]              = {     4,     4 ,      4,     4};
 Float_t TCArray_chip4_freq2[Nrun]              = {     0,    20 ,     30,    40};
 Float_t TCerrArray_chip4_freq2[Nrun]           = {     0,     0 ,      0,     0};
 Float_t FreqArray_chip4_freq2[Nrun]            = {     2,     2 ,      2,     2};
 Float_t EfficiencyArray_pl9_chip4_freq2[Nrun] = { 99.933, 99.890, 99.780,  98.725 };
 Float_t EffErrorArray_pl9_chip4_freq2[Nrun]  =  { 0.030, 0.039,  0.053,   0.131 };
 Float_t EfficiencyArray_pl10_chip4_freq2[Nrun]= { 99.949, 99.987,99.975,  99.874};
 Float_t EffErrorArray_pl10_chip4_freq2[Nrun]  = { 0.026, 0.013, 0.017,   0.04};





Float_t SNMPVArray_pl9_chip2_freq10[Nrun];
Float_t SNMPVArray_pl9_chip2_freq2[Nrun];
Float_t SNMPVArray_pl10_chip2_freq10[Nrun];
Float_t SNMPVArray_pl10_chip2_freq2[Nrun];

Float_t NoiseArray_pl9_chip2_freq10[Nrun];
Float_t NoiseArray_pl9_chip2_freq2[Nrun];
Float_t NoiseArray_pl10_chip2_freq10[Nrun];
Float_t NoiseArray_pl10_chip2_freq2[Nrun];

Float_t Ch1Array_pl9_chip2_freq10[Nrun];
Float_t Ch1Array_pl9_chip2_freq2[Nrun];
Float_t Ch1Array_pl10_chip2_freq10[Nrun];
Float_t Ch1Array_pl10_chip2_freq2[Nrun];

Float_t SNMPVArray_pl9_chip4_freq10[Nrun];
Float_t SNMPVArray_pl9_chip4_freq2[Nrun];
Float_t SNMPVArray_pl10_chip4_freq10[Nrun];
Float_t SNMPVArray_pl10_chip4_freq2[Nrun];

Float_t NoiseArray_pl9_chip4_freq10[Nrun];
Float_t NoiseArray_pl9_chip4_freq2[Nrun];
Float_t NoiseArray_pl10_chip4_freq10[Nrun];
Float_t NoiseArray_pl10_chip4_freq2[Nrun];

Float_t Ch1Array_pl9_chip4_freq10[Nrun];
Float_t Ch1Array_pl9_chip4_freq2[Nrun];
Float_t Ch1Array_pl10_chip4_freq10[Nrun];
Float_t Ch1Array_pl10_chip4_freq2[Nrun];

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
TH1F *hM14_Eff_pl9_chip2_freq10[Nrun];
TH1F *hM14_EffError_pl9_chip2_freq10[Nrun];
TH1F *hM14_Eff_pl10_chip2_freq10[Nrun];
TH1F *hM14_EffError_pl10_chip2_freq10[Nrun];
TH1F *hM14_Eff_pl9_chip2_freq2[Nrun];
TH1F *hM14_EffError_pl9_chip2_freq2[Nrun];
TH1F *hM14_Eff_pl10_chip2_freq2[Nrun];
TH1F *hM14_EffError_pl10_chip2_freq2[Nrun];

TH1F *hM14_SNseed_pl9_chip2_freq10[Nrun];
TH1F *hM14_SNseed_pl10_chip2_freq10[Nrun]; 
TH1F *hM14_SNseed_pl9_chip2_freq2[Nrun];
TH1F *hM14_SNseed_pl10_chip2_freq2[Nrun];

TH1F *hM14_Noise_pl9_chip2_freq10[Nrun];
TH1F *hM14_Noise_pl10_chip2_freq10[Nrun];
TH1F *hM14_Noise_pl9_chip2_freq2[Nrun];
TH1F *hM14_Noise_pl10_chip2_freq2[Nrun];

TH1F *hM14_Ch1_pl9_chip2_freq10[Nrun];
TH1F *hM14_Ch1_pl10_chip2_freq10[Nrun];
TH1F *hM14_Ch1_pl9_chip2_freq2[Nrun];
TH1F *hM14_Ch1_pl10_chip2_freq2[Nrun];



TH1F *hM14_Eff_pl9_chip4_freq10[Nrun];
TH1F *hM14_EffError_pl9_chip4_freq10[Nrun];
TH1F *hM14_Eff_pl10_chip4_freq10[Nrun];
TH1F *hM14_EffError_pl10_chip4_freq10[Nrun];
TH1F *hM14_Eff_pl9_chip4_freq2[Nrun];
TH1F *hM14_EffError_pl9_chip4_freq2[Nrun];
TH1F *hM14_Eff_pl10_chip4_freq2[Nrun];
TH1F *hM14_EffError_pl10_chip4_freq2[Nrun];

TH1F *hM14_SNseed_pl9_chip4_freq10[Nrun];
TH1F *hM14_SNseed_pl10_chip4_freq10[Nrun]; 
TH1F *hM14_SNseed_pl9_chip4_freq2[Nrun];
TH1F *hM14_SNseed_pl10_chip4_freq2[Nrun];

TH1F *hM14_Noise_pl9_chip4_freq10[Nrun];
TH1F *hM14_Noise_pl10_chip4_freq10[Nrun];
TH1F *hM14_Noise_pl9_chip4_freq2[Nrun];
TH1F *hM14_Noise_pl10_chip4_freq2[Nrun];

TH1F *hM14_Ch1_pl9_chip4_freq10[Nrun];
TH1F *hM14_Ch1_pl10_chip4_freq10[Nrun];
TH1F *hM14_Ch1_pl9_chip4_freq2[Nrun];
TH1F *hM14_Ch1_pl10_chip4_freq2[Nrun];

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp = new TCanvas("ccomp","MimoSTAR 2",550,10,700,800);
ccomp->Draw();
ccomp->Clear();
ccomp->SetFillColor(0);
ccomp->SetBorderMode(0);
ccomp->Divide(2,2);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(1);
ccomp_1->SetLeftMargin(0.14);
//gStyle->SetLabelOffset(0.1,"Y");
gStyle->SetTitleYSize(0.05);
ccomp_1->SetGridy(0);
ccomp_1->SetGridx(0);

TH2F *heffFrame = new TH2F("heffFrame","Mimostar 2. Efficiency (%) vs Temp. C",43,-5,42,300,98.2,100.1);
heffFrame->SetStats(0000000);
heffFrame->SetXTitle("Temperature. C");
heffFrame->SetYTitle("Efficiency (%)");
heffFrame->Draw();


gr_pl9_chip2_freq10 = new TGraphErrors(Nrun,TCArray_chip2_freq10,EfficiencyArray_pl9_chip2_freq10,
				TCerrArray_chip2_freq10,EffErrorArray_pl9_chip2_freq10);
gr_pl9_chip2_freq10->SetMarkerColor(4);
gr_pl9_chip2_freq10->SetMarkerStyle(21);
gr_pl9_chip2_freq10->SetMarkerSize(0.7);
gr_pl9_chip2_freq10->SetLineColor(4);
gr_pl9_chip2_freq10->Draw("LP");

gr_pl10_chip2_freq10 = new TGraphErrors(Nrun,TCArray_chip2_freq10,EfficiencyArray_pl10_chip2_freq10,
				TCerrArray_chip2_freq10,EffErrorArray_pl10_chip2_freq10);
gr_pl10_chip2_freq10->SetMarkerColor(2);
gr_pl10_chip2_freq10->SetMarkerStyle(21);
gr_pl10_chip2_freq10->SetMarkerSize(0.7);
gr_pl10_chip2_freq10->SetLineColor(2);
gr_pl10_chip2_freq10->Draw("LP");

gr_pl9_chip2_freq2 = new TGraphErrors(Nrun,TCArray_chip2_freq2,EfficiencyArray_pl9_chip2_freq2,
				TCerrArray_chip2_freq2,EffErrorArray_pl9_chip2_freq2);
gr_pl9_chip2_freq2->SetMarkerColor(8);
gr_pl9_chip2_freq2->SetMarkerStyle(21);
gr_pl9_chip2_freq2->SetMarkerSize(0.7);
gr_pl9_chip2_freq2->SetLineColor(8);
gr_pl9_chip2_freq2->Draw("LP");

gr_pl10_chip2_freq2 = new TGraphErrors(Nrun,TCArray_chip2_freq2,EfficiencyArray_pl10_chip2_freq2,
				TCerrArray_chip2_freq2,EffErrorArray_pl10_chip2_freq2);
gr_pl10_chip2_freq2->SetMarkerColor(1);
gr_pl10_chip2_freq2->SetMarkerStyle(21);
gr_pl10_chip2_freq2->SetMarkerSize(0.7);
gr_pl10_chip2_freq2->SetLineColor(1);
gr_pl10_chip2_freq2->Draw("LP");

gr_pl9_chip4_freq10 = new TGraphErrors(Nrun,TCArray_chip4_freq10,EfficiencyArray_pl9_chip4_freq10,
				TCerrArray_chip4_freq10,EffErrorArray_pl9_chip4_freq10);
gr_pl9_chip4_freq10->SetMarkerColor(4);
gr_pl9_chip4_freq10->SetMarkerStyle(22);
gr_pl9_chip4_freq10->SetMarkerSize(0.7);
gr_pl9_chip4_freq10->SetLineColor(4);
gr_pl9_chip4_freq10->Draw("LP");

gr_pl10_chip4_freq10 = new TGraphErrors(Nrun,TCArray_chip4_freq10,EfficiencyArray_pl10_chip4_freq10,
				TCerrArray_chip4_freq10,EffErrorArray_pl10_chip4_freq10);
gr_pl10_chip4_freq10->SetMarkerColor(2);
gr_pl10_chip4_freq10->SetMarkerStyle(22);
gr_pl10_chip4_freq10->SetMarkerSize(0.7);
gr_pl10_chip4_freq10->SetLineColor(2);
gr_pl10_chip4_freq10->Draw("LP");

gr_pl9_chip4_freq2 = new TGraphErrors(Nrun,TCArray_chip4_freq2,EfficiencyArray_pl9_chip4_freq2,
				TCerrArray_chip4_freq2,EffErrorArray_pl9_chip4_freq2);
gr_pl9_chip4_freq2->SetMarkerColor(8);
gr_pl9_chip4_freq2->SetMarkerStyle(22);
gr_pl9_chip4_freq2->SetMarkerSize(0.7);
gr_pl9_chip4_freq2->SetLineColor(8);
gr_pl9_chip4_freq2->Draw("LP");

gr_pl10_chip4_freq2 = new TGraphErrors(Nrun,TCArray_chip4_freq2,EfficiencyArray_pl10_chip4_freq2,
				TCerrArray_chip4_freq2,EffErrorArray_pl10_chip4_freq2);
gr_pl10_chip4_freq2->SetMarkerColor(1);
gr_pl10_chip4_freq2->SetMarkerStyle(22);
gr_pl10_chip4_freq2->SetMarkerSize(0.7);
gr_pl10_chip4_freq2->SetLineColor(1);
gr_pl10_chip4_freq2->Draw("LP");

leg1 = new TLegend(0.18,0.25,0.5,0.7);
leg1->SetHeader(" Chip / Matrix / Freq.");
leg1->AddEntry(gr_pl9_chip2_freq10, "Chip 2: Rad. Tol. 10 MHz","p");
leg1->AddEntry(gr_pl10_chip2_freq10,"Chip 2: Standard. 10 MHz","p");
leg1->AddEntry(gr_pl9_chip2_freq2 , "Chip 2: Rad. Tol.  2 MHz","p");
leg1->AddEntry(gr_pl10_chip2_freq2 ,"Chip 2: Standard.  2 MHz","p");
leg1->AddEntry(gr_pl9_chip4_freq10, "Chip 4: Rad. Tol. 10 MHz","p");
leg1->AddEntry(gr_pl10_chip4_freq10,"Chip 4: Standard. 10 MHz","p");
leg1->AddEntry(gr_pl9_chip4_freq2 , "Chip 4: Rad. Tol.  2 MHz","p");
leg1->AddEntry(gr_pl10_chip4_freq2 ,"Chip 4: Standard.  2 MHz","p");

leg1->SetFillStyle(0);
leg1->Draw();


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(2);
ccomp_2->SetLeftMargin(0.14);
ccomp_2->SetGridy(0);
ccomp_2->SetGridx(0);


TH2F *heffFrame2 = new TH2F("heffFrame2","Mimostar 2. S/N (MPV) vs Temp. C",43,-5,42,100,8,25);
heffFrame2->SetStats(0000000);
heffFrame2->SetXTitle("Temperature. C");
heffFrame2->SetYTitle("S/N (MPV)");
heffFrame2->Draw();

 for (Int_t i=0 ; i<Nrun ; i++){

   //chip2

   Char_t  ResultFileName[150];
   sprintf(ResultFileName,"Results/%d_pl9_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_9.root",
	   RunArray_chip2_freq10[i],RunArray_chip2_freq10[i]);
   TFile *ResultRootFile=new TFile(ResultFileName,"read");
   TCanvas *MainCanvas = (TCanvas*)ResultRootFile.Get("MainCanvas");
   TPad *mpad3 = (TPad*)MainCanvas->GetPrimitive("mpad3");
   hM14_SNseed_pl9_chip2_freq10[i]= (TH1F*)mpad3->GetPrimitive("hsnc");
   TPad *mpad4 = (TPad*)MainCanvas->GetPrimitive("mpad4");
   hM14_Noise_pl9_chip2_freq10[i]= (TH1F*)mpad4->GetPrimitive("hRTN");
   TPad *mpad6 = (TPad*)MainCanvas->GetPrimitive("mpad6");
   hM14_Ch1_pl9_chip2_freq10[i]= (TH1F*)mpad6->GetPrimitive("hqc1");
   Ch1Array_pl9_chip2_freq10[i] =
     (((TH1F*)mpad6->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
   cout<<"pl 9 chip2 freq 10 "<<(((TH1F*)mpad3->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
   SNMPVArray_pl9_chip2_freq10[i]= 
     (((TH1F*)mpad3->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl9_chip2_freq10[i] =hM14_Noise_pl9_chip2_freq10[i]->GetMean();


   Char_t  ResultFileName2[150];
   sprintf(ResultFileName2,"Results/%d_pl10_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_10.root",
	   RunArray_chip2_freq10[i],RunArray_chip2_freq10[i]);
  TFile *ResultRootFile2=new TFile(ResultFileName2,"read");
  TCanvas *MainCanvas2 = (TCanvas*)ResultRootFile2.Get("MainCanvas");
  TPad *mpad32 = (TPad*)MainCanvas2->GetPrimitive("mpad3");
  hM14_SNseed_pl10_chip2_freq10[i]= (TH1F*)mpad32->GetPrimitive("hsnc");
  TPad *mpad42 = (TPad*)MainCanvas2->GetPrimitive("mpad4");
  hM14_Noise_pl10_chip2_freq10[i]= (TH1F*)mpad42->GetPrimitive("hRTN");
  TPad *mpad62 = (TPad*)MainCanvas2->GetPrimitive("mpad6");
  hM14_Ch1_pl10_chip2_freq10[i]= (TH1F*)mpad62->GetPrimitive("hqc1");
  Ch1Array_pl10_chip2_freq10[i] =
    (((TH1F*)mpad62->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
  cout<<"pl 10 chip2 freq 10 "<<(((TH1F*)mpad32->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
  SNMPVArray_pl10_chip2_freq10[i]= 
    (((TH1F*)mpad32->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl10_chip2_freq10[i] =hM14_Noise_pl10_chip2_freq10[i]->GetMean();


   Char_t  ResultFileName3[150];
   sprintf(ResultFileName3,"Results/%d_pl9_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_9.root",
	   RunArray_chip2_freq2[i],RunArray_chip2_freq2[i]);
  TFile *ResultRootFile3=new TFile(ResultFileName3,"read");
  TCanvas *MainCanvas3 = (TCanvas*)ResultRootFile3.Get("MainCanvas");
  TPad *mpad33 = (TPad*)MainCanvas3->GetPrimitive("mpad3");
  hM14_SNseed_pl9_chip2_freq2[i]= (TH1F*)mpad33->GetPrimitive("hsnc");
  TPad *mpad43 = (TPad*)MainCanvas3->GetPrimitive("mpad4");
  hM14_Noise_pl9_chip2_freq2[i]= (TH1F*)mpad43->GetPrimitive("hRTN");
  TPad *mpad63 = (TPad*)MainCanvas3->GetPrimitive("mpad6");
  hM14_Ch1_pl9_chip2_freq2[i]= (TH1F*)mpad63->GetPrimitive("hqc1");
  Ch1Array_pl9_chip2_freq2[i] =
    (((TH1F*)mpad63->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
  cout<<"pl 9 chip2 freq 2 "<<(((TH1F*)mpad33->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
  SNMPVArray_pl9_chip2_freq2[i]= 
    (((TH1F*)mpad33->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl9_chip2_freq2[i] =hM14_Noise_pl9_chip2_freq2[i]->GetMean();



   Char_t  ResultFileName4[150];
   sprintf(ResultFileName4,"Results/%d_pl10_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_10.root",
	   RunArray_chip2_freq2[i],RunArray_chip2_freq2[i]);
  TFile *ResultRootFile4=new TFile(ResultFileName4,"read");
  TCanvas *MainCanvas4 = (TCanvas*)ResultRootFile4.Get("MainCanvas");
  TPad *mpad34 = (TPad*)MainCanvas4->GetPrimitive("mpad3");
  hM14_SNseed_pl10_chip2_freq2[i]= (TH1F*)mpad34->GetPrimitive("hsnc");
  TPad *mpad44 = (TPad*)MainCanvas4->GetPrimitive("mpad4");
  hM14_Noise_pl10_chip2_freq2[i]= (TH1F*)mpad44->GetPrimitive("hRTN");
  TPad *mpad64 = (TPad*)MainCanvas4->GetPrimitive("mpad6");
  hM14_Ch1_pl10_chip2_freq2[i]= (TH1F*)mpad64->GetPrimitive("hqc1");
  Ch1Array_pl10_chip2_freq2[i] =
    (((TH1F*)mpad64->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
  cout<<"pl 10 chip2 freq 2 "<<(((TH1F*)mpad34->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
  SNMPVArray_pl10_chip2_freq2[i]= 
    (((TH1F*)mpad34->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl10_chip2_freq2[i] =hM14_Noise_pl10_chip2_freq2[i]->GetMean();

    ResultRootFile.Close();
    delete ResultRootFile;
    ResultRootFile2.Close();
    delete ResultRootFile2;
    ResultRootFile3.Close();
    delete ResultRootFile3;
    ResultRootFile4.Close();
    delete ResultRootFile4;

   // chip4

   Char_t  ResultFileName[150];
   sprintf(ResultFileName,"Results/%d_pl9_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_9.root",
	   RunArray_chip4_freq10[i],RunArray_chip4_freq10[i]);
   TFile *ResultRootFile=new TFile(ResultFileName,"read");
   TCanvas *MainCanvas = (TCanvas*)ResultRootFile.Get("MainCanvas");
   TPad *mpad3 = (TPad*)MainCanvas->GetPrimitive("mpad3");
   hM14_SNseed_pl9_chip4_freq10[i]= (TH1F*)mpad3->GetPrimitive("hsnc");
   TPad *mpad4 = (TPad*)MainCanvas->GetPrimitive("mpad4");
   hM14_Noise_pl9_chip4_freq10[i]= (TH1F*)mpad4->GetPrimitive("hRTN");
   TPad *mpad6 = (TPad*)MainCanvas->GetPrimitive("mpad6");
   hM14_Ch1_pl9_chip4_freq10[i]= (TH1F*)mpad6->GetPrimitive("hqc1");
   Ch1Array_pl9_chip4_freq10[i] =
     (((TH1F*)mpad6->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
   cout<<"pl 9 chip4 freq 10 "<<(((TH1F*)mpad3->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
   SNMPVArray_pl9_chip4_freq10[i]= 
     (((TH1F*)mpad3->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl9_chip4_freq10[i] =hM14_Noise_pl9_chip4_freq10[i]->GetMean();


   Char_t  ResultFileName2[150];
   sprintf(ResultFileName2,"Results/%d_pl10_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_10.root",
	   RunArray_chip4_freq10[i],RunArray_chip4_freq10[i]);
  TFile *ResultRootFile2=new TFile(ResultFileName2,"read");
  TCanvas *MainCanvas2 = (TCanvas*)ResultRootFile2.Get("MainCanvas");
  TPad *mpad32 = (TPad*)MainCanvas2->GetPrimitive("mpad3");
  hM14_SNseed_pl10_chip4_freq10[i]= (TH1F*)mpad32->GetPrimitive("hsnc");
  TPad *mpad42 = (TPad*)MainCanvas2->GetPrimitive("mpad4");
  hM14_Noise_pl10_chip4_freq10[i]= (TH1F*)mpad42->GetPrimitive("hRTN");
  TPad *mpad62 = (TPad*)MainCanvas2->GetPrimitive("mpad6");
  hM14_Ch1_pl10_chip4_freq10[i]= (TH1F*)mpad62->GetPrimitive("hqc1");
  Ch1Array_pl10_chip4_freq10[i] =
    (((TH1F*)mpad62->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
  cout<<"pl 10 chip4 freq 10 "<<(((TH1F*)mpad32->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
  SNMPVArray_pl10_chip4_freq10[i]= 
    (((TH1F*)mpad32->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl10_chip4_freq10[i] =hM14_Noise_pl10_chip4_freq10[i]->GetMean();


   Char_t  ResultFileName3[150];
   sprintf(ResultFileName3,"Results/%d_pl9_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_9.root",
	   RunArray_chip4_freq2[i],RunArray_chip4_freq2[i]);
  TFile *ResultRootFile3=new TFile(ResultFileName3,"read");
  TCanvas *MainCanvas3 = (TCanvas*)ResultRootFile3.Get("MainCanvas");
  TPad *mpad33 = (TPad*)MainCanvas3->GetPrimitive("mpad3");
  hM14_SNseed_pl9_chip4_freq2[i]= (TH1F*)mpad33->GetPrimitive("hsnc");
  TPad *mpad43 = (TPad*)MainCanvas3->GetPrimitive("mpad4");
  hM14_Noise_pl9_chip4_freq2[i]= (TH1F*)mpad43->GetPrimitive("hRTN");
  TPad *mpad63 = (TPad*)MainCanvas3->GetPrimitive("mpad6");
  hM14_Ch1_pl9_chip4_freq2[i]= (TH1F*)mpad63->GetPrimitive("hqc1");
  Ch1Array_pl9_chip4_freq2[i] =
    (((TH1F*)mpad63->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
  cout<<"pl 9 chip4 freq 2 "<<(((TH1F*)mpad33->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
  SNMPVArray_pl9_chip4_freq2[i]= 
    (((TH1F*)mpad33->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl9_chip4_freq2[i] =hM14_Noise_pl9_chip4_freq2[i]->GetMean();



   Char_t  ResultFileName4[150];
   sprintf(ResultFileName4,"Results/%d_pl10_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_10.root",
	   RunArray_chip4_freq2[i],RunArray_chip4_freq2[i]);
  TFile *ResultRootFile4=new TFile(ResultFileName4,"read");
  TCanvas *MainCanvas4 = (TCanvas*)ResultRootFile4.Get("MainCanvas");
  TPad *mpad34 = (TPad*)MainCanvas4->GetPrimitive("mpad3");
  hM14_SNseed_pl10_chip4_freq2[i]= (TH1F*)mpad34->GetPrimitive("hsnc");
  TPad *mpad44 = (TPad*)MainCanvas4->GetPrimitive("mpad4");
  hM14_Noise_pl10_chip4_freq2[i]= (TH1F*)mpad44->GetPrimitive("hRTN");
  TPad *mpad64 = (TPad*)MainCanvas4->GetPrimitive("mpad6");
  hM14_Ch1_pl10_chip4_freq2[i]= (TH1F*)mpad64->GetPrimitive("hqc1");
  Ch1Array_pl10_chip4_freq2[i] =
    (((TH1F*)mpad64->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
  cout<<"pl 10 chip4 freq 2 "<<(((TH1F*)mpad34->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1)<<endl;
  SNMPVArray_pl10_chip4_freq2[i]= 
    (((TH1F*)mpad34->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_pl10_chip4_freq2[i] =hM14_Noise_pl10_chip4_freq2[i]->GetMean();







 
    ResultRootFile.Close();
    delete ResultRootFile;
    ResultRootFile2.Close();
    delete ResultRootFile2;
    ResultRootFile3.Close();
    delete ResultRootFile3;
    ResultRootFile4.Close();
    delete ResultRootFile4;

 }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ccomp->cd(2);
grSNMPV_pl9_chip2_freq10  = new TGraphErrors(Nrun,TCArray_chip2_freq10,SNMPVArray_pl9_chip2_freq10,
					     TCerrArray_chip2_freq10,TCerrArray_chip2_freq10);
grSNMPV_pl9_chip2_freq10->SetMarkerColor(4);
grSNMPV_pl9_chip2_freq10->SetMarkerStyle(21);
grSNMPV_pl9_chip2_freq10->SetMarkerSize(0.7);
grSNMPV_pl9_chip2_freq10->SetLineColor(4);
grSNMPV_pl9_chip2_freq10->Draw("LP");

grSNMPV_pl10_chip2_freq10  = new TGraphErrors(Nrun,TCArray_chip2_freq10,SNMPVArray_pl10_chip2_freq10,
					     TCerrArray_chip2_freq10,TCerrArray_chip2_freq10);
grSNMPV_pl10_chip2_freq10->SetMarkerColor(2);
grSNMPV_pl10_chip2_freq10->SetMarkerStyle(21);
grSNMPV_pl10_chip2_freq10->SetMarkerSize(0.7);
grSNMPV_pl10_chip2_freq10->SetLineColor(2);
grSNMPV_pl10_chip2_freq10->Draw("LP");

grSNMPV_pl9_chip2_freq2  = new TGraphErrors(Nrun,TCArray_chip2_freq2,SNMPVArray_pl9_chip2_freq2,
					     TCerrArray_chip2_freq2,TCerrArray_chip2_freq2);
grSNMPV_pl9_chip2_freq2->SetMarkerColor(8);
grSNMPV_pl9_chip2_freq2->SetMarkerStyle(21);
grSNMPV_pl9_chip2_freq2->SetMarkerSize(0.7);
grSNMPV_pl9_chip2_freq2->SetLineColor(8);
grSNMPV_pl9_chip2_freq2->Draw("LP");

grSNMPV_pl10_chip2_freq2  = new TGraphErrors(Nrun,TCArray_chip2_freq2,SNMPVArray_pl10_chip2_freq2,
					     TCerrArray_chip2_freq2,TCerrArray_chip2_freq2);
grSNMPV_pl10_chip2_freq2->SetMarkerColor(1);
grSNMPV_pl10_chip2_freq2->SetMarkerStyle(21);
grSNMPV_pl10_chip2_freq2->SetMarkerSize(0.7);
grSNMPV_pl10_chip2_freq2->SetLineColor(1);
grSNMPV_pl10_chip2_freq2->Draw("LP");


grSNMPV_pl9_chip4_freq10  = new TGraphErrors(Nrun,TCArray_chip4_freq10,SNMPVArray_pl9_chip4_freq10,
					     TCerrArray_chip4_freq10,TCerrArray_chip4_freq10);
grSNMPV_pl9_chip4_freq10->SetMarkerColor(4);
grSNMPV_pl9_chip4_freq10->SetMarkerStyle(22);
grSNMPV_pl9_chip4_freq10->SetMarkerSize(0.7);
grSNMPV_pl9_chip4_freq10->SetLineColor(4);
grSNMPV_pl9_chip4_freq10->Draw("LP");

grSNMPV_pl10_chip4_freq10  = new TGraphErrors(Nrun,TCArray_chip4_freq10,SNMPVArray_pl10_chip4_freq10,
					     TCerrArray_chip4_freq10,TCerrArray_chip4_freq10);
grSNMPV_pl10_chip4_freq10->SetMarkerColor(2);
grSNMPV_pl10_chip4_freq10->SetMarkerStyle(22);
grSNMPV_pl10_chip4_freq10->SetMarkerSize(0.7);
grSNMPV_pl10_chip4_freq10->SetLineColor(2);
grSNMPV_pl10_chip4_freq10->Draw("LP");

grSNMPV_pl9_chip4_freq2  = new TGraphErrors(Nrun,TCArray_chip4_freq2,SNMPVArray_pl9_chip4_freq2,
					     TCerrArray_chip4_freq2,TCerrArray_chip4_freq2);
grSNMPV_pl9_chip4_freq2->SetMarkerColor(8);
grSNMPV_pl9_chip4_freq2->SetMarkerStyle(22);
grSNMPV_pl9_chip4_freq2->SetMarkerSize(0.7);
grSNMPV_pl9_chip4_freq2->SetLineColor(8);
grSNMPV_pl9_chip4_freq2->Draw("LP");

grSNMPV_pl10_chip4_freq2  = new TGraphErrors(Nrun,TCArray_chip4_freq2,SNMPVArray_pl10_chip4_freq2,
					     TCerrArray_chip4_freq2,TCerrArray_chip4_freq2);
grSNMPV_pl10_chip4_freq2->SetMarkerColor(1);
grSNMPV_pl10_chip4_freq2->SetMarkerStyle(22);
grSNMPV_pl10_chip4_freq2->SetMarkerSize(0.7);
grSNMPV_pl10_chip4_freq2->SetLineColor(1);
grSNMPV_pl10_chip4_freq2->Draw("LP");



leg2 = new TLegend(0.18,0.25,0.5,0.55);
leg2->SetHeader(" Chip / Matrix / Freq.");
leg2->AddEntry(grSNMPV_pl9_chip2_freq10, "Chip 2: Rad. Tol. 10 MHz","p");
leg2->AddEntry(grSNMPV_pl10_chip2_freq10 ,"Chip 2: Standard. 10 MHz","p");
leg2->AddEntry(grSNMPV_pl9_chip2_freq2 , "Chip 2: Rad. Tol.  2 MHz","p");
leg2->AddEntry(grSNMPV_pl10_chip2_freq2 ,"Chip 2: Standard.  2 MHz","p");
leg2->AddEntry(grSNMPV_pl9_chip4_freq10, "Chip 4: Rad. Tol. 10 MHz","p");
leg2->AddEntry(grSNMPV_pl10_chip4_freq10 ,"Chip 4: Standard. 10 MHz","p");
leg2->AddEntry(grSNMPV_pl9_chip4_freq2 , "Chip 4: Rad. Tol.  2 MHz","p");
leg2->AddEntry(grSNMPV_pl10_chip4_freq2 ,"Chip 4: Standard.  2 MHz","p");

leg2->SetFillStyle(0);
leg2->Draw();


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(3);
ccomp_3->SetLeftMargin(0.14);
ccomp_3->SetGridy(0);
ccomp_3->SetGridx(0);
ccomp->cd(3);

TH2F *heffFrame3 = new TH2F("heffFrame3","Mimostar 2. noise (e-) vs Temp. C",43,-5,42,100,8,25);
heffFrame3->SetStats(0000000);
heffFrame3->SetXTitle("Temperature. C");
heffFrame3->SetYTitle("Noise (e-)");
heffFrame3->Draw();

grNOISE_pl9_chip2_freq10  = new TGraphErrors(Nrun,TCArray_chip2_freq10,NoiseArray_pl9_chip2_freq10,
					     TCerrArray_chip2_freq10,TCerrArray_chip2_freq10);
grNOISE_pl9_chip2_freq10->SetMarkerColor(4);
grNOISE_pl9_chip2_freq10->SetMarkerStyle(21);
grNOISE_pl9_chip2_freq10->SetMarkerSize(0.7);
grNOISE_pl9_chip2_freq10->SetLineColor(4);
grNOISE_pl9_chip2_freq10->Draw("LP");

grNOISE_pl10_chip2_freq10  = new TGraphErrors(Nrun,TCArray_chip2_freq10,NoiseArray_pl10_chip2_freq10,
					     TCerrArray_chip2_freq10,TCerrArray_chip2_freq10);
grNOISE_pl10_chip2_freq10->SetMarkerColor(2);
grNOISE_pl10_chip2_freq10->SetMarkerStyle(21);
grNOISE_pl10_chip2_freq10->SetMarkerSize(0.7);
grNOISE_pl10_chip2_freq10->SetLineColor(2);
grNOISE_pl10_chip2_freq10->Draw("LP");

grNOISE_pl9_chip2_freq2  = new TGraphErrors(Nrun,TCArray_chip2_freq2,NoiseArray_pl9_chip2_freq2,
					     TCerrArray_chip2_freq2,TCerrArray_chip2_freq2);
grNOISE_pl9_chip2_freq2->SetMarkerColor(8);
grNOISE_pl9_chip2_freq2->SetMarkerStyle(21);
grNOISE_pl9_chip2_freq2->SetMarkerSize(0.7);
grNOISE_pl9_chip2_freq2->SetLineColor(8);
grNOISE_pl9_chip2_freq2->Draw("LP");

grNOISE_pl10_chip2_freq2  = new TGraphErrors(Nrun,TCArray_chip2_freq2,NoiseArray_pl10_chip2_freq2,
					     TCerrArray_chip2_freq2,TCerrArray_chip2_freq2);
grNOISE_pl10_chip2_freq2->SetMarkerColor(1);
grNOISE_pl10_chip2_freq2->SetMarkerStyle(21);
grNOISE_pl10_chip2_freq2->SetMarkerSize(0.7);
grNOISE_pl10_chip2_freq2->SetLineColor(1);
grNOISE_pl10_chip2_freq2->Draw("LP");

grNOISE_pl9_chip4_freq10  = new TGraphErrors(Nrun,TCArray_chip4_freq10,NoiseArray_pl9_chip4_freq10,
					     TCerrArray_chip4_freq10,TCerrArray_chip4_freq10);
grNOISE_pl9_chip4_freq10->SetMarkerColor(4);
grNOISE_pl9_chip4_freq10->SetMarkerStyle(22);
grNOISE_pl9_chip4_freq10->SetMarkerSize(0.7);
grNOISE_pl9_chip4_freq10->SetLineColor(4);
grNOISE_pl9_chip4_freq10->Draw("LP");

grNOISE_pl10_chip4_freq10  = new TGraphErrors(Nrun,TCArray_chip4_freq10,NoiseArray_pl10_chip4_freq10,
					     TCerrArray_chip4_freq10,TCerrArray_chip4_freq10);
grNOISE_pl10_chip4_freq10->SetMarkerColor(2);
grNOISE_pl10_chip4_freq10->SetMarkerStyle(22);
grNOISE_pl10_chip4_freq10->SetMarkerSize(0.7);
grNOISE_pl10_chip4_freq10->SetLineColor(2);
grNOISE_pl10_chip4_freq10->Draw("LP");

grNOISE_pl9_chip4_freq2  = new TGraphErrors(Nrun,TCArray_chip4_freq2,NoiseArray_pl9_chip4_freq2,
					     TCerrArray_chip4_freq2,TCerrArray_chip4_freq2);
grNOISE_pl9_chip4_freq2->SetMarkerColor(8);
grNOISE_pl9_chip4_freq2->SetMarkerStyle(22);
grNOISE_pl9_chip4_freq2->SetMarkerSize(0.7);
grNOISE_pl9_chip4_freq2->SetLineColor(8);
grNOISE_pl9_chip4_freq2->Draw("LP");

grNOISE_pl10_chip4_freq2  = new TGraphErrors(Nrun,TCArray_chip4_freq2,NoiseArray_pl10_chip4_freq2,
					     TCerrArray_chip4_freq2,TCerrArray_chip4_freq2);
grNOISE_pl10_chip4_freq2->SetMarkerColor(1);
grNOISE_pl10_chip4_freq2->SetMarkerStyle(22);
grNOISE_pl10_chip4_freq2->SetMarkerSize(0.7);
grNOISE_pl10_chip4_freq2->SetLineColor(1);
grNOISE_pl10_chip4_freq2->Draw("LP");

leg3 = new TLegend(0.18,0.5,0.5,0.85);
leg3->SetHeader(" Chip / Matrix / Freq.");
leg3->AddEntry(grNOISE_pl9_chip2_freq10, "Chip 2: Rad. Tol. 10 MHz","p");
leg3->AddEntry(grNOISE_pl10_chip2_freq10 ,"Chip 2: Standard. 10 MHz","p");
leg3->AddEntry(grNOISE_pl9_chip2_freq2 , "Chip 2: Rad. Tol.  2 MHz","p");
leg3->AddEntry(grNOISE_pl10_chip2_freq2 ,"Chip 2: Standard.  2 MHz","p");

leg3->AddEntry(grNOISE_pl9_chip4_freq10, "Chip 4: Rad. Tol. 10 MHz","p");
leg3->AddEntry(grNOISE_pl10_chip4_freq10 ,"Chip 4: Standard. 10 MHz","p");
leg3->AddEntry(grNOISE_pl9_chip4_freq2 , "Chip 4: Rad. Tol.  2 MHz","p");
leg3->AddEntry(grNOISE_pl10_chip4_freq2 ,"Chip 4: Standard.  2 MHz","p");
leg3->SetFillStyle(0);
leg3->Draw();


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++







//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(4);
ccomp_4->SetLeftMargin(0.14);
ccomp_4->SetGridy(0);
ccomp_4->SetGridx(0);
ccomp->cd(4);

TH2F *heffFrame4 = new TH2F("heffFrame4","Mimostar 2. MPV Charge in seed (e-) vs Temp. C",43,-5,42,400,200,260);
heffFrame4->SetStats(0000000);
heffFrame4->SetXTitle("Temperature. C");
heffFrame4->SetYTitle("MPV Charge in Seed (e-)");
heffFrame4->Draw();

grCh1_pl9_chip2_freq10  = new TGraphErrors(Nrun,TCArray_chip2_freq10,Ch1Array_pl9_chip2_freq10,
					     TCerrArray_chip2_freq10,TCerrArray_chip2_freq10);
grCh1_pl9_chip2_freq10->SetMarkerColor(4);
grCh1_pl9_chip2_freq10->SetMarkerStyle(21);
grCh1_pl9_chip2_freq10->SetMarkerSize(0.7);
grCh1_pl9_chip2_freq10->SetLineColor(4);
grCh1_pl9_chip2_freq10->Draw("LP");

grCh1_pl10_chip2_freq10  = new TGraphErrors(Nrun,TCArray_chip2_freq10,Ch1Array_pl10_chip2_freq10,
					     TCerrArray_chip2_freq10,TCerrArray_chip2_freq10);
grCh1_pl10_chip2_freq10->SetMarkerColor(2);
grCh1_pl10_chip2_freq10->SetMarkerStyle(21);
grCh1_pl10_chip2_freq10->SetMarkerSize(0.7);
grCh1_pl10_chip2_freq10->SetLineColor(2);
grCh1_pl10_chip2_freq10->Draw("LP");

grCh1_pl9_chip2_freq2  = new TGraphErrors(Nrun,TCArray_chip2_freq2,Ch1Array_pl9_chip2_freq2,
					     TCerrArray_chip2_freq2,TCerrArray_chip2_freq2);
grCh1_pl9_chip2_freq2->SetMarkerColor(8);
grCh1_pl9_chip2_freq2->SetMarkerStyle(21);
grCh1_pl9_chip2_freq2->SetMarkerSize(0.7);
grCh1_pl9_chip2_freq2->SetLineColor(8);
grCh1_pl9_chip2_freq2->Draw("LP");

grCh1_pl10_chip2_freq2  = new TGraphErrors(Nrun,TCArray_chip2_freq2,Ch1Array_pl10_chip2_freq2,
					     TCerrArray_chip2_freq2,TCerrArray_chip2_freq2);
grCh1_pl10_chip2_freq2->SetMarkerColor(1);
grCh1_pl10_chip2_freq2->SetMarkerStyle(21);
grCh1_pl10_chip2_freq2->SetMarkerSize(0.7);
grCh1_pl10_chip2_freq2->SetLineColor(1);
grCh1_pl10_chip2_freq2->Draw("LP");

grCh1_pl9_chip4_freq10  = new TGraphErrors(Nrun,TCArray_chip4_freq10,Ch1Array_pl9_chip4_freq10,
					     TCerrArray_chip4_freq10,TCerrArray_chip4_freq10);
grCh1_pl9_chip4_freq10->SetMarkerColor(4);
grCh1_pl9_chip4_freq10->SetMarkerStyle(22);
grCh1_pl9_chip4_freq10->SetMarkerSize(0.7);
grCh1_pl9_chip4_freq10->SetLineColor(4);
grCh1_pl9_chip4_freq10->Draw("LP");

grCh1_pl10_chip4_freq10  = new TGraphErrors(Nrun,TCArray_chip4_freq10,Ch1Array_pl10_chip4_freq10,
					     TCerrArray_chip4_freq10,TCerrArray_chip4_freq10);
grCh1_pl10_chip4_freq10->SetMarkerColor(2);
grCh1_pl10_chip4_freq10->SetMarkerStyle(22);
grCh1_pl10_chip4_freq10->SetMarkerSize(0.7);
grCh1_pl10_chip4_freq10->SetLineColor(2);
grCh1_pl10_chip4_freq10->Draw("LP");

grCh1_pl9_chip4_freq2  = new TGraphErrors(Nrun,TCArray_chip4_freq2,Ch1Array_pl9_chip4_freq2,
					     TCerrArray_chip4_freq2,TCerrArray_chip4_freq2);
grCh1_pl9_chip4_freq2->SetMarkerColor(8);
grCh1_pl9_chip4_freq2->SetMarkerStyle(22);
grCh1_pl9_chip4_freq2->SetMarkerSize(0.7);
grCh1_pl9_chip4_freq2->SetLineColor(8);
grCh1_pl9_chip4_freq2->Draw("LP");

grCh1_pl10_chip4_freq2  = new TGraphErrors(Nrun,TCArray_chip4_freq2,Ch1Array_pl10_chip4_freq2,
					     TCerrArray_chip4_freq2,TCerrArray_chip4_freq2);
grCh1_pl10_chip4_freq2->SetMarkerColor(1);
grCh1_pl10_chip4_freq2->SetMarkerStyle(22);
grCh1_pl10_chip4_freq2->SetMarkerSize(0.7);
grCh1_pl10_chip4_freq2->SetLineColor(1);
grCh1_pl10_chip4_freq2->Draw("LP");

leg4 = new TLegend(0.18,0.55,0.5,0.88);
leg4->SetHeader(" Chip / Matrix / Freq.");
leg4->AddEntry(grCh1_pl9_chip2_freq10, "Chip 2: Rad. Tol. 10 MHz","p");
leg4->AddEntry(grCh1_pl10_chip2_freq10 ,"Chip 2: Standard. 10 MHz","p");
leg4->AddEntry(grCh1_pl9_chip2_freq2 , "Chip 2: Rad. Tol.  2 MHz","p");
leg4->AddEntry(grCh1_pl10_chip2_freq2 ,"Chip 2: Standard.  2 MHz","p");

leg4->AddEntry(grCh1_pl9_chip4_freq10, "Chip 4: Rad. Tol. 10 MHz","p");
leg4->AddEntry(grCh1_pl10_chip4_freq10 ,"Chip 4: Standard. 10 MHz","p");
leg4->AddEntry(grCh1_pl9_chip4_freq2 , "Chip 4: Rad. Tol.  2 MHz","p");
leg4->AddEntry(grCh1_pl10_chip4_freq2 ,"Chip 4: Standard.  2 MHz","p");
leg4->SetFillStyle(0);
leg4->Draw();

ccomp->cd(0);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const Int_t Nrun2 = 4;


 Int_t RunArray_vdd_T40_chip4_freq2[Nrun]           = {  14520, 14518 , 14509, 14522};
 Float_t VddArray_vdd_T40_chip4_freq2[Nrun]           = {     1.7,     2.2 ,      3.3,     3.9};
 Float_t TCerrArray_vdd_T40_chip4_freq2[Nrun]           = {     0,     0 ,      0,     0};
 Float_t EfficiencyArray_vdd_T40_pl9_chip4_freq2[Nrun] = {97.160, 98.511,98.725, 88.618};
 Float_t EffErrorArray_vdd_T40_pl9_chip4_freq2[Nrun]  =  {0.198, 0.146, 0.131, 0.380};
 Float_t EfficiencyArray_vdd_T40_pl10_chip4_freq2[Nrun] = {99.523,99.751, 99.874, 97.382 };
 Float_t EffErrorArray_vdd_T40_pl10_chip4_freq2[Nrun]  =  {0.079,0.057,0.040, 0.193};

 Int_t RunArray_vdd_T20_chip4_freq2[Nrun]           = { 14519, 14517 ,  14513, 14521};
 Float_t VddArray_vdd_T20_chip4_freq2[Nrun]           = {     1.7,     2.2 ,      3.3,     3.9};
 Float_t TCerrArray_vdd_T20_chip4_freq2[Nrun]           = {     0,     0 ,      0,     0};
 Float_t EfficiencyArray_vdd_T20_pl9_chip4_freq2[Nrun] = { 99.416, 99.807, 99.890, 87.903};
 Float_t EffErrorArray_vdd_T20_pl9_chip4_freq2[Nrun]  =  { 0.089, 0.052, 0.039, 0.374 };
 Float_t EfficiencyArray_vdd_T20_pl10_chip4_freq2[Nrun] = { 99.792, 99.939, 99.987, 96.297};
 Float_t EffErrorArray_vdd_T20_pl10_chip4_freq2[Nrun]  =  { 0.052, 0.027, 0.013, 0.216 };




Float_t SNMPVArray_vdd_T40_pl9_chip4_freq2[Nrun];
Float_t SNMPVArray_vdd_T40_pl10_chip4_freq2[Nrun];
Float_t SNMPVArray_vdd_T20_pl9_chip4_freq2[Nrun];
Float_t SNMPVArray_vdd_T20_pl10_chip4_freq2[Nrun];

Float_t NoiseArray_vdd_T40_pl9_chip4_freq2[Nrun];
Float_t NoiseArray_vdd_T40_pl10_chip4_freq2[Nrun];
Float_t NoiseArray_vdd_T20_pl9_chip4_freq2[Nrun];
Float_t NoiseArray_vdd_T20_pl10_chip4_freq2[Nrun];

Float_t Ch1Array_vdd_T40_pl9_chip4_freq2[Nrun];
Float_t Ch1Array_vdd_T40_pl10_chip4_freq2[Nrun];
Float_t Ch1Array_vdd_T20_pl9_chip4_freq2[Nrun];
Float_t Ch1Array_vdd_T20_pl10_chip4_freq2[Nrun];

TH1F *hM14_Eff_vdd_T40_pl9_chip4_freq2[Nrun];
TH1F *hM14_Eff_vdd_T40_pl10_chip4_freq2[Nrun];
TH1F *hM14_Eff_vdd_T20_pl9_chip4_freq2[Nrun];
TH1F *hM14_Eff_vdd_T20_pl10_chip4_freq2[Nrun];

TH1F *hM14_EffError_vdd_T40_pl9_chip4_freq2[Nrun];
TH1F *hM14_EffError_vdd_T40_pl10_chip4_freq2[Nrun];
TH1F *hM14_EffError_vdd_T20_pl9_chip4_freq2[Nrun];
TH1F *hM14_EffError_vdd_T20_pl10_chip4_freq2[Nrun];

TH1F *hM14_SNseed_vdd_T40_pl9_chip4_freq2[Nrun];
TH1F *hM14_SNseed_vdd_T40_pl10_chip4_freq2[Nrun];
TH1F *hM14_SNseed_vdd_T20_pl9_chip4_freq2[Nrun];
TH1F *hM14_SNseed_vdd_T20_pl10_chip4_freq2[Nrun];

TH1F *hM14_Noise_vdd_T40_pl9_chip4_freq2[Nrun];
TH1F *hM14_Noise_vdd_T40_pl10_chip4_freq2[Nrun];
TH1F *hM14_Noise_vdd_T20_pl9_chip4_freq2[Nrun];
TH1F *hM14_Noise_vdd_T20_pl10_chip4_freq2[Nrun];

TH1F *hM14_Ch1_vdd_T40_pl9_chip4_freq2[Nrun];
TH1F *hM14_Ch1_vdd_T40_pl10_chip4_freq2[Nrun];
TH1F *hM14_Ch1_vdd_T20_pl9_chip4_freq2[Nrun];
TH1F *hM14_Ch1_vdd_T20_pl10_chip4_freq2[Nrun];


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp2 = new TCanvas("ccomp2","MimoSTAR 2",550,10,700,800);
ccomp2->Draw();
ccomp2->Clear();
ccomp2->SetFillColor(0);
ccomp2->SetBorderMode(0);
ccomp2->Divide(2,2);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp2->cd(1);
ccomp2_1->SetLeftMargin(0.14);
//gStyle->SetLabelOffset(0.1,"Y");
gStyle->SetTitleYSize(0.05);
ccomp2_1->SetGridy(0);
ccomp2_1->SetGridx(0);

TH2F *heffFrame5 = new TH2F("heffFrame5","Mimostar 2. Chip4. 2 MHz. Efficiency (%) vs Vdd diode (V)",35,1,4.5,300,86.2,101.1);
heffFrame5->SetStats(0000000);
heffFrame5->SetXTitle("Vdd diode (V)");
heffFrame5->SetYTitle("Efficiency (%)");
heffFrame5->Draw();

gr_vdd_T40_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					      EfficiencyArray_vdd_T40_pl9_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,EffErrorArray_vdd_T40_pl9_chip4_freq2);
gr_vdd_T40_pl9_chip4_freq2->SetMarkerColor(1);
gr_vdd_T40_pl9_chip4_freq2->SetMarkerStyle(21);
gr_vdd_T40_pl9_chip4_freq2->SetMarkerSize(0.7);
gr_vdd_T40_pl9_chip4_freq2->SetLineColor(1);
gr_vdd_T40_pl9_chip4_freq2->Draw("LP");

gr_vdd_T40_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					       EfficiencyArray_vdd_T40_pl10_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,EffErrorArray_vdd_T40_pl10_chip4_freq2);
gr_vdd_T40_pl10_chip4_freq2->SetMarkerColor(2);
gr_vdd_T40_pl10_chip4_freq2->SetMarkerStyle(21);
gr_vdd_T40_pl10_chip4_freq2->SetMarkerSize(0.7);
gr_vdd_T40_pl10_chip4_freq2->SetLineColor(2);
gr_vdd_T40_pl10_chip4_freq2->Draw("LP");

gr_vdd_T20_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,EfficiencyArray_vdd_T20_pl9_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,EffErrorArray_vdd_T20_pl9_chip4_freq2);
gr_vdd_T20_pl9_chip4_freq2->SetMarkerColor(4);
gr_vdd_T20_pl9_chip4_freq2->SetMarkerStyle(21);
gr_vdd_T20_pl9_chip4_freq2->SetMarkerSize(0.7);
gr_vdd_T20_pl9_chip4_freq2->SetLineColor(4);
gr_vdd_T20_pl9_chip4_freq2->Draw("LP");

gr_vdd_T20_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,
					       EfficiencyArray_vdd_T20_pl10_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,EffErrorArray_vdd_T20_pl10_chip4_freq2);
gr_vdd_T20_pl10_chip4_freq2->SetMarkerColor(6);
gr_vdd_T20_pl10_chip4_freq2->SetMarkerStyle(21);
gr_vdd_T20_pl10_chip4_freq2->SetMarkerSize(0.7);
gr_vdd_T20_pl10_chip4_freq2->SetLineColor(6);
gr_vdd_T20_pl10_chip4_freq2->Draw("LP");

leg5 = new TLegend(0.18,0.25,0.5,0.45);
leg5->SetHeader("Matrix / Temperature");
leg5->AddEntry(gr_vdd_T40_pl9_chip4_freq2, "Rad. Tol. 40 deg.","p");
leg5->AddEntry(gr_vdd_T40_pl10_chip4_freq2,"Standard. 40 deg.","p");
leg5->AddEntry(gr_vdd_T20_pl9_chip4_freq2, "Rad. Tol. 20 deg.","p");
leg5->AddEntry(gr_vdd_T20_pl10_chip4_freq2,"Standard. 20 deg.","p");

leg5->SetFillStyle(0);
leg5->Draw();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

for (Int_t i=0 ; i<Nrun ; i++){

   Char_t  ResultFileName_vdd_T40_pl9_chip4_freq2[150];
   sprintf(ResultFileName_vdd_T40_pl9_chip4_freq2,
	   "Results/%d_pl9_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_9.root",
	   RunArray_vdd_T40_chip4_freq2[i],RunArray_vdd_T40_chip4_freq2[i]);
   TFile *ResultRootFile_vdd_T40_pl9_chip4_freq2=new TFile(ResultFileName_vdd_T40_pl9_chip4_freq2,"read");
   TCanvas *MainCanvas_vdd_T40_pl9_chip4_freq2 = (TCanvas*)ResultRootFile_vdd_T40_pl9_chip4_freq2.Get("MainCanvas");
   TPad *mpad3_vdd_T40_pl9_chip4_freq2 = (TPad*)MainCanvas_vdd_T40_pl9_chip4_freq2->GetPrimitive("mpad3");
   hM14_SNseed_vdd_T40_pl9_chip4_freq2[i]= (TH1F*)mpad3_vdd_T40_pl9_chip4_freq2->GetPrimitive("hsnc");
   TPad *mpad4_vdd_T40_pl9_chip4_freq2 = (TPad*)MainCanvas_vdd_T40_pl9_chip4_freq2->GetPrimitive("mpad4");
   hM14_Noise_vdd_T40_pl9_chip4_freq2[i]= (TH1F*)mpad4_vdd_T40_pl9_chip4_freq2->GetPrimitive("hRTN");
   TPad *mpad6_vdd_T40_pl9_chip4_freq2 = (TPad*)MainCanvas_vdd_T40_pl9_chip4_freq2->GetPrimitive("mpad6");
   hM14_Ch1_vdd_T40_pl9_chip4_freq2[i]= (TH1F*)mpad6_vdd_T40_pl9_chip4_freq2->GetPrimitive("hqc1");
   Ch1Array_vdd_T40_pl9_chip4_freq2[i] =
     (((TH1F*)mpad6_vdd_T40_pl9_chip4_freq2->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
   SNMPVArray_vdd_T40_pl9_chip4_freq2[i]= 
     (((TH1F*)mpad3_vdd_T40_pl9_chip4_freq2->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_vdd_T40_pl9_chip4_freq2[i] =hM14_Noise_vdd_T40_pl9_chip4_freq2[i]->GetMean();
   cout<<"---->T40_pl9 C SN N "<<Ch1Array_vdd_T40_pl9_chip4_freq2[i]<<" "<<SNMPVArray_vdd_T40_pl9_chip4_freq2[i]<<" "<<NoiseArray_vdd_T40_pl9_chip4_freq2[i]<<endl;

   Char_t  ResultFileName_vdd_T40_pl10_chip4_freq2[150];
   sprintf(ResultFileName_vdd_T40_pl10_chip4_freq2,
	   "Results/%d_pl10_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_10.root",
	   RunArray_vdd_T40_chip4_freq2[i],RunArray_vdd_T40_chip4_freq2[i]);
   TFile *ResultRootFile_vdd_T40_pl10_chip4_freq2=new TFile(ResultFileName_vdd_T40_pl10_chip4_freq2,"read");
   TCanvas *MainCanvas_vdd_T40_pl10_chip4_freq2 = (TCanvas*)ResultRootFile_vdd_T40_pl10_chip4_freq2.Get("MainCanvas");
   TPad *mpad3_vdd_T40_pl10_chip4_freq2 = (TPad*)MainCanvas_vdd_T40_pl10_chip4_freq2->GetPrimitive("mpad3");
   hM14_SNseed_vdd_T40_pl10_chip4_freq2[i]= (TH1F*)mpad3_vdd_T40_pl10_chip4_freq2->GetPrimitive("hsnc");
   TPad *mpad4_vdd_T40_pl10_chip4_freq2 = (TPad*)MainCanvas_vdd_T40_pl10_chip4_freq2->GetPrimitive("mpad4");
   hM14_Noise_vdd_T40_pl10_chip4_freq2[i]= (TH1F*)mpad4_vdd_T40_pl10_chip4_freq2->GetPrimitive("hRTN");
   TPad *mpad6_vdd_T40_pl10_chip4_freq2 = (TPad*)MainCanvas_vdd_T40_pl10_chip4_freq2->GetPrimitive("mpad6");
   hM14_Ch1_vdd_T40_pl10_chip4_freq2[i]= (TH1F*)mpad6_vdd_T40_pl10_chip4_freq2->GetPrimitive("hqc1");
   Ch1Array_vdd_T40_pl10_chip4_freq2[i] =
     (((TH1F*)mpad6_vdd_T40_pl10_chip4_freq2->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
   SNMPVArray_vdd_T40_pl10_chip4_freq2[i]= 
     (((TH1F*)mpad3_vdd_T40_pl10_chip4_freq2->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_vdd_T40_pl10_chip4_freq2[i] =hM14_Noise_vdd_T40_pl10_chip4_freq2[i]->GetMean();
   cout<<"---->T40_pl10 C SN N "<<Ch1Array_vdd_T40_pl10_chip4_freq2[i]<<" "<<SNMPVArray_vdd_T40_pl10_chip4_freq2[i]<<" "<<NoiseArray_vdd_T40_pl10_chip4_freq2[i]<<endl;

   Char_t  ResultFileName_vdd_T20_pl9_chip4_freq2[150];
   sprintf(ResultFileName_vdd_T20_pl9_chip4_freq2,
	   "Results/%d_pl9_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_9.root",
	   RunArray_vdd_T20_chip4_freq2[i],RunArray_vdd_T20_chip4_freq2[i]);
   TFile *ResultRootFile_vdd_T20_pl9_chip4_freq2=new TFile(ResultFileName_vdd_T20_pl9_chip4_freq2,"read");
   TCanvas *MainCanvas_vdd_T20_pl9_chip4_freq2 = (TCanvas*)ResultRootFile_vdd_T20_pl9_chip4_freq2.Get("MainCanvas");
   TPad *mpad3_vdd_T20_pl9_chip4_freq2 = (TPad*)MainCanvas_vdd_T20_pl9_chip4_freq2->GetPrimitive("mpad3");
   hM14_SNseed_vdd_T20_pl9_chip4_freq2[i]= (TH1F*)mpad3_vdd_T20_pl9_chip4_freq2->GetPrimitive("hsnc");
   TPad *mpad4_vdd_T20_pl9_chip4_freq2 = (TPad*)MainCanvas_vdd_T20_pl9_chip4_freq2->GetPrimitive("mpad4");
   hM14_Noise_vdd_T20_pl9_chip4_freq2[i]= (TH1F*)mpad4_vdd_T20_pl9_chip4_freq2->GetPrimitive("hRTN");
   TPad *mpad6_vdd_T20_pl9_chip4_freq2 = (TPad*)MainCanvas_vdd_T20_pl9_chip4_freq2->GetPrimitive("mpad6");
   hM14_Ch1_vdd_T20_pl9_chip4_freq2[i]= (TH1F*)mpad6_vdd_T20_pl9_chip4_freq2->GetPrimitive("hqc1");
   Ch1Array_vdd_T20_pl9_chip4_freq2[i] =
     (((TH1F*)mpad6_vdd_T20_pl9_chip4_freq2->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
   SNMPVArray_vdd_T20_pl9_chip4_freq2[i]= 
     (((TH1F*)mpad3_vdd_T20_pl9_chip4_freq2->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_vdd_T20_pl9_chip4_freq2[i] =hM14_Noise_vdd_T20_pl9_chip4_freq2[i]->GetMean();
   cout<<"---->T20_pl9 C SN N "<<Ch1Array_vdd_T20_pl9_chip4_freq2[i]<<" "<<SNMPVArray_vdd_T20_pl9_chip4_freq2[i]<<" "<<NoiseArray_vdd_T20_pl9_chip4_freq2[i]<<endl;

   Char_t  ResultFileName_vdd_T20_pl10_chip4_freq2[150];
   sprintf(ResultFileName_vdd_T20_pl10_chip4_freq2,
	   "Results/%d_pl10_Sub1_Seed5_Neig2_geomat3_T2h150_HitMap0/AllPlots_%d_10.root",
	   RunArray_vdd_T20_chip4_freq2[i],RunArray_vdd_T20_chip4_freq2[i]);
   TFile *ResultRootFile_vdd_T20_pl10_chip4_freq2=new TFile(ResultFileName_vdd_T20_pl10_chip4_freq2,"read");
   TCanvas *MainCanvas_vdd_T20_pl10_chip4_freq2 = (TCanvas*)ResultRootFile_vdd_T20_pl10_chip4_freq2.Get("MainCanvas");
   TPad *mpad3_vdd_T20_pl10_chip4_freq2 = (TPad*)MainCanvas_vdd_T20_pl10_chip4_freq2->GetPrimitive("mpad3");
   hM14_SNseed_vdd_T20_pl10_chip4_freq2[i]= (TH1F*)mpad3_vdd_T20_pl10_chip4_freq2->GetPrimitive("hsnc");
   TPad *mpad4_vdd_T20_pl10_chip4_freq2 = (TPad*)MainCanvas_vdd_T20_pl10_chip4_freq2->GetPrimitive("mpad4");
   hM14_Noise_vdd_T20_pl10_chip4_freq2[i]= (TH1F*)mpad4_vdd_T20_pl10_chip4_freq2->GetPrimitive("hRTN");
   TPad *mpad6_vdd_T20_pl10_chip4_freq2 = (TPad*)MainCanvas_vdd_T20_pl10_chip4_freq2->GetPrimitive("mpad6");
   hM14_Ch1_vdd_T20_pl10_chip4_freq2[i]= (TH1F*)mpad6_vdd_T20_pl10_chip4_freq2->GetPrimitive("hqc1");
   Ch1Array_vdd_T20_pl10_chip4_freq2[i] =
     (((TH1F*)mpad6_vdd_T20_pl10_chip4_freq2->GetPrimitive("hqc1"))->GetFunction("landau"))->GetParameter(1);
   SNMPVArray_vdd_T20_pl10_chip4_freq2[i]= 
     (((TH1F*)mpad3_vdd_T20_pl10_chip4_freq2->GetPrimitive("hsnc"))->GetFunction("landau"))->GetParameter(1);
   NoiseArray_vdd_T20_pl10_chip4_freq2[i] =hM14_Noise_vdd_T20_pl10_chip4_freq2[i]->GetMean();
   cout<<"---->T20_pl10 C SN N "<<Ch1Array_vdd_T20_pl10_chip4_freq2[i]<<" "<<SNMPVArray_vdd_T20_pl10_chip4_freq2[i]<<" "<<NoiseArray_vdd_T20_pl10_chip4_freq2[i]<<endl;




}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp2->cd(2);
ccomp2_2->SetLeftMargin(0.14);
ccomp2_2->SetGridy(0);
ccomp2_2->SetGridx(0);


TH2F *heffFrame6 = new TH2F("heffFrame6","Mimostar 2. Chip4. 2 MHz. S/N (MPV) vs vs Vdd diode (V)",30,1.0,4.5,100,0,24);
heffFrame6->SetStats(0000000);
heffFrame6->SetXTitle("Vdd diode (V)");
heffFrame6->SetYTitle("S/N (MPV)");
heffFrame6->Draw();
ccomp2->cd(2);

grSNMPV_vdd_T40_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					      SNMPVArray_vdd_T40_pl9_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,TCerrArray_vdd_T40_chip4_freq2);
grSNMPV_vdd_T40_pl9_chip4_freq2->SetMarkerColor(1);
grSNMPV_vdd_T40_pl9_chip4_freq2->SetMarkerStyle(21);
grSNMPV_vdd_T40_pl9_chip4_freq2->SetMarkerSize(0.7);
grSNMPV_vdd_T40_pl9_chip4_freq2->SetLineColor(1);
grSNMPV_vdd_T40_pl9_chip4_freq2->Draw("LP");

grSNMPV_vdd_T40_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					      SNMPVArray_vdd_T40_pl10_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,TCerrArray_vdd_T40_chip4_freq2);
grSNMPV_vdd_T40_pl10_chip4_freq2->SetMarkerColor(2);
grSNMPV_vdd_T40_pl10_chip4_freq2->SetMarkerStyle(21);
grSNMPV_vdd_T40_pl10_chip4_freq2->SetMarkerSize(0.7);
grSNMPV_vdd_T40_pl10_chip4_freq2->SetLineColor(2);
grSNMPV_vdd_T40_pl10_chip4_freq2->Draw("LP");

grSNMPV_vdd_T20_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,
					      SNMPVArray_vdd_T20_pl9_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,TCerrArray_vdd_T20_chip4_freq2);
grSNMPV_vdd_T20_pl9_chip4_freq2->SetMarkerColor(4);
grSNMPV_vdd_T20_pl9_chip4_freq2->SetMarkerStyle(21);
grSNMPV_vdd_T20_pl9_chip4_freq2->SetMarkerSize(0.7);
grSNMPV_vdd_T20_pl9_chip4_freq2->SetLineColor(4);
grSNMPV_vdd_T20_pl9_chip4_freq2->Draw("LP");

grSNMPV_vdd_T20_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,
					      SNMPVArray_vdd_T20_pl10_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,TCerrArray_vdd_T20_chip4_freq2);
grSNMPV_vdd_T20_pl10_chip4_freq2->SetMarkerColor(6);
grSNMPV_vdd_T20_pl10_chip4_freq2->SetMarkerStyle(21);
grSNMPV_vdd_T20_pl10_chip4_freq2->SetMarkerSize(0.7);
grSNMPV_vdd_T20_pl10_chip4_freq2->SetLineColor(6);
grSNMPV_vdd_T20_pl10_chip4_freq2->Draw("LP");

leg6 = new TLegend(0.18,0.25,0.5,0.45);
leg6->SetHeader("Matrix / Temperature");
leg6->AddEntry(grSNMPV_vdd_T40_pl9_chip4_freq2, "Rad. Tol. 40 deg.","p");
leg6->AddEntry(grSNMPV_vdd_T40_pl10_chip4_freq2,"Standard. 40 deg.","p");
leg6->AddEntry(grSNMPV_vdd_T20_pl9_chip4_freq2, "Rad. Tol. 20 deg.","p");
leg6->AddEntry(grSNMPV_vdd_T20_pl10_chip4_freq2,"Standard. 20 deg.","p");
leg6->SetFillStyle(0);
leg6->Draw();


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp2->cd(3);
ccomp2_3->SetLeftMargin(0.14);
ccomp2_3->SetGridy(0);
ccomp2_3->SetGridx(0);


TH2F *heffFrame7 = new TH2F("heffFrame7","Mimostar 2. Chip4. 2 MHz. Noise (e-) vs Vdd diode (V)",30,1.0,4.5,100,0,24);
heffFrame7->SetStats(0000000);
heffFrame7->SetXTitle("Vdd diode (V)");
heffFrame7->SetYTitle("Noise (e-)");
heffFrame7->Draw();
ccomp2->cd(3);

grNoise_vdd_T40_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					      NoiseArray_vdd_T40_pl9_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,TCerrArray_vdd_T40_chip4_freq2);
grNoise_vdd_T40_pl9_chip4_freq2->SetMarkerColor(1);
grNoise_vdd_T40_pl9_chip4_freq2->SetMarkerStyle(21);
grNoise_vdd_T40_pl9_chip4_freq2->SetMarkerSize(0.7);
grNoise_vdd_T40_pl9_chip4_freq2->SetLineColor(1);
grNoise_vdd_T40_pl9_chip4_freq2->Draw("LP");

grNoise_vdd_T40_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					      NoiseArray_vdd_T40_pl10_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,TCerrArray_vdd_T40_chip4_freq2);
grNoise_vdd_T40_pl10_chip4_freq2->SetMarkerColor(2);
grNoise_vdd_T40_pl10_chip4_freq2->SetMarkerStyle(21);
grNoise_vdd_T40_pl10_chip4_freq2->SetMarkerSize(0.7);
grNoise_vdd_T40_pl10_chip4_freq2->SetLineColor(2);
grNoise_vdd_T40_pl10_chip4_freq2->Draw("LP");

grNoise_vdd_T20_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,
					      NoiseArray_vdd_T20_pl9_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,TCerrArray_vdd_T20_chip4_freq2);
grNoise_vdd_T20_pl9_chip4_freq2->SetMarkerColor(4);
grNoise_vdd_T20_pl9_chip4_freq2->SetMarkerStyle(21);
grNoise_vdd_T20_pl9_chip4_freq2->SetMarkerSize(0.7);
grNoise_vdd_T20_pl9_chip4_freq2->SetLineColor(4);
grNoise_vdd_T20_pl9_chip4_freq2->Draw("LP");

grNoise_vdd_T20_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,
					      NoiseArray_vdd_T20_pl10_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,TCerrArray_vdd_T20_chip4_freq2);
grNoise_vdd_T20_pl10_chip4_freq2->SetMarkerColor(6);
grNoise_vdd_T20_pl10_chip4_freq2->SetMarkerStyle(21);
grNoise_vdd_T20_pl10_chip4_freq2->SetMarkerSize(0.7);
grNoise_vdd_T20_pl10_chip4_freq2->SetLineColor(6);
grNoise_vdd_T20_pl10_chip4_freq2->Draw("LP");

leg7 = new TLegend(0.18,0.25,0.5,0.45);
leg7->SetHeader("Matrix / Temperature");
leg7->AddEntry(grNoise_vdd_T40_pl9_chip4_freq2, "Rad. Tol. 40 deg.","p");
leg7->AddEntry(grNoise_vdd_T40_pl10_chip4_freq2,"Standard. 40 deg.","p");
leg7->AddEntry(grNoise_vdd_T20_pl9_chip4_freq2, "Rad. Tol. 20 deg.","p");
leg7->AddEntry(grNoise_vdd_T20_pl10_chip4_freq2,"Standard. 20 deg.","p");
leg7->SetFillStyle(0);
leg7->Draw();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp2->cd(4);
ccomp2_4->SetLeftMargin(0.14);
ccomp2_4->SetGridy(0);
ccomp2_4->SetGridx(0);


TH2F *heffFrame8 = new TH2F("heffFrame8","Mimostar 2. Chip4. 2 MHz. MPV Charge in seed (e-) vs Vdd diode (V)",30,1.0,4.5,170,80,250);
heffFrame8->SetStats(0000000);
heffFrame8->SetXTitle("Vdd diode (V)");
heffFrame8->SetYTitle("MPV Charge in seed (e-)");
heffFrame8->Draw();
ccomp2->cd(4);

grCh1_vdd_T40_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					      Ch1Array_vdd_T40_pl9_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,TCerrArray_vdd_T40_chip4_freq2);
grCh1_vdd_T40_pl9_chip4_freq2->SetMarkerColor(1);
grCh1_vdd_T40_pl9_chip4_freq2->SetMarkerStyle(21);
grCh1_vdd_T40_pl9_chip4_freq2->SetMarkerSize(0.7);
grCh1_vdd_T40_pl9_chip4_freq2->SetLineColor(1);
grCh1_vdd_T40_pl9_chip4_freq2->Draw("LP");

grCh1_vdd_T40_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T40_chip4_freq2,
					      Ch1Array_vdd_T40_pl10_chip4_freq2,
				TCerrArray_vdd_T40_chip4_freq2,TCerrArray_vdd_T40_chip4_freq2);
grCh1_vdd_T40_pl10_chip4_freq2->SetMarkerColor(2);
grCh1_vdd_T40_pl10_chip4_freq2->SetMarkerStyle(21);
grCh1_vdd_T40_pl10_chip4_freq2->SetMarkerSize(0.7);
grCh1_vdd_T40_pl10_chip4_freq2->SetLineColor(2);
grCh1_vdd_T40_pl10_chip4_freq2->Draw("LP");

grCh1_vdd_T20_pl9_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,
					      Ch1Array_vdd_T20_pl9_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,TCerrArray_vdd_T20_chip4_freq2);
grCh1_vdd_T20_pl9_chip4_freq2->SetMarkerColor(4);
grCh1_vdd_T20_pl9_chip4_freq2->SetMarkerStyle(21);
grCh1_vdd_T20_pl9_chip4_freq2->SetMarkerSize(0.7);
grCh1_vdd_T20_pl9_chip4_freq2->SetLineColor(4);
grCh1_vdd_T20_pl9_chip4_freq2->Draw("LP");

grCh1_vdd_T20_pl10_chip4_freq2 = new TGraphErrors(Nrun2,VddArray_vdd_T20_chip4_freq2,
					      Ch1Array_vdd_T20_pl10_chip4_freq2,
				TCerrArray_vdd_T20_chip4_freq2,TCerrArray_vdd_T20_chip4_freq2);
grCh1_vdd_T20_pl10_chip4_freq2->SetMarkerColor(6);
grCh1_vdd_T20_pl10_chip4_freq2->SetMarkerStyle(21);
grCh1_vdd_T20_pl10_chip4_freq2->SetMarkerSize(0.7);
grCh1_vdd_T20_pl10_chip4_freq2->SetLineColor(6);
grCh1_vdd_T20_pl10_chip4_freq2->Draw("LP");

leg8 = new TLegend(0.18,0.25,0.5,0.45);
leg8->SetHeader("Matrix / Temperature");
leg8->AddEntry(grCh1_vdd_T40_pl9_chip4_freq2, "Rad. Tol. 40 deg.","p");
leg8->AddEntry(grCh1_vdd_T40_pl10_chip4_freq2,"Standard. 40 deg.","p");
leg8->AddEntry(grCh1_vdd_T20_pl9_chip4_freq2, "Rad. Tol. 20 deg.","p");
leg8->AddEntry(grCh1_vdd_T20_pl10_chip4_freq2,"Standard. 20 deg.","p");
leg8->SetFillStyle(0);
leg8->Draw();



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++











ccomp2->cd(0);
 cout<<"....END"<<endl;






//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}// END
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++






