#pragma once
#pragma once
#include "CoreMinimal.h"
#include "NPShootingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootingDelegate);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class NETWORKINGPROJECT_API UNPShootingComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UNPShootingComponent();

	UFUNCTION(BlueprintCallable)
	const float Fire();
	UFUNCTION(BlueprintCallable)
	bool CanStartCharging() const;
	UFUNCTION(BlueprintCallable)
	void StartCharging();
	UFUNCTION(BlueprintCallable)
	void StopCharging();
	UFUNCTION(BlueprintCallable)
	void Disable(float Time);
	
	UPROPERTY(BlueprintAssignable)
	FShootingDelegate OnStartCharging;
	UPROPERTY(BlueprintAssignable)
	FShootingDelegate OnStopCharging;
	UPROPERTY(BlueprintAssignable)
	FShootingDelegate OnFire;
	UPROPERTY(BlueprintAssignable)
	FShootingDelegate OnChargingUpdated;
	UPROPERTY(BlueprintAssignable)
	FShootingDelegate OnDisabled;
	UPROPERTY(BlueprintAssignable)
	FShootingDelegate OnActivated;


protected:
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void BeginPlay() override;

private:


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ChargeCD;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ChargeUpdateFrequency;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ChargeSpeedMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float BaseChargeAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ChargeTime;

	UFUNCTION()
	void DisableEnd();
	UFUNCTION()
	bool IsTimerActive() const;
	UFUNCTION()
	void ClearTimer();
	UFUNCTION()
	void SetTimer();
	UFUNCTION()
	void SetDisableTimer(float time);
	UFUNCTION()
	void ClearDisableTimer();
	UFUNCTION()
	void UpdateCharging();

	bool bDisabled;
	float ChargeAccu;
	FTimerHandle ChargingTimerHandle;
	FTimerHandle DisableTimerHandle;
};