#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class G4Event;

namespace B1
{

class RunAction;

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction);
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddEdep(G4double edep) { fEdep += edep; }
    void AddEdepPerLayer(G4int layerID, G4double edep);

private:
    RunAction* fRunAction = nullptr;
    G4double fEdep = 0.;
    std::vector<G4double> fEdepPerLayer;
};

}  // namespace B1

#endif