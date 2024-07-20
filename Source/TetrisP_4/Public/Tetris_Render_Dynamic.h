
#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "TetrisSystems/TetrisRenderBase.h"
#include "Tetris_Render_Dynamic.generated.h"

class ATetris_Core;
DEFINE_LOG_CATEGORY_STATIC(Tetris_Render_Log, All, All);

UCLASS()
class TETRISP_4_API UTetris_Render_Dynamic : public UTetrisRenderBase
{
	GENERATED_BODY()

  public:
	// Sets default values for this actor's properties
	UTetris_Render_Dynamic();

	void ChangeFallingBlocksCoordinates(FVector2D *NewCoordinates); // При изменении положения падающей фигуры, после
																	  // установки новых координат в Tetris_Core

	void MakeFallingFigureStatic(); // Во время StopFallingFigure в tetris core

	void UpdateStaticBlocks_RowDestroy(int DelRow);

	void CreateNewRenderedFigure(FVector2D *Coords); // Во время CreateNextFigure в tetris core

	FVector ToLocation(FVector2D Coords);

	void StartDestroyRenderedBlocks();

	// ATRIBUTES
	ATetris_Core* TetrisCore;

	AStaticMeshActor *FallingRenderedBlocks[4];

	TArray<AStaticMeshActor *> StaticRenderedBlocks;

	UPROPERTY(EditAnywhere)
	UStaticMesh *ExampleSM;

	UPROPERTY(EditAnywhere)
	FVector ExampleScale;

	UPROPERTY(EditAnywhere)
	FRotator ExampleRotator;

  protected:
	virtual void BeginPlay() override;
};