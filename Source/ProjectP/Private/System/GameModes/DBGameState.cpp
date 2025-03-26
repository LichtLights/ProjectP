// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameModes/DBGameState.h"

#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "System/GameModes/DBExperienceManagerComponent.h"

ADBGameState::ADBGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDBAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExperienceManagerComponent = CreateDefaultSubobject<UDBExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));

	// ServerFPS = 0.0f;
}

void ADBGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADBGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

void ADBGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ADBGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if (GetLocalRole() == ROLE_Authority)
	// {
	// 	ServerFPS = GAverageFPS;
	// }
}

void ADBGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void ADBGameState::RemovePlayerState(APlayerState* PlayerState)
{
	//@TODO: This isn't getting called right now (only the 'rich' AGameMode uses it, not AGameModeBase)
	// Need to at least comment the engine code, and possibly move things around
	Super::RemovePlayerState(PlayerState);
}

void ADBGameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap)
{
	// Super::SeamlessTravelTransitionCheckpoint(bToTransitionMap);

	// Remove inactive and bots
	for (int32 i = PlayerArray.Num() - 1; i >= 0; i--)
	{
		APlayerState* PlayerState = PlayerArray[i];
		if (PlayerState && (PlayerState->IsABot() || PlayerState->IsInactive()))
		{
			RemovePlayerState(PlayerState);
		}
	}
}

UAbilitySystemComponent* ADBGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// void ADBGameState::MulticastMessageToClients_Implementation(const FDBVerbMessage Message)
// {
// 	if (GetNetMode() == NM_Client)
// 	{
// 		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
// 	}
// }
//
// void ADBGameState::MulticastReliableMessageToClients_Implementation(const FDBVerbMessage Message)
// {
// 	MulticastMessageToClients_Implementation(Message);
// }
//
// float ADBGameState::GetServerFPS() const
// {
// 	return ServerFPS;
// }

// void ADBGameState::SetRecorderPlayerState(APlayerState* NewPlayerState)
// {
// 	if (RecorderPlayerState == nullptr)
// 	{
// 		// Set it and call the rep callback so it can do any record-time setup
// 		RecorderPlayerState = NewPlayerState;
// 		OnRep_RecorderPlayerState();
// 	}
// 	else
// 	{
// 		UE_LOG(LogLyra, Warning, TEXT("SetRecorderPlayerState was called on %s but should only be called once per game on the primary user"), *GetName());
// 	}
// }
//
// APlayerState* ADBGameState::GetRecorderPlayerState() const
// {
// 	// TODO: Maybe auto select it if null?
//
// 	return RecorderPlayerState;
// }

// void ADBGameState::OnRep_RecorderPlayerState()
// {
// 	OnRecorderPlayerStateChangedEvent.Broadcast(RecorderPlayerState);
// }
