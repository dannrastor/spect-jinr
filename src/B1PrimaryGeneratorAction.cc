

#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <time.h>



B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)


{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(140.*keV);
}



B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}



void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double posx, posy, posz, vx, vy, vz, theta, phi, x0, y0;

  //source position set

  posz = -12*cm;
  posx = 0*cm;
  posy = G4UniformRand();
  
  if (posy > 0.5) posy = 1*cm;
  else posy = -1*cm;



  //for general optimization all the particles launched should hit the mask
  //do not modify

  do
  {
    phi = 6.2832*G4UniformRand();
    theta = 0.4*G4UniformRand();

    vx = sin(theta)*cos(phi);   //randomize momentum direction until the intersection point gets on the mask
    vy = sin(theta)*sin(phi);
    vz = cos(theta);

    x0 = posx - posz*vx/vz;   //intersection point
    y0 = posy - posz*vy/vz;
   }while ( !((x0 < 1*cm) && (x0 > -1*cm) && (y0 < 1*cm) && (y0 > -1*cm)) ); 

 



  fParticleGun->SetParticlePosition(G4ThreeVector(posx, posy, posz));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx, vy, vz));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

