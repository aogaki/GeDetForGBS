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

  void SetEnergy(G4double ene);

 private:
  G4ParticleGun *fParticleGun;

  void DefineCommands();
  G4GenericMessenger *fMessenger;
  G4double fEnergy;

  // RMS?  Should be Sigma or StdDev
  void SetRMS(G4double rms) { fRMS = rms; };
  G4double fRMS;
};

#endif
