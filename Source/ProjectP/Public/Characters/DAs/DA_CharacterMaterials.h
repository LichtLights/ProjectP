// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_CharacterMaterials.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UDA_CharacterMaterials : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprite Materials")
	UMaterialInstance* MaskedLitSpriteMaterialInstance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprite Materials")
	UMaterialInstance* MaskedUnlitSpriteMaterialInstance;
	
};
