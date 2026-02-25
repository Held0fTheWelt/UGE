// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Primary/InputConfigPrimaryDataAssetSet.h"
#include "Enums/InputSettingType.h"
#include "Interfaces/InputAssetInterface.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "InputConfigDataAssetSet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_InputConfigDataAsset, Log, All);

/**
 * 
 */
UCLASS()
class INPUTSYSTEM_API UInputConfigDataAssetSet : public UInputConfigPrimaryDataAssetSet, public IInputAssetInterface
{
	GENERATED_BODY()

public:
	/**
	 * Specifies the input setting type (e.g., Humanoid, Vehicle, Weapon, etc.) this configuration asset targets.
	 *
	 * Enables context-aware input configuration and supports modular input profile management.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	EInputSettingType InputSettingType = EInputSettingType::IST_NONE;

	/**
	 * Unique identifier string for this input setting configuration.
	 *
	 * Used to distinguish and reference individual input profiles in code or data tables.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FString InputSettingIdentifier = "";

	/**
	 * List of input setting types (e.g., Humanoid, Vehicle, Turret) this mapping context is intended for.
	 * Used to filter applicable input contexts for specific pawn or actor types at runtime.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Type)
	TArray<EInputSettingType> InputTypes = TArray<EInputSettingType>();

	/**
	 * Standard input mapping context used for general character input.
	 * Should reference a UEnhancedInputMappingContext asset.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mapping)
	TArray<TSoftObjectPtr<class UEnhancedInputMappingContext>> InputMappingContexts;

	/**
	 * Array of input actions associated with this configuration.
	 * Each action should reference a UEnhancedInputAction asset.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TArray<TSoftObjectPtr<class UEnhancedInputAction>> InputMappingActions;

	/**
	 * Returns an array containing the associated input mapping context.
	 * Logs an error if the context reference is missing or null.
	 *
	 * @return Array of FSoftObjectPath with the mapping context for this profile.
	 */
	virtual TArray<FSoftObjectPath> GetInputMappingContexts_Implementation() const override
	{
		TArray<FSoftObjectPath> Contexts;
		for (const auto& Context : InputMappingContexts)
		{
			if (!Context.IsNull())
			{
				Contexts.Add(Context.ToSoftObjectPath());
			}
			else
			{
				UE_LOGFMT(Log_InputConfigDataAsset, Error, "{Time}: {Line} {Class}: One of the InputMappingContexts is null. Please set it in the editor!",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
			}
		}
		return Contexts;
	}

	/**
	 * Returns an array of all required input actions for the humanoid configuration.
	 * Logs an error for each missing or unset action.
	 *
	 * @return Array of FSoftObjectPath for the configured input actions.
	 */
	virtual TArray<FSoftObjectPath> GetInputActions_Implementation() const override
	{
		TArray<FSoftObjectPath> Actions;

		for (auto Action : InputMappingActions)
		{
			if (!Action.IsNull())
			{
				Actions.Add(Action.ToSoftObjectPath());
			}
			else
			{
				UE_LOGFMT(Log_InputConfigDataAsset, Error, "{Time}: {Line} {Class}: One of the InputMappingActions is null. Please set it in the editor!",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
			}
		}

		return Actions;
	}

	/**
	 * Checks whether this data asset is ready for use.
	 *
	 * By default, verifies that InputSettingType is set and InputSettingIdentifier is not empty.
	 * Can be overridden in derived classes to add further validation logic.
	 *
	 * @return true if asset is considered valid and ready for use; false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	virtual bool IsReadyForUse() const
	{
		// Editor-only: Log missing references for user feedback
#if WITH_EDITOR
		for(auto Context : InputMappingContexts)
		{
			if (Context.IsNull())
			{
				UE_LOGFMT(Log_InputConfigDataAsset, Error, "{Time}: {Line} {Class}: One of the InputMappingContexts is null. Please set it in the editor!",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
			}
		}
#endif 

		bool IsReady = true;
		for(auto Action : InputMappingActions)
		{
			if (Action.IsNull())
			{
#if WITH_EDITOR
				UE_LOGFMT(Log_InputConfigDataAsset, Error, "{Time}: {Line} {Class}: One of the InputMappingActions is null. Please set it in the editor!",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
#endif 
				IsReady = false;
			}
		}

		// Core runtime check: ensure InputSettingType is set and InputSettingIdentifier is not empty
		return !(InputSettingType == EInputSettingType::IST_NONE || InputSettingIdentifier.IsEmpty() || InputTypes.IsEmpty()) 
			&& !InputMappingContexts.IsEmpty() &&!InputMappingActions.IsEmpty() && IsReady;
	}

#if WITH_EDITOR
	/**
	 * Editor-only: Validate the data asset and provide an error message if invalid.
	 * Override in derived classes to implement custom validation logic.
	 *
	 * @param OutError [out] Receives error information if the asset is invalid.
	 * @return true if asset is valid, false otherwise.
	 */
	virtual bool IsDataAssetValid(FString& OutError) const
	{
		return false;
	}
#endif

	/**
	 * Implementation of the IInputAssetInterface for retrieving the input setting type.
	 *
	 * @return EInputSettingType for this asset instance.
	 */
	virtual EInputSettingType GetInputSettingType_Implementation() const override { return InputSettingType; }

};
