#include "NPAmmoComponent.h"

void UNPAmmoComponent::AddAmmoContainer(TSubclassOf<UNPBaseProjectile> Type, uint32 MaxAmmo, uint32 CurrentAmmo)
{
	FAmmoContainer AmmoContainer;
	AmmoContainer.Count = CurrentAmmo;
	AmmoContainer.MaxCount = MaxAmmo;
	AmmoMap.Emplace(Type, AmmoContainer);
}

bool UNPAmmoComponent::HasAmmo(TSubclassOf<UNPBaseProjectile> Type, uint32 Count /*= 1*/) const
{
	if (AmmoMap.Contains(Type))
	{
		auto AmmoContainer = AmmoMap.Find(Type);

		if (AmmoContainer->Count >= Count)
		{
			return true;
		}

	}
	return false;
}

uint32 UNPAmmoComponent::GetAmmoCount(TSubclassOf<UNPBaseProjectile> Type) const
{
	if (AmmoMap.Contains(Type))
	{
		auto AmmoContainer = AmmoMap.Find(Type);
		return AmmoContainer->Count;
	}
	return 0;
}

void UNPAmmoComponent::UseAmmo(TSubclassOf<UNPBaseProjectile> Type, uint32 Count /*= 1*/)
{
	if (AmmoMap.Contains(Type))
	{
		auto AmmoContainer = AmmoMap.Find(Type);

		if (AmmoContainer->Count >= Count)
		{
			AmmoContainer->Count -= Count;
		}
	}
}

void UNPAmmoComponent::GainAmmo(TSubclassOf<UNPBaseProjectile> Type, uint32 Count /*= 1*/)
{
	if (AmmoMap.Contains(Type))
	{
		auto AmmoContainer = AmmoMap.Find(Type);

		if (AmmoContainer->Count >= Count)
		{
			AmmoContainer->Count += Count;
		}
	}
}
