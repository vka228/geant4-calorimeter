#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

namespace B1
{

EventAction::EventAction(RunAction* runAction) : fRunAction(runAction)
{
    fEdepPerLayer.resize(kNumberOfLayers, 0.);
    fRadialBins.resize(20, 0.);  
}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    for (auto& edep : fEdepPerLayer) edep = 0.;
    for (auto& val : fRadialBins) val = 0.;
}

void EventAction::AddEdepPerLayer(G4int layerID, G4double edep)
{
    if (layerID < 0 || layerID >= (G4int)fEdepPerLayer.size()) return;
    fEdepPerLayer[layerID] += edep;
}

void EventAction::AddEdepWithRadius(G4double edep, G4double radius)
{
    
    fEdep += edep;
    
    G4int bin = (G4int)(radius / cm);  
    if (bin >= 0 && bin < (G4int)fRadialBins.size()) {
        fRadialBins[bin] += edep;
    }
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4cout << "Event " << event->GetEventID() 
           << ": FINAL Edep = " << fEdep / MeV << " MeV" << G4endl;
    
    fRunAction->AddEventData(fEdep, fEdepPerLayer, fRadialBins);
}

}  // namespace B1