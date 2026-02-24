#pragma once

#include "CoreMinimal.h"
#include "Structs/AbilityGrant.h"
#include "Structs/AbilitySetGrant.h"
#include "GameFeatureAbilitiesEntry.generated.h"

USTRUCT()
struct FGameFeatureAbilitiesEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSoftClassPtr<class AActor> ActorClass;

	// List of abilities to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FAbilityGrant> GrantedAbilities;

	// List of attribute sets to grant to actors of the specified class 
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<FAttributeSetGrant> GrantedAttributes;

	// List of ability sets to grant to actors of the specified class
	//UPROPERTY(EditAnywhere, Category="Attributes", meta=(AssetBundles="Client,Server"))
	//TArray<TSoftObjectPtr<const UAbilitySet>> GrantedAbilitySets;
};