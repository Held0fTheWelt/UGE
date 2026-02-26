// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySubsystem.h"
#include "InventoryManagerComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventorySubsystem)

TScriptInterface<IInventoryManagerInterface> UInventorySubsystem::GetInventoryForTag(UObject* WorldContextObject, AActor* Owner, FGameplayTag Tag)
{
	TScriptInterface<IInventoryManagerInterface> Result;
	if (!Owner) { return Result; }

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World || Owner->GetWorld() != World) { return Result; }

	for (UActorComponent* Comp : Owner->GetComponents())
	{
		IInventoryManagerInterface* AsInterface = Cast<IInventoryManagerInterface>(Comp);
		if (!AsInterface) { continue; }

		UInventoryManagerComponent* AsInventory = Cast<UInventoryManagerComponent>(Comp);
		if (Tag.IsValid())
		{
			if (AsInventory && AsInventory->GetInventoryTag() == Tag)
			{
				Result.SetInterface(AsInterface);
				Result.SetObject(Comp);
				return Result;
			}
		}
		else
		{
			// No tag: return first inventory (backward compatible)
			Result.SetInterface(AsInterface);
			Result.SetObject(Comp);
			return Result;
		}
	}
	return Result;
}
