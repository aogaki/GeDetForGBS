#ifndef GDEventAction_h
#define GDEventAction_h 1

#include <G4UserEventAction.hh>

#include "GDHit.hpp"

class GDEventAction : public G4UserEventAction
{
public:
   GDEventAction();
   virtual ~GDEventAction();

   virtual void BeginOfEventAction(const G4Event *);
   virtual void EndOfEventAction(const G4Event *);

private:
   GDHitsCollection *GetHitsCollection(G4int hcID, const G4Event *event) const;
   
   G4int fHitsCollectionID;
};

#endif
