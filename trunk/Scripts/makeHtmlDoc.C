void makeHtmlDoc( Bool_t ifwindows=kFALSE){
  
  // Macros to generate the HTML documentation
  //
  // Usage: root Scripts/compiltaf.C
  //  then: root> .x Scripts/makeHtmlDoc.C
  //
  // WARNING: still a lot of error messages...
  //
  // created JB 2011/07/20
  // updated JB 2012/04/25 to include DIGBoardReader
  // updated JB 2013/02/11 to include various new classes
  // updated JB 2014/08/29 to include new Decoder and DXRayPdf classes
  
  TString dirSep;
  
  // WINDOWS settings
  if( ifwindows) {
    dirSep = "\\";
  }
  
  // LINUX/MAC settings
  else {
    dirSep = "/"; // set to "/" for LINUX or to "\\" for WINDOWS
  }
  
  TString dtDir = gSystem->pwd();
  TString docDir = dtDir+dirSep+"doc";
  TString rootSys = gSystem->Getenv("ROOTSYS");
  TString inputDir = dtDir+dirSep+"code"+dirSep+"src:"+dtDir+dirSep+"code"+dirSep+"include";
  
  cout << endl << "****** Generating the HTML documentation in " << docDir.Data() << endl;
  cout << "  access by browsing the page ClassIndex.html" << endl << endl;
  
  THtml html;
  html.SetProductName( "TAF");
  html.SetOutputDir( docDir );
  html.SetInputDir( inputDir );
  
  html.MakeClass("DGlobalTools");
  html.MakeClass("TNTBoardReader");
  html.MakeClass("PXIBoardReader");
  html.MakeClass("PXIeBoardReader");
  html.MakeClass("GIGBoardReader"); // 2012/04/25
  html.MakeClass("IMGBoardReader"); // 2013/02/11
  html.MakeClass("BoardReader"); // 2014/08/29
  html.MakeClass("VMEBoardReader"); // 2014/08/29
  html.MakeClass("AliMIMOSA22RawStreamVASingle"); // 2014/08/29
  html.MakeClass("DecoderM18"); // 2014/08/29
  html.MakeClass("DR3");
  html.MakeClass("DLine");
  html.MakeClass("DParticle");
  html.MakeClass("DCut");
  html.MakeClass("DSetup");
  html.MakeClass("DPixel");
  html.MakeClass("DStrip");
  html.MakeClass("DHit");
  html.MakeClass("DTrack");
  html.MakeClass("DEvent");
  html.MakeClass("DEventHeader");
  html.MakeClass("DAuthenticHit");
  html.MakeClass("DAuthenticPlane");
  html.MakeClass("DTransparentPlane");
  html.MakeClass("DTracker");
  html.MakeClass("DSession");
  html.MakeClass("DAlign");
  html.MakeClass("DPrecAlign");
  html.MakeClass("DAcq");
  html.MakeClass("DPlane");
  html.MakeClass("DLadder"); // 2013/02/11
  html.MakeClass("DMonteCarlo"); // 2013/02/11
  html.MakeClass("MHist");
  html.MakeClass("MRaw");
  html.MakeClass("MRax");
  html.MakeClass("MMillepede"); // 2013/02/11
  html.MakeClass("MimosaAlignAnalysis");
  html.MakeClass("MimosaAnalysis");
  html.MakeClass("DXRay2DPdf"); // 2014/07/28
  
  //html.MakeAll();

}
