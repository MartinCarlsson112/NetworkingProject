#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../NPMovementData.h"
#include "Net/UnrealNetwork.h"
#include "../Components/NPHealthComponent.h"
#include "../Components/NPShootingComponent.h"
#include "../Components/NPAmmoComponent.h"
#include "../Interfaces/DamageableInterface.h"
#include "NPPlayer.generated.h"

class UNPMovementComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API ANP_Player : public APawn, public IDamageableInterface
{
	GENERATED_BODY()

public:
	ANP_Player();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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


	void OnJumpPressed();
	void OnJumpReleased();
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void Look(float value);

	void StartCharging();
	void Shoot();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ReceiveDamage_Implementation(float Damage, ANP_Player* Instigator) override;
	bool CanDamage_Implementation() const override;

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