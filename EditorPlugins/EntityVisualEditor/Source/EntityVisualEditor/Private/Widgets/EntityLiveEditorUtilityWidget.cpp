// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EntityLiveEditorUtilityWidget.h"

#include "Components/GridSlot.h"
#include "Components/TextBlock.h"
#include "Components/DataDrivenGridPanel.h"
#include "Components/VerticalBox.h"

#include "Interfaces/EntityCoreInformationInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Widgets/EditorUtilityButtonWidget.h"
#include "Widgets/EditorUtilityTabButtonWidget.h"

#include "EditorUtilityWidgetComponents.h"

#include "Selection.h"

DEFINE_LOG_CATEGORY(Log_EntityActionsEditorUtilityWidget);

void UEntityLiveEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
    if (GEditor)
    {
        if (USelection* Sel = GEditor->GetSelectedActors())
        {
            // Fires when the selection set changes (add/remove/clear)
            SelectionChangedHandle = Sel->SelectionChangedEvent.AddUObject(
                this, &UEntityLiveEditorUtilityWidget::HandleSelectionChanged);

            // Optional: fires per object select op
            SelectObjectHandle = Sel->SelectObjectEvent.AddUObject(
                this, &UEntityLiveEditorUtilityWidget::HandleSelectObject);
        }
    }
 
    RefreshSelection(); // initial fill

    UpdateCoreEntityList();

    SetSelectedActor();

    UpdateMenuButtons();

    UpdateTabButtons();

    UpdateContent();
}

void UEntityLiveEditorUtilityWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateContent();
}

void UEntityLiveEditorUtilityWidget::NativeDestruct()
{
	Super::NativeDestruct();
    
    if (GEditor)
    {
        if (USelection* Sel = GEditor->GetSelectedActors())
        {
            if (SelectionChangedHandle.IsValid())
            {
                Sel->SelectionChangedEvent.Remove(SelectionChangedHandle);
                SelectionChangedHandle.Reset();
            }
            if (SelectObjectHandle.IsValid())
            {
                Sel->SelectObjectEvent.Remove(SelectObjectHandle);
                SelectObjectHandle.Reset();
            }
        }
    }
}

void UEntityLiveEditorUtilityWidget::HandleSelectionChanged(UObject* /*NewSelection*/)
{
    // Notify BP/UI and pull fresh selection
    OnEditorActorSelectionChanged.Broadcast();
    
    RefreshSelection();

    UpdateCoreEntityList();

    SetSelectedActor();

    UpdateMenuButtons();

    UpdateTabButtons();

    UpdateContent();
}

void UEntityLiveEditorUtilityWidget::UpdateContent()
{
    if (!SelectedActor)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: No Selected Actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
		return;
    }

    DataDrivenGridPanel->ClearChildren();

    if (CurrentSelectedTabIndex == 0)
    {
        TArray<FInformationSet> ActorInformation = IEntityCoreInformationInterface::Execute_GetSettingsVariableSets(SelectedActor);

        UpdateContentTable(ActorInformation);
    }
    else
    {
        if(LastSelectedActorComponent != nullptr)
        {
            IEntityCoreInformationInterface* LastInterface = Cast<IEntityCoreInformationInterface>(LastSelectedActorComponent);
            if (LastInterface)
            {
                FOnInformationUpdated& UpdateEvent = LastInterface->GetOnInformationUpdated();
                UpdateEvent.RemoveDynamic(this, &UEntityLiveEditorUtilityWidget::HandleInformationUpdated);
            }
            else
            {
				UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: LastSelectedActorComponent does not implement IEntityCoreInformationInterface.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME)
				);
            }
		}

        TArray<UActorComponent*> Components = SelectedActor->GetComponentsByInterface(UEntityCoreInformationInterface::StaticClass());

        if (Components.Num() == 0)
        {
            UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No Components implementing UEntityCoreInformationInterface found on Actor: {ActorName}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("ActorName", SelectedActor->GetName())
            );
            return;
        }

        if(CurrentSelectedTabIndex >= Components.Num() + 1)
        {
            UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: CurrentSelectedTabIndex {Index} is out of bounds for Components array of size {Size}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Index", CurrentSelectedTabIndex),
                ("Size", Components.Num())
            );
            return;
        }

        TArray<FInformationSet> ComponentInformation = IEntityCoreInformationInterface::Execute_GetSettingsVariableSets(Components[CurrentSelectedTabIndex - 1]);

        IEntityCoreInformationInterface* Interface = Cast<IEntityCoreInformationInterface>(Components[CurrentSelectedTabIndex - 1]);

        if (!Interface)
        {
            UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Component at index {Index} does not implement IEntityCoreInformationInterface.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Index", CurrentSelectedTabIndex));
        }
        else
        {
            FOnInformationUpdated& UpdateEvent = Interface->GetOnInformationUpdated();
            UpdateEvent.AddDynamic(this, &UEntityLiveEditorUtilityWidget::HandleInformationUpdated);
			LastSelectedActorComponent = Components[CurrentSelectedTabIndex - 1];
        }

        if (ComponentInformation.Num() == 0)
        {
            UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No Information Sets found in Component at index {Index}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Index", CurrentSelectedTabIndex)
            );
            return;
        }
		
        UpdateContentTable(ComponentInformation);
    }
}

