#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>

#include "GDConstructNaI.hpp"

GDConstructNaI::GDConstructNaI()
{
  fCheckOverlap = true;

  DefineMaterials();
  Construct();
}

GDConstructNaI::~GDConstructNaI()
{
  // Really needed?
  for (auto &&vis : fVisAttributes) delete vis;
}

void GDConstructNaI::DefineMaterials()
{
  G4NistManager *manager = G4NistManager::Instance();

  // NIST database materials
  // Only the pure materials
  // What a easy world
  fScintiMat = manager->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  fHolderMat = manager->FindOrBuildMaterial("G4_Al");
  fReflectorMat = manager->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
}

void GDConstructNaI::Construct() { fNaIDet = ConstructNaIDet(); }

G4LogicalVolume *GDConstructNaI::ConstructNaIDet()
{
  // Outer
  // You dont like hardcoding?
  auto holderD = 280. * mm;
  auto holderL = 273. * mm;
  auto holderID = 110. * mm;
  auto bulkS = new G4Tubs("Holder", holderID / 2., holderD / 2., holderL / 2.,
                          0., CLHEP::twopi);
  auto cutD = holderD + 1. * mm;
  auto cutL = holderL - 20. * mm - 15. * mm;
  auto cutID = holderD - 2. * 18. * mm;
  auto cutS =
      new G4Tubs("Cut", cutID / 2., cutD / 2., cutL / 2., 0., CLHEP::twopi);
  auto cutPos = G4ThreeVector(0., 0., 15. * mm - (holderL - cutL) / 2.);
  auto holderS = new G4SubtractionSolid("Holder", bulkS, cutS, nullptr, cutPos);
  auto holderLV = new G4LogicalVolume(holderS, fHolderMat, "Holder");
  SetVis(holderLV, G4Colour(0.5, 0.5, 0.5, 0.2), true);

  // Reflector
  auto refD = 239. * mm;
  auto refL = 253. * mm;
  auto refID = 115. * mm;
  auto refS = new G4Tubs("Reflector", refID / 2., refD / 2., refL / 2., 0.,
                         CLHEP::twopi);
  auto refLV = new G4LogicalVolume(refS, fReflectorMat, "Reflector");
  SetVis(refLV, G4Colour(1, 1, 1, 0.1), true);

  auto refPos = G4ThreeVector(0., 0., 6. * mm - (holderL - refL) / 2.);
  new G4PVPlacement(nullptr, refPos, refLV, "Reflector", holderLV, false, 0,
                    fCheckOverlap);

  // Scintillator
  auto scintiD = 234. * mm;
  auto scintiL = 250. * mm;
  auto scintiID = 120. * mm;
  auto scintiS = new G4Tubs("NaI", scintiID / 2., scintiD / 2., scintiL / 2.,
                            0., CLHEP::twopi);
  auto scintiLV = new G4LogicalVolume(scintiS, fScintiMat, "NaI");
  SetVis(scintiLV, G4Colour(1, 1, 0, 0.3), true);

  auto scintiPos = G4ThreeVector(0., 0., 3. * mm - (refL - scintiL) / 2.);
  new G4PVPlacement(nullptr, scintiPos, scintiLV, "NaI", refLV, false, 0,
                    fCheckOverlap);

  return holderLV;
}

void GDConstructNaI::SetVis(G4LogicalVolume *lv, G4Color col, G4bool visibility)
{
  auto visAttributes = new G4VisAttributes(col);
  visAttributes->SetVisibility(visibility);
  lv->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
}
