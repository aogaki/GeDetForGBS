#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>

#include "GDConstructGe.hpp"

GDConstructGe::GDConstructGe()
{
  fCheckOverlap = true;

  fGeDetL = 170. * mm;
  fHolderL = 133.50 * mm;
  fCrystalL = 91. * mm;

  DefineMaterials();
  Construct();
}

GDConstructGe::~GDConstructGe()
{
  // Really needed?
  for (auto &&vis : fVisAttributes) delete vis;
}

void GDConstructGe::DefineMaterials()
{
  G4NistManager *manager = G4NistManager::Instance();

  // NIST database materials
  // Only the pure materials
  // What a easy world
  fVacuumMat = manager->FindOrBuildMaterial("G4_Galactic");
  fCrystalMat = manager->FindOrBuildMaterial("G4_Ge");
  fHolderMat = manager->FindOrBuildMaterial("G4_Al");
  fEndcapMat = manager->FindOrBuildMaterial("G4_Al");
}

void GDConstructGe::Construct() { fGeDet = ConstructGeDet(); }

G4LogicalVolume *GDConstructGe::ConstructGeDet()
{
  // You dont like hardcoding?
  auto endcapD = 108. * mm;
  auto endcapL = fGeDetL;
  auto endcapT = 1.5 * mm;

  auto endcapS =
      new G4Tubs("Endcap", 0., endcapD / 2., endcapL / 2., 0., CLHEP::twopi);
  auto endcapLV = new G4LogicalVolume(endcapS, fEndcapMat, "Endcap");
  SetVis(endcapLV, G4Colour(1, 1, 1, 0.3), true);

  auto vacD = endcapD - 2 * endcapT;
  auto vacL = endcapL - 2 * endcapT;
  auto vacS = new G4Tubs("Vacuum", 0., vacD / 2., vacL / 2., 0., CLHEP::twopi);
  auto vacLV = new G4LogicalVolume(vacS, fVacuumMat, "Vacuum");
  SetVis(vacLV, G4Colour(0, 1, 0, 0.1), true);

  auto holderLV = ConstructGeHolder();
  auto holderPos =
      G4ThreeVector(0., 0., -((endcapL - fHolderL) / 2. - 7.5 * mm));
  auto holderRot = new G4RotationMatrix();
  new G4PVPlacement(holderRot, holderPos, holderLV, "Holder", vacLV, false, 0,
                    fCheckOverlap);

  auto crystalLV = ConstructGeCrystal();
  auto crystalPos =
      G4ThreeVector(0., 0., -((endcapL - fCrystalL) / 2. - 6. * mm));
  auto crystalRot = new G4RotationMatrix();
  new G4PVPlacement(crystalRot, crystalPos, crystalLV, "Ge", vacLV, false, 0,
                    fCheckOverlap);

  auto vacPos = G4ThreeVector();
  auto vacRot = new G4RotationMatrix();
  new G4PVPlacement(vacRot, vacPos, vacLV, "Vacuum", endcapLV, false, 0,
                    fCheckOverlap);

  return endcapLV;
}

G4LogicalVolume *GDConstructGe::ConstructGeHolder()
{
  // Simple model
  // auto holderD = 99.5 * mm;
  auto holderD = 95.1 * mm;
  auto holderID = 93.5 * mm;
  auto holderL = fHolderL;

  auto bulkS =
      new G4Tubs("Holder", 0., holderD / 2., holderL / 2., 0., CLHEP::twopi);

  auto backD = 24. * mm;
  auto backL = 25.4 * mm;
  auto backCutS = new G4Tubs("Cut", backD / 2., holderD / 2. + 1. * mm,
                             backL / 2., 0., CLHEP::twopi);

  auto backCutPos = G4ThreeVector(0., 0., (holderL / 2. - backL / 2.));
  auto noWellS =
      new G4SubtractionSolid("Holder", bulkS, backCutS, nullptr, backCutPos);

  auto wellL = 105. * mm;
  auto wellS =
      new G4Tubs("Well", 0., holderID / 2., wellL / 2., 0., CLHEP::twopi);
  auto wellPos = G4ThreeVector(0., 0., -(holderL / 2. - wellL / 2.));
  auto noHoleS =
      new G4SubtractionSolid("Holder", noWellS, wellS, nullptr, wellPos);

  auto holeL = fHolderL - 10. * mm;
  auto holeD = 16. * mm;
  auto holeS = new G4Tubs("Hole", 0., holeD / 2., holeL / 2., 0., CLHEP::twopi);
  auto holePos = G4ThreeVector(0., 0., -(holderL / 2. - holeL / 2.));
  auto subtractS =
      new G4SubtractionSolid("Holder", noHoleS, holeS, nullptr, holePos);

  auto holderLV = new G4LogicalVolume(subtractS, fHolderMat, "Holder");
  SetVis(holderLV, G4Colour(1, 0, 0, 0.5), true);

  return holderLV;
}

G4LogicalVolume *GDConstructGe::ConstructGeCrystal()
{
  auto crystalL = fCrystalL;
  auto crystalD = 90.5 * mm;
  auto bulkS =
      new G4Tubs("Crystal", 0., crystalD / 2., crystalL / 2., 0., CLHEP::twopi);

  auto notchD = 34. * mm;
  auto notchW = 3. * mm;
  auto notchH = 2. * mm;
  auto notchS = new G4Tubs("Notch", notchD / 2. - notchW, notchD / 2.,
                           notchH / 2., 0., CLHEP::twopi);
  auto notchPos = G4ThreeVector(0., 0., (crystalL - notchH) / 2.);
  auto noHoleS =
      new G4SubtractionSolid("Crystal", bulkS, notchS, nullptr, notchPos);

  auto holeL = 63. * mm;
  auto holeD = 9.5 * mm;
  auto holeS = new G4Tubs("Hole", 0., holeD / 2., holeL / 2., 0., CLHEP::twopi);
  auto holePos = G4ThreeVector(0., 0., (crystalL - holeL) / 2.);
  auto crystalS =
      new G4SubtractionSolid("Crystal", noHoleS, holeS, nullptr, holePos);

  auto crystalLV = new G4LogicalVolume(crystalS, fCrystalMat, "Ge");
  SetVis(crystalLV, G4Colour(0, 1, 1, 0.5), true);

  return crystalLV;
}

void GDConstructGe::SetVis(G4LogicalVolume *lv, G4Color col, G4bool visibility)
{
  auto visAttributes = new G4VisAttributes(col);
  visAttributes->SetVisibility(visibility);
  lv->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
}
