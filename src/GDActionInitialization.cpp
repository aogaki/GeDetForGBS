#include "GDActionInitialization.hpp"
#include "GDEventAction.hpp"
#include "GDPrimaryGeneratorAction.hpp"
#include "GDRunAction.hpp"

GDActionInitialization::GDActionInitialization() : G4VUserActionInitialization()
{
}

GDActionInitialization::~GDActionInitialization() {}

void GDActionInitialization::BuildForMaster() const
{
  SetUserAction(new GDRunAction());
}

void GDActionInitialization::Build() const
{
  SetUserAction(new GDPrimaryGeneratorAction());
  SetUserAction(new GDRunAction());
  SetUserAction(new GDEventAction());
}
