
#include "PlayerPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tetris_Core.h"
#include "Tetris_NF_Viewer.h"
#include "FigureBase.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	MenuSettings = CreateDefaultSubobject<UMenuSettings_System>("Menu Setting System");
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	if (Tetris == nullptr)
	{
	}

}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &APlayerPawn::MoveFigureRight);
	PlayerInputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &APlayerPawn::MoveFigureLeft);
	PlayerInputComponent->BindAction("MoveRight", EInputEvent::IE_Released, this, &APlayerPawn::MoveFigureRight_Released);
	PlayerInputComponent->BindAction("MoveLeft", EInputEvent::IE_Released, this, &APlayerPawn::MoveFigureLeft_Released);

	PlayerInputComponent->BindAction("RotateFigure", EInputEvent::IE_Pressed, this, &APlayerPawn::RotateFigure);
	PlayerInputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &APlayerPawn::MoveDownButtonPressed);
	PlayerInputComponent->BindAction("MoveDown", EInputEvent::IE_Released, this, &APlayerPawn::MoveDownButtonReleased);
	PlayerInputComponent->BindAction("SuperDown", EInputEvent::IE_Pressed, this, &APlayerPawn::MoveSuperDown);

	PlayerInputComponent->BindAction("TEST_RESTART", EInputEvent::IE_Pressed, this, &APlayerPawn::TEST_RESTART);
	PlayerInputComponent->BindAction("PauseGame", EInputEvent::IE_Pressed, this, &APlayerPawn::GamePauseButtonPressed);


}

UMenuSettings_System* APlayerPawn::GetMenuSettings()
{
	return MenuSettings;
}

void APlayerPawn::MoveFigureRight()
{
	Tetris->MoveFigureRight();
	if (!MoveRightLongPress_TH.IsValid())
		GetWorld()->GetTimerManager().SetTimer(MoveRightLongPress_TH, this, &APlayerPawn::MoveFigureRight, 0.2, true);
}

void APlayerPawn::MoveFigureLeft()
{
	Tetris->MoveFigureLeft();
	if (!MoveLeftLongPress_TH.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(MoveLeftLongPress_TH, this, &APlayerPawn::MoveFigureLeft, 0.2, true);
	}
}

void APlayerPawn::MoveFigureRight_Released()
{
	GetWorld()->GetTimerManager().ClearTimer(MoveRightLongPress_TH);
}

void APlayerPawn::MoveFigureLeft_Released()
{
	GetWorld()->GetTimerManager().ClearTimer(MoveLeftLongPress_TH);
}

void APlayerPawn::RotateFigure()
{
	Tetris->RotateFigure();
}

void APlayerPawn::MoveSuperDown()
{
	Tetris->MoveSuperDown();
}

void APlayerPawn::MoveDownButtonPressed()
{
	IsReleased = false;
	Tetris->MoveDownStart();
}

void APlayerPawn::MoveDownButtonReleased()
{
	IsReleased = true;
	Tetris->MoveDownStop();
}

void APlayerPawn::TEST_RESTART()
{
	Tetris->RestartGame();
}

void APlayerPawn::GamePauseButtonPressed()
{
	Tetris->Pause_UnPause_Game();
}

void APlayerPawn::OnGameUnPaused_Implementation()
{
}

void APlayerPawn::OnGamePaused_Implementation()
{
}

void APlayerPawn::OnTetrisScoreUpdated_Implementation()
{

}


//Input Functions