/////////////////////////////////////////////
//                                         //
//    PXI DAQ LIB VERSION configuration    //
//                                         //
/////////////////////////////////////////////
// Created by VR on March 20th, 2014
/////////////////////////////////////////////

//**********************************************************************
// 1) Choose the version of the library 
//    Define ONE of the two following directives
//**********************************************************************
//#define PXI_DAQ_LIB_VERSION_1_1 //for version pxi_daq_lib_v.1.1 (M-26/28)
//#define PXI_DAQ_LIB_VERSION_1_2 //for version pxi_daq_lib_v.1.2 (M-26/28)
//#define PXI_DAQ_LIB_VERSION_2_1 //for version pxi_daq_lib_v.2.1 (FSBB)
#define PXI_DAQ_LIB_VERSION_3_1 //for version pxi_daq_lib_v.3.1 (M-26/28 and FSBB)

////////////////////////////////////////////
// Some tests to avoid configuration ERROR :
//         DO NOT MODIFY
////////////////////////////////////////////
#ifdef PXI_DAQ_LIB_VERSION_1_1
  #ifdef PXI_DAQ_LIB_VERSION_1_2
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_2_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_3_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
#endif

#ifdef PXI_DAQ_LIB_VERSION_1_2
  #ifdef PXI_DAQ_LIB_VERSION_1_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_2_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_3_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
#endif

#ifdef PXI_DAQ_LIB_VERSION_2_1
  #ifdef PXI_DAQ_LIB_VERSION_1_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_1_2
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_3_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
#endif

#ifdef PXI_DAQ_LIB_VERSION_3_1
  #ifdef PXI_DAQ_LIB_VERSION_1_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_1_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
  #ifdef PXI_DAQ_LIB_VERSION_2_1
    #error in file pxi_daq_lib_config.h => Only one PXI_DAQ_LIB_VERSION can be enabled !!!
  #endif
#endif

////////////////////////////////////////////


//**********************************************************************
// 2) Configure the library 
//   (don't mind the lib parameters you are not using)
//**********************************************************************
//******************
//  lib version 1.1 
//******************
#ifdef PXI_DAQ_LIB_VERSION_1_1
  // The variable FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE is the maximum number of blocs in variable bloc size mode
  // Define ONE of the two following directives (to use 50.000 or 200.000) as function of your DAQ version
    
  #define FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE 50000 // For DAQ version before 2012/11/20
  //#define FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE 200000 // For DAQ version after 2012/11/20
#endif

//******************
//  lib version 1.2 
//******************
#ifdef PXI_DAQ_LIB_VERSION_1_2
  // STEP 1/2 :
  // Define ONE of the three following directives
  // => Check run files date in order to decide which one must be used
  //#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
  //#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
  #define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613

  // STEP 2/2 :
  // Define one of the two followong directives
  // => Check run files date in order to decide which one must be used
  //#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
  #define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012
#endif


//******************
//  lib version 2.1 
//******************
#ifdef PXI_DAQ_LIB_VERSION_2_1
// STEP 1/2 :
// Define ONE of the three following directives
// => Check run files date in order to decide which one must be used
//#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
//#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613

// STEP 2/2 :
// Define one of the two followong directives
// => Check run files date in order to decide which one must be used
//#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012

#endif

//******************
//  lib version 3.1 
//******************
#ifdef PXI_DAQ_LIB_VERSION_3_1
// STEP 1/2 :
// Define ONE of the three following directives
// => Check run files date in order to decide which one must be used
//#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
//#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613

// STEP 2/2 :
// Define one of the two followong directives
// => Check run files date in order to decide which one must be used
//#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
#define APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012

#endif

//**********************************************************************
// 3) Recompile TAF !!! AFTER a 'make clean' command !!!
//**********************************************************************




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              CONFIGS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    Group    //     User(s)     //     Experiment description   // PXI_DAQ_LIB_VERSION  //                sub-config                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IPNL (FR)   // Valerian        // 2012/08 GSI 8*Mi26 PXIe      // 1_2                  // CC1_BEFORE_071112 ; CC2_BEFORE_JULY_2012           //
//             //                 //                              //                      //                                                    //
//             //                 //                              //                      //                                                    //
//             //                 //                              //                      //                                                    //
//             //                 //                              //                      //                                                    //
//             //                 //                              //                      //                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
