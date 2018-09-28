#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4TouchableHistory.hh>
#include <G4Step.hh>
#include <G4ios.hh>
#include <g4root.hh>
#include <G4SystemOfUnits.hh>
#include <G4Material.hh>
#include <G4VProcess.hh>

#include "GDSD.hpp"
#include "GDHit.hpp"


GDSD::GDSD(const G4String &name,
             const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

GDSD::~GDSD()
{}

void GDSD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new GDHitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool GDSD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   return true;
}
