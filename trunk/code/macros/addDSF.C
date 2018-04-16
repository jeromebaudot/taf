#include "RConfig.h"
#include <string>
#include "TFile.h"
#include "THashList.h"
#include "TKey.h"
#include "TObjString.h"
#include "Riostream.h"
#include "TClass.h"
#include "TSystem.h"
#include <stdlib.h>

#include "TFileMerger.h"
#include "TROOT.h"
#include "TInterpreter.h"


addDSF()
{
    // clm add DSF files together 2013.08.28
    //
    // How to use: run TAF fisrt, then from TAF .x addDSF.C ...
    // If you see PANIC printout check your DSF files , the merging failed...

    TFileMerger merger(kTRUE,kTRUE);
    merger.OutputFile("datDSF/run34561-62_4hitsPerTrack.root");
    //
    // for(int irun = 38 ; irun <= 42; irun++)
    
    //for(int irun = 34538 ; irun <= 34542; irun++)merger.AddFile(Form("datDSF/run%d_01.root",irun));
    
    Char_t fileName[300];
    //sprintf( fileName, "datDSF/run%d_4hitsPerTrack.root", 34538);
    //cout << "Adding file " << fileName << endl;
    //merger.AddFile(fileName);
    sprintf( fileName, "datDSF/run%d_4hitsPerTrack.root", 34561);
    cout << "Adding file " << fileName << endl;
    merger.AddFile(fileName);
    sprintf( fileName, "datDSF/run%d_4hitsPerTrack.root", 34562);
    cout << "Adding file " << fileName << endl;
    merger.AddFile(fileName);
    /*sprintf( fileName, "datDSF/run%d_4hitsPerTrack_digitizer280.root", 34541);
    cout << "Adding file " << fileName << endl;
    merger.AddFile(fileName);
    sprintf( fileName, "datDSF/run%d_4hitsPerTrack_digitizer280.root", 34542);
    cout << "Adding file " << fileName << endl;
    merger.AddFile(fileName);*/
    //merger.AddFile(Form("datDSF/run%d_4hitsPerTrack_digitizer200.root",34539));
    //merger.AddFile(Form("datDSF/run%d_4hitsPerTrack_digitizer200.root",34541));
    //merger.AddFile(Form("datDSF/run%d_4hitsPerTrack_digitizer200.root",34542));
    
    
    cout << "Now merging..." << endl;
    //
    merger.SetFastMethod(kFALSE);
    //
    Bool_t status = merger.Merge();
    
    if( status == kTRUE ) printf("======> Merging was OK ! \n");
    else printf("======> PANIC PANIC PANIC Merging was NOT OK ! \n");
    
    
}