void UEntityLiveEditorUtilityWidget::UpdateContentTable(TArray<FInformationSet> Information)
{
    if(Information.Num() == 0)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No Information to display.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }
    if(!DataDrivenGridPanel)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Error, "{Time}: {Line} {Class}: DataDrivenGridPanel is not set.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

    int32 Row = 0; // keep increasing per entry

    for (const FInformationSet& InfoSet : Information)
    {
        if (InfoSet.InformationValues.Num() == 0)
        {
            UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: InformationSet '{InformationName}' has no values.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("InformationName", InfoSet.InformationName)
			);
            continue;
        }

        UTextBlock* SetCategory = NewObject<UTextBlock>(this);

        if (!SetCategory)
        {
            UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Error, "{Time}: {Line} {Class}: Failed to create TextBlock for InformationSet Category.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME)
			);
			continue;
        }

        DataDrivenGridPanel->AddChildToGrid(SetCategory);

		SetCategory->SetText(FText::FromString(InfoSet.InformationCategory));
		SetCategory->SetToolTipText(FText::FromString(InfoSet.InformationDescription));

        if (UGridSlot* CategorySlot = DataDrivenGridPanel->AddChildToGrid(SetCategory))
        {
            CategorySlot->SetRow(Row);
            CategorySlot->SetColumn(0);
            //Slot->SetPadding(FMargin(2.f));
            // Slot->SetHorizontalAlignment(HAlign_Fill); // optional
        }

        Row++; // next entry goes to the next row

        for (const FInformationSetInformation& InfoValue : InfoSet.InformationValues)
        {
            if (!InfoValue.VisibleName.IsEmpty())
            {
                UTextBlock* CategoryValueName = NewObject<UTextBlock>(this);

                if (!CategoryValueName)
                {
                    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Error, "{Time}: {Line} {Class}: Failed to create TextBlock for InformationSet Value.",
                        ("Class", GET_CLASSNAME_WITH_FUNCTION),
                        ("Line", GET_LINE_NUMBER),
                        ("Time", GET_CURRENT_TIME)
                    );
                    continue;
                }

                DataDrivenGridPanel->AddChildToGrid(CategoryValueName);

                CategoryValueName->SetText(FText::FromString(InfoValue.VisibleName));

                // Get current font info and modify the size
                FSlateFontInfo FontInfo = CategoryValueName->GetFont();
                FontInfo.Size = 12; // any point size you want
                CategoryValueName->SetFont(FontInfo);
                //CategoryValueName->SetAutoWrapText(true); // optional

                if (UGridSlot* NameSlot = DataDrivenGridPanel->AddChildToGrid(CategoryValueName))
                {
                    NameSlot->SetRow(Row);
                    NameSlot->SetColumn(1);
                    NameSlot->SetPadding(FMargin(2.f));
                    // Slot->SetHorizontalAlignment(HAlign_Fill); // optional
                }
            }

            if (!InfoValue.Value.IsEmpty())
            {
                UTextBlock* CategoryValue= NewObject<UTextBlock>(this);

                if (!CategoryValue)
                {
                    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Error, "{Time}: {Line} {Class}: Failed to create TextBlock for InformationSet Value.",
                        ("Class", GET_CLASSNAME_WITH_FUNCTION),
                        ("Line", GET_LINE_NUMBER),
                        ("Time", GET_CURRENT_TIME)
                    );
                    continue;
                }

                DataDrivenGridPanel->AddChildToGrid(CategoryValue);

                CategoryValue->SetText(FText::FromString(InfoValue.Value));
                FSlateFontInfo FontInfo = CategoryValue->GetFont();
                FontInfo.Size = 14; // any point size you want
                CategoryValue->SetFont(FontInfo);
                //CategoryValue->SetAutoWrapText(true); // optional

                if (UGridSlot* ValueSlot = DataDrivenGridPanel->AddChildToGrid(CategoryValue))
                {
                    ValueSlot->SetRow(Row);
                    ValueSlot->SetColumn(2);
                    ValueSlot->SetPadding(FMargin(2.f));
                    // Slot->SetHorizontalAlignment(HAlign_Fill); // optional
                }
            }

            ++Row; // next entry goes to the next row
        }
    }
}

