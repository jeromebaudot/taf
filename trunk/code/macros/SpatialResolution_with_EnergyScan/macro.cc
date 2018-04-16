#include "TROOT.h"
#include "src/Spatial_Resolution_BeamEScan.hh"
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <assert.h>
#include <TString.h>

using namespace std;

int main(int argc, char *argv[]) {

  const char* DataCard = "SomeMacros/DataCards/DataCard_FSBB_DS_Run35704_DESYJul2015_BeamEScan.txt";

  if(argc == 2) DataCard = argv[1];
  else {
    cout << endl;
    cout << "Wrong number of input parameters" << endl;
    cout << endl;
    assert(false);
  }

  cout << endl;
  cout << "=============================================" << endl;
  cout << "The function arguments are:"                   << endl;
  cout << "---------------------------------------------" << endl;
  cout << "DataCard  = " << DataCard  << endl;
  cout << "=============================================" << endl;
  cout << endl;

  Spatial_Resolution_BeamEScan(DataCard);

  return 0;  

}
