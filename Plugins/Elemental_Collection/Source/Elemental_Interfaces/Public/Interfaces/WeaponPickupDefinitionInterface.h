// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponPickupDefinitionInterface.generated.h"

class UObject;
class UStaticMesh;
class USoundBase;
class UNiagaraSystem;

/**
 * Interface for pickup definitions used by weapon spawners.
 * Implemented e.g. by UWeaponPickupDefinition (GameFeature_EquipmentSystem).
 * Return types use UObject* to avoid module dependencies; cast to UStaticMesh, USoundBase, UNiagaraSystem as needed.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UWeaponPickupDefinitionInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IWeaponPickupDefinitionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	TSubclassOf<UObject> GetInventoryItemDefinition() const;
	virtual TSubclassOf<UObject> GetInventoryItemDefinition_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	UObject* GetDisplayMesh() const;
	virtual UObject* GetDisplayMesh_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	int32 GetSpawnCoolDownSeconds() const;
	virtual int32 GetSpawnCoolDownSeconds_Implementation() const { return 0; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	UObject* GetPickedUpSound() const;
	virtual UObject* GetPickedUpSound_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	UObject* GetRespawnedSound() const;
	virtual UObject* GetRespawnedSound_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	UObject* GetPickedUpEffect() const;
	virtual UObject* GetPickedUpEffect_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	UObject* GetRespawnedEffect() const;
	virtual UObject* GetRespawnedEffect_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	FVector GetWeaponMeshOffset() const;
	virtual FVector GetWeaponMeshOffset_Implementation() const { return FVector::ZeroVector; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickup")
	FVector GetWeaponMeshScale() const;
	virtual FVector GetWeaponMeshScale_Implementation() const { return FVector(1.0f, 1.0f, 1.0f); }
};
