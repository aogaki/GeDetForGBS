#ifndef GDRunAction_h
#define GDRunAction_h 1

#include <G4UserRunAction.hh>
#include <G4Run.hh>


class GDRunAction: public G4UserRunAction
{
public:
   GDRunAction();
   virtual ~GDRunAction();

   virtual void BeginOfRunAction(const G4Run *);
   virtual void EndOfRunAction(const G4Run *);

private:

};

#endif
