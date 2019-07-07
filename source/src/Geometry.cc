//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geometry.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Geometry.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

// use for MultiFumctionalDetector and PrimitiveScorer
//#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4MultiSensitiveDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

//------------------------------------------------------------------------------
  Geometry::Geometry() {}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
  Geometry::~Geometry() {}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
  G4VPhysicalVolume* Geometry::Construct()
//------------------------------------------------------------------------------
{
// Get pointer to 'Material Manager'
   G4NistManager* materi_Man = G4NistManager::Instance();


//Define Material
   /*G4int z;
   G4double a;
   G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);
   G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
   G4Element* C = new G4Element("Oxygen"  , "C", z=6 , a=12.01*g/mole);*/

   G4int nelements, ncomponents;
   G4double density;
   G4bool iso = true;

   G4Element* H = materi_Man->FindOrBuildElement("H",iso) ;
   //G4Element* O = materi_Man->FindOrBuildElement("O",iso) ;
   G4Element* C = materi_Man->FindOrBuildElement("C",iso) ;

   /*G4Material* Water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
   Water->AddElement(H, 2);
   Water->AddElement(O, 1);*/

   G4Material* PVT = new G4Material("PVT", density=1.032*g/cm3, nelements=2);//Polyvinyltoluene
   PVT->AddElement(C,9);
   PVT->AddElement(H,10);
   
   G4Material* BC408= new G4Material("BC408", density=1.032*g/cm3, ncomponents=1);
   BC408->AddMaterial(PVT, 100*perCent);


// Define 'World Volume'
   // Define the shape of solid
   G4double leng_X_World = 2.0*m;         // X-full-length of world 
   G4double leng_Y_World = 2.0*m;         // Y-full-length of world 
   G4double leng_Z_World = 2.0*m;         // Z-full-length of world 
   G4Box* solid_World = 
     new G4Box("Solid_World", leng_X_World/2.0, leng_Y_World/2.0, leng_Z_World/2.0);

   // Define logical volume
   G4Material* materi_World = materi_Man->FindOrBuildMaterial("G4_AIR");
   G4LogicalVolume* logVol_World = new G4LogicalVolume(solid_World, materi_World, "LogVol_World");
   logVol_World->SetVisAttributes (G4VisAttributes::Invisible);

   // Placement of logical volume
   G4int copyNum_World = 0;               // Set ID number of world
   G4PVPlacement* physVol_World  = 
     new G4PVPlacement(G4Transform3D(), "PhysVol_World", logVol_World, 0, 
                       false, copyNum_World);


// Define Scintillator
// Define 'Rectangle'
   // Define the shape of solid
   G4double leng_X_Rectangle = 40.0*mm;
   G4double leng_Y_Rectangle = 75.0*mm;
   G4double leng_Z_Rectangle = 3.0*mm;
   G4Box* solid_Rectangle = new G4Box( "Solid_Rectangle", leng_X_Rectangle/2.0, leng_Y_Rectangle/2.0, leng_Z_Rectangle/2.0 );

   // Define logical volume
   //G4Material* materi_Rectangle = materi_Man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
   G4Material* materi_Rectangle = BC408;
   G4LogicalVolume* logVol_Rectangle =  new G4LogicalVolume( solid_Rectangle, materi_Rectangle, "LogVol_Rectangle", 0, 0, 0 );

   // Placement of logical volume
   G4double pos_X_LogV = 0.0*cm;           // X-location LogV 
   G4double pos_Y_LogV = 0.0*cm;           // Y-location LogV
   G4double pos_Z_LogV = 0.0*cm;           // Z-location LogV
   G4ThreeVector threeVect_LogV = G4ThreeVector(pos_X_LogV, pos_Y_LogV, pos_Z_LogV);
   G4RotationMatrix rotMtrx_LogV = G4RotationMatrix();
   G4Transform3D trans3D_LogV = G4Transform3D(rotMtrx_LogV, threeVect_LogV);
   G4int copyNum_LogV = 1000;                // Set ID number of LogV
   new G4PVPlacement(trans3D_LogV, "PhysVol_Rectangle", logVol_Rectangle, physVol_World, false, copyNum_LogV);


// define my sensitive volume
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
    // Sensitive detector is a MultiFunctionalDetector
    G4MultiFunctionalDetector* NGTHEPDetector = new G4MultiFunctionalDetector("NGTHEP_MFD");
    G4SDManager::GetSDMpointer()->AddNewDetector(NGTHEPDetector);
    G4VPrimitiveScorer*  primitive = new G4PSEnergyDeposit("Edep");
    NGTHEPDetector->RegisterPrimitive(primitive);
    SetSensitiveDetector("LogVol_Rectangle",NGTHEPDetector);


// Return the physical world
    return physVol_World;
}
