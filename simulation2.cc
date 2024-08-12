#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include"construction.hh"
#include"physics.hh"
#include "action.hh"

int main(int argc, char ** argv)
{
	// Crear el administrador de la simulación
	G4RunManager *runManager = new G4RunManager();

	runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());

	runManager->Initialize();

	// Crear la interfaz de usuario interactiva
	G4UIExecutive *ui = 0;
	if (argc==1)
	{
		ui = new G4UIExecutive(argc, argv);
	}
	

	// Crear el administrador de visualización y inicializarlo
	G4VisManager *visManager = new G4VisExecutive();
	visManager -> Initialize();
	// Obtener el puntero al administrador de la interfaz de usuario
	G4UImanager *UImanager =  G4UImanager::GetUIpointer();

	if (ui)
	{
		UImanager->ApplyCommand("/control/execute vis.mac");

		// Iniciar la sesión interactiva de usuario
		ui->SessionStart();

	}

	else
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);

	}




	return 0;
}