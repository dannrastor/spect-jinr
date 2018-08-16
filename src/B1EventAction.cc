

#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"


B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)

{ } 



B1EventAction::~B1EventAction()
{ }


void B1EventAction::BeginOfEventAction(const G4Event*)
{ }


void B1EventAction::EndOfEventAction(const G4Event* event)
{ 
	
	//print simulation progress to console

	G4int id = 1 + event -> GetEventID();
	if (id % 200000 == 0) printf("%d events processed...\n", id);
}


