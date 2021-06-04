
/**
* ----------------------------------------------------------------------------------
* \file              X:\prj\win\mimosis_1\run_read_light_bt\com\var_decl_def.h
* \brief Goal      : Macros for selection between variables creation, export
* \brief
* \brief
* \version         : 1.0
* \date Prj date   : 12/05/2021
* \date File date  : 12/05/2021
* \date Doc date   : 12/05/2021
* \author          : Gilles CLAUS
* \author          : gilles.claus@iphc.cnrs.fr
* \author          : CNRS - IN2P3 - IPHC 23 Rue du Loess 67037 STRASBOURG
*
* Remark : None
*
* ----------------------------------------------------------------------------------
* License   : GNU General Public License
*
* ----------------------------------------------------------------------------------
*/




// -----------------------------------------------------------------------
// Macros for variable definition / declaration
// -----------------------------------------------------------------------


#ifdef CC_INTERFACE_FILE

  #define EXTERN
  #define VAR_STATIC
  #define VAR_INIT(x) =x
  #define VAR_INIT_A2(x,y) ={x,y}

#else

  #define EXTERN
  #define VAR_STATIC
  #define VAR_INIT(x) =x
  #define VAR_INIT_A2(x,y) ={x,y}


#endif

#ifdef APP_ROOT
  #define VAR_DCL(Extern,Static,Var) Var;
  #define VAR_DCL_INIT(Extern,Static,Var,Init) Var=Init;
  #define VAR_DCL_INIT_A2(Extern,Static,Var,Init0,Init1) Var={Init0,Init1};
#else
  #define VAR_DCL(Extern,Static,Var) Extern Static Var;
  #define VAR_DCL_INIT(Extern,Static,Var,Init) Extern Static Var VAR_INIT (Init);
  #define VAR_DCL_INIT_A2(Extern,Static,Var,Init0,Init1) Extern Static Var VAR_INIT_A2 (Init0,Init1);
#endif
