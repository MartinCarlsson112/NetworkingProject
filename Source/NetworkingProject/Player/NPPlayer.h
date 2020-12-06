#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../NPMovementData.h"
#include "NPPlayer.generated.h"

class UNPMovementComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API ANP_Player : public APawn
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

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SendRotation(const FRotator& NewRotation);


	FTimerHandle InterpolationTimer;

	UFUNCTION(BlueprintPure)
	int32 GetPing() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	UNPMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Collision)
	UCapsuleComponent* CapsuleComponent;


	void OnJumpPressed();
	void OnJumpReleased();

	void MoveForward(float value);
	void MoveRight(float value);

	void Turn(float value);
	void Look(float value);

protected:

	FVector MovementInput;
	FVector TargetLocation;
	FRotator TargetRotation;


	const float LocationInterpolationSpeed = 5.0f;
	const float RotationInterpolationSpeed = 5.0f;
	const float MinDistance = 1.0f;
	const float MovementSpeed = 100.0f;

	void BeginPlay() override;

	/*Handle inputs*/


};