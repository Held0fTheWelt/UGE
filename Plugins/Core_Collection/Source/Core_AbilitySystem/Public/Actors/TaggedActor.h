// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"

#include "TaggedActor.generated.h"

// An actor that implements the gameplay tag asset interface
UCLASS()
class CORE_ABILITYSYSTEM_API ATaggedActor : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	ATaggedActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	//~End of IGameplayTagAssetInterface

	//~UObject interface
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif
	//~End of UObject interface

protected:
	// Gameplay-related tags associated with this actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FGameplayTagContainer StaticGameplayTags;

};
