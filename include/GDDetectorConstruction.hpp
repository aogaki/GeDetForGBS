#ifndef GDDetectorConstruction_h
#define GDDetectorConstruction_h 1

#include <vector>

#include <G4GenericMessenger.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VSolid.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VisAttributes.hh>

class GDDetectorConstruction : public G4VUserDetectorConstruction
{
 public:
  GDDetectorConstruction();
  virtual ~GDDetectorConstruction();

  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

 private:
  G4bool fCheckOverlap;

  // Materials
  void DefineMaterials();
  G4Material *fVacuumMat;
  G4Material *fAirMat;
  G4Material *fCrystalMat;
  G4Material *fColdFingerMat;
  G4Material *fHolderMat;
  G4Material *fEndcapMat;

  G4LogicalVolume *ConstructGeDet();
  G4LogicalVolume *ConstructGeHolder();
  G4LogicalVolume *ConstructGeCrystal();
  G4double fHolderL;
  G4double fCrystalL;
  G4double fGeDetL;

  void SetVis(G4LogicalVolume *lv, G4Color col, G4bool visibility);
  std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
