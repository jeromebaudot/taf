// @(#)maf/maf:$Name:  $:$Id: MAnalysis.cxx v.1 2005/10/02 18:03:46 sha Exp $
// Author: J. Baudot 2010/08/16
// Last modified: JB 2010/08/25 new method MiniVectors
// Last modified: JB 2011/06/18 MimosaPro
// Last modified: JB 2011/07/01 evt counter printout for MimosaFakerate
// Last modified: MG 2011/07/08 MimosaPro
// Last modified: JB 2011/07/18 MimosaPro
// Last modified: JB 2011/10/30 MimosaMiniVectors
// Last modified: JB 2011/11/23 MimosaPro and MimosaMiniVectors
// Last modified: JB 2011/11/25 MimosaPro
// Last modified: SS 2011/12/05 MimosaPro
// Last modified: JB 2012/09/05 all PixelSize changed to pixelSizeU or V
// Last modified: JB 2012/01/30 check thePlaneNumber is within known planes
// Last modified: JB 2013/05/01 method MimosaPro2Planes added
// Last Modified: BH 2013/08/20, warning corrections from clang
// Last Modified: JB 2013/08/21 MimosaPro
// Last Modified: SS 2013/10/04 MimosaPro
// Last Modified: JB 2013/11/08,11 MimosaCalibration
// Last Modified: JB 2014/01/10 MimosaProLadder
// Last Modified: JH 2014/06/16 145MimosaImaging
// Last Modified: VR 2014/06/30 replace DTIR by fWorkingDirectory in gSystem->cd() or ChangeDirectory()
// Last Modified: AP 2015/06/09 : Piece of code in MimosaPro to identify duplicated hits (same position and different time-stamp)
//                                Important for Mi22THRB, significant effect on cluster multiplicity due to this duplication


  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Defines some analysis methods used by MAF              //
  //  Mainly the main loop over all events : MimosaPro()     //
  //                                                         //
  ////////////////////////////////////////////////////////////

#ifndef _MimosaAnalysis_included_
#include "MAnalysis.h"
#endif
#include "DSetup.h"
#include "DTrack.h"

#include <TLatex.h>

ClassImp(MimosaAnalysis)

