// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Tetris_Score.generated.h"
DEFINE_LOG_CATEGORY_STATIC(Tetris_Score_Log, All, All)
class ATetris_Core;

USTRUCT(BlueprintType)
struct FScoreValues
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int TotalScore;

	UPROPERTY(BlueprintReadOnly)
	int MaxScore;

	UPROPERTY(BlueprintReadOnly)
	int DestroyedLines;
};

UCLASS()
class TETRISP_4_API UTetris_Score : public UActorComponent
{
	GENERATED_BODY()

  public:
	UTetris_Score();

	// PROPERTYS //
	UPROPERTY(EditAnywhere)
	int ScoreFor0 = 10;
	UPROPERTY(EditAnywhere)
	int ScoreFor1 = 100;
	UPROPERTY(EditAnywhere)
	int ScoreFor2 = 300;
	UPROPERTY(EditAnywhere)
	int ScoreFor3 = 750;
	UPROPERTY(EditAnywhere)
	int ScoreFor4 = 1500;

	// Functions
	void StartGame();
	void EndGame();

	void AddScoreForRows(int RowsCount = 1);

	FScoreValues &GetAllScoreValues();

	UPROPERTY(BlueprintReadWrite)
	FScoreValues ScoreValues;

  private:
	// Current Score
	  UPROPERTY()
	ATetris_Core *TetrisCore;

	void UpdateScore(int AddScore);
};
