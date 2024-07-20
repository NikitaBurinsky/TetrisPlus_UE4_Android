// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris_Core.h"
#include "FigureBase.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "PlayerPawn.h"
#include "TetrisSystems/TetrisTimeManager.h"
#include "MenuSettings_System.h"
#include "TetrisSystems/Tetris_Score.h"
#include "TetrisSystems/TetrisRender_v2.h"

// DEFAULT FUNCTIONS
//
ATetris_Core::ATetris_Core()
{
	PrimaryActorTick.bCanEverTick = true;

	RootTetris = CreateDefaultSubobject<UStaticMeshComponent>(FName("Root"));
	SetRootComponent(RootTetris);

	TetrisTime = CreateDefaultSubobject<UTetrisTimeManager>(FName("TetrisTime"));

	TetrisRender = CreateDefaultSubobject<UTetrisRender_v2>(FName("TetrisRender"));

	TetrisScore = CreateDefaultSubobject<UTetris_Score>(FName("TetrisScores"));

	FIGURES_TYPES.Collumns = Collumns;
	FIGURES_TYPES.Rows = Rows;

	// Field Initialise
	FieldInitialize();
}

void ATetris_Core::BeginPlay()
{
	AActor::BeginPlay();
	if (Field.Num() != (Rows - 1) || Field[0].Num() != (Collumns - 1))
	{
		DestroyField();
		FieldInitialize();
	}

	if (FIGURES_TYPES.Collumns != Collumns)
	{
		UE_LOG(Tetris_Core_Log, Fatal, TEXT("Rows & Collumns values must be equal in Tetris_Core & BLOCK_TYPES_s!!!"));
	}

}

void ATetris_Core::UnPauseGameBP()
{
	if (!IsGameNow())
		return;
	if (IsGamePaused())
	{
		UnPauseGame();
	}
}

void ATetris_Core::RestartGameBP()
{
	RestartGame();

}

void ATetris_Core::EndGameBP()
{
	if (!IsGameNow())
		return;
	SetGameLoosed();
	EndGame();
}

void ATetris_Core::StartGameBP()
{
	if (IsGameNow() || ProcessDesroying)
		return;
	GamePaused = false;
	StartGame();
}

void ATetris_Core::PauseGameBP()
{
	if (!IsGameNow())
		return;
	if (!IsGamePaused())
	{
		PauseGame();
	}
}

//Settings
void ATetris_Core::ChangeFilledBlocksColor(FLinearColor NewColor)
{
	TetrisRender->ChangeBlocksColor(NewColor);
}

void ATetris_Core::ChangeEmptyBlocksColor(FLinearColor NewColor)
{
	TetrisRender->ChangeEmptyBlocksColor(NewColor);
}

// ANIMATIONS
//
void ATetris_Core::Anim_FillField()
{
	TetrisRender->ClearField_Anim_Forward();
}
void ATetris_Core::Anim_ClearField()
{
	TetrisRender->ClearField_Anim_Reverse();
}
//


// Private Methods
//
void ATetris_Core::DestroyFullRow(int DelRow)
{
	if (!IsGameNow())
		return;
	UE_LOG(Tetris_Core_Log, Display, TEXT("DestroyFullRow : Start"));

	Field.RemoveAt(DelRow);
	Field.EmplaceAt(0, TArray<bool>());
	for (int c = 0; c < Collumns; ++c)
	{
		Field[0].Add(0);
	}
	TetrisRender->UpdateStaticBlocks_RowDestroy(DelRow);
	UE_LOG(Tetris_Core_Log, Display, TEXT("DestroyFullRow : Finish"));
}

void ATetris_Core::StopFallingFigure()
{
	if (!IsGameNow())
		return;

	UE_LOG(Tetris_Core_Log, Display, TEXT("StopFallingFigure : Start"));

	FVector2D Coords[4];
	for (int i = 0; i < 4; ++i)
		Coords[i] = FallingFigure->GetBlocksCoordinates()[i];

	for (int i = 0; i < 4; ++i)
	{
		if (Coords[i].X >= Rows || Coords[i].X < 0 || Coords[i].Y < 0 || Coords[i].Y >= Collumns)
		{
			UE_LOG(Tetris_Core_Log, Error, TEXT("ZHOPA IN STOP FALLING FIGURE : X = %i, Y = %i"), Coords[i].X,
				   Coords[i].Y);
			return;
		}
		Field[Coords[i].X][Coords[i].Y] = true;
	}

	TetrisTime->FigureStop_CheckForSpeedUp();
	CheckForLose();
	TetrisRender->MakeFallingFigureStatic();
	FallingFigure->Destroy();
	FallingFigure = nullptr;
	CheckForFilledRow();
	CreateFallingFigure();
	OnScoreUpdated();

	UE_LOG(Tetris_Core_Log, Display, TEXT("StopFallingFigure : Success"));
}

