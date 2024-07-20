// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TetrisRenderBase.generated.h"

class ATetris_Core;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TETRISP_4_API UTetrisRenderBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTetrisRenderBase();

protected:
	// Called when the game starts
	void BeginPlay() override;

	void ChangeFallingBlocksCoordinates(FVector2D* NewCoordinates) ; // При изменении положения падающей фигуры, после
	// установки новых координат в Tetris_Core

	void MakeFallingFigureStatic(); // Во время StopFallingFigure в tetris core

	void UpdateStaticBlocks_RowDestroy(int DelRow);

	void CreateNewRenderedFigure(FVector2D* Coords); // Во время CreateNextFigure в tetris core

	FVector ToLocation(FVector2D Coords);

	void StartDestroyRenderedBlocks();

};
