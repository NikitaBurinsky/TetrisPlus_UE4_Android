// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Tetris_Core.h"
#include "GameFramework/Actor.h"
#include "MenuSettings_System.generated.h"
class APlayerPawn;
class UTetrisTimeManager;
DEFINE_LOG_CATEGORY_STATIC(Log_MenuSettings, All, All)

#define COLOR_PRESET_OBJECTS_NUM 7

USTRUCT(BlueprintType)
struct FMenuPreset
{
	GENERATED_BODY()

		TArray<TArray<FLinearColor>> ColorPresets; //[PresetIndex][Color Object Index]


	UPROPERTY(EditAnywhere)
		int CurrentColorPreset = -1;

	UPROPERTY(EditAnywhere)
		float MainSoundLevel = 1;

	UPROPERTY(EditAnywhere)
		int GameDifficulty = 1;

	FMenuPreset() {};
};

UCLASS()
class TETRISP_4_API UMenuSettings_System : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMenuSettings_System();

	UFUNCTION(BlueprintCallable)
		void BindMaterialForColorSettings(FString name, UMaterialInstanceDynamic* Mat);

	UFUNCTION(BlueprintCallable)
		void BindMeshMaterialForColorSettings(FString name, UStaticMeshComponent* SMC);

	//For Widget Using ---------------------
	UFUNCTION(BlueprintCallable)
		void ChangeColorPreset();

	UFUNCTION(BlueprintCallable)
		void AddColorPreset(TArray<FLinearColor> tal);

	UFUNCTION(BlueprintCallable)
		void ChangeMainSoundLevel(float SoundLevel_) {};

	UFUNCTION(BlueprintCallable)
	void SetDifficulty(int dif);
	/////////////----------------------------



	void ChangeMaterialColor(FString&& MaterialName, FLinearColor Color);

	void ChangeStaticMaterialColor(int SM_index, FLinearColor Color);

protected:
	virtual void BeginPlay() override;

public:

	///  ATRIBUTES
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FMenuPreset MenuPreset;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> SM_Actors;

	TMap<FString, TArray<UMaterialInstanceDynamic*>> ColorSettingsMaterials;

	TArray<UMaterialInstanceDynamic*> SM_ColorSettings;
};

