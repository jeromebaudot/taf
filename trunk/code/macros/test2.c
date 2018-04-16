{

for(Int_t j= 0; j < 130;j++){for(Int_t i= 0; i < 130; i++){cout<<": "<<j<<" "<<i<<" "<<h2DSeedPixel->GetBinContent(j,i)<<" ";}cout<<endl;}
for(Int_t j= 1; j < 25;j++){for(Int_t i= 1; i < 129; i++){cout<<h2DSeedPixel->GetBinContent(j,i)<<" ";}cout<<endl;}
for(Int_t i= 1; i < 129; i++){for(Int_t j= 1; j < 25;j++){cout<<h2DSeedPixel->GetBinContent(j,i)<<" ";}cout<<endl;}

}
