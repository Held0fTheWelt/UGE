// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Templates/SubclassOf.h"
#include "EquipmentDefinition.generated.h"

class AActor;
class UEquipmentInstance;
class UObject;

USTRUCT()
struct GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API FEquipmentActorToSpawn
{
	GENERATED_BODY()

	FEquipmentActorToSpawn() {}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

/**
 * Definition of a piece of equipment that can be applied to a pawn.
 * AbilitySetsToGrant: objects implementing IAbilitySetInterface (e.g. from a future Ability plugin).
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API UEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UEquipmentInstance> InstanceType;

	/** Objects implementing IAbilitySetInterface to grant when equipped. */
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UObject>> AbilitySetsToGrant;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FEquipmentActorToSpawn> ActorsToSpawn;
};
