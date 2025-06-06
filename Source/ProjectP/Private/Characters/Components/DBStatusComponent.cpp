// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/DBStatusComponent.h"

#include "AbilitySystem/AttributeSets/DBStatusAttributeSet.h"
#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DBGameplayTags.h"
#include "LogChannel/DBLogChannels.h"
#include "Messages/DBVerbMessage.h"
#include "Messages/DBVerbMessageHelpers.h"
#include "System/ManagerClasses/DBAssetManager.h"
#include "System/PrimaryData/DBGameData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBStatusComponent)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_DB_Elimination_Message, "DB.Elimination.Message");

void UDBStatusComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UDBStatusComponent::UDBStatusComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	StatusAttributeSet = nullptr;
	DeathState = EDBDeathState::NotDead;
}

void UDBStatusComponent::InitializeWithAbilitySystem(UDBAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogDB, Error, TEXT("DBHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogDB, Error, TEXT("DBHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	StatusAttributeSet = AbilitySystemComponent->GetSet<UDBStatusAttributeSet>();
	if (!StatusAttributeSet)
	{
		UE_LOG(LogDB, Error, TEXT("DBHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// Register to listen for attribute changes.
	StatusAttributeSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	StatusAttributeSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	StatusAttributeSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	AbilitySystemComponent->SetNumericAttributeBase(UDBStatusAttributeSet::GetHealthAttribute(), StatusAttributeSet->GetMaxHealth());

	ClearGameplayTags();

	OnHealthChanged.Broadcast(this, StatusAttributeSet->GetHealth(), StatusAttributeSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, StatusAttributeSet->GetHealth(), StatusAttributeSet->GetHealth(), nullptr);
}

void UDBStatusComponent::UninitializeFromAbilitySystem()
{
	ClearGameplayTags();

	if (StatusAttributeSet)
	{
		StatusAttributeSet->OnHealthChanged.RemoveAll(this);
		StatusAttributeSet->OnMaxHealthChanged.RemoveAll(this);
		StatusAttributeSet->OnOutOfHealth.RemoveAll(this);
	}

	StatusAttributeSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UDBStatusComponent::GetHealth() const
{
	return (StatusAttributeSet ? StatusAttributeSet->GetHealth() : 0.0f);
}

float UDBStatusComponent::GetMaxHealth() const
{
	return (StatusAttributeSet ? StatusAttributeSet->GetMaxHealth() : 0.0f);
}

float UDBStatusComponent::GetHealthNormalized() const
{
	if (StatusAttributeSet)
	{
		const float Health = StatusAttributeSet->GetHealth();
		const float MaxHealth = StatusAttributeSet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}

void UDBStatusComponent::StartDeath()
{
	if (DeathState != EDBDeathState::NotDead)
	{
		return;
	}

	DeathState = EDBDeathState::DeathStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DBGameplayTags::Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UDBStatusComponent::FinishDeath()
{
	if (DeathState != EDBDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EDBDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DBGameplayTags::Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UDBStatusComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
	if ((DeathState == EDBDeathState::NotDead) && AbilitySystemComponent)
	{
		const TSubclassOf<UGameplayEffect> DamageGE = UDBAssetManager::GetSubclass(UDBGameData::Get().DamageGameplayEffect_SetByCaller);
		if (!DamageGE)
		{
			UE_LOG(LogDB, Error, TEXT("DBHealthComponent: DamageSelfDestruct failed for owner [%s]. Unable to find gameplay effect [%s]."), *GetNameSafe(GetOwner()), *UDBGameData::Get().DamageGameplayEffect_SetByCaller.GetAssetName());
			return;
		}

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		if (!Spec)
		{
			UE_LOG(LogDB, Error, TEXT("DBHealthComponent: DamageSelfDestruct failed for owner [%s]. Unable to make outgoing spec for [%s]."), *GetNameSafe(GetOwner()), *GetNameSafe(DamageGE));
			return;
		}

		Spec->AddDynamicAssetTag(TAG_Gameplay_DamageSelfDestruct);

		if (bFellOutOfWorld)
		{
			Spec->AddDynamicAssetTag(TAG_Gameplay_FellOutOfWorld);
		}

		const float DamageAmount = GetMaxHealth();

		Spec->SetSetByCallerMagnitude(DBGameplayTags::SetByCaller_Damage, DamageAmount);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void UDBStatusComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();

	Super::OnUnregister();	
}

void UDBStatusComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DBGameplayTags::Status_Death_Dying, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(DBGameplayTags::Status_Death_Dead, 0);
	}
}

void UDBStatusComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UDBStatusComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UDBStatusComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
// TODO: SERVER to Single-player.
#if WITH_SERVER_CODE
	if (AbilitySystemComponent && DamageEffectSpec)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		{
			FGameplayEventData Payload;
			Payload.EventTag = DBGameplayTags::GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target = AbilitySystemComponent->GetAvatarActor();
			Payload.OptionalObject = DamageEffectSpec->Def;
			Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}

		// Send a standardized verb message that other systems can observe
		{
			FDBVerbMessage Message;
			Message.Verb = TAG_DB_Elimination_Message;
			Message.Instigator = DamageInstigator;
			Message.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
			Message.Target = UDBVerbMessageHelpers::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor());
			Message.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
			//@TODO: Fill out context tags, and any non-ability-system source/instigator tags
			//@TODO: Determine if it's an opposing team kill, self-own, team kill, etc...

			UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSystem.BroadcastMessage(Message.Verb, Message);
		}

		//@TODO: assist messages (could compute from damage dealt elsewhere)?
	}

#endif // #if WITH_SERVER_CODE
}

void UDBStatusComponent::OnRep_DeathState(EDBDeathState OldDeathState)
{
	const EDBDeathState NewDeathState = DeathState;

	// Revert the death state for now since we rely on StartDeath and FinishDeath to change it.
	DeathState = OldDeathState;

	if (OldDeathState > NewDeathState)
	{
		// The server is trying to set us back but we've already predicted past the server state.
		UE_LOG(LogDB, Warning, TEXT("DBHealthComponent: Predicted past server death state [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		return;
	}

	if (OldDeathState == EDBDeathState::NotDead)
	{
		if (NewDeathState == EDBDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (NewDeathState == EDBDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
		else
		{
			UE_LOG(LogDB, Error, TEXT("DBHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}
	else if (OldDeathState == EDBDeathState::DeathStarted)
	{
		if (NewDeathState == EDBDeathState::DeathFinished)
		{
			FinishDeath();
		}
		else
		{
			UE_LOG(LogDB, Error, TEXT("DBHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}

	ensureMsgf((DeathState == NewDeathState), TEXT("DBHealthComponent: Death transition failed [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
}
