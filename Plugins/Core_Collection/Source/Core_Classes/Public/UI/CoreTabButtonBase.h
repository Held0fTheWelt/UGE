// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "UI/CoreTabListWidgetBase.h"
#include "UI/CoreButtonBase.h"

#include "CoreTabButtonBase.generated.h"

class UCommonLazyImage;
class UObject;
struct FFrame;
struct FSlateBrush;

/**
 * UCoreTabButtonBase
 *
 * Base tab button with icon support.
 * Adapted from ULyraTabButtonBase.
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class CORE_CLASSES_API UCoreTabButtonBase : public UCoreButtonBase, public ICoreTabButtonInterface
{
	GENERATED_BODY()

public:

	void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	void SetIconBrush(const FSlateBrush& Brush);

protected:

	UFUNCTION()
	virtual void SetTabLabelInfo_Implementation(const FCoreTabDescriptor& TabLabelInfo) override;

private:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> LazyImage_Icon;
};
