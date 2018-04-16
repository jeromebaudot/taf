// ADC conversion tests.
// A.Besson may 2006
//   .x code/macros/ADC_test.C
{
  Float_t Converted_output;
  Float_t Input_number1 ;
  Int_t nbits_in_ADC = 4;
  Int_t different_values;
  different_values =  TMath::Power(2,nbits_in_ADC);
  Int_t step_ADC = 8;
  Int_t LSB = 3;
  Int_t Max_input = LSB + (different_values-1) * step_ADC;
 
 
 cout<<" different_values = "<<different_values<<endl;
 const Int_t NofCycle=10;

 for(Int_t i = 0;i<Max_input+10;i++){
   Input_number1 = i;
   if(Input_number1 < LSB ){
     Converted_output = 0;
   }else if(Input_number1>=Max_input){
     Converted_output = different_values - 1; 
   }else{
     Converted_output =  1 + int(Input_number1 - LSB) /step_ADC ; 
   }
   cout<<i<<" "<<Converted_output<<" "<<endl;
   if(i/NofCycle*NofCycle == i){ 
     //  cout<<endl;
   }
}
 cout<<endl;
 cout<<" MAX input"<<Max_input<<endl;

}
