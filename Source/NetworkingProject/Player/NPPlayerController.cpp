#include "NPPlayerController.h"
#include "NPPlayer.h"
#include "../Interfaces/PlayerControlInterface.h"

void ANPPlayerController::StartSession()
{

}

void ANPPlayerController::EndSession()
{
}

void ANPPlayerController::JumpPressed()
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_JumpPressed(GetPawn());
	}
}

void ANPPlayerController::JumpReleased()
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_JumpReleased(GetPawn());
	}
}

void ANPPlayerController::FireButtonPressed()
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_FireButtonPressed(GetPawn());
	}
}

void ANPPlayerController::FireButtonReleased()
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_FireButtonReleased(GetPawn());
	}
}

void ANPPlayerController::MoveForward(float value)
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_MoveForward(GetPawn(), value);
	}
}

void ANPPlayerController::MoveRight(float value)
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_MoveRight(GetPawn(), value);
	}
}

void ANPPlayerController::Turn(float value)
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_Turn(GetPawn(), value);
	}
}

void ANPPlayerController::Look(float value)
{
	if (GetPawn())
	{
		IPlayerControlInterface::Execute_Look(GetPawn(), value);
	}
}

void ANPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Jump", IE_Pressed, this, &ANPPlayerController::JumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &ANPPlayerController::JumpReleased);
	InputComponent->BindAxis("Forward", this, &ANPPlayerController::MoveForward);
	InputComponent->BindAxis("Right", this, &ANPPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ANPPlayerController::Turn);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ANPPlayerController::FireButtonPressed);
	InputComponent->BindAction("Shoot", IE_Released, this, &ANPPlayerController::FireButtonReleased);
	InputComponent->BindAxis("Look", this, &ANPPlayerController::Look);
}