//______________________________________________________________________________
//
void MimosaAnalysis::MimosaPro(Int_t   MaxEvt,
			       Float_t TrackHitDist,
			       Float_t S2N_seed, Float_t S2N_neighbour,
			       Int_t   submatrix , Int_t GeoMatrix ,
			       Option_t*  SaveAlign,
			       Int_t UseHitMap, Int_t WriteMissedHits, Int_t WriteGoodHits)
{

  // -- Main processing macro for MIMOSA root DSF file
  //
  // Main analysis method: loops over all events found in a runXXX_0X.root file
  // Fills all histograms
  //
  // Modified: JB 2009/09/01, complete re-organisation for multi-tracks
  // Modified: JB 2009/09/01, alignement,
  // Modified: JB 2009/09/09, investigating bad track-hit matching
  // Modified: JB 2011/06/09, investigating bad track-hit matching
  // Modified: MG 2011/07/08, histograms added for efficiency map
  // Modified: JB 2011/07/18, dir name changed for missed hits file
  // Modified: JB 2011/11/04, call to generic method to compute efficiency
  // Modified: JB 2011/11/23, call to generic method for hot hit map
  // Modified: JB 2011/11/25, small fix to allow MimosaAlign
  // Modified: JB 2013/06/21, read more cuts from configuration file
  // Modified: JB 2013/08/21, new cut on seed pixel index range
  // Modified: SS 2013/10/04, new format for TheWriteMissedHits with option 2
  // Modified: JB 2014/01/21, new cut on cluster charge
  // Modified: AP 2015/01/13, new plot of efficiency vs Track to hit distace

  if(TString(SaveAlign) == TString("yes")) fSession->GetSetup()->GetAnalysisPar().SavePlots = false;

  if(!CheckIfDone("init,clear")) return;
  if(!ThePlaneNumber || ThePlaneNumber>fSession->GetTracker()->GetPlanesN()) {Warning("MimosaPro()","Please set a plane number!"); return; }
  if(!RunNumber) Error("MimosaPro","RunNumber not set! Please run InitSession  first");

  InitMimosaType();
  Info("MimosaPro","Mimosa type %d", MimosaType);

  GetAnalysisGoal();
  fIfReferenceTrack = kTRUE;

  if (!theCorFile) InitCorPar( RunNumber, ThePlaneNumber);
  cout << "InitCorPar OK" << endl ;
  gSystem->ChangeDirectory(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory


  //----ADC
  // ADC Readout !=0 if Readout is >=200
  // Modified JB 2009/07/20
  //Int_t ADCReadout=0;
  //if( (fSession->GetSetup()->GetPlanePar(ThePlaneNumber)).Readout > 199 ) {
  //  ADCReadout = (fSession->GetSetup()->GetPlanePar(ThePlaneNumber)).Readout%200;
  //}
  //----ADC

  //----------------------------------------------------------------------------------
  // -- Open the input file containing the TTree
  //----------------------------------------------------------------------------------

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0) {
    Error("MimosaPro"," The input file contains an incorrect number of events %d!",Nevt);
  }
  else {
    Info("MimosaPro","There is %d events in the input file.",Nevt);
  }

  //----------------------------------------------------------------------------------
  // -- Chose the plane(matrix) and possibly the sub-matrix.
  //----------------------------------------------------------------------------------

  ThesubmatrixNumber = submatrix;

  //----------------------------------------------------------------------------------
  // -- Init analysis variables
  //----------------------------------------------------------------------------------

  // for counting hits and tracks
  Int_t   ngoodhit    = 0; // # good hits in the DUT
  Int_t   NRecHit     = 0; // total # hits in the DUT
  Int_t   nmiss       = 0; // # good tracks in the DUT without a matched hit, JB 2009/09/08
  Int_t   nmissh      = 0; // # good tracks in the DUT with no good hit in the DUT, JB 2009/09/08
  Int_t   nmissthdist = 0; // # good tracks in the DUT with hit outside the distance limit

  Int_t Nhitinevent; // JB 2009/09/08
  Int_t Ngoodhitinevent; // # good hits in the event
  Int_t Ngoodtrackinevent; // # good tracks in the telescope in the event
  Int_t NgoodtrackinDUTinevent; // # good tracks in the telescope and in the DUT in the event

  // Counter on event status
  Int_t StatusEvent[10];
  for ( Int_t i=0 ; i<10 ; i++ ) {
    StatusEvent[i]=0;
  }


  //  To calculate parameters to prove the posibility of selection improvement of clusters
  const Short_t NCut      =  500   ;
  Float_t VCutMin         = -10.0 ;
  Float_t VCutMax         =  50.0 ;
  Int_t NKeepHit[NCut]   ;
  Int_t NKeepHitOk[NCut] ;
  Float_t VCut[NCut]     ;
  //Float_t SignalThreshold;

  //Int_t *IndexOfGoodHitsInEvent = new Int_t[CUT_MaxNbOfHits];
  Int_t IndexOfGoodHitsInEvent[4000]; // enlarge to 4000, JB 2010/08/27
  // Bool_t FillHitSeparationRootFile =  kFALSE ;//kTRUE;// kFALSE ;


  //----------------------------------------------------------------------------------
  // CUTS
  //----------------------------------------------------------------------------------
  // only sets here the values specified as arguments to the function,
  // the rest of them will be loaded with GetParameters).
  // JB 2013/08/21

  // on hits
  CUT_S2N_seed =  S2N_seed ; // S/N(seed)
  CUT_S2N_neighbour =  S2N_neighbour ; // S/N(neighbours)

  // on tracks
  Thegeomatrix = GeoMatrix; // define the area of interest in the DUT
  TrackToHitDistanceLimit =  TrackHitDist ; // Distance between the track and the hit
  cout << "--- CUTS specified:" << endl;
  cout << "  min S/N ratio: for seed = " << CUT_S2N_seed << ", for neighbours (without seed) = " << CUT_S2N_neighbour << endl;
  cout << "  max distance track to hit " << TrackToHitDistanceLimit << endl;

  // ?
  //SignalThreshold=0.;
  for(Short_t ic=0 ; ic<NCut ; ic++){
    Float_t tmp = VCutMin + (VCutMax-VCutMin)/(NCut-1)*ic ;
    VCut[ic]       = tmp*calibration ;
    NKeepHit[ic]   = 0   ;
    NKeepHitOk[ic] = 0   ;
  }

  //---ab  Baseline cuts and previous bad events.
  // apply the baseline cut to avoid possible bad reset effects.

  //Bool_t usebaselinecut = kTRUE;// kFALSE ;
  Bool_t usebaselinecut = kFALSE ;
  //---ab
  Int_t Previousbadevent = 0 ;
  Int_t Previousbadevent2 = 0 ;
  Float_t Baselinecut = 1100.0;

  //----------------------------------------------------------------------------------
  // -- get parameters
  //----------------------------------------------------------------------------------

  GetParameters(); // JB 2010/07/23

  Info("MimosaPro","Analysis variables initialized");

  //----------------------------------------------------------------------------------
  // -- prepare displays
  //----------------------------------------------------------------------------------

  PrepareOnlineDisplay();

  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();

  //----------------------------------------------------------------------------------
  // -- get the parametrization of the Eta function
  //----------------------------------------------------------------------------------

  GetMiEta();

  //----------------------------------------------------------------------------------
  // -- prepare alignment parameters
  //----------------------------------------------------------------------------------

  GetAlignment();

  //----------------------------------------------------------------------------------
  //--- Store Good and/or Bad events in ASCII FILEs
  //----------------------------------------------------------------------------------
  // Updated from AB by JB, 2009/09/08

  Int_t TheWriteGoodHits = WriteGoodHits; // 0 = no; 1 = yes.
  Char_t Header_EventList[400];
  sprintf(Header_EventList,"%s/%d_pl%i_Sub%i_Seed%i_Neig%i_geomat%i_T2h%i_chi2%i_HitMap%i/good%d_pl%i_Sub%i_Seed%i_Neig%i_geomat%i_T2h%i_chi2%i,HitMap%i.dat", CreateGlobalResultDir(), RunNumber, ThePlaneNumber, ThesubmatrixNumber, (Int_t)CUT_S2N_seed, (Int_t)CUT_S2N_neighbour, Thegeomatrix, (Int_t)TrackToHitDistanceLimit, (Int_t)TrackChi2Limit, TheUsePixelMap, RunNumber, ThePlaneNumber, ThesubmatrixNumber, (Int_t)CUT_S2N_seed, (Int_t)CUT_S2N_neighbour, Thegeomatrix, (Int_t)TrackToHitDistanceLimit, (Int_t)TrackChi2Limit, TheUsePixelMap);
  if(TheWriteGoodHits > 0) {
    outFileGoodEvt.open(Header_EventList,ios::app);
    Info("MimosaPro","Good hits information will be logged in file %s\n", Header_EventList);
  }

  Int_t TheWriteMissedHits = WriteMissedHits; // 0 = no; 1 = yes; 2 = yes but short format.
  Char_t badEvtFileName[400];
  sprintf( badEvtFileName,"%s/bad%d_pl%i_Sub%i_Seed%i_Neig%i_geomat%i_T2h%i_chi2%i_HitMap%i.dat", CreateGlobalResultDir(), RunNumber, ThePlaneNumber, ThesubmatrixNumber, (Int_t)CUT_S2N_seed, (Int_t)CUT_S2N_neighbour, Thegeomatrix, (Int_t)TrackToHitDistanceLimit, (Int_t)TrackChi2Limit, TheUsePixelMap); // dir name corrected, JB 2011/07/18
  if(TheWriteMissedHits > 0) {
    outFileBadEvt.open(badEvtFileName,ios::app);
    Info("MimosaPro","Missed hits information will be logged in file %s, (open=%d)\n", badEvtFileName, outFileBadEvt.fail());
  }

  // store the cluster info in separate ascii file:
  Char_t Headerclusterdat[100];
  sprintf(Headerclusterdat,"HitSeparation/rawhit/newcluster_%d_%d.dat",RunNumber,ThePlaneNumber);
  ofstream outFile(Headerclusterdat);
  const Int_t NofCycle=5000; //was 100


  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;//50000;//40000; //was 2000, modifyed by JB Sept 2007 to match number of events for initialization (ped and noise)
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  // ************************************
  // Call init part of the analysis here

  HotPixel_init( UseHitMap); // JB 2011/11/23
  Efficiency_init();     // JB 2011/11/04
  ClusterShape_init();   // JB 2010/04/13
  ClusterCharges_init(); // JB 2010/06/03
  ClusterPosition_init();

  // ************************************


  // a bit of cleaning
  MainCanvas->Clear();

  Info("MimosaPro","\nReady to loop over %d events (over %d in the tree)\n", MaxEvent, (int)t->GetEntries());

  // **********************************************************************************
  // ********* MAIN LOOP ***************
  // **********************************************************************************

  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaPro","Reading event %d",ievt);



    // Event wise parameters

    Short_t NhitInPlane = 0;
    //Short_t NtrackInPlane = 0;

    //-- Initilisations

    Nhitinevent=0;
    Ngoodhitinevent=0;
    Ngoodtrackinevent=0;
    NgoodtrackinDUTinevent=0;
    for ( Int_t i=0 ; i<10 ; i++ ) { StatusEvent[i]=0; }

    //-- Update display

    if (ievt == MinEvent) { // JB, 2008/08/09
      selection->Draw();
      for( Int_t il=0; il<8; il++) {
        textLabel[il]->Draw();
      }
    }

    if(ievt/NofCycle*NofCycle == ievt || ievt<10){
      if( selection->GetMaximum()>15*selection->GetBinContent(6)) MainCanvas->SetLogy(); // JB 2010/04/28
      MainCanvas->Modified();
      MainCanvas->Update();
      cout << "MimosaPro Event " << ievt <<endl;
      if (NtrkInMimo>100){
        cout<<"  temporary efficiency = "<<NofClMatchTrack<<" / "<<NtrkInMimo<<" = "<<1.*NofClMatchTrack/NtrkInMimo<<endl;
      }

    }

    //-- Update counter for efficiency

    StatusEvent[1]++;
    if(ievt / NeventRangeForEfficiency * NeventRangeForEfficiency == ievt ){ ievt_array ++; }

    //=========================
    if(MimoDebug>1) cout << " getting the event" << endl;
    t->GetEvent(ievt);
    //=========================

    DEventHeader& CurrentEventHeader=Evt->GetHeader();
    //cout << "Nframes in events " << ievt << " is " << CurrentEventHeader.GetNumberOfFrames() << endl;

    //---Store events in ascii file:
    /* DEventHeader& CurrentEventHeader=Evt->GetHeader();
     if(TheWriteGoodHits>1){
     // trigger number event(evt), evt-ievt ,mimosa loop event (ievt):
     outFileGoodEvt<<"####### evt RealEventNumber, evt-ievt , ievt"<<CurrentEventHeader.GetEventNumber()<<" "
     <<CurrentEventHeader.GetEventNumber()-ievt <<" "
     <<ievt<<endl;
     }
     */
    //    cout<<"####### evt RealEventNumber, evt-ievt , ievt"<<CurrentEventHeader.GetEventNumber()<<" "
    //	<<CurrentEventHeader.GetEventNumber()-ievt <<" "<<ievt<<endl;

    if(MimoDebug>1) cout << " getting the planes, tracks " << Evt->fT1PlanesN << " and hits" << endl;
    TClonesArray *Trpl   = Evt->GetTransparentPlanes() ; //tracks
    TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
    TClonesArray *Planes = Evt->GetAuthenticPlanes()   ; //planes
    Int_t NbOfTrpl   = Trpl->GetLast()+1   ; // total # tracks over all planes
    Int_t NbOfHits   = Hits->GetLast()+1   ; // total # hits over all planes

    // check to avoid crash due to empty event, added by JB, Sept 2008,
    // cut NbOfHits==0 removed since there may be no hit at all but the event should still be taken into account for efficiency, JB 2012/06/08
    if( NbOfTrpl==0 ) {
      if (MimoDebug > 0) Info("MimosaPro","Empty event %d: #tracks %d\n", ievt, NbOfTrpl);
      continue;
    }

    //Find the plane :
    Int_t okP = 0, okT = 0 ; // two ok added instead of one, JB Sept 2007
    DAuthenticPlane *ThePlaneStudied = 0; // init
    Int_t j=0; // init
    while(j<Planes->GetLast()+1 && okP==0) {
      ThePlaneStudied=(DAuthenticPlane*) Planes->At(j) ;
      //printf("testing %d against %d\n", ThePlaneStudied->Ppk, ThePlaneNumber);
      if (ThePlaneStudied->Ppk == ThePlaneNumber) {
        okP=1 ;
        NhitInPlane = ThePlaneStudied->PhN ;
        //NtrackInPlane = ThePlaneStudied->PtN ;
      }
      j++;
    }


    // Find the tracks in this plane and count them with okT
    // JB 2009/07/20
    DTransparentPlane *aTrpl;
    j = 0; // init
    while( j<NbOfTrpl ) {
      aTrpl=(DTransparentPlane*) Trpl->At(j) ;
      if (aTrpl->Tpk == ThePlaneNumber) {
        okT++ ;
      }
      j++;
    }


    // ****************** Filling histos
    selection->Fill(1.);  // all events

    //-- Reference plane study
    for ( Int_t iipl=0 ; iipl<TMath::Min(NbOfTrpl, 10) ; iipl++ ) {
      aTrpl = (DTransparentPlane*) Trpl->At(iipl) ;
      // select reference planes
      if( aTrpl->Tpk != ThePlaneNumber ) { // if this is not the DUT
        if (MimoDebug>3) Info("MimosaPro","Ref plane %d, track pos (%.0f, %.0f), dist.2hit (%.1f, %.1f), chi2 %f, slope (%.2f, %.2f)", aTrpl->Tpk, aTrpl->Tu, aTrpl->Tv, aTrpl->Tud, aTrpl->Tvd, aTrpl->Tchi2, aTrpl->TDOX, aTrpl->TDOY);
        hRef_Tud_vs_Tv[iipl]->Fill(aTrpl->Tu,aTrpl->Tud);
        hRef_Tud_vs_Tu[iipl]->Fill(aTrpl->Tv,aTrpl->Tud);
        hRef_Tud_vs_TDOX[iipl]->Fill(aTrpl->TDOX,aTrpl->Tud);
        hRef_Tud_vs_TDOY[iipl]->Fill(aTrpl->TDOY,aTrpl->Tud);
        hRef_Tud_vs_Chi2[iipl]->Fill(aTrpl->Tchi2,aTrpl->Tud);
        hRef_Tud_vs_Tu1[iipl]->Fill(aTrpl->Tu1,aTrpl->Tud);
        hRef_Tud_vs_Tk1[iipl]->Fill(aTrpl->Tk1,aTrpl->Tud);
      } // end if this is not the DUT
    }
    // ******************


    //----ab    CUT ON THE BASELINE TO AVOID BAD RESET EFFECTS:
    if( ((ThePlaneStudied->PFrfr1)<Baselinecut) && (usebaselinecut==kTRUE) ){okT=0; okP=0;}

    //---------------------------------------------------------------
    // if Both TransparentPlane and AuthenticPlane exist : (JB, Sept 2008)
    // and no baseline cut
    // and number of hits and tracks are within limits
    if ( okP && okT && NbOfHits<CUT_MaxNbOfHits && NbOfHits>=CUT_MinNbOfHits && NbOfTrpl>0) { // Plane exists with tracks and hits
      StatusEvent[2]++;

      // ****************** Filling histos
      selection->Fill(2.); // event with track(s) and hits in the plane
      hnhit->Fill(NhitInPlane);
      hnhitievt->Fill(ievt,NhitInPlane);
      hraw1Noise->Fill(float(ThePlaneStudied->PFn));
      hraw1Pedestal->Fill(float(ThePlaneStudied->PFp));
      hraw1CDS->Fill(float(ThePlaneStudied->PFr));
      hraw1Signal->Fill(float(ThePlaneStudied->PFq));
      hraw1Noise_time->Fill(ievt,float(ThePlaneStudied->PFn));
      hraw1Pedestal_time->Fill(ievt,float(ThePlaneStudied->PFp));
      hraw1CDS_time->Fill(ievt,float(ThePlaneStudied->PFr));
      hraw1Signal_time->Fill(ievt,float(ThePlaneStudied->PFq));
      // ******************

      double RU[2],RV[2];
      RU[0] = -NofPixelInRaw    * PixelSizeU/2.0;
      RU[1] = +NofPixelInRaw    * PixelSizeU/2.0;
      RV[0] = -NofPixelInColumn * PixelSizeV/2.0;
      RV[1] = +NofPixelInColumn * PixelSizeV/2.0;


      //---------------------------------------------------------------
      // Loop over hits BUT keep only the ones in the right plane
      for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

        DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);
        if( ahit->Hpk != ThePlaneNumber ) continue; // select only hits in DUT
        StatusEvent[6]++;
        Nhitinevent++;
        NRecHit++;

        Int_t Hotpixel = HotPixel_test( ahit->Hsk); // JB 2011/11/23

        // Now compute variables usefull for cuts

        NofPixelsInCluster = (ahit->HNNS < MaxNofPixelsInCluster)?ahit->HNNS:MaxNofPixelsInCluster;
        if (MimoDebug>2) Info("MimosaPro"," NofPixelsInCluster is set to %d (in hit %d, max %d)\n", NofPixelsInCluster, ahit->HNNS, MaxNofPixelsInCluster);

        // signal over noise ratio of the seed pixel
        Float_t snSeed;
        if( ahit->Hsn != 0.0 ) {
          snSeed=ahit->Hq0/ahit->Hn0/NoiseScope;
        }
        else { // for sparsified readout, JB 2009/05/19
          snSeed=ahit->Hq0;
        }
        if(MimoDebug>2) printf("MimosaPro: snSeed computation with Hsn=%f, Hq0=%f, Hn0=%f, NoiseScope=%f => snSeed=%.1f\n", ahit->Hsn, ahit->Hq0, ahit->Hn0, NoiseScope, snSeed);


        // charge and S/N in the second highest pixel of the hit
        // among the 4 pixels neighbouring the seed (cross)
        Float_t q2nd = 0.;
        Float_t sn2nd = 0.;
        for(Int_t i=0; i < NofPixelsInCluster; i++){
          //cout << "i=" << i << " index=" << ahit->HkM[i]- ahit->HkM[0] << " q=" << ahit->HqM[i] << endl;
          if( (abs(ahit->HkM[i]- ahit->HkM[0])==1 || abs(ahit->HkM[i]- ahit->HkM[0])== NofPixelInRaw)
             && ahit->HqM[i]>q2nd	){
            q2nd = ahit->HqM[i];
            if( ahit->HnM[i] != 0.0 ) {
              sn2nd = q2nd/ahit->HnM[i]/NoiseScope;
            }
            else { // for sparsified readout, JB 2009/05/19
              sn2nd = q2nd;
            }
          }
        }

        // charge of the cluster without the seed
        ChargeAroundSeed = (ahit->Hqc - ahit->Hq0)*calibration;

        // what the hell is this ?? JB 2010/06/03
        for(Short_t ic=0 ; ic<NCut ; ic++){
          if (ChargeAroundSeed>VCut[ic]){
            NKeepHit[ic]++ ;
          }
        }

        if(MimoDebug>2) {
          printf("MimosaPro: Hit %d, qSeed=%.1f, qAroundSeed=%.1f, hot=%d, snSeed=%.1f, sn2nd=%.1f\n", iHit, ahit->Hq0, ChargeAroundSeed, Hotpixel, snSeed, sn2nd);
        }

        // ****************** Filling histos
        selection->Fill(6.);  // all hits in MIMOSA

        h2dallhits->Fill( ahit->Hsu, ahit->Hsv);
        hallSNneighbour->Fill(ahit->HSNneighbour);
        hAllS2N->Fill(snSeed);
        hallSN_vs_SNN->Fill(ahit->HSNneighbour,snSeed);
        hQseedAll->Fill(ahit->HqM[0]*calibration);
        hseedQvsS2NAll->Fill(snSeed,ahit->HqM[0]*calibration);
        hS2N2nd->Fill(sn2nd);
        if(snSeed>sn2nd) { hS2N2All->Fill(snSeed,sn2nd); }
        else { hS2N2All->Fill(sn2nd,snSeed); }
        etal[0]->Fill(ChargeAroundSeed) ;
        if((ahit->Hq0)!=0.0){
          etal1[0]->Fill((ahit->Hqc/ahit->Hq0) - 1.0) ;
        }
        else {
	  //cout << "ahit->HNNS = " << ahit->HNNS << endl;
	  if(ahit->HNNS > 0) Warning("MimosaPro","evt %d, segment WARNING ahit->Hq0 = 0", ievt);
	}

        // ******************


        //--------------------------------------------------------------
        // If hit passes the selection cut
        if(
              ( Hotpixel == 0)
         //&& ( ahit->HqM[0]*calibration>0. ) // why is it usefull ? JB 2013/11/08
           && ( ahit->HsnPulse / ahit->Hsn >= CUT_S2N_seed )
           && ( ahit->HSNneighbour >= CUT_S2N_neighbour) //YV 27/1109 you add these 2 lines if you want S/N cuts
           && ( ChargeAroundSeed >= CUT_MinQ_neighbour )  // JB 2013/11/08
           && ( ahit->Hq0 >= CUT_Q_seed ) // JB 2013/11/08
           && ( ahit->Hqc >= CUT_Q_cluster ) // JB 2014/01/21
           && ( MinNofPixelsInCluster <= ahit->HNNS && ahit->HNNS <= MaxNofPixelsInCluster ) // JB 2013/09/11
           && ( CUT_MinSeedIndex==CUT_MaxSeedIndex ||
                (CUT_MinSeedIndex<=ahit->Hsk && ahit->Hsk<=CUT_MaxSeedIndex) ) // allows to select an index range for the seed pixel (ineffective if minIndex==maxIndex), JB 2013/08/21
           && ( CUT_MinSeedCol==CUT_MaxSeedCol ||
               (CUT_MinSeedCol<=(ahit->Hsk%NofPixelInRaw) && (ahit->Hsk%NofPixelInRaw)<=CUT_MaxSeedCol) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
           && ( CUT_MinSeedRow==CUT_MaxSeedRow ||
               (CUT_MinSeedRow<=(ahit->Hsk/NofPixelInRaw) && (ahit->Hsk/NofPixelInRaw)<=CUT_MaxSeedRow) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
           //&& 256<=(ahit->Hsk%NofPixelInColumn) && (ahit->Hsk%322)<=319 // S1:0-95, S2:96-191, S3:192-255, S4:256-319, JB 2013/08/22
           ){ // if hit passes cuts
          StatusEvent[7]++;
          ngoodhit++;
          IndexOfGoodHitsInEvent[Ngoodhitinevent] = iHit;
          Ngoodhitinevent++;
          if(MimoDebug>1) cout << " found good hit " << Ngoodhitinevent-1 << " with index " << IndexOfGoodHitsInEvent[Ngoodhitinevent-1] << " pos " << ahit->Hu << ";" << ahit->Hv << " and dist to track " << ahit->Hu-ahit->Htu << ";" << ahit->Hv-ahit->Htv << endl;

          // ****************** Filling histos
          selection->Fill(7.); // Good hits in MIMOSA

	  //AP, 2014/10/29: Displacing histos outside the loop
	  //hnGOODhit->Fill(Ngoodhitinevent);
	  //hnahitievt->Fill( ievt, Ngoodhitinevent);
          GoodHit_Fill( ahit); // JB 2010/10/06
          hgoodSN_vs_SNN->Fill(ahit->HSNneighbour,snSeed);
          BinarySpecific_HitsOnly_fill( ahit);  // JB 2010/06/03
          if( ahit->Htk >= 0) { // if a track was associated in Tree
            Float_t adist = sqrt((ahit->Hsu-ahit->Htu)*(ahit->Hsu-ahit->Htu)+(ahit->Hsv-ahit->Htv)*(ahit->Hsv-ahit->Htv)) ;
            duvall->Fill( adist); //For all the clusters with Hqc-Hq0>Cut
            hAllHuvsAllTu->Fill( ahit->Htu, ahit->Hsu);
            hAllHuvsAllTv->Fill( ahit->Htv, ahit->Hsu);
            hAllHvvsAllTu->Fill( ahit->Htu, ahit->Hsv);
            hAllHvvsAllTv->Fill( ahit->Htv, ahit->Hsv);
            //	    if(fabs( sqrt( (ahit->Htu-ahit->Hu)*(ahit->Htu-ahit->Hu) + (ahit->Htv-ahit->Hv)*(ahit->Htv-ahit->Hv) ) ) > TrackToHitDistanceLimit){
            //	    }
          } // end if a track was associated
          // ******************

        } // end if hit passes cuts
        //--------------------------------------------------------------

        else { // if hit does not pass cuts
          if(MimoDebug>1) Info("MimosaPro","Hit not passing selection cuts, ChargeAroundSeed=%f <> %f, seedCharge=%.1f <> %.0f, hotPixel %d", ChargeAroundSeed, CUT_MaxQ_neighbour, ahit->HqM[0]*calibration, 0., Hotpixel);
        }

      } // end loop on hits
      //--------------------------------------------------------------



      //AP 2015/06/09
      //Peace of code to identify duplicate hits, i.e., hits mainly in the same position but with different
      //time-stamp and pixel-multiplicity.
      Int_t IndexOfGoodHitsInEvent_tmp[4000];
      Int_t Ngoodhitinevent_tmp = 0;
      Int_t IndexOfDuplicatedHits[4000][2];
      Int_t NgoodhitDuplicateCounter = 0;
      for(Int_t iHit1=0; iHit1<Ngoodhitinevent; iHit1++) { // 1st loop over good hits
	DAuthenticHit* ahit1 = (DAuthenticHit*)Hits->At(IndexOfGoodHitsInEvent[iHit1]);
	bool IsAlreadyInDuplicateList = false;
	for(Int_t iHit_dup=0; iHit_dup<NgoodhitDuplicateCounter; iHit_dup++) {
	  if(IndexOfGoodHitsInEvent[iHit1] == IndexOfDuplicatedHits[iHit_dup][0] ||
	     IndexOfGoodHitsInEvent[iHit1] == IndexOfDuplicatedHits[iHit_dup][1]) {
	    IsAlreadyInDuplicateList = true;
	    break;
	  }
	}
	if(IsAlreadyInDuplicateList) continue;

	bool HasDuplicate = false;
	for(Int_t iHit2=iHit1+1; iHit2<Ngoodhitinevent; iHit2++) { // 2nd loop over good hits
	  DAuthenticHit* ahit2 = (DAuthenticHit*)Hits->At(IndexOfGoodHitsInEvent[iHit2]);

	  bool IsAlreadyInDuplicateList2 = false;
	  for(Int_t iHit_dup=0; iHit_dup<NgoodhitDuplicateCounter; iHit_dup++) {
	    if(IndexOfGoodHitsInEvent[iHit2] == IndexOfDuplicatedHits[iHit_dup][0] ||
	       IndexOfGoodHitsInEvent[iHit2] == IndexOfDuplicatedHits[iHit_dup][1]) {
	      IsAlreadyInDuplicateList2 = true;
	      break;
	    }
	  }
	  if(IsAlreadyInDuplicateList2) continue;

	  Double_t distanceU = TMath::Abs(ahit1->Hu  - ahit2->Hu);
	  Double_t distanceV = TMath::Abs(ahit1->Hv  - ahit2->Hv);
	  Double_t Delta_TS  = TMath::Abs(ahit1->HTS - ahit2->HTS);

	  if((distanceU <= PixelSizeU) &&
	     (distanceV <= PixelSizeV) &&
	     (Delta_TS  <= 2) //For the moment time window is +/-2 to identify the duplicated hits
	     ) {
	    HasDuplicate = true;
	    //Found duplicated hit. Chose the one with the lower Time-Stamp:
	    Int_t TheIndex1 = -999;
	    Int_t TheIndex2 = -999;
	    if(ahit1->HTS - ahit2->HTS >0) {
	      TheIndex1 = IndexOfGoodHitsInEvent[iHit2];
	      TheIndex2 = IndexOfGoodHitsInEvent[iHit1];
	    }
	    else {
	      TheIndex1 = IndexOfGoodHitsInEvent[iHit1];
	      TheIndex2 = IndexOfGoodHitsInEvent[iHit2];
	    }

	    //Put list of duplicated hits in a list
	    IndexOfDuplicatedHits[NgoodhitDuplicateCounter][0] = TheIndex1;
	    IndexOfDuplicatedHits[NgoodhitDuplicateCounter][1] = TheIndex2;
	    NgoodhitDuplicateCounter++;

	    break;
	  }

	} //end of 2nd loop over good hits

	//If hit is not duplicated put it on temporal list of good hits
	if(!HasDuplicate) {
	  IndexOfGoodHitsInEvent_tmp[Ngoodhitinevent_tmp] = IndexOfGoodHitsInEvent[iHit1];
	  Ngoodhitinevent_tmp++;
	}

      } //end of 1st loop over good hits

      //Loop over duplicated hits and put on the tamporal list the one with the earliest time-stamp
      for(Int_t iHit=0; iHit<NgoodhitDuplicateCounter; iHit++) { // loop over good duplicated hits
	IndexOfGoodHitsInEvent_tmp[Ngoodhitinevent_tmp] = IndexOfDuplicatedHits[iHit][0];
	Ngoodhitinevent_tmp++;
      } //end of loop over good duplicated hits


      //Now replace the List of good hits with the temporal list. In this way eliminate the dupplicated hits for the
      //estimation of detection efficiency, spatial resolution and cluster multiplicity
      Ngoodhitinevent = Ngoodhitinevent_tmp;
      for(Int_t iHit=0; iHit<Ngoodhitinevent; iHit++) IndexOfGoodHitsInEvent[iHit] = IndexOfGoodHitsInEvent_tmp[iHit];

      hnGOODhit->Fill(Ngoodhitinevent);
      hnahitievt->Fill( ievt, Ngoodhitinevent);

      if(MimoDebug) Info("MimosaPro","End loop on hits, found %d good hits in event %d", Ngoodhitinevent, ievt);
      const Int_t NofCycle=1000; //was 100

      //---------------------------------------------------------------
      // FIRST Loop on tracks to count how many are in the EXCLUSION GEOMATRIX
      Int_t Ngoodtrackgeominevent = 0;
      Int_t NgoodtrackSensorinevent = 0;
      for (Int_t iTrk=0 ; iTrk<NbOfTrpl ; iTrk++){ // loop on tracks

        aTrpl = (DTransparentPlane*) Trpl->At(iTrk);
        if( aTrpl->Tpk != ThePlaneNumber ) continue; // select only tracks in DUT
        TrackParameters_compute(aTrpl, alignement); // compute tu,tv

        if( TrackInMimo( GeoMatrixForTrackCut,tu,tv, ThesubmatrixNumber) /* && aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 < TrackChi2Limit */) {
          Ngoodtrackgeominevent++;
        }

	//Check if track passes through the sensor full surface
        if(tu >= RU[0] && tu <= RU[1] &&
	   tv >= RV[0] && tv <= RV[1]) {
	  NgoodtrackSensorinevent++;
        }
      } // end loop on tracks

      NTracksPerEventInSensor = NgoodtrackSensorinevent;
      hNGoodGeomTracksPerEvent->Fill( Ngoodtrackgeominevent);
      hnTracksinSensorPerEvent->Fill(NgoodtrackSensorinevent);
      double DeltaU  = RU[1] - RU[0];
      DeltaU        *= 1.0e-4; //transform from microns to cm
      double DeltaV  = RV[1] - RV[0];
      DeltaV        *= 1.0e-4; //transform from microns to cm
      double Surface = DeltaU*DeltaV*0.5;
      double TracDensity = Ngoodtrackgeominevent / Surface;
      double StupidTracDensity = TracDensity;
      hTrackDensity->Fill(StupidTracDensity);
      if( MimoDebug>1) Info("MimosaPro"," Number of tracks in EXCLUSION GEOMATRIX %d is %d.", GeoMatrixForTrackCut , Ngoodtrackgeominevent);



      //---------------------------------------------------------------
      // Loop on tracks BUT keep only the ones in the right plane
      for (Int_t iTrk=0 ; iTrk<NbOfTrpl ; iTrk++){ // loop on tracks

        aTrpl = (DTransparentPlane*) Trpl->At(iTrk);
        if( aTrpl->Tpk != ThePlaneNumber ) continue; // select only tracks in DUT

        StatusEvent[3]++;

        // ****************** Filling histos
        selection->Fill(3.); // all tracks in plane

        TrackParameters_allFill( aTrpl, ievt);
        // ******************

        if( MimoDebug>1) Info("MimosaPro"," Track %d has chi2=%f <> %f ?", iTrk, aTrpl->Tchi2, TrackChi2Limit);

        //---------------------------------------------------------------
        // if chi2 of selected track is OK
        if ( aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 < TrackChi2Limit // JB, chi2=0. is not a real pb
            && aTrpl->TtHn>=MinHitsPerTrack
            //&& aTrpl->Tdx<0.002 && aTrpl->Tdy<0.002 // insure track is flat, NOT MANDATORY, JB sept 2007
            && (MaxNbOfTracksInGeom<0 || Ngoodtrackgeominevent<=MaxNbOfTracksInGeom) /* added for MIMOSA32 case, JB 2012/08/20*/
            ){ // if good track
          StatusEvent[4]++;
          Ngoodtrackinevent++;

          TrackParameters_compute(aTrpl, alignement);

          /*******************/
          selection->Fill(4.);   // Good quality track

          hNTracksPerEvent->Fill( Ngoodtrackinevent);
          hNTracksPerEventievt->Fill( ievt, Ngoodtrackinevent);

          TrackParameters_goodFill( aTrpl, ievt);

          /*******************/

          Bool_t TrkCrossMimo = TrackInMimo( Thegeomatrix, tu ,tv, ThesubmatrixNumber);

          if( MimoDebug) Info("MimosaPro"," Is track %d in MIMOSA ? %d", iTrk, TrkCrossMimo);

          //---------------------------------------------------------------
          // if track is in the good geometry range
          if( TrkCrossMimo == kTRUE ){ // if the track is in MIMOSA
            StatusEvent[5]++;
            NgoodtrackinDUTinevent++;
            NtrkInMimo++ ;

	    hnTracksInGeomatrixVsTrackPerEvent->Fill(NgoodtrackSensorinevent);

            Previousbadevent2 = Previousbadevent;
            Previousbadevent = ievt;
            temp_NtrkInMimo[ievt_array]++;


            //-- Find the nearest good hit to this track

            TrackToHitDistance    = 10000000.0 ;
            TrackToHit2ndDistance = 10000000.0 ;
            DAuthenticHit *thehit    = 0; //This will be a real hit
            //DAuthenticHit *the2ndhit = 0;
            DAuthenticHit *ahit      = 0;
            Float_t adist;
	    Int_t TheHitIdx = -1;

            if( Ngoodhitinevent<=0 ) { // no hit at all, JB 2009/09/08
              nmissh++;
            }

            for(Int_t iHit=0; iHit<Ngoodhitinevent; iHit++) { // loop over good hits
              ahit = (DAuthenticHit*)Hits->At( IndexOfGoodHitsInEvent[iHit] );
              if(MimoDebug>1) cout << " Good hit " << iHit << " selected at index " << IndexOfGoodHitsInEvent[iHit] << " with position " << ahit->Hu << ";" << ahit->Hv << endl;
              adist = sqrt( (ahit->Hu-tu)*(ahit->Hu-tu) + (ahit->Hv-tv)*(ahit->Hv-tv) );
              if (adist<TrackToHitDistance) {
		//cout << " Good hit " << iHit << " selected at index " << IndexOfGoodHitsInEvent[iHit] << " with position " << ahit->Hu << ";" << ahit->Hv << endl;
		//the2ndhit = thehit;
                thehit    = ahit;
		TheHitIdx = IndexOfGoodHitsInEvent[iHit];
		TrackToHit2ndDistance = TrackToHitDistance;
                TrackToHitDistance    = adist;
		//cout<<ievt<<" DISTANCES "<<TrackToHitDistance<<" "<<TrackToHit2ndDistance<<endl;
              }
            } // end loop over good hits


            //-- Check the nearest hit is within the requested distance

            Bool_t aHitIsFound = kFALSE ;
            if( fabs(TrackToHitDistance) < TrackToHitDistanceLimit
               //&& fabs(ahit->Hv-tv)<200. // specific case, JB 2012/11/20
               //&& fabs(ahit->Hu-tu)<1000. // specific case, JB 2012/11/20
               ){
              aHitIsFound = kTRUE ;
            }
            else if( thehit ) { // hit there but too far away, JB 2009/09/08
              nmissthdist++;
            }

	    //AP, 13/01/2015
	    //Cumulation of hits for calculation of efficiecy vs Track-hit distance
	    for(int ItrkHitDis=0;ItrkHitDis<effi_vs_TrkHitDist->GetXaxis()->GetNbins();ItrkHitDis++) {
	      double Tmp_TrackToHitDistanceLimit = effi_vs_TrkHitDist->GetBinCenter(ItrkHitDis+1);
	      if(fabs(TrackToHitDistance) < Tmp_TrackToHitDistanceLimit) {
		effi_vs_TrkHitDist->SetBinContent(ItrkHitDis+1,effi_vs_TrkHitDist->GetBinContent(ItrkHitDis+1) + 1);
	      }
	    }


            // ****************** Filling histos
            selection->Fill(5.); // Good track in MIMOSA

            hnGOODhitwhentrack->Fill(Ngoodhitinevent);
            hnahitievt->Fill( ievt, Ngoodhitinevent);
            hTrackToClusterMinDistance->Fill(TrackToHitDistance); // Minimal distance from track to hits
	    if(TrackToHit2ndDistance<999999.0){
	      hTrackTo2ndclosestClusterDistance->Fill(TrackToHit2ndDistance); // 2nd Minimal distance from track to hits
	      hMinDistance_vs_2ndDistance->Fill(TrackToHit2ndDistance,TrackToHitDistance);
              /*
	      bool PrintTooClose2ndHitToTrack = false;
	      //PrintTooClose2ndHitToTrack = true;
	      double Pitch_distance = sqrt(pow(PixelSizeU,2) + pow(PixelSizeV,2));
	      if(TrackToHit2ndDistance<Pitch_distance && PrintTooClose2ndHitToTrack){
		double mycol,mylin;
		double mycol_2nd,mylin_2nd;
		ComputePixelPosition_UVToColRow(thehit->Hu,   thehit->Hv,   mycol,    mylin);
		ComputePixelPosition_UVToColRow(the2ndhit->Hu,the2ndhit->Hv,mycol_2nd,mylin_2nd);
		cout << endl;
		cout <<"CLOSE TEST: ievt " << ievt << ", TheHit (col,lin) = (" << mycol << "," << mylin << "); TS_hit = " << thehit->HTS << ", hit_mult = " << thehit->HNNS
		     << "; The2ndHit (col,lin) = (" << mycol_2nd << ","
		     << mylin_2nd << "); TS_2ndhit = " << the2ndhit->HTS << ", 2ndhit_mult = " << the2ndhit->HNNS << "; "
		     << "TrkToHitDistance = " << TrackToHitDistance << ", TrkTo2ndHitDistance = " << TrackToHit2ndDistance
		     << ", DeltaU/PitchU = " << (double)(thehit->Hu - the2ndhit->Hu)/PixelSizeU
		     << ", DeltaV/PitchV = " << (double)(thehit->Hv - the2ndhit->Hv)/PixelSizeV
		     << ", pitch_distance = " << Pitch_distance
		     << endl;
		cout << endl;
              }
              */
	    }
            TrkInMimo->Fill(tu,tv);  //MG 2011/07/08  for efficency 2D plot

            // ******************

            if(MimoDebug>1) cout << " Nearest hit to track at " << TrackToHitDistance << " <?> " << TrackToHitDistanceLimit<< ", hit found = " << aHitIsFound << endl;

            //---------------------------------------------------------------
            // If a good hit near this track was found
            if ( aHitIsFound ){ // if a hit matching the track was found
              StatusEvent[8]++;
              StatusEvent[9]++;
              NofClMatchTrack++;

	      //Fill histograms related with duplicated matched hits
	      //Check if matched hit is duplicated
	      Int_t IdxDuplicatedMatchedHit = -999;
	      for(Int_t iHit_dup=0; iHit_dup<NgoodhitDuplicateCounter; iHit_dup++) { // loop over good duplicated hits
		if(TheHitIdx == IndexOfDuplicatedHits[iHit_dup][0]) IdxDuplicatedMatchedHit = iHit_dup;
	      } //end of loop over good duplicated hits
	      if(IdxDuplicatedMatchedHit != -999) {
		DAuthenticHit* ahit1 = (DAuthenticHit*)Hits->At(IndexOfDuplicatedHits[IdxDuplicatedMatchedHit][0]);
		DAuthenticHit* ahit2 = (DAuthenticHit*)Hits->At(IndexOfDuplicatedHits[IdxDuplicatedMatchedHit][1]);

		hDuplicate_2DMult->Fill(ahit1->HNNS,ahit2->HNNS);
		hDuplicate_DeltaTS->Fill(ahit2->HTS - ahit1->HTS);
		hDuplicate_npixc->Fill(ahit1->HNNS);
		hDuplicate_npixc_vs_TrkDistToDiode->Fill(ahit1->HNNS,GetTrackDistantToClosestDiode(tu,tv));
	      }

	      hnGOODhitInGeomatrixVsTrackPerEvent->Fill(NgoodtrackSensorinevent);

              temp_NofClMatchTrack[ievt_array]++; // for time dependent efficiency

              // what the hell is this ?? JB 2010/06/03
              for(Short_t ic=0 ; ic<NCut ; ic++){
                if (ChargeAroundSeed>VCut[ic]){
                  NKeepHitOk[ic]++ ;
                }
              }


              // ****************************************
              // Analysis of matched hits & tracks

              NofPixelsInCluster = (thehit->HNNS < MaxNofPixelsInCluster)?thehit->HNNS:MaxNofPixelsInCluster;

              ClusterCharges_compute( thehit);  // JB 2010/06/03
              ClusterPosition_compute( thehit, alignement);  // JB 2010/06/03
              //if(MimoDebug>1) cout << " hit   in plane (Plane frame) " << hU << ", " << hV << ", " << hW << "." << endl;

              //TrackToHitDistance= sqrt((hU-tu)*(hU-tu)+(hV-tv)*(hV-tv)); // recomputed with best hit position
              // ************************************


              // ************************************
              // Fill data for precision alignement
              // get the computed hit position if not too far from digital position
              // JB 2009/09/01
              if( abs(thehit->Hu - thehit->Hsu) < 5.*PixelSizeU
                 && abs(thehit->Hv - thehit->Hsv) < 5.*PixelSizeV ){
                alignement->NewData( thehit->Hu, thehit->Hv,
				     thehit->Hresu, thehit->Hresv,
				     tx, ty, tz, tdx, tdy); // more precise
              }
              else{
                alignement->NewData( thehit->Hsu, thehit->Hsv,
				     thehit->Hresu, thehit->Hresv,
				     tx, ty, tz, tdx, tdy);  // more robust.
              }
              // ************************************



              // ************************************
              // --- write into the file info about the cluster
              //---- to be written in an ASCII file to be used for the hit separation macros.
              if(MimoDebug) {
                cout << "--- NofPixelsInCluster=" << NofPixelsInCluster << " - evt "<< ievt
                << " - HsnPulse = "<<thehit->HsnPulse
                << " - HsnNoise = "<<thehit->Hsn
                << " - HsnIndex = "<<thehit->HsnIndex
                << " - Hsn S/N = "<<thehit->HsnPulse / thehit->Hsn
                << "total charge=" << TotalCharge
                << endl;

              }

              if(TheWriteGoodHits>1){
                for(Int_t i=0; i < NofPixelsInCluster; i++){
                  outFileGoodEvt << "Evt "<< ievt << endl;
                  outFileGoodEvt << " qonly[" << i << "]="<< qonly[i]
                  << " noise[" << i << "]="<< noise[i]
                  << " snonly[" << i << "]="<< snonly[i]
                  << " indexofPix[" << i << "]="<< IndexofPix[i]
                  << endl;
                  outFileGoodEvt << " q[" << i << "]="<< q[i]
                  << " n[" << i << "]="<< n[i]
                  << " sn[" << i << "]="<< sn[i]
                  << " optimalSize = " << optimalSize
                  << endl;
                }
              }

              // fill the file with the informations
              /*if(TheWriteGoodHits==1){
               outFileGoodEvt <<"-GOOD ASSOCIATION --- RealEventNumber ="<<CurrentEventHeader.GetEventNumber()<<"; ievt = "<<ievt<<endl;
               outFileGoodEvt <<"TrackToHitDistance = "<<TrackToHitDistance<<endl;
               outFileGoodEvt <<"UofHitCG VofHitCG "<< UofHitCG<<" "<<VofHitCG <<endl;
               outFileGoodEvt <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3
               <<" tx=" << tx <<" ty=" << ty <<" tz=" << tz
               <<" tdx="<<tdx <<" tdy="<<tdy <<" hUdigital="
               << hUdigital<<" hVdigital="<< hVdigital<<" pixelsizeV"<< PixelSizeV<<endl;
               outFileGoodEvt<<"Eventheader: evt RealEventNumber, time delay Frame1 "<<CurrentEventHeader.GetEventNumber()<<" "<< CurrentEventHeader.GetTime()
               <<" "<<CurrentEventHeader.EtimeSinceLastEvent<<endl;
               outFileGoodEvt <<"-----"<<endl;
               }*/
              if (MimoDebug > 1) {
                cout <<"hUdigital="<< hUdigital<<" hVdigital="<< hVdigital<<" pixelsizeV="<< PixelSizeV<<endl;
                cout <<"UofHitCG="<< UofHitCG<<" VofHitCG="<<VofHitCG <<endl;
                cout <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3<<endl;
                cout<<" tx=" << tx <<" ty=" << ty <<" tz=" << tz <<" tdx="<<tdx <<" tdy="<<tdy <<" tu="<<tu<<" tv="<<tv<<endl;
                //cout << UCGcorr << " " << VCGcorr << " " << tx << " " << ty << " " << tz << " " << tdx << " " << tdy << endl;
                cout <<"---------------------"<<endl;
              }



              if(NofPixelsInCluster >20){
                outFile << tu << " ";
                outFile << tv << " ";
                outFile << UofPix[0] << " ";
                outFile << VofPix[0] << " ";
                outFile << UofHitEta2x2 << " ";
                outFile << VofHitEta2x2 << " ";
                for (Int_t itmp=0; itmp<NofPixelsInCluster; itmp++){
                  outFile << qonly[itmp] << " ";
                }
                for (Int_t itmp=0; itmp<NofPixelsInCluster; itmp++){
                  outFile << IndexofPix[itmp] << " ";
                }
                outFile << " \n";
              }
              // ************************************


              // ****************** Filling histos
              selection->Fill(8.); // good hit associated to a good track
              selection->Fill(9.);

              ClusterCharges_fill( thehit, ievt);   // JB 2010/06/03
              ClusterShape_fill( thehit);  // JB 2010/04/13
              BinarySpecific_fill( thehit);  // JB 2010/06/03
              ClusterPosition_fill( thehit); // JB 2010/07/21
              TrackHitPosition_fill( thehit, aTrpl, ievt); // JB 2010/07/22

              goodtracks->Fill( tu, tv); //MG 2010/06/10 "matched tracks"

	      hDist_Trck_Diode_Asso->Fill(GetTrackDistantToClosestDiode(tu,tv));
	      if(thehit->HNNS < huCGwidth_vs_Mult->GetXaxis()->GetNbins()) {
                for(int imult=0;imult<huCGwidth_vs_Mult->GetXaxis()->GetNbins()-1;imult++) {
                  if(thehit->HNNS == imult+1) {
	            hDist_Trck_Diode_Asso_vs_Mult[imult]->Fill(GetTrackDistantToClosestDiode(tu,tv));
	            break;
                  }
                }
	      }
              else {
		hDist_Trck_Diode_Asso_vs_Mult[huCGwidth_vs_Mult->GetXaxis()->GetNbins()-1]->Fill(GetTrackDistantToClosestDiode(tu,tv));
	      }
	      hnpixc_vs_TrkDistToDiode->Fill(thehit->HNNS,GetTrackDistantToClosestDiode(tu,tv));

              // Plane study
              //cout<<"+++++++++++++++"<<float(ThePlaneStudied->PFrfr1)<<endl;
              hraw1goodone->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1goodone_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1goodoneNoise_time->Fill(ievt,float(ThePlaneStudied->PFn)*calibration);
              hraw1PFrfr1GOOD->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr1GOOD_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr2GOOD->Fill(float(ThePlaneStudied->PFrfr2));
              hraw1PFrfr2GOOD_time->Fill(ievt,float(ThePlaneStudied->PFrfr2));
              hraw1NoiseGOOD_time->Fill(ievt,float(ThePlaneStudied->PFn));

              // ******************

              //-----------------------------
              // User's stuff
              // JB 2009/09/07
              //-----------------------------
              if(MimoDebug>1) cout << " STARTING user's stuff" << endl;

              for(Int_t iHit=0; iHit<Ngoodhitinevent; iHit++) { // loop over good hits
                ahit = (DAuthenticHit*)Hits->At( IndexOfGoodHitsInEvent[iHit] );
                if( thehit != ahit ) {
                  hUserHitCorrelationLine->Fill( ahit->Hsu - thehit->Hsu);
                  hUserHitCorrelationCol->Fill( ahit->Hsv - thehit->Hsv);
                }
              } // end loop over good hits


              if(MimoDebug) Info("MimosaPro","Done with good matched track-hit study");


            }  // end if a hit matching the track was found
            //---------------------------------------------------------------

            //---------------------------------------------------------------
            else { // end if no hit matching the track was found
              nmiss++;

	      //if(Ngoodtrackinevent == 1) {
		//double mycol,mylin;
		//ComputePixelPosition_UVToColRow(tu,tv,mycol,mylin);
	        //cout << "Evt = " << ievt << " has Inefficient track located at (u,v) = (" << tu << "," << tv << "); (col,lin) = (" << mycol << "," << mylin << ")" << endl;
	      //}

              if(ievt/NofCycle*NofCycle == ievt){
                cout << " Number of tracks missed = " << nmiss << endl;
              }


              //--- Do some print out

              if (MimoDebug) {
                cout <<"--INEFFICIENT event = "<<ievt<<endl;
              }

              if (MimoDebug>2) {

                cout << " -8-BAD TRACK-HIT DIST OR NO HIT evt = "<<ievt;
                cout << " dist= "<<TrackToHitDistance;
                cout << " ( " << Ngoodhitinevent << " hits in plane)" <<endl;
                cout << " -----------Info on track ----------" << endl;
                cout << " Index  of Track       = " << tk1  << endl;
                cout << " Chi2 of track         = " << chi2 << endl;
                cout << " Track slope (Tdu;Tdv) = " << tdu << " ; " << tdv << " --- In degrees :" << 57.29*acos(tdu) << " ; " << 57.29*acos(tdv)  << endl;    // 180:3.14=57.29577
                //cout <<"UofHitCG VofHitCG "<< UofHitCG<<" "<<VofHitCG <<endl;
                //cout <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3 << endl
                cout << " tx=" << tx <<" ty=" << ty <<" tz=" << tz
                <<" tdx="<<tdx <<" tdy="<<tdy <<endl;
                cout<<" tu= "<<tu<<" tv= "<<tv<<endl;

                if( Ngoodhitinevent>0 ) { // if there is at least a hit

                  cout << " -----------Info on nearest hit ----------" << endl;
                  cout << " Charge on Seed HqM[0]*cal         = " << thehit->HqM[0]*calibration << " = " << thehit->HqM[0] << "*" << calibration <<  endl;
                  cout << " thehit->Hqc * calibration         = " << thehit->Hqc * calibration  << endl;
                  cout << " thehit->Hq0 * calibration         = " << thehit->Hq0 * calibration  << endl;
                  cout << " ChargeAroundSeed = (thehit->Hqc-thehit->Hq0)*calibration = " << (thehit->Hqc-thehit->Hq0)*calibration <<  endl;
                  cout << " Noise  on Seed          = " << thehit->Hsn << endl;
                  cout << " SNR    on Seed          = ";
                  if( (thehit->Hsn)!=0. ) { cout << thehit->Hq0/(calibration*thehit->Hsn); }
                  else { cout << 0.; }
                  cout << endl;
                  cout << " Index  of Seed          = " << thehit->Hsk  << " column: " << (thehit->Hsk)%NofPixelInRaw << " -- line: "  << (thehit->Hsk)/NofPixelInRaw<< endl;
                  cout<<" hUdigital="<< thehit->Hsu <<" hVdigital="<< thehit->Hsv <<" pixelsizeV"<< PixelSizeV<<endl;

                  //cout << " Qseed (e-) = " << qonly[0];
                  //cout << " Charge on Seed q[0]               = " << q[0]      << endl;
                  //cout << " q[0] vs qonly[0] vs HqM[0]*cal    = " << q[0] << " vs " << qonly[0] << " vs " << thehit->HqM[0]*calibration << endl;
                  //cout << " q[1] vs qonly[1] vs HqM[1]*cal    = " << q[1] << " vs " << qonly[1] << " vs " << thehit->HqM[1]*calibration << endl;
                  //cout << " q[2] vs qonly[2] vs HqM[2]*cal    = " << q[2] << " vs " << qonly[2] << " vs " << thehit->HqM[2]*calibration << endl;
                  //cout << " q[3] vs qonly[3] vs HqM[3]*cal    = " << q[3] << " vs " << qonly[3] << " vs " << thehit->HqM[3]*calibration << endl;
                  //cout << " q[4] vs qonly[4] vs HqM[4]*cal    = " << q[4] << " vs " << qonly[4] << " vs " << thehit->HqM[4]*calibration << endl;
                  //cout << " q[6] vs qonly[6] vs HqM[6]*cal    = " << q[6] << " vs " << qonly[6] << " vs " << thehit->HqM[6]*calibration << endl;
                  //cout << " q[7] vs qonly[7] vs HqM[7]*cal    = " << q[7] << " vs " << qonly[7] << " vs " << thehit->HqM[7]*calibration << endl;
                  //cout << " q[8] vs qonly[8] vs HqM[8]*cal    = " << q[8] << " vs " << qonly[8] << " vs " << thehit->HqM[8]*calibration << endl;
                  //cout << " q[9] vs qonly[9] vs HqM[9]*cal    = " << q[9] << " vs " << qonly[9] << " vs " << thehit->HqM[9]*calibration << endl;
                  //cout << " q[10] vs qonly[10] vs HqM[10]*cal = " << q[10] << " vs " << qonly[10] << " vs " << thehit->HqM[10]*calibration << endl;
                  //cout << " q[11] vs qonly[11] vs HqM[11]*cal = " << q[11] << " vs " << qonly[11] << " vs " << thehit->HqM[11]*calibration << endl;
                  //cout << " q[12] vs qonly[12] vs HqM[12]*cal = " << q[12] << " vs " << qonly[12] << " vs " << thehit->HqM[12]*calibration << endl;
                  //cout << " q[13] vs qonly[13] vs HqM[13]*cal = " << q[13] << " vs " << qonly[13] << " vs " << thehit->HqM[13]*calibration << endl;
                  //cout << " q[14] vs qonly[14] vs HqM[14]*cal = " << q[14] << " vs " << qonly[14] << " vs " << thehit->HqM[14]*calibration << endl;

                  //cout << " -----------Comparison track - hit ----------" << endl;
                  //cout << " Tud = Hu-Tu  (TTree)  = " << tud << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Tvd = Hv-Tv  (TTree)  = " << tvd << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Hu                    = " << thehit->Hu << " ---UofHitCG5 = "  << UofHitCG5  << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Hv                    = " << thehit->Hv << " ---VofHitCG5 = "  << VofHitCG5  << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Tack to hit dist calc = " <<  sqrt( (UofHitCG5-tu)*(UofHitCG5-tu) + (VofHitCG5-tv)*(VofHitCG5-tv) ) << endl;

                } // end if there is at least one hit

                cout <<"-----"<<endl;

                cout <<"--NO HIT FOUND---Reco evt = "<<ievt<<endl;
                printf("event Pas de hit, impact: tu=%f  tv=%f  chi2 : %f\n",tu,tv,chi2);
                cout<< float(ThePlaneStudied->PFrfr1)<<endl;

                cout << "Event number = " <<Previousbadevent<<"; Previous event with a track in mimosa = "<<Previousbadevent2<< endl;
                cout<<"Eventheader: evt, time delay Frame1 "<<CurrentEventHeader.GetEventNumber()<<" "<< CurrentEventHeader.GetTime()
                <<" "<<CurrentEventHeader.EtimeSinceLastEvent<<" "<<ThePlaneStudied->PFrfr1<<endl;
                CurrentEventHeader.Print();
                cout <<"-----"<<endl;
              } // end if MimoDebug


              //----  fill the file with the infomations if requested
              if(TheWriteMissedHits==1){ // if write info for missed hits
                if (MimoDebug) cout << " Writing info for missed event " << ievt << endl;

                outFileBadEvt<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
                outFileBadEvt << "event counter = " << ievt << ", from DSF = " << CurrentEventHeader.GetEventNumber() << ", from daq = " << Evt->GetFrameNb() << endl;
                outFileBadEvt << "Event number = " << Previousbadevent << "; Previous event with a track in mimosa = " << Previousbadevent2 << endl;
                if( CurrentEventHeader.GetNumberOfTriggers() ) {
                  outFileBadEvt << "# triggers= " << CurrentEventHeader.GetNumberOfTriggers();
                  for( Int_t iTrig=0; iTrig<CurrentEventHeader.GetNumberOfTriggers(); iTrig++) {
                    outFileBadEvt << " : " << CurrentEventHeader.GetTriggerAt( iTrig);
                  }
                  outFileBadEvt << endl;
                }
                if( CurrentEventHeader.GetNumberOfFrames() ) {
                  outFileBadEvt << "# frames= " << CurrentEventHeader.GetNumberOfFrames();
                  for( Int_t iTrig=0; iTrig<CurrentEventHeader.GetNumberOfFrames(); iTrig++) {
                    outFileBadEvt << " : " << CurrentEventHeader.GetFrameAt( iTrig);
                  }
                  outFileBadEvt << endl;
                }
                if( CurrentEventHeader.GetNumberOfTimestamps() ) {
                  outFileBadEvt << "# timestamps= " << CurrentEventHeader.GetNumberOfTimestamps();
                  for( Int_t iTrig=0; iTrig<CurrentEventHeader.GetNumberOfTimestamps(); iTrig++) {
                    outFileBadEvt << " : " << CurrentEventHeader.GetTimestampAt( iTrig);
                  }
                  outFileBadEvt << endl;
                }

                outFileBadEvt << " total # hits in all planes = " << NbOfHits << endl;
                outFileBadEvt << " total # hits in the DUT = " << Nhitinevent << ", good hits = " << Ngoodhitinevent << endl;
                outFileBadEvt << " total # good tracks in telescope = " << Ngoodtrackinevent << endl;
                outFileBadEvt << " total # good tracks in the DUT = " << NgoodtrackinDUTinevent << endl;
                outFileBadEvt << " total # tracks in the EXCLUSION GEOMATRIX = " <<           Ngoodtrackgeominevent << endl;
                outFileBadEvt << " Track properties:" << endl;
                outFileBadEvt << "    tx = " << tx << ", ty = " << ty << ", tz = " << tz << endl;
                outFileBadEvt << "    tdx = " << tdx << ", tdy = " << tdy << endl;
                outFileBadEvt << "    tu = " << tu << ", tv = " << tv << " or col = " << (Int_t)(tu+FirstPixelShiftU)/PixelSizeU << ", row = " << (Int_t)(tv+FirstPixelShiftV)/PixelSizeV << endl;
                outFileBadEvt << "    chi2 = " << chi2 << endl;

                outFileBadEvt << " track to nearest hit distance = "<< TrackToHitDistance << endl;
                if( Ngoodhitinevent>0 ) { // if there is at least a hit
                  outFileBadEvt << " Nearest hit properties:" << endl;
                  outFileBadEvt << "   # pixels in hit " << thehit->HNNS << endl;
                  outFileBadEvt << "   (u, v) = (" << thehit->Hu << ", " << thehit->Hv << ")" << endl;
                  outFileBadEvt << "   seed (u, v) = (" << thehit->Hsu << ", " << thehit->Hsv << "), [col, row] = [" << (thehit->Hsu+FirstPixelShiftU)/PixelSizeU << ", " << (thehit->Hsv+FirstPixelShiftV)/PixelSizeV << "]" << endl;
                  outFileBadEvt << "   charge (w/o calib) in seed = " << thehit->Hq0 << ", around seed " << ahit->Hqc - ahit->Hq0 << " and in hit " << thehit->Hqc << endl;
                  outFileBadEvt << "   S/N of seed = " << thehit->HsnPulse / ahit->Hsn << ", around seed " << thehit->HSNneighbour << endl;
                } // end if there is at least a hit

                outFileBadEvt << "-----" << endl;
              } // end if write info for missed hits

              else if(TheWriteMissedHits==2){ // if write info for missed hits
                outFileBadEvt<<ievt<<","<<CurrentEventHeader.GetTriggerAt(1)<<","<<CurrentEventHeader.GetTriggerAt(0)<<","<<Ngoodhitinevent<<endl;
              }

              // ****************** Filling histos
              //MissedHit->cd();


              tuv->Fill(tu,tv);

              hchi2_nc->Fill(chi2);
              htu->Fill(tu);
              htv->Fill(tv);
              htuhtv->Fill(tu,tv);
              Int_t itu;
              Int_t itv;
              itu= Int_t(tu/PixelSizeU);
              itv= Int_t(tv/PixelSizeV);
              htuvInPix->Fill(tu-itu*PixelSizeU,tv-itv*PixelSizeV);

              hraw1badone->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1badone_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1badoneNoise_time->Fill(ievt,float(ThePlaneStudied->PFn)*calibration);
              hraw1PFrfr1BAD->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr1BAD_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr2BAD->Fill(float(ThePlaneStudied->PFrfr2));
              hraw1PFrfr2BAD_time->Fill(ievt,float(ThePlaneStudied->PFrfr2));
              hraw1NoiseBAD_time->Fill(ievt,float(ThePlaneStudied->PFn));

	      double u = (tu + 0.5*NofPixelInRaw    * PixelSizeU)/(2.0*PixelSizeU);
	      u = (u - int(u))*2.0*PixelSizeU;
	      double v = (tv + 0.5*NofPixelInColumn * PixelSizeV)/(2.0*PixelSizeV);
	      v = (v - int(v))*2.0*PixelSizeV;
	      huvCGtuv_NoAsso->Fill(u,v);
	      //compute diode position modulo:
	      //compute diode position modulo:
	      Double_t diodeu,diodev,diodew;
	      Double_t diodepitchW=10.0;
	      Int_t diodecol = -1;
	      Int_t diodelin = -1;
	      Double_t diodetrkdist=100000.0;
	      //cout<<"-------------NO ASSO TEST ---------"<<endl;
	      for(Int_t icol = 0;icol<4;icol++){
		for(Int_t ilin = 0;ilin<4;ilin++){
		  fTool.ComputeStripPosition(Matrixtype,diodecol,diodelin,diodeu,diodev,diodew,NofPixelInRaw,NofPixelInColumn,double(PixelSizeU),double(PixelSizeV),diodepitchW);
		  if(icol<3){
		    diodeu = (diodeu + 0.5*NofPixelInRaw    * PixelSizeU)/(2.0*PixelSizeU);
		    diodeu = (diodeu - int(diodeu))*2.0*PixelSizeU;
		  }else{
		    diodeu = (diodeu + 0.5*NofPixelInRaw    * PixelSizeU)/(2.0*PixelSizeU);
		    diodeu = (diodeu - int(diodeu))*2.0*PixelSizeU + 2.0*PixelSizeU;
		  }
		  if(ilin<3){
		    diodev = (diodev + 0.5*NofPixelInColumn * PixelSizeV)/(2.0*PixelSizeV);
		    diodev = (diodev - int(diodev))*2.0*PixelSizeV;
		  }else{
		    diodev = (diodev + 0.5*NofPixelInColumn * PixelSizeV)/(2.0*PixelSizeV);
		    diodev = (diodev - int(diodev))*2.0*PixelSizeV+ 2.0*PixelSizeV;
		  }
		  if(sqrt((u-diodeu)*(u-diodeu)+(v-diodev)*(v-diodev))<diodetrkdist){
		    diodetrkdist = sqrt((u-diodeu)*(u-diodeu)+(v-diodev)*(v-diodev));
		  }
		  //cout<<"NO ASSO icol ilin diodeu diodev u v diodetrkdist "<<icol<<" "<<ilin<<" diodeu="<<diodeu<<" diodev="<<diodev<<" u="<<u<<" v="<<v<<" "<<diodetrkdist<<endl;
		  diodelin++;
		}
		diodecol++;
	      }
	      //cout<<"------------END TEST ---------"<<endl;
	      //hDist_Trck_Diode_NoAsso->Fill(diodetrkdist);
	      hDist_Trck_Diode_NoAsso->Fill(GetTrackDistantToClosestDiode(tu,tv));

	      if( Ngoodhitinevent>0 ) { // if there is at least a hit (thehit is the nearest)
                hqc_nc->Fill(thehit->Hqc);
                hnpix_nc->Fill(thehit->HNNS); // Hsk); JB 2009/09/01
                huvBad->Fill( tu, tv);
                tuv1->Fill( thehit->Hsu, thehit->Hsv);
              }
              else {
                FalseHitMap->Fill(tu,tv);
              }

              // ******************

            } // end if no hit matching the track was found
            //---------------------------------------------------------------


          }  // end if the track is in MIMOSA
          //---------------------------------------------------------------

        } // end if good track
        //---------------------------------------------------------------

      } // end loop on tracks
      //---------------------------------------------------------------

      if(MimoDebug) Info("MimosaPro","End loop on tracks, found %d good tracks in DUT", NgoodtrackinDUTinevent);

    } // end if Plane exists with tracks and hits
    //---------------------------------------------------------------

    else { // if there was no plane or tracks or hits
      //Warning("MimosaPro","problem event %d with plane %d: Authentic %d?=1 or Transparent %d?=1 or #Hits %d<? %d <?%d  or #trpl %d>?0\n\n", ievt, ThePlaneNumber, okP, okT, CUT_MinNbOfHits, NbOfHits, CUT_MaxNbOfHits, NbOfTrpl);
      //return; // allow to continue and simply skip this event, JB 2009/09/01
    }


    //---------------------------------------------------------------
    //-- Do some statistics after event analysis
    //---------------------------------------------------------------
    // StatusEvent[6]>1 <=> there are some hits in the DUT
    // StatusEvent[7]>1 <=> there are some good hits in the DUT
    // StatusEvent[3]>1 <=> there are some tracks
    // StatusEvent[4]>1 <=> there is a good track
    // StatusEvent[5]>1 <=> there is a good track in the good geomatrix
    // StatusEvent[8/9]>1 <=> a hit is matched to the track

    if (MimoDebug > 1) {
      cout << "StatusEvent = ";
      for ( Int_t i=1 ; i<10 ; i++ ) {
        cout<<StatusEvent[i]<<" - ";
      }
      cout<<endl;
    }


    if(MimoDebug) Info("MimosaPro","Event %d ended\n",ievt);

  } // end of Main loop over event

  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  // ************************************
  // Call final part of analysis here
  Inefficent_end(); //AB 2015/09/04
  ClusterPosition_end(); // JB 2014/01/10
  ClusterCharges_end(); // Jb 2010/07/22
  ClusterShape_end();  // JB 2010/04/13
  BinarySpecific_end(); // JB 2010/06/03
  //FakeRate_end( NtrkInMimo); // JB 2010/07/22
  Efficiency_end( MaxEvent-MinEvent+1 ); // JB 2011/11/04

  // ************************************

  // Display final histograms
  MainCanvas->Clear();
  MainCanvas->SetBorderMode(0);
  MainCanvas->Divide(1,2);
  MainCanvas->cd(1);
  if( selection->GetMaximum()>15*selection->GetBinContent(6)) gPad->SetLogy();
  selection->Draw();
  for( Int_t il=0; il<8; il++) {
    textLabel[il]->Draw();
  }
  MainCanvas->cd(2);
  hClusterMeanForm->Draw("lego2");
  //hChargeIntegral4->Draw("surf");
  MainCanvas->Update();

  //---------------------------------------------------------------
  //-- Update and Save eta and alignment parameters
  //---------------------------------------------------------------

  // Process the distributions to get the correction functions:
  // Reshape to call single functions, JB 2011/06/19

  cout << "In the end... "<< endl;
  cout << "In the end... "<< endl;
  cout << "In the end... "<< endl;
  cout << "In the end... "<< endl;

  //if ( !strstr(SaveAlign,"no") || !strstr(SaveAlign,"NO") || !SaveAlign) {
  if ( strstr(SaveAlign,"yes") || strstr(SaveAlign,"YES") ) {

    CreateNewEta();
    fMimosaProDone=kTRUE; // required so that AlignMimosa proceeds, JB 2011/11/25
    AlignMimosa( TrackToHitDistanceLimit); // new option added, JB 2012/05/11

  }

  AverageEffic[0] = MimosaEfficiency*100.0;
  AverageEffic[1] = MimosaEfficiency_ERR*100.0;

  //AP, 13/01/2015
  //Final calculation of efficiecy vs Track-hit distance
  for(int ItrkHitDis=0;ItrkHitDis<effi_vs_TrkHitDist->GetXaxis()->GetNbins();ItrkHitDis++) {
    double Ntmp      = effi_vs_TrkHitDist->GetBinContent(ItrkHitDis+1);
    double effic     = Ntmp/NtrkInMimo;
    double effic_err = sqrt(effic*(1.0 - effic)/NtrkInMimo);

    effi_vs_TrkHitDist->SetBinContent(ItrkHitDis+1,effic*100.0);
    effi_vs_TrkHitDist->SetBinError(ItrkHitDis+1,effic_err*100.0);

    double distance  = effi_vs_TrkHitDist->GetBinCenter(ItrkHitDis+1);

    Int_t goodbin_p  = int(distance*hTrackTo2ndclosestClusterDistance->GetXaxis()->GetNbins()/hTrackTo2ndclosestClusterDistance->GetXaxis()->GetXmax());
    Double_t Ntot_p  = h2dmatchedhits->GetEntries();
    Double_t Ngood_p = hWrongAssociationProba->GetBinContent(goodbin_p)*h2dmatchedhits->GetEntries();
    Double_t wrongassociationProba_p = hWrongAssociationProba->GetBinContent(goodbin_p);
    Double_t wrongassociationProba_relativesigma_p = TMath::Sqrt(Ngood_p*(Ntot_p - Ngood_p)/Ntot_p)/Ntot_p;
    //Double_t wrongassociationProba_absolutesigma_p = TMath::Sqrt(Ngood_p*(Ntot_p - Ngood_p)/Ntot_p);

    Double_t CorrectedEfficiency_p       =  (effic - wrongassociationProba_p)/(1.0 - wrongassociationProba_p);
    Double_t CorrectedEfficiencyError_p  = pow((1.0 - wrongassociationProba_p)*effic_err,2);
    CorrectedEfficiencyError_p          += pow((1.0 - effic)*wrongassociationProba_relativesigma_p,2);
    CorrectedEfficiencyError_p           = sqrt(CorrectedEfficiencyError_p);
    CorrectedEfficiencyError_p          *= 1.0/pow(1.0 - wrongassociationProba_p,2);

    effiCorr_vs_TrkHitDist->SetBinContent(ItrkHitDis+1,CorrectedEfficiency_p*100.0);
    effiCorr_vs_TrkHitDist->SetBinError(ItrkHitDis+1,CorrectedEfficiencyError_p*100.0);
  }

  //AP, 22/04/2015
  //Calculation of detection efficiency vs track distance to closest diode
  for(int ItrkDiode=0;ItrkDiode<hDist_Trck_Diode_NoAsso->GetXaxis()->GetNbins();ItrkDiode++) {
    int Nden  = hDist_Trck_Diode_NoAsso->GetBinContent(ItrkDiode+1);
    Nden     += hDist_Trck_Diode_Asso->GetBinContent(ItrkDiode+1);
    int Nnum  = hDist_Trck_Diode_Asso->GetBinContent(ItrkDiode+1);

    double effic,effic_err;
    if(Nden > 25) {
      effic     = (double)Nnum/Nden;
      effic_err = sqrt(effic*(1.0 - effic)/Nden);
    }
    else {
      effic     = -1.0;
      effic_err = 1.0e-8;
    }
    hEffic_vs_Dist_Trck_Diode->SetBinContent(ItrkDiode+1,effic*100.0);
    hEffic_vs_Dist_Trck_Diode->SetBinError(ItrkDiode+1,effic_err*100.0);
  }

  //AB april 2015 estimate bias due to wrong association
  Int_t goodbin=  int(TrackToHitDistanceLimit*hTrackTo2ndclosestClusterDistance->GetXaxis()->GetNbins()/hTrackTo2ndclosestClusterDistance->GetXaxis()->GetXmax());
  Double_t Ntot = h2dmatchedhits->GetEntries();
  Double_t Ngood =hWrongAssociationProba->GetBinContent(goodbin)*h2dmatchedhits->GetEntries();
  Double_t wrongassociationProba = hWrongAssociationProba->GetBinContent(goodbin);
  Double_t wrongassociationProba_relativesigma = TMath::Sqrt(Ngood*(Ntot-Ngood)/Ntot)/Ntot;
  //Double_t wrongassociationProba_absolutesigma = TMath::Sqrt(Ngood*(Ntot-Ngood)/Ntot);
  Double_t CorrectedEfficiency =  (MimosaEfficiency-wrongassociationProba)/(1.-wrongassociationProba);
  Double_t CorrectedEfficiencyError  = pow((1.0 - wrongassociationProba)*MimosaEfficiency_ERR,2);
  CorrectedEfficiencyError          += pow((1.0 - MimosaEfficiency)     *wrongassociationProba_relativesigma,2);
  CorrectedEfficiencyError           = sqrt(CorrectedEfficiencyError);
  CorrectedEfficiencyError          *= 1.0/pow(1.0 - wrongassociationProba,2);

    //AP, 2015/01/13
  //Correction to the cluster multiplicity distribution
  //Subtracting out contribution of fakes from the multiplicity = 1 bin
  hnpix_c->SetBinContent(hnpix_c->FindBin(1),hnpix_c->GetBinContent(hnpix_c->FindBin(1)) - (MimosaEfficiency - CorrectedEfficiency)*NtrkInMimo);

  hNomEffic->SetBinContent(1,CorrectedEfficiency*100.0);
  hNomEffic->SetBinError(1,CorrectedEfficiencyError*100.0);

  //  Double_t Efficiencymistake = (Ntot-Ngood)/Ntot*wrongassociationProba;
  cout << "============================================" << endl;
  cout<< "Proba of wrong association with a cut < "<<TrackToHitDistanceLimit<<" microns -> "<<wrongassociationProba<<" / 1   +-  "
      <<wrongassociationProba_relativesigma<<endl;
  //  cout<<" efficiency is overestimated by "<< Efficiencymistake<<endl;
  cout<<" Measured Efficiency = "<<MimosaEfficiency*100.0<<" +- "<< MimosaEfficiency_ERR*100.0      << "%" << endl;
  cout<<" Corrected Efficiency = "<< CorrectedEfficiency*100.0<<" +- "<<   CorrectedEfficiencyError*100.0      << "%" <<endl;

  //---------------------------------------------------------------
  //-- Print statistics out
  //---------------------------------------------------------------
  TDatime aTime;
  cout << "============================================" << endl;
  cout << aTime.AsString() << endl;
  cout << "DSF file used as input: " << RootFile << endl;
  cout << "-------- RUN NUMBER = "<<RunNumber<<" Plane number = "<<ThePlaneNumber
       <<" submatrix Number = "<<ThesubmatrixNumber<<endl;
  cout << "-------- matched/good tracks in DUT = " << NofClMatchTrack << "  " << NtrkInMimo << "  " << MimosaEfficiency << endl;
  cout << "-------- Efficiency                         = " << MimosaEfficiency*100.0      <<" +- "<< MimosaEfficiency_ERR*100.0      << "%" << endl;
  cout << "-------- Nb of good rec tracks in telescope (independant of geomatrix) per event,  average = " << hNTracksPerEvent->GetMean() << " with RMS = " << hNTracksPerEvent->GetRMS() << endl; // JB 2010/07/23
  cout << "-------- Nb of rec hits in DUT = " << NRecHit << " good = " << ngoodhit << ", average per event " << hnhit->GetMean() << " with RMS " << hnhit->GetRMS() << endl;
  cout << "-------- Average pixel multiplicity in hit " << hnpix_c->GetMean() << " with RMS " << hnpix_c->GetRMS() << " (uncertainty " << hnpix_c->GetRMS()/sqrt(hnpix_c->Integral())<< ")" << endl;
  cout << "-------- Prob multiplicity (1 to 9): " << setprecision(3)<<"," << hnpix_c->GetBinContent(2)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(3)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(4)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(5)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(6)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(7)/hnpix_c->Integral()<<","<<hnpix_c->GetBinContent(8)/hnpix_c->Integral()<<","<<hnpix_c->GetBinContent(9)/hnpix_c->Integral()<<endl;
  cout << "-------- track-Hit dist cut= "<<TrackToHitDistanceLimit<<" chi2 max= "<<TrackChi2Limit<<" Geomatrix = "<<GeoMatrix<<": "<<geomUmin<<"<U<"<<geomUmax<<", "<<geomVmin<<"<V<"<<geomVmax<<endl;
  cout << "-------- minimal nb of hits required per tracks = " << MinHitsPerTrack << endl;
  if( MaxNbOfTracksInGeom > -1 ) cout << "-------- Max nb of tracks allowed " << MaxNbOfTracksInGeom << ", in exclusion geomatrix " << GeoMatrixForTrackCut<<": "<<exgeomUmin<<"<U<"<<exgeomUmax<<", "<<exgeomVmin<<"<V<"<<exgeomVmax<< endl;

  cout << "-------- number of tracks un-matched " << nmiss << ", with thdist > THdist limit " << nmissthdist << ", without hits in DUT  " << nmissh << endl; // JB 2009/09/08
  cout << "-------- Total number of event processed= "<<MaxEvent-MinEvent+1<<endl;
  cout << "-------- CUTS S/N seed and S/N neighbours= "<<CUT_S2N_seed<<" "<<CUT_S2N_neighbour<<endl;
  cout << "-------- MIN and MAX number of hits per event to evaluate efficiency "<<CUT_MinNbOfHits<<" "<<CUT_MaxNbOfHits<<endl;
  cout << "-------- calibration "<<calibration<<endl;

  if(TString(SaveAlign) == TString("yes")) fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC = false;

  if(fSession->GetSetup()->GetAnalysisPar().DoTelResolutionMC) {
    bool verbose = false;
    if(MimoDebug>1) verbose = true;
    cout << endl;
    cout << "MimosaPro:: Calling function MimosaGeneration for telescope resolution evalution with MC!" << endl;
    cout << "MimosaGeneration:: Nevents = " << fSession->GetSetup()->GetAnalysisPar().MCEvents << endl;
    cout << "MimosaGeneration:: Seed    = " << fSession->GetSetup()->GetAnalysisPar().MCSeed   << endl;
    cout << endl;

    MimosaGeneration_ToyMC(fSession->GetSetup()->GetAnalysisPar().MCEvents,
		           ThePlaneNumber,
		           ThesubmatrixNumber,
		           Thegeomatrix,
		           fSession->GetSetup()->GetAnalysisPar().DoGaussianMS,
		           fSession->GetSetup()->GetAnalysisPar().MCSeed,
		           fSession->GetSetup()->GetAnalysisPar().MCDoDisplay,
		           0.0,
		           1.0e-20,
		           0.0,
		           1.0e-20,
		           true,
		           verbose);
  }

  if(fSession->GetSetup()->GetAnalysisPar().SavePlots) {
    NPages++;

    //AP, 2015/03/09.   Printing out metadata in summary file
    char fOutName[200];
    sprintf(fOutName,"run%dPl%d_ClCharge",RunNumber,ThePlaneNumber);
    sprintf(fOutName,"%s",fTool.LocalizeDirName(fOutName));
    TString EPSName = TString(CreateGlobalResultDir()) + TString(fOutName) + TString("_tmpFile") + long(NPages) + (".pdf");
    TString EPSNameO = EPSName + TString("[");
    TString EPSNameC = EPSName + TString("]");
    TCanvas c1("c1","c1",400,400);
    c1.SetFrameFillColor(10);
    c1.SetFillColor(10);
    c1.SetLeftMargin(0.05);
    c1.SetRightMargin(0.05);
    c1.Print(EPSNameO.Data());

    TString command;
    double initX = 0.05;
    double initY = 0.05;
    double X = initX;
    double Y = 1.0 - initY;
    char ytitle[100];
    TLatex* latex = new TLatex();

    c1.Clear();
    latex->SetTextAlign(12);
    latex->SetTextSize(0.05);
    latex->SetTextColor(kBlack);
    latex->DrawLatex(0.40,Y,"Metadata");
    latex->SetTextSize(0.025);
    Y -= 2*initY;
    latex->SetTextColor(kBlue);
    latex->DrawLatex(X,Y,aTime.AsString());
    command = TString("DSF file used as input:");
    Y -= initY;
    latex->DrawLatex(X,Y,command.Data());
    command = TString(RootFile);
    Y -= 0.03;
    latex->SetTextSize(0.02);
    latex->DrawLatex(X,Y,command.Data());
    latex->SetTextSize(0.025);

    Y -= initY;
    command  = TString("RUN NUMBER = ") + long(RunNumber) + TString(" Plane number = ") + long(ThePlaneNumber);
    command += TString(" submatrix Number = ") + long(ThesubmatrixNumber);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.1f",geomUmin);
    command  = TString("Geomatrix = ") + long(GeoMatrix) + TString(" : (") + TString(ytitle);
    sprintf(ytitle,"%.1f",geomUmax);
    command += TString(" < U < ") + TString(ytitle) + TString(")#mum x (");
    sprintf(ytitle,"%.1f",geomVmin);
    command += TString(ytitle) + TString(" < V < ");
    sprintf(ytitle,"%.1f",geomVmax);
    command += TString(ytitle) + TString(")#mum");
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.1f",TrackToHitDistanceLimit);
    command  = TString("track-Hit dist cut = ") + TString(ytitle) + TString("#mum");
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.1f",TrackChi2Limit);
    command  = TString("#chi^{2}_{max} = ") + TString(ytitle);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    command  = TString("min # of hits required per tracks = ") + long(MinHitsPerTrack);
    latex->DrawLatex(X,Y,command.Data());

    if( MaxNbOfTracksInGeom > -1) {
      Y -= initY;
      command  = TString("Max nb of tracks allowed = ") + long(MaxNbOfTracksInGeom) + TString(", in exclusion geomatrix ") + long(GeoMatrixForTrackCut);
      latex->DrawLatex(X,Y,command.Data());
      Y -= initY;
      sprintf(ytitle,"%.1f",exgeomUmin);
      command  = TString("(") + TString(ytitle) + TString(" < U < ");
      sprintf(ytitle,"%.1f",exgeomUmax);
      command += TString(ytitle) + TString(")#mum x (");
      sprintf(ytitle,"%.1f",exgeomVmin);
      command += TString(ytitle) + TString(" < V < ");
      sprintf(ytitle,"%.1f",exgeomVmax);
      command += TString(ytitle) + TString(")#mum");
      latex->DrawLatex(X,Y,command.Data());
    }

    Y -= initY;
    sprintf(ytitle,"%.1f",CUT_S2N_seed);
    command  = TString("CUTS S/N seed and S/N neighbours = ") + TString(ytitle) + TString(" and ");
    sprintf(ytitle,"%.1f",CUT_S2N_neighbour);
    command += TString(ytitle);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    command  = TString("MIN and MAX # of hits per event to evaluate effic = ") + long(CUT_MinNbOfHits) + TString(" and ") + long(CUT_MaxNbOfHits);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.3f",calibration);
    command  = TString("calibration = ") + TString(ytitle);
    latex->DrawLatex(X,Y,command.Data());
    c1.Print(EPSName.Data());

    //New Page:
    c1.Clear();
    Y = 1.0 - initY;
    latex->SetTextSize(0.05);
    latex->SetTextColor(kBlack);
    latex->DrawLatex(0.30,Y,"Summary Results");
    latex->SetTextSize(0.025);
    latex->SetTextColor(kBlue);

    Y -= 2*initY;
    command  = TString("Total # events processed = ") + long(MaxEvent-MinEvent+1);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.4f",MimosaEfficiency);
    command  = TString("matched/good tracks in DUT = ") + long(NofClMatchTrack) + TString("  ") + long(NtrkInMimo) + TString("  ") + TString(ytitle);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.4f",MimosaEfficiency*100.0);
    command  = TString("Effic                  = (") + TString(ytitle) + TString(" #pm ");
    sprintf(ytitle,"%.4f",MimosaEfficiency_ERR*100.0);
    command += TString(ytitle) + TString(") %");
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.1f",TrackToHitDistanceLimit);
    command  = TString("Prob(wrong asso.,trk-hit dist = ") + TString(ytitle) + TString("#mum) = (");
    sprintf(ytitle,"%.4f",wrongassociationProba*100.0);
    command += TString(ytitle) + TString(" #pm ");
    sprintf(ytitle,"%.4f",wrongassociationProba_relativesigma*100.0);
    command += TString(ytitle) + TString(") %");
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.4f",CorrectedEfficiency*100.0);
    command  = TString("Effic(corr. wrong asso.) = (") + TString(ytitle) + TString(" #pm ");
    sprintf(ytitle,"%.4f",CorrectedEfficiencyError*100.0);
    command += TString(ytitle) + TString(") %");
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.3f",hNTracksPerEvent->GetMean());
    command  = TString("(# good rec tracks in telescope)/event, average = ") + TString(ytitle) + TString(", RMS = ");
    sprintf(ytitle,"%.3f",hNTracksPerEvent->GetRMS());
    command += TString(ytitle);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.3f",hnhit->GetMean());
    command  = TString("# of rec hits in DUT = ") + long(NRecHit) + TString(", good = ") + long(ngoodhit) + TString(", average/event = ") + TString(ytitle);
    sprintf(ytitle,"%.3f",hnhit->GetRMS());
    command += TString(", RMS = ") + TString(ytitle);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.3f",hnpix_c->GetMean());
    command  = TString("<cluster multiplicity> = ") + TString(ytitle);
    sprintf(ytitle,"%.3f",hnpix_c->GetRMS());
    command += TString(", RMS(cluster multiplicity) = ") + TString(ytitle);
    latex->DrawLatex(X,Y,command.Data());

    Y -= initY;
    sprintf(ytitle,"%.2f",double(nmissthdist));
    command  = TString("# un-matched tracks = ") + long(nmiss) + TString(", with thdist > THdist limit ") + TString(ytitle) + TString(", without hits in DUT ") + long(nmissh);
    latex->DrawLatex(X,Y,command.Data());
    c1.Print(EPSName.Data());

    c1.Print(EPSNameC.Data());
  }

  int Nlimit = 1000;
  for(int i=0;i<hnGOODhitInGeomatrixVsTrackPerEvent->GetXaxis()->GetNbins();i++) {
    double Effic,Effic_err;
    int NGoodHits,NTracks;

    NTracks   = hnTracksInGeomatrixVsTrackPerEvent->GetBinContent(i+1);
    NGoodHits = hnGOODhitInGeomatrixVsTrackPerEvent->GetBinContent(i+1);

    if(NTracks   > Nlimit &&
       NGoodHits > Nlimit) {

      Effic     = (double)NGoodHits/NTracks;
      Effic_err = sqrt(Effic*(1.0 - Effic)/NTracks);

      Effic     *= 100.0;
      Effic_err *= 100.0;
    }
    else {
      Effic     = -999.0;
      Effic_err = 1.0e-6;
    }

    hEfficiencyInGeomatrixVsTrackPerEvent->SetBinContent(i+1,Effic);
    hEfficiencyInGeomatrixVsTrackPerEvent->SetBinError(i+1,Effic_err);
  }

  //---------------------------------------------------------------
  //---- Hot pixels management
  //---------------------------------------------------------------

  HotPixel_end( MaxEvent-MinEvent+1);

  //---------------------------------------------------------------
  //-- Write histos
  //---------------------------------------------------------------

  if (fWriteHistos) {
    gSystem->cd(CreateGlobalResultDir());
    if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

    Char_t Header[200];
    sprintf(Header,"AllPlots_%d_%d.root",RunNumber,ThePlaneNumber);
    TFile* AllHist = new TFile(Header,"RECREATE");
    dir->GetList()->Write();
    AllHist->Close(); delete AllHist;
    gSystem->cd(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory

  }

  //---------------------------------------------------------------
  //-- The End
  //---------------------------------------------------------------

  fMimosaProDone=kTRUE;
  //SS 2011.12.05: Efficiency, multiplicity and corresponding errors are saved to the CSV file. Run number, plane number and geomatrix limits are saved as well.
  if (strstr(SaveAlign,"no") || strstr(SaveAlign,"NO")){
    csvfile.open ("Main_results.csv",ios::app);
    TDatime aTime;
    csvfile<<aTime.AsString()<<",";
    csvfile << RunNumber <<","<<ThePlaneNumber<<","<<geomUmin<<","<<geomUmax<<","<<geomVmin<<","<<geomVmax<<","<<NofClMatchTrack<<","<<NtrkInMimo<<","
	    << MimosaEfficiency*100.0<<","<<MimosaEfficiency_ERR*100.0<<","<<hnpix_c->GetMean()<<","<<hnpix_c->GetRMS()/sqrt(hnpix_c->GetEntries())<<",";
    csvfile<<setprecision(3)<<"," << hnpix_c->GetBinContent(2)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(3)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(4)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(5)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(6)/hnpix_c->Integral()<< "," << hnpix_c->GetBinContent(7)/hnpix_c->Integral()<<","<<hnpix_c->GetBinContent(8)/hnpix_c->Integral()<<","<<hnpix_c->GetBinContent(9)/hnpix_c->Integral()<<",";
      //MimosaResolution(); //MimosaResolution is run here in order to add information to the CSV file.
    csvfile<<RootFile<<endl;
    csvfile.close();
  }
  fClearDone=kFALSE;
  PreparePost();

}


