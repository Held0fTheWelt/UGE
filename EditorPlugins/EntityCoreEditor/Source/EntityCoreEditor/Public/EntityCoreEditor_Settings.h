// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EntityCoreEditor_Settings.generated.h"

/**
 * 
 */
UCLASS(config = CoreSettings)
class ENTITYCOREEDITOR_API UEntityCoreEditor_Settings : public UObject
{
	GENERATED_BODY()
	
public:
	UEntityCoreEditor_Settings(const FObjectInitializer& obj) : Super(obj)
	{
		DefaultAssetEditorWidget = nullptr;
		DefaultEntityEditorWidget = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Asset Editor", meta = (AllowAbstract = "false", DefaultToSelf = "true"))
	TSoftObjectPtr<class UEditorUtilityWidgetBlueprint> DefaultAssetEditorWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Entity Editor", meta = (AllowAbstract = "false", DefaultToSelf = "true"))
	TSoftObjectPtr<class UEditorUtilityWidgetBlueprint> DefaultEntityEditorWidget;

};
