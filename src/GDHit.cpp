#include <iomanip>

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "GDHit.hpp"


G4ThreadLocal G4Allocator<GDHit> *GDHitAllocator = 0;


GDHit::GDHit()
   : G4VHit()
{}

GDHit::~GDHit()
{}

GDHit::GDHit(const GDHit & /*right*/)
   : G4VHit()
{}

const GDHit &
GDHit::operator=(const GDHit & /*right*/)
{
   return *this;
}

int GDHit::operator==(const GDHit & /*right*/) const
{
   return 0;
}