//______________________________________________________________________________
//
void MimosaAnalysis::MimosaFakerate(Int_t MaxEvt, Float_t S2N_seed, Float_t S2N_neighbour, Int_t submatrix , Int_t GeoMatrix)
{

  // -- Processing macro for MIMOSA root DSF file to determine fake hit rate
  //     for a given submatrix in a given area (GeoMatrix)
  //
  // Assumes there is no track in the DSF file.
  //
  // Loops over MaxEvt events found in a runXXX_0X.root file
  // Fills all histograms
  //
  // Created: JB 2010/07/23, from a Macro written by JB & MG Fakerate_from DSF

  if(!CheckIfDone("init,clear")) return;
  if(!ThePlaneNumber || ThePlaneNumber>fSession->GetTracker()->GetPlanesN()) {Warning("MimosaFakerate()","Please set a plane number!"); return; }
  if(!RunNumber) Error("MimosaFakerate","RunNumber not set! Please run InitSession  first");

  InitMimosaType();
  Info("MimosaFakerate","Mimosa type %d", MimosaType);

  GetAnalysisGoal();

  //----------------------------------------------------------------------------------
  // -- Open the input file containing the TTree
  //----------------------------------------------------------------------------------

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0) {
    Error("MimosaFakerate"," The input file contains an incorrect number of events %d!",Nevt);
  }
  else {
    Info("MimosaFakerate","There is %d events in the input file.",Nevt);
  }

  //----------------------------------------------------------------------------------
  // -- Chose the sub-matrix.
  //----------------------------------------------------------------------------------

  GetParameters();
  ThesubmatrixNumber = submatrix;

  //----------------------------------------------------------------------------------
  // -- CUTS
  //----------------------------------------------------------------------------------

  // on hits
  CUT_S2N_seed =  S2N_seed ; // S/N(seed)
  CUT_S2N_neighbour =  S2N_neighbour ; // S/N(neighbours)
  MaxNofPixelsInCluster = fSession->GetSetup()->GetAnalysisPar().MaxNofPixelsInCluster[ThesubmatrixNumber];

  // on geometry
  Thegeomatrix = GeoMatrix; // define the area of interest in the DUT

  cout << "----------- CUTS:" << endl;
  cout << "  min S/N ratio: for seed = " << CUT_S2N_seed << ", for neighbours (without seed) = " << CUT_S2N_neighbour << endl;

  Info("MimosaFakerate","Cuts initialized");

  //----------------------------------------------------------------------------------
  // -- Analysis variables
  //----------------------------------------------------------------------------------

  Int_t NgoodEvents = 0;
  Int_t Ngoodhits = 0; // total # good hits
  Int_t NgoodhitsinGEOM = 0; // total # good hits in geom
  Int_t Ngoodhitsinevent; // # good hits in the GEOM area in the event
  Int_t NgoodhitsinGEOMinevent; // # good hits in the GEOM area in the event

  Info("MimosaFakerate","Analysis variables initialized");


  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();

  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;//50000;//40000; //was 2000, modifyed by JB Sept 2007 to match number of events for initialization (ped and noise)
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  Info("MimosaFakerate","\nReady to loop over %d events\n", MaxEvent);
  const Int_t NofCycle=1000; //was 100

  // **********************************************************************************
  // ********* MAIN LOOP ***************
  // **********************************************************************************
  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaFakerate","Reading event %d",ievt);
    if(ievt/NofCycle*NofCycle == ievt || ievt<10){
      cout << "MimosaFakerate Event " << ievt <<endl;
    }

      // Event wise parameters
      Ngoodhitsinevent = 0;
      NgoodhitsinGEOMinevent = 0;

      //=========================
      t->GetEvent(ievt);
      //=========================

      if(MimoDebug>1) cout << " getting the hits" << endl;
      TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
      Int_t NbOfHits   = Hits->GetLast()+1   ; // total # hits over all planes

      if( NbOfHits==0 ) { // check to avoid crash due to empty event
	  if (MimoDebug) Info("MimosaFakerate","Empty event %d: #hits %d\n", ievt, NbOfHits);
	  continue;
      }
      NgoodEvents++;


      //---------------------------------------------------------------
      // Loop over hits BUT keep only the ones in the right plane
      for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

	DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);
	if( ahit->Hpk != ThePlaneNumber ) continue; // select only hits in DUT

	// compute S/N with condition on the noise
	Float_t snSeed = 0.;
	if( ahit->Hn0 != 0.) {
	  snSeed = ahit->Hq0/ahit->Hn0;
	}
	else {
	  snSeed = ahit->Hq0;
	}

	//--------------------------------------------------------------
	// If hit passes the selection cuts
	if(    snSeed >= CUT_S2N_seed
	    && ahit->HSNneighbour >= CUT_S2N_neighbour
	    ){ // if hit passes cuts

	  Ngoodhits++;
	  Ngoodhitsinevent++;

	  if( TrackInMimo( Thegeomatrix, ahit->Hu, ahit->Hv, ThesubmatrixNumber) ) { // if in GEOM area
	    NgoodhitsinGEOM++;
	    NgoodhitsinGEOMinevent++;
	  } // end if GEOM area

	  hnGOODhit->Fill(Ngoodhitsinevent);
    hnahitievt->Fill( ievt, Ngoodhitsinevent);
	  GoodHit_Fill( ahit); // JB 2010/10/06

	} // end if hit passes cuts
	//--------------------------------------------------------------

	else { // if hit does not pass cuts
	  if(MimoDebug>1) Info("MimosaFakerate","Hit not passing selection cuts, S/N(seed)=%.1f <> %.1f, S/N(neighbours only)=%.1f <> %.1f, in GEOM area ? %d", snSeed, CUT_S2N_seed, ahit->HSNneighbour, CUT_S2N_neighbour, TrackInMimo( Thegeomatrix, ahit->Hu, ahit->Hv, ThesubmatrixNumber));
	}


      } // end loop on hits
      //--------------------------------------------------------------

      if(MimoDebug) Info("MimosaFakerate","End loop on hits, found %d good hits in event %d", Ngoodhitsinevent, ievt);


  } // end of Main loop over event
  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  FakeRate_end( NgoodEvents);

  //---------------------------------------------------------------
  //-- Compute rates
  //---------------------------------------------------------------

  const Short_t nquantiles = 5;
  Double_t proba[nquantiles] = {.6827, .95, .9545, .99, .9973};
  Double_t quantile[nquantiles];
  hnGOODhit->GetQuantiles( nquantiles, quantile, proba);

  Double_t averageFakeRate = hnGOODhit->GetMean() / (Double_t)NofPixels;
  Double_t rmsFakeRate = hnGOODhit->GetRMS()/sqrt(hnGOODhit->GetEntries()) / (Double_t)NofPixels;
  Double_t countingFakeRate = Ngoodhits / (Double_t)NgoodEvents / (Double_t)NofPixels;
  Double_t poissonUncertainty = sqrt(Ngoodhits) / (Double_t)NgoodEvents / (Double_t)NofPixels;
  Double_t binomialUncertainty = 1./sqrt(NgoodEvents*NofPixels);
  Int_t    nPixelsGeom = (Int_t)ceil( (geomVmax-geomVmin)/PixelSizeV * (geomUmax-geomUmin)/PixelSizeU );
  Double_t averageFakeRateGeom = NgoodhitsinGEOM / (Double_t)nPixelsGeom / (Double_t)NgoodEvents;

  cout << "---- Fake rate - Plane " << ThePlaneNumber << " -----" << endl;
  cout << "Counting " << Ngoodhits << " hits over " << NgoodEvents << " events and for " << NofPixels << " pixels" << endl;
  cout << "  average fake rate/pix = " << averageFakeRate << " or " << countingFakeRate << endl;
  cout << "  uncertainty from RMS = " << rmsFakeRate << endl;
  cout << "  uncertainty from Poisson = " << poissonUncertainty << endl;
  cout << "  uncertainty from Binamial approx = " << binomialUncertainty << endl;
  cout << "Averaging counts per pixel" << endl;
  cout << "  average fake rate/PIX = " << hNhitRateperpixel->GetMean() <<  " with RMS over pixels = " << hNhitRateperpixel->GetRMS() << endl;
  cout << endl <<"In area defined by geomatrix " << Thegeomatrix << ", limits in U: " << geomUmin << ", " << geomUmax << ", limits in V: " << geomVmin << ", " << geomVmax << endl;
  cout << "  average fake rate/pix = " << averageFakeRateGeom << " = " << NgoodhitsinGEOM << " (hit) / " << nPixelsGeom << " (pix) / " << NgoodEvents << " (evt)" << endl;


  //---------------------------------------------------------------
  //-- The End
  //---------------------------------------------------------------

  fMimosaFakerateDone=kTRUE;
  fClearDone=kFALSE;
  PreparePost();

}

//_______________________________________________________________________________________
//

void MimosaAnalysis::MimosaCluster(Int_t MaxEvt , Float_t S2N_seed, Float_t S2N_neighbour, Int_t submatrix, Int_t GeoMatrix)
{

  // Simple wrapper to MimosaCalibration without ChargeAroundSeedCutMax argument
  //
  // JB 2014/01/17

  MimosaCalibration(MaxEvt, S2N_seed, S2N_neighbour, 1.e6, submatrix, GeoMatrix);

}


//_______________________________________________________________________________________
//

