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

#include "GDConstructGe.hpp"
#include "GDConstructNaI.hpp"

class GDDetectorConstruction : public G4VUserDetectorConstruction
{
 public:
  GDDetectorConstruction();
  virtual ~GDDetectorConstruction();

  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

 private:
  G4bool fCheckOverlap;

  GDConstructGe *fConstructorGe;
  GDConstructNaI *fConstructorNaI;

  // Materials
  void DefineMaterials();
  G4Material *fAirMat;

  G4double fGeDetL;
  G4double fNaIDetL;

  void SetVis(G4LogicalVolume *lv, G4Color col, G4bool visibility);
  std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
