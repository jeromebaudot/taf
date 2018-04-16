{
// a.besson october 2005.
// .x code/macros/compareM8.C
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const Int_t Nrun = 6;
const Int_t Nsub = 4;

Int_t ChipArray[Nrun] = {2,2,2,2,2,2};
Int_t RunArray[Nrun] = {8511,8509,8508,8507,8506,8504};
Float_t ThresholdArray[Nrun] = {3.5, 4, 5, 6, 7, 8};
Float_t ThError[Nrun] = {0,0,0,0,0,0};
Float_t TempArray[Nrun] = {20,20,20,20,20,20};
Float_t NumberOf_1_goodArray[Nrun][Nsub];
Float_t NumberOf_1_goodsub2[Nrun];
Float_t NumberOf_1_goodsub3[Nrun];
Float_t NumberOf_1_goodsub4[Nrun];

Float_t NhitRateperpixel_sub2[Nrun];
Float_t NhitRateperpixel_sub3[Nrun];
Float_t NhitRateperpixel_sub4[Nrun];

Float_t Nhitperpixel_sub2[Nrun];
Float_t Nhitperpixel_sub3[Nrun];
Float_t Nhitperpixel_sub4[Nrun];


Float_t EfficiencyArray_sub2[Nrun] = {99.067, 98.242, 94.491, 89.516, 82.271, 72.169
};
Float_t EffErrorArray_sub2[Nrun] = {0.140, 0.188, 0.346, 0.444, 0.561, 0.373
}; 

Float_t EfficiencyArray_sub3[Nrun] = {99.311, 98.958, 95.339, 92.206, 84.851, 77.011
}; 
Float_t EffErrorArray_sub3[Nrun] = {0.123, 0.156, 0.317, 0.405, 0.538, 0.368
}; 

Float_t EfficiencyArray_sub4[Nrun] = {99.460, 98.800, 95.420, 91.000, 84.961, 77.008
}; 
Float_t EffErrorArray_sub4[Nrun] = {0.112, 0.164, 0.329, 0.451, 0.542, 0.354
}; 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Float_t SigmaArray[Nrun];
for (Int_t i=0 ; i<Nrun ; i++) {
  SigmaArray[i] = 1.0 + 0.9 *  ThresholdArray[i];

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp = new TCanvas("ccomp","Mimosa 8 digital",550,10,700,800);
ccomp->Draw();
ccomp->Clear();
ccomp->SetFillColor(0);
ccomp->SetBorderMode(0);
ccomp->Divide(2,2);

//gPad->cd();


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TH1F *hM8_NumberOf_1_goodhit[Nrun][Nsub];
TH1F *hM8_sub2_NhitRateperpixel[Nrun];
TH1F *hM8_sub3_NhitRateperpixel[Nrun];
TH1F *hM8_sub4_NhitRateperpixel[Nrun];
TH1F *hM8_sub2_Nhitperpixel[Nrun]; 
TH1F *hM8_sub3_Nhitperpixel[Nrun]; 
TH1F *hM8_sub4_Nhitperpixel[Nrun];
TH1F *hnGOODhit[Nrun];


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*TH1F *hEff[Nsub];
Char_t titre1[50] ; 
Char_t nom1[50]; 

for (Int_t i=0 ; i<Nsub ; i++) {
  sprintf(nom1,"eff_sub%d",i+1); 
  sprintf(titre1,"Efficiency sub %d",i+1);  // ??
  hEff[i]=new TH1F (nom1,titre1,105,-100.0,2000.0); 
  sprintf(nom1,"etal1%d",i+1); 
  sprintf(titre1,"Charge couronne/totale - histo %d",i+1); 
  etal1[i]=new TH1F (nom1,titre1,20,0.0,3.0); 
}

TH1F * = new TH1F("hQofPix3x3","Pixel charge in 3x3",50,0,500);
    TH1F *etal[2] ;
*/


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(1);

TH2F *heffFrame = new TH2F("heffFrame","M8 digital. Efficiency (%) vs Discri threshold (mV)",20,3,9,128,70,102);
heffFrame->SetStats(0000000);
heffFrame->SetXTitle("Discri. Threshold (mV)");
heffFrame->SetYTitle("Efficiency (%)");
heffFrame->Draw();


grsub2 = new TGraphErrors(Nrun,ThresholdArray,EfficiencyArray_sub2,ThError,EffErrorArray_sub2);
//gr->SetTitle("TGraphErrors Example");
grsub2->SetMarkerColor(4);
grsub2->SetMarkerStyle(21);
grsub2->SetMarkerSize(0.5);
grsub2->SetLineColor(4);
grsub2->Draw("LP");

grsub3 = new TGraphErrors(Nrun,ThresholdArray,EfficiencyArray_sub3,ThError,EffErrorArray_sub3);
//gr->SetTitle("TGraphErrors Example");
grsub3->SetMarkerColor(2);
grsub3->SetMarkerStyle(21);
grsub3->SetMarkerSize(0.5);
grsub3->SetLineColor(2);
grsub3->Draw("LP");

grsub4 = new TGraphErrors(Nrun,ThresholdArray,EfficiencyArray_sub4,ThError,EffErrorArray_sub4);
//gr->SetTitle("TGraphErrors Example");
grsub4->SetMarkerColor(3);
grsub4->SetMarkerStyle(21);
grsub4->SetMarkerSize(0.5);
grsub4->SetLineColor(3);
grsub4->Draw("LP");

leg1 = new TLegend(0.15,0.25,0.5,0.5);
leg1->SetHeader("Diode size");
leg1->AddEntry(grsub2,"1.2 x 1.2 um^2","l");
leg1->AddEntry(grsub3,"1.7 x 1.7 um^2","l");
leg1->AddEntry(grsub4,"2.4 x 2.4 um^2","l");
leg1->SetFillStyle(0);
leg1->Draw();

/*TPaveText pt1(0.85,0.5,0.88,0.9,"NDC");
pt1.SetFillColor(0);
pt1.SetBo
TText *tt1=pt1.AddText("IReS");
tt1->SetTextSize(0.03);
tt1.SetTextColor(1);
tt1.SetTextAngle(90);
pt1->Draw();
*/

tt1 = new TText(0.88,0.5,"IReS-Strasbourg. Besson");
tt1->SetNDC(kTRUE);
tt1->SetTextSize(.03);
tt1->SetTextColor(1);
tt1->SetTextAngle(90);
tt1->Draw();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(2);

TH2F *heffFrame2 = new TH2F("heffFrame2","M8 digital. Efficiency (%) vs S/N cut",20,3,9,128,70,102);
heffFrame2->SetStats(0000000);
heffFrame2->SetXTitle("Discri. S/N cut");
heffFrame2->SetYTitle("Efficiency (%)");
heffFrame2->Draw();


grsigsub2 = new TGraphErrors(Nrun,SigmaArray,EfficiencyArray_sub2,ThError,EffErrorArray_sub2);
//gr->SetTitle("TGraphErrors Example");
grsigsub2->SetMarkerColor(4);
grsigsub2->SetMarkerStyle(21);
grsigsub2->SetMarkerSize(0.5);
grsigsub2->SetLineColor(4);
grsigsub2->Draw("LP");

grsigsub3 = new TGraphErrors(Nrun,SigmaArray,EfficiencyArray_sub3,ThError,EffErrorArray_sub3);
//gr->SetTitle("TGraphErrors Example");
grsigsub3->SetMarkerColor(2);
grsigsub3->SetMarkerStyle(21);
grsigsub3->SetMarkerSize(0.5);
grsigsub3->SetLineColor(2);
grsigsub3->Draw("LP");

grsigsub4 = new TGraphErrors(Nrun,SigmaArray,EfficiencyArray_sub4,ThError,EffErrorArray_sub4);
//gr->SetTitle("TGraphErrors Example");
grsigsub4->SetMarkerColor(3);
grsigsub4->SetMarkerStyle(21);
grsigsub4->SetMarkerSize(0.5);
grsigsub4->SetLineColor(3);
grsigsub4->Draw("LP");

leg1->Draw();

tt1->Draw();



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
for (Int_t isub=2 ; isub<Nsub+1 ; isub++){
  for (Int_t i=0 ; i<Nrun ; i++){
    Char_t  ResultFileName[150];
    // 8*_pl9_Sub*_Seed4_Neig-2_geomat3_T2h150_HitMap1
    //sprintf(ResultFileName,"Results/8504_pl9_Sub2_Seed4_Neig-2_geomat3_T2h150_HitMap1/AllPlots_8504_9.root");
    sprintf(ResultFileName,"Results/RESULTS_M8_CERN_NOV2005/%d_pl9_Sub%d_Seed4_Neig-2_geomat3_T2h150_HitMap1/AllPlots_%d_9.root",
	    RunArray[i],isub,RunArray[i]);
    TFile *ResultRootFile=new TFile(ResultFileName,"read");
    //    ResultRootFile->ls();
    
    //     TShape *shape = (TShape*)f.Get(shape_name);
//cM8 = new TCanvas("cM8","Mimosa 8 digital",550,10,750,850);
    //TCanvas *cM8 = (*TCanvas)ResultRootFile.Get("cM8");
    TCanvas *cM8 = (TCanvas*)ResultRootFile.Get("cM8");
 
    TPad *mpad3 = (TPad*)cM8->GetPrimitive("mpad3");
    hM8_NumberOf_1_goodhit[i][isub-1]  = (TH1F*)mpad3->GetPrimitive("hM8_NumberOf_1_goodhit");
    //    cout<<" ENTRIES "<<hM8_NumberOf_1_goodhit[i][isub-1]->GetEntries();

    Char_t  ResultFileName2[150];
    sprintf(ResultFileName2,"Results/RESULTS_M8_CERN_NOV2005/%d_pl9_Sub4_Seed4_Neig-2_geomat4_T2h10000_HitMap1/AllPlots_%d_9.root",
	   RunArray[i],RunArray[i]);
    TFile *ResultRootFile2=new TFile(ResultFileName2,"read");
    TCanvas *cM82 = (TCanvas*)ResultRootFile2.Get("cM8");
    TCanvas *c4 = (TCanvas*)ResultRootFile2.Get("c4");
   
    TPad *mpad42 = (TPad*)cM82->GetPrimitive("mpad4");
    TPad *mpad52 = (TPad*)cM82->GetPrimitive("mpad5");
    TPad *mpad62 = (TPad*)cM82->GetPrimitive("mpad6");
    TPad *mpad72 = (TPad*)cM82->GetPrimitive("mpad7");
    TPad *mpad82 = (TPad*)cM82->GetPrimitive("mpad8");
    TPad *mpad92 = (TPad*)cM82->GetPrimitive("mpad9");
    TPad *mpad2_4 = (TPad*)c4->GetPrimitive("mpad2_4");

    //   TH1F *hM8_NumberOf_1_goodhit = (TH1F*)mpad3->GetPrimitive("hM8_NumberOf_1_goodhit");


    hnGOODhit[i] = (TH1F*)mpad2_4->GetPrimitive("hnGOODhit");
 

    //    TH1F *hM8_NumberOf_1_goodhit = (TH1F*)cM8->FindObject("hM8_NumberOf_1_goodhit");

    //    TH1F *hM8_NumberOf_1_goodhit = (TH1F*)cM8->GetListOfFunctions()->FindObject("hM8_NumberOf_1_goodhit");
    //TPaveStats *st2 = (TPaveStats*)ChargeSpread_2->GetListOfFunctions()->FindObject("stats");
    //st2->Delete();
    //----------------------------------
    //MyClass *obj = (Myclass*)pad->GetPrimitive(objectname);
    //   TH1F *hM8_NumberOf_1_goodhit = (TH1F*)cM8->GetPrimitive("hM8_NumberOf_1_goodhit");

    //    TH1F *hM8_NumberOf_1_goodhit = (TH1F*)ResultRootFile.Get("hM8_NumberOf_1_goodhit");

    NumberOf_1_goodArray[i][isub-1] = hM8_NumberOf_1_goodhit[i][isub-1]->GetMean();


    if(isub==2){
      cout<<ThresholdArray[i]<<" "<<NumberOf_1_goodArray[i][isub-1]<<" "<<EfficiencyArray_sub2[i]<<" "<<endl;
      NumberOf_1_goodsub2[i]=NumberOf_1_goodArray[i][isub-1];
      hM8_sub2_NhitRateperpixel[i] = (TH1F*)mpad72->GetPrimitive("hM8_sub2_NhitRateperpixel");
      //      cout<<" MEAN 1 " <<hM8_sub2_NhitRateperpixel[i]->GetMean()<<endl;
      hM8_sub2_Nhitperpixel[i] = (TH1F*)mpad42->GetPrimitive("hM8_sub2_Nhtiperpixel");
      NhitRateperpixel_sub2[i] = hM8_sub2_NhitRateperpixel[i]->GetMean();
      //      cout<<" MEAN" <<hM8_sub2_Nhitperpixel[i]->GetMean()<<endl;
      Nhitperpixel_sub2[i] = (hM8_sub2_Nhitperpixel[i]->GetMean() - 0.5) /  float(hnGOODhit[i]->GetEntries());
    }else if(isub==3){
      cout<<ThresholdArray[i]<<" "<<NumberOf_1_goodArray[i][isub-1]<<" "<<EfficiencyArray_sub3[i]<<" "<<endl;
      NumberOf_1_goodsub3[i]=NumberOf_1_goodArray[i][isub-1];
      hM8_sub3_NhitRateperpixel[i] = (TH1F*)mpad82->GetPrimitive("hM8_sub3_NhitRateperpixel");
      hM8_sub3_Nhitperpixel[i] = (TH1F*)mpad52->GetPrimitive("hM8_sub3_Nhtiperpixel");
      NhitRateperpixel_sub3[i] = hM8_sub3_NhitRateperpixel[i]->GetMean();
      Nhitperpixel_sub3[i] = (hM8_sub3_Nhitperpixel[i]->GetMean() - 0.5) /  float(hnGOODhit[i]->GetEntries());
    }else if(isub==4){
      cout<<ThresholdArray[i]<<" "<<NumberOf_1_goodArray[i][isub-1]<<" "<<EfficiencyArray_sub4[i]<<" "<<endl;
      NumberOf_1_goodsub4[i]=NumberOf_1_goodArray[i][isub-1];
      hM8_sub4_NhitRateperpixel[i] = (TH1F*)mpad92->GetPrimitive("hM8_sub4_NhitRateperpixel");
      hM8_sub4_Nhitperpixel[i] = (TH1F*)mpad62->GetPrimitive("hM8_sub4_Nhtiperpixel");
      NhitRateperpixel_sub4[i] = hM8_sub4_NhitRateperpixel[i]->GetMean();
      Nhitperpixel_sub4[i] = (hM8_sub4_Nhitperpixel[i]->GetMean() - 0.5) /  float(hnGOODhit[i]->GetEntries());
    }

    ResultRootFile.Close();
    delete ResultRootFile;
    ResultRootFile2.Close();
    delete ResultRootFile2;

  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(3);

TH2F *hmultFrame = new TH2F("hmultFrame","M8 digital. <Hit multiplicity>  vs Threshold",20,3,9,100,1.5,4);
hmultFrame->SetStats(0000000);
hmultFrame->SetXTitle("Discri Threshold (mV)");
hmultFrame->SetYTitle("average hit multiplicity");
hmultFrame->Draw();


grmultsub2 = new TGraphErrors(Nrun,ThresholdArray,NumberOf_1_goodsub2,ThError,ThError);
//gr->SetTitle("TGraphErrors Example");
grmultsub2->SetMarkerColor(4);
grmultsub2->SetMarkerStyle(21);
grmultsub2->SetMarkerSize(0.5);
grmultsub2->SetLineColor(4);
grmultsub2->Draw("LP");

grmultsub3 = new TGraphErrors(Nrun,ThresholdArray,NumberOf_1_goodsub3,ThError,ThError);
//gr->SetTitle("TGraphErrors Example");
grmultsub3->SetMarkerColor(2);
grmultsub3->SetMarkerStyle(21);
grmultsub3->SetMarkerSize(0.5);
grmultsub3->SetLineColor(2);
grmultsub3->Draw("LP");

grmultsub4 = new TGraphErrors(Nrun,ThresholdArray,NumberOf_1_goodsub4,ThError,ThError);
//gr->SetTitle("TGraphErrors Example");
grmultsub4->SetMarkerColor(3);
grmultsub4->SetMarkerStyle(21);
grmultsub4->SetMarkerSize(0.5);
grmultsub4->SetLineColor(3);
grmultsub4->Draw("LP");

leg2 = new TLegend(0.45,0.60,0.80,0.85);
leg2->SetHeader("Diode size");
leg2->AddEntry(grsub2,"1.2 x 1.2 um^2","l");
leg2->AddEntry(grsub3,"1.7 x 1.7 um^2","l");
leg2->AddEntry(grsub4,"2.4 x 2.4 um^2","l");
leg2->SetFillStyle(0);
leg2->Draw();
tt1->Draw();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp->cd(4);

TH2F *hmultFrame = new TH2F("hmultFrame","M8 digital. <Hit multiplicity>  vs S/N cut",20,3,9,100,1.5,4);
hmultFrame->SetStats(0000000);
hmultFrame->SetXTitle("Discri S/N cut");
hmultFrame->SetYTitle("average hit multiplicity");
hmultFrame->Draw();


grmultsub2 = new TGraphErrors(Nrun,SigmaArray,NumberOf_1_goodsub2,ThError,ThError);
//gr->SetTitle("TGraphErrors Example");
grmultsub2->SetMarkerColor(4);
grmultsub2->SetMarkerStyle(21);
grmultsub2->SetMarkerSize(0.5);
grmultsub2->SetLineColor(4);
grmultsub2->Draw("LP");

grmultsub3 = new TGraphErrors(Nrun,SigmaArray,NumberOf_1_goodsub3,ThError,ThError);
//gr->SetTitle("TGraphErrors Example");
grmultsub3->SetMarkerColor(2);
grmultsub3->SetMarkerStyle(21);
grmultsub3->SetMarkerSize(0.5);
grmultsub3->SetLineColor(2);
grmultsub3->Draw("LP");

grmultsub4 = new TGraphErrors(Nrun,SigmaArray,NumberOf_1_goodsub4,ThError,ThError);
//gr->SetTitle("TGraphErrors Example");
grmultsub4->SetMarkerColor(3);
grmultsub4->SetMarkerStyle(21);
grmultsub4->SetMarkerSize(0.5);
grmultsub4->SetLineColor(3);
grmultsub4->Draw("LP");

leg2 = new TLegend(0.45,0.60,0.80,0.85);
leg2->SetHeader("Diode size");
leg2->AddEntry(grsub2,"1.2 x 1.2 um^2","l");
leg2->AddEntry(grsub3,"1.7 x 1.7 um^2","l");
leg2->AddEntry(grsub4,"2.4 x 2.4 um^2","l");
leg2->SetFillStyle(0);
leg2->Draw();
tt1->Draw();


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp2 = new TCanvas("ccomp2","Mimosa 8 digital (2)",550,10,700,800);
ccomp2->Draw();
ccomp2->Clear();
ccomp2->SetFillColor(0);
ccomp2->SetBorderMode(0);
ccomp2->Divide(1,3);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

leg3 = new TLegend(0.5,0.50,0.80,0.85);
leg3->SetHeader("Discri Thrshold (mV)");

Int_t temppad = 0;
for (Int_t isub=2 ; isub<Nsub+1 ; isub++){
  temppad++;
  ccomp2->cd(temppad);
  for (Int_t i=0 ; i<Nrun ; i++){    
    hM8_NumberOf_1_goodhit[i][isub-1]->SetStats(0000000);
    Float_t integ = hM8_NumberOf_1_goodhit[i][isub-1]->Integral();
    hM8_NumberOf_1_goodhit[i][isub-1]->Scale(1/integ);
    hM8_NumberOf_1_goodhit[i][isub-1]->SetXTitle("Hit multiplicity");
    hM8_NumberOf_1_goodhit[i][isub-1]->SetYTitle("N entries");
    hM8_NumberOf_1_goodhit[i][isub-1]->SetFillColor(0);
    switch(isub){
    case 2:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetTitle("Hit multiplicity for different thresholds (1.2 x 1.2 um^2 diode)");
      break;
    case 3:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetTitle("Hit multiplicity for different thresholds (1.7 x 1.7 um^2 diode)");      
      break;
    case 4:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetTitle("Hit multiplicity for different thresholds (2.4 x 2.4 um^2 diode)");
    default:      
      break;
    }
    hM8_NumberOf_1_goodhit[i][isub-1]->SetLineStyle(2);
    hM8_NumberOf_1_goodhit[i][isub-1]->SetLineWidth(3);
    hM8_NumberOf_1_goodhit[i][isub-1]->SetAxisRange(0,15,"X");
    switch(i){
    case 0:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetLineColor(1);
      break;
    case 1:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetLineColor(2);
      break;
    case 2:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetLineColor(3);
      break;
    case 3:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetLineColor(4);
      break;
    case 4:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetLineColor(7);
      break;
    case 5:
      hM8_NumberOf_1_goodhit[i][isub-1]->SetLineColor(6);
      break;
    default:      
      break;
    }

    Char_t chth[50]; 
    sprintf(chth,"%3.1f",ThresholdArray[i]);
    if(isub==2){
      leg3->AddEntry(hM8_NumberOf_1_goodhit[i][2],chth,"l");
    }
    
    if(i==0){
      hM8_NumberOf_1_goodhit[i][isub-1]->Draw();
      hM8_NumberOf_1_goodhit[i][isub-1]->SetMaximum(0.8);
    }else{
      hM8_NumberOf_1_goodhit[i][isub-1]->Draw("same");
    }
  } //end of for (Int_t i=0 ; i<Nrun ; i++) loop


leg3->SetFillStyle(0);
leg3->Draw();
tt1->Draw();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp3 = new TCanvas("ccomp3","Mimosa 8 digital (3)",550,10,700,800);
ccomp3->Draw();
ccomp3->Clear();
ccomp3->SetFillColor(0);
ccomp3->SetBorderMode(0);
ccomp3->Divide(1,2);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp3->cd(1);
//      hM8_sub2_NhitRateperpixel[i] = (TH1F*)mpad7->GetPrimitive("hM8_sub2_NhitRateperpixel");;

TH2F *hNhitRate = new TH2F("hNhitRate","M8 digital. Max fake hit rate per pixel vs Threshold",20,3,9,2000,0.0000005,0.01);
hNhitRate->SetStats(0000000);
hNhitRate->SetXTitle("Discri Threshold (mV)");
hNhitRate->SetYTitle("Average hit rate per pixel/event");
ccomp3_1->SetLogy(1);
hNhitRate->Draw();


grHitRatesub2 = new TGraphErrors(Nrun,ThresholdArray,Nhitperpixel_sub2,ThError,ThError);
grHitRatesub2->SetMarkerColor(4);
grHitRatesub2->SetMarkerStyle(21);
grHitRatesub2->SetMarkerSize(0.5);
grHitRatesub2->SetLineColor(4);
grHitRatesub2->Draw("LP");

grHitRatesub3 = new TGraphErrors(Nrun,ThresholdArray,Nhitperpixel_sub3,ThError,ThError);
grHitRatesub3->SetMarkerColor(2);
grHitRatesub3->SetMarkerStyle(21);
grHitRatesub3->SetMarkerSize(0.5);
grHitRatesub3->SetLineColor(2);
grHitRatesub3->Draw("LP");

grHitRatesub4 = new TGraphErrors(Nrun,ThresholdArray,Nhitperpixel_sub4,ThError,ThError);
grHitRatesub4->SetMarkerColor(3);
grHitRatesub4->SetMarkerStyle(21);
grHitRatesub4->SetMarkerSize(0.5);
grHitRatesub4->SetLineColor(3);
grHitRatesub4->Draw("LP");
tt1->Draw();
leg2->Draw();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ccomp3->cd(2);
//      hM8_sub2_NhitRateperpixel[i] = (TH1F*)mpad7->GetPrimitive("hM8_sub2_NhitRateperpixel");;

TH2F *hNhitRateSN = new TH2F("hNhitRateSN","M8 digital. Max fake hit rate per pixel vs Threshold",20,3,9,2000,0.0000005,0.01);
hNhitRateSN->SetStats(0000000);
hNhitRateSN->SetXTitle("Discri. S/N cut");
hNhitRateSN->SetYTitle("Average hit rate per pixel/event");
ccomp3_2->SetLogy(1);
hNhitRateSN->Draw();


grHitRateSNsub2 = new TGraphErrors(Nrun,SigmaArray,Nhitperpixel_sub2,ThError,ThError);
grHitRateSNsub2->SetMarkerColor(4);
grHitRateSNsub2->SetMarkerStyle(21);
grHitRateSNsub2->SetMarkerSize(0.5);
grHitRateSNsub2->SetLineColor(4);
grHitRateSNsub2->Draw("LP");

grHitRateSNsub3 = new TGraphErrors(Nrun,SigmaArray,Nhitperpixel_sub3,ThError,ThError);
grHitRateSNsub3->SetMarkerColor(2);
grHitRateSNsub3->SetMarkerStyle(21);
grHitRateSNsub3->SetMarkerSize(0.5);
grHitRateSNsub3->SetLineColor(2);
grHitRateSNsub3->Draw("LP");

grHitRateSNsub4 = new TGraphErrors(Nrun,SigmaArray,Nhitperpixel_sub4,ThError,ThError);
grHitRateSNsub4->SetMarkerColor(3);
grHitRateSNsub4->SetMarkerStyle(21);
grHitRateSNsub4->SetMarkerSize(0.5);
grHitRateSNsub4->SetLineColor(3);
grHitRateSNsub4->Draw("LP");
tt1->Draw();
leg2->Draw();
/*
ccomp->Print("Results/M8_summary/M8dig_eff.eps","eps");
ccomp2->Print("Results/M8_summary/M8dig_mult.eps","eps");
ccomp3->Print("Results/M8_summary/M8dig_fake.eps","eps");
ccomp->Print("Results/M8_summary/M8dig_eff.gif","gif");
ccomp2->Print("Results/M8_summary/M8dig_mult.gif","gif");
ccomp3->Print("Results/M8_summary/M8dig_fake.gif","gif");
*/
cout<<" DONE"<<endl;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

