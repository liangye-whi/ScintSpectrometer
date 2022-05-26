//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file persistency/gdml/G04/src/G04SensitiveDetector.cc
/// \brief Implementation of the G04SensitiveDetector class
//
//
//

#include "G04SensitiveDetector.hh"
#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G04SensitiveDetector::G04SensitiveDetector(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), fHitsCollection(0)
{
    collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G04SensitiveDetector::~G04SensitiveDetector()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G04SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new MyHitsCollection(SensitiveDetectorName,collectionName[0]);
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID,fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G04SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4double energy;
  //G4cout << "Processing hits ...." << G4endl; 
  energy = step->GetTrack()->GetTotalEnergy();
  //G4cout << "E = " << energy << G4endl;
  MyHit* newHit = new MyHit();
  newHit->SetEnergy(energy);
  fHitsCollection->insert(newHit);
  //G4cout<<"newHit Added."<<G4endl;
  //G4ThreeVector mv = step->GetTrack()->GetMomentumDirection();
  //G4cout <<step->GetTrack()->GetParticleDefinition()->GetParticleType()<<" E="<<step->GetTrack()->GetTotalEnergy()/ eV<<" eV p=" <<mv.getX() <<" " << mv.getY()<<" "<<mv.getZ() <<G4endl;
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G04SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}
