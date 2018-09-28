#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4PVReplica.hh>
#include <G4RunManager.hh>

#include "GDDetectorConstruction.hpp"
#include "GDSD.hpp"


GDDetectorConstruction::GDDetectorConstruction()
   : fVacuumMat(nullptr)
{
   fCheckOverlap = true;

   DefineMaterials();
}

GDDetectorConstruction::~GDDetectorConstruction()
{}

void GDDetectorConstruction::DefineMaterials()
{
   G4NistManager *manager = G4NistManager::Instance();

   // NIST database materials
   fVacuumMat = manager->FindOrBuildMaterial("G4_Galactic");
}

G4VPhysicalVolume *GDDetectorConstruction::Construct()
{
   // world volume
   G4double worldX = 0.1*m;
   G4double worldY = 0.1*m;
   G4double worldZ = 0.6*m;

   G4Box *worldS = new G4Box("World", worldX / 2., worldY / 2., worldZ / 2.);
   G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, fVacuumMat, "World");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   worldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4VPhysicalVolume *worldPV
      = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", 0,
                          false, 0, fCheckOverlap);
   return worldPV;
}

void GDDetectorConstruction::ConstructSDandField()
{
   // Sensitive Detectors
   G4VSensitiveDetector *SD = new GDSD("SD", "HC");
   G4SDManager::GetSDMpointer()->AddNewDetector(SD);
   
   G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
   for(auto &&lv: *lvStore){
      if(lv->GetName().contains("LGSORow") ||
         lv->GetName().contains("NaI"))
         SetSensitiveDetector(lv->GetName(), SD);
   }
}
