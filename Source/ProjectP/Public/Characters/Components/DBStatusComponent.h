// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkComponent.h"

#include "DBStatusComponent.generated.h"


struct FGameplayEffectSpec;

class UDBStatusAttributeSet;
class UDBAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDBStatus_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDBStatus_AttributeChanged, UDBStatusComponent*, StatusComponent, float, OldValue, float, NewValue, AActor*, Instigator);


/**
 *	EDBDeathState
 *
 *	Defines current state of death.
 */
UENUM(BlueprintType)
enum class EDBDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

/**
 *	UDBHealthComponent
 *
 *	An actor component used to handle anything related to health.
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class PROJECTP_API UDBStatusComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	// Sets default values for this component's properties
	UDBStatusComponent(const FObjectInitializer& ObjectInitializer);
	
	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "DB|Health")
	static UDBStatusComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UDBStatusComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "DB|Health")
	void InitializeWithAbilitySystem(UDBAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "DB|Health")
	void UninitializeFromAbilitySystem();


	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "DB|Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "DB|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "DB|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "DB|Health")
	EDBDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "DB|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > EDBDeathState::NotDead); }

	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

	// Applies enough damage to kill the owner.
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);
	

public:

	// Delegate fired when the health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FDBStatus_AttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FDBStatus_AttributeChanged OnMaxHealthChanged;

	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FDBStatus_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FDBStatus_DeathEvent OnDeathFinished;
	
	
protected:

	virtual void OnUnregister() override;

	void ClearGameplayTags();

	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UFUNCTION()
	virtual void OnRep_DeathState(EDBDeathState OldDeathState);

	
protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UDBAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UDBStatusAttributeSet> StatusAttributeSet;

	// Replicated state used to handle dying.
	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	EDBDeathState DeathState;
};
