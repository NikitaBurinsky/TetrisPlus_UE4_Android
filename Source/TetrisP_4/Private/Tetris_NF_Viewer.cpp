// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetris_NF_Viewer.h"
#include "PlayerPawn.h"

ATetris_NF_Viewer::ATetris_NF_Viewer()
{
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>("ROOT");
	RootComponent = RootMesh;
	SetRootComponent(RootMesh);

	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>("BackgroundMesh");


	PrimaryActorTick.bCanEverTick = false;

	FString s;
	for (int i = 0; i < 8; ++i)
	{
		s = "Block:" + FString::FromInt(i + 1);
		RenderBlocks.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*s)));
	}
}

void ATetris_NF_Viewer::UpdateNextFigureViewer(AFigureBase* NextFigure)
{
	if (!NextFigure)
		return;

	int FigureInd = NextFigure->FIGURE_TYPE_INDEX;
	if (FigureInd >= 8)
		return;

	for(int i = 0; i < 8; ++i)
	{
		SetBlock_On_Off(i, Combinations_Blocks_Data.OnOff_Blocks_Sets[FigureInd][i]);
	}
}

void ATetris_NF_Viewer::BeginPlay()
{
	Super::BeginPlay();

	PrepareBlocks();
}

void ATetris_NF_Viewer::PrepareBlocks()
{
	for (int i = 0; i < 8; ++i)
	{
		RenderBlocks[i]->SetStaticMesh(ExampleMesh);
		RenderBlocks[i]->SetRelativeScale3D(ExampleTransform.GetScale3D());
	}

	float Loc[8]{ -1.5, -0.5, 0.5, 1.5, -1.5, -0.5, 0.5,1.5 };
	FVector Vec(0,0,0);

	for (int i = 0; i < 4; ++i)
	{
		Vec.Z = LocationMultiplier * 0.5;
		Vec.X = LocationMultiplier * Loc[i];
		RenderBlocks[i]->SetRelativeLocation(Vec);
	}
	
	for (int i = 4; i < 8; ++i)
	{
		Vec.Z = LocationMultiplier * -0.5;
		Vec.X = LocationMultiplier * Loc[i];
		RenderBlocks[i]->SetRelativeLocation(Vec);
	}

	PrepareMaterials();
}

void ATetris_NF_Viewer::PrepareMaterials()
{
	UMaterialInterface* Material = RenderBlocks[0]->GetMaterial(0);
	TV_ON_DynMat = UMaterialInstanceDynamic::Create(Material, this);
	TV_OFF_DynMat = UMaterialInstanceDynamic::Create(Material, this);

	TV_OFF_DynMat->SetVectorParameterValue("BaseColor", TV_Off_Color);
	TV_ON_DynMat->SetVectorParameterValue("BaseColor", TV_On_Color);

	for (int i = 0; i < 8; ++i)
	{
		RenderBlocks[i]->SetMaterial(0, TV_OFF_DynMat);
	}
	BackgroundMesh->SetMaterial(0, TV_OFF_DynMat);

	APlayerPawn* PP = Cast<APlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PP->GetMenuSettings()->BindMaterialForColorSettings("TV:Empty", TV_OFF_DynMat);
	PP->GetMenuSettings()->BindMaterialForColorSettings("TV:Filled", TV_ON_DynMat);
}

void ATetris_NF_Viewer::SetBlock_On_Off(int Ind, bool IsOn)
{
	if (IsOn)
	{
		RenderBlocks[Ind]->SetMaterial(0, TV_ON_DynMat);
		return;
	}
	else
	{
		RenderBlocks[Ind]->SetMaterial(0, TV_OFF_DynMat);
	}
}

void ATetris_NF_Viewer::Change_TV_ON_Color(FLinearColor NewColor)
{
	TV_ON_DynMat->SetVectorParameterValue("BaseColor", NewColor);
}

void ATetris_NF_Viewer::Change_TV_OFF_Color(FLinearColor NewColor)
{
	TV_OFF_DynMat->SetVectorParameterValue("BaseColor", NewColor);
}
