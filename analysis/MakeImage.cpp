#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TTree.h"

TCanvas *canv;

Double_t fitFnc(Double_t *pos, Double_t *par)
{  // This should be class not function.

  auto x = pos[0];
  auto mean = par[1];
  auto sigma = par[2];

  auto facRange = 3;
  auto limitHigh = mean + facRange * sigma;
  auto limitLow = mean - facRange * sigma;

  auto val = par[0] * TMath::Gaus(x, mean, sigma);

  auto backGround = 0.;
  if (x < limitLow)
    backGround = par[3] + par[4] * x;
  else if (x > limitHigh)
    backGround = par[5] + par[6] * x;
  else {
    auto xInc = limitHigh - limitLow;
    auto yInc = (par[5] + par[6] * limitHigh) - (par[3] + par[4] * limitLow);
    auto slope = yInc / xInc;

    backGround = (par[3] + par[4] * limitLow) + slope * (x - limitLow);
  }

  if (backGround < 0.) backGround = 0.;
  val += backGround;

  return val;
}

std::vector<Double_t> FindPeaks(TH1D *his)
{
  // Find peaks
  // Searching is till 700 keV from the first peak

  std::vector<Double_t> retVec{-1.};

  const auto nBins = his->GetNbinsX();
  const auto maxContent = his->GetMaximum();
  auto upCount = 0;
  auto downCount = 0;
  const UInt_t nSamples = 10;
  auto lastVal = 0.;
  auto lastSlope = 0.;

  for (auto iBin = nBins - nSamples; iBin - nSamples > 0; iBin -= nSamples) {
    auto val{0.};
    for (auto i = iBin - nSamples / 2; i < iBin + nSamples; i++)
      val += his->GetBinContent(i) / nSamples;

    auto slope = (val - lastVal) / nSamples;
    lastVal = val;

    // Fuck the dirty code!!!!!!
    const auto th = 5;
    if (upCount < th && slope < 0. && lastSlope < 0.) upCount = 0;

    if (lastSlope * slope > 0.) {
      if (upCount > 0 && slope < 0.)
        downCount++;
      else if (slope > 0.)
        upCount++;
    }
    lastSlope = slope;

    if (upCount >= th && downCount >= th) {
      upCount = downCount = 0;
      auto peakPos = iBin + th * nSamples;
      if (his->GetBinContent(peakPos) > maxContent / 20.) {
        if (retVec[0] == -1)
          retVec[0] = peakPos;
        else
          retVec.push_back(peakPos);
      }
    }

    if (retVec.size() >= 2) break;
  }

  if (retVec[0] == -1) retVec[0] = his->GetMaximumBin();

  return retVec;
}

Double_t Digitize(Double_t ene)
{
  // FWHM (Ge detector GC15024)
  // 2.4 keV @ 1.33MeV
  // 1.50 keV @ 122keV
  // Simple estimation of FWHM by 1 deg poly

  ene *= 1000.;  // MeV -> keV

  constexpr auto slope = (2.4 - 1.5) / (1330. - 122.);
  constexpr auto offset = 2.4 - slope * 1330.;
  // constexpr auto facFWHM = 2 * sqrt(2*log(2));
  constexpr auto facFWHM = 2.3548200;

  const auto sigma = (ene * slope + offset) / facFWHM;

  return gRandom->Gaus(ene, sigma) / 1000.;  // keV -> MeV
}

TH1D *test(Int_t ene = 10000)
{
  auto fileName = Form("resultEne%d.root", ene);
  auto file = new TFile(fileName, "READ");
  auto tree = (TTree *)file->Get("Event");

  Double_t eneGe;
  tree->SetBranchAddress("Ge", &eneGe);

  Double_t eneNaI;
  tree->SetBranchAddress("NaI", &eneNaI);

  auto HisGe =
      new TH1D("HisGe", Form("Energy distribution (%2.1f MeV)", ene / 1000.),
               20000, 0., 20.);
  HisGe->SetXTitle("[MeV]");
  HisGe->SetDirectory(0);
  auto HisAll = new TH1D("HisAll", "energy distribution", 20000, 0., 20.);

  const auto th = 0.;
  const UInt_t nEve = tree->GetEntries();
  for (auto iEve = 0; iEve < nEve; iEve++) {
    tree->GetEntry(iEve);

    if (eneNaI <= th) HisGe->Fill(Digitize(eneGe));
    HisAll->Fill(Digitize(eneGe));
  }

  auto v = FindPeaks(HisGe);
  // for (auto peak : v) std::cout << peak << "\t";
  // std::cout << std::endl;

  auto mean = v[0] / 1000.;  // keV -> MeV
  auto sigma = 0.1;

  auto f = new TF1("f", "gaus");
  f->SetRange(mean - sigma, mean + sigma);
  HisGe->Fit(f, "R");

  mean = f->GetParameter(1);
  sigma = f->GetParameter(2);
  const auto facFWHM = 2 * sqrt(2 * log(2));
  auto FWHM = sigma * facFWHM;
  f->SetRange(mean - FWHM / 2, mean + FWHM / 2);
  HisGe->Fit(f, "R");

  mean = f->GetParameter(1);
  sigma = f->GetParameter(2);
  auto rangeFac = 5.;
  auto left = HisGe->GetBinContent(HisGe->FindBin(mean - rangeFac * sigma));
  auto right = HisGe->GetBinContent(HisGe->FindBin(mean + rangeFac * sigma));
  auto myFit = new TF1("myFit", fitFnc, mean - rangeFac * sigma,
                       mean + rangeFac * sigma, 7);
  myFit->SetParameters(f->GetParameter(0), mean, sigma, left, -0.001, right,
                       0.);
  myFit->FixParameter(6, 0.);
  myFit->SetParLimits(1, mean - sigma, mean + sigma);
  HisGe->Fit(myFit, "R");

  // HisAll->Draw();
  // HisGe->SetLineColor(kCyan);

  HisGe->GetXaxis()->SetRange(HisGe->FindBin(mean - 2 * rangeFac * sigma),
                              HisGe->FindBin(mean + 2 * rangeFac * sigma));
  HisGe->Draw();

  auto result = new TF1("result", "gaus", left, right);
  result->SetParameters(myFit->GetParameter(0), myFit->GetParameter(1),
                        myFit->GetParameter(2));
  result->SetLineColor(kGreen);
  result->Draw("SAME");

  myFit->SetParameters(0, mean, sigma, myFit->GetParameter(3),
                       myFit->GetParameter(4), myFit->GetParameter(5),
                       myFit->GetParameter(6));
  myFit->SetLineColor(kCyan);
  myFit->Draw("SAME");

  canv->Print("fit.pdf", "pdf");

  // file->Close();

  return HisGe;
}

void MakeImage()
{
  canv = new TCanvas();
  canv->Print("fit.pdf[", "pdf");

  std::vector<TH1D *> hisVec;
  hisVec.push_back(test(200));
  for (auto ene = 500; ene <= 19500; ene += 500) {
    hisVec.push_back(test(ene));
  }

  canv->Print("fit.pdf]", "pdf");

  auto output = new TFile("fit.root", "RECREATE");
  for (auto &&his : hisVec) his->Write();
  output->Close();
}
