#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace B1
{

static const G4int kNumberOfLayers = 30;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    
    static G4int GetNumberOfLayers() { return kNumberOfLayers; }

private:
    G4LogicalVolume* fScoringVolume = nullptr;
};

}  // namespace B1

#endif