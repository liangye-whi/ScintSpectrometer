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
/// \file optical/OpNovice/src/OpNoviceEventAction.cc
/// \brief Implementation of the OpNoviceEventAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "HistoManager.hh"
#include "MyHit.hh"
#include "OpNoviceEventAction.hh"
#include "OpNoviceRun.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceEventAction::OpNoviceEventAction(HistoManager * histo)
  : G4UserEventAction(), fHit(false), fHistoManager(histo)
{
  fRayleigh   = 0;
  fAbsorption = 0;
  fMie        = 0;
  fBoundary   = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceEventAction::~OpNoviceEventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceEventAction::BeginOfEventAction(const G4Event*)
{
  fRayleigh   = 0;
  fAbsorption = 0;
  fMie        = 0;
  fBoundary   = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceEventAction::EndOfEventAction(const G4Event* event)
{
  OpNoviceRun* run = static_cast<OpNoviceRun*>(
    G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->AddRayleigh(fRayleigh);
  run->AddAbsorption(fAbsorption);
  run->AddMie(fMie);
  run->AddBoundary(fBoundary);
  int NHits = event->GetHCofThisEvent()->GetHC(0)->GetSize();
    //G4cout<<"Hits : "<<NHits<<G4endl;
  for(int i=0;i<NHits;++i){
    //G4cout<<"Hits "<<i+1<<" energy: "<<((MyHit*) event->GetHCofThisEvent()->GetHC(0)->GetHit(i))->GetEnergy()/eV<<" eV"<<G4endl;
    //fHistoManager->FillNtuple(((MyHit*) event->GetHCofThisEvent()->GetHC(0)->GetHit(i))->GetEnergy()/eV);
    fHistoManager->FillHisto(0,((MyHit*) event->GetHCofThisEvent()->GetHC(0)->GetHit(i))->GetEnergy()/eV,1);
    fHistoManager->FillNtuple(((MyHit*) event->GetHCofThisEvent()->GetHC(0)->GetHit(i))->GetEnergy()/eV);
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
