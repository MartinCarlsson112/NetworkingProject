#include "NPHealthComponent.h"

bool UNPHealthComponent::ReceiveDamage(float Amount)
{
	Health -= Amount;

	Health = FMath::Clamp<float>(Health, 0, MaxHealth);

	if (Health > 0)
	{
		return false;
	}
	return true;
}

void UNPHealthComponent::ReceiveHealing(float Amount)
{
	Health += Amount;
	Health = FMath::Clamp<float>(Health, 0, MaxHealth);
}

