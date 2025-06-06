// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBActivatableWidget.h"
#include "DBInGameMenu.generated.h"

class UCanvasPanel;

/**	Base class for all InGameMenu.
 *	UDBGameMenuPanel Has ModalPanel which can have many activatable widget at same time.
 *	This class has methods to access currently activated modal panel. This method should be implemented in Widget blueprint.
 */
UCLASS()
class PROJECTP_API UDBInGameMenu : public UDBActivatableWidget
{
	GENERATED_BODY()

public:
	
	/**	Get currently activated modal panel.
	 *	Must be implemented in widget blueprint.
	 *	Do not call the parent function.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCanvasPanel* GetCurrentModalPanel();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddCommonWidgetToCurrentModalPanel(UCommonActivatableWidget* WidgetToAdd);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RemoveCommonWidgetFromCurrentModalPanel(UCommonActivatableWidget* WidgetToRemove, bool bRemoveFromParent);
};
