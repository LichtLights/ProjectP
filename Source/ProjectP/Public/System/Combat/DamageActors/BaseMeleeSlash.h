// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMeleeSlash.generated.h"

class UPaperFlipbookComponent;
class UPaperZDAnimationComponent;
class UBoxComponent;

UCLASS()
class PROJECTP_API ABaseMeleeSlash : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseMeleeSlash();

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
	UPaperZDAnimationComponent* AnimationComponent;

	UFUNCTION()
	void AnimationEnded();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;
	
	UFUNCTION()
	void OnSlashOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

};
