#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../NPMovementData.h"
#include "Net/UnrealNetwork.h"
#include "../Components/NPHealthComponent.h"
#include "../Components/NPShootingComponent.h"
#include "../Components/NPAmmoComponent.h"
#include "../Interfaces/DamageableInterface.h"
#include "../Interfaces/PlayerControlInterface.h"
#include "NPPlayer.generated.h"

class UNPMovementComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API ANP_Player : public APawn, public IDamageableInterface, public IPlayerControlInterface
{
	GENERATED_BODY()

public:
	ANP_Player();

	void Tick(float DeltaSeconds) override;

	UFUNCTION(Server, Unreliable)
	void Server_SendMove(const FVector& NewLocation);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SendLocation(const FVector& LocationToSend);

	UFUNCTION(Server, Unreliable)
	void Server_SendRotation(const FRotator& NewRotation);

	UFUNCTION(Server, Reliable)
	void Server_FireProjectile(int ProjectileToShoot, const FVector& ArrowStartPosition, const FRotator& FacingRotation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireProjectile(int ProjectileToShoot, const FVector& ArrowStartPosition, const FRotator& FacingRotation);

	UFUNCTION(Server, Reliable)
	void Server_StartCharging();

	UFUNCTION(BlueprintPure)
	int32 GetPing() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	UNPMovementComponent* MovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* FPMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Shooting)
	UNPShootingComponent* ShootingComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Shooting)
	UNPAmmoComponent* AmmoComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Collision)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
	UNPHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Shooting)
	USceneComponent* FirePosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Shooting)
	UStaticMesh* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting)
	TArray<UNPBaseProjectile*> Projectiles;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FDamageResult ReceiveDamage_Implementation(float Damage, AActor* Instigator) override;
	bool CanDamage_Implementation() const override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Player Control Interface
	void JumpPressed_Implementation() override;
	void FireButtonPressed_Implementation() override;
	void FireButtonReleased_Implementation() override;
	void MoveForward_Implementation(float value) override;
	void MoveRight_Implementation(float value) override;
	void Turn_Implementation(float value) override;
	void Look_Implementation(float value) override;
	void JumpReleased_Implementation() override;

protected:
	void BeginPlay() override;


private:
	FVector MovementInput;
	FVector TargetLocation;

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Movement)
	float MovementSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ArrowForce;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ArrowDamageMultiplier;

	const float LocationInterpolationSpeed = 5.0f;
	const float RotationInterpolationSpeed = 5.0f;
	const float MinDistance = 1.0f;


	int counter = 0;
};