void UEntityLiveEditorUtilityWidget::HandleInformationUpdated(const TArray<FInformationSet>& Information)
{

}

void UEntityLiveEditorUtilityWidget::UpdateMenuButtons()
{
    if (!ButtonClass)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Error, "{Time}: {Line} {Class}: ButtonClass is not set.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }
    
    if(ButtonWidgets.Num() > 0)
    {
        for(UEditorUtilityButtonWidget* ButtonWidget : ButtonWidgets)
        {
            if(ButtonWidget)
            {
				ButtonWidget->OnButtonClickedDelegate.RemoveAll(this);
                ButtonWidget->RemoveFromParent();
            }
        }
	    
        ButtonWidgets.Empty();
	}

    if (SelectedActors.Num() == 0)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No valid Actors Selected after filtering.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

    if(!SelectedActor)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: No Selected Actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
		return;
	}

    if(SelectedActors.Num() > 0)
    {
        for(int32 i = 0; i < SelectedActors.Num(); ++i)
        {
            AActor* Actor = SelectedActors[i];
            if(!Actor)
            {
                continue;
            }

            UEditorUtilityButtonWidget* ButtonWidget = CreateWidget<UEditorUtilityButtonWidget>(this, ButtonClass);
    
            if(ButtonWidget)
            {
                ButtonWidget->SetText(FText::FromString(Actor->GetActorNameOrLabel()));
                ButtonWidget->OnButtonClickedDelegate.AddDynamic(this, &UEntityLiveEditorUtilityWidget::RefreshCurrentSelectedActor);
                EntityList->AddChild(ButtonWidget);
                ButtonWidgets.Add(ButtonWidget);

                if(CurrentSelectedActorIndex == i)
                {
                    ButtonWidget->SetSelected(true);
                }
                else
                {
                    ButtonWidget->SetSelected(false);
				}
            }
        }
	}
}

