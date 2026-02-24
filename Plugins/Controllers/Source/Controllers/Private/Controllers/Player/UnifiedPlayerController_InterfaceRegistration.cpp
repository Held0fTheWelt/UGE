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
 * File:        [aaa.cpp]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#include "Controllers/Player/UnifiedPlayerController.h"

#include "DataAssets/Primary/InputConfigDataAssetSet.h"
#include "DataAssets/EnhancedInputMappingContext.h"

#include "Interfaces/Control/PawnControlInterface.h"
#include "Interfaces/Control/HumanoidControlInterface.h"
#include "Interfaces/Control/VehicleControlInterface.h"
#include "Interfaces/Control/InteractControlInterface.h"
#include "Interfaces/Control/EngineInputControlInterface.h"
#include "Interfaces/InputAssetInterface.h"
#include "Interfaces/InputControllerInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Logging/Log_Controllers.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AUnifiedPlayerController::RegisterComponentWithInterface_Implementation(UActorComponent* Component)
{
	if (!Component)
	{
		UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Component is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	if (NotifyComponentRegisterChanged(Component, true))
	{
		UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Registered new component {ComponentName} with interface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ComponentName", *Component->GetName()));
	}
	else
	{
		UE_LOGFMT(Log_UnifiedController_Register, Verbose, "{Time}: {Line} {Class}: Failed to register component {ComponentName}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ComponentName", *Component->GetName()));
	}
}

void AUnifiedPlayerController::UnregisterComponentWithInterface_Implementation(UActorComponent* Component)
{
	if (!Component)
	{
		UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Component is null, cannot unregister.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	if (NotifyComponentRegisterChanged(Component, false))
	{
		UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Unregistered component {ComponentName} with interface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ComponentName", *Component->GetName()));
	}
	else
	{
		UE_LOGFMT(Log_UnifiedController_Register, Verbose, "{Time}: {Line} {Class}: Failed to unregister component {ComponentName} with interface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ComponentName", *Component->GetName()));
	}
}

