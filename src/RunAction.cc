#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

namespace B1
{

RunAction::RunAction()
{
    fLayerEdepSum.resize(kNumberOfLayers, 0.);
}

RunAction::~RunAction()
{
    if (fCsvFile.is_open()) fCsvFile.close();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    fTotalEdepSum = 0.;
    fTotalEdepSum2 = 0.;
    fEventCount = 0;
    for (auto& val : fLayerEdepSum) val = 0.;
    
    fCsvFile.open("calorimeter_data.csv");
    if (fCsvFile.is_open()) {
        fCsvFile << "Event,TotalEdep_MeV";
        for (G4int i = 0; i < kNumberOfLayers; i++) {
            fCsvFile << ",Layer" << i << "_keV";
        }
        fCsvFile << "\n";
    }
}

void RunAction::AddEdep(G4double edep)
{
    fTotalEdepSum += edep;
    fTotalEdepSum2 += edep * edep;
    fEventCount++;
}

void RunAction::AddEventData(G4double totalEdep, const std::vector<G4double>& layerEdep)
{
    fEventCount++;
    fTotalEdepSum += totalEdep;
    fTotalEdepSum2 += totalEdep * totalEdep;
    
    for (G4int i = 0; i < (G4int)layerEdep.size() && i < kNumberOfLayers; i++) {
        fLayerEdepSum[i] += layerEdep[i];
    }
    
    // Запись в CSV
    if (fCsvFile.is_open()) {
        fCsvFile << fEventCount - 1 << "," << totalEdep / MeV;
        for (G4int i = 0; i < (G4int)layerEdep.size() && i < kNumberOfLayers; i++) {
            fCsvFile << "," << layerEdep[i] / keV;
        }
        fCsvFile << "\n";
    }
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;
    
    G4double meanEdep = fTotalEdepSum / nofEvents;
    G4double rmsEdep = std::sqrt(fTotalEdepSum2 / nofEvents - meanEdep * meanEdep);
    
    G4cout << "========================================" << G4endl;
    G4cout << "Run summary:" << G4endl;
    G4cout << "  Events: " << nofEvents << G4endl;
    G4cout << "  Mean energy: " << meanEdep / MeV << " MeV" << G4endl;
    G4cout << "  RMS: " << rmsEdep / MeV << " MeV" << G4endl;
    G4cout << "  Resolution: " << (rmsEdep / meanEdep) * 100 << " %" << G4endl;
    G4cout << "========================================" << G4endl;
    
    G4cout << "Average layer profile:" << G4endl;
    for (G4int i = 0; i < kNumberOfLayers; i++) {
        G4double avg = fLayerEdepSum[i] / nofEvents;
        if (avg > 0) {
            G4cout << "  Layer " << i << ": " << avg / keV << " keV" << G4endl;
        }
    }
    G4cout << "========================================" << G4endl;
    
    if (fCsvFile.is_open()) {
        fCsvFile.close();
        G4cout << "Data saved to calorimeter_data.csv" << G4endl;
    }
}

}  // namespace B1