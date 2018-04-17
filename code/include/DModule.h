#ifndef _DModule_included_
#define _DModule_included_


  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  // Class Description of DModule                                               //
  //                                                                            //
  // + DModule, may be used in future (>980322) to replace parts of DAcqModule  //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////

// ROOT classes
#include "TObject.h"
#include "TArrayF.h"
#include "TArrayC.h"
#include "TString.h"

class DModule : public TObject {

 private:
  Bool_t       fIsConnected;         // flags initialization status of module
  Int_t        fValuesN;             // number of values in data array
  TArrayF     *fData;                // this is the converted data

  TString      fName;                // Name of this Module 
  TString      fTitle;               // Title of this Module
  TArrayC     *fLocation;            // start of rawvalues in Acquisition buffer
  Int_t        fCodingValuesN;       // number of channels in one input
  Int_t        fCodingBitsN;         // number of Bits decoding value in channel
  Int_t        fSplitInputsN;        // number of splitted inputs (e.g. 4 in Sirocco type A and 2 in B)
  Int_t        fSplitInput;          // a CodingValue is split according to inputs of this module

  Int_t        theValue(const Char_t *aLocation, Int_t aWidth);


 public:
  DModule(); 
  DModule(Int_t aValuesN);
  ~DModule();
  void         PrintDataToTerm();
  void         SetCodeTable(const Char_t *aLocation,
			    Int_t aCodingValuesN, 
			    Int_t aCodingBitsN,
			    Int_t aSplitInputsN,
			    Int_t aSplitInput);
  void         Update();
  void         SetName(Text_t *aName)   { fName  = aName;  }
  void         SetTitle(Text_t *aTitle) { fTitle = aTitle; } 

  TArrayF     *GetData()             { return  fData;             }
  Int_t        GetCodingValuesN()    { return  fCodingValuesN;    }
  Int_t        GetCodingBitsN()      { return  fCodingBitsN;      }
  Int_t        GetSplitInput()       { return  fSplitInput;       }
  Int_t        GetSplitInputsN()     { return  fSplitInputsN;     }

  const char*      GetName() const            { return  (const char*)fName;             }
  const char*      GetTitle() const           { return  (const char*)fTitle;            }

  
  ClassDef(DModule,1)                // Module like e.g. a Sirocco
};

#endif










