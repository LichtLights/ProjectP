// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectileFire.generated.h"

class UPaperZDAnimationComponent;
class UProjectileMovementComponent;
class UPaperFlipbookComponent;
class UBoxComponent;

UCLASS()
class PROJECTP_API ABaseProjectileFire : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectileFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UPaperFlipbookComponent* Sprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UPaperZDAnimationComponent* AnimationComponent;
	

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
};
