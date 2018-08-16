

#include <cstdlib>

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSolid.hh"
#include "G4VSensitiveDetector.hh"
#include "TimepixDetector.hh"
#include "G4SDManager.hh"
#include "RootManager.hh"



B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction()
{ }



B1DetectorConstruction::~B1DetectorConstruction()
{ }



G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  

  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;

  //     
  // World
  //


  G4double world_sizeXY = 10*cm;
  G4double world_sizeZ  = 30*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld, world_mat, "World");
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World",0, false, 0, checkOverlaps);
                     
 

 
  //
  // Getting mask configuration from file
  //

  FILE* mask_conf = fopen("../B1/mask-mura-61.txt", "r");
  int mask_size = 61;                    
  int tmp;                        
  
  int** mask_pattern = new int* [mask_size];
  
  for (int i = 0; i < mask_size; i++)       
  {
    mask_pattern[i] = new int [mask_size]; 
    for(int j = 0; j < mask_size; j++)
    {
      fscanf(mask_conf, "%d", &tmp);
      mask_pattern[i][j] = tmp;
    }
  }

  fclose(mask_conf); 

     
 


  //
  //Disc
  //

  G4Material* mask_mat = nist->FindOrBuildMaterial("G4_W");
  G4ThreeVector mask_pos = G4ThreeVector(0, 0, 0);      
  
  G4double mask_outer_radius =  1.5*cm;
  G4double mask_inner_radius =  0*cm;
  G4double mask_thickness = 1.0*mm;
  G4double mask_phi1 = 0*deg;
  G4double mask_phi2 = 360.0*deg;
  G4Tubs* solidMask =    
    new G4Tubs("sMask", mask_inner_radius, mask_outer_radius, mask_thickness/2., mask_phi1, mask_phi2);



  G4LogicalVolume* logicMask =                         
    new G4LogicalVolume(solidMask, mask_mat, "lMask");
                       
  G4VPhysicalVolume* physMask = 
    new G4PVPlacement(0, mask_pos, logicMask, "pMask", logicWorld, false, 0, checkOverlaps);



                         
  //Hole

  G4Material* hole_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4double hole_outer_radius =  0.012*cm;
  G4double hole_inner_radius =  0*cm;
  G4double hole_thickness = 1*mm;
  G4double hole_phi1 = 0.*deg;
  G4double hole_phi2 = 360.*deg;
  G4double hole_step = 0.03*cm;

  //G4Tubs* solidHole = new G4Tubs("sHole", hole_inner_radius, hole_outer_radius, hole_thickness/2., hole_phi1, hole_phi2);     

  G4Box* solidHole = new G4Box("sHole", hole_step/2.0, hole_step/2.0, hole_thickness/2.0);
   




  //
  //Hole placement
  //

  //Mask inversion is switched here
   
  int Inversion = 0;                      //0 - off, 1 - on
  MyROOTManager* RM = MyROOTManager::GetPointer();
  RM -> SetInversion(Inversion);
  
  G4double hole_pos_x, hole_pos_y; 

  G4LogicalVolume* logicHole = new G4LogicalVolume(solidHole, hole_mat, "lHole");


    for(int i = 0; i < mask_size; i++)        //here we place many air holes right into the tungsten plate
      for(int j = 0; j < mask_size; j++)
      {
    
        hole_pos_x = (j - (mask_size-1)/2.0)*hole_step;
        hole_pos_y = ((mask_size-1)/2.0 - i)*hole_step;
        G4ThreeVector hole_pos;

        hole_pos = G4ThreeVector(hole_pos_x, hole_pos_y, 0);
        
        if (mask_pattern[i][j] == Inversion)  
          new G4PVPlacement(0, hole_pos, logicHole, "pHole", logicMask, false, i*mask_size+j, checkOverlaps);  //return values are ignored   
        
      }  


       for (int i = 0; i < mask_size; i++)       
    free(mask_pattern[i]);
  free(mask_pattern);   

  

  //
  //Detector plate
  //

  G4Material* timepix_mat = nist->FindOrBuildMaterial("G4_CADMIUM_TELLURIDE");
  G4ThreeVector timepix_pos = G4ThreeVector(0, 0, 4*cm);
        
  G4double timepix_width = 14*mm;
  G4double timepix_thickness = 0.3*mm;
      

  G4Box* solidTimepix =    
    new G4Box("sTimepix", timepix_width/2., timepix_width/2., timepix_thickness/2.);
                      
  G4LogicalVolume* logicTimepix =                         
    new G4LogicalVolume(solidTimepix, timepix_mat, "lTimepix");
               
  G4VPhysicalVolume* physTimepix = 
    new G4PVPlacement(0, timepix_pos, logicTimepix, "pTimepix", logicWorld, false, 0, checkOverlaps);




//G4VSensitiveDetector setup

TimepixDetector* timepix = new TimepixDetector("/Timepix1");                    
G4SDManager* SDM = G4SDManager::GetSDMpointer(); 
SDM -> AddNewDetector(timepix);
logicTimepix -> SetSensitiveDetector(timepix);



  //
  //always return the physical World
  //
  return physWorld;
}
