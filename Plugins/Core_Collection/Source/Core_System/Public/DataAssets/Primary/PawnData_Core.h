// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Primary/InformativePrimaryDataAsset.h"
#include "DataAssets/AbilityTagRelationshipMapping.h"
#include "Interfaces/PawnDataAbilitySetProviderInterface.h"
#include "Interfaces/PawnDataTagRelationshipProviderInterface.h"
#include "Interfaces/PawnDataPawnClassProviderInterface.h"
#include "PawnData_Core.generated.h"

/**
 *	Non-mutable data asset that contains global game data.
 *	Implements IPawnDataAbilitySetProviderInterface, IPawnDataTagRelationshipProviderInterface, and IPawnDataPawnClassProviderInterface.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class CORE_SYSTEM_API UPawnData_Core : public UInformativePrimaryDataAsset, public IPawnDataAbilitySetProviderInterface, public IPawnDataTagRelationshipProviderInterface, public IPawnDataPawnClassProviderInterface
{
	GENERATED_BODY()
	
public:
	UPawnData_Core(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	// Class to instantiate for this pawn (should usually derive from ALyraPawn or ALyraCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system (concrete type e.g. UCoreAbilitySet; stored as UObject so Core_System does not depend on Core_AbilitySystem).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Abilities")
	TArray<TObjectPtr<UObject>> AbilitySets;

	// What mapping of ability tags to use for actions taken by this pawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Abilities")
	TObjectPtr<UAbilityTagRelationshipMapping> TagRelationshipMapping;

	//// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Input")
	//TObjectPtr<ULyraInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Camera")
	//TSubclassOf<ULyraCameraMode> DefaultCameraMode;

	//~IPawnDataAbilitySetProviderInterface
	TArray<UObject*> GetAbilitySetsToGrant() const override;
	void GrantAbilitySetsToAbilitySystem(UAbilitySystemComponent* ASC, UObject* SourceObject) override;

	//~IPawnDataTagRelationshipProviderInterface
	UObject* GetTagRelationshipMapping() const override { return TagRelationshipMapping.Get(); }

	//~IPawnDataPawnClassProviderInterface
	UClass* GetPawnClass() const override { return PawnClass; }
};
