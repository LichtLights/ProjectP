﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DBVerbMessageHelpers.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffectTypes.h"
#include "Messages/DBVerbMessage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBVerbMessageHelpers)

//////////////////////////////////////////////////////////////////////
// FDBVerbMessage

FString FDBVerbMessage::ToString() const
{
	FString HumanReadableMessage;
	FDBVerbMessage::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, this, /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
	return HumanReadableMessage;
}

//////////////////////////////////////////////////////////////////////
//

APlayerState* UDBVerbMessageHelpers::GetPlayerStateFromObject(UObject* Object)
{
	if (APlayerController* PC = Cast<APlayerController>(Object))
	{
		return PC->PlayerState;
	}

	if (APlayerState* TargetPS = Cast<APlayerState>(Object))
	{
		return TargetPS;
	}
	
	if (APawn* TargetPawn = Cast<APawn>(Object))
	{
		if (APlayerState* TargetPS = TargetPawn->GetPlayerState())
		{
			return TargetPS;
		}
	}
	return nullptr;
}

APlayerController* UDBVerbMessageHelpers::GetPlayerControllerFromObject(UObject* Object)
{
	if (APlayerController* PC = Cast<APlayerController>(Object))
	{
		return PC;
	}

	if (APlayerState* TargetPS = Cast<APlayerState>(Object))
	{
		return TargetPS->GetPlayerController();
	}

	if (APawn* TargetPawn = Cast<APawn>(Object))
	{
		return Cast<APlayerController>(TargetPawn->GetController());
	}

	return nullptr;
}

FGameplayCueParameters UDBVerbMessageHelpers::VerbMessageToCueParameters(const FDBVerbMessage& Message)
{
	FGameplayCueParameters Result;

	Result.OriginalTag = Message.Verb;
	Result.Instigator = Cast<AActor>(Message.Instigator);
	Result.EffectCauser = Cast<AActor>(Message.Target);
	Result.AggregatedSourceTags = Message.InstigatorTags;
	Result.AggregatedTargetTags = Message.TargetTags;
	//@TODO: = Message.ContextTags;
	Result.RawMagnitude = Message.Magnitude;

	return Result;
}

FDBVerbMessage UDBVerbMessageHelpers::CueParametersToVerbMessage(const FGameplayCueParameters& Params)
{
	FDBVerbMessage Result;
	
	Result.Verb = Params.OriginalTag;
	Result.Instigator = Params.Instigator.Get();
	Result.Target = Params.EffectCauser.Get();
	Result.InstigatorTags = Params.AggregatedSourceTags;
	Result.TargetTags = Params.AggregatedTargetTags;
	//@TODO: Result.ContextTags = ???;
	Result.Magnitude = Params.RawMagnitude;

	return Result;
}
