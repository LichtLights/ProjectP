

#include "AbilitySystem/DBGameplayEffectContext.h"

#include "AbilitySystem/DBAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBGameplayEffectContext)

FDBGameplayEffectContext* FDBGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FDBGameplayEffectContext::StaticStruct()))
	{
		return (FDBGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FDBGameplayEffectContext::SetAbilitySource(const IDBAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const IDBAbilitySourceInterface* FDBGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IDBAbilitySourceInterface>(AbilitySourceObject.Get());
}

bool FDBGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	// return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

const UPhysicalMaterial* FDBGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}
