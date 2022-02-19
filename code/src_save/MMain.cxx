// @(#)maf/maf:$Name:  $:$Id: MMain.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai
// Last Modified: VR 2014/03/30 completly rewritten for a new welcome message (easier configuration) ; for being arguments complient ; and for helpŝ
// Last Modified: VR 2014/06/29 add the args corresponding to MRaw and MRax GUIs to launch them
// Last Modified: VR 2014/06/30 support of config file parameter
// Last Modified: JB 2016/08/17 support of config directory parameter

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Mimosa Analysis Framework - main source file           //
  //                                                         //
  //                                                         //
  ////////////////////////////////////////////////////////////


#include "TRint.h"
#include "TError.h"
#include "TEnv.h"
#include "TInterpreter.h"

#ifndef _MimosaAnalysis_included_
  #include "MAnalysis.h"
#endif

#ifndef MAC
  #include "TSystem.h" // to comment for MAC
#endif


//______________________________________________________________________________
//
Int_t main(Int_t argc, Char_t **argv)
{
  //--------------------------------------------------
  //        Parameters:
  //--------------------------------------------------
  //Verbose mode (display infos)
  Bool_t verbose=kTRUE;
  //Bool_t verbose=kFALSE;

  // Debug :
  //cout << argc << endl;
  //for( Int_t i=0 ; i<argc ; i++) cout << argv[i] << endl;

// TAF arguments processing
  //--------------------------------------------------
  //        Declaration and default values:
  //--------------------------------------------------
  //**********************************
  // 'classics' root arguments 'emulation'
  // (taf executable DO NOT accept root 'classics' args like -b by default cause root args are disabled, see comments below about new TRint("TAF", NULL, NULL, NULL, 0); )
  //**********************************
  // batch mode "-b"
  Bool_t  tafrootargs_batch_bool = kFALSE;
  // exit when finished "-q"
  Bool_t  tafrootargs_quit_bool = kFALSE;
  // do not display welcome message "-l"
  Bool_t  tafrootargs_nowe_bool = kFALSE;
  // execute a macro file.C
  TString tafrootargs_execmacro_file = "";
  Bool_t  tafrootargs_execmacro_bool = kFALSE;
  //**********************************
  // taf initialisation
  //**********************************
  // Create [or NOT] a MimosaAnalysis *gTAF = new MimosaAnalysis() (made by default with kTRUE)
  TString tafinit_gtaf_cmd  = "-nogtaf";
  Bool_t  tafinit_gtaf_bool = kTRUE; // made by default, cancel with above arg
  // Launch the MRax GUI
  TString tafinit_mraxgui_cmd  = "-gui";
  //Bool_t  tafinit_mraxgui_bool = kFALSE; // not made by default
  //**********************************
  // mimosa analysis session initialisation
  //**********************************
  // for each arg:
  //  -sessinit_XXX_cmd is the keyword that MUST start by "-", that describes the expected arg. (like -run 26110 for the run number)
  //  -sessinit_XXX_arg is the value given that follows the keyword (if done), or "" if not given. MUST BE INITIALIZED to "" ! (see below)
  //  -sessinit_XXX_def is the default value, used if the corresponding parameter is not given (ie if(sessinit_XXX_arg.IsNull()))
  //          - MUST BE DEFINED for the plane number and the eventBM (should be done with InitSession() default values) because InitSession prototype is fixed for compatibility
  //          - Can be ""(empty string, not nothing!) for others, then InitSession() uses its own default values. A value can be set.
  //**********************************
  // run number
  TString sessinit_runnb_cmd = "-run"; // arg catch after this keyword
  TString sessinit_runnb_arg = "";     // init at empty
  TString sessinit_runnb_def = "";     // default value: if ""
  // plane number
  TString sessinit_plane_cmd = "-plane";
  TString sessinit_plane_arg = "";
  TString sessinit_plane_def = "0"; // MUST BE != "", SHOULD BE the same default value as InitSession()
  // event build mode
  TString sessinit_ebm_cmd = "-ebm";
  TString sessinit_ebm_arg = "";
  TString sessinit_ebm_def = "-1"; // MUST BE != "", SHOULD BE the same default value as InitSession()
  // config file
  TString sessinit_cfgFilePath_cmd = "-cfg";
  TString sessinit_cfgFilePath_arg = "";
  TString sessinit_cfgFilePath_def = "";
  // config directory
  TString sessinit_cfgDir_cmd = "-cfgdir";
  TString sessinit_cfgDir_arg = "";
  TString sessinit_cfgDir_def = "";
  // results path
  TString sessinit_mainResDirPath_cmd = "-res";
  TString sessinit_mainResDirPath_arg = "";
  TString sessinit_mainResDirPath_def = "";
  // data path :
  TString sessinit_dataDirPath_cmd = "-data";
  TString sessinit_dataDirPath_arg = "";
  TString sessinit_dataDirPath_def = "";
  // output files prefix :
  TString sessinit_outFilesPref_cmd = "-prefix";
  TString sessinit_outFilesPref_arg = "";
  TString sessinit_outFilesPref_def = "";
  // output files suffix :
  TString sessinit_outFilesSuff_cmd = "-suffix";
  TString sessinit_outFilesSuff_arg = "";
  TString sessinit_outFilesSuff_def = "_RUN";
  // debug level
  TString sessinit_debugLevel_cmd = "-debug";
  Int_t   sessinit_debugLevel_arg = 0;
  Int_t   sessinit_debugLevel_def = 0;
  //**********************************
  // TAF analysis GUIs
  //**********************************
  // Launch the MRaw GUI
  TString tafgui_mraw_cmd  = "-guiw";
  Bool_t  tafgui_mraw_bool = kFALSE; // not made by default
  // Launch the MRax GUI
  TString tafgui_mrax_cmd  = "-guix";
  Bool_t  tafgui_mrax_bool = kFALSE; // not made by default


  //--------------------------------------------------
  //        Reading and analysing arguments
  //--------------------------------------------------
  if (argc > 1) //if args are given
  {
    TString arg;
    arg = argv[1];
    //**********************************
    //   Help asked ?
    //**********************************
    if (!arg.CompareTo("--help") || !arg.CompareTo("-h") || !arg.CompareTo("-?"))
    {
      cout << "TAF arguments help:" << endl;
//
      cout << "  * TAF accepts most ROOT arguments:"<< endl;
      cout << "     [-b] : run in batch mode without graphics" << endl;
      cout << "     [-q] : exit after processing macro file" << endl;
      cout << "     [-l] : do not display welcome message" << endl;
      cout << "     [[/dir/]file.C] : execute the macro in interpreter <only one macro file>" << endl;
      cout << "     not yet supported args: [-n] [dir] [[file:]data.root]" << endl;

      cout << "  * TAF initialisation:" << endl;
      cout << "     ["<<tafinit_gtaf_cmd<<"] : do NOT create a 'MimosaAnalysis* gTAF' (made by default)" << endl;

      cout << "  * TAF debug (could be set after Init):" << endl;
      cout << "     ["<< sessinit_debugLevel_cmd <<"] debugLevel (ex.: 1, 2...), default is "<< sessinit_debugLevel_def << endl;

      cout << "  * gTAF->InitSession(...) can be made automaticaly:" << endl;
//      cout << "     ex: $ TAF [...] " << sessinit_runnb_cmd << " 26112 [" << sessinit_mainResDirPath_cmd << " ./anexperiment/tafresults] [...]" << endl;//TODO
	cout << "     ex: $ TAF [...] " << sessinit_runnb_cmd << " 26112" << endl;
        cout << "      "<< sessinit_runnb_cmd <<"  number of the run to analyse (integer>0), mandatory for auto init session"<<endl;
        cout << "     ["<< sessinit_plane_cmd <<"] number of the plane to analyse (integer>0)" << endl;
        cout << "     ["<< sessinit_ebm_cmd <<"] Event Building Mode (integer)"<< endl;
        cout << "     ["<< sessinit_cfgFilePath_cmd <<"] specific config file (ex.: myconfig.cfg ./mydir/myconfig.cfg /mydir/myconfig.cfg ...)"<< endl;
        cout << "     ["<< sessinit_cfgDir_cmd <<"] specific config directory (ex.: mydir /another/mydir ../anotherdir/mydir/ ...)"<< endl;
        cout << "     ["<< sessinit_outFilesPref_cmd <<"] output files prefix for MRax (ex.: config_yy), default is "<<  sessinit_outFilesPref_def << endl;
        cout << "     ["<< sessinit_outFilesSuff_cmd <<"] output files suffix for MRax (ex.: RUNxx), default is "<<  sessinit_outFilesSuff_def << "#" << endl;
      //      cout << "     ["<< sessinit_mainResDirPath_cmd <<"] path (directory created if not exists) where results dir/files will be created"<< endl;
//      cout << "     ["<< sessinit_dataDirPath_cmd <<"] path of run binary data in folders named 'RUNxxxxx'"<< endl;

      cout << "  * TAF GUIs :" << endl;
      cout << "     ["<<tafgui_mraw_cmd<<"] : launch the MRaw GUI" << endl;
      cout << "     ["<<tafgui_mrax_cmd<<"] : launch the MRax GUI" << endl;

      cout << "  * Help:" << endl;
      cout << "     [-h] [-?] [--help] : displays this help message" << endl;

      cout << endl;
      cout << "TAF Mimosa Analysis help:" << endl;
      cout << "     run 'gTAF->Help()' in TAF to display help" << endl;

      return 0;
    }
    //**********************************
    //   Else, analyse args
    //**********************************
    else
    {
      if(verbose) cout << " ***<Reading arguments>*** " << endl;
      for (Int_t i=1 ; i<argc ; i++)
      {
	arg = argv[i];
	//cout << arg << endl;
	//**********************************
	//   args that start with "-"
	//**********************************
	if (!arg.Index('-')) // if this arg starts with "-"
	{
	  //cout << argv[i] << endl;//debug
	  //**********************************
	  // 'classics' root arguments 'emulation'
	  //**********************************
	  // batch mode
	  if (!arg.CompareTo("-b"))
	  {
	    tafrootargs_batch_bool = kTRUE;
	    if(verbose) cout << "  * ROOT arg: running in batch mode is asked" << endl;
	  }
	  // exit when finished
	  else if (!arg.CompareTo("-q"))
	  {
	    tafrootargs_quit_bool= kTRUE;
	    if(verbose) cout << "  * ROOT arg: exit when finished is asked" << endl;
	  }
	  // do not display the welcome message
	  else if (!arg.CompareTo("-l"))
	  {
	    tafrootargs_nowe_bool = kTRUE;
	    if(verbose) cout << "  * ROOT arg: do not display the welcome message is asked" << endl;
	  }
	  //**********************************
	  // taf initialisation
	  //**********************************
	  else if (!arg.CompareTo(tafinit_gtaf_cmd))
	  {
	    tafinit_gtaf_bool = kFALSE;
	    if(verbose) cout << "  * TAF init: do NOT create a 'MimosaAnalysis* gTAF' is asked" << endl;
	  }
	  //**********************************
	  // mimosa analysis session initialisation : these args are followed by values ! (test (i+1)<argc
	  //**********************************
	  // run number
	  else if (!arg.CompareTo(sessinit_runnb_cmd) && ((i+1)<argc)) // if this arg is followed by another
	  {
	    sessinit_runnb_arg = argv[i+1];
	    if(verbose) cout << "  * InitSession: a run number is given: "<< sessinit_runnb_arg << endl;
	    i++;
	  }
	  // plane number
	  else if (!arg.CompareTo(sessinit_plane_cmd) && ((i+1)<argc)) // if this arg is followed by another
	  {
	    sessinit_plane_arg = argv[i+1];
	    if(verbose) cout << "  * InitSession: a plane is given: "<< sessinit_plane_arg << endl;
	    i++;
	  }
	  // event build mode
	  else if (!arg.CompareTo(sessinit_ebm_cmd) && ((i+1)<argc)) // if this arg is followed by another
	  {
	    sessinit_ebm_arg = argv[i+1];
	    if(verbose) cout << "  * InitSession: an EBM is given: "<< sessinit_ebm_arg << endl;
	    i++;
	  }
	  // config file
	  else if (!arg.CompareTo(sessinit_cfgFilePath_cmd) && ((i+1)<argc)) // if this arg is followed by another
	  {
	    sessinit_cfgFilePath_arg = argv[i+1];
	    if(verbose) cout << "  * InitSession: a config file is given: "<< sessinit_cfgFilePath_arg << endl;
	    i++;
	  }
	  // config directory
	  else if (!arg.CompareTo(sessinit_cfgDir_cmd) && ((i+1)<argc)) // if this arg is followed by another
	  {
	    sessinit_cfgDir_arg = argv[i+1];
	    if(verbose) cout << "  * InitSession: a config directory is given: "<< sessinit_cfgDir_arg << endl;
	    i++;
	  }
	  // results path
	  else if (!arg.CompareTo(sessinit_mainResDirPath_cmd) && ((i+1)<argc)) // if this arg is followed by another
	  {
	    sessinit_mainResDirPath_arg = argv[i+1];
	    if(verbose) cout << "  * InitSession: a results path is given: "<< sessinit_mainResDirPath_arg << endl;
	    i++;
	  }
	  // data path
	  else if (!arg.CompareTo(sessinit_dataDirPath_cmd) && ((i+1)<argc)) // if this arg is followed by another
	  {
	    sessinit_dataDirPath_arg = argv[i+1];
	    if(verbose) cout << "  * InitSession: a data path is given: "<< sessinit_dataDirPath_arg << endl;
	    i++;
	  }
          // output files suffix
          else if (!arg.CompareTo(sessinit_outFilesSuff_cmd) && ((i+1)<argc)) // if this arg is followed by another
          {
            sessinit_outFilesSuff_arg = argv[i+1];
            if(verbose) cout << "  * InitSession: an output file suffix is given: "<< sessinit_outFilesSuff_arg << endl;
            i++;
          }
          // output files prefix
          else if (!arg.CompareTo(sessinit_outFilesPref_cmd) && ((i+1)<argc)) // if this arg is followed by another
          {
            sessinit_outFilesPref_arg = argv[i+1];
            if(verbose) cout << "  * InitSession: an output file prefix is given: "<< sessinit_outFilesPref_arg << endl;
            i++;
          }
    // debug level
          else if (!arg.CompareTo(sessinit_debugLevel_cmd) && ((i+1)<argc)) // if this arg is followed by another
          {
            sessinit_debugLevel_arg = atoi(argv[i+1]);
            if(verbose) cout << "  * InitSession: a debug level is given: "<< sessinit_debugLevel_arg << endl;
            i++;
          }
	  //**********************************
	  // taf analysis GUIs
	  //**********************************
	  else if (!arg.CompareTo(tafgui_mraw_cmd))
	  {
	    tafgui_mraw_bool = kTRUE;
	    if(verbose) cout << "  * TAF GUIs : launching MRaw is asked" << endl;
	  }
	  else if (!arg.CompareTo(tafgui_mrax_cmd))
	  {
	    tafgui_mrax_bool = kTRUE;
	    if(verbose) cout << "  * TAF GUIs : launching MRax is asked" << endl;
	  }
	  //**********************************
	  //   unknow args
	  //**********************************
	  else
	  {
	    cout << "  * ERROR! Unknow argument: "<< arg << " (see taf --help)" << endl;
	    return -1;
	  }
	}
	//**********************************
	//   macro file
	//**********************************
	else if (arg.Index('-') && (arg.EndsWith(".C") || arg.EndsWith(")") ) ) // if this arg DO NOT starts with "-" AND finished with ".C"
	{
	  if(verbose) cout << "  * ROOT arg: a macro to execute is given: " << arg << endl;
	  tafrootargs_execmacro_file = arg;
	  tafrootargs_execmacro_bool = kTRUE;
	}
	//**********************************
	//   unknow args
	//**********************************
	else
	{
	  cout << "  * ERROR! Unknow argument: "<< arg << " (see taf --help)" << endl;
	  return -1;
	}
      }
      if(verbose) cout << " ***</Reading arguments>*** " << endl << endl;
    }
  }

  //--------------------------------------------------
  //        Create TAF Application
  //--------------------------------------------------
  // Managment of arguments given to the executable
  // TRint(const char* appClassName, int* argc, char** argv, void* options = 0, int numOptions = 0, Bool_t noLogo = kFALSE)
  //   When executing taf with args like $ taf arg1 arg2 arg3 ...
  //   2 ways are offered :
  //     1) let root interpreting args he knows
  //        see $ root --help
  //        Usage: root [-l] [-b] [-n] [-q] [dir] [[file:]data.root] [file1.C ... fileN.C]
  //        then, argc counts args root didn't understood (accessible via argv)
  //        BUT impossible to give a path or file as arg cause root will "catch" this arg!
  //TRint* theApp = new TRint("TAF", &argc, argv, 0, 0, kFALSE);
  //     2) ask root to do nothing with args
  //        then, all args are accessible via argc argv
  //	    an 'emulation' of root'like args is made
  TRint* theApp = new TRint("TAF", NULL, NULL, 0, 0, tafrootargs_nowe_bool);

// Some configurations
  theApp->SetPrompt("TAF> ");
  gErrorAbortLevel=kError;
  gEnv->SetValue("Root.Stacktrace", 0);
  // syncronize X server to avoid prob
  //  gEnv->SetValue("X11.Sync", 1);
  //  gEnv->SetValue("X11.FindBestVisual", 0);

  //--------------------------------------------------
  //        Work that corrersponds to arguments
  //--------------------------------------------------
  Char_t *tafcommand = new Char_t [1000];
  Long_t rvalue;

  if(verbose) cout << endl << " ***<TAF initialisation>*** "<<endl;
  //**********************************
  // 'classics' root arguments 'emulation'
  //**********************************
  if (tafrootargs_batch_bool)
  {
    gROOT->SetBatch();
    if(verbose) cout << " * TAF works in batch mode" << endl;
  }
  if (tafrootargs_quit_bool)
  {
    if(verbose) cout << " * TAF will exit when finished" << endl;
  }
  //**********************************
  // taf initialisation
  //**********************************
  if(tafinit_gtaf_bool || !sessinit_runnb_arg.IsNull())
  {
    sprintf(tafcommand,"MimosaAnalysis *gTAF = new MimosaAnalysis()");
    cout << " * Process command: "<< tafcommand ;
    rvalue = gROOT->ProcessLineSync(tafcommand);
  }

  if(verbose) cout << " ***</TAF initialisation>*** "<<endl;


  //****************************************
  // mimosa analysis session initialisation
  //***************************************
  if (!sessinit_runnb_arg.IsNull()) //if a runnumber is given
  {
    if(verbose) cout << endl << " ***<Automatic Mimosa Analysis Session Initialisation>*** "<<endl;

    // Check that default values for plane number and ebm are NOT empty
    if (sessinit_plane_def.IsNull() || sessinit_ebm_def.IsNull())
    {
      cout << "ERROR: hardcoded default values for plane number or event build mode can't be EMPTY !" << endl;
      return -1;
    }

    if(verbose) cout << " Initialisation parameters :" << endl;
    if(verbose) cout << "  * run number    <given>:   " << sessinit_runnb_arg << endl;
    //------------------------------
    // Plane
    //------------------------------
    if (! sessinit_plane_arg.IsNull()) // if "plane" arg is given
    {
      if(verbose) cout << "  * plane number  <given>:   " << sessinit_plane_arg << endl;
    }
    else // if "plane" arg is NOT given
    {
      sessinit_plane_arg = sessinit_plane_def; // then use default one (defined before)
      if(verbose) cout << "  * plane number  <default>: " << sessinit_plane_arg << endl;
    }
    //------------------------------
    // Event Build Mode
    //------------------------------
    if (! sessinit_ebm_arg.IsNull())
    {
      if(verbose) cout << "  * Ev Build Mode <given>:   " << sessinit_ebm_arg << endl;
    }
    else
    {
      sessinit_ebm_arg = sessinit_ebm_def;
      if(verbose) cout << "  * Ev Build Mode <default>: " << sessinit_ebm_arg << endl;
    }
    //------------------------------
    // Config file
    //------------------------------

    if (! sessinit_cfgFilePath_arg.IsNull())
    {
      if(verbose) cout << "  * config file   <given>:   " << sessinit_cfgFilePath_arg << endl;
    }
    else
    {
      sessinit_cfgFilePath_arg = sessinit_cfgFilePath_def;
      if(verbose)
      {
	cout << "  * config file   <default>: ";
	if (sessinit_cfgFilePath_arg.IsNull()) cout << "InitSession() default value (run"<<sessinit_runnb_arg<<".cfg)" << endl;
	else                              cout << sessinit_cfgFilePath_arg << endl;
      }
    }
    //------------------------------
    // Config directory
    //------------------------------

    if (! sessinit_cfgDir_arg.IsNull())
    {
      if(verbose) cout << "  * config directory   <given>:   " << sessinit_cfgDir_arg << endl;
    }
    else
    {
      sessinit_cfgDir_arg = sessinit_cfgDir_def;
      if(verbose)
      {
        cout << "  * config directory   <default>: ";
        if (sessinit_cfgDir_arg.IsNull()) cout << "InitSession() default value (config)" << endl;
        else                              cout << sessinit_cfgDir_arg << endl;
      }
    }
    //------------------------------
    // Output files prefix
    //------------------------------
    if (! sessinit_outFilesPref_arg.IsNull())
    {
      if(verbose) cout << "  * Output files prefix <given>:   " << sessinit_outFilesPref_arg << endl;
    }
    else
    {
      sessinit_outFilesPref_arg = sessinit_outFilesPref_def;
      if(verbose) cout << "  * Output files prefix <default>: " << sessinit_outFilesPref_arg << endl;
    }
    //------------------------------
    // Output files suffix
    //------------------------------
    if (! sessinit_outFilesSuff_arg.IsNull())
    {
      if(verbose) cout << "  * Output files suffix <given>:   " << sessinit_outFilesSuff_arg << endl;
    }
    else
    {
      sessinit_outFilesSuff_arg = sessinit_outFilesSuff_def;
      sessinit_outFilesSuff_arg += sessinit_runnb_arg;
      if(verbose) cout << "  * Output files suffix <default>: " << sessinit_outFilesSuff_arg << endl;
    }
    //------------------------------
    // Debug level
    //------------------------------
    if ( sessinit_debugLevel_arg )
    {
      if(verbose) cout << "  * Debug level <given>:   " << sessinit_debugLevel_arg << endl;
    }
    else
    {
      sessinit_debugLevel_arg = sessinit_debugLevel_def;
      if(verbose) cout << "  * Debug level <default>: " << sessinit_debugLevel_arg << endl;
    }
    //------------------------------
    // Result path //TODO
    //------------------------------
    /*
    if (! sessinit_mainResDirPath_arg.IsNull())
    {
      if(verbose) cout << "  * result path   <given>:   " << sessinit_mainResDirPath_arg << endl;
    }
    else
    {
      sessinit_mainResDirPath_arg = sessinit_mainResDirPath_def;
      if(verbose)
      {
	cout << "  * result path   <default>: ";
	if (sessinit_mainResDirPath_arg.IsNull()) cout << "InitSession() default value" << endl;
	else                              cout << sessinit_mainResDirPath_arg << endl;
      }
    }
    */
    //------------------------------
    // Data Path //TODO
    //------------------------------
    /*
    if (! sessinit_dataDirPath_arg.IsNull()) // if "data path" arg is given
    {
      if(verbose) cout << "  * data path     <given>:   " << sessinit_dataDirPath_arg << endl;
    }
    else // if "data path" arg is NOT given
    {
      sessinit_dataDirPath_arg = sessinit_dataDirPath_def; // then use default one (defined before)
      if(verbose)
      {
	cout << "  * data path     <default>: ";
	if (sessinit_dataDirPath_arg.IsNull()) cout << "InitSession() default value" << endl; // if this default value is "", use InitSession() default value
	else                               cout << sessinit_dataDirPath_arg << endl; // if this default value is NOT "", use it
      }
    }
   */
    //------------------------------
    // InitSession
    //------------------------------
    /*
    sprintf(tafcommand, "gTAF->InitSession(%s,%s,%s,\"%s\",\"%s\",\"%s\")",\
	  sessinit_runnb_arg          .Data(),\
	  sessinit_plane_arg          .Data(),\
	  sessinit_ebm_arg            .Data(),\
	  sessinit_cfgFilePath_arg    .Data());
    */

    sprintf(tafcommand, "gTAF->SetDebug(%d)", sessinit_debugLevel_arg);
    cout << " * Process command: "<< tafcommand << endl << endl;
    rvalue = gROOT->ProcessLineSync(tafcommand);

    sprintf(tafcommand, "gTAF->InitSession(%s,%s,%s,\"%s\",\"%s\")",\
	  sessinit_runnb_arg          .Data(),\
	  sessinit_plane_arg          .Data(),\
	  sessinit_ebm_arg            .Data(),\
	  sessinit_cfgFilePath_arg    .Data(),\
	  sessinit_cfgDir_arg         .Data());
    cout << " * Process command: "<< tafcommand << endl << endl;
    rvalue = gROOT->ProcessLineSync(tafcommand);
    //cout << "  return " << rvalue << endl;

    sprintf(tafcommand, "gTAF->GetSession()->SetOutputFilesPrefix(\"%s\")",sessinit_outFilesPref_arg.Data());
    cout << " * Process command: "<< tafcommand << endl << endl;
    rvalue = gROOT->ProcessLineSync(tafcommand);

    sprintf(tafcommand, "gTAF->GetSession()->SetOutputFilesSuffix(\"%s\")",sessinit_outFilesSuff_arg.Data());
    cout << " * Process command: "<< tafcommand << endl << endl;
    rvalue = gROOT->ProcessLineSync(tafcommand);

    if(verbose) cout << " ***</Automatic Mimosa Analysis Session Initialisation>*** " <<endl;

    //------------------------------
    // launch GUIs
    //------------------------------
    if (tafgui_mraw_bool)
    {
      if(verbose) cout << endl << " ***<Launching MRaw GUI>*** "<<endl;
      sprintf(tafcommand, "gTAF->GetRaw()");
      cout << " * Process command: "<< tafcommand << endl << endl;
      rvalue = gROOT->ProcessLineSync(tafcommand);
    }

    if (tafgui_mrax_bool)
    {
      if(verbose) cout << endl << " ***<Launching MRax GUI>*** "<<endl;
      sprintf(tafcommand, "gTAF->GetRax()");
      cout << " * Process command: "<< tafcommand << endl << endl;
      rvalue = gROOT->ProcessLineSync(tafcommand);
    }
  }


  //**********************************
  // execute the given macro
  //**********************************
  Int_t returnmacro = 0;
  if (tafrootargs_execmacro_bool)
  {
    if(verbose) cout << endl << " ***<Macro execution>*** "<<endl;
    if(verbose) cout << " * Macro: " << tafrootargs_execmacro_file << endl;
    //cout << gROOT->GetMacroPath() << endl;
    returnmacro = (Int_t) gROOT->Macro(tafrootargs_execmacro_file.Data());
    //theApp->ProcessFile(tafrootargs_execmacro_file.Data());

    if(verbose) cout << " ***</macro execution>*** "<<endl;
  }

  //****************************************
  // Exit taf when finished if asked
  //***************************************
  if (tafrootargs_quit_bool)
  {
    if(verbose) cout << endl <<" TAF will now exit (asked with '-q' arg)" << endl;
    gSystem->Exit(0);
  }



  theApp->Run();
  return 0;
}


