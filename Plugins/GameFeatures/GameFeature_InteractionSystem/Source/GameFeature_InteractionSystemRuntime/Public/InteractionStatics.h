// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionStatics.generated.h"

class AActor;
class UObject;
struct FFrame;
struct FInteractionOption;
struct FInteractionQuery;
struct FInteractionOptionBuilder;

/** Static helper functions for interaction system. */
UCLASS()
class GAMEFEATURE_INTERACTIONSYSTEMRUNTIME_API UInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	static AActor* GetActorFromInteractableTarget(TScriptInterface<IInteractableTarget> InteractableTarget);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	static void GetInteractableTargetsFromActor(AActor* Actor, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets);

	/** C++ only (TArray<FOverlapResult> not Blueprint-compatible). */
	static void AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	static void AppendInteractableTargetsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets);
};
