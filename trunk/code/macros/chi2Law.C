{
  // parameters:
  // [0] = normalisation
  // [1] = # degrees of freedom
  //
  // JB 2009/09/11

  TF1 chi2("chi2","[0]*pow(x,[1]/2-1)*exp(-x/2)/pow(2,[1]/2)/TMath::Gamma([1]/2)",0, 100);
  chi2.SetParameters(10000,5);
}
