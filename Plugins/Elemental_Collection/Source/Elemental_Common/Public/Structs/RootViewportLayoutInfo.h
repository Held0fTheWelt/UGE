#pragma once

#include "CoreMinimal.h"
#include "RootViewportLayoutInfo.generated.h"

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<class ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<class UPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(class ULocalPlayer* InLocalPlayer, class UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{
	}

	bool operator==(const class ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};