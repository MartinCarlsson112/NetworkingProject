#include "NPBaseProjectile.h"

UNPBaseProjectile::UNPBaseProjectile()
{
	SetIsReplicatedByDefault(true);

}

void UNPBaseProjectile::Fire(AActor* Shooter, const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier)
{

}
