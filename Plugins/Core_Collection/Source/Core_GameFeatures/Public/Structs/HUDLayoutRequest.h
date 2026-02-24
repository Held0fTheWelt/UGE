#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HUDLayoutRequest.generated.h"


USTRUCT()
struct FHUDLayoutRequest
{
	GENERATED_BODY()

	// The layout widget to spawn
	//UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	//TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	// The layer to insert the widget in
	UPROPERTY(EditAnywhere, Category = UI, meta = (Categories = "UI.Layer"))
	FGameplayTag LayerID;
};