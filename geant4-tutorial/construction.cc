#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "construction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Element.hh"
#include "G4IntersectionSolid.hh"

MyDetectorConstruction::MyDetectorConstruction() {}

MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    // Material Definitions
    G4Material* C4H10 = new G4Material("C4H10", 2.4 * g/cm3, 2);
    C4H10->AddElement(nist->FindOrBuildElement("C"), 4);
    C4H10->AddElement(nist->FindOrBuildElement("H"), 10);

    G4Element* Ar = nist->FindOrBuildElement("Ar");

    G4Material* ArC4H10 = new G4Material("ArC4H10", 1.603 * g/cm3, 2);
    ArC4H10->AddMaterial(C4H10, 70 * perCent);
    ArC4H10->AddElement(Ar, 30 * perCent);

    // Optical properties
    G4double energy[2] = {1.239841939 * eV / 0.2, 1.239841939 * eV / 0.9};
    G4double rindexArC4H10[2] = {1.2, 1.2};
    G4double rindexWorld[2] = {1.0, 1.0};

    G4MaterialPropertiesTable* mptArC4H10 = new G4MaterialPropertiesTable();
    mptArC4H10->AddProperty("RINDEX", energy, rindexArC4H10, 2);
    ArC4H10->SetMaterialPropertiesTable(mptArC4H10);

    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4MaterialPropertiesTable* mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    // World volume
    G4VSolid* solidWorldBase = new G4Tubs("solidWorldBase", 0, 50 * cm, 50 * cm, 0, 2 * M_PI);
    G4VSolid* intersectSolidWorld = new G4Tubs("intersectSolidWorld", 0, 50 * cm, 50 * cm, 0, M_PI);
    G4VSolid* solidWorld = new G4IntersectionSolid("solidWorld", solidWorldBase, intersectSolidWorld, 0, G4ThreeVector(0, 0, 0));

    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, true);

    // Radiator volume
    G4VSolid* solidRadiatorBase = new G4Tubs("solidRadiatorBase", 0, 40 * cm, 5 * cm, 0, 2 * M_PI);
    G4VSolid* intersectSolidRadiator = new G4Tubs("intersectSolidRadiator", 0, 40 * cm, 5 * cm, 0, M_PI);
    G4VSolid* solidRadiator = new G4IntersectionSolid("solidRadiator", solidRadiatorBase, intersectSolidRadiator, 0, G4ThreeVector(0, 0, 0));

    G4LogicalVolume* logicRadiator = new G4LogicalVolume(solidRadiator, ArC4H10, "logicalRadiator");
    new G4PVPlacement(0, G4ThreeVector(), logicRadiator, "physRadiator", logicWorld, false, 0, true);

    // Detector array placement remains unchanged...

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField() {
    MySensitiveDetector* sensDet = new MySensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
}
