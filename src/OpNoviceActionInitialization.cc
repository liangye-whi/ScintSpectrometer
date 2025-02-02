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
//
/// \file OpNoviceActionInitialization.cc
/// \brief Implementation of the OpNoviceActionInitialization class

#include "OpNoviceActionInitialization.hh"
#include "HistoManager.hh"
#include "OpNoviceEventAction.hh"
#include "OpNovicePrimaryGeneratorAction.hh"
#include "OpNoviceRunAction.hh"
#include "OpNoviceStackingAction.hh"
#include "OpNoviceSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceActionInitialization::OpNoviceActionInitialization(HistoManager* histo)
  : G4VUserActionInitialization(),fHistoManager(histo)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceActionInitialization::~OpNoviceActionInitialization() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceActionInitialization::BuildForMaster() const
{
  HistoManager* histo = new HistoManager();
  SetUserAction(new OpNoviceRunAction(nullptr,histo));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceActionInitialization::Build() const
{
  OpNovicePrimaryGeneratorAction* primary =
    new OpNovicePrimaryGeneratorAction();
  SetUserAction(primary);
  SetUserAction(new OpNoviceRunAction(primary, fHistoManager));
  OpNoviceEventAction* event = new OpNoviceEventAction(fHistoManager);
  SetUserAction(event);
  SetUserAction(new OpNoviceSteppingAction(event,fHistoManager));
  SetUserAction(new OpNoviceStackingAction(fHistoManager));
}
