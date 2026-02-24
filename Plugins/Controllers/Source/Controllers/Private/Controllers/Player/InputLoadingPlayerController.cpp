/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        [InputLoadingPlayerController.cpp]
 * Created:     [2025-07-20]
 * Description: Core asset-managed player controller, supporting runtime loading and registration of enhanced input assets and mappings.
 * -------------------------------------------------------------------------------
 */

#include "Controllers/Player/InputLoadingPlayerController.h"

#include "AssetManager/AssetManagedLoaderHelper.h"
#include "AssetManager/ControllerAsyncAssetLoaderHelper.h"

#include "DataAssets/Primary/InputConfigDataAssetSet.h"
#include "DataAssets/EnhancedInputAction.h"
#include "DataAssets/EnhancedInputMappingContext.h"

#include "Engine/AssetManager.h"
#include "Enums/InputSettingType.h"

#include "Interfaces/InputAssetInterface.h"

#include "Logging/Log_Controllers.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

 /**
  * Constructor
  * Initializes default state and sets up the supported component interface classes.
  */
AInputLoadingPlayerController::AInputLoadingPlayerController(const FObjectInitializer& ObjectInitializer)
{
	CurrentControlType = EInputSettingType::IST_NONE;
}

void AInputLoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	LoadCounter = 0;
	AssetsToLoadCount = 0;

	LoadPrimaryAssets();
}

void AInputLoadingPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);
	
	CurrentInputSettingTypes.Empty();
}

void AInputLoadingPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	// Reset current control type
	CurrentControlType = EInputSettingType::IST_NONE;

	PreviousInputSettingTypes = CurrentInputSettingTypes;
}

void AInputLoadingPlayerController::LoadPrimaryAssets()
{
	AssetLoaderEvents.OnPrimaryAssetLoaded.AddDynamic(this, &AInputLoadingPlayerController::OnPrimaryAssetLoaded);

	// Kick off
	UE_LOGFMT(Log_InputLoadingController, VeryVerbose, "{Time}: {Line} {Class}: Starting to load actor definition config asset.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	AssetsToLoadCount = UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
		AssetLoaderEvents,
		EAssetDefinitionType::ADT_INPUT,
		this
	);
}

void AInputLoadingPlayerController::UnbindAssetLoaderEventListener()
{
	// Unbind static delegates
	UE_LOGFMT(Log_InputLoadingController, VeryVerbose, "{Time}: {Line} {Class}: Unbinding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	AssetLoaderEvents.OnPrimaryAssetLoaded.RemoveDynamic(
		this, &AInputLoadingPlayerController::OnPrimaryAssetLoaded);
}

void AInputLoadingPlayerController::OnPrimaryAssetLoaded(UObject* InOwner, UObject* LoadedObject)
{
	if (!InOwner)
	{
		UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: Owner is null, cannot process loaded asset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	if (InOwner != this)
	{
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: LoadedObject is null, cannot process loaded asset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_InputLoadingController, Verbose, "{Time}: {Line} {Class}: Setting up loaded asset [{Index}]: {AssetName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Index", LoadCounter),
		("AssetName", LoadedObject->GetFName()));

	if (UInputConfigDataAssetSet* InputConfigDataAsset = Cast<UInputConfigDataAssetSet>(LoadedObject))
	{
		if (InputConfigDataMap.Contains(IInputAssetInterface::Execute_GetInputSettingType(InputConfigDataAsset)))
		{
			UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: InputConfigDataAssetSet for type {Type} already exists. In Name: {NewAsset} Containing Name: {CurrentAsset}. Overwriting.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Type", UEnum::GetValueAsString(IInputAssetInterface::Execute_GetInputSettingType(InputConfigDataAsset))),
				("NewAsset", GetNameSafe(InputConfigDataAsset)),
				("CurrentAsset", GetNameSafe(InputConfigDataMap[IInputAssetInterface::Execute_GetInputSettingType(InputConfigDataAsset)]))
			);
		}

		UE_LOGFMT(Log_InputLoadingController, Verbose, "{Time}: {Line} {Class}: Successfully loaded InputConfigDataAssetSet for type {Type}: {AssetName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Type", UEnum::GetValueAsString(IInputAssetInterface::Execute_GetInputSettingType(InputConfigDataAsset))),
			("AssetName", GetNameSafe(InputConfigDataAsset)));

		InputConfigDataMap.Add(IInputAssetInterface::Execute_GetInputSettingType(InputConfigDataAsset), InputConfigDataAsset);

		LoadMappingsAndActions(InputConfigDataAsset);
	}
	else
	{
		UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: Loaded asset is not of type UInputConfigDataAssetSet. Cannot process. {AssetName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("AssetName", GetNameSafe(LoadedObject)));
	}
}

