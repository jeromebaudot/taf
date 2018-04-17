//**********************************************************************************//
//Autor: Daniel Cuesta
//Purpose of Class :
//    Holds all the modules needed to perform pattern recognition with PLUME ladders
//          - Fill_Modules_HitList        : Built the list of hits on each module
//          - Particle_Reconstruction     : Call sub methods for pattern recognition depending of the number of hits in modules
//                                          It will return a list of reconstructed particles
//            -- SpatialClustering        : For each hit it will find the closest hit in the module( under a predifined limit)
//                                          and put them together as coming from the same particle
//            -- Superposition            : Same idea but with hits from different modules of the same ladder
//            -- Allignement              : Test the Allignement of hits (helps to reconstruct loopers)
//          - RecoCategorieClassification : Classified particles in the differents hits pattern (take the output list of reconstructed particles from "Particle reconstruction")
//
//**********************************************************************************//
#include "DBeaster.h"
ClassImp(DBeaster)
DBeaster::DBeaster(DTracker *Tracker)
{
  // DBeaster default constructor
  tTracker=Tracker;
  fHelixFitter = new DHelixFitter(Tracker);
}
DBeaster::~DBeaster(){
  return;
}
//===========================================================================================================
DR3 DBeaster::GetHitPositionOnLabRefFrame(int PlaneId,int HitId) {
  //Pour l'instant dans DBeaster mais devras être mis ailleurs
  //Take a Hit position in the Sensor reference frame
  //Define it in the Ladder reference frame

  aPlane=tTracker->GetPlane(PlaneId);
  Dprec=aPlane->GetPrecAlignment();
  LadderNumber=aPlane->GetLadderNumber();
  aHit = aPlane->GetHit(HitId);
  hitPositionPlane=aHit->GetPosition();
  return hitPositionLab=Dprec->TransformHitToTracker(*hitPositionPlane);
}
//===========================================================================================================
DR3 DBeaster::GetHitPositionOnLadderRefFrame(int PlaneId,int HitId) {
  //Pour l'instant dans DBeaster mais devras être mis ailleurs
  //Take a Hit position in the Sensor reference frame
  //Define it in the Ladder reference frame

  aPlane=tTracker->GetPlane(PlaneId);
  Dprec=aPlane->GetPrecAlignment();
  LadderNumber=aPlane->GetLadderNumber();
  aHit = aPlane->GetHit(HitId);
  hitPositionPlane=aHit->GetPosition();
  hitPositionLab=Dprec->TransformHitToTracker(*hitPositionPlane);
  aLadder=tTracker->GetLadder(LadderNumber);
  if (LadderNumber==1) PlaneIdx=PlaneId;
  else PlaneIdx=PlaneId-12;
  return aLadder->PlaneToLadder(*hitPositionPlane,PlaneIdx);
}
//===========================================================================================================
void DBeaster::Clear_List() {
  ListOfBeastHitsOnM1.clear();
  ListOfBeastHitsOnM2.clear();
  ListOfRecoPartsM1_SC.clear();
  ListOfRecoPartsM2_SC.clear();
  ListOfRecoPartsM1_AL.clear();
  ListOfRecoPartsM2_AL.clear();
  ListOfRecoParts_SP.clear();
  ListOfTslCandidat.clear();
  ListOfSimuPartClass.clear();
  ListOfSimuPartIdL1.clear();
  ListOfSimuPartIdL2.clear();
  return;
  }
//===========================================================================================================
void DBeaster::Fill_Modules_HitList(int LadderId){
  int iHitGlobalIdx=0;
  if (LadderId==1) {
    SensorMin=1;
    SensorMax=12;
  }else{
    SensorMin=13;
    SensorMax=24;
  }
  // std::cout << "FILL MODULES HITLIST" << '\n';
  for( int iPlane=SensorMin; iPlane<=SensorMax; iPlane++ ) {
    // loop on planes
    aPlane=tTracker->GetPlane(iPlane);
    if(aPlane->GetHitsN()==0) continue;
    for( Int_t iHit=1; iHit <= aPlane->GetHitsN(); iHit++ ) { // loop on plane hits
      aHit = aPlane->GetHit(iHit);
      // if(aHit->GetStripsInCluster()<=0) continue;
      if(aHit->GetMCHitID()<0) continue;
      if(aPlane->GetLadderNumber()==0) continue;
      ABeastHit BeastHit;
      BeastHit.LadderId               = aPlane->GetLadderNumber();
      BeastHit.SensorId               = iPlane;
      BeastHit.HitId                  = iHit;
      BeastHit.hitPositionLadderUVW   = GetHitPositionOnLadderRefFrame(iPlane,iHit);
      BeastHit.hitPositionLadderUVW(0)= BeastHit.hitPositionLadderUVW(0)/1000.;
      BeastHit.hitPositionLadderUVW(1)= BeastHit.hitPositionLadderUVW(1)/1000.;
      BeastHit.hitPositionLadderUVW(2)= BeastHit.hitPositionLadderUVW(2)/1000.;
      BeastHit.hitPositionLabXYZ      = GetHitPositionOnLabRefFrame(iPlane,iHit);
      BeastHit.McHitId                = aHit->GetMCHitID();
      //Spatial Clustering(SC)
      BeastHit.RecoId_SC  = -1;
      BeastHit.IsRec_SC   = false;
      //Superposition(SP)
      BeastHit.RecoId_SP = -1;
      BeastHit.IsRec_SP  = false;
      BeastHit.RpairedSP = 20000;
      BeastHit.IsFill_SC  = false;
      //Allignement
      BeastHit.RecoId_AL = -1;
      BeastHit.IsRec_AL  = false;
      BeastHit.VpairedAL = 20000;
      if (iPlane<=12) {
      	if(iPlane<=6) {
          BeastHit.ModuleId=1;
          ListOfBeastHitsOnM1.push_back(BeastHit); //Module 1
      	}else{
          BeastHit.ModuleId=2;
          ListOfBeastHitsOnM2.push_back(BeastHit); //Module 2
        }
      }else{
        if(iPlane<=18) {
          BeastHit.ModuleId=1;
          ListOfBeastHitsOnM1.push_back(BeastHit); //Module 1
      	}else{
          BeastHit.ModuleId=2;
          ListOfBeastHitsOnM2.push_back(BeastHit); //Module 2
        }
      }
    }//End loop on Hits
  }//End loop on Planes
  // std::cout << "Hits on M1" <<ListOfBeastHitsOnM1.size() <<'\n';
  // std::cout << "Hits on M2" <<ListOfBeastHitsOnM2.size() <<'\n';
  return;
}
//===========================================================================================================
void DBeaster::SimuCategorieClassification(){
  int NbTslL1=0;
  int NbTslL2=0;
  int iHitNPixels;
  // tTracker     = fSession->GetTracker();
  MCInfoHolder = tTracker->GetMCInfoHolder();

  NbPart=MCInfoHolder->GetNSimParticles();
  // std::cout << "Npart = "<<NbPart << '\n';
  for(int ipart=0;ipart<NbPart;ipart++){

    Module1=false;
    Module2=false;
    Ladder1=false;
    Ladder2=false;
    MoreThanTwoHits=false;
    int LadderIdTest=0;
    int HitIndex = 0;
    int ModuleTest=0;
    int NbHitsTest=MCInfoHolder->GetASimParticle(ipart).NHits;
    // std::cout << "Nb Hits = "<< NbHitsTest << '\n';
    if(NbHitsTest!=0){
      for(int ihit=0;ihit<NbHitsTest;ihit++){
        int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
        HitIndex = FirstHitIdx + ihit;
        iHitNPixels=MCInfoHolder->GetASimHit(FirstHitIdx + ihit).Npixels;
        // std::cout << "iHitNPixels = " << iHitNPixels<< '\n';
        // if(iHitNPixels>0){
          if (MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID<=12) {
            Ladder1=true;
            if (MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID<=6) {
              Module1=true;
            }else{ //end IF Ladder 1 Module 1
              Module2=true;
            }//end IF Ladder 1 Module 2
          }else{ //End if Ladder 1
            Ladder2=true;
            if (MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID<=18) {
              Module1=true;
            }else{ //end IF Ladder 2 Module 1
              Module2=true;
            } //end IF Ladder 2 Module 2
          }//End if Ladder 2
        // }//End if Npixels !=0
      }//end loop on Hits of real parts

    if(Ladder1==true && Ladder2==false){
      LadderIdTest=1;
      ListOfSimuPartIdL1.push_back(ipart);
    }
    if(Ladder1==false && Ladder2==true){
      LadderIdTest=2;
      ListOfSimuPartIdL2.push_back(ipart);
    }
  if(Module1==true && Module2==true){
		if(NbHitsTest>2){ //TWO SIDES LOOPER ELECTRON
			  ListOfSimuPartClass.push_back(4);
			  if(Ladder1){NbTslL1++;}
			  if(Ladder2){NbTslL2++;}
			  //h_cathitLadder1_Real->Fill(4);
	  }
    if(NbHitsTest==2){ //TWO SIDES ELECTRON
		    ListOfSimuPartClass.push_back(2);
        // std::cout << "push_back TS" << '\n';
		    //h_cathitLadder1_Real->Fill(2);
			}
  }else if((Module1==true && Module2==false)||(Module1==false && Module2==true)){
		if(NbHitsTest>1){ //ONE SIDE LOOPER ELECTRON
			//if(MoreThanTwoHits==true) //ONE SIDE LOOPER ELECTRON
		  ListOfSimuPartClass.push_back(3);
		  //h_cathitLadder1_Real->Fill(3);
		}else if(NbHitsTest==1){ //ONE SIDE ELECTRON
		  ListOfSimuPartClass.push_back(1);
		  //h_cathitLadder1_Real->Fill(1);
		}
	}

    }//end If !=0
  }//End Loop On parts
  // h_NbTslL1->Fill(NbTslL1);
  // h_NbTslL2->Fill(NbTslL2);
  return;
}
//===========================================================================================================

