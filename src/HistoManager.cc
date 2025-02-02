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
/// \file analysis/AnaEx02/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
:fRootFile(0), 
//  fNtuple1(0)
// fNtuple1(0), fNtuple2(0), 
 fNtuple1(0), fNtuple2(0), fNtuple3(0)
// fEabs(0.), fEgap(0.) ,fLabs(0.), fLgap(0.)
{
      
  // histograms
  for (G4int k=0; k<kMaxHisto; k++) fHisto[k] = 0;
    
  // ntuple
  //fNtuple1 = 0;
  //fNtuple2 = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  if (fRootFile) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book(G4String fileName)
{ 
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  //
  fRootFile = new TFile(fileName,"RECREATE");
  if (! fRootFile) {
    G4cout << " HistoManager::Book :" 
           << " problem creating the ROOT TFile "
           << G4endl;
    return;
  }
  else{
    G4cout << "ROOT TFILE "<<fileName<<" Open."<<G4endl;
  }
  
  // id = 0
  fHisto[0] = new TH1D("Energy", "Energy of detected photons (eV)", 100, 0., 20);
//  // id = 1
//  fHisto[1] = new TH1D("EventID", "Edep in gap (MeV)", 100, 0., 100*CLHEP::MeV);
//  // id = 2
//  fHisto[2] = new TH1D("LAbs", "trackL in absorber (mm)", 100, 0., 1*CLHEP::m);
//  // id = 3
//  fHisto[3] = new TH1D("LGap", "trackL in gap (mm)", 100, 0., 50*CLHEP::cm);

  for ( G4int i=0; i<kMaxHisto; ++i ) {
    if (! fHisto[i]) G4cout << "\n can't create histo " << i << G4endl;
  }  

  // create 1st ntuple
  fNtuple1 = new TTree("Ntuple1", "Ephoton");
  fNtuple1->Branch("Energy", &fEnergy, "Energy/D");
  fNtuple1->Branch("EventID", &fEventID, "EventID/I");

  // create 2nd ntuple 
  fNtuple2 = new TTree("Ntuple2", "EPrimaryPhoton");
  fNtuple2->Branch("PEnergy", &fPEnergy, "Energy/D");
  fNtuple2->Branch("PEventID", &fPEventID, "EventID/I");
  fNtuple2->Branch("PIsCherenkov", &fIsCherenkov, "IsCherenkov/I");
 
  fNtuple3 = new TTree("Ntuple3", "EElectron");
  fNtuple3->Branch("ELen", &fELen, "Track Length/D");
  fNtuple3->Branch("EEventID", &fEEventID, "EventID/I");
  G4cout << "\n----> Output file is open in " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{ 
  if (! fRootFile) return;
  
  fRootFile->Write();       // Writing the histograms to the file
  fRootFile->Close();       // and closing the tree (and the file)
  
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= kMaxHisto) {
    G4cerr << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (fHisto[ih]) { fHisto[ih]->Fill(xbin, weight);}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void HistoManager::Normalize(G4int ih, G4double fac)
//{
//  if (ih >= kMaxHisto) {
//    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
//           << " does not exist. (fac=" << fac << ")" << G4endl;
//    return;
//  }
//  if (fHisto[ih]) fHisto[ih]->Scale(fac);
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple1(G4double energy,G4int eid)
{
  fEnergy = energy;
  fEventID = eid;

  if (fNtuple1) fNtuple1->Fill();
}
void HistoManager::FillNtuple2(G4double energy,G4int eid,G4int isC)
{
  fPEnergy = energy;
  fPEventID = eid;
  fIsCherenkov = isC;

  if (fNtuple2) fNtuple2->Fill();
}

void HistoManager::FillNtuple3(G4double len,G4int eid)
{
  fELen =len;
  fEEventID = eid;

  if (fNtuple3) fNtuple3->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  G4cout << "\n ----> print histograms statistic \n" << G4endl;
  for ( G4int i=0; i<kMaxHisto; ++i ) {
    TH1D* h1 = fHisto[i];
    G4cout<<h1->GetEntries()<<G4endl;
    //const G4String name = h1->GetName();  

    //G4String unitCategory;
    //if (name[0] == 'E' ) unitCategory = "Energy"; 
    //if (name[0] == 'L' ) unitCategory = "Length";

    //G4cout << name
    //       << ": mean = " << G4BestUnit(h1->GetMean(), unitCategory) 
    //       << " rms = " << G4BestUnit(h1->GetRMS(), unitCategory ) 
    //       << G4endl;
  }
  G4cout<<"Ntuple2: "<<fNtuple2->GetEntries()<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