void UEntityLiveEditorUtilityWidget::UpdateTabButtons()
{
    if(!TabButtonClass)
    {
		UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Error, "{Time}: {Line} {Class}: TabButton Class is not set.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }
    
    if(ActorInformationButtons.Num() > 0)
    {
        for(UEditorUtilityTabButtonWidget* TabButtonWidget : ActorInformationButtons)
        {
            if(TabButtonWidget)
            {
				TabButtonWidget->OnTabButtonClickedDelegate.RemoveAll(this);
                TabButtonWidget->RemoveFromParent();
            }
        }
        
		ActorInformationButtons.Empty();
    }
    
    if(!SelectedActor)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: No Selected Actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

    if(!SelectedActor->GetClass()->ImplementsInterface(UEntityCoreInformationInterface::StaticClass()))
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Selected Actor does not implement UEntityCoreInformationInterface.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
	}

    UEditorUtilityTabButtonWidget* TabButtonWidget = CreateWidget<UEditorUtilityTabButtonWidget>(this, TabButtonClass);

    if (TabButtonWidget)
    {
		FText TabName = FText::FromName(IEntityCoreInformationInterface::Execute_GetCoreInformationName(SelectedActor));
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Creating Tab Button for Actor: {ActorName} with Tab Name: {TabName}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
			("ActorName", SelectedActor->GetName()),
            ("TabName", TabName.ToString())
		);
        ScrollBox_EntityInformationButtons->AddChild(TabButtonWidget);

        TabButtonWidget->SetTabName(TabName);
        TabButtonWidget->SetSelected(CurrentSelectedTabIndex == 0);
        TabButtonWidget->OnTabButtonClickedDelegate.AddDynamic(this, &UEntityLiveEditorUtilityWidget::RefreshCurrentTabSelection);
        ActorInformationButtons.Add(TabButtonWidget);
    }

	TArray<UActorComponent*> Components = SelectedActor->GetComponentsByInterface(UEntityCoreInformationInterface::StaticClass());
    
    if(Components.Num() == 0)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No Components implementing UEntityCoreInformationInterface found on Actor: {ActorName}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("ActorName", SelectedActor->GetName())
        );
        return;
    }
    for (UActorComponent* Component : Components)
    {
        if (!Component)
        {
            continue;
        }
		UEditorUtilityTabButtonWidget* ComponentTabButtonWidget = CreateWidget<UEditorUtilityTabButtonWidget>(this, TabButtonClass);
        if (ComponentTabButtonWidget)
        {
            ScrollBox_EntityInformationButtons->AddChild(ComponentTabButtonWidget);

            ComponentTabButtonWidget->SetTabName(FText::FromName(IEntityCoreInformationInterface::Execute_GetCoreInformationName(Component)));
			ComponentTabButtonWidget->SetSelected(CurrentSelectedTabIndex == ActorInformationButtons.Num());
            ComponentTabButtonWidget->OnTabButtonClickedDelegate.AddDynamic(this, &UEntityLiveEditorUtilityWidget::RefreshCurrentTabSelection);
            ActorInformationButtons.Add(ComponentTabButtonWidget);
        }
    }
    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Updated Tab Buttons for Actor: {ActorName}.",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("ActorName", SelectedActor->GetName())
	);
}

void UEntityLiveEditorUtilityWidget::UpdateCoreEntityList()
{
    if(SelectedActors.Num() == 0)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No Actors Selected.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
	}

	TArray<AActor*> NotCoreEntities;

    for (int32 i = 0; i < SelectedActors.Num(); i++)
    {
        if(!SelectedActors[i])
        {
            UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Selected Actor at index {Index} is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Index", i)
			);
            continue;
		}

        if(!SelectedActors[i]->GetClass()->ImplementsInterface(UEntityCoreInformationInterface::StaticClass()))
        {
			UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Selected Actor at index {Index} does not implement UEntityCoreInformationInterface.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Index", i)
			);
            NotCoreEntities.Add(SelectedActors[i]);
        }
        else
        {
			IEntityCoreInformationInterface::Execute_SetEntityCoreLoggingEnabled(SelectedActors[i], true);
        }
    }

    if(NotCoreEntities.Num() > 0)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Selected Actors do not implement UEntityCoreInformationInterface: {Actors}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
			("Actors", FString::JoinBy(NotCoreEntities, TEXT(", "), [](const AActor* Actor) { return Actor ? Actor->GetName() : TEXT("null"); }))
            );

        for(auto Actor : NotCoreEntities)
        {
            if(Actor)
            {
				SelectedActors.Remove(Actor);
				UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Removed Actor: {ActorName}.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("ActorName", Actor->GetName())
				);
            }
        }
    }

}

