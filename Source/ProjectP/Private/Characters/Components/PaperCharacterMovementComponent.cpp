// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/PaperCharacterMovementComponent.h"

#include "GlobalConstants.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDCharacter.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UPaperCharacterMovementComponent::UPaperCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPaperCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPaperCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPaperCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	CompensateSpriteRotation(OldVelocity);
}

void UPaperCharacterMovementComponent::CompensateSpriteRotation(const FVector& MoveVelocity) const
{
	// TODO: Consider aiming at.
	APaperZDCharacter* PaperZDCharacter = Cast<APaperZDCharacter>(GetCharacterOwner());

	if (!IsValid(PaperZDCharacter))
		return;

	if (bIsAiming)
	{
		if (RecentAimLocation.X < 0)
		{
			GetController()->SetControlRotation(FRotator(0, 180.f, 0));
			PaperZDCharacter->SetActorRelativeRotation(FRotator(0, 180.f, 0));
			PaperZDCharacter->GetSprite()->SetRelativeRotation(GlobalConstants::SpriteRotatorConstantLeft);
		}
		else if (RecentAimLocation.X > 0)
		{
			GetController()->SetControlRotation(FRotator(0, 0, 0));
			PaperZDCharacter->SetActorRelativeRotation(FRotator(0, 0, 0));
			PaperZDCharacter->GetSprite()->SetRelativeRotation(GlobalConstants::SpriteRotatorConstantRight);
		}
	}
	else
	{
		// Apply movement based on the MoveVector
		if (!MoveVelocity.IsZero())
		{
			FVector Direction = FVector(MoveVelocity.X, MoveVelocity.Y, 0.0f);
			Direction.Normalize();

			// Flip sprite(controller rotation) as direction changes.
			if (Direction.X < 0)
			{
				GetController()->SetControlRotation(FRotator(0, 180.f, 0));
				PaperZDCharacter->SetActorRelativeRotation(FRotator(0, 180.f, 0));
				PaperZDCharacter->GetSprite()->SetRelativeRotation(GlobalConstants::SpriteRotatorConstantLeft);
			}
			else if (Direction.X > 0)
			{
				GetController()->SetControlRotation(FRotator(0, 0, 0));
				PaperZDCharacter->SetActorRelativeRotation(FRotator(0, 0, 0));
				PaperZDCharacter->GetSprite()->SetRelativeRotation(GlobalConstants::SpriteRotatorConstantRight);
			}
		}
	}
	
}

