#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
	G4ParticleTable *ParticleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="gamma";
	G4ParticleDefinition *particle = ParticleTable->FindParticle(particleName);

	G4ThreeVector pos(0., 0., 0.);
	G4ThreeVector mom(0., 0., 1.);

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(10.*GeV);
	fParticleGun->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}


void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	fParticleGun->GeneratePrimaryVertex(anEvent);
}