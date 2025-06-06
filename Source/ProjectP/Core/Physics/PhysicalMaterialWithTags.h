﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "PhysicalMaterialWithTags.generated.h"

/**
 *	UPhysicalMaterialWithTags
 *
 */
UCLASS()
class PROJECTP_API UPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()

public:

	UPhysicalMaterialWithTags(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// A container of gameplay tags that game code can use to reason about this physical material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=PhysicalProperties)
	FGameplayTagContainer Tags;
};
