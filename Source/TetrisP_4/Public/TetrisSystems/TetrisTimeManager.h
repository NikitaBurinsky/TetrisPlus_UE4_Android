// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "TetrisTimeManager.generated.h"
class ATetris_Core;
class UMenuSettings_System;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TETRISP_4_API UTetrisTimeManager : public UActorComponent
{
	GENERATED_BODY()
	friend class UMenuSettings_System;

  public:
	// Sets default values for this component's properties
	UTetrisTimeManager();

  protected:
	virtual void BeginPlay() override;

  public:
	void TTM_StartGame();
	void TTM_EndGame();
	void PauseGame();
	void UnPauseGame();
	void FallingFigure_Tick();
	void UpdateTimeSpeedLevel();
	void StartSuperDown();
	void EndSuperDown();

	void FigureStop_CheckForSpeedUp();

	FTimerHandle FigureFall_Base;

	FTimerHandle FigureFall_Extra;

	ATetris_Core *TetrisCore;

	// Atributes
	int DelayIndex = 0;
	int SuperDownDelayIndex = 0;
	int TickCount = 0;

	/// PROPERTY ///

	//Edit in MenuSettings
	int StartSpeedIndex = 0;

	UPROPERTY(EditAnywhere)
	TArray<float> Delays;

	UPROPERTY(EditAnywhere)
	int TickCountToUpdate = 100;

	UPROPERTY(EditAnywhere)
	TArray<float> SuperDownDelays;
};
