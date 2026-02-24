// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityTask.h"
#include "EditorUtilityTask_PrepareUndo.generated.h"

/**
 * 
 */
UCLASS()
class EDITORMANAGER_API UEditorUtilityTask_PrepareUndo : public UEditorUtilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Utility")
	FString UtilityName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Utility")
	FString UtilityDescription;

public:
	virtual void BeginExecution() override;
};