void DBeaster::SimuCategorieClassificationNew(){
  // std::cout << "inside SimuCategorieClassification" << '\n';
  int NbTslL1=0;
  int NbTslL2=0;
  int iHitNPixels;
  // tTracker     = fSession->GetTracker();
  MCInfoHolder = tTracker->GetMCInfoHolder();
  NbPart=MCInfoHolder->GetNSimParticles();
  // std::cout << "Nbpart = "<<NbPart << '\n';
  for(int ipart=0;ipart<NbPart;ipart++){

    Module1=false;
    Module2=false;
    Ladder1=false;
    Ladder2=false;
    MoreThanTwoHits=false;
    int LadderIdTest=0;
    int HitIndex = 0;
    int ModuleTest=0;
    int NbHitsTest=MCInfoHolder->GetASimParticle(ipart).NHits;
    // std::cout << "Nb Hits = "<< NbHitsTest<< '\n';
    if(NbHitsTest!=0){
      for(int ihit=0;ihit<NbHitsTest;ihit++){
        int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
        HitIndex = FirstHitIdx + ihit;
        iHitNPixels=MCInfoHolder->GetASimHit(FirstHitIdx + ihit).Npixels;
        // std::cout << "iHitNPixels = " << iHitNPixels<< '\n';
        if(iHitNPixels>0){
          // std::cout << "PIXELS OK" << '\n';
          if (MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID<=12) {
            Ladder1=true;
            if (MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID<=6) {
              Module1=true;
            }else{ //end IF Ladder 1 Module 1
              Module2=true;
            }//end IF Ladder 1 Module 2
          }else{ //End if Ladder 1
            Ladder2=true;
            if (MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID<=18) {
              Module1=true;
            }else{ //end IF Ladder 2 Module 1
              Module2=true;
            } //end IF Ladder 2 Module 2
          }//End if Ladder 2
        }//End if Npixels !=0
      }//end loop on Hits of real parts

    if(Ladder1==true && Ladder2==false){
      LadderIdTest=1;
      ListOfSimuPartIdL1.push_back(ipart);
    }
    if(Ladder1==false && Ladder2==true){
      LadderIdTest=2;
      ListOfSimuPartIdL2.push_back(ipart);
    }
    double MC_VrtXpos = MCInfoHolder->GetASimParticle(ipart).ProdVtx.X();
		double MC_VrtYpos = MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y();
		double MC_VrtZpos = MCInfoHolder->GetASimParticle(ipart).ProdVtx.Z();
      // std::cout << "Vrtx Pos X = "<<MC_VrtXpos
      //           << "Vrtx Pos Y = "<<MC_VrtYpos
      //           << "Vrtx Pos Z = "<<MC_VrtZpos
      //           << '\n';
  if(Module1==true && Module2==true){
		if(NbHitsTest>2){ //TWO SIDES LOOPER ELECTRON
			  ListOfSimuPartClass.push_back(5);
	  }
    if(NbHitsTest==2){ //TWO SIDES ELECTRON
      double  RVertex = sqrt( pow(MC_VrtXpos,2)
                      + pow(MC_VrtYpos,2)
                      + pow(MC_VrtZpos,2));
                      // std::cout << "Two Sides with Rvertex = "<< RVertex<< '\n';
      ListOfSimuPartClass.push_back(2);
      // std::cout << "push_back TS" << '\n';
		  // if(RVertex<1.)  ListOfSimuPartClass.push_back(2);
      // else            ListOfSimuPartClass.push_back(3);
			}
      // std::cout << "ON TS" << '\n';
  }else if((Module1==true && Module2==false)||(Module1==false && Module2==true)){
		if(NbHitsTest>1){ //ONE SIDE LOOPER ELECTRON
		  ListOfSimuPartClass.push_back(4);
      // std::cout << "OSL" << '\n';
		}else if(NbHitsTest==1){ //ONE SIDE ELECTRON
		  ListOfSimuPartClass.push_back(1);
      // std::cout << "OS" << '\n';
		}
    // std::cout << "ON OS" << '\n';
	}else{
    ListOfSimuPartClass.push_back(-999);
  }
  int TestpatternType = -999;
  TestpatternType =  ListOfSimuPartClass[ipart];
  if(Ladder1==true || Ladder2==true){
    // if(Ladder1) std::cout << "LadderId = 1"<< '\n';
    // if(Ladder2) std::cout << "LadderId = 2"<< '\n';
    // std::cout << "Particle Pattern Type: "<< TestpatternType<< '\n';
  }
  }
  }//End Loop On parts
  // h_NbTslL1->Fill(NbTslL1);
  // h_NbTslL2->Fill(NbTslL2);
  return;
}
//===========================================================================================================
void DBeaster::RecoCategorieClassification(int ladderId,int iHitSeuil){
  // std::cout << "RecoCategorieClassification For Ladder" << ladderId<<'\n';
  int NbPart=ListOfRecoParts_SP.size();
  int LadderIdTest;
  for(int ipart=0;ipart<NbPart;ipart++){
    Module1=false;
    Module2=false;
    Ladder1=false;
    Ladder2=false;
    MoreThanTwoHits=false;

    int NbHitsTest=ListOfRecoParts_SP[ipart].HitList.size();;
    if(NbHitsTest>2){MoreThanTwoHits=true;}
    for(int ihit=0;ihit<NbHitsTest;ihit++){

    	int HitIndex  = ListOfRecoParts_SP[ipart].HitList[ihit].HitId;
    	LadderIdTest  = ListOfRecoParts_SP[ipart].HitList[ihit].LadderId;
    	int ModuleTest= ListOfRecoParts_SP[ipart].HitList[ihit].ModuleId;

    	if(LadderIdTest==1){Ladder1=true;}
    	if(LadderIdTest==2){Ladder2=true;}
    	if(ModuleTest==1){Module1=true;}
    	if(ModuleTest==2){Module2=true;}
    }//end loop on Hits of reco parts
    //if(Ladder1==true & Ladder2==false)

    if(LadderIdTest==ladderId){
      if(Module1==true && Module2==true){
      	if(MoreThanTwoHits==true){ //TWO SIDES LOOPER ELECTRON
          // std::cout << "******RECO TSL ****** " << '\n';
      	  ListOfRecoParts_SP[ipart].ClassType=4;
    	  }else if(NbHitsTest==2){ //TWO SIDES ELECTRON
          // std::cout << "******RECO TS ****** " << '\n';
      	  ListOfRecoParts_SP[ipart].ClassType=2;
    	  }
      }else if((Module1==true && Module2==false)||(Module1==false && Module2==true)){
        if(NbHitsTest>1){
  	        //if(MoreThanTwoHits==true)//ONE SIDE LOOPER ELECTRON
            // std::cout << "******RECO OSL ****** " << '\n';
  	         ListOfRecoParts_SP[ipart].ClassType=3;
        }else if(NbHitsTest==1){ //ONE SIDE ELECTRON
          // std::cout << "******RECO OS ****** " << '\n';
  	       ListOfRecoParts_SP[ipart].ClassType=1;
  	      }
      }
    }
    // int Classtype=ListOfRecoParts_SP[ipart].ClassType;
  }//End Loop on parts

    FillListOfBeastRecoPart(ladderId);

  return;
}

//===========================================================================================================
void DBeaster::Particle_Reconstruction(int ladderId){
    double ScSeuil=0.;
    double SpSeuil=0.;
    if(ladderId==1){
      ScSeuil=15.;
      SpSeuil=15.;
    }
    if(ladderId==2){
      ScSeuil=10.;
      SpSeuil=10.;
    }
    if((ListOfBeastHitsOnM1.size()==1&&ListOfBeastHitsOnM2.size()==0)||(ListOfBeastHitsOnM1.size()==0 && ListOfBeastHitsOnM2.size()==1)){
      // cout<<"CAT1"<<endl;
      //OneHit=true;

      SpatialClustering(ScSeuil);
      FillListOfRecoPartsSC();
      FillListOfRecoPartsSP();
    }
    if((ListOfBeastHitsOnM1.size()+ListOfBeastHitsOnM2.size())>1){
      if((ListOfBeastHitsOnM1.size()==1) && (ListOfBeastHitsOnM2.size()==1)){
      	// cout<<"CAT2"<<endl;
      	SpatialClustering(ScSeuil);
      	Superposition(SpSeuil);
      	FillListOfRecoPartsSC();
      	FillListOfRecoPartsSP();
      }else{
    	// cout<<"CAT3"<<endl;
    	SpatialClustering(ScSeuil);
    	Superposition(SpSeuil);
    	// Allignement(3.);
    	FillListOfRecoPartsSC();
    	FillListOfRecoPartsSP();

    	// FillListOfRecoPartsSPnew();
    	// FillTslCandidat();
      }
    }
    // FillListOfBeastRecoPart(ladderId);
    return;
}

