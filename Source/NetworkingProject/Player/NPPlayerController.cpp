#include "NPPlayerController.h"
#include "NPPlayer.h"
#include "../Interfaces/PlayerControlInterface.h"

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

int32 ANPPlayerController::GetAmmoCount() const
{
	auto TempPawn = GetPawn<ANP_Player>();
	if (TempPawn)
	{
		return TempPawn->GetAmmoCount();
	}
	return 0;
}

int32 ANPPlayerController::GetCurrentHealth() const
{
	return 2;
}

float ANPPlayerController::GetChargeTime() const
{
	return 3;
}

void ANPPlayerController::ShowDebugMenu()
{
	CreateDebugWidget();

	if (DebugMenuInstance == nullptr)
	{
		return;
	}
	DebugMenuInstance->SetVisibility(ESlateVisibility::Visible);
	DebugMenuInstance->BP_OnShowWdiget();
}

void ANPPlayerController::HideDebugMenu()
{
	if (DebugMenuInstance == nullptr)
	{
		return;
	}

	DebugMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
	DebugMenuInstance->BP_OnHideWdiget();

}

void ANPPlayerController::CreateDebugWidget()
{
	if (DebugMenuClass == nullptr)
	{
		return;
	}

	if (!IsLocalController())
	{
		return;
	}

	if (DebugMenuInstance == nullptr)
	{
		DebugMenuInstance = CreateWidget<UNPNetDebugWidget>(GetWorld(), DebugMenuClass);
		DebugMenuInstance->AddToViewport();
	}

}

void ANPPlayerController::Handle_DebugMenuPressed()
{
	if (DebugMenuInstance == nullptr)
	{
		return;
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
	InputComponent->BindAction("DebugMenu", IE_Pressed, this, &ANPPlayerController::Handle_DebugMenuPressed);
}

void ANPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CreateDebugWidget();
	if (DebugMenuInstance != nullptr)
	{
		DebugMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}
