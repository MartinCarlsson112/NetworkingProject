#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Projectile/NPAmmoData.h"
#include "PickupInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

class NETWORKINGPROJECT_API IPickupInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Pickup)
	bool CanPickup(const FNPAmmoPickupData& AmmoData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Pickup)
	void ReceivePickup(const FNPAmmoPickupData& AmmoData);
};
