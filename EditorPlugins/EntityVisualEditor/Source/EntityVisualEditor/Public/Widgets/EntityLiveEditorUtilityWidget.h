// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Structs/InformationSet.h"
#include "EntityLiveEditorUtilityWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_EntityActionsEditorUtilityWidget, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEditorActorSelectionChanged);

/**
 * 
 */
UCLASS()
class ENTITYVISUALEDITOR_API UEntityLiveEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	
	/** Blueprint can bind to this to update UI */
	UPROPERTY(BlueprintAssignable, Category = "Editor|Selection")
	FOnEditorActorSelectionChanged OnEditorActorSelectionChanged;

	/** Call to pull current selection and update your list view */
	UFUNCTION(BlueprintCallable, Category = "Editor|Selection")
	void RefreshSelection();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_EntityName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UDataDrivenGridPanel* DataDrivenGridPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* EntityList;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityScrollBox* ScrollBox_EntityInformationButtons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default)
	TSubclassOf<class UEditorUtilityButtonWidget> ButtonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default)
	TSubclassOf<class UEditorUtilityTabButtonWidget> TabButtonClass;


private:
	UPROPERTY(Transient)
	TArray<class AActor*> SelectedActors;
	UPROPERTY(Transient)
	class AActor* SelectedActor;
	UPROPERTY(Transient)
	int32 CurrentSelectedActorIndex;
	UPROPERTY(Transient)
	int32 CurrentSelectedTabIndex;
	UPROPERTY(Transient)
	class UActorComponent* LastSelectedActorComponent = nullptr;
	
	UPROPERTY(Transient)
	TArray<class UEditorUtilityButtonWidget*> ButtonWidgets;
	UPROPERTY(Transient)
	TArray<class UEditorUtilityTabButtonWidget*> ActorInformationButtons;

private:
	FDelegateHandle SelectionChangedHandle;
	FDelegateHandle SelectObjectHandle;

	void HandleSelectionChanged(UObject* /*NewSelection*/);
	void HandleSelectObject(UObject* /*SelectedObject*/);

	void UpdateCoreEntityList();

	void UpdateMenuButtons();

	void UpdateTabButtons();

	void UpdateContent();

	void UpdateContentTable(TArray<FInformationSet> Information);

	void SetSelectedActor();

	UFUNCTION()
	void RefreshCurrentSelectedActor(class UUserWidget* ButtonWidget);
	UFUNCTION()
	void RefreshCurrentTabSelection(class UUserWidget* TabButtonWidget);
	UFUNCTION()
	void HandleInformationUpdated(const TArray<FInformationSet>& Information);
};
