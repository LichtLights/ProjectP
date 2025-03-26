// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GlobalPostProcessManager.generated.h"

class UPostProcessComponent;

UENUM(BlueprintType)
enum class EOutlineColor : uint8
{
	White = 0,
	Green = 1,
	Red = 2
};

UCLASS()
class PROJECTP_API AGlobalPostProcessManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGlobalPostProcessManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Post Process")
	UPostProcessComponent* PostProcessComp;

	UFUNCTION(BlueprintCallable, Category = "Post Process")
	void SetPostProcessSettings(const FPostProcessSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "Post Process")
	FPostProcessSettings GetPostProcessSettings() const;
};
