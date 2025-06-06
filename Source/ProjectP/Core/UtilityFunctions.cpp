#include "UtilityFunctions.h"

#include "Characters/PlayerCharacters/DBPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// UInventoryComponent* GetPlayerInventoryComponent(const UWorld* World)
// {
// 	ADBPlayerCharacter* PlayerCharacter = Cast<ADBPlayerCharacter>(
// 		UGameplayStatics::GetPlayerCharacter(World, 0));
// 	if (!PlayerCharacter)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null at GetPlayerInventoryComponent()"));
// 		return nullptr;
// 	}
//
// 	UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
// 	if (!InventoryComponent)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("InventoryComponent is null at GetPlayerInventoryComponent()"));
// 		return nullptr;
// 	}
//
// 	return InventoryComponent;
// }
