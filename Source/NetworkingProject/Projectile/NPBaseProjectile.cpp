#include "NPBaseProjectile.h"

UNPBaseProjectile::UNPBaseProjectile()
{
	SetIsReplicatedByDefault(true);
}

void UNPBaseProjectile::Fire(const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier)
{

}

void UNPBaseProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsRooted())
	{
		RemoveFromRoot();
	}
	Super::EndPlay(EndPlayReason);
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
