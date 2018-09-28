#ifndef GDActionInitialization_h
#define GDActionInitialization_h 1

#include <G4VUserActionInitialization.hh>
#include <globals.hh>


class GDActionInitialization : public G4VUserActionInitialization
{
public:
   GDActionInitialization();
   virtual ~GDActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
      
};

#endif
