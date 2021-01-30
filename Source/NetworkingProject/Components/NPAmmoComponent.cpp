#include "NPAmmoComponent.h"
#include "Net/UnrealNetwork.h"

UNPAmmoComponent::UNPAmmoComponent()
{
	Ammo.SetNum((int32)ENPAmmoTypes::COUNT);
}

int32 UNPAmmoComponent::GetAmmoCount(ENPAmmoTypes Type) const
{
	return Ammo[(int32)Type].Count;
}

bool UNPAmmoComponent::CanGainAmmo(ENPAmmoTypes Type) const
{
	return Ammo[(int32)Type].Count < Ammo[(int32)Type].MaxCount;
}

bool UNPAmmoComponent::UseAmmo(ENPAmmoTypes Type, int32 Count /*= 1*/)
{
	if (Count > 0)
	{
		if (Ammo[(int32)Type].Count - Count >= 0)
		{
			Ammo[(int32)Type].Count -= Count;
			return true;
		}
	}
	return false;
}

bool UNPAmmoComponent::GainAmmo(ENPAmmoTypes Type, int32 Count /*= 1*/)
{
	if (Count > 0)
	{
		if (CanGainAmmo(Type))
		{
			Ammo[(int32)Type].Count = FMath::Clamp<int>(Ammo[(int32)Type].Count + Count, 0, Ammo[(int32)Type].MaxCount);
			return true;
		}
	}
	return false;
}

void UNPAmmoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UNPAmmoComponent, Ammo);
}
