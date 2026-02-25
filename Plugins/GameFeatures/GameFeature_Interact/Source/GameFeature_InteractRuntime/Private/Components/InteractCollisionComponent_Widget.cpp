#include "Components/InteractCollisionComponent.h"

#include "Blueprint/UserWidget.h"

#include "Interfaces/InteractWidgetInterface.h"

#include "Logging/GF_Interact_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

bool UInteractCollisionComponent::ShowWidget(TSubclassOf<UUserWidget> InWidgetTemplate)
{
	if(InWidgetTemplate == nullptr)
	{
		if (WidgetTemplate.IsNull())
		{
			UE_LOGFMT(Log_Interact_Widget, Error, "{Time}: {Line} {Class}: WidgetTemplate is null. Cannot show widget.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return false;
		}

		if(!WidgetTemplate.IsValid())
		{
			WidgetTemplate.LoadSynchronous();
		}

		InWidgetTemplate = WidgetTemplate.Get();
	}

	if (!InteractWidgetInstance)
	{
		InteractWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InWidgetTemplate);
	}

	if (InteractWidgetInstance)
	{
		InteractWidgetInstance->AddToViewport();

		if (InteractUIImage.IsValid())
		{
			IInteractWidgetInterface::Execute_SetImage(InteractWidgetInstance, InteractUIImage.Get());
		}
		else
		{
			UE_LOGFMT(Log_Interact_Widget, Warning, "{Time}: {Line} {Class}: InteractUIImage is not valid. Using default image.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
		// Set the text of the widget
		FString Text = "Interact with " + GetOwner()->GetFName().ToString();
		IInteractWidgetInterface::Execute_SetText(InteractWidgetInstance, Text);
		return true;
	}
	else
	{
		UE_LOGFMT(Log_Interact_Widget, Error, "{Time}: {Line} {Class}: Failed to create InteractWidgetInstance from template: {Template}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Template", InWidgetTemplate ? *GetNameSafe(InWidgetTemplate) : nullptr),
			("Time", GET_CURRENT_TIME));
		return false;
	}
}

bool UInteractCollisionComponent::RemoveWidget()
{
	if (InteractWidgetInstance)
	{
		// Remove the widget from the viewport
		InteractWidgetInstance->RemoveFromParent();
		UE_LOGFMT(Log_Interact_Widget, VeryVerbose, "{Time}: {Line} {Class}: Successfully removed InteractWidgetInstance from viewport.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return true;
	}
	else
	{
		UE_LOGFMT(Log_Interact_Widget, Verbose, "{Time}: {Line} {Class}: InteractWidgetInstance is null. Cannot set text for interaction.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}
}