// Getters
//
TArray<bool> ATetris_Core::GetRowByIndex(int32 Ind)
{
	return Field[Ind];
}

FVector2D ATetris_Core::GetFallingBlockCoords(int i)
{
	return FVector2D(FallingFigure->GetBlockByIndex(i).X, FallingFigure->GetBlockByIndex(i).Y);
}

TArray<TArray<FVector2D>> &ATetris_Core::GetFiguresStartupLocations()
{
	return FIGURES_TYPES.Figures_Starts;
}

TArray<TArray<TArray<FVector2D>>> &ATetris_Core::GetFiguresDeltaRotationLocs()
{
	return FIGURES_TYPES.FiguresRotations;
}

FAllExtraValues ATetris_Core::GetPublicUIData()
{
	return PublicUIData;
}

inline bool ATetris_Core::IsGamePaused()
{
	return GamePaused;
}

// SAFETY --------------------------------------
//
inline bool ATetris_Core::IsGameNow()
{
	return GameNow;
}

inline bool ATetris_Core::IsUnsafeProcess()
{
	return ProcessDesroying || !GameNow || GamePaused;
}

inline void ATetris_Core::SetGameLoosed()
{
	GameNow = false;
}

inline void ATetris_Core::SetIsDestroyProcess(bool IsProcess)
{
	ProcessDesroying = IsProcess;
}

// Checks Coordinates
//
bool ATetris_Core::CheckIsCoordinatesEmpty(FVector2D Coords)
{
	if (Coords.X >= Rows || Coords.X < 0 || Coords.Y >= Collumns || Coords.Y < 0)
		return false;
	return !Field[Coords.X][Coords.Y];
}

bool ATetris_Core::IsNextCoordsEmpty(FVector2D *Blocks, int AddDown, int AddRight) // True если координаты пусты
{
	int CheckLocRow, CheckLocCol;

	for (int i = 0; i < 4; ++i)
	{
		CheckLocRow = Blocks[i].X + AddDown;
		CheckLocCol = Blocks[i].Y + AddRight;
		UE_LOG(Tetris_Core_Log, Warning, TEXT("Block: %i  | PotentialCoordinates: R - %i , C - %i"), i, CheckLocRow,
			   CheckLocCol);

		if (CheckLocRow >= Rows || CheckLocRow < 0 || CheckLocCol < 0 || CheckLocCol >= Collumns)
		{
			UE_LOG(Tetris_Core_Log, Error, TEXT("Block: %i - Out of Field, cant go there"), i);
			return false;
		}

		if (Field[CheckLocRow][CheckLocCol])
		{
			UE_LOG(Tetris_Core_Log, Error, TEXT("Block: %i - Cell is not empty, cant go there"), i);
			return false;
		}
	}

	return true;
}

// Check For Game Events
//
void ATetris_Core::CheckForFilledRow()
{
	bool IsRowFilled;
	int DeletingRowsCount = 0;
	for (int r = 0; r < Rows; ++r)
	{
		IsRowFilled = true;
		for (int c = 0; c < Collumns; ++c)
		{
			if (!Field[r][c])
				IsRowFilled = false;
		}
		if (IsRowFilled)
		{
			APawn* temp = GetWorld()->GetFirstPlayerController()->GetPawn();
			Cast<APlayerPawn>(temp)->OnLineDestroyed();

			DestroyFullRow(r);
			++DeletingRowsCount;
		}
	}
	TetrisScore->AddScoreForRows(DeletingRowsCount);
}

void ATetris_Core::CheckForLose() // In StopFallingFigure(), before FallingBlockDestroy
{
	for (int i = 0; i < 4; ++i)
	{
		if (FallingFigure->GetBlocksCoordinates()[i].X == 0)
		{
			GameNow = false;
			UE_LOG(Tetris_Core_Log, Warning, TEXT("Static Block on coordinate X=0 => Lose "));
		}
	}
}

void ATetris_Core::OnScoreUpdated_Implementation()
{
	APawn *temp = GetWorld()->GetFirstPlayerController()->GetPawn();
	Cast<APlayerPawn>(temp)->OnTetrisScoreUpdated();
}

// GAMELOOP EVENTS
void ATetris_Core::UpdateFallingFigure()
{
	if (IsUnsafeProcess())
		return;
	if (!FallingFigure)
		return;

	FallingFigure->UpdateFigure();
	TetrisRender->ChangeFallingBlocksCoordinates(FallingFigure->GetBlocksCoordinates());
}

void ATetris_Core::StartGame_Implementation()
{
	if (ProcessDesroying)
		return;

	GameNow = true;
	FieldFilling();

	TetrisTime->TTM_StartGame();
	TetrisScore->StartGame();
	OnScoreUpdated();

	CreateNextFigure();
	CreateFallingFigure();
}

