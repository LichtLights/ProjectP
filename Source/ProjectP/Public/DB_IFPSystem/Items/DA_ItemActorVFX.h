// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ItemActorVFX.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECTP_API UDA_ItemActorVFX : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rarity Aura")
	UNiagaraSystem* VFXRarityAuraCommon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rarity Aura")
	UNiagaraSystem* VFXRarityAuraMagic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rarity Aura")
	UNiagaraSystem* VFXRarityAuraRare;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rarity Aura")
	UNiagaraSystem* VFXRarityAuraUnique;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Picked Up Pop")
	UNiagaraSystem* VFXPickedUpPopCommon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Picked Up Pop")
	UNiagaraSystem* VFXPickedUpPopMagic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Picked Up Pop")
	UNiagaraSystem* VFXPickedUpPopRare;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Picked Up Pop")
	UNiagaraSystem* VFXPickedUpPopUnique;
};
