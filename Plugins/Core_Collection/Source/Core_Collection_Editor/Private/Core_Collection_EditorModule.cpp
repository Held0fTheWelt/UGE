// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Core_Collection_EditorModule.h"
#include "Core_CollectionEditorModeCommands.h"

#include "PropertyEditorModule.h"
#include "PropertyViewer/FEntityDefinitionCustomization.h"
#include "GameEditorStyle.h"
#include "CoreEditor.h"
#include "AbilitySystemGlobals.h"
#include "DataValidationModule.h"
#include "Core_Development/Public/CoreDeveloperSettings.h"
#include "Editor/UnrealEdEngine.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "GameplayAbilitiesEditorModule.h"
#include "GameplayCueInterface.h"
#include "GameplayCueNotify_Burst.h"
#include "GameplayCueNotify_BurstLatent.h"
#include "GameplayCueNotify_Looping.h"
#include "AssetTypeActions_CoreContextEffectsLibrary.h"
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "UObject/UObjectIterator.h"
#include "UnrealEdGlobals.h"
#include "Validation/EditorValidator.h"
// ExperienceManagerComponent is from GameFeature_ExperienceSystem when present
// Do not #include .cpp files here - they are compiled as part of the module

class SWidget;

#define LOCTEXT_NAMESPACE "CoreEditor"

// This function tells the GameplayCue editor what classes to expose when creating new notifies.
static void GetGameplayCueDefaultClasses(TArray<UClass*>& Classes)
{
	Classes.Empty();
	Classes.Add(UGameplayCueNotify_Burst::StaticClass());
	Classes.Add(AGameplayCueNotify_BurstLatent::StaticClass());
	Classes.Add(AGameplayCueNotify_Looping::StaticClass());
}

// This function tells the GameplayCue editor what classes to search for GameplayCue events.
static void GetGameplayCueInterfaceClasses(TArray<UClass*>& Classes)
{
	Classes.Empty();

	for (UClass* Class : TObjectRange<UClass>())
	{
		if (Class->IsChildOf<AActor>() && Class->ImplementsInterface(UGameplayCueInterface::StaticClass()))
		{
			Classes.Add(Class);
		}
	}
}

// This function tells the GameplayCue editor where to create the GameplayCue notifies based on their tag.
static FString GetGameplayCuePath(FString GameplayCueTag)
{
	FString Path = FString(TEXT("/Game"));

	//@TODO: Try plugins (e.g., GameplayCue.ShooterGame.Foo should be in ShooterCore or something)

	// Default path to the first entry in the UAbilitySystemGlobals::GameplayCueNotifyPaths.
	if (IGameplayAbilitiesModule::IsAvailable())
	{
		IGameplayAbilitiesModule& GameplayAbilitiesModule = IGameplayAbilitiesModule::Get();

		if (GameplayAbilitiesModule.IsAbilitySystemGlobalsAvailable())
		{
			UAbilitySystemGlobals* AbilitySystemGlobals = GameplayAbilitiesModule.GetAbilitySystemGlobals();
			check(AbilitySystemGlobals);

			TArray<FString> GetGameplayCueNotifyPaths = AbilitySystemGlobals->GetGameplayCueNotifyPaths();

			if (GetGameplayCueNotifyPaths.Num() > 0)
			{
				Path = GetGameplayCueNotifyPaths[0];
			}
		}
	}

	GameplayCueTag.RemoveFromStart(TEXT("GameplayCue."));

	FString NewDefaultPathName = FString::Printf(TEXT("%s/GCN_%s"), *Path, *GameplayCueTag);

	return NewDefaultPathName;
}

static bool HasPlayWorld()
{
	return GEditor->PlayWorld != nullptr;
}

static bool HasNoPlayWorld()
{
	return !HasPlayWorld();
}

static bool HasPlayWorldAndRunning()
{
	return HasPlayWorld() && !GUnrealEd->PlayWorld->bDebugPauseExecution;
}

static void OpenCommonMap_Clicked(const FString MapPath)
{
	if (ensure(MapPath.Len()))
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(MapPath);
	}
}

static bool CanShowCommonMaps()
{
	return HasNoPlayWorld() && !GetDefault<UCoreDeveloperSettings>()->CommonEditorMaps.IsEmpty();
}

static TSharedRef<SWidget> GetCommonMapsDropdown()
{
	FMenuBuilder MenuBuilder(true, nullptr);
	
	for (const FSoftObjectPath& Path : GetDefault<UCoreDeveloperSettings>()->CommonEditorMaps)
	{
		if (!Path.IsValid())
		{
			continue;
		}
		
		const FText DisplayName = FText::FromString(Path.GetAssetName());
		MenuBuilder.AddMenuEntry(
			DisplayName,
			LOCTEXT("CommonPathDescription", "Opens this map in the editor"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateStatic(&OpenCommonMap_Clicked, Path.ToString()),
				FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
				FIsActionChecked(),
				FIsActionButtonVisible::CreateStatic(&HasNoPlayWorld)
			)
		);
	}

	return MenuBuilder.MakeWidget();
}

static void CheckGameContent_Clicked()
{
	UEditorValidator::ValidateCheckedOutContent(/*bInteractive=*/true, EDataValidationUsecase::Manual);
}

