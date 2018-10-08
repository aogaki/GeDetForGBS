#ifndef GDConstructNaI_h
#define GDConstructNaI_h 1

#include <vector>

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4VisAttributes.hh>

class GDConstructNaI
{
 public:
  GDConstructNaI();
  ~GDConstructNaI();

  G4LogicalVolume *GetNaI() { return fNaIDet; };

 private:
  G4bool fCheckOverlap;

  void Construct();
  G4LogicalVolume *fNaIDet;

  // Materials
  void DefineMaterials();
  G4Material *fScintiMat;
  G4Material *fHolderMat;
  G4Material *fReflectorMat;

  G4LogicalVolume *ConstructNaIDet();

  void SetVis(G4LogicalVolume *lv, G4Color col, G4bool visibility);
  std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
