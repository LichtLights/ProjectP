// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/DBEnemyCharacter.h"

#include "GlobalConstants.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Components/PaperCharacterMovementComponent.h"
#include "Characters/Enemies/EnemyComponents/EnemyAbilitySystemComponent.h"

// Sets default values
ADBEnemyCharacter::ADBEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UPaperCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)
	)
{
	// Components
	AbilitySystemComp = ObjectInitializer.CreateDefaultSubobject<UEnemyAbilitySystemComponent>(this, TEXT("Ability System Component"));
	// AbilitySystemComp->SetIsReplicated(true);
	// AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	// StatusAttributeSet = CreateDefaultSubobject<UDBStatusAttributeSet>(TEXT("StatusAttributeSet"));
	// CombatSet = CreateDefaultSubobject<UDBCombatSet>(TEXT("CombatSet"));
}

UAbilitySystemComponent* ADBEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}