void UEntityLiveEditorUtilityWidget::SetSelectedActor()
{
    if (SelectedActors.Num() == 0)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No valid Actors Selected after filtering.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

    SelectedActor = SelectedActors[CurrentSelectedActorIndex];

    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Selected Actors: {Num}.",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("Num", SelectedActors.Num())
    );

    if (SelectedActor)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Selected Actor: {ActorName}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("ActorName", SelectedActor->GetName())
        );

        Text_EntityName->SetText(FText::FromString(SelectedActor->GetActorNameOrLabel()));
    }
    else
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Selected Actor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
		);
    }
}

void UEntityLiveEditorUtilityWidget::RefreshCurrentSelectedActor(UUserWidget* ButtonWidget)
{
    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Refreshing current selected actor from ButtonWidget.",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME)
	);

    if (!ButtonWidget)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: ButtonWidget is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

    UEditorUtilityButtonWidget* UtilityButton = Cast<UEditorUtilityButtonWidget>(ButtonWidget);
    if (!UtilityButton)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: ButtonWidget is not of type UEditorUtilityButtonWidget.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }
    
    int32 Index = ButtonWidgets.IndexOfByKey(UtilityButton);
    if (Index == INDEX_NONE || Index >= SelectedActors.Num())
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Invalid ButtonWidget index: {Index}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Index", Index)
        );
        return;
    }
    
    CurrentSelectedActorIndex = Index;
    CurrentSelectedTabIndex = 0; // Reset to first tab
    
    SetSelectedActor();
    
    UpdateMenuButtons();
    
    UpdateTabButtons();
    
    UpdateContent();
    
    // Notify that the selection has changed
	OnEditorActorSelectionChanged.Broadcast();
}

void UEntityLiveEditorUtilityWidget::RefreshCurrentTabSelection(UUserWidget* TabButtonWidget)
{
    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Refreshing current tab selection from TabButtonWidget.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
    );

    if (!TabButtonWidget)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: TabButtonWidget is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

    UEditorUtilityTabButtonWidget* UtilityTabButton = Cast<UEditorUtilityTabButtonWidget>(TabButtonWidget);
    if (!UtilityTabButton)
    {
		UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: TabButton Widget is not of type UEditorUtilityTabButtonWidget.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

    int32 Index = ActorInformationButtons.IndexOfByKey(UtilityTabButton);
    if (Index == INDEX_NONE || Index >= ActorInformationButtons.Num())
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Invalid TabButtonWidget index: {Index}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Index", Index)
        );
        return;
    }

    CurrentSelectedTabIndex = Index;
	UpdateContent();
    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Current Selected Tab Index: {Index}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("Index", CurrentSelectedTabIndex)
    );
    
	// Notify that the selection has changed
    OnEditorActorSelectionChanged.Broadcast();
    
    // Optionally, you can also update the UI to reflect the new tab selection
    for (UEditorUtilityTabButtonWidget* Button : ActorInformationButtons)
    {
        if (Button)
        {
            Button->SetSelected(Button == UtilityTabButton);
        }
    }
    
    UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Updated content for current tab selection.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME)
	);
}

void UEntityLiveEditorUtilityWidget::RefreshSelection()
{
    if(SelectedActors.Num() > 0)
    {
        for (AActor* Actor : SelectedActors)
            {
            if (Actor && Actor->GetClass()->ImplementsInterface(UEntityCoreInformationInterface::StaticClass()))
            {
                IEntityCoreInformationInterface::Execute_SetEntityCoreLoggingEnabled(Actor, false);
            }
        }
    }

	Text_EntityName->SetText(FText::GetEmpty());
    SelectedActors.Empty();
    CurrentSelectedActorIndex = 0;
	CurrentSelectedTabIndex = 0;
    SelectedActor = nullptr;
    if (GEditor)
    {
        if (USelection* Sel = GEditor->GetSelectedActors())
        {
            Sel->GetSelectedObjects<AActor>(SelectedActors);
        }
    }

    if (SelectedActors.Num() == 0)
    {
        UE_LOGFMT(Log_EntityActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No Actors Selected.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return;
    }

}

void UEntityLiveEditorUtilityWidget::HandleSelectObject(UObject* /*SelectedObject*/)
{
    // Optional immediate reaction
    OnEditorActorSelectionChanged.Broadcast();
    RefreshSelection();
}