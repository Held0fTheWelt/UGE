#include "PropertyViewer/PrimaryAssetSetArrayCustomization.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "Misc/PackageName.h"
#include "Structs/PrimaryAssetSetArray.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"

#define LOCTEXT_NAMESPACE "FPrimaryAssetSetArrayCustomization"

TSharedRef<IPropertyTypeCustomization> FPrimaryAssetSetArrayCustomization::MakeInstance()
{
	return MakeShareable(new FPrimaryAssetSetArrayCustomization);
}

void FPrimaryAssetSetArrayCustomization::UpdateAssetNames(TSharedPtr<IPropertyHandle> InArrayHandle)
{
	AssetNames.Empty();
	if (!InArrayHandle.IsValid()) return;

	uint32 NumElements = 0;
	TSharedPtr<IPropertyHandleArray> ArrayProperty = InArrayHandle->AsArray();
	if (!ArrayProperty.IsValid()) return;

	ArrayProperty->GetNumElements(NumElements);

	for (uint32 i = 0; i < NumElements; ++i)
	{
		TSharedPtr<IPropertyHandle> ElementHandle = ArrayProperty->GetElement(i);
		FString PathString;
		if (ElementHandle->GetValueAsFormattedString(PathString) == FPropertyAccess::Success && !PathString.IsEmpty())
		{
			FString AssetName = FPackageName::GetShortName(PathString);
			TArray<FString> AssetParts;
			AssetName.ParseIntoArray(AssetParts, TEXT("_"), true);
			AssetNames.Add(MakeShared<FString>(AssetParts.Num() > 0 ? AssetParts.Last() : AssetName));
		}
		else
		{
			AssetNames.Add(MakeShared<FString>(TEXT("<None>")));
		}
	}
}

void FPrimaryAssetSetArrayCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> StructPropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ArrayHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPrimaryAssetSetArray, CurrentPrimaryDataAssets));
	IndexHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPrimaryAssetSetArray, SetIndex));
	PropUtils = CustomizationUtils.GetPropertyUtilities();

	UpdateAssetNames(ArrayHandle);

	if (ArrayHandle.IsValid() && PropUtils.IsValid())
	{
		ArrayHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPrimaryAssetSetArrayCustomization::HandleArrayChanged));
	}

	int32 CurrentIndex = 0;
	if (IndexHandle.IsValid())
		IndexHandle->GetValue(CurrentIndex);

	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MaxDesiredWidth(400.f)
		[
			SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&AssetNames)
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
					{
						return SNew(STextBlock).Text(FText::FromString(*InItem));
					})
				.OnSelectionChanged_Lambda([this, StructPropertyHandle](TSharedPtr<FString> NewSelection, ESelectInfo::Type)
					{
						int32 NewIndex = AssetNames.IndexOfByPredicate([&](const TSharedPtr<FString>& Ptr) { return Ptr == NewSelection; });
						if (NewIndex >= 0 && IndexHandle.IsValid())
						{
							IndexHandle->SetValue(NewIndex);

							TArray<void*> RawData;
							StructPropertyHandle->AccessRawData(RawData);
							if (RawData.Num() > 0)
							{
								FPrimaryAssetSetArray* AssetSetArray = static_cast<FPrimaryAssetSetArray*>(RawData[0]);
								if (AssetSetArray)
								{
									AssetSetArray->SetIndex = NewIndex; // ← Setze den Index in der Struct-Instanz
									AssetSetArray->OnPrimaryAssetSetSelected.Broadcast(); // ← Hier das Event senden!
									UE_LOG(LogTemp, Verbose, TEXT("Selected asset: %s at index %d"), *(*NewSelection), NewIndex);
								}
								else
								{
									UE_LOG(LogTemp, Warning, TEXT("AssetSetArray is null in OnSelectionChanged."));
								}
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("RawData is empty or invalid in OnSelectionChanged."));
							}

							if (PropUtils.IsValid())
								PropUtils->ForceRefresh();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Selected index is invalid or IndexHandle is not valid."));
						}
					})
				.IsEnabled(AssetNames.Num() > 0)
				.InitiallySelectedItem(AssetNames.IsValidIndex(CurrentIndex) ? AssetNames[CurrentIndex] : nullptr)
				[
					SNew(STextBlock)
						.Text_Lambda([this]()
							{
								int32 CurrentIndex = 0;
								if (IndexHandle.IsValid())
									IndexHandle->GetValue(CurrentIndex);
								if (AssetNames.IsValidIndex(CurrentIndex))
									return FText::FromString(*AssetNames[CurrentIndex]);
								return LOCTEXT("NoneSelected", "<None>");
							})
				]
		];

	// Zugriff auf Struct-Instanz und Delegate binden
	TArray<void*> RawData;
	StructPropertyHandle->AccessRawData(RawData);
	if (RawData.Num() > 0)
	{
		FPrimaryAssetSetArray* AssetSetArray = static_cast<FPrimaryAssetSetArray*>(RawData[0]);
		if (AssetSetArray)
		{
			AssetSetArray->OnPrimaryAssetSetsChanged.AddSP(this, &FPrimaryAssetSetArrayCustomization::HandleAssetsChanged);
		}
	}
}

void FPrimaryAssetSetArrayCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle>,
	IDetailChildrenBuilder&,
	IPropertyTypeCustomizationUtils&)
{
	// Keine Kinder nötig, Button entfällt
}

void FPrimaryAssetSetArrayCustomization::HandleArrayChanged()
{
	if (ArrayHandle.IsValid())
	{
		UpdateAssetNames(ArrayHandle);
		if (PropUtils.IsValid())
			PropUtils->ForceRefresh();
	}
}

void FPrimaryAssetSetArrayCustomization::HandleAssetsChanged()
{
	if(!PropUtils)
	{
		UE_LOG(LogTemp, Warning, TEXT("Property Utilities is not valid in HandleAssetsChanged."));
		return;
	}

	if (PropUtils.IsValid())
	{
		PropUtils->ForceRefresh();
	}
}

#undef LOCTEXT_NAMESPACE