void ATetris_Core::EndGame_Implementation()
{
	SetIsDestroyProcess(true);
	TetrisTime->TTM_EndGame();
	TetrisRender->StartDestroyRenderedBlocks();
}

// FIELD -------------------------------------------------
//
void ATetris_Core::FieldInitialize()
{
	for (int i = 0; i < Rows; ++i)
	{
		Field.Add(TArray<bool>());
		for (int f = 0; f < Collumns; ++f)
		{
			Field[i].Add(false);
		}
	}
}
void ATetris_Core::FieldFilling(bool IsFilled)
{
	for (int r = 0; r < Rows; ++r)
		for (int c = 0; c < Collumns; ++c)
		{
			Field[r][c] = 0;
		}
}
void ATetris_Core::DestroyField()
{
	int Rw = Field.Num();
	int Coll = Field[0].Num();
	for (int r = 0; r < Rw; ++r)
	{
		Field[r].Empty();
	}
	Field.Empty();
}
// FIGURES -----------------------------------------------
//
void ATetris_Core::CreateNextFigure()
{
	if (NextFigure == nullptr)
	{
		NextFigure = GetWorld()->SpawnActor<AFigureBase>();
		if (NextFigure != nullptr)
		{
			NextFigure->InitialiseFigure(this, FMath::Abs(FMath::Rand() % TypesNumber), Rows, Collumns);
		}
		PublicUIData.NextFigure = NextFigure;
	}
}
void ATetris_Core::CreateFallingFigure()
{
	UE_LOG(Tetris_Core_Log, Display, TEXT("CreateFallingFigure() : Start"));
	FallingFigure = NextFigure;
	NextFigure = nullptr;
	TetrisRender->CreateNewRenderedFigure(FallingFigure->GetBlocksCoordinates());
	if(!IsUnsafeProcess())
	CreateNextFigure();

	if (!IsGameNow())
	{
		EndGame_Implementation();
		return;
	}
	for (int i = 0; i < 4; ++i)
	{
		if (!CheckIsCoordinatesEmpty(FallingFigure->GetBlockByIndex(i)))
		{
			GameNow = false;
			UE_LOG(Tetris_Core_Log, Warning, TEXT("Lose - Cant Create Falling Figure in Default Coordinates"));
			EndGame_Implementation();
			return;
		}
	}
	UE_LOG(Tetris_Core_Log, Display, TEXT("CreateFallingFigure : Success\n"));
}

// INPUT /////////////////
//
void ATetris_Core::MoveFigureRight()
{
	if (IsUnsafeProcess())
		return;
	FallingFigure->AddMovementRight();
	TetrisRender->ChangeFallingBlocksCoordinates(FallingFigure->GetBlocksCoordinates());
}
void ATetris_Core::MoveFigureLeft()
{
	if (IsUnsafeProcess())
		return;
	FallingFigure->AddMovementLeft();
	TetrisRender->ChangeFallingBlocksCoordinates(FallingFigure->GetBlocksCoordinates());
}
void ATetris_Core::RotateFigure()
{
	if (IsUnsafeProcess())
		return;
	FallingFigure->RotateToNext();
	TetrisRender->ChangeFallingBlocksCoordinates(FallingFigure->GetBlocksCoordinates());
}
void ATetris_Core::MoveSuperDown()
{
	if (IsUnsafeProcess())
		return;
	FallingFigure->AddMovementSuperDown();
	TetrisRender->ChangeFallingBlocksCoordinates(FallingFigure->GetBlocksCoordinates());
}
void ATetris_Core::MoveDownStart()
{
	if (!IsGameNow() || ProcessDesroying)
		return;
	TetrisTime->StartSuperDown();
}
void ATetris_Core::MoveDownStop()
{
	if (!IsGameNow() || ProcessDesroying)
		return;
	TetrisTime->EndSuperDown();
}
void ATetris_Core::RestartGame()
{
	static	bool NextTimerSet = false;
	if (IsGameNow())
	{
		GameNow = false;
		EndGame_Implementation();
	}
	if (ProcessDesroying || GamePaused)
	{
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATetris_Core::RestartGame);
	}
	else
	{
		StartGameBP();
		NextTimerSet = false;
	}
}

void ATetris_Core::Pause_UnPause_Game()
{
	if (!IsGameNow())
		return;
	if (IsGamePaused())
	{
		UnPauseGame();
	}
	else 
	{
		PauseGame();
	}


}

void ATetris_Core::PauseGame_Implementation()
{
	GamePaused = true;
	TetrisTime->PauseGame();
	APlayerPawn* pp = Cast<APlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	pp->OnGamePaused();
}
void ATetris_Core::UnPauseGame_Implementation()
{
	GamePaused = false;
	TetrisTime->UnPauseGame();
	APlayerPawn* pp = Cast<APlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	pp->OnGameUnPaused();
}