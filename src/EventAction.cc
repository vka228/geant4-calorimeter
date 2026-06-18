#include "EventAction.hh"
#include "RunAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

namespace B1
{

EventAction::EventAction(RunAction* runAction) : fRunAction(runAction)
{
    fEdepPerLayer.resize(30, 0.);  
}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    for (auto& edep : fEdepPerLayer) {
        edep = 0.;
    }
}

void EventAction::AddEdepPerLayer(G4int layerID, G4double edep)
{
    if (layerID < 0 || layerID >= (G4int)fEdepPerLayer.size()) return;
    fEdepPerLayer[layerID] += edep;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4cout << "Event " << event->GetEventID() 
           << ": FINAL Edep = " << fEdep / MeV << " MeV" << G4endl;
    
    fRunAction->AddEventData(fEdep, fEdepPerLayer);
}

}  // namespace B1