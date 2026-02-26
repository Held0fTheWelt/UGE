// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "PickupDefinition.generated.h"

class UObject;
class USoundBase;
class UStaticMesh;

/**
 * Data asset used to configure a pickup.
 * InventoryItemDefinition: class of the inventory item (e.g. from a future Inventory plugin).
 */
UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "WarCollection Pickup Data", ShortTooltip = "Data asset used to configure a pickup."))
class GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API UPickupDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup|Equipment")
	TSubclassOf<UObject> InventoryItemDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup|Mesh")
	TObjectPtr<UStaticMesh> DisplayMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup")
	int32 SpawnCoolDownSeconds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup")
	TObjectPtr<USoundBase> PickedUpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup")
	TObjectPtr<USoundBase> RespawnedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup")
	TObjectPtr<UNiagaraSystem> PickedUpEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup")
	TObjectPtr<UNiagaraSystem> RespawnedEffect;
};

/**
 * Data asset used to configure a weapon pickup.
 */
UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "WarCollection Weapon Pickup Data", ShortTooltip = "Data asset used to configure a weapon pickup."))
class GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API UWeaponPickupDefinition : public UPickupDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup|Mesh")
	FVector WeaponMeshOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarCollection|Pickup|Mesh")
	FVector WeaponMeshScale = FVector(1.0f, 1.0f, 1.0f);
};
