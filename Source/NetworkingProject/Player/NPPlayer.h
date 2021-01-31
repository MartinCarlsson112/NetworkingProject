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
#include "../Projectile/NPArrowProjectile.h"
#include "Camera/CameraComponent.h"
#include "../Interfaces/PickupInterface.h"
#include "GameFramework/SpringArmComponent.h"

#include "NPPlayer.generated.h"

class UNPMovementComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;

UENUM(BlueprintType)
enum ENPArmState
{
	EAS_Idle,
	EAS_Draw,
	EAS_Shoot,
	EAS_Climbing,
	EAS_Dash
};

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API ANP_Player : public APawn, public IDamageableInterface, public IPickupInterface, public IPlayerControlInterface
{
	GENERATED_BODY()

public:
	ANP_Player();

	void OnConstruction(const FTransform& Transform) override;

	void Tick(float DeltaSeconds) override;

	UFUNCTION(Server, Unreliable)
	void Server_SendMove(const FVector& NewLocation);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SendLocation(const FVector& LocationToSend);

	UFUNCTION(Server, Unreliable)
	void Server_SendRotation(const FRotator& NewRotation);

	UFUNCTION(Server, Reliable)
	void Server_FireProjectile(const FVector& ArrowStartPosition, const FRotator& FacingRotation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireProjectile(int ProjectileToShoot, const FVector& ArrowStartPosition, const FRotator& FacingRotation);

	UFUNCTION(Server, Reliable)
	void Server_StartCharging();


	UFUNCTION(BlueprintCallable)
	int32 GetAmmoCount() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentCharge() const;


	UFUNCTION(BlueprintPure)
	int32 GetPing() const;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	UCameraComponent* FPSCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	USpringArmComponent* ThirdPersonSpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Movement)
	UNPMovementComponent* MovementComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Mesh)
	USkeletalMeshComponent* FPMesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Shooting)
	UNPShootingComponent* ShootingComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Shooting)
	UNPAmmoComponent* AmmoComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Collision)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Health)
	UNPHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Shooting)
	USceneComponent* FirePosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Shooting)
	UStaticMesh* ProjectileMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Shooting)
	TArray<UNPArrowProjectile*> Arrows;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = Shooting)
	TArray<int> FreeList;

	//free list

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FDamageResult ReceiveDamage_Implementation(float Damage, AActor* Instigator) override;
	bool CanDamage_Implementation() const override;

	void ReturnArrow(int Index);
	int GetArrow();

	void JumpPressed_Implementation() override;
	void FireButtonPressed_Implementation() override;
	void FireButtonReleased_Implementation() override;
	void MoveForward_Implementation(float value) override;
	void MoveRight_Implementation(float value) override;
	void Turn_Implementation(float value) override;
	void Look_Implementation(float value) override;
	void JumpReleased_Implementation() override;

	bool CanPickup_Implementation(const FNPAmmoPickupData& AmmoData) override;
	void ReceivePickup_Implementation(const FNPAmmoPickupData& AmmoData) override;


private:
	const float LocationInterpolationSpeed = 5.0f;
	const float RotationInterpolationSpeed = 5.0f;
	const float MinDistance = 1.0f;




	FVector MovementInput;
	FVector TargetLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	TEnumAsByte<ENPAmmoTypes> SelectedAmmoType;

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Movement)
	float MovementSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ArrowForce;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ArrowDamageMultiplier;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	int32 NumberOfArrowInstances;
protected:
	void BeginPlay() override;

};