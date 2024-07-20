// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tetris_Core.h"
#include "FigureBase.generated.h"


DEFINE_LOG_CATEGORY_STATIC(Figure_Core_Log, All, All)

USTRUCT(BlueprintType)
struct FigureBody
{
	GENERATED_BODY()

	FigureBody(FVector2D *B1);
	FigureBody(){};

	FVector2D Blocks[4];
};

UCLASS()
class TETRISP_4_API AFigureBase : public AActor
{
	GENERATED_BODY()

  public:
	AFigureBase();
	void InitialiseFigure(ATetris_Core *TB, short FigureIndex, int R, int C);

	virtual void UpdateFigure();

	virtual void AddMovementFall();
	virtual void AddMovementRight();
	virtual void AddMovementLeft();
	virtual void AddMovementSuperDown();

	virtual void RotateToNext();

	// Getters
	FVector2D *GetBlocksCoordinates();

	FVector2D& GetBlockByIndex(int i)
	{
		return Body.Blocks[i];
	};

	FVector2D GetBlockPrev(int i)
	{
		FVector2D temp;
		temp.X = Body.Blocks[i].X - 1;
		temp.Y = Body.Blocks[i].Y - 1;
		if (temp.Y < 0)
			return FVector2D(404, 404);

		return temp;

	}



	// Atributes
	UPROPERTY(BlueprintReadWrite)
	int FIGURE_TYPE_INDEX;
	short ROTATION_INDEX = -1;

	UPROPERTY(BlueprintReadWrite)
	FigureBody Body;

	ATetris_Core *TetrisBody;

	short Rows;
	short Collumns;
};
