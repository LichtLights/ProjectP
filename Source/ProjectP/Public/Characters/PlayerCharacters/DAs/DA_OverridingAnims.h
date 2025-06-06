// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_OverridingAnims.generated.h"

class UPaperZDAnimSequence;
/**
 * 
 */
UCLASS()
class PROJECTP_API UDA_OverridingAnims : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* AS_PrimaryAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* AS_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* AS_Parry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* AS_HitReact_Light;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* AS_HitReact_Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* AS_HitReact_Heavy;
	
};
