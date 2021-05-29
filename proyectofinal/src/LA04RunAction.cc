/* 
 * LA04RunAction.cc: Implementación para la clase
 * LA04RunAction.
 * 
 * Archivo de ejemplo de Geant4 para la unidad 3
 * del curso de Laboratorio Avanzado ECFM-USAC
 * 
 * Héctor Pérez
 * abril 2021
 * 
 * Basado en el ejemplo B1 de Geant4.10.06.p03
 */

#include "LA04RunAction.hh"
#include "LA04PrimaryGeneratorAction.hh"
#include "LA04DetectorConstruction.hh"
// #include "LA04Run.hh"
#include <fstream>

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"
#include "g4root.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LA04RunAction::LA04RunAction()
: G4UserRunAction(),
  fEdep(0.),
  fDistance(0.),
  fNump(0.)
  
{ 
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fDistance);
  accumulableManager->RegisterAccumulable(fNump);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LA04RunAction::~LA04RunAction()
{



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LA04RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LA04RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  G4double edep  = fEdep.GetValue();
  G4double Distance  = fDistance.GetValue();
  G4double Nump  = fNump.GetValue();

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const LA04PrimaryGeneratorAction* generatorAction
   = static_cast<const LA04PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << "Energy deposited per run, in scoring volume : " 
     << G4BestUnit(edep,"Energy")
     << " Distance traveled per run, in scoring volume : " 
     << G4BestUnit(Distance,"Length")
     << G4endl
     << "Electrones generados: "
     << Nump
     << G4endl
     << "Número de eventos: "
     << nofEvents
     << " Eficiencia : " 
     << Nump/nofEvents
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;


    /*Escritura en un archivo de texto con la eficiencia */

     std::ofstream datos;   //Se abre un archivo de texto
     datos.open("eficienciaenerg.txt",std::ios::app);

     datos <<Nump/nofEvents<< std::endl; //Se rellena con la eficiencia en cada corrida. 
     datos.close();



}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LA04RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LA04RunAction::AddDistance(G4double distance)
{
  fDistance  += distance;
}

void LA04RunAction::AddNump(G4double nump)
{
  fNump  += nump;
}

