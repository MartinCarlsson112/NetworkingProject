#pragma once
#include "CoreMinimal.h"
#include "../Projectile/NPBaseProjectile.h"
#include "../Projectile/NPAmmoData.h"
#include "NPAmmoComponent.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FAmmoContainer
{
	GENERATED_BODY()

	FAmmoContainer()
	{
		Count = 0;
		MaxCount = 0;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Count;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxCount;
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class NETWORKINGPROJECT_API UNPAmmoComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UNPAmmoComponent();

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoCount(ENPAmmoTypes Type) const;
	UFUNCTION(BlueprintCallable)
	bool CanGainAmmo(ENPAmmoTypes Type) const;
	UFUNCTION(BlueprintCallable)
	bool UseAmmo(ENPAmmoTypes Type, int32 Count = 1);
	UFUNCTION(BlueprintCallable)
	bool GainAmmo(ENPAmmoTypes Type, int32 Count = 1);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, meta = (AllowPrivateAccess = "true"))
	TArray<FAmmoContainer> Ammo;
};