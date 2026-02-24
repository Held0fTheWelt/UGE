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
 * File:        [InputLoadingPlayerController.h]
 * Created:     [2025-06-12]
 * Description: Abstract base controller for loading and configuring enhanced input mappings via asset references.
 * -------------------------------------------------------------------------------
 */

#pragma once

#include "CoreMinimal.h"
#include "Controllers/ModularPlayerController.h"
#include "Enums/InputSettingType.h"
#include "Structs/AssetLoaderEvents.h"
#include "InputLoadingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CONTROLLERS_API AInputLoadingPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	/** Standard constructor */
	AInputLoadingPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/** Current input control type (e.g., keyboard, gamepad, etc.), or IST_NONE if unset */
	EInputSettingType CurrentControlType = EInputSettingType::IST_NONE;

	UPROPERTY(Transient)
	TMap<EInputSettingType, class UInputConfigDataAssetSet*> InputConfigDataMap;

	UPROPERTY(Transient)
	int8 LoadCounter = 0;
	UPROPERTY(Transient)
	int8 AssetsToLoadCount = 0;

protected:
	virtual void BeginPlay() override;
	/** Called when a pawn is possessed by this controller */
	virtual void OnPossess(APawn* PawnToPossess) override;
	virtual void OnUnPossess() override;

	/** Update input settings based on the new mapping*/
	virtual void SetupEnhancedInputMappingContext(class UEnhancedInputMappingContext* MappingContext);

	/** Called when an enhanced input action asset has finished loading */
	virtual void SetupEnhancedInputAction(class UEnhancedInputAction* InputAction);

	/**
	* Called when input settings are about to be updated (intended to be overridden in subclasses).
	* Return true to allow further loading.
	*/
	virtual bool UpdateSettingToLoad(APawn* PawnToPossess) { return false; };

	/** Check if all input assets are loaded */
	bool AllInputAssetsLoaded() const;

private:
	UPROPERTY(Transient)
	TArray<EInputSettingType> PreviousInputSettingTypes;
	UPROPERTY(Transient)
	TArray<EInputSettingType> CurrentInputSettingTypes;

private:
	FAssetLoaderEvents AssetLoaderEvents;
#pragma region "AssetManager"

	// Outer
protected:
	/** Wird aufgerufen, wenn das Asset-Management-System bereit ist, die Assets zu laden */
	virtual void LoadPrimaryAssets();

	/** Wird gefeuert, wenn Endplay den Pawn abmeldet*/
	void UnbindAssetLoaderEventListener();

private:
	/** Wird für jedes einzeln geladene DataAsset gefeuert */
	UFUNCTION()
	void OnPrimaryAssetLoaded(class UObject* InOwner, class UObject* LoadedObject);

	// Inner
	void LoadMappingsAndActions(UInputConfigDataAssetSet* DataSet);
	
	UFUNCTION()
	void OnInputAssetsLoaded(uint8 TypeAsInt);

#pragma endregion "AssetManager"
};
