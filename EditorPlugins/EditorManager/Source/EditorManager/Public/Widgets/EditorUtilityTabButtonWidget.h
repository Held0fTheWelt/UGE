// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorUtilityTabButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabButtonClicked, class UUserWidget*, Widget);

/**
 * 
 */
UCLASS()
class EDITORMANAGER_API UEditorUtilityTabButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEditorUtilityTabButtonWidget(const FObjectInitializer& ObjectInitializer);

	FOnTabButtonClicked OnTabButtonClickedDelegate;

	void SetSelected(bool bSelected);
	void SetTabName(const FText& Name);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TabName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TabButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab Button")
	class UTexture2D* SelectedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab Button")
	class UTexture2D* UnselectedIcon;

private:
	UFUNCTION()
	void OnTabButtonClicked();
	//UFUNCTION()
	//void OnTabButtonHovered();
	//UFUNCTION()
	//void OnTabButtonUnhovered();
};
