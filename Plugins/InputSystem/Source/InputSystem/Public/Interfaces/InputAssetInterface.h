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
 * File:        [InputAssetInterface.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/InputSettingType.h"
#include "InputAssetInterface.generated.h"

/**
 * UInputAssetInterface
 *
 * Unreal Engine interface class for input asset objects.
 * Use this interface to standardize access to input settings, input mapping contexts, and input actions across various objects.
 */
UINTERFACE(MinimalAPI)
class UInputAssetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IInputAssetInterface
 *
 * Defines the core contract for accessing input-related metadata from any asset implementing this interface.
 * Classes implementing this interface must provide input setting type, associated mapping contexts, and input actions.
 */
class INPUTSYSTEM_API IInputAssetInterface
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves the type of input setting represented by this asset.
	 *
	 * @return An EInputSettingType enumerator value describing the input type (e.g., Axis, Action, etc.).
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	EInputSettingType GetInputSettingType() const;

	/**
	 * Returns an array of soft object paths to input mapping contexts associated with this asset.
	 *
	 * @return Array of FSoftObjectPath referencing UInputMappingContext assets.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	TArray<FSoftObjectPath> GetInputMappingContexts() const;

	/**
	 * Returns an array of soft object paths to input action assets utilized by this input configuration.
	 *
	 * @return Array of FSoftObjectPath referencing UInputAction assets.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	TArray<FSoftObjectPath> GetInputActions() const;
};
