#include <iostream>
#include <vector>

#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TTree.h"

TH1D *HisGe;
TH1D *HisAll;

Double_t fitFnc(Double_t *pos, Double_t *par)
{  // This should be class not function.

  auto x = pos[0];
  auto mean = par[1];
  auto sigma = par[2];

  auto facRange = 2;
  auto limitHigh = mean + facRange * sigma;
  auto limitLow = mean - facRange * sigma;

  auto val = par[0] * TMath::Gaus(x, mean, sigma);

  auto result = 0.;
  if (x < limitLow)
    result = par[3] + par[4] * x;
  else if (x > limitHigh)
    result = par[5] + par[6] * x;
  else {
    auto xInc = limitHigh - limitLow;
    auto yInc = (par[5] + par[6] * limitHigh) - (par[3] + par[4] * limitLow);
    auto slope = yInc / xInc;

    result = (par[3] + par[4] * limitLow) + slope * (x - limitLow);
  }

  if (result < 0.) result = 0.;
  val += result;

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

void test(Int_t ene = 19500)
{
  auto fileName = Form("resultEne%d.root", ene);
  auto file = new TFile(fileName, "READ");
  auto tree = (TTree *)file->Get("Event");

  Double_t eneGe;
  tree->SetBranchAddress("Ge", &eneGe);

  Double_t eneNaI;
  tree->SetBranchAddress("NaI", &eneNaI);

  HisGe =
      new TH1D("HisGe", Form("Energy distribution (%2.1f MeV)", ene / 1000.),
               20000, 0., 20.);
  HisGe->SetXTitle("[MeV]");
  HisAll = new TH1D("HisAll", "energy distribution", 20000, 0., 20.);

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
                       -0.01);
  myFit->SetParLimits(1, mean - sigma, mean + sigma);
  HisGe->Fit(myFit, "R");

  HisAll->GetXaxis()->SetRange(HisGe->FindBin(mean - 2 * rangeFac * sigma),
                              HisGe->FindBin(mean + 2 * rangeFac * sigma));

  HisAll->Draw();
  HisGe->SetLineColor(kBlack);

  HisGe->Draw("SAME");
/*
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
*/
}
