// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DBCharacter.h"
#include "System/GameModes/DBExperienceDefinition.h"
#include "DBPlayerCharacter.generated.h"

class UDBWeaponMasteryComponent;
class UDBHeroComponent;
class UDBCameraComponent;
class UInventoryComponent;
class UPlayerAbilitySystemComponent;
class USpringArmComponent;
class UDBAbilitySystemComponent;
struct FInputActionValue;
class UDBStatusAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionInterrupted);

UCLASS()
class PROJECTP_API ADBPlayerCharacter : public ADBCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	UPROPERTY()
	TObjectPtr<const UDBPawnData> PawnData;
	void SetPawnData(const UDBPawnData* InPawnData);
	void OnExperienceLoaded(const UDBExperienceDefinition* UDBExperienceDefinition);

	
/*	Action functions.
 *	All booleans related to actions are declared here.
 */
public:

	// Delegate that broadcast when hit.
	FOnActionInterrupted OnActionInterrupted;
	
	UFUNCTION()
	void Interact();

	UFUNCTION()
	FVector GetAimAtMouse();
	FVector RecentAimLocation = FVector::ZeroVector;
	
	UFUNCTION()
	virtual void PrimaryAttack();

	// TODO: TImerHandle should be replaced with a FZDOnAnimationOverrideEndSignature.
	FTimerHandle PrimaryAttackTimerHandle;
	void AttackAnimTimerExpired();

	
/*	Animation settings.
 */
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data Assets")
	class UDA_OverridingAnims* DA_OverridingAnims;


/*	Interaction settings. TODO: DEPRECATED
 */
public:
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float InteractionRange = 200.f;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	class USphereComponent* InteractionSphere;

	// WBP of InteractionWidget should be assigned.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	class UWidgetComponent* InteractionWidgetComponent;

public:
	
	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentInteractingInteractable() const { return CurrentInteractingInteractable; }

	UFUNCTION(BlueprintCallable)
	void SwitchCurrentInteractable();
	
private:
	
	UPROPERTY()
	TArray<AActor*> InteractablesCanInteractWith;
	UPROPERTY()
	int32 CurrentInteractableIndex = INDEX_NONE;
	
	UPROPERTY()
	AActor* CurrentInteractingInteractable = nullptr;
	
	void UpdateInteractionWidget(AActor* Interactable);

	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDBCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerAbilitySystemComponent> AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	const UDBStatusAttributeSet* StatusAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	UDBHeroComponent* HeroComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDBWeaponMasteryComponent> WeaponMasteryComp;

	
/*	Game system functions.
 */
public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UDBWeaponMasteryComponent* GetWeaponMasteryComponent() const { return WeaponMasteryComp; }
};
