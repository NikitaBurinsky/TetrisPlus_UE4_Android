// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FigureBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Tetris_NF_Viewer.generated.h"


DEFINE_LOG_CATEGORY_STATIC(Tetris_NF_Viewer_________E,All,All)

struct TV_Blocks_Combinations
{
	TArray<TArray<bool>> OnOff_Blocks_Sets //[FigureTypeIndex][BlockIndex]
	{
		//Cube
		{0,1,1,0,0,1,1,0},
			//S
		{ 0,1,1,0,1,1,0,0 },
		//Stick
		{1,1,1,1,0,0,0,0},
		//T
		{0,1,0,0,1,1,1,0},
		//J
		{1,0,0,0,1,1,1,0},
		//L
		{ 0,0,1,0,1,1,1,0 },
		//Z
		{ 1,1,0,0,0,1,1,0 }
	};
};



UCLASS()
class TETRISP_4_API ATetris_NF_Viewer : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateNextFigureViewer(AFigureBase* NextFigure);
	//-----------------------------------------------------


	ATetris_NF_Viewer();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RootMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackgroundMesh;

	// ARRAY OF TV BLOCKS
	//
	UPROPERTY(VisibleAnywhere, Category = "TV : Blocks")
	TArray<UStaticMeshComponent*> RenderBlocks;

	//EXAMPLE VALUES
	//
	UPROPERTY(EditAnywhere, Category = "TV : Blocks")
	UStaticMesh* ExampleMesh;

	UPROPERTY(EditAnywhere, Category = "TV : Blocks")
	FTransform ExampleTransform;

	UPROPERTY(EditAnywhere, Category = "TV : Blocks")
		int LocationMultiplier = 200;

	UPROPERTY(EditAnywhere, Category = "TV : Blocks")
		FLinearColor TV_On_Color;

	UPROPERTY(EditAnywhere, Category = "TV : Blocks")
		FLinearColor TV_Off_Color;


	virtual void BeginPlay();
private:
	//StartUp
	void PrepareBlocks();
	void PrepareMaterials();

	//Private Methods
	void SetBlock_On_Off(int Ind, bool IsOn);


	//ATRIBUTES
	//
	UMaterialInstanceDynamic* TV_ON_DynMat;
	UMaterialInstanceDynamic* TV_OFF_DynMat;
	TV_Blocks_Combinations Combinations_Blocks_Data;



	// SETTINGS 
	void Change_TV_ON_Color(FLinearColor NewColor);
	void Change_TV_OFF_Color(FLinearColor NewColor);
};
