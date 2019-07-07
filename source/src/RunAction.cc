#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RunAction::RunAction()
 : G4UserRunAction()
{ 
// Create analysis manager
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);        // Histo Id starts from 1 not from0

  // Book histograms, ntuple
  //
  
  // Creating histograms
  analysisManager->CreateH1("Edep","Edep Sum", 100, 0., 1500*keV);  //総和


  // Creating ntuple
  analysisManager->CreateNtuple("NGTHEP", "Edep_br");
  analysisManager->CreateNtupleDColumn("Edep_br");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
 // Get analysis manager and open an output file
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4String fileName = "NGTHEP";
  analysisManager->OpenFile(fileName);
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  //save histograms & ntuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