bool AUnifiedPlayerController::NotifyComponentRegisterChanged(UActorComponent* Component, bool Registering)
{
	if (Component)
	{
		EInputConfiguration CurrentControlConfiguration = EInputConfiguration::IC_NONE;

		if (Component->GetClass()->ImplementsInterface(UInputControllerInterface::StaticClass()))
		{
			CurrentControlConfiguration = IInputControllerInterface::Execute_GetInputConfiguration(Component);
		}

		bool bAlreadyRegistered = false;
		TArray<UClass*> TargetInterfaceClasses = GetComponentInterfaceClass(Component);
		if (TargetInterfaceClasses.Num() == 0)
		{
			UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Component {ComponentName} does not implement any registered interface.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ComponentName", *Component->GetName()));
			return false;
		}

		for (UClass* TargetInterfaceClass : TargetInterfaceClasses)
		{
			UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Registering component {ComponentName} with interface {InterfaceClass}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
				("ComponentName", *Component->GetName()), ("InterfaceClass", *TargetInterfaceClass->GetName()));
		}

		TArray<bool> ActionPerformed = TArray<bool>();
		ActionPerformed.Init(false, TargetInterfaceClasses.Num());

		for(int32 i = 0; i < TargetInterfaceClasses.Num(); i++)
		{
			UClass* TargetInterfaceClass = TargetInterfaceClasses[i];

			bAlreadyRegistered = RegisteredComponentsByClass.Contains(TargetInterfaceClass) && RegisteredComponentsByClass[TargetInterfaceClass] == Component;	

			if (RegisteredComponentsByClass.Contains(TargetInterfaceClass))
			{
				if (RegisteredComponentsByClass[TargetInterfaceClass] != Component)
				{
					UE_LOGFMT(Log_UnifiedController_Register, Verbose, "{Time}: {Line} {Class}: Component {ComponentName} is already registered with a different component for interface {InterfaceClass}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("ComponentName", *Component->GetName()), ("InterfaceClass", *TargetInterfaceClass->GetName()));
					UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
					if (!InputSubsystem)
					{
						UE_LOGFMT(Log_UnifiedController, Error, "{Time}: {Line} {Class}: EnhancedInputLocalPlayerSubsystem not found! Cannot unbind mapping context.",
							("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
						return false;
					}
					if (InputConfigDataMap[GetInputSettingForClass(TargetInterfaceClass)]->InputMappingContexts.Num() > 0)
					{
						for(const auto& MappingContext : InputConfigDataMap[GetInputSettingForClass(TargetInterfaceClass)]->InputMappingContexts)
						{
							if (MappingContext.IsValid())
							{
								InputSubsystem->RemoveMappingContext(MappingContext.Get());
							}
						}
					}
					RegisteredComponentsByClass[TargetInterfaceClass] = Component;
					ActionPerformed[i] = true;
					
					AddInputMapping(GetInputSettingForClass(TargetInterfaceClass), EInputConfiguration::IC_NONE);
					continue;
				}
			}

			if (Registering)
			{
				if (!bAlreadyRegistered)
				{
					RegisteredComponentsByClass.Add(TargetInterfaceClass, Component);
					UE_LOGFMT(Log_UnifiedController_Register, Log, "{Time}: {Line} {Class}: Interface registered successfully: {InterfaceClass} -> {ComponentName}",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("InterfaceClass", *TargetInterfaceClass->GetName()), ("ComponentName", *Component->GetName()));
					UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Successfully registered component {ComponentName} with interface {InterfaceClass}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("ComponentName", *Component->GetName()), ("InterfaceClass", *TargetInterfaceClass->GetName()));
					ActionPerformed[i] = true;
					AddInputMapping(GetInputSettingForClass(TargetInterfaceClass), EInputConfiguration::IC_NONE);
					continue;
				}
				else
				{
					UE_LOGFMT(Log_UnifiedController_Register, Verbose, "{Time}: {Line} {Class}: Component {ComponentName} is already registered with a different component for interface {InterfaceClass}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("ComponentName", *Component->GetName()), ("InterfaceClass", *TargetInterfaceClass->GetName()));
					UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
					if (!InputSubsystem)
					{
						UE_LOGFMT(Log_UnifiedController, Error, "{Time}: {Line} {Class}: EnhancedInputLocalPlayerSubsystem not found! Cannot unbind mapping context.",
							("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
						return false;
					}
					if (InputConfigDataMap[GetInputSettingForClass(TargetInterfaceClass)]->InputMappingContexts.Num() > 0)
					{
						for (const auto& MappingContext : InputConfigDataMap[GetInputSettingForClass(TargetInterfaceClass)]->InputMappingContexts)
						{
							if (MappingContext.IsValid())
							{
								InputSubsystem->RemoveMappingContext(MappingContext.Get());
								UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Removed mapping context {MappingContextName} for control type {ControlType}.",
									("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
									("MappingContextName", *MappingContext->GetName()), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
							}
						}
					}
					RegisteredComponentsByClass[TargetInterfaceClass] = Component;
					ActionPerformed[i] = true;
					AddInputMapping(GetInputSettingForClass(TargetInterfaceClass), EInputConfiguration::IC_NONE);
					continue;
				}
			}
			else
			{
				if (bAlreadyRegistered)
				{
					if (RegisteredComponentsByClass.Remove(TargetInterfaceClass) > 0)
					{
						UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Successfully unregistered component {ComponentName} with interface {InterfaceClass}.",
							("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
							("ComponentName", *Component->GetName()), ("InterfaceClass", *TargetInterfaceClass->GetName()));
						ActionPerformed[i] = true;
						continue;
					}
				}
				else
				{
					UE_LOGFMT(Log_UnifiedController_Register, Log,
						"{Time}: {Line} {Class}: Component {ComponentName} is not registered with interface {InterfaceClass}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION),
						("Line", GET_LINE_NUMBER),
						("Time", GET_CURRENT_TIME),
						("ComponentName", Component ? *Component->GetName() : TEXT("nullptr")),
						("InterfaceClass", TargetInterfaceClass ? *TargetInterfaceClass->GetName() : TEXT("nullptr"))
					);
					ActionPerformed[i] = false;
					continue;
				}
			}
			for(int32 j = 0; j < ActionPerformed.Num(); j++)
			{
				if (!ActionPerformed[j])
				{
					UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Failed to register/unregister component {ComponentName} with interface {InterfaceClass}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("ComponentName", *Component->GetName()), ("InterfaceClass", *TargetInterfaceClasses[j]->GetName()));
					return false;
				}
			}
		}
		return true;
	}
	else
	{
		UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Component is null, cannot register/unregister.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	}
	return false;
}

void AUnifiedPlayerController::AddInputMapping(EInputSettingType SettingsType, EInputConfiguration Configuration)
{
	if (InputConfigDataMap.Contains(SettingsType))
	{
		if (auto InputConfigDataSet = InputConfigDataMap[SettingsType])
		{
			if (Configuration == EInputConfiguration::IC_NONE)
			{
				UE_LOGFMT(Log_UnifiedController_Register, Display, "{Time}: {Line} {Class}: Using IC_NONE for input mapping: adding all contexts for settings type {SettingsType} (logical assignment).",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
					("SettingsType", UEnum::GetValueAsString(SettingsType)));
			}

			TArray<TSoftObjectPtr<class UEnhancedInputMappingContext>> InputMappingContexts = InputConfigDataSet->InputMappingContexts;

			if (InputMappingContexts.Num() == 0)
			{
				UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: No input mapping contexts found for settings type {SettingsType}.",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
					("SettingsType", UEnum::GetValueAsString(SettingsType)));
				return;
			}

			for (const auto& MappingContext : InputMappingContexts)
			{
				if (!MappingContext.IsValid())
				{
					MappingContext.LoadSynchronous();
					if (!MappingContext.IsValid())
					{
						UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Input mapping context is invalid for settings type {SettingsType}.",
							("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
							("SettingsType", UEnum::GetValueAsString(SettingsType)));
						continue;
					}
				}

				UE_LOGFMT(Log_UnifiedController_Register, Verbose, "{Time}: {Line} {Class}: Processing input mapping context {MappingContextName} for settings type {SettingsType} and Configuration {Configuration}.",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
					("MappingContextName", *MappingContext.Get()->GetName()), ("SettingsType", UEnum::GetValueAsString(SettingsType)),
					("Configuration", UEnum::GetValueAsString(Configuration)));

				// When Configuration != IC_NONE, add contexts that match the requested configuration or have IC_NONE
				// (so all setting-type contexts coexist; e.g. IST_VEHICLE and IST_CONTROLS_ENGINE each add their own).
				const EInputConfiguration ContextConfig = MappingContext.Get()->InputConfiguration;
				const bool bMatchesRequested = (ContextConfig == Configuration);
				const bool bIsBaseContext = (ContextConfig == EInputConfiguration::IC_NONE);
				const bool bShouldAdd = (Configuration == EInputConfiguration::IC_NONE) ? true : (bMatchesRequested || bIsBaseContext);

				if (!bShouldAdd)
				{
					UE_LOGFMT(Log_UnifiedController_Register, Verbose, "{Time}: {Line} {Class}: Input mapping context {MappingContextName} has configuration {MappingConfiguration}, expected {ExpectedConfiguration}. Skipping.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("MappingContextName", *MappingContext.Get()->GetName()), ("MappingConfiguration", UEnum::GetValueAsString(ContextConfig)),
						("ExpectedConfiguration", UEnum::GetValueAsString(Configuration)));
					continue;
				}
				if (Configuration != EInputConfiguration::IC_NONE && bIsBaseContext)
				{
					UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Adding base mapping context {MappingContextName} (IC_NONE) for settings type {SettingsType}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("MappingContextName", *MappingContext.Get()->GetName()), ("SettingsType", UEnum::GetValueAsString(SettingsType)));
				}

				if (MappingContext.Get())
				{
					const FString ContextName = MappingContext.Get()->GetName();


					UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Adding input mapping context {MappingContextName} for settings type {SettingsType}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("MappingContextName", *ContextName), ("SettingsType", UEnum::GetValueAsString(SettingsType)));

					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
					{
						constexpr int32 Priority = 0;
						Subsystem->AddMappingContext(MappingContext.LoadSynchronous(), Priority);
						ActiveMappingContextNames.AddUnique(ContextName);
						UE_LOGFMT(Log_UnifiedController_Register, Log, "{Time}: {Line} {Class}: MappingContext {MappingContext} added with priority {Priority} for control type {ControlType}.",
							("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
							("MappingContext", *ContextName), ("Priority", Priority), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
					}
					else
					{
						UE_LOGFMT(Log_UnifiedController_Register, Error, "{Time}: {Line} {Class}: EnhancedInputLocalPlayerSubsystem not found! Cannot bind mapping context for control type {ControlType}.",
							("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
					}
				}
				else
				{
					UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Mapping context is null for settings type {SettingsType}.",
						("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
						("SettingsType", UEnum::GetValueAsString(SettingsType)));
				}
			}

			UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Successfully added input mapping contexts for settings type {SettingsType}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
				("SettingsType", UEnum::GetValueAsString(SettingsType)));
		}
		else
		{
			UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Input config data set is null for settings type {SettingsType}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
				("SettingsType", UEnum::GetValueAsString(SettingsType)));
		}
	}
	else
	{
		UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Input config data map does not contain settings type {SettingsType}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("SettingsType", UEnum::GetValueAsString(SettingsType)));
	}

	UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Finished adding input mapping contexts for settings type {SettingsType}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
		("SettingsType", UEnum::GetValueAsString(SettingsType)));
}

#include "Interfaces/Control/CameraControlInterface.h"
#include "Interfaces/Control/InteractControlInterface.h"
#include "Interfaces/Control/HumanoidControlInterface.h"
#include "Interfaces/Control/TurretControlInterface.h"
#include "Interfaces/Control/VehicleControlInterface.h"
#include "Interfaces/Control/HelicopterControlInterface.h"
#include "Interfaces/Control/EngineControlInterface.h"
#include "Interfaces/Control/EngineInputControlInterface.h"

EInputSettingType AUnifiedPlayerController::GetInputSettingForClass(UClass* Class) const
{
	if (Class->IsChildOf(UHumanoidControlInterface::StaticClass()))
		return EInputSettingType::IST_HUMANOID;
	if (Class->IsChildOf(UVehicleControlInterface::StaticClass()))
		return EInputSettingType::IST_VEHICLE;
	if (Class->IsChildOf(UInteractControlInterface::StaticClass()))
		return EInputSettingType::IST_INTERACT;
	if (Class->IsChildOf(UEngineControlInterface::StaticClass()))
		return EInputSettingType::IST_CONTROL_ENGINE;
	if (Class->IsChildOf(UEngineInputControlInterface::StaticClass()))
		return EInputSettingType::IST_CONTROLS_ENGINE;
	if (Class->IsChildOf(UCameraControlInterface::StaticClass()))
		return EInputSettingType::IST_CONTROL_CAMERA;
	if (Class->IsChildOf(UTurretControlInterface::StaticClass()))
		return EInputSettingType::IST_TURRET;
	if (Class->IsChildOf(UHelicopterControlInterface::StaticClass()))
		return EInputSettingType::IST_HELICOPTER;

	return EInputSettingType();
}

TArray<UClass*> AUnifiedPlayerController::GetComponentInterfaceClass(UActorComponent* Component) const
{
	TArray<UClass*> InterfaceClasses = TArray<UClass*>();

	if (!Component)
	{
		UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Component is null, cannot get interface class.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return InterfaceClasses;
	}

	UClass* ComponentClass = Component->GetClass();
	if (!ComponentClass)
	{
		UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Component class is null, cannot get interface class.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return InterfaceClasses;
	}

	UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Checking component {ComponentName} for registered interfaces.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ComponentName", *Component->GetName()));

	for (const auto& InterfaceClass : ComponentInterfaceClasses)
	{
		if (ComponentClass->ImplementsInterface(InterfaceClass))
		{
			UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Component {ComponentName} implements interface {InterfaceClass}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
				("ComponentName", *Component->GetName()), ("InterfaceClass", *InterfaceClass->GetName()));
			InterfaceClasses.Add(InterfaceClass);
		}
	}
	
	if (InterfaceClasses.Num() == 0)
	{
		UE_LOGFMT(Log_UnifiedController_Register, Warning, "{Time}: {Line} {Class}: Component {ComponentName} does not implement any registered interface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ComponentName", *Component->GetName()));
		return InterfaceClasses;
	}

	UE_LOGFMT(Log_UnifiedController_Register, VeryVerbose, "{Time}: {Line} {Class}: Component {ComponentName} implements the following interfaces: {Interfaces}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
		("ComponentName", *Component->GetName()), ("Interfaces", FString::JoinBy(InterfaceClasses, TEXT(", "), [](const UClass* Class) { return Class ? Class->GetName() : TEXT("nullptr"); })));

	return InterfaceClasses;
}
