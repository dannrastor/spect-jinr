#include "TimepixHit.hh"
#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"


//default container for hit data


TimepixHit::TimepixHit() : G4VHit()
{ }

TimepixHit::~TimepixHit()
{ }


void TimepixHit::Print()
{ }

void TimepixHit::Draw()
{ }

void TimepixHit::AddX(G4double a)
{
	pos_x = a;	
}

void TimepixHit::AddY(G4double a)
{
	pos_y = a;	
}

void TimepixHit::AddE(G4double a)
{
	edep = a;	
}

void TimepixHit::AddK(G4double a)
{
	ekin = a;	
}

G4double TimepixHit::GetX()
{
	return pos_x;
}

G4double TimepixHit::GetY()
{
	return pos_y;
}

G4double TimepixHit::GetE()
{
	return edep;
}

G4double TimepixHit::GetK()
{
	return ekin;
}

