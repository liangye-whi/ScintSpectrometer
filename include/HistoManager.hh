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
/// \file analysis/AnaEx02/include/HistoManager.hh
/// \brief Definition of the HistoManager class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;
class TTree;
class TH1D;

const G4int kMaxHisto = 1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
  public:
    HistoManager();
   ~HistoManager();
   
    void Book(G4String);
    void Save();

    void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
//    void Normalize(G4int id, G4double fac);    

    void FillNtuple1(G4double, G4int);
    void FillNtuple2(G4double, G4int,G4int);
    void FillNtuple3(G4double, G4int);
//    void FillNtuple(G4double energyAbs, G4double energyGap,
//                    G4double trackLAbs, G4double trackLGap);
//    
    void PrintStatistic();
        
  private:
    TFile*   fRootFile;
    TH1D*    fHisto[kMaxHisto];            
    TTree*   fNtuple1;    
    TTree*   fNtuple2;    
    TTree*   fNtuple3;    

    G4double fEnergy;
    G4int fEventID;
    G4double fPEnergy;
    G4int fPEventID;
    G4int fIsCherenkov;
    G4double fELen;
    G4int fEEventID;
//    G4double fLabs;
//    G4double fLgap;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

