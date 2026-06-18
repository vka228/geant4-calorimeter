#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include <cmath>
#include <iomanip>
#include <sstream>

namespace B1
{

RunAction::RunAction()
{
    fLayerEdepSum.resize(kNumberOfLayers, 0.);
    fRadialBinsSum.resize(20, 0.);
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
    for (auto& val : fRadialBinsSum) val = 0.;
    
    // Получаем ID потока
    G4int threadId = G4Threading::G4GetThreadId();
    
    // Создаем имя файла с ID потока
    std::ostringstream filename;
    filename << "calorimeter_data_thread" << threadId << ".csv";
    
    fCsvFile.open(filename.str(), std::ios::out | std::ios::trunc);
    if (fCsvFile.is_open()) {
        fCsvFile << "Event,BeamEnergy_GeV,TotalEdep_MeV";
        for (G4int i = 0; i < kNumberOfLayers; i++) {
            fCsvFile << ",Layer" << i << "_keV";
        }
        for (G4int i = 0; i < 20; i++) {
            fCsvFile << ",Radial" << i << "_keV";
        }
        fCsvFile << "\n";
        G4cout << "Thread " << threadId << ": Writing to " << filename.str() << G4endl;
    }
}

void RunAction::AddEventData(G4double beamEnergy,
                            G4double totalEdep,
                            const std::vector<G4double>& layerEdep,
                            const std::vector<G4double>& radialBins)
{
    fEventCount++;
    fTotalEdepSum += totalEdep;
    fTotalEdepSum2 += totalEdep * totalEdep;
    
    for (G4int i = 0; i < (G4int)layerEdep.size() && i < kNumberOfLayers; i++) {
        fLayerEdepSum[i] += layerEdep[i];
    }
    
    for (G4int i = 0; i < (G4int)radialBins.size() && i < (G4int)fRadialBinsSum.size(); i++) {
        fRadialBinsSum[i] += radialBins[i];
    }
    
    // Запись в свой файл
    if (fCsvFile.is_open()) {
        fCsvFile << std::fixed << std::setprecision(6);
        fCsvFile << fEventCount << "," 
                 << beamEnergy / GeV << ","
                 << totalEdep / MeV;
        for (G4int i = 0; i < (G4int)layerEdep.size() && i < kNumberOfLayers; i++) {
            fCsvFile << "," << layerEdep[i] / keV;
        }
        for (G4int i = 0; i < (G4int)radialBins.size(); i++) {
            fCsvFile << "," << radialBins[i] / keV;
        }
        fCsvFile << "\n";
    }
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    if (fCsvFile.is_open()) {
        fCsvFile.flush();
        fCsvFile.close();
        G4int threadId = G4Threading::G4GetThreadId();
        G4cout << "Thread " << threadId << ": Data saved" << G4endl;
    }
    
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;
    
    G4double meanEdep = fTotalEdepSum / nofEvents;
    G4double rmsEdep = std::sqrt(fTotalEdepSum2 / nofEvents - meanEdep * meanEdep);
    
    G4cout << "\n========================================" << G4endl;
    G4cout << "Thread " << G4Threading::G4GetThreadId() << " summary:" << G4endl;
    G4cout << "  Events: " << nofEvents << G4endl;
    G4cout << "  Mean energy: " << meanEdep / MeV << " MeV" << G4endl;
    G4cout << "  RMS: " << rmsEdep / MeV << " MeV" << G4endl;
    G4cout << "  Resolution: " << (rmsEdep / meanEdep) * 100 << " %" << G4endl;
    G4cout << "========================================\n" << G4endl;
}

}  // namespace B1