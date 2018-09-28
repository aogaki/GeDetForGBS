#ifndef GDHit_h
#define GDHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class GDHit : public G4VHit
{
public:
   GDHit();
   virtual ~GDHit();
   GDHit(const GDHit &right);
   const GDHit &operator=(const GDHit &right);
   int operator==(const GDHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods

private:

};

typedef G4THitsCollection<GDHit> GDHitsCollection;

extern G4ThreadLocal G4Allocator<GDHit> *GDHitAllocator;

inline void *GDHit::operator new(size_t)
{
   if (!GDHitAllocator)
      GDHitAllocator = new G4Allocator<GDHit>;
   return (void *)GDHitAllocator->MallocSingle();
}

inline void GDHit::operator delete(void *hit)
{
   GDHitAllocator->FreeSingle((GDHit *) hit);
}

#endif
