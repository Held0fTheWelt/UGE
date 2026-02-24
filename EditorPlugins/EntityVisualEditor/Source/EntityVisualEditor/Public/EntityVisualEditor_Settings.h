#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "EntityVisualEditor_Settings.generated.h"
/**
 *
 */
UCLASS(Config = Game, defaultconfig)
class ENTITYVISUALEDITOR_API UEntityVisualEditor_Settings : public UObject
{
	GENERATED_BODY()

public:
	UEntityVisualEditor_Settings(const FObjectInitializer& obj);

	UPROPERTY(Config, EditAnywhere, Category = "Entity Visual Live Content Editor")
	int32 FontSize = 12;
};
