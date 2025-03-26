// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/CanvasPanel.h"
#include "DBGameMenuPanel.generated.h"

class UCommonActivatableWidget;
struct FGameplayTag;
class UCanvasPanel;

/**	GameMenuPanel base class.
 *	This class has @GameMenuModalPanel which can have multiple activatable widget at the same time.
 *	Can push, pop via BPFL functions from UDBPanelExtension.
 */
UCLASS()
class PROJECTP_API UDBGameMenuPanel : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	UDBGameMenuPanel(const FObjectInitializer& ObjectInitializer);

	/**	The function for refreshing panel. like re-binding widgets, clearing modal panel etc.
	 *	Should be implemented in blueprint for each specific GameMenuPanel.
	 *	This function calls @ClearGameMenuModalPanel by default.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RefreshPanel();

	UFUNCTION(BlueprintCallable)
	UCanvasPanel* GetGameMenuModalPanel() { return GameMenuModalPanel; }

	UFUNCTION(BlueprintCallable)
	void ClearGameMenuModalPanel() const { GameMenuModalPanel->ClearChildren(); }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryPanel", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> GameMenuModalPanel;
};


UCLASS()
class PROJECTP_API UDBPanelExtension : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static UCommonActivatableWidget* PushWidgetToInGameMenuModalPanel
	(
		const ULocalPlayer* LocalPlayer,
		UCommonActivatableWidget* WidgetToPush,
		bool &bPushSuccess
	);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static bool PopWidgetFromInGameMenuModalPanel(UCommonActivatableWidget* WidgetToPop);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static void ClearInGameMenuModalPanel();
};