{
// a.besson april 2006.
// .x code/macros/resolution2.C
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// fixed planes:3,4,5,6  mobile planes:1,2,7,8
// H/U planes: 1,3,5,7 V/V planes: 2,4,6,8
/*
//----------------------
// V planes:
//----------------------
Float_t Res_F15_M3 = 3.0;
Float_t Res_F15_M7 = 3.0;
Float_t Res_F37_M1 = 3.0;
Float_t Res_F37_M5 = 3.0;

//----------------------
// H planes:
//----------------------
Float_t Res_F26_M4 = 3.5;
Float_t Res_F26_M8 = 3.0;
Float_t Res_F48_M2 = 3.0;
Float_t Res_F48_M6 = 3.0;
Float_t ResA = 0.0;
Float_t ResB = 0.0;

//----------------------
const Int_t NPlanes = 8;
Float_t tZ[NPlanes] = {95.5,93.5,84.5,86.5,-84.5,-86.5,-95.5,-93.5};
Float_t RESO[NPlanes] = {0,0,0,0,0,0,0,0};



Float_t az2 =  0.0;
Float_t bz2 =  0.0;
Float_t cz2 =  0.0;
Float_t dz2 =  0.0;
Float_t zz2 =  0.0;

//----------------------
// Resolution
//----------------------

// - F26 M48:
zz2 = tZ[6-1] - tZ[2-1] ;
zz2 *= zz2;
az2 = tZ[6-1] - tZ[4-1] ;
az2 *= az2;
az2 = az2 / zz2;
bz2 = tZ[2-1] - tZ[4-1] ;
bz2 *= bz2;
bz2 = bz2 / zz2;
cz2 = tZ[6-1] - tZ[8-1] ;
cz2 *= cz2;
cz2 = cz2 / zz2;
dz2 = tZ[2-1] - tZ[8-1] ;
dz2 *= dz2;
dz2 = dz2 / zz2;
ResA = Res_F26_M4;
ResB = Res_F26_M8;
RESO[6-1] = sqrt( (ResB*ResB  - cz2 / az2 * ResA*ResA) / ( -bz2*cz2/az2  + dz2 ) ) ;
RESO[2-1] = sqrt( ( ResA*ResA - bz2*RESO[6-1]*RESO[6-1] ) / az2 );

cout<< Res_F26_M4*Res_F26_M4 <<" = "<< az2 * RESO[2-1]*RESO[2-1] + bz2*RESO[6-1]*RESO[6-1]<<endl;
cout<< Res_F26_M4*Res_F26_M4 <<" = "<< az2 * RESO[2-1]*RESO[2-1] <<" + "<< bz2*RESO[6-1]*RESO[6-1]<<endl;
cout<< Res_F26_M4*Res_F26_M4 <<" = "<< az2 <<" * "<< RESO[2-1]*RESO[2-1] <<" + "<< bz2<<" * "<<RESO[6-1]*RESO[6-1]<<endl;
cout<< Res_F26_M4*Res_F26_M4 <<" = "<< az2 <<" * "<< RESO[2-1]<<" * "<<RESO[2-1] <<" + "<< bz2<<" * "<<RESO[6-1]<<" * "<<RESO[6-1]<<endl;
cout<< tZ[2-1] - tZ[4-1]<<endl;
cout<< (tZ[2-1] - tZ[4-1] )* (tZ[2-1] - tZ[4-1] )<<endl;
cout<< (tZ[2-1] - tZ[4-1] )* (tZ[2-1] - tZ[4-1] ) / zz2<<endl;


// - F48 M26:
zz2 = tZ[8-1] - tZ[4-1] ;
zz2 *= zz2;
az2 = tZ[8-1] - tZ[2-1] ;
az2 *= az2;
az2 = az2 / zz2;
bz2 = tZ[4-1] - tZ[2-1] ;
bz2 *= bz2;
bz2 = bz2 / zz2;
cz2 = tZ[8-1] - tZ[6-1] ;
cz2 *= cz2;
cz2 = cz2 / zz2;
dz2 = tZ[4-1] - tZ[6-1] ;
dz2 *= dz2;
dz2 = dz2 / zz2;
ResA = Res_F48_M2;
ResB = Res_F48_M6;
RESO[8-1] = sqrt( (ResB*ResB  - cz2 / az2 * ResA*ResA) / ( -bz2*cz2/az2  + dz2 ) ) ;
RESO[4-1] = sqrt( ( ResA*ResA - bz2*RESO[8-1]*RESO[8-1] ) / az2 );

// - F15 M37:
zz2 = tZ[5-1] - tZ[1-1] ;
zz2 *= zz2;
az2 = tZ[5-1] - tZ[3-1] ;
az2 *= az2;
az2 = az2 / zz2;
bz2 = tZ[1-1] - tZ[3-1] ;
bz2 *= bz2;
bz2 = bz2 / zz2;
cz2 = tZ[5-1] - tZ[7-1] ;
cz2 *= cz2;
cz2 = cz2 / zz2;
dz2 = tZ[1-1] - tZ[7-1] ;
dz2 *= dz2;
dz2 = dz2 / zz2;
ResA = Res_F15_M3;
ResB = Res_F15_M7;
RESO[5-1] = sqrt( (ResB*ResB  - cz2 / az2 * ResA*ResA) / ( -bz2*cz2/az2  + dz2 ) ) ;
RESO[1-1] = sqrt( ( ResA*ResA - bz2*RESO[5-1]*RESO[5-1] ) / az2 );

// - F37 M15:
zz2 = tZ[7-1] - tZ[3-1] ;
zz2 *= zz2;
az2 = tZ[7-1] - tZ[1-1] ;
az2 *= az2;
az2 = az2 / zz2;
bz2 = tZ[3-1] - tZ[1-1] ;
bz2 *= bz2;
bz2 = bz2 / zz2;
cz2 = tZ[7-1] - tZ[5-1] ;
cz2 *= cz2;
cz2 = cz2 / zz2;
dz2 = tZ[3-1] - tZ[5-1] ;
dz2 *= dz2;
dz2 = dz2 / zz2;
ResA = Res_F37_M1;
ResB = Res_F37_M5;
RESO[7-1] = sqrt( (ResB*ResB  - cz2 / az2 * ResA*ResA) / ( -bz2*cz2/az2  + dz2 ) ) ;
RESO[3-1] = sqrt( ( ResA*ResA - bz2*RESO[7-1]*RESO[7-1] ) / az2 );
cout<<"--------------------------------------------"<<endl;
cout<<" fixed planes:3,4,5,6  mobile planes:1,2,7,8"<<endl;
cout<<" H/U planes: 1,3,5,7 V/V planes: 2,4,6,8"<<endl;
for(Int_t i=0 ; i<NPlanes ; i++){
  cout<<" RESO PLAN "<<i+1<<" = "<<RESO[i]<<endl;
}

for(Int_t i=0 ; i<NPlanes ; i++){
  tZ[i] = tZ[i]*0.0001;
}
*/

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------
const Int_t NPlanes = 8;
Float_t tZ[NPlanes] = {95.5,93.5,84.5,86.5,-84.5,-86.5,-95.5,-93.5};
Float_t RESO[NPlanes] = {0,0,0,0,0,0,0,0};

//----------------------
// H planes:
//----------------------
Float_t RES_F28_M4 = 3.528;
Float_t RES_F28_M6 = 2.902;
Float_t RES_F26_M4 = 2.925;
Float_t RES_F48_M6 = 2.831;
//RES_F28_M4= 3.0;
//RES_F28_M6= 3.0;
//RES_F26_M4= 3.0;
//RES_F48_M6= 3.0;
//----------------------
// V planes:
//----------------------
Float_t RES_F17_M3 = 3.208;
Float_t RES_F17_M5 = 3.508;
Float_t RES_F15_M3 = 3.250;
Float_t RES_F37_M5 = 3.537;


Float_t a,b,c,d,e,f,g,h;
a = (tZ[8-1]-tZ[4-1]) *(tZ[8-1]-tZ[4-1]) /(tZ[8-1]-tZ[2-1]) /(tZ[8-1]-tZ[2-1]) ;
b = (tZ[2-1]-tZ[4-1]) *(tZ[2-1]-tZ[4-1]) /(tZ[8-1]-tZ[2-1]) /(tZ[8-1]-tZ[2-1]) ;
c = (tZ[8-1]-tZ[6-1]) *(tZ[8-1]-tZ[6-1]) /(tZ[8-1]-tZ[2-1]) /(tZ[8-1]-tZ[2-1]) ;
d = (tZ[2-1]-tZ[6-1]) *(tZ[2-1]-tZ[6-1]) /(tZ[8-1]-tZ[2-1]) /(tZ[8-1]-tZ[2-1]) ;
e = (tZ[6-1]-tZ[4-1]) *(tZ[6-1]-tZ[4-1]) /(tZ[6-1]-tZ[2-1]) /(tZ[6-1]-tZ[2-1]) ;
f = (tZ[2-1]-tZ[4-1]) *(tZ[2-1]-tZ[4-1]) /(tZ[6-1]-tZ[2-1]) /(tZ[6-1]-tZ[2-1]) ;
g = (tZ[8-1]-tZ[6-1]) *(tZ[8-1]-tZ[6-1]) /(tZ[8-1]-tZ[4-1]) /(tZ[8-1]-tZ[4-1]) ;
h = (tZ[4-1]-tZ[6-1]) *(tZ[4-1]-tZ[6-1]) /(tZ[8-1]-tZ[4-1]) /(tZ[8-1]-tZ[4-1]) ;

Float_t A,B,C,D;
A = RES_F28_M4 * RES_F28_M4; 
B = RES_F28_M6 * RES_F28_M6;
C = RES_F26_M4 * RES_F26_M4;
D = RES_F48_M6 * RES_F48_M6;

Float_t X2;
X2 = ( (D+ a*g/c * (B - A*c/a) )
       * ( f -e/c-a/c )
       - ( 1 + a*g/c )
       * ( C - B*e/c -a/c*(B-A*c/a))
       ) / (
	    ( f-e/c-a/c)
	    * ( h+a*g/c*(d-b*c/a))
	    -(1+a*g/c)
	    *(-e*d/c-a/c*( d-b*c/a))
	    );

  cout<<" X2 "<<X2<<endl;

cout<<"--------------------------------------------"<<endl;

cout<<" a b c d e f g h "<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<e<<" "<<f<<" "<<g<<" "<<h<<endl;

/*
RES_F28_M4
RES_F28_M6
RES_F26_M4
RES_F48_M6
*/

RESO[2-1] =     ( 
		 (
		  (RES_F28_M6*RES_F28_M6)
		  -(RES_F48_M6*RES_F48_M6)
		  +(RES_F28_M4*RES_F28_M4)*g
		  -(d-h+b*g)*((RES_F28_M6*RES_F28_M6) - (RES_F26_M4*RES_F26_M4)/f + (RES_F28_M4*RES_F28_M4)/f )
		  )/
		 ( (c+a*g)-(d-h+b*g)*(c-e/f+a/f) )
		 );



RESO[8-1] =  sqrt( 
		  (RES_F28_M6*RES_F28_M6)
		  -(RES_F48_M6*RES_F48_M6)/f
		  +(RES_F28_M4*RES_F28_M4)/f
		  -(c-e/f+a/f)*RESO[2-1]*RESO[2-1]
		  );

RESO[4-1] =  sqrt( 
		  (RES_F28_M4*RES_F28_M4)
		  -a*RESO[2-1]*RESO[2-1]
		  -b*RESO[8-1]*RESO[8-1]
		  );
RESO[6-1] =  sqrt( 
		  (RES_F28_M6*RES_F28_M6)
		  -c*RESO[2-1]*RESO[2-1]
		  -d*RESO[8-1]*RESO[8-1]
		  );

cout<<"--------------------------------------------"<<endl;
cout<<"APPROX 1"<<endl;
cout<<sqrt( (RES_F28_M4*RES_F28_M4) / (a+b+1) )<<endl;
cout<<sqrt( (RES_F28_M6*RES_F28_M6) / (c+d+1) )<<endl;
cout<<sqrt( (RES_F26_M4*RES_F26_M4) / (e+f+1) )<<endl;
cout<<sqrt( (RES_F48_M6*RES_F48_M6) / (g+h+1) )<<endl;
cout<<"--------------------------------------------"<<endl;
cout<<"APPROX 2"<<endl;
cout<<sqrt( (RES_F28_M4*RES_F28_M4 - RES_F26_M4*RES_F26_M4) / (a+b-e-f) )<<endl;
cout<<"--------------------------------------------"<<endl;

cout<<"(RES_F28_M6*RES_F28_M6) "<<(RES_F28_M6*RES_F28_M6)<<endl;
cout<<"-(RES_F48_M6*RES_F48_M6) "<<-(RES_F48_M6*RES_F48_M6) <<endl;
cout<<"+(RES_F28_M4*RES_F28_M4)*g "<< +(RES_F28_M4*RES_F28_M4)*g<<endl;
cout<<" -(d-h+b*g)*((RES_F28_M6*RES_F28_M6) - (RES_F26_M4*RES_F26_M4)/f + (RES_F28_M4*RES_F28_M4)/f ) "<<  -(d-h+b*g)*((RES_F28_M6*RES_F28_M6) - (RES_F26_M4*RES_F26_M4)/f + (RES_F28_M4*RES_F28_M4)/f )<<endl;

cout<<" numerateur "<< (RES_F28_M6*RES_F28_M6)
		  -(RES_F48_M6*RES_F48_M6)
		  +(RES_F28_M4*RES_F28_M4)*g
		  -(d-h+b*g)*((RES_F28_M6*RES_F28_M6) - (RES_F26_M4*RES_F26_M4)/f + (RES_F28_M4*RES_F28_M4)/f ) <<endl;

cout<<"(c+a*g) "<<(c+a*g) <<endl;
cout<<"-(d-h+b*g)*(c-e/f+a/f) "<<-(d-h+b*g)*(c-e/f+a/f) <<endl;
cout<<" (c+a*g)-(d-h+b*g)*(c-e/f+a/f)  "<< (c+a*g)-(d-h+b*g)*(c-e/f+a/f)  <<endl;

/*
cout<<" "<< <<endl;
cout<<" "<< <<endl;
cout<<" "<< <<endl;
cout<<" "<< <<endl;
cout<<" "<< <<endl;
*/
cout<<"--------------------------------------------"<<endl;

  cout<<" RESO PLAN "<<2<<" = "<<RESO[2-1]<<endl;
  cout<<" RESO PLAN "<<4<<" = "<<RESO[4-1]<<endl;
  cout<<" RESO PLAN "<<6<<" = "<<RESO[6-1]<<endl;
  cout<<" RESO PLAN "<<8<<" = "<<RESO[8-1]<<endl;


const Int_t Npitch = 3;
Float_t TA_resol[Npitch]              = {    1.5,  2.0 , 2.9};
Float_t TA_resolERR[Npitch]              = {    0.1,  0.1 , 0.15};
Float_t TA_pitch[Npitch]              = {  20.0,  30.0 , 40.0};
Float_t TA_pitchERR[Npitch]              = {  0.0,  0.0 , 0.0};


TH2F *hFrame = new TH2F("hFrame","Mimosa 9: resolution vs pitch",30,15,45,100,1.0,3.5);
hFrame->SetStats(0000000);
hFrame->SetXTitle("Pitch (#mum)");
hFrame->SetYTitle("Resolution (#mum)");
hFrame->Draw();

hFrame->GetXaxis()->SetLabelOffset(0.011);
  hFrame->GetYaxis()->SetLabelOffset(0.011);
  hFrame->GetYaxis()->SetLabelSize(0.04);

gr_res_pitch = new TGraphErrors(Npitch,TA_pitch,TA_resol,TA_pitchERR,TA_resolERR);
gr_res_pitch->SetMarkerColor(4);
gr_res_pitch->SetMarkerStyle(21);
gr_res_pitch->SetMarkerSize(0.8);
gr_res_pitch->SetLineColor(4);
gr_res_pitch->Draw("LP");


 Float_t rightmax = 3.5;
 Float_t rightmin = 1.0;
 TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(), gPad->GetUymax(),rightmin,rightmax,510,"+LU");
 // axis->SetLineColor(kRed);
 // axis->SetLabelColor(kRed);
 axis->Draw();
 TGaxis *axisx = new TGaxis(gPad->GetUxmin(),gPad->GetUymax(),gPad->GetUxmax(), gPad->GetUymax(),rightmin,rightmax,510,"-U");
 axisx->Draw();


   // TAxis *xaxis= hFrame->GetXaxis();
   //xaxis


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}// END
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
