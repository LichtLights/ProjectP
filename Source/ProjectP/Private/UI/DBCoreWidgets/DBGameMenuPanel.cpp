// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBCoreWidgets/DBGameMenuPanel.h"

#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "UI/DBCoreWidgets/DBInGameMenu.h"
#include "UI/DBCoreWidgets/InventoryWidgets/DBInventoryPanel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBGameMenuPanel)

UDBGameMenuPanel::UDBGameMenuPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDBGameMenuPanel::RefreshPanel_Implementation()
{
	ClearGameMenuModalPanel();
}

UCommonActivatableWidget* UDBPanelExtension::PushWidgetToInGameMenuModalPanel(const ULocalPlayer* LocalPlayer,
                                                                              UCommonActivatableWidget* WidgetToPush, bool &bPushSuccess)
{
	bPushSuccess = false;
	
	if (!ensure(LocalPlayer) || !ensure(WidgetToPush != nullptr))
	{
		return nullptr;
	}

	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				UCommonActivatableWidgetContainerBase* GameMenuLayer = RootLayout->GetLayerWidget(
					FGameplayTag::RequestGameplayTag(FName("UI.Layer.GameMenu"))
				);
					
				auto ActiveMenu = GameMenuLayer->GetActiveWidget();
				if (!ActiveMenu) return nullptr;

				if (auto InGameMenu = Cast<UDBInGameMenu>(ActiveMenu))
				{
					bPushSuccess = InGameMenu->AddCommonWidgetToCurrentModalPanel(WidgetToPush);
				}

			}
		}
	}

	return WidgetToPush;
}

bool UDBPanelExtension::PopWidgetFromInGameMenuModalPanel(UCommonActivatableWidget* WidgetToPop)
{
	if (!WidgetToPop)
	{
		// Ignore request to pop an already deleted widget
		return false;
	}

	if (const ULocalPlayer* LocalPlayer = WidgetToPop->GetOwningLocalPlayer())
	{
		if (const UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
		{
			if (const UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
				{
					UCommonActivatableWidgetContainerBase* GameMenuLayer = RootLayout->GetLayerWidget(
						FGameplayTag::RequestGameplayTag(FName("UI.Layer.GameMenu"))
					);
					
					auto ActiveMenu = GameMenuLayer->GetActiveWidget();
					if (!ActiveMenu) return false;

					if (auto InGameMenu = Cast<UDBInGameMenu>(ActiveMenu))
					{
						return InGameMenu->RemoveCommonWidgetFromCurrentModalPanel(WidgetToPop, true);
					}
				}
			}
		}
	}
	return false;
}

void UDBPanelExtension::ClearInGameMenuModalPanel()
{

}
