//#include <TText.h>
//#include <TAxis.h>
//#include <TGraph.h>
//#include <TCutG.h>
//#include <TMath.h>
//______________________________________________________________________________
Double_t probCorrectHit(Double_t rho, Double_t sigX, Double_t sigY, Int_t CL = -1){

     Double_t result = 1.;
     Double_t denominator = (2*TMath::Pi()*sigX*sigY*rho + 1.);
     if(CL < 0)
	  result = 1./denominator;
     else{
	  Double_t erfValue = TMath::Erf(CL * sqrt(denominator/2.));
	  result = 1./denominator*erfValue*erfValue;
     }
     return result;
     
}

//______________________________________________________________________________
Double_t calculateSigmaMS(Double_t mass, Double_t energy, Double_t xOverX0){
     Double_t p = TMath::Sqrt(energy*energy - mass*mass);
     Double_t p2 = p*p;
     Double_t beta = p/energy;
     Double_t beta2 = beta*beta;
     Double_t theta2;
     theta2=0.0136*0.0136/(beta2*p2)*TMath::Abs(xOverX0);
     //if (xOverX0>0.001){
       double lt = 1+0.038*TMath::Log(TMath::Abs(xOverX0/beta2));
       if (lt>0) theta2 *= lt*lt;
     //}
     return sqrt(theta2);
}

//______________________________________________________________________________
Int_t Gpalette = 55;//56;//55;/*kRainBow*/ // Rainbow colors
Int_t GetColorFromPalete( Double_t z, Double_t zmin, Double_t zmax, Int_t nBins, Int_t palette = Gpalette){
     // Color setting
     gStyle->SetPalette(palette);

     /// http://root.cern.ch/phpBB3/viewtopic.php?f=12&t=4101&p=16258 -> See THistPainter::PaintColorLevels()
     Int_t ncolors  = gStyle->GetNumberOfColors() ;
     Int_t ndiv = 0;
     if (ndiv == 0 ) { ndiv = gStyle->GetNumberContours() ; }
     if (ndiv == 0) ndiv = nBins;
     Int_t ndivz = TMath::Abs(ndiv) ;
     
     Double_t dz = zmax - zmin ;
     Double_t scale = ndivz/dz ;
     Int_t color = Int_t(0.01+(z-zmin)*scale) ;
     Int_t theColor = Int_t((color+0.99)*Float_t(ncolors)/Float_t(ndivz)) ;
     if (theColor > ncolors-1) theColor = ncolors-1 ;  
     // printf ("\ntheColor=%d",theColor) ;
     return (gStyle->GetColorPalette(theColor)) ;

}

////______________________________________________________________________________
//TF1 *f1, *f2;
//double fIntersections(double *x, double *par){
//     return TMath::Abs(f1->EvalPar(x,par) - f2->EvalPar(x,par));
//}

//______________________________________________________________________________
TGraph* GetIntersections(TGraph &a, TGraph &b)
{
   // Return a TGraph with the points of intersection
   TGraph *interPoint = new TGraph();
   Int_t i = 0;
   
   // Loop over all points in this TGraph
   for(size_t a_i = 0; a_i < a.GetN()-1; ++a_i)
   {
      // Loop over all points in the other TGraph
      for(size_t b_i = 0; b_i < b.GetN()-1; ++b_i)
      {
         
         // Get the current point, and the next point for each of the objects
         Double_t x1, y1, x2, y2 = 0;
         Double_t ax1, ay1, ax2, ay2 = 0;
         a.GetPoint(a_i, x1, y1);
         a.GetPoint(a_i+1, x2, y2);
         b.GetPoint(b_i, ax1, ay1);
         b.GetPoint(b_i+1, ax2, ay2);

         // Calculate the intersection between two straight lines, x axis
         Double_t x = (ax1 *(ay2 *(x1-x2)+x2 * y1 - x1 * y2 )+ ax2 * (ay1 * (-x1+x2)- x2 * y1+x1 * y2)) 
                            / (-(ay1-ay2) * (x1-x2)+(ax1-ax2)* (y1-y2));
                            
         // Calculate the intersection between two straight lines, y axis
         Double_t y = (ax1 * ay2 * (y1-y2)+ax2 * ay1 * (-y1+y2)+(ay1-ay2) * (x2 * y1-x1 * y2))/(-(ay1-ay2) * (x1-x2)+(ax1-ax2) * (y1-y2));

         // Find the tightest interval along the x-axis defined by the four points
         Double_t xrange_min = TMath::Max(TMath::Min(x1, x2), TMath::Min(ax1, ax2));
         Double_t xrange_max = TMath::Min(TMath::Max(x1, x2), TMath::Max(ax1, ax2));

         // If points from the two lines overlap, they are trivially intersecting
         if ((x1 == ax1 && y1 == ay1) || (x2 == ax2 && y2 == ay2)){               
            interPoint->SetPoint(i, (x1 == ax1 && y1 == ay1) ? x1 : x2, (x1 == ax1 && y1 == ay1) ? y1 : y2);
            i++;
            
         } 
         // If the intersection between the two lines is within the tight range, add it to the list of intersections.
         else if(x > xrange_min && x < xrange_max)
         {
            interPoint->SetPoint(i,x, y);
            i++;
         }
      }
   }
   return interPoint;
}

//______________________________________________________________________________
TGraph* GetIntersections(TGraph&a, TCutG &b)
{
   // Return a TGraph with the points of intersection
   TGraph *interPoint = new TGraph();
   Int_t i = 0;

   // Loop over all points in this TGraph
   for(size_t a_i = 0; a_i < a.GetN()-1; ++a_i)
   {
      // Loop over all points in the TCutG region
      for(size_t b_i = 0; b_i < b.GetN()-1; ++b_i)
      {
         
         // Get the current point, and the next point for each of the objects
         Double_t x1, y1, x2, y2 = 0;
         Double_t ax1, ay1, ax2, ay2 = 0;
         a.GetPoint(a_i, x1, y1);
         a.GetPoint(a_i+1, x2, y2);
         b.GetPoint(b_i, ax1, ay1);
         b.GetPoint(b_i+1, ax2, ay2);

         // Calculate the intersection between two straight lines, x axis
         Double_t x = (ax1 *(ay2 *(x1-x2)+x2 * y1 - x1 * y2 )+ ax2 * (ay1 * (-x1+x2)- x2 * y1+x1 * y2)) 
                            / (-(ay1-ay2) * (x1-x2)+(ax1-ax2)* (y1-y2));
                            
         // Calculate the intersection between two straight lines, y axis
         Double_t y = (ax1 * ay2 * (y1-y2)+ax2 * ay1 * (-y1+y2)+(ay1-ay2) * (x2 * y1-x1 * y2))/(-(ay1-ay2) * (x1-x2)+(ax1-ax2) * (y1-y2));

         // Find the tightest interval along the x-axis defined by the four points
         Double_t xrange_min = max(min(x1, x2), min(ax1, ax2));
         Double_t xrange_max = min(max(x1, x2), max(ax1, ax2));

         // If points from the two lines overlap, they are trivially intersecting
         if ((x1 == ax1 and y1 == ay1) or (x2 == ax2 and y2 == ay2)){               
            interPoint->SetPoint(i, (x1 == ax1 and y1 == ay1) ? x1 : x2, (x1 == ax1 and y1 == ay1) ? y1 : y2);
            i++;
            
         } 
         // If the intersection between the two lines is within the tight range, add it to the list of intersections.
         else if(x > xrange_min && x < xrange_max)
         {
            interPoint->SetPoint(i,x, y);
            i++;
            
         }
      }
   }
   return interPoint;
}

