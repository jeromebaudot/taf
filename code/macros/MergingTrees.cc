void MergingTrees(const char* FileList = "List_Mi22THRB7_150kRad_15e11newOcm2_Thr4.5mV.txt",
		  const char* file_out = "datDSF/run3034_3035_3038_01.root")
{

  char ccc1[1000];

  cout << endl;
  cout << "Mergin serveral root file created by DSFProduction:" << endl;

  TChain ch("T");
  ifstream fp(FileList);
  while(fp >> ccc1) {
    cout << "Adding file " << ccc1 << " to the chain!!!" << endl;
    ch.Add(ccc1);
  }
  fp.close();
  cout << "Saving merged TTree in file " << file_out << endl;
  ch.Merge(file_out);
  cout << endl;

  return;

}
