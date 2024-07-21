// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMeshActor.h"
#include "TetrisRenderBase.h"
#include "Materials/MaterialInstance.h"
#include "TetrisRender_v2.generated.h"

DEFINE_LOG_CATEGORY_STATIC(Tetris_Render_2_Log, All, All)

class ATetris_Core;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TETRISP_4_API UTetrisRender_v2 : public UTetrisRenderBase
{
	friend class ATetris_Core;

	GENERATED_BODY()

private:
	//INITIALIZE FIELD ----------------------------------------------
	//

	UTetrisRender_v2();

	virtual void BeginPlay() override;


	void CreateField();

	void SetFieldVisibility(bool IsVisibleField = false);

	// TETRIS RENDER PUBLIC INTERFACE -----------------------------
	//
public:
	void CreateNewRenderedFigure(FVector2D* Coords);

	void ChangeFallingBlocksCoordinates(FVector2D* NewCoordinates);

	void UpdateStaticBlocks_RowDestroy(int DelRow);

	void MakeFallingFigureStatic();

	void StartDestroyRenderedBlocks();

private:
	// Clear Field Animation
	//
	void ClearField_Anim_Forward();
	void ClearField_Anim_Reverse();
	int CurDelR;
	bool Delay1Frame = false;
	//
	FVector ToLocation(FVector2D Coords);
	//
	//Функция используется для определения того, как будут выглядеть занятые\свободные клетки
	inline void SetBlockInUse(int RowInd, int ColInd, bool IsUsed); 

	inline bool IsBlockEmpty(int RowInd, int ColInd);

	//Settings
	void StartUpRenderMaterials();
	void ChangeBlocksColor(FLinearColor& NewColor);
	void ChangeEmptyBlocksColor(FLinearColor& NewColor);
	UPROPERTY()
	UMaterialInstanceDynamic* EmptyBlockMaterial;

	
	UPROPERTY()
	UMaterialInstanceDynamic* FilledBlockMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Field : Blocks" )
		FLinearColor FilledBlocksDefaultColor;

	UPROPERTY(EditAnywhere, Category = "Field : Blocks")
		FLinearColor EmptyBlocksDefaultColor;


	//ATRIBUTES------------------------------------------------
	//
private:
	UPROPERTY()
	ATetris_Core* TetrisCore;

	TArray<TArray<AStaticMeshActor*>> RenderField; // [Rows][Collumns]

	UPROPERTY()
	FVector2D CurrentCoords[4];

	//FIELD PRESETS
	UPROPERTY(EditAnywhere, Category = "Field : Size")
		int Rows = 20;

	UPROPERTY(EditAnywhere, Category = "Field : Size")
		int Collumns = 10;

	UPROPERTY(EditAnywhere, Category = "Field : Blocks")
		UStaticMesh* ExampleActor;

	UPROPERTY(EditAnywhere, Category = "Field : Blocks")
		FVector ExampleScale;

	UPROPERTY(EditAnywhere, Category = "Field : Blocks")
		FRotator ExampleRotation;

	UPROPERTY(EditAnywhere, Category = "Field : Blocks")
		int LocationMultiplier = 200;



};
