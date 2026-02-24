// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnExtensionComponentInterface.generated.h"

class UAbilitySystemComponent;

UINTERFACE(MinimalAPI, BlueprintType)
class UPawnExtensionComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for the pawn extension component (provided by GameFeature_PawnExtension).
 * Core and other modules use this interface so they do not depend on the concrete GameFeature module.
 */
class ELEMENTAL_INTERFACES_API IPawnExtensionComponentInterface
{
	GENERATED_BODY()

public:
	virtual void CheckDefaultInitialization() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PawnExtension")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PawnExtension")
	void UninitializeAbilitySystem();
	virtual void UninitializeAbilitySystem_Implementation() {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PawnExtension")
	void HandleControllerChanged();
	virtual void HandleControllerChanged_Implementation() {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PawnExtension")
	void HandlePlayerStateReplicated();
	virtual void HandlePlayerStateReplicated_Implementation() {}

	/** PawnData as UObject so Elemental does not depend on Core; implementation may cast to project-specific type. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PawnExtension")
	void SetPawnData(UObject* PawnData);
	virtual void SetPawnData_Implementation(UObject* PawnData) {}

	/** Initialize the ability system with the given ASC and owner (e.g. player state). Called by HeroSystem via interface. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PawnExtension")
	void InitializeAbilitySystem(UAbilitySystemComponent* ASC, UObject* Owner);
	virtual void InitializeAbilitySystem_Implementation(UAbilitySystemComponent* ASC, UObject* Owner) {}
};
