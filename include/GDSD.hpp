#ifndef GDSD_h
#define GDSD_h 1

#include <G4VSensitiveDetector.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "GDHit.hpp"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class GDSD: public G4VSensitiveDetector
{
public:
   GDSD(const G4String &name,
            const G4String &hitsCollectionName);
   virtual ~GDSD();

   virtual void Initialize(G4HCofThisEvent *hce);
   virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
   GDHitsCollection *fHitsCollection;
};

#endif
