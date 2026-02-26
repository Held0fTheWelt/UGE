// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_FromEquipment.generated.h"

class UEquipmentInstance;
class UObject;

/**
 * An ability granted by and associated with an equipment instance.
 * Base: Engine UGameplayAbility (no Core/Lyra dependency).
 */
UCLASS()
class GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API UGameplayAbility_FromEquipment : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="WarCollection|Ability")
	UEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "WarCollection|Ability")
	UObject* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
