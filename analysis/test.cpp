TH1D *HisGe;


Double_t Digitize(Double_t ene)
{
   // FWHM (Ge detector GC15024)
   // 2.4 keV @ 1.33MeV
   // 1.50 keV @ 122keV
   // Simple estimation of FWHM by 1 deg poly

   ene *= 1000.; // MeV -> keV
   
   constexpr auto slope = (2.4 - 1.5) / (1330. - 122.);
   constexpr auto offset = 2.4 - slope * 1330.;
   // constexpr auto facFWHM = 2 * sqrt(2*log(2));
   constexpr auto facFWHM = 2.3548200;

   const auto sigma = (ene * slope + offset) / facFWHM;

   return gRandom->Gaus(ene, sigma) / 1000.; // keV -> MeV
}

void test(Int_t ene = 19500)
{
   auto fileName = Form("resultEne%d.root", ene);
   auto file = new TFile(fileName, "READ");
   auto tree = (TTree*)file->Get("Event");

   
   Double_t eneGe;
   tree->SetBranchAddress("Ge", &eneGe);

   Double_t eneNaI;
   tree->SetBranchAddress("NaI", &eneNaI);


   HisGe = new TH1D("HisGe", "energy distribution", 20000, 0., 20.);
   
   const auto th = 0.;
   const UInt_t nEve = tree->GetEntries();
   for(auto iEve = 0; iEve < nEve; iEve++){
      tree->GetEntry(iEve);

      if(eneNaI <= th) HisGe->Fill(Digitize(eneGe));
   }

   HisGe->Draw();
}