void MimosaAnalysis::MimosaCalibration(Int_t MaxEvt , Float_t S2N_seed, Float_t S2N_neighbour, Float_t ChargeAroundSeedCutMax, Int_t submatrix, Int_t GeoMatrix)
{
  // -- Processing macro for MIMOSA root DSF file
  //     for a single given submatrix in a given area (GeoMatrix)
  //
  // Allows to characterize clusters after selection.
  // Also try to determine the position of the calibration peak
  //   if monochromatic X-ray data.
  //
  // Note that calibration constant is not taken into account for
  //  plots dedicated to calibration. These are kept in ADCu!
  // "ChargeAroundSeedCutMax" is also n ADCu
  //
  // Do not take care of possible tracks in the DSF file.
  //
  // Loops over MaxEvt events found in a runXXX_0X.root file
  // Fills all histograms (including for binary output)
  //
  // Created: JB 2010/07/27, from the old MCalib class
  // Modified: JB 2013/11/08, new mechanism to load cut values and new cuts
  // Modified: JB 2013/11/11, histo for binary output added
  // Modified: JB 2014/01/21, new cut on cluster charge

  if(!CheckIfDone("init,clear")) return;
  if(!ThePlaneNumber || ThePlaneNumber>fSession->GetTracker()->GetPlanesN()) {Warning("MimosaCalibration()","Please set a plane number (1 or 2)"); return; }
  if(!RunNumber) Error("MimosaCalibration","RunNumber not set! Please run InitSession  first");

  InitMimosaType();
  Info("MimosaCalibration","Mimosa type %d", MimosaType);

  GetAnalysisGoal();

  CorStatus=2; // indicate CorPar file is not filled, JB 2010/08/27

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0) {
    Error("MimosaCalibration"," The input file contains an incorrect number of events %d!",Nevt);
  }
  else {
    Info("MimosaCalibration","There is %d events in the input file.",Nevt);
  }

  //----------------------------------------------------------------------------------
  // -- Chose the sub-matrix.
  //----------------------------------------------------------------------------------

  ThesubmatrixNumber = submatrix;
  Thegeomatrix = GeoMatrix; // define the area of interest in the DUT
  GetParameters();

  //----------------------------------------------------------------------------------
  // -- CUTS
  //----------------------------------------------------------------------------------
  // only sets here the values specified as arguments to the function,
  // the rest of them will be loaded with GetParameters).
  // JB 2013/11/08

  CUT_S2N_seed = S2N_seed ; // S/N(seed)
  CUT_S2N_neighbour = S2N_neighbour ; // S/N(neighbours)
  CUT_MaxQ_neighbour = ChargeAroundSeedCutMax; // Q without seed

  cout << "----------- additional CUTS:" << endl;
  cout << "  min S/N ratio: for seed = " << CUT_S2N_seed << ", for neighbours (without seed) = " << CUT_S2N_neighbour << endl;
  if( fIfCalibration )cout << "  max Q for neighbours (without seed) = " << CUT_MaxQ_neighbour << endl;

  Info("MimosaCalibration","Cuts initialized");

  //----------------------------------------------------------------------------------
  // -- Analysis variables
  //----------------------------------------------------------------------------------

  //SetCalibration(1); // This is mandatory to not modify the ADC scale

  Int_t NgoodEvents = 0;
  Int_t Ngoodhits = 0; // total # good hits in the area
  Int_t Ngoodhitsinevent; // # good hits in the event in the area
  Int_t NgoodhitsinPeak = 0; // total # good hits in the calibration peak
  Int_t NbOfHits = 0;  // # hits over all planes in the event
  Int_t totalNOfHits = 0; // total # hits in the plane

  Int_t *seedList=NULL; // list of hits, value=-1 means not selected, seed index otherwise

  Info("MimosaCalibration","Analysis variables initialized");


  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();

  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;//50000;//40000; //was 2000, modifyed by JB Sept 2007 to match number of events for initialization (ped and noise)
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  // ************************************
  // Call init part of the analysis here

  ClusterShape_init();
  ClusterCharges_init();
  ClusterPosition_init(); // JB 2014/01/10

  // ************************************

  Info("MimosaCalibration","\nReady to loop over %d events\n", MaxEvent);
  const Int_t NofCycle=1000;

  // **********************************************************************************
  // ********* MAIN LOOP ***************
  // **********************************************************************************
  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaCalibration","Reading event %d",ievt);

    // Event wise parameters
    Ngoodhitsinevent = 0;

    //=========================
    t->GetEvent(ievt);
    //=========================

    if(ievt/NofCycle*NofCycle == ievt || ievt<10){
      cout << "MimosaCluster Event " << ievt <<endl;
    }

    if(MimoDebug>1) cout << " getting the hits" << endl;
    TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
    NbOfHits   = Hits->GetLast()+1; // total # hits over all planes
    //DAuthenticPlane *thePlane = (DAuthenticPlane*) Evt->GetAuthenticPlanes()->At(ThePlaneNumber);

    /*if( NbOfHits==0 || !thePlane ) { // check to avoid crash due to empty event
     if (MimoDebug) Info("MimosaCalibration","Empty event %d: #hits %d or missing plane %d\n", ievt, NbOfHits, ThePlaneNumber);
     continue;
     }*/
    NgoodEvents++;
    seedList = new Int_t[NbOfHits];

    int NfiredPixelsInEvent = 0;

    //---------------------------------------------------------------
    // Loop over hits BUT keep only the ones in the right plane
    if(MimoDebug>1) cout << " Looping over " << NbOfHits << " hits" << endl;
    for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

      seedList[iHit] = -1; //init seed index (hit not selected will keep this index)

      DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);
      if( ahit->Hpk != ThePlaneNumber ) continue; // select only hits in DUT

      totalNOfHits++;

      // compute S/N with condition on the noise
      Float_t snSeed = 0.;
      if( ahit->Hn0 != 0.) {
        snSeed = ahit->Hq0/ahit->Hn0;
      }
      else {
        snSeed = ahit->Hq0;
      }

      if (MimoDebug>1) printf( "MimosaCalibration: trying hit %d against 1st selection: snSeed=%5.1f >? %5.1f, Qseed=%5.1f >? %5.1f\n", iHit, snSeed, CUT_S2N_seed, ahit->Hq0, CUT_Q_seed);
      //--------------------------------------------------------------
      // If hit passes the first selection cuts
      if(    snSeed >= CUT_S2N_seed
         && ( ahit->Hq0 >= CUT_Q_seed ) // JB 2013/11/08
         && ( MinNofPixelsInCluster <= ahit->HNNS && ahit->HNNS <= MaxNofPixelsInCluster ) // JB 2013/09/11
         && TrackInMimo( Thegeomatrix, ahit->Hu, ahit->Hv, ThesubmatrixNumber)
         && ( CUT_MinSeedIndex==CUT_MaxSeedIndex ||
             (CUT_MinSeedIndex<=ahit->Hsk && ahit->Hsk<=CUT_MaxSeedIndex) ) // allows to select an index range for the seed pixel (ineffective if minIndex==maxIndex), JB 2013/08/21
         && ( CUT_MinSeedCol==CUT_MaxSeedCol ||
             (CUT_MinSeedCol<=(ahit->Hsk%NofPixelInRaw) && (ahit->Hsk%NofPixelInRaw)<=CUT_MaxSeedCol) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
         && ( CUT_MinSeedRow==CUT_MaxSeedRow ||
             (CUT_MinSeedRow<=(ahit->Hsk/NofPixelInRaw) && (ahit->Hsk/NofPixelInRaw)<=CUT_MaxSeedRow) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
         // && 256<=(ahit->Hsk%NofPixelInColumn) && (ahit->Hsk%322)<=319 // S1:0-95, S2:96-191, S3:192-255, S4:256-319, JB 2013/08/22
         && ( CUT_MinNbOfHits<=NbOfHits && NbOfHits<=CUT_MaxNbOfHits )

         ){ // if hit passes first cuts

        if( fIfCalibration ) { // If goal is calibration, added JB 2014/01/10
          if (MimoDebug>1) printf( "MimosaCalibration: trying hit %d against calib peak selection: Qneighbour=%5.1f <? %5.1f\n", iHit, ahit->Hqc - ahit->Hq0, CUT_MaxQ_neighbour);
          //--------------------------------------------------------------
          // If hit in the peak, usefull to determine the calibration constant
          if( ahit->Hqc - ahit->Hq0 < CUT_MaxQ_neighbour ) { // if hit in the calibration peak

            //Ngoodhits++;
            NgoodhitsinPeak++;
            if (MimoDebug>1) printf( "MimosaCalibration: hit %d selected for peak\n", iHit);

            seedList[iHit] = IndexofPix[0];

            hqSeedCalibration->Fill( ahit->Hq0);
            hqNeighbourCalibration->Fill( ahit->Hqc - ahit->Hq0);
            hqSeedVsNeighbourCalibration->Fill( ahit->Hq0, ahit->Hqc - ahit->Hq0);

          } // end if hit in peak
          //--------------------------------------------------------------
        } // end If goal is calibration

        if (MimoDebug>1) printf( "MimosaCalibration: trying hit %d against 2nd selection: snNeighbour=%5.1f >? %5.1f, Qneighbour=%5.1f >? %5.1f\n",
				 iHit, ahit->HSNneighbour, CUT_S2N_neighbour, ahit->Hqc - ahit->Hq0, CUT_MinQ_neighbour);
        //--------------------------------------------------------------
        // If hit passes the second selection cuts
        if(  ahit->HSNneighbour >= CUT_S2N_neighbour
           && ( ahit->Hqc - ahit->Hq0 >= CUT_MinQ_neighbour ) // JB 2013/11/08
           && ( ahit->Hqc >= CUT_Q_cluster ) // JB 2014/01/21
           ) { // if hit passes full cuts

          Ngoodhits++;
          Ngoodhitsinevent++;
          if (MimoDebug>1) printf( "MimosaCalibration: hit %d selected as %dth hit of event and %ith hit overall\n", iHit, Ngoodhitsinevent, Ngoodhits);

          seedList[iHit] = IndexofPix[0];

          NofPixelsInCluster = (ahit->HNNS < MaxNofPixelsInCluster)?ahit->HNNS:MaxNofPixelsInCluster;
          if (MimoDebug>2) Info("MimosaPro"," NofPixelsInCluster is set to %d (in hit %d, max %d)\n", NofPixelsInCluster, ahit->HNNS, MaxNofPixelsInCluster);

          ClusterCharges_compute( ahit);
          ClusterPosition_compute( ahit, alignement);

          hnGOODhit->Fill(Ngoodhitsinevent);
          hnahitievt->Fill( ievt, Ngoodhitsinevent);
          GoodHit_Fill( ahit);

          BinarySpecific_HitsOnly_fill( ahit);
          BinarySpecific_fill( ahit);
          ClusterCharges_fill( ahit, ievt);
          ClusterPosition_fill( ahit);
          ClusterShape_fill( ahit);

	  NfiredPixelsInEvent += ahit->HNNS;

        } // end if hit passes full cut
        //--------------------------------------------------------------

      } // end if hit passes first cuts
      //--------------------------------------------------------------

      else { // if hit does not pass cuts
        if(MimoDebug>1) Info("MimosaCalibration","Hit not passing selection cuts, S/N(seed)=%.1f <> %.1f, S/N(neighbours only)=%.1f <> %.1f, # pixels %d, in GEOM area ? %d", snSeed, CUT_S2N_seed, ahit->HSNneighbour, CUT_S2N_neighbour, NofPixelsInCluster, TrackInMimo( Thegeomatrix, ahit->Hu, ahit->Hv, ThesubmatrixNumber));
      }


    } // end loop on hits
    //--------------------------------------------------------------

    h_pixels_event->Fill(NfiredPixelsInEvent);

    // Build all possible distance between selected hits (after 2nd cuts)
    Int_t x1=0,y1=0,x2=0,y2=0;
    Float_t dist=0.;
    Int_t countHit = 0;
    for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++) { // loop on hits
      if( seedList[iHit]!=-1 ) { // use selected hits only
        countHit++;
        x1 = seedList[iHit]%NofPixelInRaw;
        y1 = seedList[iHit]/NofPixelInRaw;

        for (Int_t iHit2=iHit+1 ; iHit2<NbOfHits ; iHit2++) {
          if( seedList[iHit2]!=-1 ) { // use selected hits only
            x2 = seedList[iHit2]%NofPixelInRaw;
            y2 = seedList[iHit2]/NofPixelInRaw;

            dist = TMath::Sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
            hSeedBetweenDist->Fill( dist );
          } // end selected hits 2
        } // end second loop on hits
      } // end selected hits 1
    } // end loop on hits

    delete seedList;

    if(MimoDebug) Info("MimosaCalibration","End loop on hits, found %d good hits in event %d (total %d good hits)", Ngoodhitsinevent, ievt, Ngoodhits);

  } // end of Main loop over event
  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  huv_rate->Scale(1.0/NgoodEvents);

  // ************************************
  // Call final part of analysis here

  ClusterPosition_end(); // JB 2014/01/10
  ClusterCharges_end();
  ClusterShape_end();
  BinarySpecific_end();

  // ************************************


  printf("\n*------------------------------------*\n");
  printf(" %d hits read, %d selected, %d rejected, in peak %d", totalNOfHits, Ngoodhits, totalNOfHits-Ngoodhits, NgoodhitsinPeak);
  printf("\n*------------------------------------*\n");


  //---------------------------------------------------------------
  //-- The End
  //---------------------------------------------------------------

  fMimosaCalibrationDone=kTRUE;
  fClearDone=kFALSE;
  PreparePost();

}

//______________________________________________________________________________
//
void MimosaAnalysis::MimosaMiniVectors(Int_t MaxEvt, Int_t TrackHitDist, Short_t plane0, Float_t S2N_seed0, Float_t S2N_neighbour0, Int_t submatrix0, Int_t GeoMatrix0, Short_t plane1, Float_t S2N_seed1, Float_t S2N_neighbour1, Int_t submatrix1, Int_t GeoMatrix1)
{

  // -- Combined analysis of two nearby planes to produce minivectors
  //    and compare them to a track
  //
  // Main analysis method: loops over all events found in a runXXX_0X.root file
  // Fills all histograms
  //
  // Created : JB 2010/09/01, from NCS orginal stuff 2010/01/21
  // Modified: JB 2011/10/30, histo filling
  // Modified: JB 2011/11/04, call to generic method to compute efficiency
  // Modified: JB 2013/11/08, new mechanism to load cut values and new cuts
  // Modified: JB 2015/12/15, take 2nd plane geomatrix into account

  if(!CheckIfDone("init,clear")) return;
  if(!RunNumber) Error("MimosaPro","RunNumber not set! Please run InitSession  first");

  Int_t theplanenumber[2] = { plane0, plane1};
  ThePlaneNumber = plane0; // JB 2011/10/30, for display
  ThePlaneNumber2 = plane1;

  InitMimosaType();
  Info("MimosaPro","Mimosa type %d", MimosaType);

  GetAnalysisGoal();
  fIfReferenceTrack = kTRUE;
  fIfMiniVector = kTRUE;

  gSystem->ChangeDirectory(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory


  //----------------------------------------------------------------------------------
  // -- Open the input file containing the TTree
  //----------------------------------------------------------------------------------

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0) {
    Error("MimosaMiniVectors"," The input file contains an incorrect number of events %d!",Nevt);
  }
  else {
    Info("MimosaMiniVectors","There is %d events in the input file.",Nevt);
  }

  //----------------------------------------------------------------------------------
  // -- Init analysis variables
  //----------------------------------------------------------------------------------

  // for counting hits and tracks
  Int_t   ngoodhit    = 0; // # good hits in the DUT
  Int_t   NRecHit     = 0; // total # hits in the DUT
  Int_t   nmiss       = 0; // # good tracks in the DUT without a matched hit, JB 2009/09/08
  Int_t   nmissh[2]   = {0, 0}; // # good tracks in the DUT with no good hit in the DUT, JB 2009/09/08
  Int_t   nmissthdist[2] = {0, 0}; // # good tracks in the DUT with hit outside the distance limit

  Int_t Nhitinevent; // JB 2009/09/08
  Int_t Ngoodhitinevent[2]; // # good hits in the event
  Int_t Ngoodtrackinevent; // # good tracks in the telescope in the event
  Int_t NgoodtrackinDUTinevent; // # good tracks in the telescope and in the DUT in the event

  Float_t trackToHitDistance[2] = { 100000., 100000.};

  // Counter on event status
  Int_t StatusEvent[10];
  for ( Int_t i=0 ; i<10 ; i++ ) {
    StatusEvent[i]=0;
  }

  Int_t IndexOfGoodHitsInEvent[2][5000]; // enlarge to 5000, Jb 209/09/21


  //----------------------------------------------------------------------------------
  // -- get parameters
  //----------------------------------------------------------------------------------

  ThesubmatrixNumber = submatrix0; // JB 2015/12/15
  GetParameters(); // JB 2010/07/23

  //----------------------------------------------------------------------------------
  // CUTS
  //----------------------------------------------------------------------------------

  // on baseline
  Bool_t usebaselinecut = kFALSE ;
  Float_t Baselinecut = 1100.0;

  // on hits

  Float_t cut_S2N_seed[2] = {S2N_seed0, S2N_seed1}; // S/N(seed)
  Float_t cut_S2N_neighbour[2] = {S2N_neighbour0, S2N_neighbour1}; // S/N(neighbours)
  Int_t   maxNofPixelsInCluster[2] = {fSession->GetSetup()->GetAnalysisPar().MaxNofPixelsInCluster[submatrix0], fSession->GetSetup()->GetAnalysisPar().MaxNofPixelsInCluster[submatrix1]};

  // on tracks
  TrackToHitDistanceLimit =  TrackHitDist ; // Distance between the track and the hit
  TrackChi2Limit = fSession->GetSetup()->GetAnalysisPar().TrackChi2Limit;

  // on geometry
  // First plane geomatrix
  Thegeomatrix = GeoMatrix0; // define the area of interest in the DUT
  geomUmin = fSession->GetSetup()->GetAnalysisPar().Umin[ThesubmatrixNumber][Thegeomatrix];
  geomUmax = fSession->GetSetup()->GetAnalysisPar().Umax[ThesubmatrixNumber][Thegeomatrix];
  geomVmin = fSession->GetSetup()->GetAnalysisPar().Vmin[ThesubmatrixNumber][Thegeomatrix];
  geomVmax = fSession->GetSetup()->GetAnalysisPar().Vmax[ThesubmatrixNumber][Thegeomatrix];

  // Second plane geomatrix
  // JB 2015/12/15
  ThesubmatrixNumber2 = submatrix1;
  Thegeomatrix2 = GeoMatrix1; // define the area of interest in the DUT
  geomUmin2 = fSession->GetSetup()->GetAnalysisPar().Umin[ThesubmatrixNumber2][Thegeomatrix2];
  geomUmax2 = fSession->GetSetup()->GetAnalysisPar().Umax[ThesubmatrixNumber2][Thegeomatrix2];
  geomVmin2 = fSession->GetSetup()->GetAnalysisPar().Vmin[ThesubmatrixNumber2][Thegeomatrix2];
  geomVmax2 = fSession->GetSetup()->GetAnalysisPar().Vmax[ThesubmatrixNumber2][Thegeomatrix2];

  cout << "----------- CUTS:" << endl;
  cout << "  Baseline cut " << Baselinecut << " applied ? " << usebaselinecut << endl;
  for( Short_t iplane=0; iplane<2; iplane++) {
    cout << "  For plane " << theplanenumber[iplane] << endl;
    cout << "    max # pixels in a hit: " << maxNofPixelsInCluster[iplane] << endl;
    cout << "    min S/N ratio: for seed = " << cut_S2N_seed[iplane] << ", for neighbours (without seed) = " << cut_S2N_neighbour[iplane] << endl;
    if ( iplane==0 ) {
      cout << "  geomatrix is " << Thegeomatrix << ", limits in U: " << geomUmin << ", " << geomUmax << ", limits in V: " << geomVmin << ", " << geomVmax << endl;
    }
    else {
      cout << "  geomatrix is " << Thegeomatrix2 << ", limits in U: " << geomUmin2 << ", " << geomUmax2 << ", limits in V: " << geomVmin2 << ", " << geomVmax2 << endl;
    }
  }
  cout << "  max distance track to hit " << TrackToHitDistanceLimit << endl;
  cout << "  max track chi2 " << TrackChi2Limit << endl;

  Info("MimosaMiniVectors","Analysis variables initialized");

  //----------------------------------------------------------------------------------
  // -- prepare displays
  //----------------------------------------------------------------------------------

  PrepareOnlineDisplay();

  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();

  //----------------------------------------------------------------------------------
  // -- get the alignement parameters and parametrization of the Eta function
  //----------------------------------------------------------------------------------

  DPrecAlign *align[2];
  TFile* theCorFiles[2] = { 0, 0};

  for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes

    if (!theCorFiles[iplane]) InitCorPar( RunNumber, theplanenumber[iplane]);
    theCorFiles[iplane] = theCorFile;

    if( theCorFiles[iplane]->IsOpen() && theCorFiles[iplane]->FindKey("alignement") ) { // if the CorPar files is already initialized
      align[iplane] =(DPrecAlign*)theCorFiles[iplane]->Get("alignement");
      align[iplane]->Print();
    }
    else {
      Error("MimosaMiniVectors","No alignement found from the file %s, for plane %d, STOPPING\n", theCorFiles[iplane]->GetName(), theplanenumber[iplane]);
    }

    align[iplane]->PrintAll(); // JB 2011/10/31
    Info("MimosaMiniVectors","Alignement read for plane %d", theplanenumber[iplane]);
  } // end loop on planes

  GetMiEta(); // JB 2010/09/01 shall be aplied to both planes as well!



  // store the cluster info in separate ascii file:
  Char_t Headerclusterdat[100];
  sprintf(Headerclusterdat,"HitSeparation/rawhit/newcluster_%d_%d.dat",RunNumber,ThePlaneNumber);
  ofstream outFile(Headerclusterdat);
  Int_t NofCycle=1000; //was 100


  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;//50000;//40000; //was 2000, modifyed by JB Sept 2007 to match number of events for initialization (ped and noise)
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  // ************************************
  // Call init part of the analysis here

  MaxNofPixelsInCluster = TMath::Max( maxNofPixelsInCluster[0], maxNofPixelsInCluster[1]);
  Efficiency_init();     // JB 2011/11/04
  ClusterCharges_init();
  ClusterPosition_init();

  // ************************************

  // a bit of cleaning
  MainCanvas->Clear();

  Info("MimosaMiniVectors","\nReady to loop over %d events\n", MaxEvent);

  // **********************************************************************************
  // ********* MAIN LOOP ***************
  // **********************************************************************************
  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaMiniVectors","Reading event %d",ievt);

    //-- Initilisations

    Nhitinevent=0;
    Ngoodhitinevent[0]=0;
    Ngoodhitinevent[1]=0;
    Ngoodtrackinevent=0;
    NgoodtrackinDUTinevent=0;
    for ( Int_t i=0 ; i<10 ; i++ ) { StatusEvent[i]=0; }

    //-- Update display

    if (ievt == MinEvent) { // JB, 2008/08/09
      selection->Draw();
      for( Int_t il=0; il<8; il++) {
        textLabel[il]->Draw();
      }
    }

    if(ievt/NofCycle*NofCycle == ievt){
      if( ievt>1000) NofCycle = 2000;
      if( ievt>5000) NofCycle = 5000;
      if( selection->GetMaximum()>15*selection->GetBinContent(6)) MainCanvas->SetLogy(); // JB 2010/04/28
      MainCanvas->Modified();
      MainCanvas->Update();
      cout << "MimosaMiniVectors Event " << ievt <<endl;
      if (NtrkInMimo>100){
        cout<<"  temporary efficiency = "<<NofClMatchTrack<<" / "<<NtrkInMimo<<" = "<<1.*NofClMatchTrack/NtrkInMimo<<endl;
      }
    }

    //-- Update counter for efficiency

    StatusEvent[1]++;
    if(ievt / NeventRangeForEfficiency * NeventRangeForEfficiency == ievt ){ ievt_array ++; }

    //=========================
    if(MimoDebug>1) cout << " getting the event" << endl;
    t->GetEvent(ievt);
    //=========================

    //DEventHeader& CurrentEventHeader=Evt->GetHeader();

    if(MimoDebug>1) cout << " getting the planes, tracks and hits" << endl;
    TClonesArray *Trpl   = Evt->GetTransparentPlanes() ; //tracks
    TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
    TClonesArray *Planes = Evt->GetAuthenticPlanes()   ; //planes
    Int_t NbOfTrpl   = Trpl->GetLast()+1   ; // total # tracks over all planes
    Int_t NbOfHits   = Hits->GetLast()+1   ; // total # hits over all planes
    Int_t NbOfPlanes = Planes->GetLast()+1 ; // Total Number of Planes

    // check to avoid crash due to empty event, added by JB, Sept 2008,
    if( NbOfTrpl==0 || NbOfHits==0 || NbOfPlanes==0 ) {
      if (MimoDebug) Info("MimosaMiniVectors","Empty event %d: #tracks %d, #hits %d, #planes %d \n", ievt, NbOfTrpl, NbOfHits, NbOfPlanes);
      continue;
    }

    // get pointers to the planes studied
    DAuthenticPlane *ThePlaneStudied[2] = {0, 0}; // init
    DAuthenticPlane *aPlane;
    for( Int_t iplane=0; iplane<NbOfPlanes; iplane++) {
      aPlane = (DAuthenticPlane*) Planes->At(iplane);
      if ( aPlane->Ppk == theplanenumber[0] ) ThePlaneStudied[0] = aPlane;
      else if ( aPlane->Ppk == theplanenumber[1] ) ThePlaneStudied[1] = aPlane;
    }


    // ****************** Filling histos
    selection->Fill(1.);  // all events

    // ******************

    //---------------------------------------------------------------
    // if planes are there
    // and baseline cuts OK
    // and number of hits and tracks are within limits
    if ( ThePlaneStudied[0] && ThePlaneStudied[1]
        && (usebaselinecut==kFALSE || (ThePlaneStudied[0]->PFrfr1<Baselinecut))
        && (usebaselinecut==kFALSE || (ThePlaneStudied[1]->PFrfr1<Baselinecut))
        && NbOfHits<CUT_MaxNbOfHits && NbOfHits>=CUT_MinNbOfHits
        ) { // Plane exists with tracks and hits
      StatusEvent[2]++;

      // ****************** Filling histos
      selection->Fill(2.); // event with track(s) and hits in the plane

      hnhit->Fill(ThePlaneStudied[0]->PhN + ThePlaneStudied[1]->PhN);
      hnhitievt->Fill(ievt,ThePlaneStudied[0]->PhN + ThePlaneStudied[1]->PhN);
      // ******************

      //---------------------------------------------------------------
      // Loop over hits BUT keep only the ones in the right plane
      if(MimoDebug>1) cout << " looping over " << NbOfHits << " hits" << endl;
      for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

        DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);

        //------------------------------------------
        for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes

          //------------------------------------------
          if( ahit->Hpk == theplanenumber[iplane] ) { // if hit belongs to expected plane
            StatusEvent[6]++;
            Nhitinevent++;
            NRecHit++;

            // Now compute variables usefull for cuts

            NofPixelsInCluster = (ahit->HNNS < maxNofPixelsInCluster[iplane])?ahit->HNNS:maxNofPixelsInCluster[iplane];
            if (MimoDebug>2) Info("MimosaMiniVectors"," NofPixelsInCluster is set to %d (in hit %d, max %d)\n", NofPixelsInCluster, ahit->HNNS, maxNofPixelsInCluster[iplane]);

            // signal over noise ratio of the seed pixel
            Float_t snSeed;
            if( ahit->Hn0 > 0.1 ) {
              snSeed=ahit->Hq0/ahit->Hn0/NoiseScope;
            }
            else { // for sparsified readout, JB 2009/05/19
              snSeed=ahit->Hq0;
            }

            // charge and S/N in the second highest pixel of the hit
            // among the 4 pixels neighbouring the seed (cross)
            Float_t q2nd = 0.;
            Float_t sn2nd = 0.;
            for(Int_t i=0; i < NofPixelsInCluster; i++){
              //cout << "i=" << i << " index=" << ahit->HkM[i]- ahit->HkM[0] << " q=" << ahit->HqM[i] << endl;
              if( (abs(ahit->HkM[i]- ahit->HkM[0])==1 || abs(ahit->HkM[i]- ahit->HkM[0])== NofPixelInRaw)
                 && ahit->HqM[i]>q2nd	){
                q2nd = ahit->HqM[i];
                if( ahit->HnM[i] != 0.0 ) {
                  sn2nd = q2nd/ahit->HnM[i]/NoiseScope;
                }
                else { // for sparsified readout, JB 2009/05/19
                  sn2nd = q2nd;
                }
              }
            }

            // charge of the cluster without the seed
            ChargeAroundSeed = (ahit->Hqc - ahit->Hq0)*calibration;

            if(MimoDebug>2) {
              printf("MimosaMiniVectors: Hit %d, qSeed=%.1f, qAroundSeed=%.1f, snSeed=%.1f, sn2nd=%.1f\n", iHit, ahit->Hq0, ChargeAroundSeed,snSeed, sn2nd);
            }

            // ****************** Filling histos
            selection->Fill(6.);  // all hits in MIMOSA

            // ******************


            //--------------------------------------------------------------
            // If hit passes the selection cut
            if( ChargeAroundSeed >= CUT_MinQ_neighbour
               && ahit->HqM[0]*calibration>0.
               //&&( (ahit->HsnPulse / ahit->Hsn   >= CUT_S2N_seed ) && ( ahit->HSNneighbour >= CUT_S2N_neighbour)) //YV 27/1109 you add this line if you want S/N cuts
               //&& NofPixelsInCluster >= 2
               ){ // if hit passes cuts
              StatusEvent[7]++;
              ngoodhit++;
              IndexOfGoodHitsInEvent[iplane][Ngoodhitinevent[iplane]] = iHit;
              Ngoodhitinevent[iplane]++;
              if(MimoDebug>2) cout << " plane " << theplanenumber[iplane] << " found good hit " << Ngoodhitinevent[iplane]-1 << " with index " << IndexOfGoodHitsInEvent[iplane][Ngoodhitinevent[iplane]-1] << " pos " << ahit->Hu << ";" << ahit->Hv << " and dist to track " << ahit->Hu-ahit->Htu << ";" << ahit->Hv-ahit->Htv << endl;

              // ****************** Filling histos
              selection->Fill(7.); // Good hits in MIMOSA

              hnGOODhit->Fill(Ngoodhitinevent[iplane]);
              hnahitievt->Fill( ievt, Ngoodhitinevent[iplane]);
              GoodHit_Fill( ahit); // JB 2010/10/06
              hgoodSN_vs_SNN->Fill(ahit->HSNneighbour,snSeed);

              BinarySpecific_HitsOnly_fill( ahit);

              if( ahit->Htk >= 0) { // if a track was associated in Tree
                Float_t adist = sqrt((ahit->Hsu-ahit->Htu)*(ahit->Hsu-ahit->Htu)+(ahit->Hsv-ahit->Htv)*(ahit->Hsv-ahit->Htv)) ;
                duvall->Fill( adist); //For all the clusters with Hqc-Hq0>Cut
                hAllHuvsAllTu->Fill( ahit->Htu, ahit->Hsu);
                hAllHuvsAllTv->Fill( ahit->Htv, ahit->Hsu);
                hAllHvvsAllTu->Fill( ahit->Htu, ahit->Hsv);
                hAllHvvsAllTv->Fill( ahit->Htv, ahit->Hsv);
                if(fabs( sqrt( (ahit->Htu-ahit->Hu)*(ahit->Htu-ahit->Hu) + (ahit->Htv-ahit->Hv)*(ahit->Htv-ahit->Hv) ) ) > TrackToHitDistanceLimit){
                }
              } // end if a track was associated
              // ******************

            } // end if hit passes cuts
            //--------------------------------------------------------------

            else { // if hit does not pass cuts
              if(MimoDebug>2) printf(" plane %d, hit %d NOT passing selection cuts, ChargeAroundSeed=%f <> %f, seedCharge=%.1f <> %.0f, NofPixelsInCluster=%d\n", theplanenumber[iplane], iHit, ChargeAroundSeed, CUT_MinQ_neighbour, ahit->HqM[0]*calibration, 0., NofPixelsInCluster);
            }

          } // end if hit belongs to expected plane
          //--------------------------------------------------------------

        } // end loop on planes
        //--------------------------------------------------------------

      } // end loop on hits
      //--------------------------------------------------------------

      if(MimoDebug) Info("MimosaMiniVectors","End loop on hits, found %d and %d good hits from planes", Ngoodhitinevent[0], Ngoodhitinevent[1]);


      //---------------------------------------------------------------
      // Loop on tracks BUT keep only the ones in the right plane
      if(MimoDebug>1) cout << " looping over " << NbOfTrpl << " tracks" << endl;
      for (Int_t iTrk=0 ; iTrk<NbOfTrpl ; iTrk++){ // loop on tracks

        DTransparentPlane *aTrpl = (DTransparentPlane*) Trpl->At(iTrk);
        if( aTrpl->Tpk != theplanenumber[0] ) continue; // select only tracks in DUT

        StatusEvent[3]++;


        // ****************** Filling histos
        selection->Fill(3.); // all tracks in plane

        TrackParameters_allFill( aTrpl, ievt);
        // ******************

        if( MimoDebug>2) cout << " Track " << iTrk << " has chi2=" << aTrpl->Tchi2 << " <?> " << TrackChi2Limit << endl;

        //---------------------------------------------------------------
        // if chi2 of selected track is OK
        if ( aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 < TrackChi2Limit // JB, chi2=0. is not a real pb
            //&& aTrpl->Tdx<0.002 && aTrpl->Tdy<0.002 // insure track is flat, NOT MANDATORY, JB sept 2007
            ){ // if good track
          StatusEvent[4]++;
          Ngoodtrackinevent++;

          // compute track information for each plane
          for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
            alignement = align[iplane];
            TrackParameters_compute(aTrpl, alignement);
            trackPosUVW[iplane].SetValue( tu, tv, 0.);
            trackPosXYZ[iplane].SetValue( tx, ty, tz);
            //trackPosUVW[iplane].SetValue( alignement->TransformTrackToPlane(tx,ty,tz,tdx,tdy));
            //trackPosXYZ[iplane].SetValue( alignement->GetTrackPosition());
            if( MimoDebug>2) {
              cout << "Plane " << theplanenumber[iplane] << endl;
              cout << " track XYZ " << trackPosXYZ[iplane](0) << ", " << trackPosXYZ[iplane](1) << ", " << trackPosXYZ[iplane](2) << endl;
              cout << " track UVW " << trackPosUVW[iplane](0) << ", " << trackPosUVW[iplane](1) << ", " << trackPosUVW[iplane](2) << endl;
            }
          }

          /*******************/
          selection->Fill(4.);   // Good quality track

          hNTracksPerEvent->Fill( Ngoodtrackinevent);
          hNTracksPerEventievt->Fill( ievt, Ngoodtrackinevent);

          TrackParameters_goodFill( aTrpl, ievt);

          /*******************/

          Bool_t TrkCrossMimo = TrackInMimo( Thegeomatrix, trackPosUVW[0](0), trackPosUVW[0](1), ThesubmatrixNumber);
          if(MimoDebug>1) cout << " Is track " << iTrk << "in first MIMOSA ? " << TrkCrossMimo << endl;

          // 2nd test in 2nd plane, JB 2015/12/15
          TrkCrossMimo &= TrackInMimo( Thegeomatrix2, trackPosUVW[1](0), trackPosUVW[1](1), ThesubmatrixNumber2);
          if(MimoDebug>1) cout << " Is track " << iTrk << "in both MIMOSA ? " << TrkCrossMimo << endl;

          //---------------------------------------------------------------
          // if track is in the good geometry range
          if( TrkCrossMimo == kTRUE ){ // if the track is in MIMOSA
            StatusEvent[5]++;
            NgoodtrackinDUTinevent++;
            NtrkInMimo++ ;

            if(MimoDebug) Info("MimosaMiniVectors"," Good (chi2=%f) track %d found in MIMOSA", aTrpl->Tchi2, iTrk);

            temp_NtrkInMimo[ievt_array]++;

            //-- Find the nearest good hit to this track in each plane
            DAuthenticHit *thehit[2] = {0, 0};
            Bool_t aHitIsFound[2] = {kFALSE, kFALSE};
            DAuthenticHit *ahit = 0;
            Float_t adist;

            //------------------------------------------
            for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes

              trackToHitDistance[iplane] = 10000000.0 ;

              if( Ngoodhitinevent[iplane]<=0 ) { // no hit at all, JB 2009/09/08
                nmissh[iplane]++;
              }

              if(MimoDebug>2) cout << "  Trying to associate hits of plane " << theplanenumber[iplane] << ", loop on " << Ngoodhitinevent[iplane] << " candidates." << endl;
              for(Int_t iHit=0; iHit<Ngoodhitinevent[iplane]; iHit++) { // loop over good hits
                ahit = (DAuthenticHit*)Hits->At( IndexOfGoodHitsInEvent[iplane][iHit] );
                adist = sqrt( (ahit->Hu-trackPosUVW[iplane](0))*(ahit->Hu-trackPosUVW[iplane](0)) + (ahit->Hv-trackPosUVW[iplane](1))*(ahit->Hv-trackPosUVW[iplane](1)) );
                if(MimoDebug>2) cout << " Good hit " << iHit << " for plane " << theplanenumber[iplane] << " selected at index " << IndexOfGoodHitsInEvent[iplane][iHit] << " with position " << ahit->Hu << ";" << ahit->Hv << " and dist " << adist << " <? " << TrackToHitDistanceLimit << endl;
                if (adist<trackToHitDistance[iplane]) {
                  thehit[iplane]=ahit;
                  trackToHitDistance[iplane]=adist;
                }
              } // end loop over good hits


              //-- Check the nearest hit is within the requested distance

              if( fabs(trackToHitDistance[iplane]) < TrackToHitDistanceLimit ){
                aHitIsFound[iplane] = kTRUE ;
              }
              else if( thehit[iplane] ) { // hit there but too far away, JB 2009/09/08
                nmissthdist[iplane]++;
              }

              if(MimoDebug) cout << " Plane " << theplanenumber[iplane] << ": Nearest hit to track at " << trackToHitDistance[iplane] << " <?> " << TrackToHitDistanceLimit<< ", hit found = " << aHitIsFound[iplane] << endl;

            } // end loop on planes
            //---------------------------------------------------------------

            // ****************** Filling histos
            selection->Fill(5.); // Good track in MIMOSA

            for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
              hnGOODhitwhentrack->Fill(Ngoodhitinevent[iplane]);
              hTrackToClusterMinDistance->Fill(trackToHitDistance[iplane]); // Minimal distance from track to hits
            }

            // ******************

            //---------------------------------------------------------------
            // If both good hits near this track were found
            if ( aHitIsFound[0] && aHitIsFound[1] ){ // if a hit matching the track was found
              StatusEvent[8]++;
              NofClMatchTrack++;
              temp_NofClMatchTrack[ievt_array]++; // for time dependent efficiency

              if(MimoDebug) Info("MimosaMiniVectors"," Two good hits (plane %d, hit %d & plane %d, hit %d) matching the track", theplanenumber[0], thehit[0]->Hhk, theplanenumber[1], thehit[1]->Hhk);

              // ****************************************
              // Analysis of matched hits & tracks

              for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
                NofPixelsInCluster = (thehit[iplane]->HNNS < maxNofPixelsInCluster[iplane])?thehit[iplane]->HNNS:maxNofPixelsInCluster[iplane];
                MaxNofPixelsInCluster = maxNofPixelsInCluster[iplane];
                ClusterCharges_compute( thehit[iplane]);
                ClusterPosition_compute( thehit[iplane], align[iplane]);
                hitPosUVW[iplane].SetValue( thehit[iplane]->Hu, thehit[iplane]->Hv, 0.);
                hitPosXYZ[iplane].SetValue( align[iplane]->TransformHitToTracker(hitPosUVW[iplane]));
                if(MimoDebug>2) {
                  cout << "Plane " << theplanenumber[iplane] << endl;
                  cout << " hit UVW " << hitPosUVW[iplane](0) << ", " << hitPosUVW[iplane](1) << ", " << hitPosUVW[iplane](2) << endl;
                  cout << " hit XYZ " << hitPosXYZ[iplane](0) << ", " << hitPosXYZ[iplane](1) << ", " << hitPosXYZ[iplane](2) << endl;
                }
              } // end loop on planes

              MiniVector_compute();

              // ************************************


              // ****************** Filling histos
              selection->Fill(8.); // good hits associated to a good track

              ClusterPosition_fill( thehit[0]); // added, JB 2011/10/30
              MiniVector_fill( thehit[0], thehit[1]);

              // ******************

              if(MimoDebug) Info("MimosaMiniVectors","Done with good matched track-hits study");

            }  // end if both hit matching the track were found
            //---------------------------------------------------------------

            //---------------------------------------------------------------
            else { // end if no hit matching the track was found
              nmiss++;

              if(ievt/NofCycle*NofCycle == ievt){
                cout << " Number of tracks missed = " << nmiss << endl;
              }

              //--- Do some print out
              if (MimoDebug>2) {

                cout << " -8-BAD TRACK-HIT DIST OR NO HIT evt = "<<ievt;
                for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
                  cout << " dist=" << trackToHitDistance[iplane];
                  cout << " #good hits=" << Ngoodhitinevent[iplane] << ";";
                } // end loop on planes
                cout << endl;
                cout << " -----------Info on track ----------" << endl;
                cout << " Index  of Track       = " << tk1  << endl;
                cout << " Chi2 of track         = " << chi2 << endl;
                cout << " Track slope (Tdu;Tdv) = " << tdu << " ; " << tdv << " --- In degrees :" << 57.29*acos(tdu) << " ; " << 57.29*acos(tdv)  << endl;    // 180:3.14=57.29577
                //cout <<"UofHitCG VofHitCG "<< UofHitCG<<" "<<VofHitCG <<endl;
                //cout <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3 << endl
                cout << " tx=" << tx <<" ty=" << ty <<" tz=" << tz
                <<" tdx="<<tdx <<" tdy="<<tdy <<endl;
                cout<<" tu= "<<tu<<" tv= "<<tv<<endl;

                if( Ngoodhitinevent[0]>0 ) { // if there is at least a hit

                  cout << " -----------Info on nearest hit ----------" << endl;
                  cout << " Charge on Seed HqM[0]*cal         = " << thehit[0]->HqM[0]*calibration << " = " << thehit[0]->HqM[0] << "*" << calibration <<  endl;
                  cout << " thehit[0]->Hqc * calibration         = " << thehit[0]->Hqc * calibration  << endl;
                  cout << " thehit[0]->Hq0 * calibration         = " << thehit[0]->Hq0 * calibration  << endl;
                  cout << " ChargeAroundSeed = (thehit[0]->Hqc-thehit[0]->Hq0)*calibration = " << (thehit[0]->Hqc-thehit[0]->Hq0)*calibration <<  endl;
                  cout << " Noise  on Seed          = " << thehit[0]->Hsn << endl;
                  cout << " SNR    on Seed          = ";
                  if( (thehit[0]->Hsn)!=0. ) { cout << thehit[0]->Hq0/(calibration*thehit[0]->Hsn); }
                  else { cout << 0.; }
                  cout << endl;
                  cout << " Index  of Seed          = " << thehit[0]->Hsk  << " column: " << (thehit[0]->Hsk)%NofPixelInRaw << " -- line: "  << (thehit[0]->Hsk)/NofPixelInRaw<< endl;
                  cout<<" hUdigital="<< thehit[0]->Hsu <<" hVdigital="<< thehit[0]->Hsv <<" pixelsizeV"<< PixelSizeV<<endl;
                } // end if there is at least one hit

                else {
                  cout <<"-----------NO HIT FOUND-----------"<<endl;
                  for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
                    cout <<" Plane "<<theplanenumber[iplane]<<endl;
                    cout <<"  Raw value frame 1="<< Float_t(ThePlaneStudied[iplane]->PFrfr1) <<endl;
                    cout <<"  Raw value frame 2="<< Float_t(ThePlaneStudied[iplane]->PFrfr2) <<endl;
                  } // end loop on planes
                }

                cout <<"-----"<<endl;
              } // end if MimoDebug



              tuv->Fill(tu,tv);

              hchi2_nc->Fill(chi2);
              htu->Fill(tu);
              htv->Fill(tv);
              htuhtv->Fill(tu,tv);
              Int_t itu;
              Int_t itv;
              itu= Int_t(tu/PixelSizeU);
              itv= Int_t(tv/PixelSizeV);
              htuvInPix->Fill(tu-itu*PixelSizeU,tv-itv*PixelSizeV);

              if( Ngoodhitinevent[0]>0 && thehit[0] ) { // if there is at least a hit (thehit is the nearest)
                hqc_nc->Fill(thehit[0]->Hqc);
                hnpix_nc->Fill(thehit[0]->HNNS); // Hsk); JB 2009/09/01
                huvBad->Fill( tu, tv);
                tuv1->Fill( thehit[0]->Hsu, thehit[0]->Hsv);
              }
              else {
                FalseHitMap->Fill(tu,tv);
              }

              // ******************

            } // end if no hit matching the track was found
            //---------------------------------------------------------------

          }  // end if the track is in MIMOSA
          //---------------------------------------------------------------

        } // end if good track
        //---------------------------------------------------------------

      } // end loop on tracks
      //---------------------------------------------------------------

      if(MimoDebug) Info("MimosaMiniVectors","End loop on tracks, found %d good tracks in DUT", NgoodtrackinDUTinevent);

    } // end if Plane exists with tracks and hits
    //---------------------------------------------------------------

    else { // if there was no plane or tracks or hits
      Warning("MimosaMiniVectors","problem event %d with plane %d: #Hits %d<? %d <?%d, pointer to planes are %p and %p, other conditions %d and %d\n\n", ievt, ThePlaneNumber, CUT_MinNbOfHits, NbOfHits, CUT_MaxNbOfHits, ThePlaneStudied[0], ThePlaneStudied[1], (usebaselinecut==kFALSE && ThePlaneStudied[0]->PFrfr1<Baselinecut), (usebaselinecut==kFALSE && ThePlaneStudied[1]->PFrfr1<Baselinecut) // removing warning: adding casts because of 'int' format instead of 'DAuthenticPlane *' BH 2013/08/20
              );
      //return; // allow to continue and simply skip this event, JB 2009/09/01
    }


    if(MimoDebug) Info("MimosaMiniVectors","Event %d ended\n",ievt);

  } // end of Main loop over event
  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  // ************************************
  // Call final part of analysis here

  ClusterPosition_end(); // JB 2014/01/10
  ClusterCharges_end();
  BinarySpecific_end();
  Efficiency_end( MaxEvent-MinEvent+1 ); // JB 2011/11/04

  // ************************************

  // Display final histograms
  MainCanvas->Clear();
  if( selection->GetMaximum()>15*selection->GetBinContent(6)) MainCanvas->SetLogy();
  selection->Draw();
  for( Int_t il=0; il<8; il++) {
    textLabel[il]->Draw();
  }
  MainCanvas->Update();


  //---------------------------------------------------------------
  //-- Print statistics out
  //---------------------------------------------------------------

  cout << "-------- RUN NUMBER = "<<RunNumber<<" Plane numbers = "<<theplanenumber[0]<<", "<<theplanenumber[1]
  <<" submatrix numbers = "<<submatrix0<<", "<<submatrix1<<endl;
  cout << "-------- matched/good tracks in DUT = " << NofClMatchTrack << "  " << NtrkInMimo << "  " << MimosaEfficiency << endl;
  cout << "-------- Efficiency = " << MimosaEfficiency*100.0 <<" +- "<< MimosaEfficiency_ERR*100.0 << "%" << endl;
  cout << "-------- Nb of good rec tracks in telescope per event,  average = " << hNTracksPerEvent->GetMean() << " with RMS = " << hNTracksPerEvent->GetRMS() << endl; // JB 2010/07/23
  cout << "-------- Nb of rec hits in DUT = " << NRecHit << " good = " << ngoodhit << ", average per event " << hnhit->GetMean() << " with RMS " << hnhit->GetRMS() << endl;
  cout << "-------- track-Hit dist cut= "<<TrackToHitDistanceLimit<<" chi2 max= "<<TrackChi2Limit<<" Geomatrix = "<<Thegeomatrix<<endl;
  cout << "-------- number of tracks un-matched " << nmiss << ", with thdist > THdist limit " << nmissthdist << ", without hits in DUT  " << nmissh << endl; // JB 2009/09/08
  cout << "-------- Total number of event processed= "<<MaxEvent-MinEvent+1<<endl;
  cout << "-------- CUTS S/N seed and S/N neighbours= "<<CUT_S2N_seed<<" "<<CUT_S2N_neighbour<<endl;
  cout << "-------- MIN and MAX number of hits per event to evaluate efficiency "<<CUT_MinNbOfHits<<" "<<CUT_MaxNbOfHits<<endl;
  cout << "-------- calibration "<<calibration<<endl;

  //---------------------------------------------------------------
  //-- Write histos
  //---------------------------------------------------------------

  if (fWriteHistos) {
    gSystem->cd(CreateGlobalResultDir());
    if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

    Char_t Header[200];
    sprintf(Header,"AllPlots_%d_%d.root",RunNumber,ThePlaneNumber);
    TFile* AllHist = new TFile(Header,"RECREATE");
    dir->GetList()->Write();
    AllHist->Close(); delete AllHist;
    gSystem->cd(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory

  }

  //---------------------------------------------------------------
  //-- The End
  //---------------------------------------------------------------

  fMimosaMiniVectorsDone=kTRUE;
  fClearDone=kFALSE;
  PreparePost();
}


