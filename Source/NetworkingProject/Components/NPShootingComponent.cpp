#include "NPShootingComponent.h"

UNPShootingComponent::UNPShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	ChargeUpdateFrequency = 0.01f;
	ChargeCD = 0.2f;
	ChargeTime = 1.0f;
	BaseChargeAmount = 0.3f;
	ChargeSpeedMultiplier = 1.0f;
}


const float UNPShootingComponent::Fire()
{
	auto ChargeResult = ChargeAccu;
	if (IsTimerActive())
	{
		ClearTimer();
		ChargeAccu = 0;
		OnFire.Broadcast();
		Disable(ChargeCD);
	}		
	float ChargeAmount = ChargeResult / ChargeTime;
	return ChargeAmount + BaseChargeAmount;
}

bool UNPShootingComponent::CanStartCharging() const
{
	return !IsTimerActive() && !bDisabled;
}

void UNPShootingComponent::StartCharging()
{
	if (CanStartCharging())
	{
		OnStartCharging.Broadcast();
		SetTimer();
	}
}

void UNPShootingComponent::StopCharging()
{
	if (IsTimerActive())
	{
		ClearTimer();
		OnStopCharging.Broadcast();
		ChargeAccu = 0;
		Disable(ChargeCD);
	}
}

void UNPShootingComponent::Disable(float Time)
{
	bDisabled = true;
	if (IsTimerActive())
	{
		StopCharging();
	}
	SetDisableTimer(Time);
}


void UNPShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	bDisabled = false;
	ChargeAccu = 0;
}

void UNPShootingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (IsTimerActive())
	{
		ClearTimer();
	}
}

bool UNPShootingComponent::IsTimerActive() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(ChargingTimerHandle);
}

void UNPShootingComponent::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandle);
}

void UNPShootingComponent::SetTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ChargingTimerHandle, this, &UNPShootingComponent::UpdateCharging, ChargeUpdateFrequency, true);
}

void UNPShootingComponent::SetDisableTimer(float Time)
{
	ClearDisableTimer();
	bDisabled = true;
	GetWorld()->GetTimerManager().SetTimer(DisableTimerHandle, this, &UNPShootingComponent::DisableEnd, Time, false);
}

void UNPShootingComponent::ClearDisableTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(DisableTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DisableTimerHandle);
		DisableEnd();
	}
}


void UNPShootingComponent::DisableEnd()
{
	bDisabled = false;
	OnActivated.Broadcast();
}

void UNPShootingComponent::UpdateCharging()
{
	ChargeAccu += GetWorld()->GetDeltaSeconds() * ChargeSpeedMultiplier;
	ChargeAccu = FMath::Clamp<float>(ChargeAccu, 0, ChargeTime);
	OnChargingUpdated.Broadcast();
}
