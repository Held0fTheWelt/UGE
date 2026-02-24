// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityLibrary.h"
#include "EditorManagerUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EDITORMANAGER_API UEditorManagerUtilityLibrary : public UEditorUtilityLibrary
{
	GENERATED_BODY()
	
public:
	static TArray<UObject*> BeginTransactionAndGetSelectedAssets(
		const FString& UtilityName,
		const FString& UtilityDescription = "",
		UObject* PrimaryObject = nullptr
	);

	UFUNCTION(BlueprintCallable, Category = "Editor Utility")
	static void CheckoutSaveAndEndTransaction(
		const TArray<UObject*>& ObjectsToCheckout
	);

	static UWorld* GetCurrentEditorWorld()
	{
		if (GEditor)
		{
			FWorldContext* WorldContext = GEditor->GetWorldContextFromPIEInstance(-1);
			if (WorldContext)
			{
				return WorldContext->World();
			}
			// Oder klassisch:
			return GEditor->GetEditorWorldContext().World();
		}
		return nullptr;
	}
	/** Returns currently selected level actors in the editor. */
	UFUNCTION(BlueprintCallable, Category = "Editor|Selection", meta = (DevelopmentOnly))
	static void GetSelectedLevelActors(TArray<AActor*>& OutActors);

	/** Subscribe to selection changed (call once, store the handle if you need to unbind). */
	static FDelegateHandle BindOnSelectionChanged(const FSimpleDelegate& InCallback);

	/** Unbind when done. */
	static void UnbindOnSelectionChanged(FDelegateHandle& Handle);

	//UFUNCTION(BlueprintCallable, Category = "Editor Utility")
	//static void CheckoutSaveAndEndTransaction(
	//	const TArray<UObject*>& ObjectsToCheckout,
	//	const TArray<UObject*>& ObjectsToSave,
	//	const TArray<UObject*>& ObjectsToEndTransaction,
	//	const FString& TransactionDescription
	//);
};
