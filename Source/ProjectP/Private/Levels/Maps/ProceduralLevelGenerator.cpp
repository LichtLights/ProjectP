// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/Maps/ProceduralLevelGenerator.h"

#include "Levels/Maps/Tiles/GeneratedTile.h"


// Sets default values
AProceduralLevelGenerator::AProceduralLevelGenerator()
{
	// Automatically call GenerateDungeon on BeginPlay
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AProceduralLevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralLevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AProceduralLevelGenerator::InitializeGrid()
{
	// Resize the grid to match the level dimensions
	Grid.SetNum(GridWidth);
	for (int32 X = 0; X < GridWidth; ++X)
	{
		Grid[X].SetNum(GridHeight);
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			// Each cell starts with all possible tiles from the tileset
			Grid[X][Y] = Tileset;
		}
	}
}

FVector2D AProceduralLevelGenerator::GetCellWithLowestEntropy()
{
	// Start with maximum entropy
	int32 LowestEntropy = Tileset.Num(); // Maximum entropy is the number of tiles in the tileset
	FVector2D CellWithLowestEntropy;

	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			int32 CurrentEntropy = Grid[X][Y].Num(); // Get the number of possible tiles in this cell

			// Ignore cells that are already collapsed (only 1 tile left)
			if (CurrentEntropy < LowestEntropy && CurrentEntropy > 1)
			{
				LowestEntropy = CurrentEntropy;
				CellWithLowestEntropy = FVector2D(X, Y);
			}
		}
	}

	return CellWithLowestEntropy;
}

void AProceduralLevelGenerator::CollapseNextTile()
{
	FVector2D Cell = GetCellWithLowestEntropy();

	// Randomly choose a tile from the possible options in the selected cell
	TArray<AGeneratedTile*>& PossibleTiles = Grid[Cell.X][Cell.Y];
	AGeneratedTile* SelectedTile = PossibleTiles[FMath::RandRange(0, PossibleTiles.Num() - 1)];

	// Collapse the cell by setting it to only the selected tile
	Grid[Cell.X][Cell.Y] = { SelectedTile };

	// Propagate constraints to neighbors
	PropagateConstraints(Cell, SelectedTile);
}

void AProceduralLevelGenerator::PropagateConstraints(FVector2D CollapsedCell, AGeneratedTile* CollapsedTile)
{
	// Define the neighbor offsets (Right, Left, Top, Bottom)
	FVector2D NeighborOffsets[4] = { FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1) };

	for (FVector2D Offset : NeighborOffsets)
	{
		FVector2D NeighborCell = CollapsedCell + Offset;

		// Check if the neighbor is within grid bounds
		if (NeighborCell.X >= 0 && NeighborCell.X < GridWidth && NeighborCell.Y >= 0 && NeighborCell.Y < GridHeight)
		{
			TArray<AGeneratedTile*>& NeighborTiles = Grid[NeighborCell.X][NeighborCell.Y];

			// // Remove incompatible tiles from the neighbor's possible options
			// NeighborTiles.RemoveAll([=](AGeneratedTile* NeighborTile)
			// {
			// 	return !IsValidNeighbor(CollapsedTile, NeighborTile, Offset);
			// });

			// If the neighbor now has only one option left, collapse it
			if (NeighborTiles.Num() == 1)
			{
				CollapseNextTile(); // Recursively collapse the neighbor
			}
		}
	}
}

bool AProceduralLevelGenerator::IsValidNeighbor(AGeneratedTile* CurrentTile, AGeneratedTile* NeighborTile, FVector2D Direction)
{
	// Compare the appropriate sides based on the direction of the neighbor
	if (Direction == FVector2D(1, 0))  // Right neighbor
	{
		return CurrentTile->TileConnections.RightConnection == NeighborTile->TileConnections.LeftConnection;
	}
	else if (Direction == FVector2D(-1, 0))  // Left neighbor
	{
		return CurrentTile->TileConnections.LeftConnection == NeighborTile->TileConnections.RightConnection;
	}
	else if (Direction == FVector2D(0, 1))  // Top neighbor
	{
		return CurrentTile->TileConnections.TopConnection == NeighborTile->TileConnections.BottomConnection;
	}
	else if (Direction == FVector2D(0, -1))  // Bottom neighbor
	{
		return CurrentTile->TileConnections.BottomConnection == NeighborTile->TileConnections.TopConnection;
	}

	return false;
}

void AProceduralLevelGenerator::InitializeTileset()
{
	// Example 1: Ground tile that can connect on all sides (Open connections)
	AGeneratedTile* FloorTile = NewObject<AGeneratedTile>();
	FloorTile->TileConnections = {
		ETileConnectionInfo::Open,
		ETileConnectionInfo::Open,
		ETileConnectionInfo::Open,
		ETileConnectionInfo::Open
	}; // All sides are "Open"
	FloorTile->TileType = ETileType::Floor;
	// GroundTile->TileSprite = GroundTileSprite; // Optionally assign a Paper2D sprite for visuals
	Tileset.Add(FloorTile); // Add it to the Tileset array

	// Example 2: Wall tile with closed connections (impassable)
	AGeneratedTile* WallTile = NewObject<AGeneratedTile>();
	WallTile->TileConnections = {
		ETileConnectionInfo::Closed,
		ETileConnectionInfo::Closed,
		ETileConnectionInfo::Closed,
		ETileConnectionInfo::Closed
	}; // All sides are "Closed"
	WallTile->TileType = ETileType::BlockedWithDooDads;
	// WallTile->TileSprite = WallTileSprite; // Assign a sprite if using Paper2D
	Tileset.Add(WallTile); // Add it to the Tileset array

	// Add more tiles as needed
}

void AProceduralLevelGenerator::PlaceDoodads()
{
	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			AGeneratedTile* Tile = Grid[X][Y][0];  // Get collapsed tile
            
			if (Tile->IsEligibleForDoodad())
			{
				PlaceDoodadOnTile(Tile);
			}
		}
	}
}

void AProceduralLevelGenerator::PlaceDoodadOnTile(AGeneratedTile* Tile)
{
	// if (Tile->TileType == "LootRoom")
	// {
	// 	Spawn a loot box
	// 	SpawnDoodad(LootBoxClass, Tile->GetActorLocation());
	// }
	// else if (Tile->TileType == "Doorway")
	// {
	// 	Spawn a door
	// 	SpawnDoodad(DoorClass, Tile->GetActorLocation());
	// }
	
	// Add more doodads as needed...
}

void AProceduralLevelGenerator::SetPlayerStartAndExtraction()
{
	// FVector2D StartPoint = SelectRandomStartPoint();
	// FVector2D ExtractionPoint = SelectRandomExtractionPoint();
	//
	// // Ensure there is a minimum distance between start and extraction
	// while (CalculateManhattanDistance(StartPoint, ExtractionPoint) < MinimumDistance)
	// {
	// 	ExtractionPoint = SelectRandomExtractionPoint();
	// }
	//
	// PlacePlayerStart(StartPoint);
	// PlaceExtractionPoint(ExtractionPoint);
}

int32 AProceduralLevelGenerator::CalculateManhattanDistance(FVector2D PointA, FVector2D PointB)
{
	return FMath::Abs(PointA.X - PointB.X) + FMath::Abs(PointA.Y - PointB.Y);
}
