// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralLevelGenerator.generated.h"

class AGeneratedTile;

UCLASS()
class PROJECTP_API AProceduralLevelGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProceduralLevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 GridWidth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 GridHeight;

	// Store possibilities for each grid cell
	TArray<TArray<TArray<AGeneratedTile*>>> Grid;

	// The set of all possible tiles that can be used by the generator
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Tiles")
	TArray<AGeneratedTile*> Tileset;

	// Initialize the grid and tile data
	void InitializeGrid();
	FVector2D GetCellWithLowestEntropy();
	void CollapseNextTile();
	void PropagateConstraints(FVector2D CollapsedCell, AGeneratedTile* CollapsedTile);
	bool IsValidNeighbor(AGeneratedTile* CurrentTile, AGeneratedTile* NeighborTile, FVector2D Direction);
	// Initialize the tileset (ground, walls, etc.)
	void InitializeTileset();
	
	void PlaceDoodads();
	void PlaceDoodadOnTile(AGeneratedTile* Tile);
	
	void SetPlayerStartAndExtraction();
	int32 CalculateManhattanDistance(FVector2D PointA, FVector2D PointB);
};
