#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4RunManager.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Torus.hh>
#include <G4Tubs.hh>
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "GDDetectorConstruction.hpp"
#include "GDSD.hpp"

GDDetectorConstruction::GDDetectorConstruction()
{
  fCheckOverlap = true;

  fGeDetL = 170. * mm;
  fNaIDetL = 273. * mm;
  fGeOffset = 165. * mm;

  DefineMaterials();

  fConstructorGe = new GDConstructGe();
  fConstructorNaI = new GDConstructNaI();
}

GDDetectorConstruction::~GDDetectorConstruction()
{
  // Really needed?
  for (auto &&vis : fVisAttributes) delete vis;
  delete fConstructorGe;
}

void GDDetectorConstruction::DefineMaterials()
{
  G4NistManager *manager = G4NistManager::Instance();

  // NIST database materials
  fAirMat = manager->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume *GDDetectorConstruction::Construct()
{
  // world volume
  G4double worldX = 5. * m;
  G4double worldY = 5. * m;
  G4double worldZ = 10. * m;

  G4Box *worldS = new G4Box("World", worldX / 2., worldY / 2., worldZ / 2.);
  G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, fAirMat, "World");
  SetVis(worldLV, G4Color::White(), false);

  auto GeDetLV = fConstructorGe->GetGe();
  auto GePos = G4ThreeVector(0., 0., fGeDetL / 2. + fGeOffset);
  auto GeRot = new G4RotationMatrix();
  new G4PVPlacement(GeRot, GePos, GeDetLV, "GeDet", worldLV, false, 0,
                    fCheckOverlap);

  auto NaIDetLV = fConstructorNaI->GetNaI();
  auto NaIPos = G4ThreeVector(0., 0., fNaIDetL / 2.);
  // auto NaIPos = G4ThreeVector(0., 0., 0.);
  auto NaIRot = new G4RotationMatrix();
  new G4PVPlacement(NaIRot, NaIPos, NaIDetLV, "NaIDet", worldLV, false, 0,
                    fCheckOverlap);

  G4VPhysicalVolume *worldPV = new G4PVPlacement(
      nullptr, G4ThreeVector(), worldLV, "World", 0, false, 0, fCheckOverlap);
  return worldPV;
}

void GDDetectorConstruction::ConstructSDandField()
{
  // Sensitive Detectors
  G4VSensitiveDetector *SD = new GDSD("SD", "HC");
  G4SDManager::GetSDMpointer()->AddNewDetector(SD);

  G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
  for (auto &&lv : *lvStore) {
    if (lv->GetName().contains("Ge") || lv->GetName().contains("NaI"))
      SetSensitiveDetector(lv->GetName(), SD);
  }
}

void GDDetectorConstruction::SetVis(G4LogicalVolume *lv, G4Color col,
                                    G4bool visibility)
{
  auto visAttributes = new G4VisAttributes(col);
  visAttributes->SetVisibility(visibility);
  lv->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
}