//================================================================================================================
void DBeaster::SpatialClustering(double Seuil){
  // std::cout << "*** Spatial Clustering ***" << '\n';
  double RseuilSC=Seuil;
  int NbRecoPartsM1_SC=0;

  for(int ihit=0;ihit<ListOfBeastHitsOnM1.size();ihit++){
    // std::cout << "ihit = " <<ihit <<'\n';
    for(int jhit=0;jhit<ListOfBeastHitsOnM1.size();jhit++){
      if(ihit>=jhit) continue;
      // std::cout << "***** jhit = " <<jhit <<'\n';
      	U1=ListOfBeastHitsOnM1[ihit].hitPositionLadderUVW(0);
      	U2=ListOfBeastHitsOnM1[jhit].hitPositionLadderUVW(0);
        // std::cout << "U1 = " <<U1<< " U2 = "<<U2<<'\n';
      	dU=fabs(U1-U2);
        // std::cout << "dU = " <<dU<< '\n';
      	V1=ListOfBeastHitsOnM1[ihit].hitPositionLadderUVW(1);
      	V2=ListOfBeastHitsOnM1[jhit].hitPositionLadderUVW(1);
        // std::cout << "V1 = " <<V1<< " V2 = "<<V2<<'\n';
      	dV=fabs(V1-V2);
        // std::cout << "dV = " <<dV<< '\n';
      	dR=sqrt( pow(dU,2) + pow(dV,2) );
        // std::cout << "dR = " <<dR<< '\n';
        // std::cout << "RseuilSC = "<< RseuilSC<< '\n';
       	if(dR<RseuilSC){
          // std::cout << "Sc seuil OK M1" << '\n';
      	  if(!ListOfBeastHitsOnM1[ihit].IsRec_SC){
            ListOfBeastHitsOnM1[ihit].RecoId_SC=NbRecoPartsM1_SC;
      	    if(ListOfBeastHitsOnM1[ihit].IsRec_SC){
      	      ListOfBeastHitsOnM1[ihit].RecoId_SC=ListOfBeastHitsOnM1[jhit].RecoId_SC;
      	    }
      	    if(!ListOfBeastHitsOnM1[jhit].IsRec_SC){
              ListOfBeastHitsOnM1[jhit].RecoId_SC=ListOfBeastHitsOnM1[ihit].RecoId_SC;
      	      NbRecoPartsM1_SC++;
              ListOfBeastHitsOnM1[ihit].IsRec_SC=true;
              ListOfBeastHitsOnM1[jhit].IsRec_SC=true;
      	    }
      	  }
      	  if(ListOfBeastHitsOnM1[ihit].IsRec_SC){
      	    if(!ListOfBeastHitsOnM1[jhit].IsRec_SC){
      	      ListOfBeastHitsOnM1[jhit].RecoId_SC=ListOfBeastHitsOnM1[ihit].RecoId_SC;
              ListOfBeastHitsOnM1[jhit].IsRec_SC=true;
      	    }
      	    if((ListOfBeastHitsOnM1[jhit].IsRec_SC)&&(ListOfBeastHitsOnM1[jhit].RecoId_SC!=ListOfBeastHitsOnM1[ihit].RecoId_SC)){
      	      if(ListOfBeastHitsOnM1[ihit].RecoId_SC<ListOfBeastHitsOnM1[jhit].RecoId_SC){
            		for(int khit=0;khit<ListOfBeastHitsOnM1.size();khit++){
            		  if(ListOfBeastHitsOnM1[khit].RecoId_SC==ListOfBeastHitsOnM1[jhit].RecoId_SC){
            		    ListOfBeastHitsOnM1[khit].RecoId_SC=ListOfBeastHitsOnM1[ihit].RecoId_SC;
            		  }
            		}
      	      }
      	      if(ListOfBeastHitsOnM1[ihit].RecoId_SC>ListOfBeastHitsOnM1[jhit].RecoId_SC){
            		for(int khit=0;khit<ListOfBeastHitsOnM1.size();khit++){
            		  if(ListOfBeastHitsOnM1[khit].RecoId_SC==ListOfBeastHitsOnM1[ihit].RecoId_SC){
            		    ListOfBeastHitsOnM1[khit].RecoId_SC=ListOfBeastHitsOnM1[jhit].RecoId_SC;
            		  }
            		}
      	      }
      	    }
      	  }
      	}//END test Seuil
      dR=0;
    }//END J Loop
    if(!ListOfBeastHitsOnM1[ihit].IsRec_SC){
      NbRecoPartsM1_SC++;
      ListOfBeastHitsOnM1[ihit].IsRec_SC=true;
    }
  }//END I Loop

  int NbRecoPartsM2_SC=0;

  for(int ihit=0;ihit<ListOfBeastHitsOnM2.size();ihit++){
    // std::cout << "**ihit = " <<ihit <<'\n';
    for(int jhit=0;jhit<ListOfBeastHitsOnM2.size();jhit++){
      if(ihit>=jhit) continue;
      // std::cout << "***** jhit = " <<jhit <<'\n';
        U1=ListOfBeastHitsOnM2[ihit].hitPositionLadderUVW(0);
        U2=ListOfBeastHitsOnM2[jhit].hitPositionLadderUVW(0);
        dU=abs(U1-U2);
        // std::cout << "dU = " <<dU<< '\n';
        V1=ListOfBeastHitsOnM2[ihit].hitPositionLadderUVW(1);
        V2=ListOfBeastHitsOnM2[jhit].hitPositionLadderUVW(1);
        dV=abs(V1-V2);
        // std::cout << "dV = " <<dV<< '\n';
        dR=sqrt( pow(dU,2) + pow(dV,2) );
        if(dR<RseuilSC){
          // std::cout << "Sc seuil OK M2" << '\n';
          if(!ListOfBeastHitsOnM2[ihit].IsRec_SC){
            ListOfBeastHitsOnM2[ihit].RecoId_SC=NbRecoPartsM2_SC;
            if(ListOfBeastHitsOnM2[ihit].IsRec_SC){
              ListOfBeastHitsOnM2[ihit].RecoId_SC=ListOfBeastHitsOnM2[jhit].RecoId_SC;
            }
            if(!ListOfBeastHitsOnM2[jhit].IsRec_SC){
              ListOfBeastHitsOnM2[jhit].RecoId_SC=ListOfBeastHitsOnM2[ihit].RecoId_SC;
              NbRecoPartsM2_SC++;
              ListOfBeastHitsOnM2[ihit].IsRec_SC=true;
              ListOfBeastHitsOnM2[jhit].IsRec_SC=true;
            }
          }
          if(ListOfBeastHitsOnM2[ihit].IsRec_SC){
            if(!ListOfBeastHitsOnM2[jhit].IsRec_SC){
              ListOfBeastHitsOnM2[jhit].RecoId_SC=ListOfBeastHitsOnM2[ihit].RecoId_SC;
              ListOfBeastHitsOnM2[jhit].IsRec_SC=true;
            }
            if((ListOfBeastHitsOnM2[jhit].IsRec_SC)&&(ListOfBeastHitsOnM2[jhit].RecoId_SC!=ListOfBeastHitsOnM2[ihit].RecoId_SC)){
              if(ListOfBeastHitsOnM2[ihit].RecoId_SC<ListOfBeastHitsOnM2[jhit].RecoId_SC){
                for(int khit=0;khit<ListOfBeastHitsOnM2.size();khit++){
                  if(ListOfBeastHitsOnM2[khit].RecoId_SC==ListOfBeastHitsOnM2[jhit].RecoId_SC){
                    ListOfBeastHitsOnM2[khit].RecoId_SC=ListOfBeastHitsOnM2[ihit].RecoId_SC;
                  }
                }
              }
              if(ListOfBeastHitsOnM2[ihit].RecoId_SC>ListOfBeastHitsOnM2[jhit].RecoId_SC){
                for(int khit=0;khit<ListOfBeastHitsOnM2.size();khit++){
                  if(ListOfBeastHitsOnM2[khit].RecoId_SC==ListOfBeastHitsOnM2[ihit].RecoId_SC){
                    ListOfBeastHitsOnM2[khit].RecoId_SC=ListOfBeastHitsOnM2[jhit].RecoId_SC;
                  }
                }
              }
            }
          }
        }//END test Seuil
      dR=0;
    }//END J Loop
    if(!ListOfBeastHitsOnM2[ihit].IsRec_SC){
      NbRecoPartsM2_SC++;
      ListOfBeastHitsOnM2[ihit].IsRec_SC=true;
    }
  }//END I Loop
  return;
}
//=======================================================================================================

