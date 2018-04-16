#include <algorithm>

void noise_matrix(){

Short_t debug = 0;

double ped_plane1,ped_plane2,ped_plane3,ped_plane4;
double noise_plane1,noise_plane2,noise_plane3,noise_plane4;
double pixelID = 1;
int ipixel = 1;
int jpixel = 512;

  TH2F *hPedestalpl1  = new TH2F("hPedestalpl1","hPedestalpl1",512,1,512,512,1,512);
  TH2F *hPedestalpl2  = new TH2F("hPedestalpl2","hPedestalpl2",512,1,512,512,1,512);
  TH2F *hPedestalpl3  = new TH2F("hPedestalpl3","hPedestalpl3",512,1,512,512,1,512);
  TH2F *hPedestalpl4  = new TH2F("hPedestalpl4","hPedestalpl4",512,1,512,512,1,512);

  TH1F *hPedDistripl1  = new TH1F("hPedDistripl1","hPedDistripl1",512,-512,512);
  TH1F *hPedDistripl2  = new TH1F("hPedDistripl2","hPedDistripl2",512,-512,512);
  TH1F *hPedDistripl3  = new TH1F("hPedDistripl3","hPedDistripl3",512,-512,512);
  TH1F *hPedDistripl4  = new TH1F("hPedDistripl4","hPedDistripl4",512,-512,512);


  TH2F *hnoisepl1  = new TH2F("hnoisepl1","hnoisepl1",512,1,512,512,1,512);
  TH2F *hnoisepl2  = new TH2F("hnoisepl2","hnoisepl2",512,1,512,512,1,512);
  TH2F *hnoisepl3  = new TH2F("hnoisepl3","hnoisepl3",512,1,512,512,1,512);
  TH2F *hnoisepl4  = new TH2F("hnoisepl4","hnoisepl4",512,1,512,512,1,512);

  TH1F *hNoiseDistripl1  = new TH1F("hNoiseDistripl1","hNoiseDistripl1",512,-512,512);
  TH1F *hNoiseDistripl2  = new TH1F("hNoiseDistripl2","hNoiseDistripl2",512,-512,512);
  TH1F *hNoiseDistripl3  = new TH1F("hNoiseDistripl3","hNoiseDistripl3",512,-512,512);
  TH1F *hNoiseDistripl4  = new TH1F("hNoiseDistripl4","hNoiseDistripl4",512,-512,512);

 // Old inputs
 // /rawcmos12/data_tnt/data_tnt_mimosa5/5012/noise_Card0140/iedestaux_mimosa_05_Card_0140_date_08-Oct-2010.txt
 // /rawcmos17b/mario/mimosa5_tnt/Am/5008/noise_Card0140/piedestaux_mimosa_05_Card_0140_date_08-Oct-2010.txt
  ifstream InputFile_piedestaux("/rawcmos17a/data/mimosa5_tnt_labo/5030/noise_Card0140/piedestaux_mimosa_05_Card_0140_date_15-Oct-2010.txt", ios::in);

  TFile *OutputFile = new TFile("Noise_run5030.root","RECREATE");

  while(InputFile_piedestaux.eof()==0){
	InputFile_piedestaux
		>> ped_plane1
		>> ped_plane2
		>> ped_plane3
		>> ped_plane4
		;

hPedestalpl1->SetBinContent(ipixel,jpixel,ped_plane1);
hPedestalpl2->SetBinContent(ipixel,jpixel,ped_plane2);
hPedestalpl3->SetBinContent(ipixel,jpixel,ped_plane3);
hPedestalpl4->SetBinContent(ipixel,jpixel,ped_plane4);

hPedDistripl1->Fill(ped_plane1);
hPedDistripl2->Fill(ped_plane2);
hPedDistripl3->Fill(ped_plane3);
hPedDistripl4->Fill(ped_plane4);
 if( debug) cout<<pixelID<<"      "<<ipixel<<"       "<<jpixel<<"       "<<ped_plane1<<"     "<<ped_plane2<<"     "<<ped_plane3<<"     "<<ped_plane4<<endl;

	if(ipixel==512 ){
	ipixel = 0;
	jpixel--;
	}
	ipixel++;
	pixelID++;

//   Char_t rootFile[50];
//   sprintf(rootFile,"Noise_run%d.root",tSession->GetRunNumber());
//   cout << "\n-- Saving histos and canvas into " << rootFile << endl;
//   TFile fRoot(rootFile,"RECREATE");
//   cnoise->Write();
//   cpedestal->Write();
//   cdistri->Write();
//   fRoot.Close();

  }
  InputFile_piedestaux.close();

 ipixel = 1;
 jpixel = 512;

 // Old inputs
 // /rawcmos12/data_tnt/data_tnt_mimosa5/5012/noise_Card0140/sigma_mimosa_05_Card_0140_date_08-Oct-2010.txt
 // iedestaux_mimosa_05_Card_0140_date_08-Oct-2010.txt/rawcmos17b/mario/mimosa5_tnt/Am/5008/noise_Card0140/sigma_mimosa_05_Card_0140_date_08-Oct-2010.txt
  ifstream InputFile_noise("/rawcmos17a/data/mimosa5_tnt_labo/5030/noise_Card0140/sigma_mimosa_05_Card_0140_date_15-Oct-2010.txt", ios::in);

  while(InputFile_noise.eof()==0){
	InputFile_noise
		>> noise_plane1
		>> noise_plane2
		>> noise_plane3
		>> noise_plane4
		;

hnoisepl1->SetBinContent(ipixel,jpixel,noise_plane1);
hnoisepl2->SetBinContent(ipixel,jpixel,noise_plane2);
hnoisepl3->SetBinContent(ipixel,jpixel,noise_plane3);
hnoisepl4->SetBinContent(ipixel,jpixel,noise_plane4);

hNoiseDistripl1->Fill(noise_plane1);
hNoiseDistripl2->Fill(noise_plane2);
hNoiseDistripl3->Fill(noise_plane3);
hNoiseDistripl4->Fill(noise_plane4);
 if( debug) cout<<pixelID<<"      "<<ipixel<<"       "<<jpixel<<"       "<<noise_plane1<<"     "<<noise_plane1<<"     "<<noise_plane1<<"     "<<noise_plane1<<endl;

	if(ipixel==512 ){
	ipixel = 0;
	jpixel--;
	}
	ipixel++;
	pixelID++;

  }



InputFile_noise.close();


/*
piedestal = new TCanvas("piedestal_matrix","",1000,600);
piedestal->Divide(2,2);

piedestal->cd(1);
hPedestalpl1->Draw();

piedestal->cd(2);
hPedestalpl2->Draw();

piedestal->cd(3);
hPedestalpl3->Draw();

piedestal->cd(4);
hPedestalpl4->Draw();

piedestal->Write();
*/


piedestal_distri = new TCanvas("piedestal_distri","",1000,600);

piedestal_distri->Divide(2,2);

piedestal_distri->cd(1);
hPedDistripl1->Draw();

piedestal_distri->cd(2);
hPedDistripl2->Draw();

piedestal_distri->cd(3);
hPedDistripl3->Draw();

piedestal_distri->cd(4);
hPedDistripl4->Draw();

piedestal_distri->Write();



/*
noise = new TCanvas("noise_matrix","",1000,600);
noise->Divide(2,2);

noise->cd(1);
hnoisepl1->Draw();

noise->cd(2);
hnoisepl2->Draw();

noise->cd(3);
hnoisepl3->Draw();

noise->cd(4);
hnoisepl4->Draw();

noise->Write();
*/

noise_distri = new TCanvas("noise_matrix_2","",1000,600);
noise_distri->Divide(2,2);

noise_distri->cd(1);
hNoiseDistripl1->Draw();

noise_distri->cd(2);
hNoiseDistripl2->Draw();

noise_distri->cd(3);
hNoiseDistripl3->Draw();

noise_distri->cd(4);
hNoiseDistripl4->Draw();

noise_distri->Write();




hPedestalpl1->Write();
hPedestalpl2->Write();
hPedestalpl3->Write();
hPedestalpl4->Write();

hPedDistripl1->Write();
hPedDistripl2->Write();
hPedDistripl3->Write();
hPedDistripl4->Write();

hNoiseDistripl1->Write();
hNoiseDistripl2->Write();
hNoiseDistripl3->Write();
hNoiseDistripl4->Write();

hnoisepl1->Write();
hnoisepl2->Write();
hnoisepl3->Write();
hnoisepl4->Write();


OutputFile->Write();

 cout << endl << "Histos written in " << OutputFile->GetName() << endl << endl;
}
