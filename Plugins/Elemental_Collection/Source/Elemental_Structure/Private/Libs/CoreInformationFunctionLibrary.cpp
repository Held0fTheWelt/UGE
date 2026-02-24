// Fill out your copyright notice in the Description page of Project Settings.


#include "Libs/CoreInformationFunctionLibrary.h"

FInformationSet UCoreInformationFunctionLibrary::GetInformationSet(const FString& Category, const FString& Name, const FString& Description)
{
    FInformationSet InformationSet;
	InformationSet.InformationCategory = Category;
	InformationSet.InformationName = Name;
	InformationSet.InformationDescription = Description;
    return InformationSet;
}

void UCoreInformationFunctionLibrary::AddInformationValue(FInformationSet& InformationSet, const FString& VisibleName, const FString& Value, EValueType ValueType)
{
	FInformationSetInformation NewValue;
	NewValue.VisibleName = VisibleName;
	NewValue.Value = Value;
	NewValue.ValueType = ValueType;
	InformationSet.InformationValues.Add(NewValue);
}
