// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/IInteractableTarget.h"
#include "Core/Actors/Parents/A_ItemActor.h"
#include "GameFramework/Actor.h"
#include "DBItemActor.generated.h"

UCLASS()
class PROJECTP_API ADBItemActor : public AA_ItemActor, public IInteractableTarget
{
	GENERATED_BODY()

public:

	ADBItemActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	//~IInteractableTarget interface
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) override;
	virtual bool IsInteractionReady() override;

	// virtual void ReadyInteraction(AActor* PlayerCharacter) override;
	// virtual void TriggerInteraction(AActor* PlayerCharacter) override;
	// virtual void EndInteraction(AActor* PlayerCharacter) override;
	//~End of IInteractableTarget interface
	
	bool bCanBeInteracted = false;
	UFUNCTION()
	void UpdateCanBeInteracted();

	// Initialize function
	UFUNCTION(BlueprintCallable)
	void InitializeDBItemActor();

	
	// Initial components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* BoxComp;
	FRotator InitialBoxRotation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPaperSpriteComponent* ItemSprite;
	
	// VFX
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDA_ItemActorVFX* DA_ItemActorVFX;




	
	// Launch and spin effect
	UFUNCTION(BlueprintCallable)
	void LaunchAndSpin(bool bIsBurst);
	
	void ApplySpin() const;
	void StopSpinAndReset();
	
	bool bIsSpinning = false;
	
	// Timer handle for spinning effect
	FTimerHandle SpinTimerHandle;
	FTimerHandle StopPhysicsTimerHandle;

	UFUNCTION()
	void OnBoxComponentHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
	
};
