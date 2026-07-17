#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIExtensionCompatibility.generated.h"

/** Serializable compatibility identity used by Lyra UI extension assets. */
USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
    GENERATED_BODY()

    UPROPERTY()
    FGuid RegistrationId;
};

/** UGE-owned fallback extension point; composed UI can replace it through reviewed redirects. */
UCLASS(BlueprintType, Blueprintable)
class UIEXTENSION_API UUIExtensionPointWidget : public UUserWidget
{
    GENERATED_BODY()
};