static void RegisterGameEditorMenus()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& Section = Menu->AddSection("PlayGameExtensions", TAttribute<FText>(), FToolMenuInsert("Play", EToolMenuInsertType::After));

	FToolMenuEntry CheckContentEntry = FToolMenuEntry::InitToolBarButton(
		"CheckContent",
		FUIAction(
			FExecuteAction::CreateStatic(&CheckGameContent_Clicked),
			FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateStatic(&HasNoPlayWorld)),
		LOCTEXT("CheckContentButton", "Check Content"),
		LOCTEXT("CheckContentDescription", "Runs the Content Validation job on all checked out assets to look for warnings and errors"),
		FSlateIcon(FGameEditorStyle::GetStyleSetName(), "GameEditor.CheckContent")
	);
	CheckContentEntry.StyleNameOverride = "CalloutToolbar";
	Section.AddEntry(CheckContentEntry);

	FToolMenuEntry CommonMapEntry = FToolMenuEntry::InitComboButton(
		"CommonMapOptions",
		FUIAction(
			FExecuteAction(),
			FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateStatic(&CanShowCommonMaps)),
		FOnGetContent::CreateStatic(&GetCommonMapsDropdown),
		LOCTEXT("CommonMaps_Label", "Common Maps"),
		LOCTEXT("CommonMaps_ToolTip", "Some commonly desired maps while using the editor"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Level")
	);
	CommonMapEntry.StyleNameOverride = "CalloutToolbar";
	Section.AddEntry(CommonMapEntry);
}

void FCore_Collection_EditorModule::StartupModule()
{
	FGameEditorStyle::Initialize();

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyEditor.RegisterCustomPropertyTypeLayout(
		"EntityDefinition",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEntityDefinitionCustomization::MakeInstance)
	);

	PropertyEditor.NotifyCustomizationModuleChanged();

	FCore_CollectionEditorModeCommands::Register();

	if (!IsRunningGame())
	{
		FModuleManager::Get().OnModulesChanged().AddRaw(this, &FCore_Collection_EditorModule::ModulesChangedCallback);

		BindGameplayAbilitiesEditorDelegates();

		if (FSlateApplication::IsInitialized())
		{
			ToolMenusHandle = UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateStatic(&RegisterGameEditorMenus));
		}

		FEditorDelegates::BeginPIE.AddRaw(this, &FCore_Collection_EditorModule::OnBeginPIE);
		FEditorDelegates::EndPIE.AddRaw(this, &FCore_Collection_EditorModule::OnEndPIE);
	}

	// Register the Context Effects Library asset type actions (only if GameFeature_FeedbackSystemRuntime is loaded).
	{
		TSharedRef<FAssetTypeActions_CoreContextEffectsLibrary> AssetAction = MakeShared<FAssetTypeActions_CoreContextEffectsLibrary>();
		if (AssetAction->GetSupportedClass())
		{
			IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
			CoreContextEffectsLibraryAssetAction = AssetAction;
			AssetTools.RegisterAssetTypeActions(AssetAction);
		}
	}
}

void FCore_Collection_EditorModule::OnBeginPIE(bool bIsSimulating)
{
	// Note: WarCollection uses ExperienceManagerComponent instead of a subsystem
	// We'll handle PIE experience loading through the GameState component if needed
	// For now, this is a placeholder that can be extended if needed
}

void FCore_Collection_EditorModule::OnEndPIE(bool bIsSimulating)
{
}

void FCore_Collection_EditorModule::ShutdownModule()
{
	// Unregister the Context Effects Library asset type actions.
	{
		FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
		TSharedPtr<IAssetTypeActions> AssetAction = CoreContextEffectsLibraryAssetAction.Pin();
		if (AssetToolsModule && AssetAction)
		{
			AssetToolsModule->Get().UnregisterAssetTypeActions(AssetAction.ToSharedRef());
		}
	}

	FEditorDelegates::BeginPIE.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);

	// Undo UToolMenus
	if (UObjectInitialized() && ToolMenusHandle.IsValid())
	{
		UToolMenus::UnRegisterStartupCallback(ToolMenusHandle);
	}

	UnbindGameplayAbilitiesEditorDelegates();
	FModuleManager::Get().OnModulesChanged().RemoveAll(this);

	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditor = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditor.UnregisterCustomPropertyTypeLayout("EntityDefinition");
	}

	FCore_CollectionEditorModeCommands::Unregister();

	FGameEditorStyle::Shutdown();
}

void FCore_Collection_EditorModule::BindGameplayAbilitiesEditorDelegates()
{
	IGameplayAbilitiesEditorModule& GameplayAbilitiesEditorModule = IGameplayAbilitiesEditorModule::Get();

	GameplayAbilitiesEditorModule.GetGameplayCueNotifyClassesDelegate().BindStatic(&GetGameplayCueDefaultClasses);
	GameplayAbilitiesEditorModule.GetGameplayCueInterfaceClassesDelegate().BindStatic(&GetGameplayCueInterfaceClasses);
	GameplayAbilitiesEditorModule.GetGameplayCueNotifyPathDelegate().BindStatic(&GetGameplayCuePath);
}

void FCore_Collection_EditorModule::UnbindGameplayAbilitiesEditorDelegates()
{
	if (IGameplayAbilitiesEditorModule::IsAvailable())
	{
		IGameplayAbilitiesEditorModule& GameplayAbilitiesEditorModule = IGameplayAbilitiesEditorModule::Get();
		GameplayAbilitiesEditorModule.GetGameplayCueNotifyClassesDelegate().Unbind();
		GameplayAbilitiesEditorModule.GetGameplayCueInterfaceClassesDelegate().Unbind();
		GameplayAbilitiesEditorModule.GetGameplayCueNotifyPathDelegate().Unbind();
	}
}

void FCore_Collection_EditorModule::ModulesChangedCallback(FName ModuleThatChanged, EModuleChangeReason ReasonForChange)
{
	if ((ReasonForChange == EModuleChangeReason::ModuleLoaded) && (ModuleThatChanged.ToString() == TEXT("GameplayAbilitiesEditor")))
	{
		BindGameplayAbilitiesEditorDelegates();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Collection_EditorModule, Core_Collection_Editor)