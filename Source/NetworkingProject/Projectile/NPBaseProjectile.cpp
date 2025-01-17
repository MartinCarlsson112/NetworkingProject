#include "NPBaseProjectile.h"
#include "../Player/NPPlayer.h"
UNPBaseProjectile::UNPBaseProjectile()
{
	SetIsReplicatedByDefault(true);
	SetAbsolute(true, true);
}
void UNPBaseProjectile::ReturnArrow()
{
	Player->ReturnArrow(Index);
}

void UNPBaseProjectile::Initialize(const int ProjectileIndex, ANP_Player* PlayerRef)
{
	Index = ProjectileIndex;
	Player = PlayerRef;
}

void UNPBaseProjectile::Fire(const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier)
{

}

void UNPBaseProjectile::ApplyCorrection(const FVector& Direction)
{

}
void UNPBaseProjectile::Expire()
{
	DeactivateProjectile();
	ReturnArrow();
}
void UNPBaseProjectile::ActivateProjectile()
{
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetComponentTickEnabled(true);
	SetGenerateOverlapEvents(true);
	SetHiddenInGame(false);
	SetActiveFlag(true);
}

void UNPBaseProjectile::DeactivateProjectile()
{
	SetHiddenInGame(true);
	SetGenerateOverlapEvents(false);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetComponentTickEnabled(false);
	SetActiveFlag(false);
}
