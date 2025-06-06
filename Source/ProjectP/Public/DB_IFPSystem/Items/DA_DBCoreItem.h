// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Items/DA_CoreItem.h"
#include "DA_DBCoreItem.generated.h"

class UPaperSprite;
/**
 * 
 */
UCLASS()
class PROJECTP_API UDA_DBCoreItem : public UDA_CoreItem
{
	GENERATED_BODY()

public:

	/**Item sprite representing this item.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core Settings", Meta = (MultiLine = "True"), AssetRegistrySearchable)
	UPaperSprite* ItemSprite = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Core Settings")
	TSubclassOf<class UW_AttachmentParent> AttachmentWidget;
	
	virtual TSubclassOf<UW_AttachmentParent> GetAttachmentWidgetClass() override;
};
