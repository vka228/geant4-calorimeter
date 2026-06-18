/// \file SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4LogicalVolume* volume = step->GetPreStepPoint()
        ->GetTouchableHandle()
        ->GetVolume()
        ->GetLogicalVolume();
    
    G4String volName = volume->GetName();
    G4double edepStep = step->GetTotalEnergyDeposit();
    
    if (volName.contains("Scint") && edepStep > 0) {
        G4int layerID = step->GetPreStepPoint()
            ->GetTouchableHandle()
            ->GetCopyNumber();
        
        G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
        G4double radius = std::sqrt(pos.x()*pos.x() + pos.y()*pos.y());
        
        fEventAction->AddEdepPerLayer(layerID, edepStep);
        
        fEventAction->AddEdepWithRadius(edepStep, radius);
    }
}//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
