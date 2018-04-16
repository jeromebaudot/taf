#define _FILE_OFFSET_BITS 64
#include "TMath.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TFile.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
#include <iostream>
#include "TTree.h"
#include "DEvent.h"

void frames2textfile(  Int_t nEvents=500, Int_t nCumulFrames=100, Int_t nPixelsU=512, Int_t nPixelsV=512, Int_t cutSeed=25, Short_t debug=0){

  // Create superframes from hit positions
  //
  // Launch taf, then:
  //TAF> TFile f("datDSF/run1007_01.root")
  //TAF> .L ./code/macros/frames2textfile.C
  //TAF> frames2textfile(100, 10, 512, 512)
  //
  // JB 2017/11/9
  
  // Get TTree and nb of events to read
  TTree *t1 = (TTree*)gROOT->FindObject("T");
  DEvent *Evt = new DEvent() ;
  TBranch *branch = t1->GetBranch("fEvent");
  branch->SetAddress(&Evt);
  nEvents = (nEvents==0)?t1->GetEntries():TMath::Min(nEvents,(Int_t)t1->GetEntries());
  cout << "Entries in the tree: " << t1->GetEntries();
  cout << ", will read " << nEvents << endl;
  
  // Init variables and output file
  TH2F *hFrame = new TH2F("hframe", "frame", nPixelsU, 0, nPixelsU, nPixelsV, 0, nPixelsV);
  Int_t previousFrame = -1;
  Int_t frameCounter = 0;
  Int_t hitCounter = 0;
  Int_t superFrameCounter = 0;
  Int_t totalFrameCounter = 0;
  TClonesArray *Hits;
  DAuthenticHit *ahit;
  Int_t aIndex;
//  Int_t indexes[400];
//  Int_t charges[400];
  
  FILE *txtFile;
  txtFile = fopen("mimosa.txt","w");

  
  // MAIN LOOP
  for ( int loop2 = 0 ; loop2 < nEvents ; loop2++){ // loop on tree entries
    t1->GetEntry(loop2);
    totalFrameCounter++;
    if( previousFrame < Evt->fEvt ) {
      frameCounter++;
      previousFrame = Evt->fEvt;
    }
    Hits   = Evt->GetAuthenticHits();
    Int_t NbOfHits = Hits->GetLast()+1;
    hitCounter += NbOfHits;
    if(debug) cout << "Event " << previousFrame << " with " << NbOfHits << " hits  (current fCount = " << frameCounter << ")" << endl;
    
    // If enough frames added in superFrame, generate a new superFrame
    if ( frameCounter >= nCumulFrames ) {
      printf("** SuperFrame %d completed with %d frame, including %d hits. Current event is %d/%d\n", superFrameCounter, frameCounter, hitCounter, loop2, totalFrameCounter);
      //      txtFile = fopen("mimosa.txt","a");
      for ( Int_t iBinX=1; iBinX<=hFrame->GetNbinsX(); iBinX++) {
        for ( Int_t iBinY=1; iBinY<=hFrame->GetNbinsY(); iBinY++) {
          fprintf( txtFile, "%d\t", (Int_t)hFrame->GetBinContent( iBinX, iBinY) );
        }
        fprintf( txtFile, "\n");
      }
      //      fclose(txtFile);
      hFrame->Reset();
      frameCounter = 0;
      hitCounter = 0;
      superFrameCounter++;
    }
    
    
    // Loop on hits in this
    for (Int_t iHit=0 ; iHit<NbOfHits ; iHit++){ // loop on hits
      ahit = (DAuthenticHit*)Hits->At(iHit);
      if(debug) printf("      found hit %d with %d pixels and Qseed=%d (<?> %d cut)\n", iHit, ahit->HNNS, ahit->Hq0, cutSeed);

      if( cutSeed < ahit->Hq0 ) { // check seed charge is large enough
	
	Int_t *indexes = ahit->HkM;
	Float_t *charges = ahit->HqM;
	for( Int_t iStrip=0; iStrip<=ahit->HNNS; iStrip++ ) {
//          aIndex = ahit->HkM[iStrip];
	  aIndex = indexes[iStrip];
	  hFrame->Fill( aIndex%nPixelsU, aIndex/nPixelsU, charges[iStrip]);
	  if( debug>1) printf("        strip[%d,%d] = %d\n", aIndex%nPixelsU, aIndex/nPixelsU, charges[iStrip]);
	}
	
      } // end if seed charge large enough
      
    } //end loop on hits

    
  } // end loop on tree entries
  
  fclose(txtFile);

  printf("\n File mimosa.txt saved after %d frames in total read and %d superframes generated.\n", totalFrameCounter, superFrameCounter);  
}
