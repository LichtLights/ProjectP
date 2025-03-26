// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Player/PlayerState/DBPlayerState.h"

#include "AbilitySystem/DBAbilitySet.h"
#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "Characters/DBCharacter.h"
#include "Characters/Components/DBPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "LogChannel/DBLogChannels.h"
#include "Messages/DBVerbMessage.h"
#include "System/GameModes/DBExperienceManagerComponent.h"
#include "System/GameModes/DBGameMode.h"
#include "System/PlayerController/DBPlayerController.h"
#include "System/PrimaryData/DBPawnData.h"

const FName ADBPlayerState::NAME_DBAbilityReady("DBAbilitiesReady");

ADBPlayerState::ADBPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MyPlayerConnectionType(EDBPlayerConnectionType::Player)
{
	
}

ADBPlayerController* ADBPlayerState::GetDBPlayerController() const
{
	return Cast<ADBPlayerController>(GetOwner());
}

UDBAbilitySystemComponent* ADBPlayerState::GetDBAbilitySystemComponent() const
{
	// ADBCharacter* DBCharacter = Cast<ADBCharacter>(GetDBPlayerController()->GetCharacter());
	ADBCharacter* DBCharacter = Cast<ADBCharacter>(GetPawn());
	if (DBCharacter)
	{
		UDBAbilitySystemComponent* DBASC = Cast<UDBAbilitySystemComponent>(DBCharacter->GetAbilitySystemComponent());
		if (DBASC)
		{
			return DBASC;
		}
	}
		
	return nullptr;
}

UAbilitySystemComponent* ADBPlayerState::GetAbilitySystemComponent() const
{
	return GetDBAbilitySystemComponent();
}

void ADBPlayerState::SetPawnData(const UDBPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogDB, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UDBAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_DBAbilityReady);
	
	ForceNetUpdate();
}

void ADBPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADBPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// TODO: ASC has been moved to Character.
	// check(AbilitySystemComponent);
	// AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	// TODO: Moved to PlayerCharacter.
	// UWorld* World = GetWorld();
	// if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	// {
	// 	AGameStateBase* GameState = GetWorld()->GetGameState();
	// 	check(GameState);
	// 	UDBExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDBExperienceManagerComponent>();
	// 	check(ExperienceComponent);
	// 	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnDBExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	// }
}

void ADBPlayerState::Reset()
{
	Super::Reset();
}

void ADBPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	
	if (UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void ADBPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	//@TODO: Copy stats
}

void ADBPlayerState::OnDeactivated()
{
	bool bDestroyDeactivatedPlayerState = false;

	switch (GetPlayerConnectionType())
	{
	case EDBPlayerConnectionType::Player:
	case EDBPlayerConnectionType::InactivePlayer:
		//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
		// (e.g., for long running servers where they might build up if lots of players cycle through)
		bDestroyDeactivatedPlayerState = true;
		break;
	default:
		bDestroyDeactivatedPlayerState = true;
		break;
	}
	
	SetPlayerConnectionType(EDBPlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void ADBPlayerState::OnReactivated()
{
	if (GetPlayerConnectionType() == EDBPlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(EDBPlayerConnectionType::Player);
	}
}

void ADBPlayerState::SetPlayerConnectionType(EDBPlayerConnectionType NewType)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyPlayerConnectionType, this);
	MyPlayerConnectionType = NewType;
}

void ADBPlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void ADBPlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 ADBPlayerState::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool ADBPlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void ADBPlayerState::ClientBroadcastMessage_Implementation(const FDBVerbMessage Message)
{
	// This check is needed to prevent running the action when in standalone mode
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void ADBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ADBPlayerState::OnExperienceLoaded(const UDBExperienceDefinition* CurrentExperience)
{
	// TODO: Moved to PlayerCharacter.
	// if (ADBGameMode* DBGameMode = GetWorld()->GetAuthGameMode<ADBGameMode>())
	// {
	// 	if (const UDBPawnData* NewPawnData = DBGameMode->GetPawnDataForController(GetOwningController()))
	// 	{
	// 		
	// 		SetPawnData(NewPawnData);
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogDB, Error, TEXT("ADBPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
	// 	}
	// }
}

void ADBPlayerState::OnRep_PawnData()
{
	
}
