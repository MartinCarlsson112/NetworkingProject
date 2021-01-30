#include "NPHealthComponent.h"

UNPHealthComponent::UNPHealthComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

bool UNPHealthComponent::ReceiveDamage(int Amount)
{
	Health -= Amount;
	Health = FMath::Clamp<float>(Health, 0, MaxHealth);
	if (Health > 0)
	{
		return true;
	}
	return false;
}

void UNPHealthComponent::ReceiveHealing(int Amount)
{
	Health += Amount;
	Health = FMath::Clamp<float>(Health, 0, MaxHealth);
}

int UNPHealthComponent::GetCurrentHealth()
{
	return Health;
}

