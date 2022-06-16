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
/// \file OpNoviceSteppingAction.cc
/// \brief Implementation of the OpNoviceSteppingAction class

#include "HistoManager.hh"
#include "OpNoviceSteppingAction.hh"
#include "OpNoviceRun.hh"
#include "G4Event.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceSteppingAction::OpNoviceSteppingAction(OpNoviceEventAction* event,HistoManager* histo)
  : G4UserSteppingAction()
  , fEventAction(event)
  , eTrackLength(0.)
  , fHistoManager(histo)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceSteppingAction::~OpNoviceSteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceSteppingAction::UserSteppingAction(const G4Step* step)
{
  static G4ParticleDefinition* opticalphoton =
    G4OpticalPhoton::OpticalPhotonDefinition();

  const G4ParticleDefinition* particleDef =
    step->GetTrack()->GetDynamicParticle()->GetParticleDefinition();

  ///* uncomment to add cut
  if(particleDef == opticalphoton)
  {
  //  if(step->GetTrack()->GetTrackLength()>10*cm and step->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="Scint"){
     if(step->GetTrack()->GetTrackLength()>50*cm){
      //G4cout<<step->GetTrack()->GetTrackID()<<" "<<step->GetTrack()->GetTrackLength()/mm<<" mm"<<G4endl;
      step->GetTrack()->SetTrackStatus(fStopAndKill);
    }

  }
  //*/
  //if(particleDef == G4Electron::Electron() and step->GetTrack()->GetTrackStatus()==fStopAndKill){
  //  G4cout<<"Electron track length = "<<step->GetTrack()->GetTrackLength()/mm<<" mm "<<G4endl;
  //}
  if(particleDef == opticalphoton)
  {
    G4StepPoint* endPoint = step->GetPostStepPoint();
    const G4VProcess* pds = endPoint->GetProcessDefinedStep();
    G4String procname     = pds->GetProcessName();
    if(procname.compare("OpRayleigh") == 0)
      fEventAction->AddRayleigh();
    else if(procname.compare("OpAbsorption") == 0)
      fEventAction->AddAbsorption();
    else if(procname.compare("OpMieHG") == 0)
      fEventAction->AddMie();

    // for boundary scattering, process name in 'transportation'.
    // Need to check differently:
    if(endPoint->GetStepStatus() == fGeomBoundary)
    {
      G4OpBoundaryProcessStatus theStatus = Undefined;
      G4ProcessManager* opManager         = opticalphoton->GetProcessManager();
      G4int n_proc = opManager->GetPostStepProcessVector(typeDoIt)->entries();
      G4ProcessVector* postStepDoItVector =
        opManager->GetPostStepProcessVector(typeDoIt);
      for(G4int i = 0; i < n_proc; ++i)
      {
        G4VProcess* currentProcess = (*postStepDoItVector)[i];

        G4OpBoundaryProcess* opProc =
          dynamic_cast<G4OpBoundaryProcess*>(currentProcess);
        if(opProc)
          theStatus = opProc->GetStatus();
      }
      if(theStatus != Undefined && theStatus != NotAtBoundary &&
         theStatus != StepTooSmall)
      {
        fEventAction->AddBoundary();
      }
    }
    //G4cout<<"Next Dir: "<<step->GetTrack()->GetDynamicParticle()->GetMomentumDirection().x()<<" "<<step->GetTrack()->GetDynamicParticle()->GetMomentumDirection().y()<< " "<<step->GetTrack()->GetDynamicParticle()->GetMomentumDirection().z()<<G4endl;
  }
  /* uncomment to trace Cerenkov
  if(particleDef == G4Electron::Electron()){
    G4StepPoint* endPoint = step->GetPostStepPoint();
     if(endPoint->GetPhysicalVolume()){  
    G4cout<<endPoint->GetPhysicalVolume()->GetName() <<G4endl;}
    if(endPoint->GetStepStatus() == fGeomBoundary)
      G4cout<<"Endpoint On Boundary"<<G4endl;

    //if(endPoint->GetPhysicalVolume()){
    //  if(endPoint->GetPhysicalVolume()->GetName()=="Vial" or endPoint->GetPhysicalVolume()->GetName()=="Scint"){
    //      //G4cout<<endPoint->GetPhysicalVolume()->GetName() <<G4endl;
    //      eTrackLength +=step->GetStepLength();
    //  }
    //}
    //if(step->GetTrack()->GetTrackStatus()==fStopAndKill){
    //  //G4cout<<"Track eid "<<G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()<<G4endl;
    //  //fHistoManager->FillNtuple3(eTrackLength,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() ); 
    //  G4cout<<"Track len "<<eTrackLength<<G4endl;
    //  //eTrackLength = 0. ;
    //}
    //if(endPoint->GetPhysicalVolume()){
    //  if(endPoint->GetPhysicalVolume()->GetName()=="Vial" or endPoint->GetPhysicalVolume()->GetName()=="Scint"){
    G4cout<< step->GetTrack()->GetTrackLength() <<","<<endPoint->GetTotalEnergy()/MeV<<","
      << (endPoint->GetTotalEnergy()/MeV>0.7750641?"Cherenkov":"")

    <<","<< step->GetSecondary()->size()
    <<","<< step->GetSecondaryInCurrentStep()->size()<<G4endl;
        //G4cout<< step->GetTrack()->GetTrackStatus() <<G4endl;
        //if(step->GetTrack()->GetTrackStatus()==fStopAndKill or step->GetTrack()->GetTrackStatus()==fStopButAlive){
        //  eTrackLength+=step->GetTrack()->GetTrackLength()-33.75;
        //  G4cout<<step->GetTrack()->GetTrackLength() <<G4endl;
        //}
        
  }*/
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
