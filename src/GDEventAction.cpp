#include <G4Event.hh>
#include <G4SDManager.hh>
#include <g4root.hh>

#include "GDEventAction.hpp"


GDEventAction::GDEventAction()
   : G4UserEventAction(),
     fHitsCollectionID(-1)
{}

GDEventAction::~GDEventAction()
{}

GDHitsCollection *GDEventAction::GetHitsCollection(G4int hcID, const G4Event *event)
const
{
   GDHitsCollection *hitsCollection 
      = static_cast<GDHitsCollection *>(
         event->GetHCofThisEvent()->GetHC(hcID));
  
   if ( ! hitsCollection ) {
      G4ExceptionDescription msg;
      msg << "Cannot access hitsCollection ID " << hcID;
      // check how to use G4Exception
      G4Exception("GDEventAction::GetHitsCollection()",
                  "GDCode0003", FatalException, msg);
   }         

   return hitsCollection;
}

void GDEventAction::BeginOfEventAction(const G4Event *)
{}

void GDEventAction::EndOfEventAction(const G4Event *event)
{
   if (fHitsCollectionID == -1)
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("HC");
   
   GDHitsCollection *hc = GetHitsCollection(fHitsCollectionID, event);
   const G4int kHit = hc->entries();
   for (G4int iHit = 0; iHit < kHit; iHit++) {
      GDHit *newHit = (*hc)[iHit];
   }

}

