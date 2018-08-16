#include  "G4VSensitiveDetector.hh"
#include "TimepixDetector.hh"
#include "TimepixHit.hh"
#include "G4THitsCollection.hh"
#include "G4VHitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "RootManager.hh"

TimepixDetector::TimepixDetector(G4String name) 
: G4VSensitiveDetector(name)
{ 
	collectionName.insert("TimepixHitCollection");
}

TimepixDetector::~TimepixDetector()
{ }

G4bool TimepixDetector::ProcessHits(G4Step* step, G4TouchableHistory* hist)
{ 
	

	TimepixHit* hit = new TimepixHit();

	G4StepPoint* point = step->GetPreStepPoint();
	G4ThreeVector pos = point->GetPosition();

	G4double edep = step -> GetTotalEnergyDeposit();
	G4double kin = step -> GetTrack() -> GetKineticEnergy();

	hit -> AddX(pos.x());
	hit -> AddY(pos.y());
	hit -> AddE(edep);
	hit -> AddK(kin);

	 


	THC -> insert(hit);
}

void TimepixDetector::Initialize(G4HCofThisEvent* HCE)
{ 
	
	//default hit storing system from examples

	THC = new TimepixHitCollection(GetName(), collectionName[0]);
	static G4int HCID = -1;
	if (HCID < 0 ) HCID = GetCollectionID(0);
	HCE -> AddHitsCollection(HCID, THC);
	//HCE -> AddHitsCollection(HCID, hitsCollection);
}

void TimepixDetector::EndOfEvent(G4HCofThisEvent* HCE)
{ 
	G4int id = G4SDManager::GetSDMpointer() -> GetCollectionID("Timepix1/TimepixHitCollection");
	TimepixHitCollection* coll = (TimepixHitCollection*)HCE -> GetHC(id);	

	G4int number = coll -> entries();  //number of hits in this event

	G4double hit_x = 0;
	G4double hit_y = 0;
	G4double energy = 0;
	G4double kinetic = 0;


	//after end of each event hit collection is reviewed to analyze hits
	//in case there were multiple simulation steps (hits) in detector, mean values are calculated
	

	for (int i = 0; i < number; i++)
	{
		TimepixHit* hit = (*coll)[i];
	
		hit_x+= hit -> GetX();
		hit_y+= hit -> GetY();
		energy+= hit -> GetE();
		kinetic+= hit -> GetK();		
	}

	
	//store data to ROOT file if hit amount is nonzero 

	if (number != 0)
	{
		hit_x = hit_x/mm/number;
		hit_y = hit_y/mm/number;
		energy = energy/keV;
		kinetic = kinetic/keV/number;

		MyROOTManager* RM = MyROOTManager::GetPointer();		
		RM -> FillHist(hit_x, hit_y);
		RM -> FillTree(hit_x, hit_y, energy, kinetic);
	}


}

	


