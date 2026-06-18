#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>
#include <fstream>

class G4Run;

namespace B1
{

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    void AddEdep(G4double edep);
    void AddEventData(G4double totalEdep, 
                     const std::vector<G4double>& layerEdep,
                     const std::vector<G4double>& radialBins);

private:
    G4double fTotalEdepSum = 0.;
    G4double fTotalEdepSum2 = 0.;
    G4int fEventCount = 0;
    std::vector<G4double> fLayerEdepSum;
    std::vector<G4double> fRadialBinsSum;  
    
    std::ofstream fCsvFile;
};

}  // namespace B1

#endif