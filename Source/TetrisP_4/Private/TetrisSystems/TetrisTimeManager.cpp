// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisSystems/TetrisTimeManager.h"
#include "Tetris_Core.h"

// Sets default values for this component's properties
UTetrisTimeManager::UTetrisTimeManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	TetrisCore = Cast<ATetris_Core>(GetOwner());
}

// Called when the game starts
void UTetrisTimeManager::BeginPlay()
{
	Super::BeginPlay();
	if (Delays.Num() <= 0)
	{
		UE_LOG(Tetris_Time_Log, Error, TEXT("NEED TO ADD DELAY LEVELS IN FTetrisTimerManager!"));
	}
	if (SuperDownDelays.Num() <= 0)
	{
		UE_LOG(Tetris_Time_Log, Error, TEXT("NEED TO ADD SUPER DOWN DELAY LEVELS IN FTetrisTimerManager!"));
	}
	if (!TetrisCore)
	{
		UE_LOG(Tetris_Time_Log, Error, TEXT("Failed cast in Tetristime Constructor (TetrisCore* == nullptr)"));
	}
}

void UTetrisTimeManager::TTM_StartGame()
{
	UE_LOG(Tetris_Time_Log, Display, TEXT("TetrisTime_Manager::StartGame() : Start"));
	DelayIndex = 0;
	SuperDownDelayIndex = 0;
	TickCount = StartSpeedIndex;
	TetrisCore->GetWorld()->GetTimerManager().SetTimer(FigureFall_Base, this, &UTetrisTimeManager::FallingFigure_Tick,
													   Delays[DelayIndex], true);
	UE_LOG(Tetris_Time_Log, Display, TEXT("TetrisTime_Manager::StartGame() : Success"));
}

void UTetrisTimeManager::TTM_EndGame()
{
	UE_LOG(Tetris_Time_Log, Display, TEXT("TetrisTime_Manager::End() : Start"));
	TetrisCore->GetWorld()->GetTimerManager().ClearTimer(FigureFall_Base);
	TetrisCore->GetWorld()->GetTimerManager().ClearTimer(FigureFall_Extra);
	UE_LOG(Tetris_Time_Log, Display, TEXT("TetrisTime_Manager::End() : Success"));
}

void UTetrisTimeManager::PauseGame()
{
	GetWorld()->GetTimerManager().PauseTimer(FigureFall_Base);
	GetWorld()->GetTimerManager().PauseTimer(FigureFall_Extra);
}

void UTetrisTimeManager::UnPauseGame()
{
	GetWorld()->GetTimerManager().UnPauseTimer(FigureFall_Base);
	GetWorld()->GetTimerManager().UnPauseTimer(FigureFall_Extra);
}

void UTetrisTimeManager::FallingFigure_Tick()
{
	TetrisCore->UpdateFallingFigure();
}

void UTetrisTimeManager::UpdateTimeSpeedLevel()
{
	UE_LOG(Tetris_Time_Log, Display, TEXT("Start Update Time"));

	++DelayIndex;
	++SuperDownDelayIndex;
	if (SuperDownDelayIndex >= SuperDownDelays.Num())
	{
		SuperDownDelayIndex = SuperDownDelays.Num() - 1;
		UE_LOG(Tetris_Time_Log, Display, TEXT("Super Down Time Speed Not Updated (No More Updates)"));
	}
	if (DelayIndex >= Delays.Num())
	{
		UE_LOG(Tetris_Time_Log, Display, TEXT("Time Speed Not Updated (No More Updates)"));
		return;
	}

	// UI Data Update
	TetrisCore->PublicUIData.SpeedLevel = DelayIndex;
	//

	GetWorld()->GetTimerManager().ClearTimer(FigureFall_Base);
	GetWorld()->GetTimerManager().SetTimer(FigureFall_Base, this, &UTetrisTimeManager::FallingFigure_Tick,
										   Delays[DelayIndex], true);
	UE_LOG(Tetris_Time_Log, Display, TEXT("Time Speed Updated"));
}

void UTetrisTimeManager::StartSuperDown()
{
	GetWorld()->GetTimerManager().PauseTimer(FigureFall_Base);
	GetWorld()->GetTimerManager().SetTimer(FigureFall_Extra, this, &UTetrisTimeManager::FallingFigure_Tick,
										   SuperDownDelays[SuperDownDelayIndex], true, 0);
}

void UTetrisTimeManager::EndSuperDown()
{
	GetWorld()->GetTimerManager().ClearTimer(FigureFall_Extra);
	GetWorld()->GetTimerManager().UnPauseTimer(FigureFall_Base);
}

void UTetrisTimeManager::FigureStop_CheckForSpeedUp()
{
	++TickCount;
	UE_LOG(Tetris_Time_Log, Display, TEXT("Tick Count : %i"), TickCount);
	if (TickCount >= TickCountToUpdate)
	{
		TickCount = 0;
		UpdateTimeSpeedLevel();
	}
}

//