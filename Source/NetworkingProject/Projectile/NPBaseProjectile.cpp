#include "NPBaseProjectile.h"

UNPBaseProjectile::UNPBaseProjectile()
{
	SetIsReplicatedByDefault(true);
	SetAbsolute(true, true);
}

void UNPBaseProjectile::Fire(const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier)
{

}

void UNPBaseProjectile::ActivateArrow()
{
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetComponentTickEnabled(true);
	SetGenerateOverlapEvents(true);
	SetHiddenInGame(false);
}

void UNPBaseProjectile::DeactivateArrow()
{
	SetHiddenInGame(true);
	SetGenerateOverlapEvents(false);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetComponentTickEnabled(false);
}
