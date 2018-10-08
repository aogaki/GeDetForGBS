#include <G4Event.hh>
#include <G4SDManager.hh>
#include <g4root.hh>

#include "GDEventAction.hpp"

GDEventAction::GDEventAction() : G4UserEventAction(), fHitsCollectionID(-1) {}

GDEventAction::~GDEventAction() {}

GDHitsCollection *GDEventAction::GetHitsCollection(G4int hcID,
                                                   const G4Event *event) const
{
  GDHitsCollection *hitsCollection =
      static_cast<GDHitsCollection *>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    // check how to use G4Exception
    G4Exception("GDEventAction::GetHitsCollection()", "GDCode0003",
                FatalException, msg);
  }

  return hitsCollection;
}

void GDEventAction::BeginOfEventAction(const G4Event *) {}

void GDEventAction::EndOfEventAction(const G4Event *event)
{
  if (fHitsCollectionID == -1)
    fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("HC");

  G4AnalysisManager *anaMan = G4AnalysisManager::Instance();

  auto eventID = event->GetEventID();
  auto depEneGe = 0.;
  auto depEneNaI = 0.;

  GDHitsCollection *hc = GetHitsCollection(fHitsCollectionID, event);
  const G4int kHit = hc->entries();
  for (G4int iHit = 0; iHit < kHit; iHit++) {
    GDHit *newHit = (*hc)[iHit];

    anaMan->FillNtupleIColumn(0, 0, eventID);
    anaMan->FillNtupleSColumn(0, 1, newHit->GetName());
    anaMan->FillNtupleDColumn(0, 2, newHit->GetDepEne());
    anaMan->FillNtupleDColumn(0, 3, newHit->GetTime());

    anaMan->AddNtupleRow(0);

    if (newHit->GetName() == "Ge")
      depEneGe += newHit->GetDepEne();
    else if (newHit->GetName() == "NaI")
      depEneNaI += newHit->GetDepEne();
  }

  if (depEneGe > 0.) {
    anaMan->FillNtupleIColumn(1, 0, eventID);
    anaMan->FillNtupleDColumn(1, 1, depEneGe);
    anaMan->FillNtupleDColumn(1, 2, depEneNaI);
    anaMan->AddNtupleRow(1);
  }
}