void DBeaster::Superposition(double Seuil){
  // std::cout << "*** Superposition***" << '\n';

  double RseuilSP=Seuil;
  int NbRecoParts_SP=0;
  if((ListOfBeastHitsOnM1.size()!=0) && (ListOfBeastHitsOnM2.size() !=0)){
    for(int ihit=0;ihit<ListOfBeastHitsOnM1.size();ihit++){
      // std::cout << "** ihit" <<ihit <<'\n';
      double RminSP=3000;
      int jhitmin=-1;
      for(int jhit=0;jhit<ListOfBeastHitsOnM2.size();jhit++){
        // std::cout << "***** jhit" <<jhit <<'\n';
        U1=ListOfBeastHitsOnM1[ihit].hitPositionLadderUVW(0);
        U2=ListOfBeastHitsOnM2[jhit].hitPositionLadderUVW(0);
        // std::cout << "Plane Id fot U1 = " <<ListOfBeastHitsOnM1[ihit].SensorId << '\n';
        // std::cout << "U1 =" <<U1<<'\n';
        // std::cout << "U2 =" <<U2<<'\n';
      	dU=fabs(U1-U2);
        // std::cout << "dU = " <<dU<< '\n';
        V1=ListOfBeastHitsOnM1[ihit].hitPositionLadderUVW(1);
        V2=-ListOfBeastHitsOnM2[jhit].hitPositionLadderUVW(1);
        // std::cout << "V1 =" <<V1<<'\n';
        // std::cout << "V2 =" <<V2<<'\n';
      	dV=fabs(V1-V2);
        // std::cout << "dV = " <<dV<< '\n';
      	dR=sqrt( pow(dU,2) + pow(dV,2) );
        // std::cout << "dR = " << dR <<'\n';
      	if(dR<=RminSP){
      	  if(!ListOfBeastHitsOnM2[jhit].IsRec_SP){
      	  //if(!(Frame.ListOfHitsM2[jhit].IsRec_SP))
      	    RminSP=dR;
      	    jhitmin=jhit;
      	  }
      	  if((ListOfBeastHitsOnM2[jhit].IsRec_SP) && (dR<ListOfBeastHitsOnM2[jhit].RpairedSP) ){
      	  //if((Frame.ListOfHitsM2[jhit].IsRec_SP) && (dR<Frame.ListOfHitsM2[jhit].RpairedSP) )
      	    RminSP=dR;
      	    jhitmin=jhit;
      	  }
      	}
      }//END LOOP OF jhit
      // std::cout << "RminSP = " << RminSP <<'\n';
      // std::cout << "RseuilSP = " << RseuilSP <<'\n';
      if(RminSP<=RseuilSP){
        // std::cout << "SP seuil OK" << '\n';
      	ListOfBeastHitsOnM1[ihit].RecoId_SP=NbRecoParts_SP;
      	ListOfBeastHitsOnM2[jhitmin].RecoId_SP=NbRecoParts_SP;
      	ListOfBeastHitsOnM1[ihit].RpairedSP=RminSP;
      	ListOfBeastHitsOnM2[jhitmin].RpairedSP=RminSP;
      	ListOfBeastHitsOnM1[ihit].IsRec_SP=true;
      	ListOfBeastHitsOnM2[jhitmin].IsRec_SP=true;
      	NbRecoParts_SP++;
      }
    }
  }//END IF NbHits!=0
  return;
}
//=======================================================================================================
void DBeaster::Allignement(double Seuil){
  double VseuilAL=Seuil;
  int NbRecoPartsM1_AL=0;
  if((ListOfBeastHitsOnM2.size()!=0)){
  //if((Frame.NbHitsM1 !=0))
    for(int ihit=0;ihit<ListOfBeastHitsOnM1.size();ihit++){
      double ihitRecoID_SC= ListOfBeastHitsOnM1[ihit].RecoId_SC;
      if(ihitRecoID_SC!=-1){
      	double VminAL=3000;
      	int jhitmin=-1;
      	for(int jhit=0;jhit<ListOfBeastHitsOnM1.size();jhit++){
          if(ihit>=jhit) continue;
      	  double jhitRecoID_SC= ListOfBeastHitsOnM1[jhit].RecoId_SC;
      	  if(jhitRecoID_SC!=-1){
            V1=ListOfBeastHitsOnM1[ihit].hitPositionLadderUVW(1);
            V2=ListOfBeastHitsOnM1[jhit].hitPositionLadderUVW(1);
      	    dV=abs(V1-V2);
      	    if(dV<=VminAL){
      	      if(!ListOfBeastHitsOnM1[jhit].IsRec_AL){
      	      //if(!(Frame.ListOfHitsM1[jhit].IsRec_AL))
  		          VminAL=dV;
                jhitmin=jhit;
      	      }
      	      if((ListOfBeastHitsOnM1[jhit].IsRec_AL) && (dV<ListOfBeastHitsOnM1[jhit].VpairedAL) ){
      	      //if((Frame.ListOfHitsM1[jhit].IsRec_AL) && (dV<Frame.ListOfHitsM1[jhit].VpairedAL) )
      		        VminAL=dV;
    		          jhitmin=jhit;
      	      }
      	    }
      	  }
      	}//END LOOP OF jhit
      	if(VminAL<=VseuilAL){
      	  ListOfBeastHitsOnM1[ihit].RecoId_AL=NbRecoPartsM1_AL;
      	  //Frame.ListOfHitsM1[ihit].RecoId_AL=Frame.NbRecoPartsM1_AL;
      	  ListOfBeastHitsOnM1[jhitmin].RecoId_AL=NbRecoPartsM1_AL;
      	  //Frame.ListOfHitsM1[jhitmin].RecoId_AL=Frame.NbRecoPartsM1_AL;
      	  ListOfBeastHitsOnM1[ihit].VpairedAL=VminAL;
      	  //Frame.ListOfHitsM1[ihit].VpairedAL=VminAL;
      	  ListOfBeastHitsOnM1[jhitmin].VpairedAL=VminAL;
      	  //Frame.ListOfHitsM1[jhitmin].VpairedAL=VminAL;
      	  ListOfBeastHitsOnM1[ihit].IsRec_AL=true;
      	  ListOfBeastHitsOnM1[jhitmin].IsRec_AL=true;
      	  NbRecoPartsM1_AL++;
      	}
      }//End if Non Reco
    }//END Loop on Ihits
  }//END IF NbHits!=0

  int NbRecoPartsM2_AL=0;
  if((ListOfBeastHitsOnM2.size()!=0)){
  //if((Frame.NbHitsM2 !=0))
    for(int ihit=0;ihit<ListOfBeastHitsOnM2.size();ihit++){
      double ihitRecoID_SC= ListOfBeastHitsOnM2[ihit].RecoId_SC;
      if(ihitRecoID_SC!=-1){
      	double VminAL=3000;
      	int jhitmin=-1;
      	for(int jhit=0;jhit<ListOfBeastHitsOnM2.size();jhit++){
          if(ihit>=jhit) continue;
      	  double jhitRecoID_SC= ListOfBeastHitsOnM2[jhit].RecoId_SC;
      	  if(jhitRecoID_SC!=-1){
            V1=ListOfBeastHitsOnM2[ihit].hitPositionLadderUVW(1);
            V2=ListOfBeastHitsOnM2[jhit].hitPositionLadderUVW(1);
      	    dV=abs(V1-V2);
      	    if(dV<=VminAL){
      	      if(!ListOfBeastHitsOnM2[jhit].IsRec_AL){
      	      //if(!(Frame.ListOfHitsM2[jhit].IsRec_AL))
  		          VminAL=dV;
                jhitmin=jhit;
      	      }
      	      if((ListOfBeastHitsOnM2[jhit].IsRec_AL) && (dV<ListOfBeastHitsOnM2[jhit].VpairedAL) ){
      	      //if((Frame.ListOfHitsM2[jhit].IsRec_AL) && (dV<Frame.ListOfHitsM2[jhit].VpairedAL) )
      		        VminAL=dV;
    		          jhitmin=jhit;
      	      }
      	    }
      	  }
      	}//END LOOP OF jhit
      	if(VminAL<=VseuilAL){
      	  ListOfBeastHitsOnM2[ihit].RecoId_AL=NbRecoPartsM2_AL;
      	  //Frame.ListOfHitsM2[ihit].RecoId_AL=Frame.NbRecoPartsM2_AL;
      	  ListOfBeastHitsOnM2[jhitmin].RecoId_AL=NbRecoPartsM2_AL;
      	  //Frame.ListOfHitsM2[jhitmin].RecoId_AL=Frame.NbRecoPartsM2_AL;
      	  ListOfBeastHitsOnM2[ihit].VpairedAL=VminAL;
      	  //Frame.ListOfHitsM2[ihit].VpairedAL=VminAL;
      	  ListOfBeastHitsOnM2[jhitmin].VpairedAL=VminAL;
      	  //Frame.ListOfHitsM2[jhitmin].VpairedAL=VminAL;
      	  ListOfBeastHitsOnM2[ihit].IsRec_AL=true;
      	  ListOfBeastHitsOnM2[jhitmin].IsRec_AL=true;
      	  NbRecoPartsM2_AL++;
      	}
      }//End if Non Reco
    }//END Loop on Ihits
  }//END IF NbHits!=0

  return;
}
//=======================================================================================================
void DBeaster::FillListOfRecoPartsSC(){
// std::cout << "/*/*// Fill Reco Part SC" << '\n';
  double NbRecoPartsM1=0;
  for(int ihit=0; ihit<ListOfBeastHitsOnM1.size();ihit++){
    double ihitRecoID= ListOfBeastHitsOnM1[ihit].RecoId_SC;

    //if(ihitRecoID!=0 && (IsScRecoM1[ihit]!=1))
    if(ihitRecoID!=-1 && (!ListOfBeastHitsOnM1[ihit].IsFill_SC)){
    //if(ihitRecoID!=-1 && !(Frame.ListOfHitsM1[ihit].IsRec_SCreco))
      ABeastRecoPart particle;
      particle.HitList.clear();
      particle.HitScIdList.clear();
      particle.HitSpIdList.clear();
      particle.HitList.push_back(ListOfBeastHitsOnM1[ihit]);
      particle.HitScIdList.push_back(ListOfBeastHitsOnM1[ihit].RecoId_SC);
      particle.HitSpIdList.push_back(ListOfBeastHitsOnM1[ihit].RecoId_SP);
      particle.IsRec_SPreco=false;
      particle.ClassType=0;
      // std::cout << "PushBack Sc Part M1" << '\n';
      ListOfRecoPartsM1_SC.push_back(particle);
      ListOfBeastHitsOnM1[ihit].IsFill_SC=true;
      // IsScFillM1[ihit]=1;
      NbRecoPartsM1++;

      for(int jhit=0;jhit<ListOfBeastHitsOnM1.size();jhit++){
       if(ihit>=jhit) continue;
         double jhitRecoID= ListOfBeastHitsOnM1[jhit].RecoId_SC;
         if(ihitRecoID==jhitRecoID && (!ListOfBeastHitsOnM1[jhit].IsFill_SC)){
          ListOfRecoPartsM1_SC[NbRecoPartsM1-1].HitList.push_back(ListOfBeastHitsOnM1[jhit]);
          ListOfRecoPartsM1_SC[NbRecoPartsM1-1].HitScIdList.push_back(ListOfBeastHitsOnM1[jhit].RecoId_SC);
          ListOfRecoPartsM1_SC[NbRecoPartsM1-1].HitSpIdList.push_back(ListOfBeastHitsOnM1[jhit].RecoId_SP);

          ListOfBeastHitsOnM1[jhit].IsFill_SC=true;
          // IsScFillM1[jhit]=1;
      	 }
      }
    }
    //if(ihitRecoID==0)
    if(ihitRecoID==-1){
      ABeastRecoPart particle;
      particle.HitList.clear();
      particle.HitScIdList.clear();
      particle.HitSpIdList.clear();
      particle.HitList.push_back(ListOfBeastHitsOnM1[ihit]);
      particle.HitScIdList.push_back(ListOfBeastHitsOnM1[ihit].RecoId_SC);
      particle.HitSpIdList.push_back(ListOfBeastHitsOnM1[ihit].RecoId_SP);
      particle.IsRec_SPreco=false;
      particle.IsRec_ALreco=true;
      particle.ClassType=0;
      ListOfRecoPartsM1_SC.push_back(particle);
            // std::cout << "PushBack Sc Part M1" << '\n';

      ListOfBeastHitsOnM1[ihit].IsFill_SC=true;
      // IsScFillM1[ihit]=1;
      NbRecoPartsM1++;
      double ihitRecoID_AL= ListOfBeastHitsOnM1[ihit].RecoId_AL;
      if(ihitRecoID_AL!=-1){
      	for(int jhit=0;jhit<ListOfBeastHitsOnM1.size();jhit++){
          if(ihit>=jhit) continue;
      	    double jhitRecoID_AL= ListOfBeastHitsOnM1[jhit].RecoId_AL;
      	    if(ihitRecoID_AL==jhitRecoID_AL ){
      	      ListOfRecoPartsM1_SC[NbRecoPartsM1-1].HitList.push_back(ListOfBeastHitsOnM1[jhit]);
      	      ListOfRecoPartsM1_SC[NbRecoPartsM1-1].HitScIdList.push_back(ListOfBeastHitsOnM1[jhit].RecoId_SC);
      	      ListOfRecoPartsM1_SC[NbRecoPartsM1-1].HitSpIdList.push_back(ListOfBeastHitsOnM1[jhit].RecoId_SP);
              ListOfBeastHitsOnM1[jhit].IsFill_SC=true;
              // IsScFillM1[jhit]=1;
      	    }
      	}
      }
    }
  }

  double NbRecoPartsM2=0;
  for(int ihit=0; ihit<ListOfBeastHitsOnM2.size();ihit++){
    double ihitRecoID= ListOfBeastHitsOnM2[ihit].RecoId_SC;

    //if(ihitRecoID!=0 && (IsScRecoM2[ihit]!=1))
    if((ihitRecoID!=-1) && (!ListOfBeastHitsOnM2[ihit].IsFill_SC)){
    //if(ihitRecoID!=-1 && !(Frame.ListOfHitsM2[ihit].IsRec_SCreco))
      ABeastRecoPart particle;
      particle.HitList.clear();
      particle.HitScIdList.clear();
      particle.HitSpIdList.clear();
      particle.HitList.push_back(ListOfBeastHitsOnM2[ihit]);
      particle.HitScIdList.push_back(ListOfBeastHitsOnM2[ihit].RecoId_SC);
      particle.HitSpIdList.push_back(ListOfBeastHitsOnM2[ihit].RecoId_SP);
      particle.IsRec_SPreco=false;
      particle.ClassType=0;
      ListOfRecoPartsM2_SC.push_back(particle);
      // std::cout << "PushBack Sc Part M2" << '\n';
      ListOfBeastHitsOnM2[ihit].IsFill_SC=true;
      // IsScFillM2[ihit]=1;
      NbRecoPartsM2++;

      for(int jhit=0;jhit<ListOfBeastHitsOnM2.size();jhit++){
       if(ihit>=jhit) continue;
         double jhitRecoID= ListOfBeastHitsOnM2[jhit].RecoId_SC;
         if(ihitRecoID==jhitRecoID && (!ListOfBeastHitsOnM2[jhit].IsFill_SC)){
          ListOfRecoPartsM2_SC[NbRecoPartsM2-1].HitList.push_back(ListOfBeastHitsOnM2[jhit]);
          ListOfRecoPartsM2_SC[NbRecoPartsM2-1].HitScIdList.push_back(ListOfBeastHitsOnM2[jhit].RecoId_SC);
          ListOfRecoPartsM2_SC[NbRecoPartsM2-1].HitSpIdList.push_back(ListOfBeastHitsOnM2[jhit].RecoId_SP);
          ListOfBeastHitsOnM2[jhit].IsFill_SC=true;
          // IsScFillM2[jhit]=1;
         }
      }
    }
    //if(ihitRecoID==0)
    if(ihitRecoID==-1){
      ABeastRecoPart particle;
      particle.HitList.clear();
      particle.HitScIdList.clear();
      particle.HitSpIdList.clear();
      particle.HitList.push_back(ListOfBeastHitsOnM2[ihit]);
      particle.HitScIdList.push_back(ListOfBeastHitsOnM2[ihit].RecoId_SC);
      particle.HitSpIdList.push_back(ListOfBeastHitsOnM2[ihit].RecoId_SP);
      particle.IsRec_SPreco=false;
      particle.IsRec_ALreco=true;
      particle.ClassType=0;
      ListOfRecoPartsM2_SC.push_back(particle);
      // std::cout << "PushBack Sc Part M2" << '\n';
      ListOfBeastHitsOnM2[ihit].IsFill_SC=true;
      // IsScFillM2[ihit]=1;
      NbRecoPartsM2++;
      double ihitRecoID_AL= ListOfBeastHitsOnM2[ihit].RecoId_AL;
      if(ihitRecoID_AL!=-1){
        for(int jhit=0;jhit<ListOfBeastHitsOnM2.size();jhit++){
          if(ihit>=jhit) continue;
            double jhitRecoID_AL= ListOfBeastHitsOnM2[jhit].RecoId_AL;
            if(ihitRecoID_AL==jhitRecoID_AL ){
              ListOfRecoPartsM2_SC[NbRecoPartsM2-1].HitList.push_back(ListOfBeastHitsOnM2[jhit]);
              ListOfRecoPartsM2_SC[NbRecoPartsM2-1].HitScIdList.push_back(ListOfBeastHitsOnM2[jhit].RecoId_SC);
              ListOfRecoPartsM2_SC[NbRecoPartsM2-1].HitSpIdList.push_back(ListOfBeastHitsOnM2[jhit].RecoId_SP);
              ListOfBeastHitsOnM2[jhit].IsFill_SC=true;
              // IsScFillM2[jhit]=1;
            }
        }
      }
    }
  }

 return;
}
//=======================================================================================================
void DBeaster::FillListOfRecoPartsSP(){
  ListOfRecoParts_SP.clear();
  // std::cout << "Inside fill List size = " << ListOfRecoParts_SP.size() <<'\n';
  double NbPartsM1=ListOfRecoPartsM1_SC.size();
  double NbPartsM2=ListOfRecoPartsM2_SC.size();
  double NbRecoPartsSP=0;

 if(NbPartsM1!=0 && NbPartsM2!=0){
  for(int ipart=0;ipart<NbPartsM1;ipart++){
    int ipartNbHits=ListOfRecoPartsM1_SC[ipart].HitList.size();
    for(int jpart=0;jpart<NbPartsM2;jpart++){
      int jpartNbHits=ListOfRecoPartsM2_SC[jpart].HitList.size();
      int NbCorrespondance=0;
      for(int ihit=0;ihit<ipartNbHits;ihit++){
      	int ihitRecoPartID_SP=ListOfRecoPartsM1_SC[ipart].HitSpIdList[ihit];
        for(int jhit=0;jhit<jpartNbHits;jhit++){
      	  int jhitRecoPartID_SP=ListOfRecoPartsM2_SC[jpart].HitSpIdList[jhit];
      	  if((ihitRecoPartID_SP!=-1)&&(ihitRecoPartID_SP == jhitRecoPartID_SP)){
      	    NbCorrespondance++;
      	  }
	       }//END loop on hits from jpart
      }//END loop on hits from ipart
      if( (NbCorrespondance==1) && ( ( (ipartNbHits==1) && (jpartNbHits==1) ) || (ListOfRecoPartsM1_SC[ipart].IsRec_ALreco || ListOfRecoPartsM2_SC[jpart].IsRec_ALreco ) ) ){
      	ListOfRecoPartsM1_SC[ipart].IsRec_SPreco=true;
      	ListOfRecoPartsM2_SC[jpart].IsRec_SPreco=true;
      	ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
        // std::cout << "PushBack SP Part" << '\n';

      	ListOfRecoParts_SP[NbRecoPartsSP].HitList.insert(ListOfRecoParts_SP[NbRecoPartsSP].HitList.end(),ListOfRecoPartsM2_SC[jpart].HitList.begin(),ListOfRecoPartsM2_SC[jpart].HitList.end());
      	NbRecoPartsSP++;
      }
      if(NbCorrespondance>1){
      	ListOfRecoPartsM1_SC[ipart].IsRec_SPreco=true;
      	ListOfRecoPartsM2_SC[jpart].IsRec_SPreco=true;
      	ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
        // std::cout << "+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-PushBack SP Part TSL" << '\n';
      	ListOfRecoParts_SP[NbRecoPartsSP].HitList.insert(ListOfRecoParts_SP[NbRecoPartsSP].HitList.end(),ListOfRecoPartsM2_SC[jpart].HitList.begin(),ListOfRecoPartsM2_SC[jpart].HitList.end());
      	NbRecoPartsSP++;
      }
      if(ipart==(NbPartsM1-1)){
      	if( !(ListOfRecoPartsM2_SC[jpart].IsRec_SPreco) ){
      	  ListOfRecoParts_SP.push_back(ListOfRecoPartsM2_SC[jpart]);
          // std::cout << "PushBack SP Part" << '\n';
      	  NbRecoPartsSP++;
      	}
      }
    }//END loop on parts from M2
    if( !(ListOfRecoPartsM1_SC[ipart].IsRec_SPreco)){
      ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
      // std::cout << "PushBack SP Part" << '\n';
      NbRecoPartsSP++;
    }
  }//END loop on parts from M1
 }//END IF NbParts!=0
 if(NbPartsM1==0){
  for(int jpart=0;jpart<NbPartsM2;jpart++){
    ListOfRecoParts_SP.push_back(ListOfRecoPartsM2_SC[jpart]);
    // std::cout << "PushBack SP Part" << '\n';
  }
 }
 if(NbPartsM2==0){
  for(int ipart=0;ipart<NbPartsM1;ipart++){
    ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
    // std::cout << "PushBack SP Part" << '\n';
  }
 }
  return;
}
//=======================================================================================================
void DBeaster::FillListOfBeastRecoPart(int ladderID) {
  int NbRecoParts=ListOfRecoParts_SP.size();
  // std::cout << "TEST/// LadderNumber" << ladderID<<'\n';
  if (ladderID==1) ListOfBeastRecoParts_L1.clear();
  if (ladderID==2) ListOfBeastRecoParts_L2.clear();

  for (size_t ipart = 0; ipart < NbRecoParts; ipart++) {
    if (ladderID==1) {
      // std::cout << "Fill Beast RecoPart L1" << '\n';
      ListOfBeastRecoParts_L1.push_back(ListOfRecoParts_SP[ipart]);
    }
    if(ladderID==2){
      // std::cout << "Fill Beast RecoPart L2" << '\n';
      ListOfBeastRecoParts_L2.push_back(ListOfRecoParts_SP[ipart]);
    }
  }
}
//=======================================================================================================
// void DBeaster::PurityTest(){
//
//   for(int ipart=0;ipart<ListOfRecoParts_SP.size();ipart++){
//     int RecoPartClassType=ListOfRecoParts_SP[ipart].ClassType;
//     for(int ihit=0;ihit<ListOfRecoParts_SP[ipart].HitList.size();ihit++){
//       int PartIdx=HitParticleIdx[ListOfRecoParts_SP[ipart].HitList[ihit]];
//       int SimuPartClassType=ListOfSimuPartClass[PartIdx];
//       switch(SimuPartClassType){
//       	case 1:
//       	  h_SimuOS->Fill(RecoPartClassType);
//       	  break;
//       	case 2:
//       	  h_SimuTS->Fill(RecoPartClassType);
//       	  break;
//       	case 3:
//       	  h_SimuOSL->Fill(RecoPartClassType);
//       	  break;
//       	case 4:
//       	  h_SimuTSL->Fill(RecoPartClassType);
//       	  break;
//       }//End Switch
//     }//End loop on Reco Hits
//   }//End loop on Reco Parts
//
// return;
// }
// //=======================================================================================================
// void DBeaster::PurityTest_Partner(){
//
//   for(int ipart=0;ipart<ListOfRecoParts_SP.size();ipart++){
//     int RecoPartClassType=ListOfRecoParts_SP[ipart].ClassType;
//     bool Partner=true;
//     for(int ihit=0;ihit<ListOfRecoParts_SP[ipart].HitList.size();ihit++){
//
//       int PartIdxihit=HitParticleIdx[ListOfRecoParts_SP[ipart].HitList[ihit]];
//       int SimuPartClassTypeihit=ListOfSimuPartClass[PartIdxihit];
//
//       for(int jhit=0;jhit<ListOfRecoParts_SP[ipart].HitList.size();jhit++){
//       	int PartIdxjhit=HitParticleIdx[ListOfRecoParts_SP[ipart].HitList[jhit]];
//       	//int SimuPartClassTypejhit=ListOfSimuPartClass[PartIdxjhit];
//       	if(ihit!=jhit){
//       	  if(PartIdxihit==PartIdxjhit){
//       	    if(Partner){
//       	      Partner=true;
//       	    }else{
//       	      Partner=false;
//       	    }
//       	  }else{
//       	    Partner=false;
//       	  }
//       	}
//       }//End loop on jhit
//       if(Partner){
//       	switch(SimuPartClassTypeihit){
//       	  case 1:
//       	    h_SimuOS_Good->Fill(RecoPartClassType);
//       	    break;
//       	  case 2:
//       	    h_SimuTS_Good->Fill(RecoPartClassType);
//       	    break;
//       	  case 3:
//       	    h_SimuOSL_Good->Fill(RecoPartClassType);
//       	    break;
//       	  case 4:
//       	    h_SimuTSL_Good->Fill(RecoPartClassType);
//       	    break;
//       	}//End Switch
//       }else{
//       	switch(SimuPartClassTypeihit){
//       	  case 1:
//       	    h_SimuOS_Bad->Fill(RecoPartClassType);
//       	    break;
//       	  case 2:
//       	    h_SimuTS_Bad->Fill(RecoPartClassType);
//       	    break;
//       	  case 3:
//       	    h_SimuOSL_Bad->Fill(RecoPartClassType);
//       	    break;
//       	  case 4:
//       	    h_SimuTSL_Bad->Fill(RecoPartClassType);
//       	    break;
//       	}//End Switch
//       }//End if Partner
//     }//End loop on Reco Hits (ihit)
//   }//End loop on Reco Parts
//
// return;
// }
//=======================================================================================================
//
// void DBeaster::Categorie_Momentum(){
//
//   for(int ipart=0;ipart<ParticleNb;ipart++){
//     for(int ihit=0;ihit<ParticleNHits[ipart];ihit++){
//       int HitIdx=ParticleFirstHitIdx[ipart]+ihit;
//       double px=HitmomMeVX[HitIdx];
//       double py=HitmomMeVY[HitIdx];
//       double pz=HitmomMeVZ[HitIdx];
//       double P=sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
//       switch(ListOfSimuPartClass[ipart]){
//       	case 1:
//       	  h_MomentumOS->Fill(abs(P));
//       	  break;
//       	case 2:
//       	  h_MomentumTS->Fill(abs(P));
//       	  break;
//       	case 3:
//       	  h_MomentumOSL->Fill(abs(P));
//       	  break;
//       	case 4:
//       	  h_MomentumTSL->Fill(abs(P));
//       	  break;
//       }//End switch
//     }//End loop on hits
//   }//End loop on parts
//
// return;
// }
// //=======================================================================================================
// void DBeaster::Angular_DistributionSimuTS(){
//   double ep=2.27;
//   for(int ipart=0;ipart<ListOfSimuPartIdL1.size();ipart++){
//     int ClassType=ListOfSimuPartClass[ListOfSimuPartIdL1[ipart]];
//     int HitIndex=ParticleFirstHitIdx[ipart];
//     if(ClassType==2){
//
//       double U1=HitRecoUmm[HitIndex];
//       double U2=HitRecoUmm[HitIndex+1];
//       double V1=HitRecoVmm[HitIndex];
//       double V2=HitRecoVmm[HitIndex+1];
//       double dU=U2-U1;
//       double dV=V2-V1;
//       double Angle=(TMath::ATan(ep/sqrt(pow(dU,2)+pow(dV,2))))*(180./TMath::Pi());
//
//       h_AngularDistributionTsL1->Fill(Angle);
//     }
//   }
//   for(int ipart=0;ipart<ListOfSimuPartIdL2.size();ipart++){
//     int ClassType=ListOfSimuPartClass[ListOfSimuPartIdL2[ipart]];
//     int HitIndex=ParticleFirstHitIdx[ipart];
//     if(ClassType==2){
//
//       double U1=HitRecoUmm[HitIndex];
//       double U2=HitRecoUmm[HitIndex+1];
//       double V1=HitRecoVmm[HitIndex];
//       double V2=HitRecoVmm[HitIndex+1];
//       double dU=U2-U1;
//       double dV=V2-V1;
//       double Angle=(TMath::ATan(ep/sqrt(pow(dU,2)+pow(dV,2))))*(180./TMath::Pi());
//
//       h_AngularDistributionTsL2->Fill(Angle);
//     }
//   }
//
// return;
// }
// //=======================================================================================================
// void DBeaster::Angular_DistributionSimuTSL(){
//   double ep=2.27;
//   int CurrentIhit;
//   int CurrentJhit;
// //   cout<<"**********************Ladder 1"<< endl;
//   for(int ipart=0;ipart<ListOfSimuPartIdL1.size();ipart++){
// //     cout<<"*****ipart " << ipart <<endl;
//     int ClassType=ListOfSimuPartClass[ListOfSimuPartIdL1[ipart]];
//
// //     cout<<"Test 1 "<<endl;
// //      cout<<"*****ClassType " << ClassType <<endl;
//     if(ClassType==4){
//       //IsPairedDA.clear();
//       //RPairedDA.clear();
//       CurrentIhit=0;
//       for(int ihit=0;ihit<ParticleNHits[ipart];ihit++){
// // 	cout<<"Test 1bis   "<<endl;
// 	int iHitIndex=ParticleFirstHitIdx[ipart]+ihit;
// // 	cout<<"Test 1ter "<<endl;
// 	if(HitmoduleID[iHitIndex]==1){
// // 	  cout<<"--------Current ihit"<<CurrentIhit <<endl;
// 	  double RminSP=5000;
// 	  int jhitmin=-1;
// 	  CurrentJhit=0;
// 	  for(int jhit=0;jhit<ParticleNHits[ipart];jhit++){
// 	    int jHitIndex=ParticleFirstHitIdx[ipart]+jhit;
// 	    if(HitmoduleID[jHitIndex]==2){
// // 	       cout<<"Current jhit"<<CurrentJhit <<endl;
// 	      /*
// 	      if(CurrentIhit==0){
// 		IsPairedDA.push_back(-1);
// 		RPairedDA.push_back(5000);
// 	      }
// 	     */
// 	      double U1=HitRecoULaddermm[iHitIndex];
// 	      double U2=HitRecoULaddermm[jHitIndex];
// 	      double dU=abs(U1-U2);
//
// 	      double V1=HitRecoVLaddermm[iHitIndex];
// 	      double V2=HitRecoVLaddermm[jHitIndex];
// 	      double dV=abs(V1-V2);
//
// 	      double dR=sqrt( pow(dU,2) + pow(dV,2) );
// 	      if(dR<=RminSP){
// // 		cout<<"Test 2 "<<endl;
// 		//if(IsPairedDA[CurrentJhit]!=1){
// // 		  cout<<"Test 3 "<<endl;
// 		  RminSP=dR;
// 		  //jhitmin=CurrentJhit;
// 		  jhitmin=jhit;
// 		//}
// 		/*
// 		if((IsPairedDA[CurrentJhit]==1) && (dR<RPairedDA[CurrentJhit])){
// // 		  cout<<"Test 4 "<<endl;
// 		  RminSP=dR;
// 		  jhitmin=CurrentJhit;
// 		}
// 		*/
// 	      }
//
// 	      CurrentJhit++;
// 	    }//End if jHit is on M2
// 	  }//End for jhit
// 	  //IsPairedDA[jhitmin]=1;
// 	  //RPairedDA[jhitmin]=RminSP;
// 	  double Angle=(TMath::ATan(ep/RminSP))*(180./TMath::Pi());
// 	  if(Angle>0.05){
// 	    //cout<<"*Fill TSL L"<< endl;
// 	    h_AngularDistributionTslL1->Fill(Angle);
// 	  }
// 	  CurrentIhit++;
// 	}//End IF Ihit is on M1
//       }//End For ihit
//     }//End if TSL
//   }//End loop on L1 parts
// // cout<<"**********************END Ladder 1"<< endl;
// // cout<<"**********************Ladder 2"<< endl;
//  for(int ipart=0;ipart<ListOfSimuPartIdL2.size();ipart++){
//     int ClassType=ListOfSimuPartClass[ListOfSimuPartIdL2[ipart]];
//     if(ClassType==4){
//       for(int ihit=0;ihit<ParticleNHits[ipart];ihit++){
// 	int iHitIndex=ParticleFirstHitIdx[ipart]+ihit;
// 	if(HitmoduleID[iHitIndex]==1){
// 	  double RminSP=3000;
// 	  int jhitmin=-1;
// 	  for(int jhit=0;jhit<ParticleNHits[ipart];jhit++){\
// 	    int jHitIndex=ParticleFirstHitIdx[ipart]+jhit;
// 	    if(HitmoduleID[jHitIndex]==2){
// 	      double U1=HitRecoULaddermm[iHitIndex];
// 	      double U2=HitRecoULaddermm[jHitIndex];
// 	      double dU=abs(U1-U2);
//
// 	      double V1=HitRecoVLaddermm[iHitIndex];
// 	      double V2=HitRecoVLaddermm[jHitIndex];
// 	      double dV=abs(V1-V2);
//
// 	      double dR=sqrt( pow(dU,2) + pow(dV,2) );
// 	      if(dR<=RminSP){
// 		RminSP=dR;
// 		jhitmin=jhit;
// 	      }
// 	    }//End if jHit is on M2
// 	  }//End for jhit
// 	  double Angle=(TMath::ATan(ep/RminSP))*(180./TMath::Pi());
// 	  if(Angle>0.5){
// 	    h_AngularDistributionTslL2->Fill(Angle);
// 	  }
// 	}//End IF Ihit is on M1
//
//       }//End For ihit
//     }//End if TSL
//   }//End loop on L2 parts
// // cout<<"**********************END Ladder 2"<< endl;
// return;
// }
//=======================================================================================================
void DBeaster::FillListOfRecoPartsSPnew(){

  double NbPartsM1=ListOfRecoPartsM1_SC.size();
  double NbPartsM2=ListOfRecoPartsM2_SC.size();
  double NbRecoPartsSP=0;

 if(NbPartsM1!=0 && NbPartsM2!=0){
  for(int ipart=0;ipart<NbPartsM1;ipart++){
    int ipartNbHits=ListOfRecoPartsM1_SC[ipart].HitList.size();

    for(int jpart=0;jpart<NbPartsM2;jpart++){
      int jpartNbHits=ListOfRecoPartsM2_SC[jpart].HitList.size();
      int NbCorrespondance=0;
      for(int ihit=0;ihit<ipartNbHits;ihit++){

			//int ihitRecoPartID_SP=ListOfBeastHitsOnM1[ihit].RecoId_SP;
				int ihitRecoPartID_SP=ListOfRecoPartsM1_SC[ipart].HitSpIdList[ihit];

				for(int jhit=0;jhit<jpartNbHits;jhit++){

				  //int jhitRecoPartID_SP=ListOfSpIdM2[jhit];
				  int jhitRecoPartID_SP=ListOfRecoPartsM2_SC[jpart].HitSpIdList[jhit];

				  if((ihitRecoPartID_SP!=-1)&&(ihitRecoPartID_SP == jhitRecoPartID_SP)){
				    NbCorrespondance++;
				  }

				}//END loop on hits from jpart

      }//END loop on hits from ipart
		  if( (NbCorrespondance==1) && ( ( (ipartNbHits==1) && (jpartNbHits==1) ) || (ListOfRecoPartsM1_SC[ipart].IsRec_ALreco || ListOfRecoPartsM2_SC[jpart].IsRec_ALreco ) ) ){
				ListOfRecoPartsM1_SC[ipart].IsRec_SPreco=true;
				ListOfRecoPartsM2_SC[jpart].IsRec_SPreco=true;
				ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
				ListOfRecoParts_SP[NbRecoPartsSP].HitList.insert(ListOfRecoParts_SP[NbRecoPartsSP].HitList.end(),ListOfRecoPartsM2_SC[jpart].HitList.begin(),ListOfRecoPartsM2_SC[jpart].HitList.end());

				double ep=2.27;
				// double U1=HitRecoULaddermm[ListOfRecoPartsM1_SC[ipart].HitList[0]];
        double U1=ListOfRecoPartsM1_SC[ipart].HitList[0].hitPositionLadderUVW(0);
				// double U2=HitRecoULaddermm[ListOfRecoPartsM2_SC[jpart].HitList[0]];
        double U2=ListOfRecoPartsM2_SC[ipart].HitList[0].hitPositionLadderUVW(0);
				double dU=abs(U1-U2);

				// double V1=HitRecoVLaddermm[ListOfRecoPartsM1_SC[ipart].HitList[0]];
        double V1=ListOfRecoPartsM1_SC[ipart].HitList[0].hitPositionLadderUVW(1);
				// double V2=HitRecoVLaddermm[ListOfRecoPartsM2_SC[jpart].HitList[0]];
        double V2=ListOfRecoPartsM2_SC[ipart].HitList[0].hitPositionLadderUVW(1);
				double dV=abs(V1-V2);

				double dR=sqrt( pow(dU,2) + pow(dV,2) );
				double Angle=(TMath::ATan(ep/dR))*(180./TMath::Pi());
				//cout<<"Angle="<<Angle <<endl;
				if((6.>Angle) || (Angle>20.)){
					  //cout<<"TSL Candidat"<<endl;
					ListOfTslCandidat.push_back(NbRecoPartsSP);
				}
				NbRecoPartsSP++;
		  }
		  if(NbCorrespondance>1){
				ListOfRecoPartsM1_SC[ipart].IsRec_SPreco=true;
				ListOfRecoPartsM2_SC[jpart].IsRec_SPreco=true;
				ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
				ListOfRecoParts_SP[NbRecoPartsSP].HitList.insert(ListOfRecoParts_SP[NbRecoPartsSP].HitList.end(),ListOfRecoPartsM2_SC[jpart].HitList.begin(),ListOfRecoPartsM2_SC[jpart].HitList.end());
				NbRecoPartsSP++;
		  }
		  if(ipart==(NbPartsM1-1)){
				if( !(ListOfRecoPartsM2_SC[jpart].IsRec_SPreco) ){
				  ListOfRecoParts_SP.push_back(ListOfRecoPartsM2_SC[jpart]);
				  NbRecoPartsSP++;
				}
		  }
    }//END loop on parts from M2
    if( !(ListOfRecoPartsM1_SC[ipart].IsRec_SPreco)){
      ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
      NbRecoPartsSP++;
    }
  }//END loop on parts from M1
 }//END IF NbParts!=0
 if(NbPartsM1==0){
  for(int jpart=0;jpart<NbPartsM2;jpart++){
    ListOfRecoParts_SP.push_back(ListOfRecoPartsM2_SC[jpart]);
  }
 }
 if(NbPartsM2==0){
  for(int ipart=0;ipart<NbPartsM1;ipart++){
    ListOfRecoParts_SP.push_back(ListOfRecoPartsM1_SC[ipart]);
  }
 }
  return;
  std::vector< vector<int> > v;
}
//=======================================================================================================
void DBeaster::FillTslCandidat(){
  int NbTslCandidat=ListOfTslCandidat.size();
  if(NbTslCandidat>1){
    for(int ipart=0;ipart<NbTslCandidat;ipart++){
      int IdxSpReco=ListOfTslCandidat[ipart];
      if(ipart==0){
	ListOfRecoParts_SP.push_back(ListOfRecoParts_SP[IdxSpReco]);
      }else{
      int NbSpRecoPart=ListOfRecoParts_SP.size();
      ListOfRecoParts_SP[NbSpRecoPart-1].HitList.insert(ListOfRecoParts_SP[NbSpRecoPart-1].HitList.end(),ListOfRecoParts_SP[IdxSpReco].HitList.begin(),ListOfRecoParts_SP[IdxSpReco].HitList.end());
      }
      ListOfRecoParts_SP.erase(ListOfRecoParts_SP.begin()+IdxSpReco);
    }
  }
return;
}
//===========================================================================================================
void DBeaster::HelixFitBuildUpStudy(int iLadder){

  // ListOfTsIpCandidat
  // tTracker = GetTracker();
  // aPlane = tTracker->GetPlane(1);
  // DR3 PosXYZ = aPlane->GetPosition();
  // std::cout << "--------oooo00OO00oooo-----oooo00OO00oooo" << '\n';
  // std::cout << "PosXYZ -> X = "<< (PosXYZ)(0)
  //           << "PosXYZ -> Y = "<< (PosXYZ)(1)
  //           << "PosXYZ -> Z = "<< (PosXYZ)(2)<< '\n';
  // Dprec  = aPlane->GetPrecAlignment();
  // std::cout << "------------------------------------------" << '\n';
  // std::cout << "x1000" << '\n';
  // DR3 PosXYZTest((PosXYZ)(0)*1e-3,(PosXYZ)(1)*1e-3,(PosXYZ)(2)*1e-3);
  // DR3 HelixPosUVW = Dprec->TransformHitToPlane(PosXYZTest);
  // std::cout << "PosUVW -> U = "<< (HelixPosUVW)(0)
  //           << " PosUVW -> V = "<< (HelixPosUVW)(1)
  //           << " PosUVW -> W = "<< (HelixPosUVW)(2)<< '\n';
  // std::cout << "------------------------------------------" << '\n';
  // std::cout << "x1" << '\n';
  // DR3 HelixPosUVWbis = Dprec->TransformHitToPlane(PosXYZ);
  // std::cout << "PosUVW -> U = "<< (HelixPosUVWbis)(0)
  //           << " PosUVW -> V = "<< (HelixPosUVWbis)(1)
  //           << " PosUVW -> W = "<< (HelixPosUVWbis)(2)<< '\n';
  // std::cout << "--------oooo00OO00oooo-----oooo00OO00oooo" << '\n';

  BuildMcTrueTsFromIpList(iLadder);
  // BuildMcTrueTsList(iLadder);
  // BuildFakeRecoTsList(iLadder);
  return;
}
//=======================================================================================================
void DBeaster::BuildMcTrueTsFromIpList(int iLadder){
  MCInfoHolder = tTracker->GetMCInfoHolder();
  ListOfTsIpCandidat.clear();
  if(iLadder == 1 ){
    int NParts=ListOfSimuPartIdL1.size();
    for(int ipart=0;ipart<NParts;ipart++){
      int iPartIdx = ListOfSimuPartIdL1[ipart];
      if( ListOfSimuPartClass[iPartIdx] == 2 ){
        ListOfTsIpCandidat.push_back(MCInfoHolder->GetASimParticle(iPartIdx));
      }
    }
  }
  if(iLadder == 2){
    int NParts=ListOfSimuPartIdL2.size();
    for(int ipart=0;ipart<NParts;ipart++){
      int iPartIdx = ListOfSimuPartIdL2[ipart];
      if(ListOfSimuPartClass[iPartIdx] == 2 ){
        ListOfTsIpCandidat.push_back(MCInfoHolder->GetASimParticle(iPartIdx));
      }
    }
  }
  TVector3 HelixParameter;
  DHelixFitter *aHelixFitter;
  aHelixFitter=GetHelixFitter();
  TVector3 r1;
  TVector3 r2;
  TVector2 r1local;
  TVector2 r2local;
  std::vector<TVector3> ListOfHitsTsIpCandidat;
  int NbParts=ListOfTsIpCandidat.size();
  std::cout << "NPART = " <<NbParts << '\n';
  for(int ipart=0;ipart<NbParts;ipart++){

    int NbHits      = ListOfTsIpCandidat[ipart].NHits;
    std::cout << "NHits = "<<NbHits << '\n';
    int FirstHitIdx = ListOfTsIpCandidat[ipart].FirstHitIdx;
    if(ListOfTsIpCandidat[ipart].NHits != 2) continue;
    r1 = MCInfoHolder->GetASimHit(FirstHitIdx + 0).PosInXYZmm;
    r1local= MCInfoHolder->GetASimHit(FirstHitIdx + 0).PosAveUVmm;
    r2 = MCInfoHolder->GetASimHit(FirstHitIdx + 1).PosInXYZmm;
    r2local = MCInfoHolder->GetASimHit(FirstHitIdx + 1).PosAveUVmm;

    double px = MCInfoHolder->GetASimHit(FirstHitIdx + 0).MomentumInXYZMeV.X();
    double py = MCInfoHolder->GetASimHit(FirstHitIdx + 0).MomentumInXYZMeV.Y();
    double pz = MCInfoHolder->GetASimHit(FirstHitIdx + 0).MomentumInXYZMeV.Z();
    double pt = sqrt( pow(px,2) + pow(py,2));
    // if(pt<200){
    //   std::cout << "TRANSVERSE MOMEMTUM Less than 200 MeV" << '\n';
    //   continue;
    // }else{
      ListOfHitsTsIpCandidat.clear();
      ListOfHitsTsIpCandidat.push_back(r1);
      ListOfHitsTsIpCandidat.push_back(r2);
      TVector3 r3(int(MCInfoHolder->GetASimHit(FirstHitIdx + 0).sensorID),
             int(MCInfoHolder->GetASimHit(FirstHitIdx + 1).sensorID),
               0.);
      ListOfHitsTsIpCandidat.push_back(r3);

      HelixParameter = aHelixFitter->GetHelixParameter(ListOfHitsTsIpCandidat);
      std::cout << "Sensor ID 1 = "<< MCInfoHolder->GetASimHit(FirstHitIdx + 0).sensorID<<" Sensor ID 2 = "<< MCInfoHolder->GetASimHit(FirstHitIdx + 1).sensorID<< '\n';
      std::cout << "Transverse   Momentum = " << pt <<"[MeV]"<< '\n';
      std::cout << "Longitudinal Momentum = " << pz <<"[MeV]"<< '\n';
      double TanL = pz/pt;
      std::cout << "TanLambda = "<< TanL << '\n';
      // std::cout << "Momentum Hit 0 = " <<  MCInfoHolder->GetASimHit(FirstHitIdx + 0).MomentumInXYZMeV.Mag() << '\n';

      std::cout << "Fitted Tansverse Momentum = " << HelixParameter.X()*1000 <<"[MeV]"<< '\n';

      // std::cout << "Position 1 X = "<< r1.X() << "[mm]"
      //           << ", Y = "<< r1.Y() << "[mm]"
      //           << ", Z = "<< r1.Z() << "[mm]" << '\n';
      // std::cout << "Position 2 X = "<< r2.X() << "[mm]"
      //           << ", Y = "<< r2.Y() << "[mm]"
      //           << ", Z = "<< r2.Z() << "[mm]" << '\n';
      // std::cout << "Position 1 U = "<< r1local.X() << "[mm]"
      //           << ", V = "<< r1local.Y() << "[mm]"
      //           <<  '\n';
      // std::cout << "Position 2 U = "<< r2local.X() << "[mm]"
      //           << ", V = "<< r2local.Y() << "[mm]"
      //           << '\n';
      // DR3 R1Local;
      // DR3 R1LabBis;
      // DR3 R1Lab(r1.X()*1e3,r1.Y()*1e3,r1.Z()*1e3);
      // DR3 R2Local;
      // DR3 R2LabBis;
      // DR3 R2Lab(r2.X()*1e3,r2.Y()*1e3,r2.Z()*1e3);
      //
      // aPlane = tTracker->GetPlane(r3.X()+1);
      // Dprec  = aPlane->GetPrecAlignment();
      // R1Local = Dprec->TransformHitToPlane(R1Lab);
      // R1LabBis = Dprec->TransformHitToTracker(R1Local);
      // aPlane = tTracker->GetPlane(r3.Y()+1);
      // Dprec  = aPlane->GetPrecAlignment();
      // R2Local = Dprec->TransformHitToPlane(R2Lab);
      // R2LabBis = Dprec->TransformHitToTracker(R2Local);
      //
      // std::cout << "Test TransformHitToPlane" << '\n';
      // std::cout << "Position 1 U = "<< (R1Local)(0)/1000<< "[mm]"
      //           << ", V = "<< (R1Local)(1)/1000 << "[mm]"
      //           << ", W = " << (R1Local)(2)/1000 << '\n';
      // std::cout << "Position 2 U = "<< (R2Local)(0)/1000 << "[mm]"
      //           << ", V = "<< (R2Local)(1)/1000 << "[mm]"
      //           << ", W = " << (R2Local)(2)/1000 << '\n';
      //           std::cout << "*------ RLab Bis -------*" << '\n';
      // std::cout << "Position 1 X = "<< (R1LabBis)(0) << "[mm]"
      //           << ", Y = "<< (R1LabBis)(1) << "[mm]"
      //           << ", Z = "<< (R1LabBis)(2) << "[mm]" << '\n';
      // std::cout << "Position 2 X = "<< (R2LabBis)(0)/1000 << "[mm]"
      //           << ", Y = "<< (R2LabBis)(1)/1000 << "[mm]"
      //           << ", Z = "<< (R2LabBis)(2)/1000 << "[mm]" << '\n';
    // }

  }
  return;
}
//=======================================================================================================
void DBeaster::BuildMcTrueTsList(int iLadder){
  MCInfoHolder = tTracker->GetMCInfoHolder();
  ListOfTsCandidat.clear();
  if(iLadder == 1 ){
    int NParts=ListOfSimuPartIdL1.size();
    for(int ipart=0;ipart<NParts;ipart++){
      int iPartIdx = ListOfSimuPartIdL1[ipart];
      if( ListOfSimuPartClass[iPartIdx]== 3 ){
        ListOfTsCandidat.push_back(MCInfoHolder->GetASimParticle(iPartIdx));
      }
    }
  }
  if(iLadder == 2){
    int NParts=ListOfSimuPartIdL2.size();
    for(int ipart=0;ipart<NParts;ipart++){
      int iPartIdx = ListOfSimuPartIdL2[ipart];
      if(ListOfSimuPartIdL2[iPartIdx] == 3 ){
        ListOfTsCandidat.push_back(MCInfoHolder->GetASimParticle(iPartIdx));
      }
    }
  }
  TVector3 HelixParameter;
  DHelixFitter *aHelixFitter;
  aHelixFitter=GetHelixFitter();
  TVector3 r1;
  TVector3 r2;

  std::vector<TVector3> ListOfHitsTsCandidat;
  int NbParts=ListOfTsCandidat.size();
  for(int ipart=0;ipart<NbParts;NbParts++){

    int NbHits      = ListOfTsCandidat[ipart].NHits;
    int FirstHitIdx = ListOfTsCandidat[ipart].FirstHitIdx;
    if(ListOfTsCandidat[ipart].NHits != 2) continue;
    r1 = MCInfoHolder->GetASimHit(FirstHitIdx + 0).PosInXYZmm;
    r2 = MCInfoHolder->GetASimHit(FirstHitIdx + 1).PosInXYZmm;
    // r1=ListOfTsIpCandidat[ipart].HitList[0].hitPositionLabXYZ;
    // r2=ListOfTsIpCandidat[ipart].HitList[1].hitPositionLabXYZ;
    ListOfHitsTsCandidat.clear();
    ListOfHitsTsCandidat.push_back(r1);
    ListOfHitsTsCandidat.push_back(r2);
    HelixParameter = aHelixFitter->GetHelixParameter(ListOfHitsTsCandidat);
  }
  return;
}

