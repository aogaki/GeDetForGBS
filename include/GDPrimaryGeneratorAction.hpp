#ifndef GDPrimaryGeneratorAction_h
#define GDPrimaryGeneratorAction_h 1

#include <TH2.h>

#include <G4Event.hh>
#include <G4GenericMessenger.hh>
#include <G4ParticleGun.hh>
#include <G4Threading.hh>
#include <G4ThreeVector.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

class GDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
 public:
  GDPrimaryGeneratorAction();
  virtual ~GDPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event *);

 private:
  G4ParticleGun *fParticleGun;
  G4double fEnergy;
};

#endif
