

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Debug/NPNetDebugWidget.h"
#include "NPPlayerController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class NETWORKINGPROJECT_API ANPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION()	
	void JumpPressed();

	UFUNCTION()	
	void JumpReleased();

	UFUNCTION()	
	void FireButtonPressed();

	UFUNCTION()
	void FireButtonReleased();

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void Turn(float value);

	UFUNCTION()
	void Look(float value);
	
	UFUNCTION(BlueprintPure)
	int32 GetAmmoCount() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentHealth() const;

	UFUNCTION(BlueprintPure)
	float GetChargeTime() const;

	void ShowDebugMenu();

	void HideDebugMenu();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Debug)
	TSubclassOf<UNPNetDebugWidget> DebugMenuClass;

protected:
	void CreateDebugWidget();

	UPROPERTY(Transient)
	UNPNetDebugWidget* DebugMenuInstance = nullptr;

	void Handle_DebugMenuPressed();


	void SetupInputComponent() override;


	void BeginPlay() override;

};
