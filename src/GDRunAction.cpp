#include <g4root.hh>

#include "GDRunAction.hpp"

GDRunAction::GDRunAction() : G4UserRunAction()
{
  auto anaMan = G4AnalysisManager::Instance();
  // anaMan->SetNtupleMerging(true);
  anaMan->SetVerboseLevel(1);
  G4String fileName = "result";
  anaMan->SetFileName(fileName);

  anaMan->CreateNtuple("GBS", "Ge and NaI");
  anaMan->CreateNtupleIColumn(0, "EveID");
  anaMan->CreateNtupleSColumn(0, "Name");
  anaMan->CreateNtupleDColumn(0, "DepEne");
  anaMan->CreateNtupleDColumn(0, "Time");

  anaMan->CreateNtuple("Event", "Ge and NaI");
  anaMan->CreateNtupleIColumn(1, "EveID");
  anaMan->CreateNtupleDColumn(1, "Ge");
  anaMan->CreateNtupleDColumn(1, "NaI");

  anaMan->FinishNtuple();
}

GDRunAction::~GDRunAction() { delete G4AnalysisManager::Instance(); }

void GDRunAction::BeginOfRunAction(const G4Run *)
{
  G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
  anaMan->OpenFile();
}

void GDRunAction::EndOfRunAction(const G4Run *)
{
  G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
  anaMan->Write();
  anaMan->CloseFile();
}