void AInputLoadingPlayerController::LoadMappingsAndActions(UInputConfigDataAssetSet* DataSet)
{
	if(!DataSet)
	{
		UE_LOGFMT(Log_InputLoadingController, Error, "{Time}: {Line} {Class}: DataSet is null. Cannot load mappings and actions.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!DataSet)
	{
		UE_LOGFMT(Log_InputLoadingController, Error, "{Time}: {Line} {Class}: InputConfigDataAssetSet for type {Type} is null. Cannot load mappings and actions.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Type", UEnum::GetValueAsString(DataSet->InputSettingType)));
		return;
	}
	
	EInputSettingType Type = IInputAssetInterface::Execute_GetInputSettingType(DataSet);
	uint8 TypeAsInt = static_cast<uint8>(Type);

	UE_LOGFMT(Log_InputLoadingController, VeryVerbose, "{Time}: {Line} {Class}: Starting to load Input Mappings and Actions for type {Type} and int {Int}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Type", UEnum::GetValueAsString(Type)),
		("Int", TypeAsInt)
		);

	TArray<TSoftObjectPtr<UObject>> AssetsToLoad = TArray<TSoftObjectPtr<UObject>>();
	TArray<TSoftClassPtr<UObject>> ClassesToLoad = TArray<TSoftClassPtr<UObject>>();

	AssetsToLoad.Append(IInputAssetInterface::Execute_GetInputMappingContexts(DataSet));
	AssetsToLoad.Append(IInputAssetInterface::Execute_GetInputActions(DataSet));

	UControllerAsyncAssetLoaderHelper* LoaderHelper = UAssetManagedLoaderHelper::LoadInputAssetsAsync(
		this,
		AssetsToLoad,
		ClassesToLoad,
		TypeAsInt
	);
	
	if (!LoaderHelper)
	{
		UE_LOGFMT(Log_InputLoadingController, Error, "{Time}: {Line} {Class}: Failed to create UControllerAsyncAssetLoaderHelper for loading Input Mappings and Actions for type {Type}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Type", UEnum::GetValueAsString(IInputAssetInterface::Execute_GetInputSettingType(DataSet))));
		return;
	}

	LoaderHelper->OnAllAssetsLoaded.AddDynamic(this, &AInputLoadingPlayerController::OnInputAssetsLoaded);

	UE_LOGFMT(Log_InputLoadingController, VeryVerbose, "{Time}: {Line} {Class}: Successfully started loading Input Mappings and Actions for type {Type}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Type", UEnum::GetValueAsString(IInputAssetInterface::Execute_GetInputSettingType(DataSet))));
}

