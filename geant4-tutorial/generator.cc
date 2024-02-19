#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName="mu-";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
    
    G4ThreeVector pos(0.,0.,0.);

    // Angle in the xz-plane is 45 degrees
    // For simplicity, assuming y-component is 0 for a 45-degree angle in xz-plane
    G4double angleXZ = 90. * deg; // Converting degrees to radians if needed
    G4double cosAngle = std::cos(angleXZ);
    G4double sinAngle = std::sin(angleXZ);

    // Assuming the particle is moving in the xz-plane with a 45-degree angle
    G4ThreeVector mom(cosAngle, 0., sinAngle);
    
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(10.*GeV);
    fParticleGun->SetParticleDefinition(particle);
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
