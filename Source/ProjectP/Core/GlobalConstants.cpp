#include "GlobalConstants.h"

namespace GlobalConstants
{
	// Related to Level gameplay
	const FRotator SpriteRotatorConstantRight = FRotator(0.0f, 0.0f, -20.0f);
	const FRotator SpriteRotatorConstantLeft = FRotator(0.0f, 0.0f, 20.0f);
	const FRotator CameraRotatorConstant = FRotator(-60, -90, 0);
	
	const FRotator SlashRotatorConstant = FRotator(0, 0, -90);

	const FRotator TileMapRotatorConstant = FRotator(0, 0, -90.0);

	// Related to Inventory
	const float InventoryGridCellSize = 64.0f;

	// Related to Widgets
	const int32 ZOrder_Hover = 2;
}