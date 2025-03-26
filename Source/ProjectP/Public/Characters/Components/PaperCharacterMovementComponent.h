// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperCharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTP_API UPaperCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPaperCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;



	
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	/*	Sets the sprite rotation facing camera. Should be called when sprite rotation is changed.
	 */
	UFUNCTION(BlueprintCallable)
	void CompensateSpriteRotation(const FVector& MoveVelocity) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RecentAimLocation = FVector::ZeroVector;
	UFUNCTION(BlueprintCallable)
	void SetRecentAimLocation(const FVector AimLocation) { this->RecentAimLocation = AimLocation; }
	UFUNCTION(BlueprintCallable)
	void ClearRecentAimLocation() { this->RecentAimLocation = FVector::ZeroVector; }

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAiming = false;
	UFUNCTION(BlueprintCallable)
	void SetIsAiming(const bool IsAiming) { this->bIsAiming = IsAiming; }
};
