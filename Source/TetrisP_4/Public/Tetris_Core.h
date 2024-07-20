// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "FigureTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tetris_Core.generated.h"

DEFINE_LOG_CATEGORY_STATIC(Tetris_Core_Log, All, All)
DEFINE_LOG_CATEGORY_STATIC(Tetris_Time_Log, All, All)

class AFigureBase;
class UTetrisTimeManager;
class UTetrisRender_v2;
struct FScoreValues;
class UTetris_Score;
class UTetris_Render_Dynamic;
class UMenuSettings_System;

USTRUCT(BlueprintType)
struct FAllExtraValues
{
	GENERATED_BODY()

	FAllExtraValues()
	{
		DestroyedLines = 0;
		MaxScore = 0;
		SpeedLevel = 0;
		TotalScore = 0;
		NextFigure = nullptr;
	}

	UPROPERTY(BlueprintReadOnly)
	int SpeedLevel;

	UPROPERTY(BlueprintReadOnly)
	AFigureBase *NextFigure;

	UPROPERTY(BlueprintReadOnly)
	int DestroyedLines;

	UPROPERTY(BlueprintReadOnly)
	int TotalScore;

	UPROPERTY(BlueprintReadOnly)
	int MaxScore;
};

UCLASS()
class TETRISP_4_API ATetris_Core : public AActor
{
	friend class UTetrisRender_v2;
	friend class AFigureBase;
	friend class UMenuSettings_System;
	friend class APlayerPawn;
	friend class UTetris_Render_Dynamic;
	friend class UTetris_Score;
	friend class UTetrisTimeManager;

	GENERATED_BODY()

  public:
	ATetris_Core();

  protected:
	virtual void BeginPlay() override;

  public:
	  //BP Callable Methods
	  UFUNCTION(BlueprintCallable)
		  void StartGameBP();

	  UFUNCTION(BlueprintCallable)
		  void PauseGameBP();

	  UFUNCTION(BlueprintCallable)
		  void UnPauseGameBP();

	  UFUNCTION(BlueprintCallable)
		  void RestartGameBP();

	  UFUNCTION(BlueprintCallable)
		  void EndGameBP();

	  // BP Settings Methods
	  UFUNCTION(BlueprintCallable)
	  void ChangeFilledBlocksColor(FLinearColor NewColor);

	  UFUNCTION(BlueprintCallable)
		void ChangeEmptyBlocksColor(FLinearColor NewColor);

	  //ANIMATIONS
	  UFUNCTION(BlueprintCallable)
		 void Anim_FillField();

	  UFUNCTION(BlueprintCallable)
		  void Anim_ClearField();


	  // PUBLIC GETTERS ------------------------
	  //
	  UFUNCTION(BlueprintCallable)
		  TArray<bool> GetRowByIndex(int32 Ind);

	  UFUNCTION(BlueprintCallable)
		  FVector2D GetFallingBlockCoords(int i);



	// GAMEPLAY EVENTS

	UFUNCTION(BlueprintNativeEvent)
	void StartGame();

	UFUNCTION(BlueprintNativeEvent)
	void EndGame();

	UFUNCTION(BlueprintNativeEvent)
	void PauseGame();

	UFUNCTION(BlueprintNativeEvent)
	void UnPauseGame();

	UFUNCTION(BlueprintNativeEvent)
	void OnScoreUpdated();

	// INPUT ACTIONS
	//
	void MoveFigureRight();
	void MoveFigureLeft();
	void RotateFigure();
	void MoveSuperDown();
	void MoveDownStart();
	void MoveDownStop();
	void RestartGame();
	void Pause_UnPause_Game();

  private:
	// CHECKERS ----------------------------
	//
	bool IsNextCoordsEmpty(FVector2D *Blocks, int AddDown = 0, int AddRight = 0);
	bool CheckIsCoordinatesEmpty(FVector2D Coords);

	// Gamecycle Subevents -----------------
	//
	inline void DestroyFullRow(int DelRow);

	// FALL TICK ---------------------------
	//
	void UpdateFallingFigure();
	void CheckForFilledRow();
	void CheckForLose();

	// SUBSYSTEMS ----------------------------
	//
	UPROPERTY(EditAnywhere, Category = "Tetris Subsystems")
	UTetrisRender_v2 *TetrisRender;

	UPROPERTY(EditAnywhere, Category = "Tetris Subsystems")
	UTetrisTimeManager *TetrisTime;

	UPROPERTY(EditAnywhere, Category = "Tetris Subsystems")
	UTetris_Score *TetrisScore;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *RootTetris;

	// FIELD  ---------------------------------
	//
	UPROPERTY(EditAnywhere, Category = "Field")
	int Rows = 20;

	UPROPERTY(EditAnywhere, Category = "Field")
	int Collumns = 10;

	TArray<TArray<bool>> Field;

	void FieldInitialize();
	void FieldFilling(bool IsFilled = false);
	void DestroyField();

	// FIGURES -----------------------------
	// Fugure is created when the previous one stops and Stop Falling Figure() is called
	//
	UPROPERTY(EditAnywhere)
	FBLOCKS_TYPES_s FIGURES_TYPES;

	UPROPERTY()
	AFigureBase *FallingFigure;

	UPROPERTY()
	AFigureBase *NextFigure;

	inline void CreateFallingFigure();
	inline void CreateNextFigure();
	void StopFallingFigure();

	TArray<TArray<FVector2D>> &GetFiguresStartupLocations();
	TArray<TArray<TArray<FVector2D>>> &GetFiguresDeltaRotationLocs();

	// Public Data ---------------------------    (Especially for UI)
	//
  public:
	UFUNCTION(BlueprintCallable)
	FAllExtraValues GetPublicUIData();

  private:
	FAllExtraValues PublicUIData;

	// Booleans -----------------------------
	//
	bool GameNow = false;

	bool GamePaused = false;

	inline bool IsGamePaused();

	inline bool IsGameNow();

	// Processes Safety
	bool ProcessDesroying = false;

	inline bool IsUnsafeProcess();

	inline void SetGameLoosed();

	inline void SetIsDestroyProcess(bool IsProcess);
};
