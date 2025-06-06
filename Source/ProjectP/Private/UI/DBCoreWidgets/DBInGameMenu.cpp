// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBCoreWidgets/DBInGameMenu.h"

#include "Components/CanvasPanel.h"

UCanvasPanel* UDBInGameMenu::GetCurrentModalPanel_Implementation()
{
	// This will trigger a runtime error if this function is not overridden in blueprint.
	checkf(false, TEXT("GetCurrentModalPanel must be overridden in your widget blueprint!"));
	
	return nullptr;
}

bool UDBInGameMenu::AddCommonWidgetToCurrentModalPanel_Implementation(UCommonActivatableWidget* WidgetToAdd)
{
	if (!ensure(WidgetToAdd != nullptr)) return false;

	UCanvasPanel* CurrentModalPanel = GetCurrentModalPanel();

	if (CurrentModalPanel)
	{
		CurrentModalPanel->AddChildToCanvas(WidgetToAdd);
		return true;
	}
	
	return false;
}

bool UDBInGameMenu::RemoveCommonWidgetFromCurrentModalPanel_Implementation(UCommonActivatableWidget* WidgetToRemove, bool bRemoveFromParent)
{
	if (!ensure(WidgetToRemove != nullptr)) return false;

	UCanvasPanel* CurrentModalPanel = GetCurrentModalPanel();

	if (CurrentModalPanel)
	{
		CurrentModalPanel->RemoveChild(WidgetToRemove);
		
		if (bRemoveFromParent)
		{
			WidgetToRemove->RemoveFromParent();
		}
		
		return true;
	}
	
	return false;
}
