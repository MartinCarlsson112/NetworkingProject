#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPAmmoData.h"
#include "Components/BoxComponent.h"
#include "NPAmmoPickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAmmoPickupDelegate);


UCLASS(BlueprintType, Blueprintable, ClassGroup="NP", meta = (BlueprintSpawnableComponent))
class NETWORKINGPROJECT_API UNPAmmoPickup : public UBoxComponent
{
	GENERATED_BODY()
	
public:	
	UNPAmmoPickup();

	UFUNCTION(BlueprintCallable)
	void ActivatePickup();

	UFUNCTION(BlueprintCallable)
	void DeactivatePickup();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintAssignable)
	FAmmoPickupDelegate OnPickup;

private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ammo")
	FNPAmmoPickupData AmmoData;



public:
	void BeginPlay() override;

};
