// .x code/macros/computediffetavalues.C
{
  Double_t Pitch = 1.0;
  //  Int_t nBITADC = 2;
  Int_t different_values;
  Double_t Eta_value, denominateur;
  Double_t Error = 0.0001;
  Int_t compteur;
  Double_t tableau_valeurs[265000];
  //---------
  for(Int_t nBITADC = 1; nBITADC<8; nBITADC++){

  //---debut
  compteur = 0;
  different_values =  TMath::Power(2,nBITADC);
  for(Int_t i=0; i<different_values; i++){
    for(Int_t j=0; j<different_values; j++){
      for(Int_t k=0; k<different_values; k++){
	//test sur la forme du cluster
	if((j>=i)&&(j>=k)){
	  //calcul eta value:
	  denominateur = float(i+j+k);
	  //test denominateur:
	  if(denominateur!=0.0){
	    Eta_value = float(i-k) / denominateur;
	    //   cout<<"--- i j k eta "<<i<<" "<<j<<" "<<k<<" "<<Eta_value<<endl;;
	    
	    //check if it is a new value:
	    Int_t newvalueok = 1;
	    for(Int_t m=0; m<compteur; m++){
	      //    cout<<"Eta_value tableau_valeurs[m] "<<Eta_value<<" "<<tableau_valeurs[m] <<" "<< (Eta_value - tableau_valeurs[m]) 
	      //	  <<" "<<  TMath::Abs(Eta_value - tableau_valeurs[m]) <<endl;
	      if(TMath::Abs(Eta_value - tableau_valeurs[m] )< Error){newvalueok = 0;}
	    }

	    //  cout<<"--- ok "<<newvalueok<<endl;
	    // if it is a new value, store this new value, increase compteur:
	    if(newvalueok==1){
	      tableau_valeurs[compteur] = Eta_value;
	      compteur++;
	    }
	  } //(denominateur!=0.0)
	} // (j>=i)&&(j>=k)
      } //loop k
    } //loop j
  } //loop i

  /*  for(Int_t i=0; i<compteur; i++){
    cout<<"i eta "<<i<<" "<<tableau_valeurs[i]<<endl;
  }
  */

  cout<<"nBITADC = "<<nBITADC<<" -> DIFFERENT VALUES = "<<compteur<<endl;
}


}
/*
nBITADC = 1 -> DIFFERENT VALUES = 3
nBITADC = 2 -> DIFFERENT VALUES = 17
nBITADC = 3 -> DIFFERENT VALUES = 89
nBITADC = 4 -> DIFFERENT VALUES = 407
nBITADC = 5 -> DIFFERENT VALUES = 1743
nBITADC = 6 -> DIFFERENT VALUES = 4935
*/
