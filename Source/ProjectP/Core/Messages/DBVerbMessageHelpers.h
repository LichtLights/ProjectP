// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "DBVerbMessageHelpers.generated.h"

struct FGameplayCueParameters;
struct FDBVerbMessage;

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;

/**
 * 
 */
UCLASS()
class PROJECTP_API UDBVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "DB")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "DB")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "DB")
	static FGameplayCueParameters VerbMessageToCueParameters(const FDBVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "DB")
	static FDBVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};

