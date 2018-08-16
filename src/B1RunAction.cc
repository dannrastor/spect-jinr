

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <ctime>
#include "RootManager.hh"



B1RunAction::B1RunAction()
: G4UserRunAction()

{ }


B1RunAction::~B1RunAction()
{ }

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
G4Random::setTheEngine(new CLHEP::RanecuEngine);       //these lines generate and set a random seed
G4int Seed = time(NULL);
long seeds[2];
seeds[0]=(long) Seed; 
seeds[1]=(long) (Seed * G4UniformRand());
CLHEP::HepRandom::getTheEngine()->setSeeds(seeds, -1);



MyROOTManager* RM = MyROOTManager::GetPointer();       //output root file is created here
RM -> Initialize();

printf("Begin of a new run\n");
}



void B1RunAction::EndOfRunAction(const G4Run* run)
{ 
	MyROOTManager* RM = MyROOTManager::GetPointer();		//saving data to root file
	RM -> Finalize();
	printf("Done!\n");
}


