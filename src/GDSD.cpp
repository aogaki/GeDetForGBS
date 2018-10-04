#include <G4HCofThisEvent.hh>
#include <G4Material.hh>
#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4TouchableHistory.hh>
#include <G4VProcess.hh>
#include <G4ios.hh>
#include <g4root.hh>

#include "GDHit.hpp"
#include "GDSD.hpp"

GDSD::GDSD(const G4String &name, const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

GDSD::~GDSD() {}

void GDSD::Initialize(G4HCofThisEvent *hce)
{
  fHitsCollection =
      new GDHitsCollection(SensitiveDetectorName, collectionName[0]);

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool GDSD::ProcessHits(G4Step *step, G4TouchableHistory * /*history*/)
{
  auto depEne = step->GetTotalEnergyDeposit();
  if (depEne == 0.) return false;

  auto newHit = new GDHit();
  newHit->SetDepEne(depEne);

  G4StepPoint *preStepPoint = step->GetPreStepPoint();
  G4String volumeName = preStepPoint->GetPhysicalVolume()->GetName();
  newHit->SetName(volumeName);

  auto globalTime = preStepPoint->GetGlobalTime();
  newHit->SetTime(globalTime);

  fHitsCollection->insert(newHit);
  return true;
}
