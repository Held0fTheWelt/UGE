#pragma once

#include "CoreMinimal.h"
#include "Structs/SharedRepMovement.h"
#include "GameplayTagContainer.h"
#include "HUDElementEntry.generated.h"

USTRUCT()
struct FHUDElementEntry
{
	GENERATED_BODY()

	// The widget to spawn
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<class UUserWidget> WidgetClass;

	// The slot ID where we should place this widget
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};