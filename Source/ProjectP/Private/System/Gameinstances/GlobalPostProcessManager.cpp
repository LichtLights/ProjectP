// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Gameinstances/GlobalPostProcessManager.h"

#include "Components/PostProcessComponent.h"
#include "GameFramework/GameUserSettings.h"


// Sets default values
AGlobalPostProcessManager::AGlobalPostProcessManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the Post Process Component
	PostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("GlobalPostProcessComponent"));
	PostProcessComp->SetupAttachment(RootComponent);
	PostProcessComp->bUnbound = true;  // Set the component to be unbound
	PostProcessComp->Priority = 100;
	
}

// Called when the game starts or when spawned
void AGlobalPostProcessManager::BeginPlay()
{
	Super::BeginPlay();

	// Optionally, initialize or adjust the settings here
	if (PostProcessComp)
	{
		// TODO: Get FPostProcessSettings that saved somewhere and call SetPostProcessSettings(). GPT says: UGameUserSettings(This holds postprocess quality)
		// This setting is for game config, not player settings.
		// SetPostProcessSettings(setting);
		
		// Load and apply the post-process material for outlining
		UMaterialInterface* OutlineMaterialWhite = LoadObject<UMaterialInterface>(
			nullptr,
			TEXT("/Game/PathToYourMaterial/M_Outline.M_Outline")
		);
		if (OutlineMaterialWhite)
		{
			PostProcessComp->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.0f, OutlineMaterialWhite));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutlineMaterialWhite is null at AGlobalPostProcessManager::BeginPlay()"));
		}
		
		// Load and apply the post-process material for outlining
		UMaterialInterface* OutlineMaterialRed = LoadObject<UMaterialInterface>(
			nullptr,
			TEXT("/Game/PathToYourMaterial/M_Outline.M_Outline")
		);
		if (OutlineMaterialRed)
		{
			PostProcessComp->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.0f, OutlineMaterialRed));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutlineMaterialRed is null at AGlobalPostProcessManager::BeginPlay()"));
		}
	}
}

// Called every frame
void AGlobalPostProcessManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGlobalPostProcessManager::SetPostProcessSettings(const FPostProcessSettings& Settings)
{
	if (PostProcessComp)
	{
		PostProcessComp->Settings = Settings;
	}
}

FPostProcessSettings AGlobalPostProcessManager::GetPostProcessSettings() const
{
	if (PostProcessComp)
	{
		return PostProcessComp->Settings;
	}

	return FPostProcessSettings();
}
