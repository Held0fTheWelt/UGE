// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/SoftObjectPtr.h"

#include "CommonUIExtensions.generated.h"

enum class ECommonInputType : uint8;
template <typename T> class TSubclassOf;

struct FFrame;
struct FGameplayTag;

UCLASS()
class ELEMENTAL_COMMON_API UCommonUIExtensions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UCommonUIExtensions() {}

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions", meta = (WorldContext = "WidgetContextObject"))
	static ECommonInputType GetOwningPlayerInputType(const class UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingTouch(const class UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static class UCommonActivatableWidget* PushContentToLayer_ForPlayer(const class ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<class UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void PushStreamedContentToLayer_ForPlayer(const class ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<class UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void PopContentFromLayer(class UCommonActivatableWidget* ActivatableWidget);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static class ULocalPlayer* GetLocalPlayerFromController(class APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static FName SuspendInputForPlayer(class APlayerController* PlayerController, FName SuspendReason);

	static FName SuspendInputForPlayer(class ULocalPlayer* LocalPlayer, FName SuspendReason);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void ResumeInputForPlayer(class APlayerController* PlayerController, FName SuspendToken);

	static void ResumeInputForPlayer(class ULocalPlayer* LocalPlayer, FName SuspendToken);

private:
	static int32 InputSuspensions;
};

