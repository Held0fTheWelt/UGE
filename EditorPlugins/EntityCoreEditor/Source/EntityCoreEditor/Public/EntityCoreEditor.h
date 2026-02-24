#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"

DECLARE_LOG_CATEGORY_EXTERN(EntityCoreEditor, Log, All);

/**
 * Core editor module responsible for handling editor functionality of the Entity system.
 */
class FEntityCoreEditor : public IModuleInterface
{
public:
	//~ Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~ End IModuleInterface

	/** Öffnet den konfigurierten Entity Editor als UtilityWidget */
	static void OpenEntityEditor();

private:
	/** Wird aufgerufen, wenn der Menüeintrag im World Outliner angeklickt wird */
	static void HandleEntityEditorRequest();

	/** Nur im Play-/Simulate-Modus aktiv */
	static bool IsPlayWorldActive();

	/** Registriert den Menüeintrag im World Outliner Kontextmenü */
	static void RegisterOutlinerMenu();

public:
	/** Aktive Tab ID */
	static FName TabID;

	/** CommandList für Menüeinträge */
	static TSharedPtr<class FUICommandList> CommandList;
};
