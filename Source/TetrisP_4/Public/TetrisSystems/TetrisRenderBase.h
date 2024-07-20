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

	void ChangeFallingBlocksCoordinates(FVector2D* NewCoordinates) ; // ��� ��������� ��������� �������� ������, �����
	// ��������� ����� ��������� � Tetris_Core

	void MakeFallingFigureStatic(); // �� ����� StopFallingFigure � tetris core

	void UpdateStaticBlocks_RowDestroy(int DelRow);

	void CreateNewRenderedFigure(FVector2D* Coords); // �� ����� CreateNextFigure � tetris core

	FVector ToLocation(FVector2D Coords);

	void StartDestroyRenderedBlocks();

};