//______________________________________________________________________________
//
void PrintAlineWithStars(const TString &str, const Int_t & maxLength, const Bool_t &center)
{
// Print on the standard output (cout) a string given in argument, surrounded with "*" at left and right
//  the right "*" is placed according to maxLength argument to generate a beautiful welcome message
//  center option is possible

  TString stringtoprint;
  stringtoprint =  " * ";
  Int_t nbofspaces;

  if (!center)
  {
    stringtoprint += str;
    nbofspaces = maxLength - str.Length();
    for (Int_t i =0 ; i< nbofspaces ; i++) stringtoprint += " ";
  }
  else
  {
    nbofspaces = (maxLength - str.Length())/2;
    for (Int_t i =0 ; i< nbofspaces ; i++) stringtoprint += " ";
    stringtoprint += str;
    for (Int_t i =0 ; i< nbofspaces ; i++) stringtoprint += " ";
    if ((maxLength - str.Length())%2) stringtoprint += " ";
  }
  stringtoprint +=  " *";
  cout << stringtoprint << endl;
}


//______________________________________________________________________________
//
void TRint::PrintLogo(Bool_t lite)
{

  if(lite) return;

// Print the TAF logo on standard output.
  ///////////////////////////////////////////////////////////
  // Configuration
  ///////////////////////////////////////////////////////////
  // Title of welcome message
 std::vector<TString> title;
  title.push_back("W E L C O M E  to  T A F");
  title.push_back("TAPI Analysis Framework");
  // Build infos
  TString version = " Version : DEV on trunk";
  TString svnurl  = " SVN url : http://sbgtrac.in2p3.fr/svn/CMOS/taf/trunk/";
  TString svnrev  = " SVN rev : ";
  //---------------------------------
  //    svn revision infos :
  // If SVN_REVISION definition is made in the Makefile, choose Option 1;
  // If this definition is disabled in Makefile, choose Option 2;
  svnrev += SVN_REVISION; // Option 1
  //svnrev += "??";       // Option 2
  //---------------------------------

  //  Authors by alphabetic order
 std::vector<TString> authors;
  authors.push_back("J.Baudot(1)");
  authors.push_back("A.Besson(1)");
  authors.push_back("B.Boitrelle(3)");
  authors.push_back("N.Chon-Sen(1)");
  authors.push_back("L.Cousin(1)");
  authors.push_back("R.De Masi(1)");
  authors.push_back("C.Dritsa(1)");
  authors.push_back("M.Gelin(1)");
  authors.push_back("Y.Gornoushkin(1)");
  authors.push_back("D.Grandjean(1)");
  authors.push_back("L.Molnar(1)");
  authors.push_back("G.Orazi(1)");
  authors.push_back("V.Reithinger(2)");
  authors.push_back("S.Senyukov(1)");
  authors.push_back("S.Shabetai(1)");
  authors.push_back("Y.Voutsinas(1)");
  authors.push_back("A. Perez Perez(1)");
  //  Institutions by cited order
 std::vector<TString> institutions;
  institutions.push_back(" (1) IPHC - Strasbourg");
  institutions.push_back(" (2) IPNL - Lyon");
  institutions.push_back(" (3) DESY - Hamburg");


  ///////////////////////////////////////////////////////////
  // Generate the welcome message
  ///////////////////////////////////////////////////////////
  // Generate the strings with authors 3by3) and institutions
 std::vector<TString> authors3by3;
  Int_t nbOfAuthors = authors.size();
  TString tempstring;
  for (Int_t i=0 ; i<nbOfAuthors ; i+=3)
  {
    tempstring =  " ";
    tempstring += authors.at(i);
    if ((i+1)<nbOfAuthors)
    {
      tempstring += ", ";
      tempstring += authors.at(i+1);
    }
    if ((i+2)<nbOfAuthors)
    {
      tempstring += ", ";
      tempstring += authors.at(i+2);
    }
    authors3by3.push_back(tempstring);
  }

  // Calcul max length of a line
  Int_t maxLength = 0; //max length (in char) of a line
  for (Int_t i=0 ; i<int(title.size()); i++) if (title.at(i).Length()     > maxLength) maxLength = title.at(i).Length();
  for (Int_t i=0 ; i<int(authors3by3.size()) ; i++) if (authors3by3.at(i).Length() > maxLength) maxLength = authors3by3.at(i).Length();
  if (version.Length() > maxLength) maxLength = version.Length();
  if (svnurl.Length()  > maxLength) maxLength = svnurl.Length();
  if (svnrev.Length()  > maxLength) maxLength = svnrev.Length();
  TString linestars = " ";
  for (Int_t i=0 ; i<maxLength+4 ; i++) linestars+="*";

  ///////////////////////////////////////////////////////////
  // Print the welcome message
  ///////////////////////////////////////////////////////////
  cout << linestars << endl;
  for (Int_t i=0 ; i<int(title.size()); i++)
  {
    PrintAlineWithStars(title.at(i),maxLength,kTRUE);
  }
  PrintAlineWithStars("",maxLength,kFALSE);

  PrintAlineWithStars("Build info:",maxLength,kFALSE);
  PrintAlineWithStars(version,maxLength,kFALSE);
  PrintAlineWithStars(svnurl,maxLength,kFALSE);
  PrintAlineWithStars(svnrev,maxLength,kFALSE);
  TString rootver = " ROOT ver: ";rootver += gROOT->GetVersion();
  PrintAlineWithStars(rootver,maxLength,kFALSE);

  #ifdef R__UNIX
  if (!strcmp(gVirtualX->GetName(), "X11TTF"))
  {
    PrintAlineWithStars(" FreeType Engine v1.x used to render TrueType fonts",maxLength,kFALSE);
  }
  #endif
  #ifdef _REENTRANT
  PrintAlineWithStars(" Compiled with thread support",maxLength,kFALSE);
  #endif
  PrintAlineWithStars("",maxLength,kFALSE);

  PrintAlineWithStars("Authors:",maxLength,kFALSE);
  for (Int_t i=0 ; i<int(authors3by3.size()); i++)
  {
    PrintAlineWithStars(authors3by3.at(i),maxLength,kFALSE);
  }
  PrintAlineWithStars("Institutions:",maxLength,kFALSE);
  for (Int_t i=0 ; i<int(institutions.size()) ; i++)
  {
    PrintAlineWithStars(institutions.at(i),maxLength,kFALSE);
  }
  PrintAlineWithStars("",maxLength,kFALSE);

  PrintAlineWithStars("Help/Tips:",maxLength,kFALSE);
  PrintAlineWithStars(" run '$TAF --help'",maxLength,kFALSE);
  PrintAlineWithStars("",maxLength,kFALSE);

  cout << linestars << endl;


  #ifdef R__UNIX
#if 0
  //Popdown X logo, only if started with -splash option
  for (Int_t i = 0; i < Argc(); i++) \
  if (!strcmp(Argv(i), "-splash")) kill(getppid(), SIGUSR1); // Commented by JB 2009/04/30 because kill unknown
#endif
  #endif
}
