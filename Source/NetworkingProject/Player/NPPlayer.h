#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPPlayer.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UNPMovementComponent;

UCLASS()
class NETWORKINGPROJECT_API ANP_Player : public APawn
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//update movement
	void Tick(float DeltaSeconds) override;

	//send to clients

	//Send to server

	UFUNCTION(BlueprintPure)
	int32 GetPing() const;


protected:
	void BeginPlay() override;

	/*Handle inputs*/


};