#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <fstream>
#include <vector>
#include <mutex>

namespace B1
{

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;
    
    void AddEventData(G4double beamEnergy,
                     G4double totalEdep,
                     const std::vector<G4double>& layerEdep,
                     const std::vector<G4double>& radialBins);

private:
    std::ofstream fCsvFile;
    G4double fTotalEdepSum;
    G4double fTotalEdepSum2;
    G4int fEventCount;
    std::vector<G4double> fLayerEdepSum;
    std::vector<G4double> fRadialBinsSum;
};

}

#endif