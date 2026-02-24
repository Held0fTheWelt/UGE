// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/SubclassOf.h"

#include "UObject/WeakObjectPtr.h"
#include "CoreLoadingScreenSubsystem.generated.h"

class UObject;
class UUserWidget;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoreLoadingScreenWidgetChangedDelegate, TSubclassOf<UUserWidget>, NewWidgetClass);

/**
 * UCoreLoadingScreenSubsystem
 *
 *	Tracks/stores the current loading screen configuration in a place
 *	that persists across map transitions.
 *	Adapted from LyraLoadingScreenSubsystem.
 */
UCLASS()
class CORE_CLASSES_API UCoreLoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCoreLoadingScreenSubsystem();

	// Sets the loading screen widget class to display inside of the loading screen widget host
	UFUNCTION(BlueprintCallable, Category = "Core|LoadingScreen")
	void SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	// Returns the last set loading screen widget class to display inside of the loading screen widget host
	UFUNCTION(BlueprintPure, Category = "Core|LoadingScreen")
	TSubclassOf<UUserWidget> GetLoadingScreenContentWidget() const;

private:
	UPROPERTY(BlueprintAssignable, meta=(AllowPrivateAccess))
	FCoreLoadingScreenWidgetChangedDelegate OnLoadingScreenWidgetChanged;

	UPROPERTY()
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;
};
