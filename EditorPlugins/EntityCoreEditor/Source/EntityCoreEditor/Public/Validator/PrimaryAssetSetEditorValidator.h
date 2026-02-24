// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "PrimaryAssetSetEditorValidator.generated.h"

/**
 * 
 */
UCLASS()
class ENTITYCOREEDITOR_API UPrimaryAssetSetEditorValidator : public UEditorValidatorBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Override this to determine whether or not you can use this validator given this context.
	 * Context can be used to add errors if validation cannot be performed because of some issue
	 */
	virtual bool CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject, FDataValidationContext& InContext) const override
	{
		return false;
	}

	/** Override this to validate in C++ with access to FDataValidationContext */
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& Context) override
	{
		return EDataValidationResult::NotValidated;
	}

	/** Override this to return additional assets that need validation because of other changes in a changelist. e.g. referencers of a specific type may be broken by edits to a dependency. */
	virtual TArray<FAssetData> GetAssetsToValidateFromChangelist(UDataValidationChangelist* InChangelist, FDataValidationContext& InContext) const override
	{
		return {};
	}
};
