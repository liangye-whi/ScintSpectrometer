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
/// \file OpNovice/src/OpNoviceStackingAction.cc
/// \brief Implementation of the OpNoviceStackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNoviceStackingAction.hh"
#include "OpNoviceRun.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceStackingAction::OpNoviceStackingAction(HistoManager* histo)
  : G4UserStackingAction()
  , fScintillationCounter(0)
  , fCerenkovCounter(0)
  , fHistoManager(histo)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceStackingAction::~OpNoviceStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ClassificationOfNewTrack OpNoviceStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  {  // particle is optical photon
    if(aTrack->GetParentID() > 0)
    {  // particle is secondary
      if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation")
        ++fScintillationCounter;
      else if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
        ++fCerenkovCounter;

      if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
      {
        //G4cout<<"DEBUG"<<aTrack->GetTotalEnergy()/eV<<" "<<G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()<<G4endl;
        fHistoManager->FillNtuple2(aTrack->GetTotalEnergy()/eV,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID(),1);
        //G4cout<< "C photon from parent ID " << aTrack->GetParentID()<<G4endl;
      }
      else
      {
        //if(G4RunManager::GetRunManager()->GetCurrentEvent()==nullptr)G4cout<<"Here ERROR!!!!!"<<G4endl;
        fHistoManager->FillNtuple2(aTrack->GetTotalEnergy()/eV,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID(),0);
        //G4cout<< "NC photon from parent ID " << aTrack->GetParentID()<<G4endl;
      }
    }
  }
  return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceStackingAction::NewStage()
{
  // G4cout << "Number of Scintillation photons produced in this event : "
  //       << fScintillationCounter << G4endl;
  // G4cout << "Number of Cerenkov photons produced in this event : "
  //       << fCerenkovCounter << G4endl;

  OpNoviceRun* run = static_cast<OpNoviceRun*>(
    G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->AddScintillation((G4double) fScintillationCounter);
  run->AddCerenkov((G4double) fCerenkovCounter);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceStackingAction::PrepareNewEvent()
{
  fScintillationCounter = 0;
  fCerenkovCounter      = 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
