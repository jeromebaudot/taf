{
TF1 *func1 = new TF1("func1","TMath::Gaus(x,0,1,1)",-20,20);
TF1 *func2 = new TF1("func1","TMath::Gaus(x,0,2,1)",-20,20);
TF1 *func3 = new TF1("func1","TMath::Gaus(x,0,4,1)",-20,20);
TF1 *func4 = new TF1("func1","TMath::Gaus(x,4,1,1)",-20,20);

  
func1->SetLineStyle(1);
func1->Draw();

func2->SetLineStyle(2);
func2->Draw("SAME");

func3->SetLineStyle(3);
func3->Draw("SAME");

func4->SetLineStyle(4);
func4->Draw("SAME");

leg1 = new TLegend(0.18,0.25,0.5,0.7);
leg1->AddEntry(func1,"<x> = 0; sigma = 1","l");
leg1->AddEntry(func2,"<x> = 0; sigma = 2","l");
leg1->AddEntry(func3,"<x> = 0; sigma = 4","l");
leg1->AddEntry(func4,"<x> = 4; sigma = 1","l");
leg1->SetFillStyle(0);
leg1->Draw();

}