void AInputLoadingPlayerController::OnInputAssetsLoaded(uint8 TypeAsInt)
{
	EInputSettingType Type = static_cast<EInputSettingType>(TypeAsInt);

	if (!InputConfigDataMap.Contains(Type))
	{
		UE_LOGFMT(Log_InputLoadingController, Error, "{Time}: {Line} {Class}: InputConfigDataMap does not contain type {Type}. Cannot load mappings and actions.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Type", UEnum::GetValueAsString(Type)));
		return;
	}

	UInputConfigDataAssetSet* InputConfigDataAsset = InputConfigDataMap[Type];

	if (!InputConfigDataAsset)
	{
		UE_LOGFMT(Log_InputLoadingController, Error, "{Time}: {Line} {Class}: InputConfigDataAssetSet for type {Type} is null. Cannot load mappings and actions.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Type", UEnum::GetValueAsString(Type)));
		return;
	}

	// Check if mappings and actions are already loaded
	TArray<TSoftObjectPtr<UObject>> AssetsLoaded = TArray<TSoftObjectPtr<UObject>>();
	AssetsLoaded.Append(IInputAssetInterface::Execute_GetInputMappingContexts(InputConfigDataAsset));
	AssetsLoaded.Append(IInputAssetInterface::Execute_GetInputActions(InputConfigDataAsset));

	TArray<FString> ActionNamesBound;
	bool bAllAssetsLoaded = true;
	for (int32 i = 0; i < AssetsLoaded.Num(); i++)
	{
		if(!AssetsLoaded[i].IsValid())
		{
			UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: Input Asset Context [{Index}] is not valid: {AssetName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Index", i),
				("AssetName", *AssetsLoaded[i].ToString()));
			bAllAssetsLoaded = false;
			continue;
		}

		if (AssetsLoaded[i]->IsA<UEnhancedInputAction>())
		{
			UEnhancedInputAction* InputAction = Cast<UEnhancedInputAction>(AssetsLoaded[i].Get());
			if (!InputAction)
			{
				UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: Input Asset Action [{Index}] is not of type UEnhancedInputAction: {AssetName}",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("Index", i),
					("AssetName", *AssetsLoaded[i].ToString()));
				bAllAssetsLoaded = false;
				continue;
			}
			else
			{
				SetupEnhancedInputAction(InputAction);
				ActionNamesBound.Add(InputAction->GetName());
			}
		}
		else if (AssetsLoaded[i]->IsA<UEnhancedInputMappingContext>())
		{
			UEnhancedInputMappingContext* MappingContext = Cast<UEnhancedInputMappingContext>(AssetsLoaded[i].Get());
			if (!MappingContext)
			{
				UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: Input Asset Context [{Index}] is not of type UEnhancedInputMappingContext: {AssetName}",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("Index", i),
					("AssetName", *AssetsLoaded[i].ToString()));
				bAllAssetsLoaded = false;
				continue;
			}
		}
	}

	if (!bAllAssetsLoaded)
	{
		UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: Not all Input Assets for type {Type} are loaded. Cannot apply mappings and actions.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Type", UEnum::GetValueAsString(Type)));
		return;
	}

	LoadCounter++;

	UE_LOGFMT(Log_InputLoadingController, Verbose, "{Time}: {Line} {Class}: Successfully loaded all Input Mappings and Actions for type {Type}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Type", UEnum::GetValueAsString(Type)));

	// Diagnostic: which actions were bound for this config (readable in log; if empty, add actions to DataAsset in Editor)
	{
		const FString TypeStr = UEnum::GetValueAsString(Type);
		const FString ActionsList = ActionNamesBound.Num() > 0
			? FString::Join(ActionNamesBound, TEXT(", "))
			: TEXT("(none)");
		UE_LOGFMT(Log_InputLoadingController, Log, "---------- Input config loaded: {Type} ----------",
			("Type", TypeStr));
		UE_LOGFMT(Log_InputLoadingController, Log, "  Actions bound (keys will work only if listed here): {Actions}",
			("Actions", ActionsList));
		if (ActionNamesBound.Num() == 0)
		{
			UE_LOGFMT(Log_InputLoadingController, Warning, "  -> No actions in this config. Add entries to 'Input Mapping Actions' in DA_InputConfig_* in Editor.");
		}
		UE_LOGFMT(Log_InputLoadingController, Log, "--------------------------------------------------");
	}
}

void AInputLoadingPlayerController::SetupEnhancedInputMappingContext(UEnhancedInputMappingContext* MappingContext)
{
	if (!IsValid(MappingContext))
	{
		UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: MappingContext is not valid: {MappingContextName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("MappingContextName", *GetNameSafe(MappingContext)));
		return;
	}

	// Add the context to the EnhancedInput subsystem and track it as active
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
		UE_LOGFMT(Log_InputLoadingController, Log, "{Time}: {Line} {Class}: MappingContext {MappingContext} added with priority 0 for control type {ControlType}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("MappingContext", *MappingContext->GetName()), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
	}
	else
	{
		UE_LOGFMT(Log_InputLoadingController, Error, "{Time}: {Line} {Class}: EnhancedInputLocalPlayerSubsystem not found! Cannot bind mapping context for control type {ControlType}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
	}
}

void AInputLoadingPlayerController::SetupEnhancedInputAction(UEnhancedInputAction* InputAction)
{
	if (!IsValid(InputAction))
	{
		UE_LOGFMT(Log_InputLoadingController, Warning, "{Time}: {Line} {Class}: InputAction is not valid: {InputActionName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("InputActionName", *GetNameSafe(InputAction)));
		return;
	}

	UE_LOGFMT(Log_InputLoadingController, Log, "{Time}: {Line} {Class}: Setting up InputAction: {InputActionName} for control type {ControlType}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("InputActionName", *InputAction->GetName()),
		("ControlType", UEnum::GetValueAsString(InputAction->InputActionType)));
}

bool AInputLoadingPlayerController::AllInputAssetsLoaded() const
{
	return LoadCounter > 0 && LoadCounter == AssetsToLoadCount;
}