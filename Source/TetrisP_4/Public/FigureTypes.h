#pragma once
#include "CoreMinimal.h"
#include "FigureTypes.generated.h"

#define TypesNumber 7

USTRUCT()
struct FBLOCKS_TYPES_s
{
	GENERATED_BODY()

	typedef FVector2D V;

	// Должно быть всегда равно Rows в Tetris_Core
	UPROPERTY(EditAnywhere)
	int Rows = 20;

	// Должно быть всегда равно Collumns в Tetris_Core
	UPROPERTY(EditAnywhere)
	int Collumns = 10;

	TArray<TArray<FVector2D>> Figures_Starts;// [FigureType][BlockIndex]

	TArray<TArray<TArray<FVector2D>>> FiguresRotations;//  [FigureType][RotationIndex][BlockIndex]


	FBLOCKS_TYPES_s()
	{
		
		// STARTS //
		Figures_Starts = // [FigureType][BlockIndex]
			{			 
			 // 0 Cube+
			 {V(0, (Collumns / 2) - 1), V(0, (Collumns / 2)), V(1, (Collumns / 2) - 1), V(1, (Collumns / 2))},

			 // 1 Z + 
			 {V(0, Collumns / 2 - 1), V(0, (Collumns / 2) ), V(1, (Collumns / 2) ), V(1, (Collumns / 2) + 1)},

			 // 2 Stick + 
			 {V(0, (Collumns / 2 - 2)), V(0, (Collumns / 2)- 1), V(0, (Collumns / 2) ), V(0, (Collumns / 2) + 1)},

			 // 3 T +
			 {V(1, (Collumns / 2) - 1), V(0, (Collumns / 2)),V(1, (Collumns / 2)),V(1, (Collumns / 2) + 1)},
			 // 4 L + 
			 {V(0, (Collumns / 2) - 1), V(1,(Collumns / 2) - 1),V(2,(Collumns / 2) - 1),V(2,(Collumns / 2))},
			 // 5 J + 
			 {V(0, (Collumns / 2)), V(1,(Collumns / 2)),V(2,(Collumns / 2)),V(2,(Collumns / 2)-1)},

			 // 6 S +
			 {V(1,(Collumns / 2) - 1), V(1,(Collumns / 2)),V(0,(Collumns / 2)),V(0,(Collumns / 2) + 1)}
		};



		// ROTATIONS //

		FiguresRotations = //  [FigureType][RotationIndex][BlockIndex]
		{
			////////////////  0    -    Cube ////////////////
			 {{V(0, 0), V(0, 0), V(0, 0), V(0, 0)}},

			////////////////  1     -     Z //////////////////
			 {
			  {V(-1, 2), V(0, 1), V(-1, 0), V(0, -1)},
			  {V(1, -2), V(0, -1), V(1, 0), V(0, 1)}
			 },

			 ////////////////  2    -    Stick ////////////////
			 {
				{V(2, 2), V(-1, 1), V(0, 0), V(1, -1)},
				{V(-2, -2), V(1, -1), V(0, 0), V(-1, 1)}
			 },

			 ////////////////  3    -    T     ///////////////
			{
			{V(1,1), V(0,0), V(0,0), V(0,0)},
			{V(0,0),V(1,-1),V(0,0),V(0,0)},
			{V(0,0),V(0,0),V(0,0),V(-1,-1)},
			{V(-1,-1),V(-1,1),V(0,0),V(1,1)}
			},
				////////////////  4    -    L     ///////////////
			{
			{V(1,-1),V(0,0),V(-1,1),V(-2,0)},
			{V(1,1),V(0,0),V(-1,-1),V(0,-2)},
			{V(-1,1),V(0,0),V(1,-1),V(2,0)},
			{V(-1,-1),V(0,0),V(1,1),V(0,2)}
			},
			////////////////  5    -    J     ///////////////
			{
			{V(1,-1),V(0,0),V(-1,1),V(0,2)},
			{V(1,1),V(0,0),V(-1,-1),V(-2,0)},
			{V(-1,1),V(0,0),V(1,-1),V(0,-2)},
			{V(-1,-1),V(0,0),V(1,1),V(2,0)}
			},
			////////////////  6    -    S     ///////////////
			{
			{V(1,1),V(0,0),V(1,-1),V(0,-2)},
			{V(-1,-1),V(0,0),V(-1,1),V(0,2)}
			}
		};

	}
};
