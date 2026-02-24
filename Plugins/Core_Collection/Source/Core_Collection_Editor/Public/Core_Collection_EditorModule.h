// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Modules/ModuleManager.h"

class IAssetTypeActions;

/**
 * This is the module definition for the editor mode. You can implement custom functionality
 * as your plugin module starts up and shuts down. See IModuleInterface for more extensibility options.
 */
class CORE_COLLECTION_EDITOR_API FCore_Collection_EditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnBeginPIE(bool bIsSimulating);
	void OnEndPIE(bool bIsSimulating);

	void BindGameplayAbilitiesEditorDelegates();
	void UnbindGameplayAbilitiesEditorDelegates();
	void ModulesChangedCallback(FName ModuleThatChanged, EModuleChangeReason ReasonForChange);

private:
	TWeakPtr<IAssetTypeActions> CoreContextEffectsLibraryAssetAction;
	FDelegateHandle ToolMenusHandle;
};
