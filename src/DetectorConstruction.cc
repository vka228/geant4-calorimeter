#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4String.hh"  // Добавляем для G4String

namespace B1
{

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // standard import + calorimeter materials
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");
    G4Material* scint = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    
    // world definition
    G4double worldSize = 2.0 * m;
    G4Box* solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), 
                                                     logicWorld, "World", 
                                                     0, false, 0, true);
    
    // calorimeter params
    G4int nLayers = 30;
    G4double pbThick = 2.0 * mm;
    G4double scintThick = 4.0 * mm;
    G4double calorSize = 30.0 * cm;
    
    G4double layerThick = pbThick + scintThick;
    G4double totalThick = nLayers * layerThick;
    
    // air contaier
    G4Box* solidContainer = new G4Box("Container", 
                                      calorSize/2, calorSize/2, totalThick/2);
    G4LogicalVolume* logicContainer = new G4LogicalVolume(solidContainer, air, "Container");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
                     logicContainer, "Container", 
                     logicWorld, false, 0, true);
    
    // calorimeter itself
    for (G4int i = 0; i < nLayers; i++) {
        G4double zPos = -totalThick/2 + (i + 0.5) * layerThick;
        
        // Pb
        G4String pbName = "Pb_" + std::to_string(i);
        G4String pbLVName = "Pb_LV_" + std::to_string(i);
        
        G4Box* solidPb = new G4Box(pbName, 
                                   calorSize/2, calorSize/2, pbThick/2);
        G4LogicalVolume* logicPb = new G4LogicalVolume(solidPb, lead, pbLVName);
        new G4PVPlacement(0, G4ThreeVector(0, 0, zPos - scintThick/2), 
                         logicPb, pbName, 
                         logicContainer, false, i, true);

        G4VisAttributes* pbVis = new G4VisAttributes(G4Colour(0.3 , 0.3, 0.3));
        logicPb->SetVisAttributes(pbVis);
        
        // scinc 
        G4String scintName = "Scint_" + std::to_string(i);
        G4String scintLVName = "Scint_LV_" + std::to_string(i);
        
        G4Box* solidScint = new G4Box(scintName, 
                                      calorSize/2, calorSize/2, scintThick/2);
        G4LogicalVolume* logicScint = new G4LogicalVolume(solidScint, scint, scintLVName);
        new G4PVPlacement(0, G4ThreeVector(0, 0, zPos + pbThick/2), 
                         logicScint, scintName, 
                         logicContainer, false, i, true);
        
        G4VisAttributes* scintVis = new G4VisAttributes(G4Colour(0.2, 0.3 + 0.6*(i%2), 0.8));
        logicScint->SetVisAttributes(scintVis);
        
        if (i == 0) {
            fScoringVolume = logicScint;
        }
    }
    
    // visualization
    G4VisAttributes* worldVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.1));
    logicWorld->SetVisAttributes(worldVis);
    logicContainer->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    // some logs
    G4cout << "====================================" << G4endl;
    G4cout << "Calorimeter constructed:" << G4endl;
    G4cout << "  Layers: " << nLayers << G4endl;
    G4cout << "  Pb thickness: " << pbThick/mm << " mm" << G4endl;
    G4cout << "  Scint thickness: " << scintThick/mm << " mm" << G4endl;
    G4cout << "  Total thickness: " << totalThick/cm << " cm" << G4endl;
    G4cout << "  Transverse size: " << calorSize/cm << " cm" << G4endl;
    G4cout << "====================================" << G4endl;
    
    return physWorld;
}

} // namespace B1