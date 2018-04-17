void compiltaf( Bool_t ifwindows=kFALSE, Bool_t ifclean=kFALSE, Bool_t verbose==kFALSE){

  // This macro compiles TAF inline and load libraries
  // If successfull, you can then use the pointer gTAF to work
  //
  // Option 1 (ifWindows) : set to 1 when under WINDOWS, default (0) is for LINUX/MAC-OS
  // Option 2 (ifclean)   : set to 1 to clean any previous compilation results before re-compiling
  // Option 1 (verbose)   : set to 1 to get more messages
  //
  // ALWAYS RUN IT FROM THE TAF MAIN DIRECTORY
  // Usage:
  // root Scripts/compiltaf.C        // for default under Linux/Mac-OS
  // root Scripts/compiltaf.C(1)     // for default under Windows
  //
  // JB 2011/03/30
  // Modified: JB 2011/07/08 to compile with Windows
  // Modified: JB 2011/07/11 class PXIeBoardReader added
  // Modified: JB 2012/04/25 class GIGBoardReader added
  // Modified: JB 2013/02/11 various classes added
  // Modified: JB 2014/07/28 class DXRay2DPdf added

  TString dirSep, libEnv;

  // WINDOWS settings
  if( ifwindows) {
    dirSep = "\\";
    libEnv = "LIB";
  }

  // LINUX/MAC settings
  else {
    dirSep = "/"; // set to "/" for LINUX or to "\\" for WINDOWS
    libEnv = "LD_LIBRARY_PATH";
    //TString dtDir = gSystem->Getenv("DTDIR");
  }

  TString dtDir = gSystem->pwd();
  TString rootSys = gSystem->Getenv("ROOTSYS");
  TString includePath = "-I";
  includePath += rootSys+dirSep+"include -I"+dtDir+dirSep+"code"+dirSep+"include";
  gSystem->SetIncludePath( includePath);
  // note the "/" required at the end of the Dir variables
  TString buildDir = dtDir+dirSep+"bin"+dirSep+"TAF"+dirSep+"inlib"+dirSep;
  TString libraryPath = gSystem->Getenv(libEnv.Data());
  libraryPath += buildDir;
  gSystem->Setenv( libEnv.Data(), libraryPath);
  TString srcDir = dtDir+dirSep+"code"+dirSep+"src"+dirSep;
  TString srcSuf(".cxx");
  TString libSuf = ".";
  libSuf += gSystem->GetSoExt();
  TString objSuf = ".";
  objSuf += gSystem->GetObjExt();

  gSystem->mkdir( buildDir, kTRUE); // create the buildDir directory if needed
  gSystem->cd( buildDir); // we compile in the buildDir directory


  if( verbose) {
    cout << endl << "****** COMPILING TAF LIBRARIES *******" << endl << endl;
    cout << " using ROOT v" << gROOT->GetVersion() << endl;
    cout << " from sources in " << srcDir.Data() << endl;
    cout << " with includes as " << gSystem->GetIncludePath() << endl;
    cout << " into build dir " << buildDir.Data() << endl;
    cout << " library path is " << gSystem->Getenv(libEnv.Data()) << endl;
    cout << " object extension is " << objSuf.Data() << endl;
    cout << " library extension is " << libSuf.Data() << endl;
  }

  TString libList[50]; // change array size if more libraries
  Short_t nLibs = 0;

  // order of libs matters, do not change!
  libList[nLibs++] = "DGlobalTools";
  libList[nLibs++] = "TNTBoardReader";
  libList[nLibs++] = "PXIBoardReader";
  libList[nLibs++] = "PXIeBoardReader";
  libList[nLibs++] = "GIGBoardReader";
  libList[nLibs++] = "IMGBoardReader"; // 2013/02/11
  libList[nLibs++] = "BoardReader"; // 2014/05/14
  libList[nLibs++] = "VMEBoardReader"; // 2014/05/14
  libList[nLibs++] = "AliMIMOSA22RawStreamVASingle"; // 2014/05/14
  libList[nLibs++] = "DecoderM18"; // 2014/05/15
  libList[nLibs++] = "DR3";
  libList[nLibs++] = "DLine";
  libList[nLibs++] = "DParticle";
  libList[nLibs++] = "DCut";
  libList[nLibs++] = "DSetup";
  libList[nLibs++] = "DPixel";
  libList[nLibs++] = "DStrip";
  libList[nLibs++] = "DHit";
  libList[nLibs++] = "DTrack";
  libList[nLibs++] = "DEvent";
  libList[nLibs++] = "DTracker";
  libList[nLibs++] = "DSession";
  libList[nLibs++] = "DAlign";
  libList[nLibs++] = "DPrecAlign";
  libList[nLibs++] = "DAcq";
  libList[nLibs++] = "DPlane";
  libList[nLibs++] = "DLadder"; //2013
  libList[nLibs++] = "DMonteCarlo"; // 2013/02/11
  libList[nLibs++] = "MAlign";
  libList[nLibs++] = "MHist";
  libList[nLibs++] = "MRaw";
  libList[nLibs++] = "MRax";
  libList[nLibs++] = "MAnalysis";
  libList[nLibs++] = "MCommands";
  libList[nLibs++] = "MPrep";
  libList[nLibs++] = "MPost";
  libList[nLibs++] = "MMillepede"; // 2013/02/11
  libList[nLibs++] = "MAlignment"; // 2013/11/15
  libList[nLibs++] = "DXRay2DPdf"; // 2014/07/28
  libList[nLibs++] = "DBeaster";  // 2017/03/08
  libList[nLibs++] = "DHelixFitter";  // 2017/07
  libList[nLibs++] = "DHelix";  // 2017/09

  // clean step, if required
  if (ifclean) {
    for( Short_t iLib=0; iLib<nLibs; iLib++) {
      gSystem->Unlink( libList[iLib]+objSuf);
      gSystem->Unlink( libList[iLib]+libSuf);
    }
  }

   // compilation step
  Bool_t success = kTRUE;
  for( Short_t iLib=0; iLib<nLibs; iLib++) P{
    if( !success ) break;
    if(verbose) cout << endl << "Compiling " << libList[iLib] << endl;
    success &= gSystem->CompileMacro( srcDir+libList[iLib]+srcSuf, "kc", libList[iLib]);
  }

  cout << endl;

  // loading step
  for( Short_t iLib=0; iLib<nLibs; iLib++) {
    if( !success ) break;
    if(verbose) cout << "Loading " << libList[iLib] << endl;
    success &= !gSystem->Load( libList[iLib]+libSuf);
  }

  // Chage dir to main dir
  gSystem->cd( dtDir);

  // If success, create the pointer to start
  if( !success ) {
    cout << endl << "*** ERROR ***, Compilation or loading aborted!" << endl << endl;
    return;
  }
  cout << "Start with: MimosaAnalysis *gTAF = new MimosaAnalysis()" << endl;
  cout << "Then type: gTAF->Help() for help." << endl;

}
