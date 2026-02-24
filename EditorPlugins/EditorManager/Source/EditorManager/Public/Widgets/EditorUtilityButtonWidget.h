// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorUtilityButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, class UUserWidget*, Widget);

/**
 * 
 */
UCLASS()
class EDITORMANAGER_API UEditorUtilityButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetText(const FText& InText);
	void SetSelected(bool bSelected);

	FOnButtonClicked OnButtonClickedDelegate;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Style")
	FLinearColor ButtonSelectedColor = FLinearColor(0.384266f, 0.384266f, 0.384266f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Style")
	FLinearColor ButtonUnselectedColor = FLinearColor(0.495466f, 0.495466f, 0.495466f, 0.0f);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnButtonClicked();
};
