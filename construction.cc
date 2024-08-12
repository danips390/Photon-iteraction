#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() : G4VUserDetectorConstruction() 
{
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector construction");
    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");

    nCols=30;
    nRows=30;
    DefineMaterial();


}

MyDetectorConstruction::~MyDetectorConstruction() {}

void MyDetectorConstruction::DefineMaterial()
{

    G4NistManager *nist = G4NistManager::Instance();

    SiO2= new G4Material("SiO2", 2.201*g/cm3, 2 );
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);


    H2O= new G4Material("H2O", 1.000*g/cm3, 2 );
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    C=nist->FindOrBuildElement("C");

    Aerogel=new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel->AddMaterial(SiO2, 62.5*perCent);
    Aerogel->AddMaterial(H2O, 37.4*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4double energy[2]={1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexAerogel[2]={1.1, 1.1};
    G4double rindexWorld[2]={1.0, 1.0};


    Target = nist->FindOrBuildMaterial("G4_Fe");
    DetectorMaterial = nist->FindOrBuildMaterial("G4_Si");
    //Target=nist->FindOrBuildElement("Os");
    //DetectorMaterial=nist->FindOrBuildElement("Si");
    G4MaterialPropertiesTable *mptAerogel= new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
    G4MaterialPropertiesTable *mptworld= new G4MaterialPropertiesTable();
    mptworld->AddProperty("RINDEX", energy, rindexWorld, 2);

    

    Aerogel->SetMaterialPropertiesTable(mptAerogel);
    
    worldMat->SetMaterialPropertiesTable(mptworld);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
    

    G4double xWorld = 0.5*m;
    G4double yworld= 0.5*m;
    G4double zworld=0.5*m;

    solidWorld = new G4Box("solidWorld", xWorld, yworld, zworld); //Pusimos las dimensiones de un cubo

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    PhysWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "PhysWorld", 0, false, 0, true);

    solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.05*m);
    logicalRadiator = new G4LogicalVolume(solidRadiator, Target, "logicalRadiator");

    physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicalRadiator, "physRadiator", logicWorld, false, 0, true);

    solidDetector=new G4Box("solidDetector", xWorld/nRows, yworld/nCols, 0.01*m);

    logicDetector = new G4LogicalVolume(solidDetector, DetectorMaterial, "logicDetector");

    //Vamos a crear un array para todos los detectores sensibles a fotones
    for (G4int i=0; i<nRows; i++)
    {
        for (G4int j=0; j<nCols; j++)
        {
            PhysDetector= new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nRows, -0.5*m+(j+0.5)*m/nCols, 0.49*m), logicDetector, "PhysDetector", logicWorld, false, j+i*nCols, true);
        }
    }

    return PhysWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet= new MySensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
}