//______________________________________________________________________________
//
void MimosaAnalysis::MimosaPro2Planes(Int_t MaxEvt, Int_t TrackHitDist, Short_t plane0, Float_t S2N_seed0, Float_t S2N_neighbour0, Int_t submatrix0, Short_t plane1, Float_t S2N_seed1, Float_t S2N_neighbour1, Int_t submatrix1, Int_t GeoMatrix)
{

  // -- Combined analysis of two nearby planes
  //     performed in the tracker coordinate
  //
  // Main analysis method: loops over all events found in a runXXX_0X.root file
  // Fills all histograms
  //
  // Created : JB 2013/05/01, from the MimosaMiniVector method
  // Modified: JB 2013/11/08, new mechanism to load cut values and new cuts

  if(!CheckIfDone("init,clear")) return;
  if(!RunNumber) Error("MimosaPro","RunNumber not set! Please run InitSession  first");

  Int_t theplanenumber[2] = { plane0, plane1};
  ThePlaneNumber = plane0;
  ThePlaneNumber2 = plane1;

  InitMimosaType();
  Info("MimosaPro","Mimosa type %d", MimosaType);

  GetAnalysisGoal();
  fIfReferenceTrack = kTRUE;

  gSystem->ChangeDirectory(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory


  //----------------------------------------------------------------------------------
  // -- Open the input file containing the TTree
  //----------------------------------------------------------------------------------

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0) {
    Error("MimosaPro2Planes"," The input file contains an incorrect number of events %d!",Nevt);
  }
  else {
    Info("MimosaPro2Planes","There is %d events in the input file.",Nevt);
  }

  //----------------------------------------------------------------------------------
  // -- Init analysis variables
  //----------------------------------------------------------------------------------

  // for counting hits and tracks
  Int_t   ngoodhit    = 0; // # good hits in the DUT
  Int_t   NRecHit     = 0; // total # hits in the DUT
  Int_t   nmiss       = 0; // # good tracks in the DUT without a matched hit
  Int_t   nmissh[2]   = {0, 0}; // # good tracks in the DUT with no good hit in the DUT
  Int_t   nmissthdist[2] = {0, 0}; // # good tracks in the DUT with hit outside the distance limit

  Int_t Nhitinevent; //
  Int_t Ngoodhitinevent[2]; // # good hits in the event
  Int_t Ngoodtrackinevent; // # good tracks in the telescope in the event
  Int_t NgoodtrackinDUTinevent; // # good tracks in the telescope and in the DUT in the event

  Float_t trackToHitDistance[2] = { 100000., 100000.};

  // Counter on event status
  Int_t StatusEvent[10];
  for ( Int_t i=0 ; i<10 ; i++ ) {
    StatusEvent[i]=0;
  }

  Int_t IndexOfGoodHitsInEvent[2][5000]; // enlarge to 5000


  //----------------------------------------------------------------------------------
  // -- get parameters
  //----------------------------------------------------------------------------------

  ThesubmatrixNumber = submatrix0;
  GetParameters();

  //----------------------------------------------------------------------------------
  // CUTS
  //----------------------------------------------------------------------------------

  // on baseline
  Bool_t usebaselinecut = kFALSE ;
  Float_t Baselinecut = 1100.0;

  // on hits

  Float_t cut_S2N_seed[2] = {S2N_seed0, S2N_seed1}; // S/N(seed)
  Float_t cut_S2N_neighbour[2] = {S2N_neighbour0, S2N_neighbour1}; // S/N(neighbours)
  Int_t   maxNofPixelsInCluster[2] = {fSession->GetSetup()->GetAnalysisPar().MaxNofPixelsInCluster[submatrix0], fSession->GetSetup()->GetAnalysisPar().MaxNofPixelsInCluster[submatrix1]};

  // on tracks
  TrackToHitDistanceLimit =  TrackHitDist ; // Distance between the track and the hit
  TrackChi2Limit = fSession->GetSetup()->GetAnalysisPar().TrackChi2Limit;

  // on geometry
  Thegeomatrix = GeoMatrix; // define the area of interest in the DUT
  geomUmin = fSession->GetSetup()->GetAnalysisPar().Umin[submatrix0][Thegeomatrix];
  geomUmax = fSession->GetSetup()->GetAnalysisPar().Umax[submatrix0][Thegeomatrix];
  geomVmin = fSession->GetSetup()->GetAnalysisPar().Vmin[submatrix0][Thegeomatrix];
  geomVmax = fSession->GetSetup()->GetAnalysisPar().Vmax[submatrix0][Thegeomatrix];

  cout << "----------- CUTS:" << endl;
  cout << "  Baseline cut " << Baselinecut << " applied ? " << usebaselinecut << endl;
  cout << "  # hits (min, max): " << CUT_MinNbOfHits << " - " << CUT_MaxNbOfHits << endl;
  cout << "  min Q around seed: " << CUT_MinQ_neighbour << endl;
  for( Short_t iplane=0; iplane<2; iplane++) {
    cout << "  For plane " << theplanenumber[iplane] << endl;
    cout << "    max # pixels in a hit: " << maxNofPixelsInCluster[iplane] << endl;
    cout << "    min S/N ratio: for seed = " << cut_S2N_seed[iplane] << ", for neighbours (without seed) = " << cut_S2N_neighbour[iplane] << endl;
  }
  cout << "  max distance track to hit " << TrackToHitDistanceLimit << endl;
  cout << "  max track chi2 " << TrackChi2Limit << endl;
  cout << "  geomatrix is " << Thegeomatrix << ", limits in U: " << geomUmin << ", " << geomUmax << ", limits in V: " << geomVmin << ", " << geomVmax << endl;

  Info("MimosaPro2Planes","Analysis variables initialized");

  //----------------------------------------------------------------------------------
  // -- prepare displays
  //----------------------------------------------------------------------------------

  PrepareOnlineDisplay();

  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();
  // rebin hit map histograms to take into account geomatrix limits
  hxy->SetBins( (Int_t)((geomUmax-geomUmin)/PixelSizeU), geomUmin, geomUmax, (Int_t)((geomVmax-geomVmin)/PixelSizeV), geomVmin, geomVmax );
  hhx->SetBins( (Int_t)((geomUmax-geomUmin)/PixelSizeU), geomUmin, geomUmax );
  hhy->SetBins( (Int_t)((geomVmax-geomVmin)/PixelSizeV), geomVmin, geomVmax );

  //----------------------------------------------------------------------------------
  // -- get the alignement parameters and parametrization of the Eta function
  //----------------------------------------------------------------------------------

  DPrecAlign *align[2];
  TFile* theCorFiles[2] = { 0, 0};

  for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes

    if (!theCorFiles[iplane]) InitCorPar( RunNumber, theplanenumber[iplane]);
    theCorFiles[iplane] = theCorFile;

    if( theCorFiles[iplane]->IsOpen() && theCorFiles[iplane]->FindKey("alignement") ) { // if the CorPar files is already initialized
      align[iplane] =(DPrecAlign*)theCorFiles[iplane]->Get("alignement");
      align[iplane]->Print();
    }
    else {
      Error("MimosaPro2Planes","No alignement found from the file %s, for plane %d, STOPPING\n", theCorFiles[iplane]->GetName(), theplanenumber[iplane]);
    }

    align[iplane]->PrintAll();
    Info("MimosaPro2Planes","Alignement read for plane %d", theplanenumber[iplane]);
  } // end loop on planes

  GetMiEta(); // JB 2010/09/01 shall be aplied to both planes as well!



  // store the cluster info in separate ascii file:
  Char_t Headerclusterdat[100];
  sprintf(Headerclusterdat,"HitSeparation/rawhit/newcluster_%d_%d.dat",RunNumber,ThePlaneNumber);
  ofstream outFile(Headerclusterdat);
  Int_t NofCycle=1000; //was 100


  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  // ************************************
  // Call init part of the analysis here

  MaxNofPixelsInCluster = TMath::Max( maxNofPixelsInCluster[0], maxNofPixelsInCluster[1]);
  Efficiency_init();
  ClusterCharges_init();
  ClusterPosition_init();

  // ************************************

  // a bit of cleaning
  MainCanvas->Clear();

  Info("MimosaPro2Planes","\nReady to loop over %d events\n", MaxEvent);

  // **********************************************************************************
  // ********* MAIN LOOP ***************
  // **********************************************************************************
  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaPro2Planes","Reading event %d",ievt);

    //-- Initilisations

    Nhitinevent=0;
    Ngoodhitinevent[0]=0;
    Ngoodhitinevent[1]=0;
    Ngoodtrackinevent=0;
    NgoodtrackinDUTinevent=0;
    for ( Int_t i=0 ; i<10 ; i++ ) { StatusEvent[i]=0; }

    //-- Update display

    if (ievt == MinEvent) {
      selection->Draw();
      for( Int_t il=0; il<8; il++) {
        textLabel[il]->Draw();
      }
    }

    if(ievt/NofCycle*NofCycle == ievt){
      if( ievt>1000) NofCycle = 2000;
      if( ievt>5000) NofCycle = 5000;
      if( selection->GetMaximum()>15*selection->GetBinContent(6)) MainCanvas->SetLogy();
      MainCanvas->Modified();
      MainCanvas->Update();
      cout << "MimosaPro2Planes Event " << ievt <<endl;
      if (NtrkInMimo>100){
        cout<<"  temporary efficiency = "<<NofClMatchTrack<<" / "<<NtrkInMimo<<" = "<<1.*NofClMatchTrack/NtrkInMimo<<endl;
      }
    }

    //-- Update counter for efficiency

    StatusEvent[1]++;
    if(ievt / NeventRangeForEfficiency * NeventRangeForEfficiency == ievt ){ ievt_array ++; }

    //=========================
    if(MimoDebug>1) cout << " getting the event" << endl;
    t->GetEvent(ievt);
    //=========================


    if(MimoDebug>1) cout << " getting the planes, tracks and hits" << endl;
    TClonesArray *Trpl   = Evt->GetTransparentPlanes() ; //tracks
    TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
    TClonesArray *Planes = Evt->GetAuthenticPlanes()   ; //planes
    Int_t NbOfTrpl   = Trpl->GetLast()+1   ; // total # tracks over all planes
    Int_t NbOfHits   = Hits->GetLast()+1   ; // total # hits over all planes
    Int_t NbOfPlanes = Planes->GetLast()+1 ; // Total Number of Planes

    // check to avoid crash due to empty event
    if( NbOfTrpl==0 || NbOfHits==0 || NbOfPlanes==0 ) {
      if (MimoDebug) Info("MimosaPro2Planes","Empty event %d: #tracks %d, #hits %d, #planes %d \n", ievt, NbOfTrpl, NbOfHits, NbOfPlanes);
      continue;
    }

    // get pointers to the planes studied
    DAuthenticPlane *ThePlaneStudied[2] = {0, 0}; // init
    DAuthenticPlane *aPlane;
    for( Int_t iplane=0; iplane<NbOfPlanes; iplane++) {
      aPlane = (DAuthenticPlane*) Planes->At(iplane);
      if ( aPlane->Ppk == theplanenumber[0] ) ThePlaneStudied[0] = aPlane;
      else if ( aPlane->Ppk == theplanenumber[1] ) ThePlaneStudied[1] = aPlane;
    }


    // ****************** Filling histos
    selection->Fill(1.);  // all events

    // ******************

    //---------------------------------------------------------------
    // if planes are there
    // and baseline cuts OK
    // and number of hits and tracks are within limits
    if ( ThePlaneStudied[0] && ThePlaneStudied[1]
        && (usebaselinecut==kFALSE || (ThePlaneStudied[0]->PFrfr1<Baselinecut))
        && (usebaselinecut==kFALSE || (ThePlaneStudied[1]->PFrfr1<Baselinecut))
        && NbOfHits<CUT_MaxNbOfHits && NbOfHits>=CUT_MinNbOfHits
        ) { // Plane exists with tracks and hits
      StatusEvent[2]++;

      // ****************** Filling histos
      selection->Fill(2.); // event with track(s) and hits in the plane

      hnhit->Fill(ThePlaneStudied[0]->PhN + ThePlaneStudied[1]->PhN);
      hnhitievt->Fill(ievt,ThePlaneStudied[0]->PhN + ThePlaneStudied[1]->PhN);
      // ******************

      //---------------------------------------------------------------
      // Loop over hits BUT keep only the ones in the right plane
      if(MimoDebug>1) cout << " looping over " << NbOfHits << " hits" << endl;
      for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

        DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);

        //------------------------------------------
        for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes

          //------------------------------------------
          if( ahit->Hpk == theplanenumber[iplane] ) { // if hit belongs to expected plane
            StatusEvent[6]++;
            Nhitinevent++;
            NRecHit++;

            // Now compute variables usefull for cuts

            NofPixelsInCluster = (ahit->HNNS < maxNofPixelsInCluster[iplane])?ahit->HNNS:maxNofPixelsInCluster[iplane];
            if (MimoDebug>2) Info("MimosaPro2Planes"," NofPixelsInCluster is set to %d (in hit %d, max %d)\n", NofPixelsInCluster, ahit->HNNS, maxNofPixelsInCluster[iplane]);

            // signal over noise ratio of the seed pixel
            Float_t snSeed;
            if( ahit->Hn0 > 0.1 ) {
              snSeed=ahit->Hq0/ahit->Hn0/NoiseScope;
            }
            else { // for sparsified readout, JB 2009/05/19
              snSeed=ahit->Hq0;
            }

            // charge and S/N in the second highest pixel of the hit
            // among the 4 pixels neighbouring the seed (cross)
            Float_t q2nd = 0.;
            Float_t sn2nd = 0.;
            for(Int_t i=0; i < NofPixelsInCluster; i++){
              //cout << "i=" << i << " index=" << ahit->HkM[i]- ahit->HkM[0] << " q=" << ahit->HqM[i] << endl;
              if( (abs(ahit->HkM[i]- ahit->HkM[0])==1 || abs(ahit->HkM[i]- ahit->HkM[0])== NofPixelInRaw)
                 && ahit->HqM[i]>q2nd	){
                q2nd = ahit->HqM[i];
                if( ahit->HnM[i] != 0.0 ) {
                  sn2nd = q2nd/ahit->HnM[i]/NoiseScope;
                }
                else { // for sparsified readout, JB 2009/05/19
                  sn2nd = q2nd;
                }
              }
            }

            // charge of the cluster without the seed
            ChargeAroundSeed = (ahit->Hqc - ahit->Hq0)*calibration;

            if(MimoDebug>2) {
              printf("MimosaPro2Planes: Hit %d, qSeed=%.1f, qAroundSeed=%.1f, snSeed=%.1f, sn2nd=%.1f\n", iHit, ahit->Hq0, ChargeAroundSeed,snSeed, sn2nd);
            }

            // ****************** Filling histos
            selection->Fill(6.);  // all hits in MIMOSA

            // ******************


            //--------------------------------------------------------------
            // If hit passes the selection cut
            if(
               snSeed >= cut_S2N_seed[iplane]
               && ( ahit->HSNneighbour >= cut_S2N_neighbour[iplane])
               && ( ChargeAroundSeed >= CUT_MinQ_neighbour )
               && ( ahit->HNNS <= maxNofPixelsInCluster[iplane] )
               //&& TrackInMimo( Thegeomatrix, ahit->Hu, ahit->Hv)

               ){ // if hit passes cuts
              StatusEvent[7]++;
              ngoodhit++;
              IndexOfGoodHitsInEvent[iplane][Ngoodhitinevent[iplane]] = iHit;
              Ngoodhitinevent[iplane]++;
              if(MimoDebug>2) cout << " plane " << theplanenumber[iplane] << " found good hit " << Ngoodhitinevent[iplane]-1 << " with index " << IndexOfGoodHitsInEvent[iplane][Ngoodhitinevent[iplane]-1] << " pos " << ahit->Hu << ";" << ahit->Hv << " and dist to track " << ahit->Hu-ahit->Htu << ";" << ahit->Hv-ahit->Htv << endl;

              // ****************** Filling histos
              selection->Fill(7.); // Good hits in MIMOSA

              hnGOODhit->Fill(Ngoodhitinevent[iplane]);
              hnahitievt->Fill( ievt, Ngoodhitinevent[iplane]);
              GoodHit_Fill( ahit); // JB 2010/10/06
              hgoodSN_vs_SNN->Fill(ahit->HSNneighbour,snSeed);

              BinarySpecific_HitsOnly_fill( ahit);

              if( ahit->Htk >= 0) { // if a track was associated in Tree
                Float_t adist = sqrt((ahit->Hsu-ahit->Htu)*(ahit->Hsu-ahit->Htu)+(ahit->Hsv-ahit->Htv)*(ahit->Hsv-ahit->Htv)) ;
                duvall->Fill( adist); //For all the clusters with Hqc-Hq0>Cut
                hAllHuvsAllTu->Fill( ahit->Htu, ahit->Hsu);
                hAllHuvsAllTv->Fill( ahit->Htv, ahit->Hsu);
                hAllHvvsAllTu->Fill( ahit->Htu, ahit->Hsv);
                hAllHvvsAllTv->Fill( ahit->Htv, ahit->Hsv);
                if(fabs( sqrt( (ahit->Htu-ahit->Hu)*(ahit->Htu-ahit->Hu) + (ahit->Htv-ahit->Hv)*(ahit->Htv-ahit->Hv) ) ) > TrackToHitDistanceLimit){
                }
              } // end if a track was associated
              // ******************

            } // end if hit passes cuts
            //--------------------------------------------------------------

            else { // if hit does not pass cuts
              if(MimoDebug>2) printf(" plane %d, hit %d NOT passing selection cuts, ChargeAroundSeed=%f <> %f, seedCharge=%.1f <> %.0f, NofPixelsInCluster=%d\n", theplanenumber[iplane], iHit, ChargeAroundSeed, CUT_MinQ_neighbour, ahit->HqM[0]*calibration, 0., NofPixelsInCluster);
            }

          } // end if hit belongs to expected plane
          //--------------------------------------------------------------

        } // end loop on planes
        //--------------------------------------------------------------

      } // end loop on hits
      //--------------------------------------------------------------

      if(MimoDebug) Info("MimosaPro2Planes","End loop on hits, found %d and %d good hits from planes", Ngoodhitinevent[0], Ngoodhitinevent[1]);


      //---------------------------------------------------------------
      // Loop on tracks BUT keep only the ones in the right plane
      if(MimoDebug>1) cout << " looping over " << NbOfTrpl << " tracks" << endl;
      for (Int_t iTrk=0 ; iTrk<NbOfTrpl ; iTrk++){ // loop on tracks

        DTransparentPlane *aTrpl = (DTransparentPlane*) Trpl->At(iTrk);
        if( aTrpl->Tpk != theplanenumber[0] ) continue; // select only tracks in DUT

        StatusEvent[3]++;


        // ****************** Filling histos
        selection->Fill(3.); // all tracks in plane

        TrackParameters_allFill( aTrpl, ievt);
        // ******************

        if( MimoDebug>2) cout << " Track " << iTrk << " has chi2=" << aTrpl->Tchi2 << " <?> " << TrackChi2Limit << endl;

        //---------------------------------------------------------------
        // if chi2 of selected track is OK
        if ( aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 < TrackChi2Limit // JB, chi2=0. is not a real pb
            //&& aTrpl->Tdx<0.002 && aTrpl->Tdy<0.002 // insure track is flat, NOT MANDATORY
            ){ // if good track
          StatusEvent[4]++;
          Ngoodtrackinevent++;

          // compute track information for each plane
          for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
            alignement = align[iplane];
            TrackParameters_compute(aTrpl, alignement);
            trackPosUVW[iplane].SetValue( tu, tv, 0.);
            trackPosXYZ[iplane].SetValue( tx, ty, tz);
            //trackPosUVW[iplane].SetValue( alignement->TransformTrackToPlane(tx,ty,tz,tdx,tdy));
            //trackPosXYZ[iplane].SetValue( alignement->GetTrackPosition());
            if( MimoDebug>2) {
              cout << "Plane " << theplanenumber[iplane] << endl;
              cout << " track XYZ " << trackPosXYZ[iplane](0) << ", " << trackPosXYZ[iplane](1) << ", " << trackPosXYZ[iplane](2) << endl;
              cout << " track UVW " << trackPosUVW[iplane](0) << ", " << trackPosUVW[iplane](1) << ", " << trackPosUVW[iplane](2) << endl;
            }
          }

          /*******************/
          selection->Fill(4.);   // Good quality track

          hNTracksPerEvent->Fill( Ngoodtrackinevent);
          hNTracksPerEventievt->Fill( ievt, Ngoodtrackinevent);

          TrackParameters_goodFill( aTrpl, ievt);

          /*******************/

          Bool_t TrkCrossMimo = TrackInMimo( Thegeomatrix, trackPosUVW[0](0), trackPosUVW[0](1), ThesubmatrixNumber);

          if(MimoDebug>1) cout << " Is track " << iTrk << "in MIMOSA ? " << TrkCrossMimo << endl;

          //---------------------------------------------------------------
          // if track is in the good geometry range
          if( TrkCrossMimo == kTRUE ){ // if the track is in MIMOSA
            StatusEvent[5]++;
            NgoodtrackinDUTinevent++;
            NtrkInMimo++ ;

            if(MimoDebug) Info("MimosaPro2Planes"," Good (chi2=%f) track %d found in MIMOSA", aTrpl->Tchi2, iTrk);

            temp_NtrkInMimo[ievt_array]++;

            //-- Find the nearest good hit to this track in each plane
            DAuthenticHit *thehit[2] = {0, 0};
            Bool_t aHitIsFound[2] = {kFALSE, kFALSE};
            DAuthenticHit *ahit = 0;
            Float_t adist;

            //------------------------------------------
            for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes

              trackToHitDistance[iplane] = 10000000.0 ;

              if( Ngoodhitinevent[iplane]<=0 ) { // no hit at all, JB 2009/09/08
                nmissh[iplane]++;
              }

              if(MimoDebug>2) cout << "  Trying to associate hits of plane " << theplanenumber[iplane] << ", loop on " << Ngoodhitinevent[iplane] << " candidates." << endl;
              for(Int_t iHit=0; iHit<Ngoodhitinevent[iplane]; iHit++) { // loop over good hits
                ahit = (DAuthenticHit*)Hits->At( IndexOfGoodHitsInEvent[iplane][iHit] );
                adist = sqrt( (ahit->Hu-trackPosUVW[iplane](0))*(ahit->Hu-trackPosUVW[iplane](0)) + (ahit->Hv-trackPosUVW[iplane](1))*(ahit->Hv-trackPosUVW[iplane](1)) );
                if(MimoDebug>2) cout << " Good hit " << iHit << " for plane " << theplanenumber[iplane] << " selected at index " << IndexOfGoodHitsInEvent[iplane][iHit] << " with position " << ahit->Hu << ";" << ahit->Hv << " and dist " << adist << " <? " << TrackToHitDistanceLimit << endl;
                if (adist<trackToHitDistance[iplane]) {
                  thehit[iplane]=ahit;
                  trackToHitDistance[iplane]=adist;
                }
              } // end loop over good hits


              //-- Check the nearest hit is within the requested distance

              if( fabs(trackToHitDistance[iplane]) < TrackToHitDistanceLimit ){
                aHitIsFound[iplane] = kTRUE ;
              }
              else if( thehit[iplane] ) { // hit there but too far away, JB 2009/09/08
                nmissthdist[iplane]++;
              }

              if(MimoDebug) cout << " Plane " << theplanenumber[iplane] << ": Nearest hit to track at " << trackToHitDistance[iplane] << " <?> " << TrackToHitDistanceLimit<< ", hit found = " << aHitIsFound[iplane] << endl;

            } // end loop on planes
            //---------------------------------------------------------------

            // ****************** Filling histos
            selection->Fill(5.); // Good track in MIMOSA

            for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
              hnGOODhitwhentrack->Fill(Ngoodhitinevent[iplane]);
              hTrackToClusterMinDistance->Fill(trackToHitDistance[iplane]); // Minimal distance from track to hits
            }

            // ******************

            //---------------------------------------------------------------
            // If a good hit in one of the two plane near this track was found
            if ( aHitIsFound[0] || aHitIsFound[1] ){ // if a hit matching the track was found
              StatusEvent[8]++;
              NofClMatchTrack++;
              temp_NofClMatchTrack[ievt_array]++; // for time dependent efficiency

              if(MimoDebug) Info("MimosaPro2Planes"," Good hit found for plane %d, hit %d and for plane %d, hit %d) matching the track", theplanenumber[0], (aHitIsFound[0]?thehit[0]->Hhk:0), theplanenumber[1], (aHitIsFound[1]?thehit[1]->Hhk:0));

              // ****************************************
              // Analysis of matched hits & tracks

              for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
                if( aHitIsFound[iplane] ) { // if a hit was found for this plane
                  NofPixelsInCluster = (thehit[iplane]->HNNS < maxNofPixelsInCluster[iplane])?thehit[iplane]->HNNS:maxNofPixelsInCluster[iplane];
                  MaxNofPixelsInCluster = maxNofPixelsInCluster[iplane];
                  ClusterCharges_compute( thehit[iplane]);
                  ClusterPosition_compute( thehit[iplane], align[iplane]);
                  hitPosUVW[iplane].SetValue( thehit[iplane]->Hu, thehit[iplane]->Hv, 0.);
                  hitPosXYZ[iplane].SetValue( align[iplane]->TransformHitToTracker(hitPosUVW[iplane]));
                  if(MimoDebug>2) {
                    cout << "Plane " << theplanenumber[iplane] << endl;
                    cout << " hit UVW " << hitPosUVW[iplane](0) << ", " << hitPosUVW[iplane](1) << ", " << hitPosUVW[iplane](2) << endl;
                    cout << " hit XYZ " << hitPosXYZ[iplane](0) << ", " << hitPosXYZ[iplane](1) << ", " << hitPosXYZ[iplane](2) << endl;
                  }

                  ClusterPosition_fill( thehit[iplane]);

                } // end if a hit was found for this plane
              } // end loop on planes

              // ************************************


              // ****************** Filling histos
              selection->Fill(8.); // good hits associated to a good track


              // ******************

              if(MimoDebug) Info("MimosaPro2Planes","Done with good matched track-hits study");

            }  // end if both hit matching the track were found
            //---------------------------------------------------------------

            //---------------------------------------------------------------
            else { // end if no hit matching the track was found
              nmiss++;

              if(ievt/NofCycle*NofCycle == ievt){
                cout << " Number of tracks missed = " << nmiss << endl;
              }

              //--- Do some print out
              if (MimoDebug>2) {

                cout << " -8-BAD TRACK-HIT DIST OR NO HIT evt = "<<ievt;
                for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
                  cout << " dist=" << trackToHitDistance[iplane];
                  cout << " #good hits=" << Ngoodhitinevent[iplane] << ";";
                } // end loop on planes
                cout << endl;
                cout << " -----------Info on track ----------" << endl;
                cout << " Index  of Track       = " << tk1  << endl;
                cout << " Chi2 of track         = " << chi2 << endl;
                cout << " Track slope (Tdu;Tdv) = " << tdu << " ; " << tdv << " --- In degrees :" << 57.29*acos(tdu) << " ; " << 57.29*acos(tdv)  << endl;    // 180:3.14=57.29577
                //cout <<"UofHitCG VofHitCG "<< UofHitCG<<" "<<VofHitCG <<endl;
                //cout <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3 << endl
                cout << " tx=" << tx <<" ty=" << ty <<" tz=" << tz
                <<" tdx="<<tdx <<" tdy="<<tdy <<endl;
                cout<<" tu= "<<tu<<" tv= "<<tv<<endl;

                if( Ngoodhitinevent[0]>0 ) { // if there is at least a hit

                  cout << " -----------Info on nearest hit ----------" << endl;
                  cout << " Charge on Seed HqM[0]*cal         = " << thehit[0]->HqM[0]*calibration << " = " << thehit[0]->HqM[0] << "*" << calibration <<  endl;
                  cout << " thehit[0]->Hqc * calibration         = " << thehit[0]->Hqc * calibration  << endl;
                  cout << " thehit[0]->Hq0 * calibration         = " << thehit[0]->Hq0 * calibration  << endl;
                  cout << " ChargeAroundSeed = (thehit[0]->Hqc-thehit[0]->Hq0)*calibration = " << (thehit[0]->Hqc-thehit[0]->Hq0)*calibration <<  endl;
                  cout << " Noise  on Seed          = " << thehit[0]->Hsn << endl;
                  cout << " SNR    on Seed          = ";
                  if( (thehit[0]->Hsn)!=0. ) { cout << thehit[0]->Hq0/(calibration*thehit[0]->Hsn); }
                  else { cout << 0.; }
                  cout << endl;
                  cout << " Index  of Seed          = " << thehit[0]->Hsk  << " column: " << (thehit[0]->Hsk)%NofPixelInRaw << " -- line: "  << (thehit[0]->Hsk)/NofPixelInRaw<< endl;
                  cout<<" hUdigital="<< thehit[0]->Hsu <<" hVdigital="<< thehit[0]->Hsv <<" pixelsizeV"<< PixelSizeV<<endl;
                } // end if there is at least one hit

                else {
                  cout <<"-----------NO HIT FOUND-----------"<<endl;
                  for( Short_t iplane=0; iplane<2; iplane++) { // loop on planes
                    cout <<" Plane "<<theplanenumber[iplane]<<endl;
                    cout <<"  Raw value frame 1="<< Float_t(ThePlaneStudied[iplane]->PFrfr1) <<endl;
                    cout <<"  Raw value frame 2="<< Float_t(ThePlaneStudied[iplane]->PFrfr2) <<endl;
                  } // end loop on planes
                }

                cout <<"-----"<<endl;
              } // end if MimoDebug



              tuv->Fill(tu,tv);

              hchi2_nc->Fill(chi2);
              htu->Fill(tu);
              htv->Fill(tv);
              htuhtv->Fill(tu,tv);
              Int_t itu;
              Int_t itv;
              itu= Int_t(tu/PixelSizeU);
              itv= Int_t(tv/PixelSizeV);
              htuvInPix->Fill(tu-itu*PixelSizeU,tv-itv*PixelSizeV);

              if( Ngoodhitinevent[0]>0 && thehit[0] ) { // if there is at least a hit (thehit is the nearest)
                hqc_nc->Fill(thehit[0]->Hqc);
                hnpix_nc->Fill(thehit[0]->HNNS); // Hsk); JB 2009/09/01
                huvBad->Fill( tu, tv);
                tuv1->Fill( thehit[0]->Hsu, thehit[0]->Hsv);
              }
              else {
                FalseHitMap->Fill(tu,tv);
              }

              // ******************

            } // end if no hit matching the track was found
            //---------------------------------------------------------------

          }  // end if the track is in MIMOSA
          //---------------------------------------------------------------

        } // end if good track
        //---------------------------------------------------------------

      } // end loop on tracks
      //---------------------------------------------------------------

      if(MimoDebug) Info("MimosaPro2Planes","End loop on tracks, found %d good tracks in DUT", NgoodtrackinDUTinevent);

    } // end if Plane exists with tracks and hits
    //---------------------------------------------------------------

    else { // if there was no plane or tracks or hits
      Warning("MimosaPro2Planes","problem event %d with plane %d: #Hits %d<? %d <?%d, pointer to planes are %d and %d, other conditions %d and %d\n\n",
	      int(ievt),
	      int(ThePlaneNumber),
	      int(CUT_MinNbOfHits),
	      int(NbOfHits),
	      int(CUT_MaxNbOfHits),
	      ThePlaneStudied[0]->Ppk,
	      ThePlaneStudied[1]->Ppk,
	      int((usebaselinecut==kFALSE && ThePlaneStudied[0]->PFrfr1<Baselinecut)),
	      int((usebaselinecut==kFALSE && ThePlaneStudied[1]->PFrfr1<Baselinecut)) // removing warning: adding casts because of 'int' format instead of 'DAuthenticPlane *' BH 2013/08/20
              );
      //return; // allow to continue and simply skip this event, JB 2009/09/01
    }


    if(MimoDebug) Info("MimosaPro2Planes","Event %d ended\n",ievt);

  } // end of Main loop over event
  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  // ************************************
  // Call final part of analysis here

  ClusterPosition_end(); // JB 2014/01/10
  ClusterCharges_end();
  BinarySpecific_end();
  Efficiency_end( MaxEvent-MinEvent+1 ); // JB 2011/11/04

  // ************************************

  // Display final histograms
  MainCanvas->Clear();
  if( selection->GetMaximum()>15*selection->GetBinContent(6)) MainCanvas->SetLogy();
  selection->Draw();
  for( Int_t il=0; il<8; il++) {
    textLabel[il]->Draw();
  }
  MainCanvas->Update();


  //---------------------------------------------------------------
  //-- Print statistics out
  //---------------------------------------------------------------

  cout << "-------- RUN NUMBER = "<<RunNumber<<" Plane numbers = "<<theplanenumber[0]<<", "<<theplanenumber[1]
  <<" submatrix numbers = "<<submatrix0<<", "<<submatrix1<<endl;
  cout << "-------- matched/good tracks in DUT = " << NofClMatchTrack << "  " << NtrkInMimo << "  " << MimosaEfficiency << endl;
  cout << "-------- Efficiency = " << MimosaEfficiency*100.0 <<" +- "<< MimosaEfficiency_ERR*100.0 << "%" << endl;
  cout << "-------- Nb of good rec tracks in telescope per event,  average = " << hNTracksPerEvent->GetMean() << " with RMS = " << hNTracksPerEvent->GetRMS() << endl; // JB 2010/07/23
  cout << "-------- Nb of rec hits in DUT = " << NRecHit << " good = " << ngoodhit << ", average per event " << hnhit->GetMean() << " with RMS " << hnhit->GetRMS() << endl;
  cout << "-------- track-Hit dist cut= "<<TrackToHitDistanceLimit<<" chi2 max= "<<TrackChi2Limit<<" Geomatrix = "<<Thegeomatrix<<endl;
  cout << "-------- number of tracks un-matched " << nmiss << ", with thdist > THdist limit " << nmissthdist << ", without hits in DUT  " << nmissh << endl; // JB 2009/09/08
  cout << "-------- Total number of event processed= "<<MaxEvent-MinEvent+1<<endl;
  cout << "-------- CUTS S/N seed and S/N neighbours= "<<CUT_S2N_seed<<" "<<CUT_S2N_neighbour<<endl;
  cout << "-------- MIN and MAX number of hits per event to evaluate efficiency "<<CUT_MinNbOfHits<<" "<<CUT_MaxNbOfHits<<endl;
  cout << "-------- calibration "<<calibration<<endl;

  //---------------------------------------------------------------
  //-- Write histos
  //---------------------------------------------------------------

  if (fWriteHistos) {
    gSystem->cd(CreateGlobalResultDir());
    if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

    Char_t Header[200];
    sprintf(Header,"AllPlots_%d_%d.root",RunNumber,ThePlaneNumber);
    TFile* AllHist = new TFile(Header,"RECREATE");
    dir->GetList()->Write();
    AllHist->Close(); delete AllHist;
    gSystem->cd(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory

  }

  //---------------------------------------------------------------
  //-- The End
  //---------------------------------------------------------------

  fMimosaPro2PlanesDone=kTRUE;
  fClearDone=kFALSE;
  PreparePost();
}


