#include "PropertyViewer/FEntityDefinitionCustomization.h"
#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "Structs/EntityDefinition.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
//#include "DataAssets/Primary/EntityIdentifierCollectionPrimaryDataAsset.h"
#include "AssetRegistry/AssetRegistryModule.h"

TSharedRef<IPropertyTypeCustomization> FEntityDefinitionCustomization::MakeInstance()
{
	return MakeShareable(new FEntityDefinitionCustomization);
}

void FEntityDefinitionCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> StructPropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	SelectedEntityHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEntityDefinition, SelectedEntity));

	RefreshEntityNames();

	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(200.f)
		[
			SNew(SComboBox<TSharedPtr<FName>>)
				.OptionsSource(&AvailableNames)
				.OnGenerateWidget(this, &FEntityDefinitionCustomization::GenerateComboItem)
				.OnSelectionChanged(this, &FEntityDefinitionCustomization::OnSelectionChanged)
				.Content()
				[
					SNew(STextBlock).Text(this, &FEntityDefinitionCustomization::GetCurrentSelection)
				]
		];
}

void FEntityDefinitionCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle> StructPropertyHandle,
	IDetailChildrenBuilder& StructBuilder,
	IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	// Keine Kinder anzeigen – alles passiert im Header
}

#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "DataAssets/Primary/ActorDefinitionPrimaryDataAsset.h"

void FEntityDefinitionCustomization::RefreshEntityNames()
{
	AvailableNames.Empty();

	FPrimaryAssetId AssetId(UActorDefinitionPrimaryDataAsset::GetAssetType(), TEXT("DA_ActorDefinition"));
	UAssetManager& AssetManager = UAssetManager::Get();
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);

	UObject* LoadedObject = AssetPath.TryLoad();
	
	auto* Def = Cast<UActorDefinitionPrimaryDataAsset>(LoadedObject);

	if (!Def)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not synchronously load DA_ActorDefinition!"));
		return;
	}

	UDataTable* ActorDataTable = Def->ActorDefinition.LoadSynchronous();

	if (!ActorDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorDataTable is null in FEntityDefinitionCustomization::RefreshEntityNames"));
		return;
	}

	TArray<FName> RowNames = ActorDataTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		if (RowName != NAME_None)
		{
			AvailableNames.Add(MakeShared<FName>(RowName));
		}
	}
}

TSharedRef<SWidget> FEntityDefinitionCustomization::GenerateComboItem(TSharedPtr<FName> Item)
{
	return SNew(STextBlock).Text(FText::FromName(*Item));
}

void FEntityDefinitionCustomization::OnSelectionChanged(TSharedPtr<FName> NewValue, ESelectInfo::Type)
{
	if (SelectedEntityHandle.IsValid() && NewValue.IsValid())
	{
		SelectedEntityHandle->SetValue(*NewValue);
	}
}

FText FEntityDefinitionCustomization::GetCurrentSelection() const
{
	FName CurrentValue;
	if (SelectedEntityHandle.IsValid() && SelectedEntityHandle->GetValue(CurrentValue) == FPropertyAccess::Success)
	{
		return FText::FromName(CurrentValue);
	}
	return FText::FromString("None");
}
