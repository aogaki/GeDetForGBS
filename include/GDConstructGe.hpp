#ifndef GDConstructGe_h
#define GDConstructGe_h 1

#include <vector>

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4VisAttributes.hh>

class GDConstructGe
{
 public:
  GDConstructGe();
  ~GDConstructGe();

  G4LogicalVolume *GetGe() { return fGeDet; };

 private:
  G4bool fCheckOverlap;

  void Construct();
  G4LogicalVolume *fGeDet;

  // Materials
  void DefineMaterials();
  G4Material *fVacuumMat;
  G4Material *fCrystalMat;
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