//=======================================================================================================
void DBeaster::BuildFakeRecoTsList(int iLadder){
  ListOfRecoTS.clear();
  ListOfFakeRecoTs.clear();
  MCInfoHolder = tTracker->GetMCInfoHolder();
  if(iLadder == 1){
    int Npart = ListOfBeastRecoParts_L1.size();
    for(int ipart=0;ipart<Npart;ipart++){
      if(ListOfBeastRecoParts_L1[ipart].ClassType == 2){
        ListOfRecoTS.push_back(ListOfBeastRecoParts_L1[ipart]);
      }
    }
  }
  if(iLadder == 2){
    int Npart = ListOfBeastRecoParts_L2.size();
    for(int ipart=0;ipart<Npart;ipart++){
      if(ListOfBeastRecoParts_L2[ipart].ClassType == 2){
        ListOfRecoTS.push_back(ListOfBeastRecoParts_L2[ipart]);
      }
    }
  }
  for(int jpart=0;jpart<ListOfRecoTS.size();jpart++){
    bool IsFake = false;
    for(int ihit=0;ihit<ListOfRecoTS[jpart].HitList.size();ihit++){
      int McHitId     = ListOfRecoTS[jpart].HitList[ihit].McHitId;
      int McPartIdx   = MCInfoHolder->GetASimHit(McHitId).ParticleIdx;
      int McClassType = ListOfSimuPartClass[McPartIdx];
      if(McClassType !=2) IsFake = true;
    }
    if(IsFake) ListOfFakeRecoTs.push_back(ListOfRecoTS[jpart]);
   }

   TVector3 HelixParameter;
   DHelixFitter *aHelixFitter;
   aHelixFitter=GetHelixFitter();
   TVector3 r1;
   TVector3 r2;
   TVector3 r3; // Contain Sensor Number for both hits
   std::vector<TVector3> ListOfHitsFakeRecoTs;
   int NbParts=ListOfFakeRecoTs.size();
   for(int ipart=0;ipart<NbParts;NbParts++){
     if(ListOfFakeRecoTs[ipart].HitList.size() != 2) continue;

     r1 = MCInfoHolder->GetASimHit(ListOfFakeRecoTs[ipart].HitList[0].McHitId).PosInXYZmm;
     r2 = MCInfoHolder->GetASimHit(ListOfFakeRecoTs[ipart].HitList[1].McHitId).PosInXYZmm;
     TVector3 r3(MCInfoHolder->GetASimHit(ListOfFakeRecoTs[ipart].HitList[0].McHitId).sensorID,
            MCInfoHolder->GetASimHit(ListOfFakeRecoTs[ipart].HitList[1].McHitId).sensorID,
              0.);
     // r1=ListOfTsIpCandidat[ipart].HitList[0].hitPositionLabXYZ;
     // r2=ListOfTsIpCandidat[ipart].HitList[1].hitPositionLabXYZ;
     ListOfHitsFakeRecoTs.clear();
     ListOfHitsFakeRecoTs.push_back(r1);
     ListOfHitsFakeRecoTs.push_back(r2);
     ListOfHitsFakeRecoTs.push_back(r3);

     HelixParameter = aHelixFitter->GetHelixParameter(ListOfHitsFakeRecoTs);
   }
  return;
}