//______________________________________________________________________________
//
void MimosaAnalysis::MimosaVertex(Int_t MaxEvt, Short_t plane1, Short_t plane2)
{

  // -- Vertex finder with track from the root DSF file
  //
  // Created: JB 2011/07/26

  if(!CheckIfDone("init,clear")) return;
  if(!RunNumber) Error("MimosaVertex","RunNumber not set! Please run InitSession  first");

  InitMimosaType();
  Info("MimosaVertex","Mimosa type %d", MimosaType);

  GetAnalysisGoal();
  fIfReferenceTrack = kTRUE;
  fIfVertex = kTRUE;

  //----------------------------------------------------------------------------------
  // -- Open the input file containing the TTree
  //----------------------------------------------------------------------------------

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0) {
    Error("MimosaVertex"," The input file contains an incorrect number of events %d!",Nevt);
  }
  else {
    Info("MimosaVertex","There is %d events in the input file.",Nevt);
  }

  //----------------------------------------------------------------------------------
  // -- Init analysis variables
  //----------------------------------------------------------------------------------

  // for counting hits and tracks
  Int_t Nhitinevent;
  Int_t Ntrackinevent;
  //Int_t Ngoodhitinevent; // # good hits in the event
  Int_t Ngoodtrackinevent; // # good tracks in the telescope in the event

  // Counter on event status
  Int_t StatusEvent[10];
  for ( Int_t i=0 ; i<10 ; i++ ) {
    StatusEvent[i]=0;
  }
  const Int_t NofCycle=5000; //was 100

  //----------------------------------------------------------------------------------
  // -- get parameters
  //----------------------------------------------------------------------------------

  GetParameters();

  Info("MimosaVertex","Analysis variables initialized");

  //----------------------------------------------------------------------------------
  // -- prepare displays
  //----------------------------------------------------------------------------------

  PrepareOnlineDisplay();

  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();

  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;//50000;//40000; //was 2000, modifyed by JB Sept 2007 to match number of events for initialization (ped and noise)
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  // ************************************
  // Call init part of the analysis here


  // ************************************


  // a bit of cleaning
  MainCanvas->Clear();

  Info("MimosaVertex","\nReady to loop over %d events (over %lld in the tree)\n", MaxEvent, t->GetEntries()); // removing warning: specifying type 'Long64_t' (aka 'long long') format BH 2013/08/20

  // **********************************************************************************
  // ********* MAIN LOOP ***************
  // **********************************************************************************
  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaVertex","Reading event %d",ievt);

    //-- Initilisations

    //Short_t NhitInPlane = 0;
    //Short_t NtrackInPlane = 0;
    Nhitinevent=0;
    Ntrackinevent=0;
    //Ngoodhitinevent=0;
    Ngoodtrackinevent=0;

    DTransparentPlane *aTrpl;

    for ( Int_t i=0 ; i<10 ; i++ ) { StatusEvent[i]=0; }

    //-- Update display

    if (ievt == MinEvent) {
      selection->Draw();
      for( Int_t il=0; il<8; il++) {
	textLabel[il]->Draw();
      }
    }

    if(ievt/NofCycle*NofCycle == ievt || ievt<10){
      if( selection->GetMaximum()>15*selection->GetBinContent(6)) MainCanvas->SetLogy(); // JB 2010/04/28
      MainCanvas->Modified();
      MainCanvas->Update();
      cout << "MimosaVertex Event " << ievt <<endl;
    }

    //-- Update counter for efficiency

    StatusEvent[1]++;

    //=========================
    if(MimoDebug>1) cout << " getting the event" << endl;
    t->GetEvent(ievt);
    //=========================

    if(MimoDebug>1) cout << " getting the planes, tracks and hits" << endl;
    TClonesArray *Trpl   = Evt->GetTransparentPlanes() ; //tracks
    TClonesArray *Planes = Evt->GetAuthenticPlanes()   ; //planes
    Int_t NbOfTrpl   = Trpl->GetLast()+1   ; // total # tracks over all planes

    // check to avoid crash due to empty event, added by JB, Sept 2008,
    if( NbOfTrpl==0 ) {
      if (MimoDebug) Info("MimosaVertex","Empty event %d: #tracks %d\n", ievt, NbOfTrpl);
      continue;
    }

    // compute the number of tracks from the one present in the planes with status 0
    DAuthenticPlane *ThePlaneStudied = 0; // init
    Int_t j=0; // init
    while(j<Planes->GetLast()+1) {
      ThePlaneStudied=(DAuthenticPlane*) Planes->At(j) ;
      //printf("testing %d against %d\n", ThePlaneStudied->Ppk, ThePlaneNumber);
      if (ThePlaneStudied->Ppk == plane1 || ThePlaneStudied->Ppk == plane2 ) {
	Nhitinevent += ThePlaneStudied->PhN;
	Ntrackinevent += ThePlaneStudied->PtN;
      }
      j++;
    }

    // ****************** Filling histos
    selection->Fill(1.);  // all events

    // ******************

    //---------------------------------------------------------------
    // if Both TransparentPlane and AuthenticPlane exist : (JB, Sept 2008)
    // and number of hits and tracks are within limits
    if ( Nhitinevent<CUT_MaxNbOfHits && Nhitinevent>=CUT_MinNbOfHits && NbOfTrpl>0) { // Plane exists with tracks and hits
      StatusEvent[2]++;

      // ****************** Filling histos
      selection->Fill(2.); // event with track(s) and hits in the plane
      hnhit->Fill(Nhitinevent);
      hnhitievt->Fill(ievt,Nhitinevent);
      hNTracksPerEvent->Fill(Ntrackinevent);
      // ******************

      //---------------------------------------------------------------
      // Loop on tracks BUT keep only the ones in the right plane
      for (Int_t iTrk=0 ; iTrk<NbOfTrpl ; iTrk++){ // loop on tracks

	aTrpl = (DTransparentPlane*) Trpl->At(iTrk);

	StatusEvent[3]++;


	// ****************** Filling histos
	selection->Fill(3.); // all tracks in plane

	//TrackParameters_allFill( aTrpl, ievt);
	// ******************

	if( MimoDebug>1) Info("MimosaPro"," Track %d has chi2=%f <> %f ?", iTrk, aTrpl->Tchi2, TrackChi2Limit);

	//---------------------------------------------------------------
	// if chi2 of selected track is OK
	if ( aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 < TrackChi2Limit // JB, chi2=0. is not a real pb
	    //&& aTrpl->Tdx<0.002 && aTrpl->Tdy<0.002 // insure track is flat, NOT MANDATORY, JB sept 2007
	    ){ // if good track
	  StatusEvent[4]++;
	  Ngoodtrackinevent++;

	  //TrackParameters_compute(aTrpl, alignement);

	  /*******************/
	  selection->Fill(4.);   // Good quality track

	  hNTracksPerEvent->Fill( Ngoodtrackinevent);
	  hNTracksPerEventievt->Fill( ievt, Ngoodtrackinevent);

	  //TrackParameters_goodFill( aTrpl, ievt);

	  /*******************/


	} // end if good track
	//---------------------------------------------------------------

      } // end loop on tracks
      //---------------------------------------------------------------

      if(MimoDebug) Info("MimosaPro","End loop on tracks, found %d good tracks", Ngoodtrackinevent);


    } // end if Plane exists with tracks and hits
    //---------------------------------------------------------------

    if(MimoDebug) Info("MimosaPro","Event %d ended\n",ievt);

  } // end of Main loop over event
  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  // ************************************
  // Call final part of analysis here


  // ************************************


  // Display final histograms
  MainCanvas->Clear();
  MainCanvas->SetBorderMode(0);
  MainCanvas->Divide(1,2);
  MainCanvas->cd(1);
  if( selection->GetMaximum()>15*selection->GetBinContent(6)) gPad->SetLogy();
  selection->Draw();
  for( Int_t il=0; il<8; il++) {
    textLabel[il]->Draw();
  }
  MainCanvas->cd(2);
  MainCanvas->Update();

  fMimosaProDone=kTRUE;

  //---------------------------------------------------------------
  //-- Print statistics out
  //---------------------------------------------------------------

  cout << "-------- RUN NUMBER = "<<RunNumber<<" Plane number = "<<ThePlaneNumber;
  cout << "-------- Nb of good rec tracks in telescope per event,  average = " << hNTracksPerEvent->GetMean() << " with RMS = " << hNTracksPerEvent->GetRMS() << endl; // JB 2010/07/23
  cout << "-------- Total number of event processed= "<<MaxEvent-MinEvent+1<<endl;

  //---------------------------------------------------------------
  //-- Write histos
  //---------------------------------------------------------------

  if (fWriteHistos) {
    gSystem->cd(CreateGlobalResultDir());
    if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

    Char_t Header[200];
    sprintf(Header,"VertexPlots_%d_%d.root",RunNumber,ThePlaneNumber);
    TFile* AllHist = new TFile(Header,"RECREATE");
    dir->GetList()->Write();
    AllHist->Close(); delete AllHist;
    gSystem->cd(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory

  }

  //---------------------------------------------------------------
  //-- The End
  //---------------------------------------------------------------

  fMimosaProDone=kTRUE;
  fClearDone=kFALSE;
  PreparePost();

}


//______________________________________________________________________________
//
void MimosaAnalysis::MimosaVertexFinder(Int_t MaxEvt, Int_t submatrix , Int_t GeoMatrix, Float_t chi2Cut)
{

  // ???
  //
  // Loic ?

  if(!CheckIfDone("init,clear")) return;
  if(!ThePlaneNumber || ThePlaneNumber>fSession->GetTracker()->GetPlanesN()) {Warning("MimosaVertexFinder()","Please set a plane number!"); return; }
  if(!RunNumber) Error("MimosaVertexFinder","RunNumber not set! Please run InitSession  first");

  InitMimosaType();
  Info("MimosaVertexFinder","Mimosa type %d", MimosaType);

  //----------------------------------------------------------------------------------
  // -- Open the input file containing the TTree
  //----------------------------------------------------------------------------------

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0) {
    Error("MimosaVertexFinder"," The input file contains an incorrect number of events %d!",Nevt);
  }
  else {
    Info("MimosaVertexFinder","There is %d events in the input file.",Nevt);
  }

  //----------------------------------------------------------------------------------
  // -- Chose the sub-matrix.
  //----------------------------------------------------------------------------------

  GetParameters();
  ThesubmatrixNumber = submatrix;

  //----------------------------------------------------------------------------------
  // -- CUTS
  //----------------------------------------------------------------------------------

  // on tracks
  //TrackToHitDistanceLimit =  TrackHitDist ; // Distance between the track and the hit
  //TrackChi2Limit = fSession->GetSetup()->GetAnalysisPar().TrackChi2Limit;


  // on geometry
  Thegeomatrix = GeoMatrix; // define the area of interest in the DUT
  geomUmin = fSession->GetSetup()->GetAnalysisPar().Umin[ThesubmatrixNumber][Thegeomatrix];
  geomUmax = fSession->GetSetup()->GetAnalysisPar().Umax[ThesubmatrixNumber][Thegeomatrix];
  geomVmin = fSession->GetSetup()->GetAnalysisPar().Vmin[ThesubmatrixNumber][Thegeomatrix];
  geomVmax = fSession->GetSetup()->GetAnalysisPar().Vmax[ThesubmatrixNumber][Thegeomatrix];

  cout << "----------- CUTS:" << endl;
  cout << "  # hits (min, max): " << CUT_MinNbOfHits << " - " << CUT_MaxNbOfHits << endl;
  cout << "  max # pixels in a hit: " << MaxNofPixelsInCluster << endl;
  cout << "  min S/N ratio: for seed = " << CUT_S2N_seed << ", for neighbours (without seed) = " << CUT_S2N_neighbour << endl;
  cout << "  geomatrix is " << Thegeomatrix << ", limits in U: " << geomUmin << ", " << geomUmax << ", limits in V: " << geomVmin << ", " << geomVmax << endl;

  Info("MimosaVertexFinder","Cuts initialized");

  //----------------------------------------------------------------------------------
  // -- Analysis variables
  //----------------------------------------------------------------------------------

  //Int_t NgoodEvents = 0;
  //Int_t Ngoodhits = 0; // total # good hits
  //Int_t NgoodhitsinGEOM = 0; // total # good hits in geom
  //Int_t Ngoodhitsinevent; // # good hits in the GEOM area in the event
  //Int_t NgoodhitsinGEOMinevent; // # good hits in the GEOM area in the event

  Info("MimosaVertexFinder","Analysis variables initialized");


  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();

  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;//50000;//40000; //was 2000, modifyed by JB Sept 2007 to match number of events for initialization (ped and noise)
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  Info("MimosaVertexFinder","\nReady to loop over %d events\n", MaxEvent);
  const Int_t NofCycle=1000; //was 100


  Int_t counter = 0;

  Int_t DPrecAlignMethod = fSession->GetSetup()->GetTrackerPar().DPrecAlignMethod;   // LC 2015/01/31
  DPrecAlign *anAlign = new DPrecAlign(DPrecAlignMethod);

  anAlign->SetRotations( 0., 0., 0.);
  anAlign->SetTranslation( 0., 0., 0.);
  anAlign->CalculatePlane();
  anAlign->PrintAll();

  // **********************************************************************************
  // ********* MAIN LOOP
  // **********************************************************************************


  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

      if(MimoDebug) Info("MimosaVertexFinder","Reading event %d",ievt);
      if(ievt/NofCycle*NofCycle == ievt || ievt<10){
        cout << "MimosaVertexFinder Event " << ievt <<endl;
      }


    //=========================
    t->GetEvent(ievt);
    //=========================
    //DEventHeader& CurrentEventHeader=Evt->GetHeader();

/*
    DTracker *tTracker = fSession->GetTracker();
    DTrack   *aTrack;

    for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks
      aTrack = tTracker->GetTrack(iTrack);

      std::cout<<"aTrack->GetVertexX() = "<<aTrack->GetVertexX()<<std::endl;

      hVertexPosX->Fill( aTrack->GetVertexX() );
      hVertexPosY->Fill( aTrack->GetVertexY() );
      hVertexPosXY->Fill( aTrack->GetVertexX(), aTrack->GetVertexY() );
      hVertexTrackDistance->Fill( sqrt(aTrack->GetVertexX()*aTrack->GetVertexX() + aTrack->GetVertexY()*aTrack->GetVertexY() + aTrack->GetVertexZ()*aTrack->GetVertexZ() ) );
   }
*/

    TClonesArray *Trpl = Evt->GetTransparentPlanes() ;
    Int_t NbOfTrpl = Trpl->GetLast()+1 ; // total # tracks over all planes

    if( NbOfTrpl==0 ) continue; // if no track continue :)

    // Find the tracks in this plane and count them with okT
    DTransparentPlane *aTrpl = NULL;

    Int_t j = 0; // init
    Int_t okT = 0;

    while( j<NbOfTrpl ) {
      aTrpl=(DTransparentPlane*) Trpl->At(j) ;
      if (aTrpl->Tpk == ThePlaneNumber) okT++;
      j++;
    }

    std::cout<<" Number of tracks = "<<okT<<std::endl;

    hVertexTrackChi2->Fill(aTrpl->Tchi2);

    if (aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 <= chi2Cut)
    {

      std::cout<<"Track Chi2 = "<<aTrpl->Tchi2<<std::endl;
/*
      //TrackParameters_compute :
      tx  = aTrpl->Tx ;
      ty  = aTrpl->Ty ;
      tz  = aTrpl->Tz ;
      tdx = aTrpl->Tdx ;
      tdy = aTrpl->Tdy ;

      std::cout<<"Tu = "<<aTrpl->Tu<<"  Tv = "<<aTrpl->Tv<<std::endl;

      DataPoints aPosition(0, 0, tx, ty, tz, tdx, tdy);

      anAlign->CalculateIntersection(&aPosition);

      DR3 pos = anAlign->GetTrackPosition();
      cout << "x = " << pos(0) << ", y = " << pos(1) << endl;
*/
      ++counter;
      std::cout<<"Counter = "<<counter<<std::endl;
/*
      hVertexPosX->Fill( pos(0) );
      hVertexPosY->Fill( pos(1) );
      hVertexPosXY->Fill( pos(0), pos(1) );
      hVertexTrackDistance->Fill( sqrt(pos(0)*pos(0) + pos(1)*pos(1)) );
*/
      hVertexPosX->Fill( aTrpl->TvertexU );
      hVertexPosY->Fill( aTrpl->TvertexV );
      hVertexPosZ->Fill( aTrpl->TvertexW );
      hVertexPosXY->Fill( aTrpl->TvertexU, aTrpl->TvertexV );
      hVertexTrackDistance->Fill( sqrt(aTrpl->TvertexU*aTrpl->TvertexU + aTrpl->TvertexV*aTrpl->TvertexV + aTrpl->TvertexW*aTrpl->TvertexW ) );
    }

  } // end of Main loop over event

  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  cVertexFinder = new TCanvas("cVertexFinder","MimosaVertexFinder",800,600);
  cVertexFinder->cd();

  cVertexFinder->Divide(2, 3);

  //TPad* cVertexFinder_1=(TPad*)(cVertexFinder->GetPrimitive("cVertexFinder_1"));
  cVertexFinder->cd(1);
  AutoZoom(hVertexPosX)->Draw();
  //hVertexPosX->SaveAs("hVertexPosX.root");

  //TPad* cVertexFinder_2=(TPad*)(cVertexFinder->GetPrimitive("cVertexFinder_2"));
  cVertexFinder->cd(2);
  AutoZoom(hVertexPosY)->Draw();

  cVertexFinder->cd(3);
  AutoZoom(hVertexPosZ)->Draw();
  //hVertexPosZ->SaveAs("hVertexPosZ.root");
  //TPad* cVertexFinder_3=(TPad*)(cVertexFinder->GetPrimitive("cVertexFinder_3"));
  cVertexFinder->cd(4);
  AutoZoom(hVertexPosXY)->Draw();

  cVertexFinder->cd(5);
  AutoZoom(hVertexTrackDistance)->Draw();

  TF1* fitFunction = new TF1("fitFunction","[0]+[1]*x*TMath::Exp([2]*x)",0,10);
  fitFunction->SetParameters(0., 1., -0.1);
  hVertexTrackDistance->Fit( "fitFunction" );
  fitFunction->Draw("SAME");

  cVertexFinder->cd(6);
  AutoZoom(hVertexTrackChi2)->Draw();

}

