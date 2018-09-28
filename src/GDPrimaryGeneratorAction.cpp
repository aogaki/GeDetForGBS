#include <TFile.h>

#include <G4AutoLock.hh>
#include <G4ChargedGeantino.hh>
#include <G4IonTable.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>
#include <g4root.hh>

#include "GDPrimaryGeneratorAction.hpp"

static G4int nEveInPGA = 0;
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;

GDPrimaryGeneratorAction::GDPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr)
{
  fParticleGun = new G4ParticleGun(1);

  auto parTable = G4ParticleTable::GetParticleTable();

  auto particle = parTable->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particle);

  auto sourcePos = -5. * cm;
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., sourcePos));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

  fEnergy = 10. * MeV;
  fParticleGun->SetParticleEnergy(fEnergy);
}

GDPrimaryGeneratorAction::~GDPrimaryGeneratorAction() { delete fParticleGun; }

void GDPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  fParticleGun->GeneratePrimaryVertex(event);

  G4AutoLock lock(&mutexInPGA);
  if (nEveInPGA++ % 10000 == 0)
    G4cout << nEveInPGA - 1 << " events done" << G4endl;
}
