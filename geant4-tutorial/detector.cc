#include "detector.hh"
#include <fstream>
#include <vector>
#include "G4OpticalPhoton.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *R0hist)
{

	G4Track *track = aStep->GetTrack();

   	 // Check if the track is an optical photon
  	  if(track->GetDefinition() != G4OpticalPhoton::Definition())
  	  {
   	     return false; // Not an optical photon, do not process this hit
  	  }

	track->SetTrackStatus(fStopAndKill);

	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

	G4ThreeVector posPhoton = preStepPoint->GetPosition();

	//G4cout << "Photon position: " << posPhoton << G4endl;

	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

	G4int copyNo = touchable->GetCopyNumber();

	//G4cout << "Copy number: " << copyNo << G4endl;

	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();

	//G4cout << "Detector position: " << posDetector << G4endl;


	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0, evt);
	man->FillNtupleDColumn(1, posDetector[0]);
	man->FillNtupleDColumn(2, posDetector[1]);
	man->FillNtupleDColumn(3, posDetector[2]);
	man->AddNtupleRow(0);

    // Open a text file for writing
    std::ofstream outFile("output.txt", std::ios::app); // Change "output.txt" to your desired file name

    // Check if the file is open
    if (outFile.is_open()) {
        // Write the position data to the file
        outFile << posDetector[0] << " " << posDetector[1] << std::endl;

        // Close the file
        outFile.close();
    } else {
        G4cout << "Error opening the output file." << G4endl;
    }

    return true;

}