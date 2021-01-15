#pragma once
#include "CoreMinimal.h"
#include "../Projectile/NPBaseProjectile.h"
#include "NPAmmoComponent.generated.h"



USTRUCT(Blueprintable, BlueprintType)
struct FAmmoContainer
{
	GENERATED_BODY()

	uint32 Count;
	uint32 MaxCount;
};

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API UNPAmmoComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	void AddAmmoContainer(TSubclassOf<UNPBaseProjectile> Type, uint32 MaxAmmo, uint32 CurrentAmmo);

	bool HasAmmo(TSubclassOf<UNPBaseProjectile> Type, uint32 Count = 1) const;

	uint32 GetAmmoCount(TSubclassOf<UNPBaseProjectile> Type) const;

	void UseAmmo(TSubclassOf<UNPBaseProjectile> Type, uint32 Count = 1);

	void GainAmmo(TSubclassOf<UNPBaseProjectile> Type, uint32 Count = 1);

private:
	TMap<TSubclassOf<UNPBaseProjectile>, FAmmoContainer> AmmoMap;
};