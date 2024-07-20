#include "FigureBase.h"
#include "FigureTypes.h"
#include "TetrisSystems/TetrisRender_v2.h"
#include "Tetris_Core.h"

AFigureBase::AFigureBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFigureBase::InitialiseFigure(ATetris_Core *TB, short FigureIndex, int R, int C)
{
	UE_LOG(Figure_Core_Log, Warning, TEXT("Start Initialize Figure : %s"), *GetName());
	TetrisBody = TB;
	ROTATION_INDEX = -1;
	FIGURE_TYPE_INDEX = FigureIndex;
	Rows = R;
	Collumns = C;
	for (int i = 0; i < 4; ++i)
	{
		Body.Blocks[i] = TetrisBody->GetFiguresStartupLocations()[FIGURE_TYPE_INDEX][i];
		UE_LOG(Figure_Core_Log, Display, TEXT("Block: %i  -  X : %i, Y : %i"), i,
			   TetrisBody->GetFiguresStartupLocations()[FIGURE_TYPE_INDEX][i].X,
			   TetrisBody->GetFiguresStartupLocations()[FIGURE_TYPE_INDEX][i].Y);
	}
	UE_LOG(Figure_Core_Log, Warning, TEXT("End Initialize Figure"));
}

void AFigureBase::UpdateFigure()
{
	AddMovementFall();
}

void AFigureBase::AddMovementFall()
{
	if (!TetrisBody->IsNextCoordsEmpty(Body.Blocks, 1))
	{
		TetrisBody->StopFallingFigure();
		return;
	}

	for (int i = 0; i < 4; ++i)
	{
		Body.Blocks[i].X += 1;
	}
}

void AFigureBase::AddMovementRight()
{

	if (!TetrisBody->IsNextCoordsEmpty(Body.Blocks, 0, 1))
	{return;}

	for (int i = 0; i < 4; ++i)
	{
		Body.Blocks[i].Y += 1;
	}
}

void AFigureBase::AddMovementLeft()
{
	if (!TetrisBody->IsNextCoordsEmpty(Body.Blocks, 0, -1))
	{return;}

	for (int i = 0; i < 4; ++i)
	{
		Body.Blocks[i].Y -= 1;
	}
}

void AFigureBase::AddMovementSuperDown()
{
	FVector2D Coords[4];
	for (int i = 0; i < 4; ++i)
		Coords[i] = Body.Blocks[i];

	while (TetrisBody->IsNextCoordsEmpty(Coords, 1))
	{
		for (int i = 0; i < 4; ++i)
		{
			++Coords[i].X;
		}
	}
	for (int i = 0; i < 4; ++i)
		Body.Blocks[i] = Coords[i];
	TetrisBody->TetrisRender->ChangeFallingBlocksCoordinates(GetBlocksCoordinates());
	TetrisBody->StopFallingFigure();
}

void AFigureBase::RotateToNext()
{
	++ROTATION_INDEX;
	if (ROTATION_INDEX >= TetrisBody->GetFiguresDeltaRotationLocs()[FIGURE_TYPE_INDEX].Num())
		ROTATION_INDEX = 0;

	FVector2D Temp[4];
	for (int i = 0; i < 4; ++i)
	{

		Temp[i].X =
			Body.Blocks[i].X + TetrisBody->GetFiguresDeltaRotationLocs()[FIGURE_TYPE_INDEX][ROTATION_INDEX][i].X;
		Temp[i].Y =
			Body.Blocks[i].Y + TetrisBody->GetFiguresDeltaRotationLocs()[FIGURE_TYPE_INDEX][ROTATION_INDEX][i].Y;
		if (!TetrisBody->CheckIsCoordinatesEmpty(Temp[i]))
		{
			--ROTATION_INDEX;
			return;
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		Body.Blocks[i] = Temp[i];
	}
	return;
}

FVector2D *AFigureBase::GetBlocksCoordinates()
{
	return Body.Blocks;
}

// FIGURE BODY CONSTRUCTOR
FigureBody::FigureBody(FVector2D *B1)
{
	for (int i = 0; i < 4; ++i)
	{
		Blocks[i] = B1[i];
	}
}
