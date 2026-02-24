// Fill out your copyright notice in the Description page of Project Settings.


#include "Libs/EntityCoreEditorFunctionLibrary.h"

FText UEntityCoreEditorFunctionLibrary::GetAssetNameFromInternalNameString(const FString& InternalName)
{
	FString AssetPath = InternalName.RightChop(1); // Remove the leading slash
	TArray<FString> Parts;
	AssetPath.ParseIntoArray(Parts, TEXT("."), true);
	return FText::FromString(Parts[0]);
}
