﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageHandleInterface.generated.h"

enum class EDamageType : uint8;
// This class does not need to be modified.
UINTERFACE()
class UDamageHandleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTP_API IDamageHandleInterface
{
	GENERATED_BODY()

// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void ReceiveDamage(float Damage, EDamageType DamageType);
	
};
