#pragma once
#include "CoreMinimal.h"
#include "NPAmmoData.generated.h"

UENUM(BlueprintType)
enum ENPAmmoTypes
{
	Arrow,
	Bomb,
	Ice,
	COUNT
};

USTRUCT(BlueprintType)
struct FNPAmmoPickupData
{
	GENERATED_BODY()

	FNPAmmoPickupData() 
	{
		AmmoType = ENPAmmoTypes::Arrow;
		Count = 1;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ammo")
	TEnumAsByte<ENPAmmoTypes> AmmoType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ammo")
	int32 Count;
	
};