//______________________________________________________________________________
//
void MimosaAnalysis::MimosaProLadder(Int_t MaxEvt, Int_t TrackHitDist, Float_t S2N_seed, Float_t S2N_neighbour  , Int_t submatrix , Int_t GeoMatrix ,Option_t*  SaveAlign,  Int_t UseHitMap, Int_t WriteMissedHits, Int_t WriteGoodHits)
{

  // -- Main processing macro for MIMOSA root DSF file with a ladder as DUT
  //
  // Main analysis method: loops over all events found in a runXXX_0X.root file
  // Fills all histograms
  //
  // Copy of MimosaPro on 2014/02/10, JB


  if(!CheckIfDone("init,clear")) return;
  if(!TheLadderNumber || TheLadderNumber>fSession->GetTracker()->GetNumberOfLadders()) {Warning("MimosaPro()","Please set a ladder number!"); return; }
  if(!RunNumber) Error("MimosaPro","RunNumber not set! Please run InitSession  first");

  InitMimosaType();
  Info("MimosaPro","Mimosa type %d", MimosaType);

  GetAnalysisGoal();
  fIfReferenceTrack = kTRUE;

  if (!theCorFile) InitCorPar( RunNumber, TheLadderNumber, "ladder");
  cout << "InitCorPar OK" << endl ;
  gSystem->ChangeDirectory(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory


  //----ADC
  // ADC Readout !=0 if Readout is >=200
  // Modified JB 2009/07/20
  //Int_t ADCReadout=0;
  //if( (fSession->GetSetup()->GetPlanePar(ThePlaneNumber)).Readout > 199 ) {
  //  ADCReadout = (fSession->GetSetup()->GetPlanePar(ThePlaneNumber)).Readout%200;
  //}
  //----ADC

  //----------------------------------------------------------------------------------
  // -- Open the input file containing the TTree
  //----------------------------------------------------------------------------------

  Nevt = OpenInputFile(); // total number of events in the tree

  //----------------------------------------------------------------------------------
  // -- Chose the plane(matrix) and possibly the sub-matrix.
  //----------------------------------------------------------------------------------

  ThesubmatrixNumber = submatrix;

  //----------------------------------------------------------------------------------
  // -- Init analysis variables
  //----------------------------------------------------------------------------------

  // for counting hits and tracks
  Int_t   ngoodhit    = 0; // # good hits in the DUT
  Int_t   NRecHit     = 0; // total # hits in the DUT
  Int_t   nmiss       = 0; // # good tracks in the DUT without a matched hit, JB 2009/09/08
  Int_t   nmissh      = 0; // # good tracks in the DUT with no good hit in the DUT, JB 2009/09/08
  Int_t   nmissthdist = 0; // # good tracks in the DUT with hit outside the distance limit

  Int_t Nhitinevent; // JB 2009/09/08
  Int_t Ngoodhitinevent; // # good hits in the event
  Int_t Ngoodtrackinevent; // # good tracks in the telescope in the event
  Int_t NgoodtrackinDUTinevent; // # good tracks in the telescope and in the DUT in the event

  // Counter on event status
  Int_t StatusEvent[10];
  for ( Int_t i=0 ; i<10 ; i++ ) {
    StatusEvent[i]=0;
  }


  //  To calculate parameters to prove the posibility of selection improvement of clusters
  const Short_t NCut      =  500   ;
  Float_t VCutMin         = -10.0 ;
  Float_t VCutMax         =  50.0 ;
  Int_t NKeepHit[NCut]   ;
  Int_t NKeepHitOk[NCut] ;
  Float_t VCut[NCut]     ;
  //Float_t SignalThreshold;

  //Int_t *IndexOfGoodHitsInEvent = new Int_t[CUT_MaxNbOfHits];
  Int_t IndexOfGoodHitsInEvent[8000]; // enlarge to 4000, JB 2010/08/27
  // Bool_t FillHitSeparationRootFile =  kFALSE ;//kTRUE;// kFALSE ;


  //----------------------------------------------------------------------------------
  // CUTS
  //----------------------------------------------------------------------------------
  // only sets here the values specified as arguments to the function,
  // the rest of them will be loaded with GetParameters).
  // JB 2013/08/21

  // on hits
  CUT_S2N_seed =  S2N_seed ; // S/N(seed)
  CUT_S2N_neighbour =  S2N_neighbour ; // S/N(neighbours)

  // on tracks
  Thegeomatrix = GeoMatrix; // define the area of interest in the DUT
  TrackToHitDistanceLimit =  TrackHitDist ; // Distance between the track and the hit
  cout << "--- CUTS specified:" << endl;
  cout << "  min S/N ratio: for seed = " << CUT_S2N_seed << ", for neighbours (without seed) = " << CUT_S2N_neighbour << endl;
  cout << "  max distance track to hit " << TrackToHitDistanceLimit << endl;

  // ?
  //SignalThreshold=0.;
  for(Short_t ic=0 ; ic<NCut ; ic++){
    Float_t tmp = VCutMin + (VCutMax-VCutMin)/(NCut-1)*ic ;
    VCut[ic]       = tmp*calibration ;
    NKeepHit[ic]   = 0   ;
    NKeepHitOk[ic] = 0   ;
  }

  //---ab  Baseline cuts and previous bad events.
  // apply the baseline cut to avoid possible bad reset effects.

  //Bool_t usebaselinecut = kTRUE;// kFALSE ;
  Bool_t usebaselinecut = kFALSE ;
  //---ab
  Int_t Previousbadevent = 0 ;
  Int_t Previousbadevent2 = 0 ;
  Float_t Baselinecut = 1100.0;

  //----------------------------------------------------------------------------------
  // -- get parameters
  //----------------------------------------------------------------------------------

  GetParameters(); // JB 2010/07/23

  Info("MimosaPro","Analysis variables initialized");

  //----------------------------------------------------------------------------------
  // -- prepare displays
  //----------------------------------------------------------------------------------

  PrepareOnlineDisplay();

  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------

  BookingHistograms();

  //----------------------------------------------------------------------------------
  // -- get the parametrization of the Eta function
  //----------------------------------------------------------------------------------

  GetMiEta();

  //----------------------------------------------------------------------------------
  // -- prepare alignment parameters
  //----------------------------------------------------------------------------------

  GetAlignment();

  //----------------------------------------------------------------------------------
  //--- Store Good and/or Bad events in ASCII FILEs
  //----------------------------------------------------------------------------------
  // Updated from AB by JB, 2009/09/08

  Int_t TheWriteGoodHits = WriteGoodHits; // 0 = no; 1 = yes.
  Char_t Header_EventList[400];
  sprintf(Header_EventList,"%s/%d_pl%i_Sub%i_Seed%i_Neig%i_geomat%i_T2h%i_chi2%i_HitMap%i/good%d_pl%i_Sub%i_Seed%i_Neig%i_geomat%i_T2h%i_chi2%i,HitMap%i.dat", CreateGlobalResultDir(), RunNumber, ThePlaneNumber, ThesubmatrixNumber, (Int_t)CUT_S2N_seed, (Int_t)CUT_S2N_neighbour, Thegeomatrix, (Int_t)TrackToHitDistanceLimit, (Int_t)TrackChi2Limit, TheUsePixelMap, RunNumber, ThePlaneNumber, ThesubmatrixNumber, (Int_t)CUT_S2N_seed, (Int_t)CUT_S2N_neighbour, Thegeomatrix, (Int_t)TrackToHitDistanceLimit, (Int_t)TrackChi2Limit, TheUsePixelMap);
  if(TheWriteGoodHits > 0) {
    outFileGoodEvt.open(Header_EventList,ios::app);
    Info("MimosaPro","Good hits information will be logged in file %s\n", Header_EventList);
  }

  Int_t TheWriteMissedHits = WriteMissedHits; // 0 = no; 1 = yes; 2 = yes but short format.
  Char_t badEvtFileName[400];
  sprintf( badEvtFileName,"%s/bad%d_pl%i_Sub%i_Seed%i_Neig%i_geomat%i_T2h%i_chi2%i_HitMap%i.dat", CreateGlobalResultDir(), RunNumber, ThePlaneNumber, ThesubmatrixNumber, (Int_t)CUT_S2N_seed, (Int_t)CUT_S2N_neighbour, Thegeomatrix, (Int_t)TrackToHitDistanceLimit, (Int_t)TrackChi2Limit, TheUsePixelMap); // dir name corrected, JB 2011/07/18
  if(TheWriteMissedHits > 0) {
    outFileBadEvt.open(badEvtFileName,ios::app);
    Info("MimosaPro","Missed hits information will be logged in file %s, (open=%d)\n", badEvtFileName, outFileBadEvt.fail());
  }

  // store the cluster info in separate ascii file:
  Char_t Headerclusterdat[100];
  sprintf(Headerclusterdat,"HitSeparation/rawhit/newcluster_%d_%d.dat",RunNumber,ThePlaneNumber);
  ofstream outFile(Headerclusterdat);
  const Int_t NofCycle=5000; //was 100


  //---------------------------------------------------------------
  //-- Prepare the event loop
  //---------------------------------------------------------------

  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;//50000;//40000; //was 2000, modifyed by JB Sept 2007 to match number of events for initialization (ped and noise)
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;


  // ************************************
  // Call init part of the analysis here

  HotPixel_init( UseHitMap); // JB 2011/11/23
  Efficiency_init();     // JB 2011/11/04
  ClusterShape_init();   // JB 2010/04/13
  ClusterCharges_init(); // JB 2010/06/03
  ClusterPosition_init();

  // ************************************


  // a bit of cleaning
  MainCanvas->Clear();

  Info("MimosaPro","\nReady to loop over %d events (over %d in the tree)\n", MaxEvent, (int)t->GetEntries());

  // **********************************************************************************
  // ********* MAIN LOOP ***************
  // **********************************************************************************

  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaPro","Reading event %d",ievt);



    // Event wise parameters

    Short_t NhitInPlane = 0;
    //Short_t NtrackInPlane = 0;

    //-- Initilisations

    Nhitinevent=0;
    Ngoodhitinevent=0;
    Ngoodtrackinevent=0;
    NgoodtrackinDUTinevent=0;
    for ( Int_t i=0 ; i<10 ; i++ ) { StatusEvent[i]=0; }

    //-- Update display

    if (ievt == MinEvent) { // JB, 2008/08/09
      selection->Draw();
      for( Int_t il=0; il<8; il++) {
        textLabel[il]->Draw();
      }
    }

    if(ievt/NofCycle*NofCycle == ievt || ievt<10){
      if( selection->GetMaximum()>15*selection->GetBinContent(6)) MainCanvas->SetLogy(); // JB 2010/04/28
      MainCanvas->Modified();
      MainCanvas->Update();
      cout << "MimosaPro Event " << ievt <<endl;
      if (NtrkInMimo>100){
        cout<<"  temporary efficiency = "<<NofClMatchTrack<<" / "<<NtrkInMimo<<" = "<<1.*NofClMatchTrack/NtrkInMimo<<endl;
      }

    }

    //-- Update counter for efficiency

    StatusEvent[1]++;
    if(ievt / NeventRangeForEfficiency * NeventRangeForEfficiency == ievt ){ ievt_array ++; }

    //=========================
    if(MimoDebug>1) cout << " getting the event" << endl;
    t->GetEvent(ievt);
    //=========================

    DEventHeader& CurrentEventHeader=Evt->GetHeader();

    //---Store events in ascii file:
    /* DEventHeader& CurrentEventHeader=Evt->GetHeader();
     if(TheWriteGoodHits>1){
     // trigger number event(evt), evt-ievt ,mimosa loop event (ievt):
     outFileGoodEvt<<"####### evt RealEventNumber, evt-ievt , ievt"<<CurrentEventHeader.GetEventNumber()<<" "
     <<CurrentEventHeader.GetEventNumber()-ievt <<" "
     <<ievt<<endl;
     }
     */
    //    cout<<"####### evt RealEventNumber, evt-ievt , ievt"<<CurrentEventHeader.GetEventNumber()<<" "
    //	<<CurrentEventHeader.GetEventNumber()-ievt <<" "<<ievt<<endl;

    if(MimoDebug>1) cout << " getting the planes, tracks " << Evt->fT1PlanesN << " and hits" << endl;
    TClonesArray *Trpl   = Evt->GetTransparentPlanes() ; //tracks
    TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
    TClonesArray *Planes = Evt->GetAuthenticPlanes()   ; //planes
    Int_t NbOfTrpl   = Trpl->GetLast()+1   ; // total # tracks over all planes
    Int_t NbOfHits   = Hits->GetLast()+1   ; // total # hits over all planes

    // check to avoid crash due to empty event, added by JB, Sept 2008,
    // cut NbOfHits==0 removed since there may be no hit at all but the event should still be taken into account for efficiency, JB 2012/06/08
    if( NbOfTrpl==0 ) {
      if (MimoDebug > 0) Info("MimosaPro","Empty event %d: #tracks %d\n", ievt, NbOfTrpl);
      continue;
    }

    //Find the plane :
    Int_t okP = 0, okT = 0 ; // two ok added instead of one, JB Sept 2007
    DAuthenticPlane *ThePlaneStudied = 0; // init
    Int_t j=0; // init
    while(j<Planes->GetLast()+1 && okP==0) {
      ThePlaneStudied=(DAuthenticPlane*) Planes->At(j) ;
      //printf("testing %d against %d\n", ThePlaneStudied->Ppk, ThePlaneNumber);
      if (ThePlaneStudied->Ppk == ThePlaneNumber) {
        okP=1 ;
        NhitInPlane = ThePlaneStudied->PhN ;
        //NtrackInPlane = ThePlaneStudied->PtN ;
      }
      j++;
    }


    // Find the tracks in this plane and count them with okT
    // JB 2009/07/20
    DTransparentPlane *aTrpl;
    j = 0; // init
    while( j<NbOfTrpl ) {
      aTrpl=(DTransparentPlane*) Trpl->At(j) ;
      if (aTrpl->Tpk == ThePlaneNumber) {
        okT++ ;
      }
      j++;
    }


    // ****************** Filling histos
    selection->Fill(1.);  // all events

    //-- Reference plane study
    for ( Int_t iipl=0 ; iipl<TMath::Min(NbOfTrpl, 10) ; iipl++ ) {
      aTrpl = (DTransparentPlane*) Trpl->At(iipl) ;
      // select reference planes
      if( aTrpl->Tpk != ThePlaneNumber ) { // if this is not the DUT
        if (MimoDebug>3) Info("MimosaPro","Ref plane %d, track pos (%.0f, %.0f), dist.2hit (%.1f, %.1f), chi2 %f, slope (%.2f, %.2f)", aTrpl->Tpk, aTrpl->Tu, aTrpl->Tv, aTrpl->Tud, aTrpl->Tvd, aTrpl->Tchi2, aTrpl->TDOX, aTrpl->TDOY);
        hRef_Tud_vs_Tv[iipl]->Fill(aTrpl->Tu,aTrpl->Tud);
        hRef_Tud_vs_Tu[iipl]->Fill(aTrpl->Tv,aTrpl->Tud);
        hRef_Tud_vs_TDOX[iipl]->Fill(aTrpl->TDOX,aTrpl->Tud);
        hRef_Tud_vs_TDOY[iipl]->Fill(aTrpl->TDOY,aTrpl->Tud);
        hRef_Tud_vs_Chi2[iipl]->Fill(aTrpl->Tchi2,aTrpl->Tud);
        hRef_Tud_vs_Tu1[iipl]->Fill(aTrpl->Tu1,aTrpl->Tud);
        hRef_Tud_vs_Tk1[iipl]->Fill(aTrpl->Tk1,aTrpl->Tud);
      } // end if this is not the DUT
    }
    // ******************


    //----ab    CUT ON THE BASELINE TO AVOID BAD RESET EFFECTS:
    if( ((ThePlaneStudied->PFrfr1)<Baselinecut) && (usebaselinecut==kTRUE) ){okT=0; okP=0;}

    //---------------------------------------------------------------
    // if Both TransparentPlane and AuthenticPlane exist : (JB, Sept 2008)
    // and no baseline cut
    // and number of hits and tracks are within limits
    if ( okP && okT && NbOfHits<CUT_MaxNbOfHits && NbOfHits>=CUT_MinNbOfHits && NbOfTrpl>0) { // Plane exists with tracks and hits
      StatusEvent[2]++;

      // ****************** Filling histos
      selection->Fill(2.); // event with track(s) and hits in the plane
      hnhit->Fill(NhitInPlane);
      hnhitievt->Fill(ievt,NhitInPlane);
      hraw1Noise->Fill(float(ThePlaneStudied->PFn));
      hraw1Pedestal->Fill(float(ThePlaneStudied->PFp));
      hraw1CDS->Fill(float(ThePlaneStudied->PFr));
      hraw1Signal->Fill(float(ThePlaneStudied->PFq));
      hraw1Noise_time->Fill(ievt,float(ThePlaneStudied->PFn));
      hraw1Pedestal_time->Fill(ievt,float(ThePlaneStudied->PFp));
      hraw1CDS_time->Fill(ievt,float(ThePlaneStudied->PFr));
      hraw1Signal_time->Fill(ievt,float(ThePlaneStudied->PFq));
      // ******************

      //---------------------------------------------------------------
      // Loop over hits BUT keep only the ones in the right plane
      for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

        DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);
        if( IsPlaneInLadder( ahit->Hpk, TheLadderNumber ) ) continue; // select only hits in DUT
        StatusEvent[6]++;
        Nhitinevent++;
        NRecHit++;

        Int_t Hotpixel = HotPixel_test( ahit->Hsk); // JB 2011/11/23

        // Now compute variables usefull for cuts

        NofPixelsInCluster = (ahit->HNNS < MaxNofPixelsInCluster)?ahit->HNNS:MaxNofPixelsInCluster;
        if (MimoDebug>2) Info("MimosaPro"," NofPixelsInCluster is set to %d (in hit %d, max %d)\n", NofPixelsInCluster, ahit->HNNS, MaxNofPixelsInCluster);

        // signal over noise ratio of the seed pixel
        Float_t snSeed;
        if( ahit->Hsn != 0.0 ) {
          snSeed=ahit->Hq0/ahit->Hn0/NoiseScope;
        }
        else { // for sparsified readout, JB 2009/05/19
          snSeed=ahit->Hq0;
        }
        if(MimoDebug>2) printf("MimosaPro: snSeed computation with Hsn=%f, Hq0=%f, Hn0=%f, NoiseScope=%f => snSeed=%.1f\n", ahit->Hsn, ahit->Hq0, ahit->Hn0, NoiseScope, snSeed);


        // charge and S/N in the second highest pixel of the hit
        // among the 4 pixels neighbouring the seed (cross)
        Float_t q2nd = 0.;
        Float_t sn2nd = 0.;
        for(Int_t i=0; i < NofPixelsInCluster; i++){
          //cout << "i=" << i << " index=" << ahit->HkM[i]- ahit->HkM[0] << " q=" << ahit->HqM[i] << endl;
          if( (abs(ahit->HkM[i]- ahit->HkM[0])==1 || abs(ahit->HkM[i]- ahit->HkM[0])== NofPixelInRaw)
             && ahit->HqM[i]>q2nd	){
            q2nd = ahit->HqM[i];
            if( ahit->HnM[i] != 0.0 ) {
              sn2nd = q2nd/ahit->HnM[i]/NoiseScope;
            }
            else { // for sparsified readout, JB 2009/05/19
              sn2nd = q2nd;
            }
          }
        }

        // charge of the cluster without the seed
        ChargeAroundSeed = (ahit->Hqc - ahit->Hq0)*calibration;

        // what the hell is this ?? JB 2010/06/03
        for(Short_t ic=0 ; ic<NCut ; ic++){
          if (ChargeAroundSeed>VCut[ic]){
            NKeepHit[ic]++ ;
          }
        }

        if(MimoDebug>2) {
          printf("MimosaPro: Hit %d, qSeed=%.1f, qAroundSeed=%.1f, hot=%d, snSeed=%.1f, sn2nd=%.1f\n", iHit, ahit->Hq0, ChargeAroundSeed, Hotpixel, snSeed, sn2nd);
        }

        // ****************** Filling histos
        selection->Fill(6.);  // all hits in MIMOSA

        h2dallhits->Fill( ahit->Hsu, ahit->Hsv);
        hallSNneighbour->Fill(ahit->HSNneighbour);
        hAllS2N->Fill(snSeed);
        hallSN_vs_SNN->Fill(ahit->HSNneighbour,snSeed);
        hQseedAll->Fill(ahit->HqM[0]*calibration);
        hseedQvsS2NAll->Fill(snSeed,ahit->HqM[0]*calibration);
        hS2N2nd->Fill(sn2nd);
        if(snSeed>sn2nd) { hS2N2All->Fill(snSeed,sn2nd); }
        else { hS2N2All->Fill(sn2nd,snSeed); }
        etal[0]->Fill(ChargeAroundSeed) ;
        if((ahit->Hq0)!=0.0){
          etal1[0]->Fill((ahit->Hqc/ahit->Hq0) - 1.0) ;
        }
        else{
	  if(ahit->HNNS > 0) Warning("MimosaPro","evt %d, segment WARNING ahit->Hq0 = 0", ievt);
	}

        // ******************


        //--------------------------------------------------------------
        // If hit passes the selection cut
        if(
           ( Hotpixel == 0)
           //&& ( ahit->HqM[0]*calibration>0. ) // why is it usefull ? JB 2013/11/08
           && ( ahit->HsnPulse / ahit->Hsn >= CUT_S2N_seed )
           && ( ahit->HSNneighbour >= CUT_S2N_neighbour) //YV 27/1109 you add these 2 lines if you want S/N cuts
           && ( ChargeAroundSeed >= CUT_MinQ_neighbour )  // JB 2013/11/08
           && ( ahit->Hq0 >= CUT_Q_seed ) // JB 2013/11/08
           && ( ahit->Hqc >= CUT_Q_cluster ) // JB 2014/01/21
           && ( MinNofPixelsInCluster <= ahit->HNNS && ahit->HNNS <= MaxNofPixelsInCluster ) // JB 2013/09/11
           && ( CUT_MinSeedIndex==CUT_MaxSeedIndex ||
               (CUT_MinSeedIndex<=ahit->Hsk && ahit->Hsk<=CUT_MaxSeedIndex) ) // allows to select an index range for the seed pixel (ineffective if minIndex==maxIndex), JB 2013/08/21
           && ( CUT_MinSeedCol==CUT_MaxSeedCol ||
               (CUT_MinSeedCol<=(ahit->Hsk%NofPixelInRaw) && (ahit->Hsk%NofPixelInRaw)<=CUT_MaxSeedCol) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
           && ( CUT_MinSeedRow==CUT_MaxSeedRow ||
               (CUT_MinSeedRow<=(ahit->Hsk/NofPixelInRaw) && (ahit->Hsk/NofPixelInRaw)<=CUT_MaxSeedRow) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
           //&& 256<=(ahit->Hsk%NofPixelInColumn) && (ahit->Hsk%322)<=319 // S1:0-95, S2:96-191, S3:192-255, S4:256-319, JB 2013/08/22
           ){ // if hit passes cuts
          StatusEvent[7]++;
          ngoodhit++;
          IndexOfGoodHitsInEvent[Ngoodhitinevent] = iHit;
          Ngoodhitinevent++;
          if(MimoDebug>1) cout << " found good hit " << Ngoodhitinevent-1 << " with index " << IndexOfGoodHitsInEvent[Ngoodhitinevent-1] << " pos " << ahit->Hu << ";" << ahit->Hv << " and dist to track " << ahit->Hu-ahit->Htu << ";" << ahit->Hv-ahit->Htv << endl;

          // ****************** Filling histos
          selection->Fill(7.); // Good hits in MIMOSA

          hnGOODhit->Fill(Ngoodhitinevent);
          hnahitievt->Fill( ievt, Ngoodhitinevent);
          GoodHit_Fill( ahit); // JB 2010/10/06
          hgoodSN_vs_SNN->Fill(ahit->HSNneighbour,snSeed);
          BinarySpecific_HitsOnly_fill( ahit);  // JB 2010/06/03
          if( ahit->Htk >= 0) { // if a track was associated in Tree
            Float_t adist = sqrt((ahit->Hsu-ahit->Htu)*(ahit->Hsu-ahit->Htu)+(ahit->Hsv-ahit->Htv)*(ahit->Hsv-ahit->Htv)) ;
            duvall->Fill( adist); //For all the clusters with Hqc-Hq0>Cut
            hAllHuvsAllTu->Fill( ahit->Htu, ahit->Hsu);
            hAllHuvsAllTv->Fill( ahit->Htv, ahit->Hsu);
            hAllHvvsAllTu->Fill( ahit->Htu, ahit->Hsv);
            hAllHvvsAllTv->Fill( ahit->Htv, ahit->Hsv);
            //	    if(fabs( sqrt( (ahit->Htu-ahit->Hu)*(ahit->Htu-ahit->Hu) + (ahit->Htv-ahit->Hv)*(ahit->Htv-ahit->Hv) ) ) > TrackToHitDistanceLimit){
            //	    }
          } // end if a track was associated
          // ******************

        } // end if hit passes cuts
        //--------------------------------------------------------------

        else { // if hit does not pass cuts
          if(MimoDebug>1) Info("MimosaPro","Hit not passing selection cuts, ChargeAroundSeed=%f <> %f, seedCharge=%.1f <> %.0f, hotPixel %d", ChargeAroundSeed, CUT_MaxQ_neighbour, ahit->HqM[0]*calibration, 0., Hotpixel);
        }

      } // end loop on hits
      //--------------------------------------------------------------

      if(MimoDebug) Info("MimosaPro","End loop on hits, found %d good hits in event %d", Ngoodhitinevent, ievt);
      const Int_t NofCycle=1000; //was 100



      //---------------------------------------------------------------
      // FIRST Loop on tracks to count how many are in the EXCLUSION GEOMATRIX
      Int_t Ngoodtrackgeominevent = 0;
      for (Int_t iTrk=0 ; iTrk<NbOfTrpl ; iTrk++){ // loop on tracks

        aTrpl = (DTransparentPlane*) Trpl->At(iTrk);
        if( IsPlaneInLadder( aTrpl->Tpk, TheLadderNumber ) ) continue; // select only tracks in DUT
        TrackParameters_compute(aTrpl, alignement); // compute tu,tv

        if( TrackInMimo( GeoMatrixForTrackCut,tu,tv,ThesubmatrixNumber) /* && aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 < TrackChi2Limit */) {
          Ngoodtrackgeominevent++;
        }

      } // end loop on tracks
      hNGoodGeomTracksPerEvent->Fill( Ngoodtrackgeominevent);
      if( MimoDebug>1) Info("MimosaPro"," Number of tracks in EXCLUSION GEOMATRIX %d is %d.", GeoMatrixForTrackCut , Ngoodtrackgeominevent);



      //---------------------------------------------------------------
      // Loop on tracks BUT keep only the ones in the right plane
      for (Int_t iTrk=0 ; iTrk<NbOfTrpl ; iTrk++){ // loop on tracks

        aTrpl = (DTransparentPlane*) Trpl->At(iTrk);
        if( aTrpl->Tpk != ThePlaneNumber ) continue; // select only tracks in DUT

        StatusEvent[3]++;


        // ****************** Filling histos
        selection->Fill(3.); // all tracks in plane

        TrackParameters_allFill( aTrpl, ievt);
        // ******************

        if( MimoDebug>1) Info("MimosaPro"," Track %d has chi2=%f <> %f ?", iTrk, aTrpl->Tchi2, TrackChi2Limit);

        //---------------------------------------------------------------
        // if chi2 of selected track is OK
        if ( aTrpl->Tchi2 >= 0. && aTrpl->Tchi2 < TrackChi2Limit // JB, chi2=0. is not a real pb
            && aTrpl->TtHn>=MinHitsPerTrack
            //&& aTrpl->Tdx<0.002 && aTrpl->Tdy<0.002 // insure track is flat, NOT MANDATORY, JB sept 2007
            && (MaxNbOfTracksInGeom<0 || Ngoodtrackgeominevent<=MaxNbOfTracksInGeom) /* added for MIMOSA32 case, JB 2012/08/20*/
            ){ // if good track
          StatusEvent[4]++;
          Ngoodtrackinevent++;

          TrackParameters_compute(aTrpl, alignement);

          /*******************/
          selection->Fill(4.);   // Good quality track

          hNTracksPerEvent->Fill( Ngoodtrackinevent);
          hNTracksPerEventievt->Fill( ievt, Ngoodtrackinevent);

          TrackParameters_goodFill( aTrpl, ievt);

          /*******************/

          Bool_t TrkCrossMimo = TrackInMimo( Thegeomatrix, tu ,tv, ThesubmatrixNumber);

          if( MimoDebug) Info("MimosaPro"," Is track %d in MIMOSA ? %d", iTrk, TrkCrossMimo);

          //---------------------------------------------------------------
          // if track is in the good geometry range
          if( TrkCrossMimo == kTRUE ){ // if the track is in MIMOSA
            StatusEvent[5]++;
            NgoodtrackinDUTinevent++;
            NtrkInMimo++ ;

            Previousbadevent2 = Previousbadevent;
            Previousbadevent = ievt;
            temp_NtrkInMimo[ievt_array]++;


            //-- Find the nearest good hit to this track

            TrackToHitDistance = 10000000.0 ;
            DAuthenticHit *thehit = 0; //This will be a real hit
            DAuthenticHit *ahit = 0;
            Float_t adist;

            if( Ngoodhitinevent<=0 ) { // no hit at all, JB 2009/09/08
              nmissh++;
            }

            for(Int_t iHit=0; iHit<Ngoodhitinevent; iHit++) { // loop over good hits
              ahit = (DAuthenticHit*)Hits->At( IndexOfGoodHitsInEvent[iHit] );
              if(MimoDebug>1) cout << " Good hit " << iHit << " selected at index " << IndexOfGoodHitsInEvent[iHit] << " with position " << ahit->Hu << ";" << ahit->Hv << endl;
              adist = sqrt( (ahit->Hu-tu)*(ahit->Hu-tu) + (ahit->Hv-tv)*(ahit->Hv-tv) );
              if (adist<TrackToHitDistance) {
                thehit=ahit;
                TrackToHitDistance=adist;
              }
            } // end loop over good hits


            //-- Check the nearest hit is within the requested distance

            Bool_t aHitIsFound = kFALSE ;
            if( fabs(TrackToHitDistance) < TrackToHitDistanceLimit
               //&& fabs(ahit->Hv-tv)<200. // specific case, JB 2012/11/20
               //&& fabs(ahit->Hu-tu)<1000. // specific case, JB 2012/11/20
               ){
              aHitIsFound = kTRUE ;
            }
            else if( thehit ) { // hit there but too far away, JB 2009/09/08
              nmissthdist++;
            }

            // ****************** Filling histos
            selection->Fill(5.); // Good track in MIMOSA

            hnGOODhitwhentrack->Fill(Ngoodhitinevent);
            hTrackToClusterMinDistance->Fill(TrackToHitDistance); // Minimal distance from track to hits
            TrkInMimo->Fill(tu,tv);  //MG 2011/07/08  for efficency 2D plot

            // ******************

            if(MimoDebug>1) cout << " Nearest hit to track at " << TrackToHitDistance << " <?> " << TrackToHitDistanceLimit<< ", hit found = " << aHitIsFound << endl;

            //---------------------------------------------------------------
            // If a good hit near this track was found
            if ( aHitIsFound ){ // if a hit matching the track was found
              StatusEvent[8]++;
              StatusEvent[9]++;
              NofClMatchTrack++;
              temp_NofClMatchTrack[ievt_array]++; // for time dependent efficiency

              // what the hell is this ?? JB 2010/06/03
              for(Short_t ic=0 ; ic<NCut ; ic++){
                if (ChargeAroundSeed>VCut[ic]){
                  NKeepHitOk[ic]++ ;
                }
              }


              // ****************************************
              // Analysis of matched hits & tracks

              NofPixelsInCluster = (thehit->HNNS < MaxNofPixelsInCluster)?thehit->HNNS:MaxNofPixelsInCluster;

              ClusterCharges_compute( thehit);  // JB 2010/06/03
              ClusterPosition_compute( thehit, alignement);  // JB 2010/06/03
              if(MimoDebug>1) cout << " hit   in plane (Plane frame) " << hU << ", " << hV << ", " << hW << "." << endl;

              TrackToHitDistance= sqrt((hU-tu)*(hU-tu)+(hV-tv)*(hV-tv)); // recomputed with best hit position
              // ************************************


              // ************************************
              // Fill data for precision alignement
              // get the computed hit position if not too far from digital position
              // JB 2009/09/01
              if( abs(thehit->Hu - thehit->Hsu) < 5.*PixelSizeU
                 && abs(thehit->Hv - thehit->Hsv) < 5.*PixelSizeV ){
                alignement->NewData( thehit->Hu, thehit->Hv,
				     thehit->Hresu, thehit->Hresv,
				     tx, ty, tz, tdx, tdy); // more precise
              }
              else{
                alignement->NewData( thehit->Hsu, thehit->Hsv,
				     thehit->Hresu, thehit->Hresv,
				     tx, ty, tz, tdx, tdy);  // more robust.
              }
              // ************************************



              // ************************************
              // --- write into the file info about the cluster
              //---- to be written in an ASCII file to be used for the hit separation macros.
              if(MimoDebug) {
                cout << "--- NofPixelsInCluster=" << NofPixelsInCluster << " - evt "<< ievt
                << " - HsnPulse = "<<thehit->HsnPulse
                << " - HsnNoise = "<<thehit->Hsn
                << " - HsnIndex = "<<thehit->HsnIndex
                << " - Hsn S/N = "<<thehit->HsnPulse / thehit->Hsn
                << "total charge=" << TotalCharge
                << endl;

              }

              if(TheWriteGoodHits>1){
                for(Int_t i=0; i < NofPixelsInCluster; i++){
                  outFileGoodEvt << "Evt "<< ievt << endl;
                  outFileGoodEvt << " qonly[" << i << "]="<< qonly[i]
                  << " noise[" << i << "]="<< noise[i]
                  << " snonly[" << i << "]="<< snonly[i]
                  << " indexofPix[" << i << "]="<< IndexofPix[i]
                  << endl;
                  outFileGoodEvt << " q[" << i << "]="<< q[i]
                  << " n[" << i << "]="<< n[i]
                  << " sn[" << i << "]="<< sn[i]
                  << " optimalSize = " << optimalSize
                  << endl;
                }
              }

              // fill the file with the informations
              /*if(TheWriteGoodHits==1){
               outFileGoodEvt <<"-GOOD ASSOCIATION --- RealEventNumber ="<<CurrentEventHeader.GetEventNumber()<<"; ievt = "<<ievt<<endl;
               outFileGoodEvt <<"TrackToHitDistance = "<<TrackToHitDistance<<endl;
               outFileGoodEvt <<"UofHitCG VofHitCG "<< UofHitCG<<" "<<VofHitCG <<endl;
               outFileGoodEvt <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3
               <<" tx=" << tx <<" ty=" << ty <<" tz=" << tz
               <<" tdx="<<tdx <<" tdy="<<tdy <<" hUdigital="
               << hUdigital<<" hVdigital="<< hVdigital<<" pixelsizeV"<< PixelSizeV<<endl;
               outFileGoodEvt<<"Eventheader: evt RealEventNumber, time delay Frame1 "<<CurrentEventHeader.GetEventNumber()<<" "<< CurrentEventHeader.GetTime()
               <<" "<<CurrentEventHeader.EtimeSinceLastEvent<<endl;
               outFileGoodEvt <<"-----"<<endl;
               }*/
              if (MimoDebug > 1) {
                cout <<"hUdigital="<< hUdigital<<" hVdigital="<< hVdigital<<" pixelsizeV="<< PixelSizeV<<endl;
                cout <<"UofHitCG="<< UofHitCG<<" VofHitCG="<<VofHitCG <<endl;
                cout <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3<<endl;
                cout<<" tx=" << tx <<" ty=" << ty <<" tz=" << tz <<" tdx="<<tdx <<" tdy="<<tdy <<" tu="<<tu<<" tv="<<tv<<endl;
                //cout << UCGcorr << " " << VCGcorr << " " << tx << " " << ty << " " << tz << " " << tdx << " " << tdy << endl;
                cout <<"---------------------"<<endl;
              }



              if(NofPixelsInCluster >20){
                outFile << tu << " ";
                outFile << tv << " ";
                outFile << UofPix[0] << " ";
                outFile << VofPix[0] << " ";
                outFile << UofHitEta2x2 << " ";
                outFile << VofHitEta2x2 << " ";
                for (Int_t itmp=0; itmp<NofPixelsInCluster; itmp++){
                  outFile << qonly[itmp] << " ";
                }
                for (Int_t itmp=0; itmp<NofPixelsInCluster; itmp++){
                  outFile << IndexofPix[itmp] << " ";
                }
                outFile << " \n";
              }
              // ************************************


              // ****************** Filling histos
              selection->Fill(8.); // good hit associated to a good track
              selection->Fill(9.);

              ClusterCharges_fill( thehit, ievt);   // JB 2010/06/03
              ClusterShape_fill( thehit);  // JB 2010/04/13
              BinarySpecific_fill( thehit);  // JB 2010/06/03
              ClusterPosition_fill( thehit); // JB 2010/07/21
              TrackHitPosition_fill( thehit, aTrpl, ievt); // JB 2010/07/22

              goodtracks->Fill( tu, tv); //MG 2010/06/10 "matched tracks"

              // Plane study
              //cout<<"+++++++++++++++"<<float(ThePlaneStudied->PFrfr1)<<endl;
              hraw1goodone->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1goodone_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1goodoneNoise_time->Fill(ievt,float(ThePlaneStudied->PFn)*calibration);
              hraw1PFrfr1GOOD->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr1GOOD_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr2GOOD->Fill(float(ThePlaneStudied->PFrfr2));
              hraw1PFrfr2GOOD_time->Fill(ievt,float(ThePlaneStudied->PFrfr2));
              hraw1NoiseGOOD_time->Fill(ievt,float(ThePlaneStudied->PFn));

              // ******************

              //-----------------------------
              // User's stuff
              // JB 2009/09/07
              //-----------------------------
              if(MimoDebug>1) cout << " STARTING user's stuff" << endl;

              for(Int_t iHit=0; iHit<Ngoodhitinevent; iHit++) { // loop over good hits
                ahit = (DAuthenticHit*)Hits->At( IndexOfGoodHitsInEvent[iHit] );
                if( thehit != ahit ) {
                  hUserHitCorrelationLine->Fill( ahit->Hsu - thehit->Hsu);
                  hUserHitCorrelationCol->Fill( ahit->Hsv - thehit->Hsv);
                }
              } // end loop over good hits


              if(MimoDebug) Info("MimosaPro","Done with good matched track-hit study");


            }  // end if a hit matching the track was found
            //---------------------------------------------------------------

            //---------------------------------------------------------------
            else { // end if no hit matching the track was found
              nmiss++;

              if(ievt/NofCycle*NofCycle == ievt){
                cout << " Number of tracks missed = " << nmiss << endl;
              }


              //--- Do some print out

              if (MimoDebug) {
                cout <<"--INEFFICIENT event = "<<ievt<<endl;
              }

              if (MimoDebug>2) {

                cout << " -8-BAD TRACK-HIT DIST OR NO HIT evt = "<<ievt;
                cout << " dist= "<<TrackToHitDistance;
                cout << " ( " << Ngoodhitinevent << " hits in plane)" <<endl;
                cout << " -----------Info on track ----------" << endl;
                cout << " Index  of Track       = " << tk1  << endl;
                cout << " Chi2 of track         = " << chi2 << endl;
                cout << " Track slope (Tdu;Tdv) = " << tdu << " ; " << tdv << " --- In degrees :" << 57.29*acos(tdu) << " ; " << 57.29*acos(tdv)  << endl;    // 180:3.14=57.29577
                //cout <<"UofHitCG VofHitCG "<< UofHitCG<<" "<<VofHitCG <<endl;
                //cout <<"UofHitEta3= "<<UofHitEta3<<" VofHitEta3= " <<VofHitEta3 << endl
                cout << " tx=" << tx <<" ty=" << ty <<" tz=" << tz
                <<" tdx="<<tdx <<" tdy="<<tdy <<endl;
                cout<<" tu= "<<tu<<" tv= "<<tv<<endl;

                if( Ngoodhitinevent>0 ) { // if there is at least a hit

                  cout << " -----------Info on nearest hit ----------" << endl;
                  cout << " Charge on Seed HqM[0]*cal         = " << thehit->HqM[0]*calibration << " = " << thehit->HqM[0] << "*" << calibration <<  endl;
                  cout << " thehit->Hqc * calibration         = " << thehit->Hqc * calibration  << endl;
                  cout << " thehit->Hq0 * calibration         = " << thehit->Hq0 * calibration  << endl;
                  cout << " ChargeAroundSeed = (thehit->Hqc-thehit->Hq0)*calibration = " << (thehit->Hqc-thehit->Hq0)*calibration <<  endl;
                  cout << " Noise  on Seed          = " << thehit->Hsn << endl;
                  cout << " SNR    on Seed          = ";
                  if( (thehit->Hsn)!=0. ) { cout << thehit->Hq0/(calibration*thehit->Hsn); }
                  else { cout << 0.; }
                  cout << endl;
                  cout << " Index  of Seed          = " << thehit->Hsk  << " column: " << (thehit->Hsk)%NofPixelInRaw << " -- line: "  << (thehit->Hsk)/NofPixelInRaw<< endl;
                  cout<<" hUdigital="<< thehit->Hsu <<" hVdigital="<< thehit->Hsv <<" pixelsizeV"<< PixelSizeV<<endl;

                  //cout << " Qseed (e-) = " << qonly[0];
                  //cout << " Charge on Seed q[0]               = " << q[0]      << endl;
                  //cout << " q[0] vs qonly[0] vs HqM[0]*cal    = " << q[0] << " vs " << qonly[0] << " vs " << thehit->HqM[0]*calibration << endl;
                  //cout << " q[1] vs qonly[1] vs HqM[1]*cal    = " << q[1] << " vs " << qonly[1] << " vs " << thehit->HqM[1]*calibration << endl;
                  //cout << " q[2] vs qonly[2] vs HqM[2]*cal    = " << q[2] << " vs " << qonly[2] << " vs " << thehit->HqM[2]*calibration << endl;
                  //cout << " q[3] vs qonly[3] vs HqM[3]*cal    = " << q[3] << " vs " << qonly[3] << " vs " << thehit->HqM[3]*calibration << endl;
                  //cout << " q[4] vs qonly[4] vs HqM[4]*cal    = " << q[4] << " vs " << qonly[4] << " vs " << thehit->HqM[4]*calibration << endl;
                  //cout << " q[6] vs qonly[6] vs HqM[6]*cal    = " << q[6] << " vs " << qonly[6] << " vs " << thehit->HqM[6]*calibration << endl;
                  //cout << " q[7] vs qonly[7] vs HqM[7]*cal    = " << q[7] << " vs " << qonly[7] << " vs " << thehit->HqM[7]*calibration << endl;
                  //cout << " q[8] vs qonly[8] vs HqM[8]*cal    = " << q[8] << " vs " << qonly[8] << " vs " << thehit->HqM[8]*calibration << endl;
                  //cout << " q[9] vs qonly[9] vs HqM[9]*cal    = " << q[9] << " vs " << qonly[9] << " vs " << thehit->HqM[9]*calibration << endl;
                  //cout << " q[10] vs qonly[10] vs HqM[10]*cal = " << q[10] << " vs " << qonly[10] << " vs " << thehit->HqM[10]*calibration << endl;
                  //cout << " q[11] vs qonly[11] vs HqM[11]*cal = " << q[11] << " vs " << qonly[11] << " vs " << thehit->HqM[11]*calibration << endl;
                  //cout << " q[12] vs qonly[12] vs HqM[12]*cal = " << q[12] << " vs " << qonly[12] << " vs " << thehit->HqM[12]*calibration << endl;
                  //cout << " q[13] vs qonly[13] vs HqM[13]*cal = " << q[13] << " vs " << qonly[13] << " vs " << thehit->HqM[13]*calibration << endl;
                  //cout << " q[14] vs qonly[14] vs HqM[14]*cal = " << q[14] << " vs " << qonly[14] << " vs " << thehit->HqM[14]*calibration << endl;

                  //cout << " -----------Comparison track - hit ----------" << endl;
                  //cout << " Tud = Hu-Tu  (TTree)  = " << tud << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Tvd = Hv-Tv  (TTree)  = " << tvd << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Hu                    = " << thehit->Hu << " ---UofHitCG5 = "  << UofHitCG5  << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Hv                    = " << thehit->Hv << " ---VofHitCG5 = "  << VofHitCG5  << endl;//          << " line: " << tk1%NofPixelInRaw           << " -- column: "  << tk1/NofPixelInRaw<< endl;
                  //cout << " Tack to hit dist calc = " <<  sqrt( (UofHitCG5-tu)*(UofHitCG5-tu) + (VofHitCG5-tv)*(VofHitCG5-tv) ) << endl;

                } // end if there is at least one hit

                cout <<"-----"<<endl;

                cout <<"--NO HIT FOUND---Reco evt = "<<ievt<<endl;
                printf("event Pas de hit, impact: tu=%f  tv=%f  chi2 : %f\n",tu,tv,chi2);
                cout<< float(ThePlaneStudied->PFrfr1)<<endl;

                cout << "Event number = " <<Previousbadevent<<"; Previous event with a track in mimosa = "<<Previousbadevent2<< endl;
                cout<<"Eventheader: evt, time delay Frame1 "<<CurrentEventHeader.GetEventNumber()<<" "<< CurrentEventHeader.GetTime()
                <<" "<<CurrentEventHeader.EtimeSinceLastEvent<<" "<<ThePlaneStudied->PFrfr1<<endl;
                CurrentEventHeader.Print();
                cout <<"-----"<<endl;
              } // end if MimoDebug


              //----  fill the file with the infomations if requested
              if(TheWriteMissedHits==1){ // if write info for missed hits
                if (MimoDebug) cout << " Writing info for missed event " << ievt << endl;

                outFileBadEvt<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
                outFileBadEvt << "event counter = " << ievt << ", from DSF = " << CurrentEventHeader.GetEventNumber() << ", from daq = " << Evt->GetFrameNb() << endl;
                outFileBadEvt << "Event number = " << Previousbadevent << "; Previous event with a track in mimosa = " << Previousbadevent2 << endl;
                if( CurrentEventHeader.GetNumberOfTriggers() ) {
                  outFileBadEvt << "# triggers= " << CurrentEventHeader.GetNumberOfTriggers();
                  for( Int_t iTrig=0; iTrig<CurrentEventHeader.GetNumberOfTriggers(); iTrig++) {
                    outFileBadEvt << " : " << CurrentEventHeader.GetTriggerAt( iTrig);
                  }
                  outFileBadEvt << endl;
                }
                if( CurrentEventHeader.GetNumberOfFrames() ) {
                  outFileBadEvt << "# frames= " << CurrentEventHeader.GetNumberOfFrames();
                  for( Int_t iTrig=0; iTrig<CurrentEventHeader.GetNumberOfFrames(); iTrig++) {
                    outFileBadEvt << " : " << CurrentEventHeader.GetFrameAt( iTrig);
                  }
                  outFileBadEvt << endl;
                }
                if( CurrentEventHeader.GetNumberOfTimestamps() ) {
                  outFileBadEvt << "# timestamps= " << CurrentEventHeader.GetNumberOfTimestamps();
                  for( Int_t iTrig=0; iTrig<CurrentEventHeader.GetNumberOfTimestamps(); iTrig++) {
                    outFileBadEvt << " : " << CurrentEventHeader.GetTimestampAt( iTrig);
                  }
                  outFileBadEvt << endl;
                }

                outFileBadEvt << " total # hits in all planes = " << NbOfHits << endl;
                outFileBadEvt << " total # hits in the DUT = " << Nhitinevent << ", good hits = " << Ngoodhitinevent << endl;
                outFileBadEvt << " total # good tracks in telescope = " << Ngoodtrackinevent << endl;
                outFileBadEvt << " total # good tracks in the DUT = " << NgoodtrackinDUTinevent << endl;
                outFileBadEvt << " total # tracks in the EXCLUSION GEOMATRIX = " <<           Ngoodtrackgeominevent << endl;
                outFileBadEvt << " Track properties:" << endl;
                outFileBadEvt << "    tx = " << tx << ", ty = " << ty << ", tz = " << tz << endl;
                outFileBadEvt << "    tdx = " << tdx << ", tdy = " << tdy << endl;
                outFileBadEvt << "    tu = " << tu << ", tv = " << tv << " or col = " << (Int_t)(tu+FirstPixelShiftU)/PixelSizeU << ", row = " << (Int_t)(tv+FirstPixelShiftV)/PixelSizeV << endl;
                outFileBadEvt << "    chi2 = " << chi2 << endl;

                outFileBadEvt << " track to nearest hit distance = "<< TrackToHitDistance << endl;
                if( Ngoodhitinevent>0 ) { // if there is at least a hit
                  outFileBadEvt << " Nearest hit properties:" << endl;
                  outFileBadEvt << "   # pixels in hit " << thehit->HNNS << endl;
                  outFileBadEvt << "   (u, v) = (" << thehit->Hu << ", " << thehit->Hv << ")" << endl;
                  outFileBadEvt << "   seed (u, v) = (" << thehit->Hsu << ", " << thehit->Hsv << "), [col, row] = [" << (thehit->Hsu+FirstPixelShiftU)/PixelSizeU << ", " << (thehit->Hsv+FirstPixelShiftV)/PixelSizeV << "]" << endl;
                  outFileBadEvt << "   charge (w/o calib) in seed = " << thehit->Hq0 << ", around seed " << ahit->Hqc - ahit->Hq0 << " and in hit " << thehit->Hqc << endl;
                  outFileBadEvt << "   S/N of seed = " << thehit->HsnPulse / ahit->Hsn << ", around seed " << thehit->HSNneighbour << endl;
                } // end if there is at least a hit

                outFileBadEvt << "-----" << endl;
              } // end if write info for missed hits

              else if(TheWriteMissedHits==2){ // if write info for missed hits
                outFileBadEvt<<ievt<<","<<CurrentEventHeader.GetTriggerAt(1)<<","<<CurrentEventHeader.GetTriggerAt(0)<<","<<Ngoodhitinevent<<endl;
              }

              // ****************** Filling histos
              //MissedHit->cd();


              tuv->Fill(tu,tv);

              hchi2_nc->Fill(chi2);
              htu->Fill(tu);
              htv->Fill(tv);
              htuhtv->Fill(tu,tv);
              Int_t itu;
              Int_t itv;
              itu= Int_t(tu/PixelSizeU);
              itv= Int_t(tv/PixelSizeV);
              htuvInPix->Fill(tu-itu*PixelSizeU,tv-itv*PixelSizeV);

              hraw1badone->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1badone_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1badoneNoise_time->Fill(ievt,float(ThePlaneStudied->PFn)*calibration);
              hraw1PFrfr1BAD->Fill(float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr1BAD_time->Fill(ievt,float(ThePlaneStudied->PFrfr1));
              hraw1PFrfr2BAD->Fill(float(ThePlaneStudied->PFrfr2));
              hraw1PFrfr2BAD_time->Fill(ievt,float(ThePlaneStudied->PFrfr2));
              hraw1NoiseBAD_time->Fill(ievt,float(ThePlaneStudied->PFn));

              if( Ngoodhitinevent>0 ) { // if there is at least a hit (thehit is the nearest)
                hqc_nc->Fill(thehit->Hqc);
                hnpix_nc->Fill(thehit->HNNS); // Hsk); JB 2009/09/01
                huvBad->Fill( tu, tv);
                tuv1->Fill( thehit->Hsu, thehit->Hsv);
              }
              else {
                FalseHitMap->Fill(tu,tv);
              }

              // ******************

            } // end if no hit matching the track was found
            //---------------------------------------------------------------


          }  // end if the track is in MIMOSA
          //---------------------------------------------------------------

        } // end if good track
        //---------------------------------------------------------------

      } // end loop on tracks
      //---------------------------------------------------------------

      if(MimoDebug) Info("MimosaPro","End loop on tracks, found %d good tracks in DUT", NgoodtrackinDUTinevent);

    } // end if Plane exists with tracks and hits
    //---------------------------------------------------------------

    else { // if there was no plane or tracks or hits
      Warning("MimosaPro","problem event %d with plane %d: Authentic %d?=1 or Transparent %d?=1 or #Hits %d<? %d <?%d  or #trpl %d>?0\n\n", ievt, ThePlaneNumber, okP, okT, CUT_MinNbOfHits, NbOfHits, CUT_MaxNbOfHits, NbOfTrpl);
      //return; // allow to continue and simply skip this event, JB 2009/09/01
    }


    //---------------------------------------------------------------
    //-- Do some statistics after event analysis
    //---------------------------------------------------------------
    // StatusEvent[6]>1 <=> there are some hits in the DUT
    // StatusEvent[7]>1 <=> there are some good hits in the DUT
    // StatusEvent[3]>1 <=> there are some tracks
    // StatusEvent[4]>1 <=> there is a good track
    // StatusEvent[5]>1 <=> there is a good track in the good geomatrix
    // StatusEvent[8/9]>1 <=> a hit is matched to the track

    if (MimoDebug > 1) {
      cout << "StatusEvent = ";
      for ( Int_t i=1 ; i<10 ; i++ ) {
        cout<<StatusEvent[i]<<" - ";
      }
      cout<<endl;
    }


    if(MimoDebug) Info("MimosaPro","Event %d ended\n",ievt);

  } // end of Main loop over event

  //**********************************************************************************
  //******************************* END OF MAIN LOOP *********************************
  //**********************************************************************************

  // ************************************
  // Call final part of analysis here

  ClusterPosition_end(); // JB 2014/01/10
  ClusterCharges_end(); // Jb 2010/07/22
  ClusterShape_end();  // JB 2010/04/13
  BinarySpecific_end(); // JB 2010/06/03
  FakeRate_end( NtrkInMimo); // JB 2010/07/22
  Efficiency_end( MaxEvent-MinEvent+1 ); // JB 2011/11/04

  // ************************************

  // Display final histograms
  MainCanvas->Clear();
  MainCanvas->SetBorderMode(0);
  MainCanvas->Divide(1,2);
  MainCanvas->cd(1);
  if( selection->GetMaximum()>15*selection->GetBinContent(6)) gPad->SetLogy();
  selection->Draw();
  for( Int_t il=0; il<8; il++) {
    textLabel[il]->Draw();
  }
  MainCanvas->cd(2);
  hClusterMeanForm->Draw("lego2");
  //hChargeIntegral4->Draw("surf");
  MainCanvas->Update();

  //---------------------------------------------------------------
  //-- Update and Save eta and alignment parameters
  //---------------------------------------------------------------

  // Process the distributions to get the correction functions:
  // Reshape to call single functions, JB 2011/06/19

  cout << "In the end... "<< endl;
  cout << "In the end... "<< endl;
  cout << "In the end... "<< endl;
  cout << "In the end... "<< endl;

  //if ( !strstr(SaveAlign,"no") || !strstr(SaveAlign,"NO") || !SaveAlign) {
  if ( strstr(SaveAlign,"yes") || strstr(SaveAlign,"YES") ) {

    CreateNewEta();
    fMimosaProDone=kTRUE; // required so that AlignMimosa proceeds, JB 2011/11/25
    AlignMimosa( TrackToHitDistanceLimit); // new option added, JB 2012/05/11

  }

  //---------------------------------------------------------------
  //-- Print statistics out
  //---------------------------------------------------------------
  cout << "DSF file used as input: " << RootFile << endl;
  cout << "-------- RUN NUMBER = "<<RunNumber<<" Plane number = "<<ThePlaneNumber
  <<" submatrix Number = "<<ThesubmatrixNumber<<endl;
  cout << "-------- matched/good tracks in DUT = " << NofClMatchTrack << "  " << NtrkInMimo << "  " << MimosaEfficiency << endl;
  cout << "-------- Efficiency = " << MimosaEfficiency*100.0 <<" +- "<< MimosaEfficiency_ERR*100.0 << "%" << endl;
  cout << "-------- Nb of good rec tracks in telescope (independant of geomatrix) per event,  average = " << hNTracksPerEvent->GetMean() << " with RMS = " << hNTracksPerEvent->GetRMS() << endl; // JB 2010/07/23
  cout << "-------- Nb of rec hits in DUT = " << NRecHit << " good = " << ngoodhit << ", average per event " << hnhit->GetMean() << " with RMS " << hnhit->GetRMS() << endl;
  cout << "-------- Average pixel multiplicity in hit " << hnpix_c->GetMean() << " with RMS " << hnpix_c->GetRMS() << " (uncertainty " << hnpix_c->GetRMS()/sqrt(hnpix_c->GetEntries())<< ")" << endl;
  cout << "-------- track-Hit dist cut= "<<TrackToHitDistanceLimit<<" chi2 max= "<<TrackChi2Limit<<" Geomatrix = "<<GeoMatrix<<": "<<geomUmin<<"<U<"<<geomUmax<<", "<<geomVmin<<"<V<"<<geomVmax<<endl;
  cout << "-------- minimal nb of hits required per tracks = " << MinHitsPerTrack << endl;
  if( MaxNbOfTracksInGeom > -1 ) cout << "-------- Max nb of tracks allowed " << MaxNbOfTracksInGeom << ", in exclusion geomatrix " << GeoMatrixForTrackCut<<": "<<exgeomUmin<<"<U<"<<exgeomUmax<<", "<<exgeomVmin<<"<V<"<<exgeomVmax<< endl;

  cout << "-------- number of tracks un-matched " << nmiss << ", with thdist > THdist limit " << nmissthdist << ", without hits in DUT  " << nmissh << endl; // JB 2009/09/08
  cout << "-------- Total number of event processed= "<<MaxEvent-MinEvent+1<<endl;
  cout << "-------- CUTS S/N seed and S/N neighbours= "<<CUT_S2N_seed<<" "<<CUT_S2N_neighbour<<endl;
  cout << "-------- MIN and MAX number of hits per event to evaluate efficiency "<<CUT_MinNbOfHits<<" "<<CUT_MaxNbOfHits<<endl;
  cout << "-------- calibration "<<calibration<<endl;

  //---------------------------------------------------------------
  //---- Hot pixels management
  //---------------------------------------------------------------

  HotPixel_end( MaxEvent-MinEvent+1);

  //---------------------------------------------------------------
  //-- Write histos
  //---------------------------------------------------------------

  if (fWriteHistos) {
    gSystem->cd(CreateGlobalResultDir());
    if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

    Char_t Header[200];
    sprintf(Header,"AllPlots_%d_%d.root",RunNumber,ThePlaneNumber);
    TFile* AllHist = new TFile(Header,"RECREATE");
    dir->GetList()->Write();
    AllHist->Close(); delete AllHist;
    gSystem->cd(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory

  }

  //---------------------------------------------------------------
  //-- The End
  //---------------------------------------------------------------

  fMimosaProDone=kTRUE;
  //SS 2011.12.05: Efficiency, multiplicity and corresponding errors are saved to the CSV file. Run number, plane number and geomatrix limits are saved as well.
  if (strstr(SaveAlign,"no") || strstr(SaveAlign,"NO")){
    csvfile.open ("Main_results.csv",ios::app);
    TDatime aTime;
    csvfile<<aTime.AsString()<<",";
    csvfile << RunNumber <<","<<ThePlaneNumber<<","<<geomUmin<<","<<geomUmax<<","<<geomVmin<<","<<geomVmax<<","<<NofClMatchTrack<<","<<NtrkInMimo<<","<<MimosaEfficiency*100.0<<","<<MimosaEfficiency_ERR*100.0<<","<<hnpix_c->GetMean()<<","<<hnpix_c->GetRMS()/sqrt(hnpix_c->GetEntries())<<",";
    //MimosaResolution(); //MimosaResolution is run here in order to add information to the CSV file.
    csvfile<<RootFile<<endl;
    csvfile.close();
  }
  fClearDone=kFALSE;
  PreparePost();
}

//______________________________________________________________________________
//
void MimosaAnalysis::MimosaImaging(Int_t MaxEvt, Int_t submatrix, Int_t GeoMatrix, Double_t theta_init, Int_t Npeak, Double_t S, Double_t W, Bool_t FirstLoop, Bool_t chooseFit)
{
  // -- Processing macro for MIMOSA root DSF file
  //     for a single given submatrix in a given area (GeoMatrix)
  //
  // Allows to determine the spatial resolution of a sensor using parallel bands chart.
  // There code contains three part:
  //    - First:   determination of the best rotation angle "minTheta" to use a frame parallel to the bands
  //    - Second:  data projection on this new frame, different clusters multiplicities
  //    - Third:   fit of the data projection on x, using different clusters multiplicities
  //
  // Input variables:
  //    - theta_init = estimation of the angle minTheta
  //    - Npeak      = number of bands
  //    - S          = estimation of the distance between the center of two peaks
  //    - W          = estimation of the peak width
  //
  //    - FirstLoop  = does the rotation angle must be computed?
  //                   if FirstLoop=true then all the code is performed: evaluation of the best angle theta + spatial resolution
  //                   if FirstLoop=false then the first loop is skipped: the angle theta_init is used to evaluate the spatial resolution
  //
  //    - chooseFit  = which fit function has to be used?
  //                   if chooseFit=true then the distance S between bands and the bands width W are assumed unchanged
  //                   if chooseFit=false then the distance S between bands and the bands width W changed for each bands
  //
  // Created: JH 2014/06/16


  if(!CheckIfDone("init,clear")) return;
  if(!ThePlaneNumber || ThePlaneNumber>fSession->GetTracker()->GetPlanesN()) {Warning("MimosaImaging()","Please set a plane number (1 or 2)"); return; }
  if(!RunNumber) Error("MimosaImaging","RunNumber not set! Please run InitSession  first");

  InitMimosaType();
  Info("MimosaImaging","Mimosa type %d", MimosaType);

  CorStatus=2; // indicate CorPar file is not filled, JB 2010/08/27

  GetAnalysisGoal();

  Nevt = OpenInputFile(); // total number of events in the tree
  if( Nevt<=0 ) Error("MimosaImaging"," The input file contains an incorrect number of events %d!",Nevt);
  else Info("MimosaImaging","There is %d events in the input file.",Nevt);


  //----------------------------------------------------------------------------------
  // -- Chose the sub-matrix.
  //----------------------------------------------------------------------------------
  ThesubmatrixNumber = submatrix;
  Thegeomatrix = GeoMatrix; // define the area of interest in the DUT
  GetParameters();


  //----------------------------------------------------------------------------------
  // -- Analysis variables
  //----------------------------------------------------------------------------------
  Int_t Ngoodhits = 0; // total # good hits in the area
  Int_t Ngoodhitsinevent; // # good hits in the event in the area
  //Int_t NgoodhitsinPeak = 0; // total # good hits in the calibration peak
  Int_t NbOfHits = 0;  // # hits over all planes in the event
  Int_t totalNOfHits = 0; // total # hits in the plane

  Int_t *seedList=NULL; // list of hits, value=-1 means not selected, seed index otherwise

  Info("MimosaImaging","Analysis variables initialized");


  //----------------------------------------------------------------------------------
  // -- booking histograms
  //----------------------------------------------------------------------------------
  BookingHistograms();

  //-- Histogram to compute the best theta value
  Double_t deltaThetaCut = 20;
  const Int_t Nbin_ThetaCut  = 200;
  Double_t R_ThetaCut[2];
  R_ThetaCut[0] = theta_init - deltaThetaCut;
  R_ThetaCut[1] = theta_init + deltaThetaCut;
  const Int_t Nbin_ThetaScan = 400;
  Double_t R_ThetaScan[2];
  R_ThetaScan[0] = 0.0;
  R_ThetaScan[1] = 180.0;

  h2RmsOnThetaScanVsThetaCut = new TH2F("h2RmsOnThetaScanVsThetaCut", "RMS vs #theta_{cut} and #theta_{scan}",
                                        Nbin_ThetaScan, R_ThetaScan[0], R_ThetaScan[1],
                                        Nbin_ThetaCut, R_ThetaCut[0], R_ThetaCut[1]);
  h2RmsOnThetaScanVsThetaCut->GetXaxis()->SetTitle("#theta_{scan} (degree)");
  h2RmsOnThetaScanVsThetaCut->GetYaxis()->SetTitle("#theta_{cut} (degree)");
  h2RmsOnThetaScanVsThetaCut->GetZaxis()->SetTitle("RMS (#mum)");
  h2RmsOnThetaScanVsThetaCut->GetXaxis()->CenterTitle(true);
  h2RmsOnThetaScanVsThetaCut->GetYaxis()->CenterTitle(true);
  h2RmsOnThetaScanVsThetaCut->GetZaxis()->CenterTitle(true);
  h2RmsOnThetaScanVsThetaCut->SetLineColor(kBlue);
  h2RmsOnThetaScanVsThetaCut->SetLineWidth(2);

  //----------------------------------------------------------------------------------
  //-- Prepare the event loop
  //----------------------------------------------------------------------------------

  //---------------------------------------------------------------
  //  Chose the min and max event number you want to loop on:
  Int_t MinEvent = fSession->GetSetup()->GetPlanePar(ThePlaneNumber).InitialNoise;
  Int_t MaxEvent = TMath::Min((Long64_t)MaxEvt,t->GetEntries()) ;

  //---------------------------------------------------------------
  // Variables and arrays initialisation
  Double_t minTheta, ImHu, ImHv;
  Int_t numberOfHitsInEvent = 0;
  Int_t numberOfHitsInCut[Nbin_ThetaCut];
  const int Nstep( h1RmsOnTheta->GetXaxis()->GetNbins() );

  Double_t Mean_array[Nbin_ThetaScan][Nbin_ThetaCut], RMS_array[Nbin_ThetaScan][Nbin_ThetaCut];

  for (Int_t i_thetaCut=0; i_thetaCut<Nbin_ThetaCut; i_thetaCut++) {
    numberOfHitsInCut[i_thetaCut] = 0;
    for (Int_t i_thetaScan=0; i_thetaScan<Nbin_ThetaScan; i_thetaScan++) {
      Mean_array[i_thetaScan][i_thetaCut] = 0;
      RMS_array[i_thetaScan][i_thetaCut] = 0;
    }
  }

  if ( FirstLoop ) Info("MimosaImaging","\n\nReady to loop over %d events and %d theta step\n", MaxEvent, Nstep);
  else             Info("MimosaImaging","\n\nReady to loop over %d events\n", MaxEvent);

  //---------------------------------------------------------------
  // Call init part of the analysis here
  ProjectionImaging_init();
  ClusterShape_init();
  ClusterCharges_init();
  ClusterPosition_init();


  // **********************************************************************************
  // ******************************* FIRST MAIN LOOP **********************************
  // Goal: determination of the best rotation angle "minTheta" to use a frame parallel
  // to the bands.
  //
  // The best angle "minTheta" is chosen when the data distribution have the lowest RMS.
  // h1RmsOnTheta: histogram displaying the RMS vs theta.
  // **********************************************************************************
  if ( FirstLoop ) { // if all the code is performed

    cout << "*----------------* FIRST MAIN LOOP *----------------*" << endl;
    cout << "*- Determination of the best rotation angle Theta  -*\n" << endl;


    for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // First main loop over events

      if(MimoDebug) Info("MimosaImaging","Reading event %d",ievt);

      // Event wise parameters
      Ngoodhitsinevent = 0;

      t->GetEvent(ievt);

      if(MimoDebug>1) cout << " getting the hits" << endl;
      TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
      NbOfHits   = Hits->GetLast()+1; // total # hits over all planes


      //---------------------------------------------------------------
      // Loop over hits BUT keep only the ones in the right plane
      if(MimoDebug>1) cout << " Looping over " << NbOfHits << " hits" << endl;
      for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

        DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);
        if( ahit->Hpk != ThePlaneNumber ) continue; // select only hits in DUT


        if ( fIfImaging ) {

          if( Thegeomatrix<0 || Thegeomatrix>3 ) {
            printf("MimosaImaging: WARNING, you asked for a geomatrix (%d) which does not exist, changed to 0\n", Thegeomatrix);
            Thegeomatrix = 0;
          }

          //---------------------------------------------------------------
          // Data projection on the estimated frame of the bands


          for (Int_t i_thetaCut=0; i_thetaCut<Nbin_ThetaCut; i_thetaCut++) { // loop over thetaCut

            Double_t thetaCut = h2RmsOnThetaScanVsThetaCut->GetYaxis()->GetBinCenter(i_thetaCut+1);
            ImHu = (ahit->HuCG)*TMath::Cos(thetaCut*TMath::Pi()/180.0) + (ahit->HvCG)*TMath::Sin(thetaCut*TMath::Pi()/180.0);
            ImHv = -(ahit->HuCG)*TMath::Sin(thetaCut*TMath::Pi()/180.0) + (ahit->HvCG)*TMath::Cos(thetaCut*TMath::Pi()/180.0);

            if ( (ImHu >= geomUmin) && (ImHu <= geomUmax) && (ImHv >= geomVmin) && (ImHv <= geomVmax) ) { // cut over theta_init

              numberOfHitsInCut[i_thetaCut]++;

              //---------------------------------------------------------------
              // Calculation of the data RMS depending on the angle theta
              for (Int_t i_thetaScan=0; i_thetaScan<Nbin_ThetaScan; i_thetaScan++) { // Loop over theta

                Double_t theta = h2RmsOnThetaScanVsThetaCut->GetXaxis()->GetBinCenter(i_thetaScan+1)*TMath::Pi()/180.0;
                ImHu = (ahit->HuCG)*TMath::Cos(theta) + (ahit->HvCG)*TMath::Sin(theta);

                Mean_array[i_thetaScan][i_thetaCut] += ImHu;
                RMS_array[i_thetaScan][i_thetaCut] += TMath::Power(ImHu,2);

              } // end Loop over theta

            } // end cut over theta_init

          } // end i_thetaCut

        } // end if

      } // end loop on hits

      if(MimoDebug) Info("MimosaImaging","End loop on hits, found %d good hits in event %d (total %d good hits)", Ngoodhitsinevent, ievt, Ngoodhits);

      if ( ievt/10000.0 == int(ievt/10000.0) && ievt != 0 ) cout << "Number of studied events: " << ievt << endl;

    } // end of Main loop over event


    //---------------------------------------------------------------
    // Get the best value of theta to study data in the frame of the bands.
    // Plot of the RMS vs theta.
    Double_t minRMS = 1.0e20;
    minTheta = -999;

    for (Int_t i_thetaCut=0; i_thetaCut<Nbin_ThetaCut; i_thetaCut++) {
      for (Int_t i_thetaScan=0; i_thetaScan<Nbin_ThetaScan; i_thetaScan++) {

        Mean_array[i_thetaScan][i_thetaCut] /= numberOfHitsInCut[i_thetaCut];
        RMS_array[i_thetaScan][i_thetaCut] /= numberOfHitsInCut[i_thetaCut];
        RMS_array[i_thetaScan][i_thetaCut] -= TMath::Power(Mean_array[i_thetaScan][i_thetaCut],2);
        RMS_array[i_thetaScan][i_thetaCut] = TMath::Sqrt(RMS_array[i_thetaScan][i_thetaCut]);

        h2RmsOnThetaScanVsThetaCut->SetBinContent(i_thetaScan+1, i_thetaCut+1, RMS_array[i_thetaScan][i_thetaCut]);

        if (RMS_array[i_thetaScan][i_thetaCut] < minRMS) {
          minRMS = RMS_array[i_thetaScan][i_thetaCut];
          minTheta = h2RmsOnThetaScanVsThetaCut->GetXaxis()->GetBinCenter(i_thetaScan+1);
        }
      }
    }

    cout << "\n*------------------------------------*" << endl;
    cout << "Theta min = " << minTheta << "    RMS min = " << minRMS << endl;
    cout << "*------------------------------------*" << endl;

  } // end if FirstLoop

  // **********************************************************************************
  // ***************************** END OF FIRST MAIN LOOP *****************************
  // **********************************************************************************


  // **********************************************************************************
  // ******************************* SECOND MAIN LOOP *********************************
  // Goal: data projection on the frame of the bands, now using the best theta value.
  //
  // h1ProjectionOnX: data projection on the x-axis.
  // h1ProjectionOnY: data projection on the y-axis.
  //
  // Histograms for different multiplicities are also filled: h1ProjectionOnXMult and
  // h1ProjectionOnYMult, projection on x and y respectively.
  //    - Multiplicity = 1
  //    - Multiplicity = 2
  //    - Multiplicity = 3
  //    - Multiplicity > 3
  // **********************************************************************************
  if ( !FirstLoop ) {
    minTheta = theta_init;
    cout << "\nThe input FirstLoop is set to false." << endl;
    cout << "The first main loop is skipped:  theta is set to " << minTheta << " degree" << endl;
  }

  //-- Align bins with pixels
  float pitchImHu = 0, pitchImHv = 0;
  float minThetaRad = minTheta*TMath::DegToRad();
  float Theta1      = TMath::ATan(PixelSizeV/PixelSizeU);

  pitchImHu = PixelSizeU; // init added by JB 2016/04/17
  pitchImHv = PixelSizeV;
  if(TMath::Abs(minThetaRad) >= 0 && TMath::Abs(minThetaRad) < Theta1) {
    pitchImHu = PixelSizeU/TMath::Cos(TMath::Abs(minThetaRad));
    pitchImHv = PixelSizeV/TMath::Cos(TMath::Abs(minThetaRad));
  }
  else if(TMath::Abs(minThetaRad) >= Theta1 && TMath::Abs(minThetaRad) < TMath::Pi() - Theta1) {
    pitchImHu = PixelSizeV/TMath::Sin(TMath::Abs(minThetaRad));
    pitchImHv = PixelSizeU/TMath::Sin(TMath::Abs(minThetaRad));
  }
  else if(TMath::Abs(minThetaRad) >= (TMath::Pi() - Theta1) && TMath::Abs(minThetaRad) < TMath::Pi()) {
    pitchImHu = -PixelSizeU/TMath::Cos(TMath::Abs(minThetaRad));
    pitchImHv = -PixelSizeV/TMath::Cos(TMath::Abs(minThetaRad));
  }

  cout << "\nIntiate: geomUmin = "<< geomUmin << "     geomUmax = " << geomUmax << endl;

  float divisionU    = geomUmax / pitchImHu;
  int   intdivisionU = int(divisionU);
  if (TMath::Abs(divisionU - intdivisionU) > 1.0e-6 ) {
    if(divisionU >= -1 && divisionU <= 0) geomUmax = 0.0*pitchImHu;
    else if(divisionU > 0) geomUmax = (int(geomUmax/pitchImHu) + 1)*pitchImHu;
    else if(divisionU <-1) geomUmax = (int(geomUmax/pitchImHu) + 0)*pitchImHu;
  }
  divisionU = geomUmin / pitchImHu;
  intdivisionU = int(divisionU);
  if (TMath::Abs(divisionU - intdivisionU) > 1.0e-6 ) {
    if (divisionU >= -1 && divisionU <= 0) geomUmin = -1.0*pitchImHu;
    else if(divisionU > 0) geomUmin = (int(geomUmin/pitchImHu) + 0)*pitchImHu;
    else if(divisionU <-1) geomUmin = (int(geomUmin/pitchImHu) - 1)*pitchImHu;
  }

  cout << "New: geomUmin = "<< geomUmin << "     geomUmax = " << geomUmax << endl;
  cout << "PitchU = "<< PixelSizeU << "     PitchEffective = " << pitchImHu << endl;

  cout << "\nIntiate: geomVmin = "<< geomVmin << "     geomVmax = " << geomVmax << endl;

  Double_t divisionV = geomVmax / pitchImHv;
  Int_t intdivisionV = int(divisionV);
  if (TMath::Abs(divisionV - intdivisionV) > 1.0e-6 ) {
    if (divisionV >= -1 && divisionV <= 0) geomVmax = 0.0*pitchImHv;
    else if(divisionV > 0) geomVmax = (int(geomVmax/pitchImHv) + 1)*pitchImHv;
    else if(divisionV <-1) geomVmax = (int(geomVmax/pitchImHv) + 0)*pitchImHv;
  }
  divisionV = geomVmin / pitchImHv;
  intdivisionV = int(divisionV);
  if (TMath::Abs(divisionV - intdivisionV) > 1.0e-6 ) {
    if(divisionV >= -1 && divisionV <= 0) geomVmin = -1.0*pitchImHv;
    else if(divisionV > 0) geomVmin = (int(geomVmin/pitchImHv) + 0)*pitchImHv;
    else if(divisionV <-1) geomVmin = (int(geomVmin/pitchImHv) - 1)*pitchImHv;
  }

  cout << "New: geomVmin = "<< geomVmin << "     geomVmax = " << geomVmax << endl;
  cout << "PitchU = "<< PixelSizeU << "     PitchEffective = " << pitchImHv << endl;

  int NofEffectivePitchU = int((geomUmax-geomUmin)/pitchImHu);
  if(TMath::Abs(((geomUmax-geomUmin)/NofEffectivePitchU) - pitchImHu) > 1.0e-3) {
    if(((geomUmax-geomUmin)/NofEffectivePitchU) - pitchImHu > 0)      NofEffectivePitchU += 1;
    else if(((geomUmax-geomUmin)/NofEffectivePitchU) - pitchImHu < 0) NofEffectivePitchU -= 1;
  }

  int NofEffectivePitchV = int((geomVmax-geomVmin)/pitchImHv);
  if(TMath::Abs(((geomVmax-geomVmin)/NofEffectivePitchV) - pitchImHv) > 1.0e-3) {
    if(((geomVmax-geomVmin)/NofEffectivePitchV) - pitchImHv > 0)      NofEffectivePitchV += 1;
    else if(((geomVmax-geomVmin)/NofEffectivePitchV) - pitchImHv < 0) NofEffectivePitchV -= 1;
  }

  cout << "Bins U        = " << NofEffectivePitchU << endl;
  cout << "New Range U   = (" << geomUmax << "," << geomUmin << ")" << endl;
  cout << "Width U       = " << (geomUmax - geomUmin)/NofEffectivePitchU << endl;
  cout << "Pitch Effec U = " << pitchImHu << endl;

  cout << "Bins V        = " << NofEffectivePitchV << endl;
  cout << "New Range V   = (" << geomVmax << "," << geomVmin << ")" << endl;
  cout << "Width V       = " << (geomVmax - geomVmin)/NofEffectivePitchV << endl;
  cout << "Pitch Effec V = " << pitchImHv << endl;

  h2DXprimeVsYprime = new TH2F("h2DXprimeVsYprime",
			       "X' vs Y' for all multiplicities",
			       NofEffectivePitchU, geomUmin, geomUmax,
			       NofEffectivePitchV, geomVmin, geomVmax);
  h2DXprimeVsYprime->GetXaxis()->SetTitle("X' (#mum)");
  h2DXprimeVsYprime->GetYaxis()->SetTitle("Y' (#mum)");
  h2DXprimeVsYprime->GetZaxis()->SetTitle("hits");
  h2DXprimeVsYprime->GetXaxis()->CenterTitle(true);
  h2DXprimeVsYprime->GetYaxis()->CenterTitle(true);
  h2DXprimeVsYprime->GetZaxis()->CenterTitle(true);
  h2DXprimeVsYprime->SetLineColor(kBlue);
  h2DXprimeVsYprime->SetLineWidth(2);

  h1ProjectionOnX->SetBins( NofEffectivePitchU, geomUmin, geomUmax);
  h1ProjectionOnY->SetBins( NofEffectivePitchV, geomVmin, geomVmax);
  h2dgoodhits->SetBins(NofPixelInRaw, -NofPixelInRaw*PixelSizeU/2.0,    NofPixelInRaw*PixelSizeU/2.0,
                       NofPixelInColumn, -NofPixelInColumn*PixelSizeV/2.0, NofPixelInColumn*PixelSizeV/2.0);
  for (Int_t i=0; i<4; i++) {
    h2GoodHitsMult[i]->SetBins( NofPixelInRaw, -NofPixelInRaw*PixelSizeU/2.0,    NofPixelInRaw*PixelSizeU/2.0,
                                NofPixelInColumn, -NofPixelInColumn*PixelSizeV/2.0, NofPixelInColumn*PixelSizeV/2.0);
    h1ProjectionOnYMult[i]->SetBins( NofEffectivePitchV, geomVmin, geomVmax);
  }


  cout << "\n*----------------* SECOND MAIN LOOP *----------------*" << endl;
  cout << "*-     Determination of the spatial resolution      -*\n" << endl;

  NbOfHits = 0;
  totalNOfHits = 0;

  TString TheOutFileName = TString(fWorkingDirectory) + TString("file_out_91893.out");
  std::ofstream file_out(TheOutFileName.Data());
  char ytitle[100];

  for ( Int_t ievt=MinEvent ; ievt<=MaxEvent ; ievt++ ) { // Main loop over event

    if(MimoDebug) Info("MimosaImaging","Reading event %d",ievt);

    t->GetEvent(ievt);

    if(MimoDebug>1) cout << " getting the hits" << endl;
    TClonesArray *Hits   = Evt->GetAuthenticHits()     ; //hits (all planes)
    NbOfHits   = Hits->GetLast()+1; // total # hits over all planes

    numberOfHitsInEvent = 0;
    seedList = new Int_t[NbOfHits];

    //---------------------------------------------------------------
    // Loop over hits BUT keep only the ones in the right plane
    if(MimoDebug>1) cout << " Looping over " << NbOfHits << " hits" << endl;
    for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits

      DAuthenticHit *ahit = (DAuthenticHit*)Hits->At(iHit);
      if( ahit->Hpk != ThePlaneNumber ) continue; // select only hits in DUT

      seedList[iHit] = -1; //init seed index (hit not selected will keep this index)


      if ( fIfImaging ) {

        ImHu = (ahit->HuCG)*TMath::Cos(minTheta*TMath::Pi()/180.0) + (ahit->HvCG)*TMath::Sin(minTheta*TMath::Pi()/180.0);
        ImHv = -(ahit->HuCG)*TMath::Sin(minTheta*TMath::Pi()/180.0) + (ahit->HvCG)*TMath::Cos(minTheta*TMath::Pi()/180.0);


        if ( (ImHu >= geomUmin) && (ImHu <= geomUmax) && (ImHv >= geomVmin) && (ImHv <= geomVmax) ) { // cut over the region of interest


          if( ( ahit->Hq0 >= CUT_Q_seed ) // JB 2013/11/08
            && ( MinNofPixelsInCluster <= ahit->HNNS && ahit->HNNS <= MaxNofPixelsInCluster ) // JB 2013/09/11
            && ( CUT_MinSeedIndex==CUT_MaxSeedIndex ||
                (CUT_MinSeedIndex<=ahit->Hsk && ahit->Hsk<=CUT_MaxSeedIndex) ) // allows to select an index range for the seed pixel (ineffective if minIndex==maxIndex), JB 2013/08/21
            && ( CUT_MinSeedCol==CUT_MaxSeedCol ||
                (CUT_MinSeedCol<=(ahit->Hsk%NofPixelInRaw) && (ahit->Hsk%NofPixelInRaw)<=CUT_MaxSeedCol) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
            && ( CUT_MinSeedRow==CUT_MaxSeedRow ||
                (CUT_MinSeedRow<=(ahit->Hsk/NofPixelInRaw) && (ahit->Hsk/NofPixelInRaw)<=CUT_MaxSeedRow) ) // allows to select an col range for the seed pixel (ineffective if minCol==maxCol), JB 2013/08/22
            && ( CUT_MinNbOfHits<=NbOfHits && NbOfHits<=CUT_MaxNbOfHits )
            && ( ahit->Hqc >= CUT_Q_cluster ) // JB 2014/01/21
          ) {

            seedList[iHit] = IndexofPix[0];
            NofPixelsInCluster = (ahit->HNNS < MaxNofPixelsInCluster)?ahit->HNNS:MaxNofPixelsInCluster;


            ClusterCharges_compute( ahit );
            ClusterPosition_compute( ahit, alignement );


            BinarySpecific_HitsOnly_fill( ahit );
            BinarySpecific_fill( ahit );
            ClusterCharges_fill( ahit, ievt );
            ClusterPosition_fill( ahit );
            ClusterShape_fill( ahit );

            Int_t multiplicity = ahit->HNNS;
            GoodHit_Fill( ahit );
            ProjectionImaging_Fill( ahit, minTheta );
            totalNOfHits++;
            numberOfHitsInEvent++;

            TString command;
            sprintf(ytitle,"%.10f",ImHu);
            command  = TString(ytitle) + TString("    ");
            sprintf(ytitle,"%.10f",ImHv);
            command += TString(ytitle) + TString("    ");
            sprintf(ytitle,"%d",multiplicity);
            command += TString(ytitle);
            file_out << command.Data() << std::endl;


	    h2DXprimeVsYprime->Fill(ImHu,ImHv);

          } // end CUTS

        } // end cut over the region of interest

      } // end ifImaging

    } // end loop on hits

    hnGOODhit->Fill( numberOfHitsInEvent );

    delete seedList;
    if ( ievt/10000.0 == int(ievt/10000.0) && ievt != 0 ) cout << "Number of studied events: " << ievt << endl;

  } // end Second main loop over event

  file_out.close();

  //---------------------------------------------------------------
  // Call final part of analysis here
  ClusterPosition_end();
  ClusterCharges_end();
  ClusterShape_end();
  BinarySpecific_end();
  ProjectionImaging_end( totalNOfHits );

  // **********************************************************************************
  // **************************** END OF SECOND MAIN LOOP *****************************
  // **********************************************************************************


  // **********************************************************************************
  // ****************************** THIRD MAIN PART ***********************************
  // Goal: fit of the data projection on x, depending on different multiplicities
  //
  // **********************************************************************************
  fMimosaImagingDone=kTRUE;
  fClearDone=kFALSE;

  PreparePost();
  CheckImaging( minTheta, Npeak, S, W, FirstLoop, chooseFit );
}
