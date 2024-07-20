// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuSettings_System.h"
#include "PlayerPawn.generated.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerPawn_Log, All, All)

class ATetris_Core;
class ATetris_NF_Viewer;

UCLASS()
class TETRISP_4_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATetris_Core* Tetris;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATetris_NF_Viewer* Tetris_NF_Viewer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COLOR SETTINGS")
	UMenuSettings_System* MenuSettings;

	UMenuSettings_System* GetMenuSettings();

//INPUT ACTIONS 
	UFUNCTION(BlueprintCallable)
	void MoveFigureRight();

	UFUNCTION(BlueprintCallable)
	void MoveFigureLeft();

	UFUNCTION(BlueprintCallable)
	void MoveFigureRight_Released();

	UFUNCTION(BlueprintCallable)
	void MoveFigureLeft_Released();

	UFUNCTION(BlueprintCallable)
	void RotateFigure();

	UFUNCTION(BlueprintCallable)
	void MoveSuperDown();

	UFUNCTION(BlueprintCallable)
	void MoveDownButtonPressed();

	UFUNCTION(BlueprintCallable)
	void MoveDownButtonReleased();

	UFUNCTION(BlueprintCallable)
	void TEST_RESTART();

	UFUNCTION(BlueprintCallable)
	void GamePauseButtonPressed();
	///////////////------------------------

	UFUNCTION(BlueprintNativeEvent)
		void OnTetrisScoreUpdated();

	UFUNCTION(BlueprintNativeEvent)
		void OnGamePaused();

	UFUNCTION(BlueprintNativeEvent)
		void OnGameUnPaused();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLineDestroyed();

	bool IsReleased;
	FTimerHandle MoveLeftLongPress_TH;
	FTimerHandle MoveRightLongPress_TH;
};
