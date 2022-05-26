#include "G4VHit.hh"
#include "G4THitsCollection.hh"

class MyHit : public G4VHit
{
  public:
    void SetEnergy(G4double ee){ energy = ee; }
    G4double GetEnergy(){ return energy; }
// the class definition as before
// utility functions (called by Geant4)
//virtual void Draw();
//virtual void Print();
  private:
    G4double energy;
};

typedef G4THitsCollection<MyHit> MyHitsCollection;
