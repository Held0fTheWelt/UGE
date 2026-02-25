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
 * File:        [Input_Settings.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Enums/InputSettingType.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Input_Settings.generated.h"

/**
 * FInputData
 *
 * Structure representing a single input configuration entry.
 * Can be used both as a DataTable row definition and for direct storage in settings arrays.
 * Encapsulates the logical type of input and an associated identifier.
 */
USTRUCT(BlueprintType)
struct FInputData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FInputData()
	{
		// Sets the default input setting type to 'None' for safety.
		InputSettingType = EInputSettingType::IST_NONE;
	}

	/**
	 * The type of input setting (e.g., button, axis, mouse, gamepad, etc.).
	 * This enumerator should capture all supported input modalities.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (RowType = "GUID"))
	EInputSettingType InputSettingType;

	/**
	 * Unique identifier or object path for this input configuration.
	 * Typically used for mapping to an action, axis, or specific configuration asset.
	 * Should be globally unique within the input system to prevent ambiguity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (RowType = "ObjectPath"))
	FString InputDataIdentifier;
};

/**
 * UInput_Settings
 *
 * Centralized, configurable UObject for managing input configuration data.
 * Designed to be project-specific, providing a persistent source of truth for input mappings.
 * Integrates with Unreal's config system, allowing for game-wide customization via configuration files.
 */
UCLASS(Config = Game, defaultconfig)
class INPUTSYSTEM_API UInput_Settings : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Constructor.
	 * Initializes the InputSettings array to be empty by default.
	 */
	UInput_Settings(const FObjectInitializer& obj)
	{
		InputSettings = TArray<FInputData>();
	};

	/**
	 * Array of input data entries describing all configured input settings.
	 * Can be modified in the editor and is serialized to the game's configuration files (e.g., DefaultGame.ini).
	 * Each entry encapsulates both the type and identifier for an input binding.
	 */
	UPROPERTY(Config, EditAnywhere, Category = Input)
	TArray<FInputData> InputSettings;
};
