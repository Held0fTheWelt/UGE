// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorToolHeaderWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDITORMANAGER_API UEditorToolHeaderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_EditorToolHeader;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_EditorToolHeaderVersion;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_EditorToolHeader;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor Tool Header")
	FText EditorToolHeaderText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor Tool Header")
	FText EditorToolVersionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor Tool Header")
	TSoftObjectPtr<class UTexture2D> EditorToolHeaderImage;


protected:
	virtual void NativePreConstruct() override;
};
