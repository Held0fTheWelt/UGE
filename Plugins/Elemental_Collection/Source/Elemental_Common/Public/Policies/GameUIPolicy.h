// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Enums/LocalMultiplayerInteractionMode.h"
#include "Structs/RootViewportLayoutInfo.h"
#include "GameUIPolicy.generated.h"

UCLASS(Abstract, Blueprintable, Within = GameUIManagerSubsystem)
class ELEMENTAL_COMMON_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	template <typename GameUIPolicyClass = UGameUIPolicy>
	static GameUIPolicyClass* GetGameUIPolicyAs(const class UObject* WorldContextObject)
	{
		return Cast<GameUIPolicyClass>(GetGameUIPolicy(WorldContextObject));
	}

	static UGameUIPolicy* GetGameUIPolicy(const class UObject* WorldContextObject);

public:
	virtual class UWorld* GetWorld() const override;
	class UGameUIManagerSubsystem* GetOwningUIManager() const;
	class UPrimaryGameLayout* GetRootLayout(const class UCommonLocalPlayer* LocalPlayer) const;

	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const { return LocalMultiplayerInteractionMode; }

	void RequestPrimaryControl(class UPrimaryGameLayout* Layout);

protected:
	void AddLayoutToViewport(class UCommonLocalPlayer* LocalPlayer, class UPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(class UCommonLocalPlayer* LocalPlayer, class UPrimaryGameLayout* Layout);

	virtual void OnRootLayoutAddedToViewport(class UCommonLocalPlayer* LocalPlayer, class UPrimaryGameLayout* Layout);
	virtual void OnRootLayoutRemovedFromViewport(class UCommonLocalPlayer* LocalPlayer, class UPrimaryGameLayout* Layout);
	virtual void OnRootLayoutReleased(class UCommonLocalPlayer* LocalPlayer, class UPrimaryGameLayout* Layout);

	void CreateLayoutWidget(class UCommonLocalPlayer* LocalPlayer);
	TSubclassOf<class UPrimaryGameLayout> GetLayoutWidgetClass(class UCommonLocalPlayer* LocalPlayer);

private:
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UPrimaryGameLayout> LayoutClass;

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

private:
	void NotifyPlayerAdded(class UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(class UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(class UCommonLocalPlayer* LocalPlayer);

	friend class UGameUIManagerSubsystem;
};