

#include "TetrisSystems/Tetris_Score.h"
#include "PlayerPawn.h"
#include "Tetris_Core.h"

UTetris_Score::UTetris_Score()
{
	PrimaryComponentTick.bCanEverTick = false;
	ScoreValues.MaxScore = 0;
	TetrisCore = Cast<ATetris_Core>(GetOwner());
}

void UTetris_Score::StartGame()
{
	ScoreValues.TotalScore = 0;
	TetrisCore->PublicUIData.TotalScore = 0;
	TetrisCore->OnScoreUpdated();
}

void UTetris_Score::EndGame()
{
}

void UTetris_Score::AddScoreForRows(int RowsCount)
{
	TetrisCore->PublicUIData.DestroyedLines += RowsCount;
	ScoreValues.DestroyedLines += RowsCount;
	
	switch(RowsCount)
	{
	case 1:
	UpdateScore(ScoreFor1);
	case 2:
	UpdateScore(ScoreFor2);
	case 3:
	UpdateScore(ScoreFor3);
	case 4:
	UpdateScore(ScoreFor4);
	case 0:
	UpdateScore(ScoreFor0);
	}
}

FScoreValues &UTetris_Score::GetAllScoreValues()
{
	return ScoreValues;
}

void UTetris_Score::UpdateScore(int AddScore)
{
	TetrisCore->PublicUIData.TotalScore += AddScore;
	ScoreValues.TotalScore += AddScore;
	if (ScoreValues.TotalScore > ScoreValues.MaxScore)
	{
		ScoreValues.MaxScore = ScoreValues.TotalScore;
		TetrisCore->PublicUIData.MaxScore = ScoreValues.TotalScore;
	}
	UE_LOG(Tetris_Score_Log, Display, TEXT("Score Added;   Current Score: %i"), ScoreValues.TotalScore);
}
