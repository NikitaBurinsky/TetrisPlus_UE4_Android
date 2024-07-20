// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisSystems/TetrisRender_v2.h"
#include "PlayerPawn.h"
#include "Tetris_Core.h"
#include <exception>

// Sets default values for this component's properties
UTetrisRender_v2::UTetrisRender_v2()
{
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("TetrisRender_2 Constructor : Start"));
	PrimaryComponentTick.bCanEverTick = false;

	TetrisCore = Cast<ATetris_Core>(GetOwner());

	UE_LOG(Tetris_Render_2_Log, Display, TEXT("TetrisRender_2 Constructor : Success"));

}

void UTetrisRender_v2::BeginPlay()
{
	UActorComponent::BeginPlay();

	CreateField();
	SetFieldVisibility(false);
}

void UTetrisRender_v2::CreateField()
{
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("Create Field : Start"));
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("Create Field : Spawning Actors"));
	for (int r = 0; r < Rows; ++r)
	{
		RenderField.Add(TArray<AStaticMeshActor*>());
		for (int c = 0; c < Collumns; ++c)
		{
			AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>();
			Block->GetStaticMeshComponent()->SetStaticMesh(ExampleActor);
			Block->SetMobility(EComponentMobility::Movable);
			Block->SetActorScale3D(ExampleScale);
			Block->SetActorRelativeRotation(ExampleRotation);
			Block->SetActorLocation(ToLocation(FVector2D(r, c)));
			RenderField[r].Add(Block);
		}

		UE_LOG(Tetris_Render_2_Log, Display, TEXT("Create Field : Row Spawned : %i"), r);
	}
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("Create Field : Startup Render Materials"));

	StartUpRenderMaterials();

	UE_LOG(Tetris_Render_2_Log, Display, TEXT("Create Field : Success"));

}

void UTetrisRender_v2::SetFieldVisibility(bool IsVisibleField)
{
	for (int r = 0; r < Rows; ++r)
	{
		for (int c = 0; c < Collumns; ++c)
		{
			SetBlockInUse(r, c, IsVisibleField);
		}
	}
}


// ---------------------------------------------------

void UTetrisRender_v2::CreateNewRenderedFigure(FVector2D* Coords)
{
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("CreateNewRenderfigure : Start"));
	for (int i = 0; i < 4; ++i)
	{
		if (Coords + i)
			UE_LOG(Tetris_Render_2_Log, Display, TEXT("Checking pointer to Coords Array : %i/4"), i + 1);
	}
	
		for (int i = 0; i < 4; ++i)
		{
			CurrentCoords[i] = Coords[i];
			SetBlockInUse(CurrentCoords[i].X, CurrentCoords[i].Y, true);
			UE_LOG(Tetris_Render_2_Log, Display, TEXT("CurrentRenderFigureBlockLocation Set : %i - %s"), i, *Coords[i].ToString());
		}
		UE_LOG(Tetris_Render_2_Log, Display, TEXT("CreateNewRenderfigure : Success"));
}

void UTetrisRender_v2::ChangeFallingBlocksCoordinates(FVector2D* NewCoordinates)
{
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("Change Falling Block Coords : Start"));

	for (int i = 0; i < 4; ++i)
	{
		SetBlockInUse(CurrentCoords[i].X, CurrentCoords[i].Y, false);
	}

	for (int i = 0; i < 4; ++i)
	{
		SetBlockInUse(NewCoordinates[i].X, NewCoordinates[i].Y, true);

		CurrentCoords[i].X = NewCoordinates[i].X;
		CurrentCoords[i].Y = NewCoordinates[i].Y;

	}

	UE_LOG(Tetris_Render_2_Log, Display, TEXT("Change Falling Block Coords : End"));
}

void UTetrisRender_v2::UpdateStaticBlocks_RowDestroy(int DelRow)
{
	for (int c = 0; c < Collumns; ++c)
	{
		SetBlockInUse(DelRow, c, false);
	}

	//Update all static blocks on destroyed blocks 
	for (int r = DelRow; r > 0; --r)
	{
		for (int c = 0; c < Collumns; ++c)
		{
			if(IsBlockEmpty(r,c) && !IsBlockEmpty(r-1,c))
			{
				SetBlockInUse(r, c, true);
				SetBlockInUse(r - 1, c, false);
			}
		}
	}
}

void UTetrisRender_v2::MakeFallingFigureStatic()
{
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("MakeFallingFigureStatic : Succeed"));
}

FVector UTetrisRender_v2::ToLocation(FVector2D Coords)
{
	FVector FV;
	FV.Y = Coords.Y * LocationMultiplier;
	FV.X = 0;
	FV.Z = (20 - Coords.X) * LocationMultiplier;
	if (TetrisCore)
		FV += TetrisCore->GetActorLocation();
	return FV;
}

inline void UTetrisRender_v2::SetBlockInUse(int RowInd, int ColInd, bool IsUsed)
{
	if(RowInd > -1 && RowInd < Rows && ColInd > -1 && ColInd < Collumns)
	{
		if (IsUsed)
		{
				UStaticMeshComponent* TempSMC = RenderField[RowInd][ColInd]->GetStaticMeshComponent();
				TempSMC->GetMaterial(0);
				TempSMC->SetMaterial(0, FilledBlockMaterial);
		}
		else
		{
			if (RenderField[RowInd][ColInd] && EmptyBlockMaterial)
				RenderField[RowInd][ColInd]->GetStaticMeshComponent()->SetMaterial(0, EmptyBlockMaterial);
		}
	}
}

inline bool UTetrisRender_v2::IsBlockEmpty(int RowInd, int ColInd)
{
	return RenderField[RowInd][ColInd]->GetStaticMeshComponent()->GetMaterial(0) != FilledBlockMaterial;
}



/// SETTINGS SETS
void UTetrisRender_v2::ChangeBlocksColor(FLinearColor& NewColor)
{
	FilledBlockMaterial->SetVectorParameterValue("BaseColor", NewColor);
}

void UTetrisRender_v2::ChangeEmptyBlocksColor(FLinearColor& NewColor)
{
	EmptyBlockMaterial->SetVectorParameterValue("BaseColor", NewColor);
}

void UTetrisRender_v2::StartUpRenderMaterials()
{
	UMaterialInterface* DefMaterial = RenderField[0][0]->GetStaticMeshComponent()->GetMaterial(0);
	EmptyBlockMaterial = UMaterialInstanceDynamic::Create(DefMaterial, this);
	FilledBlockMaterial = UMaterialInstanceDynamic::Create(DefMaterial, this);
	FilledBlockMaterial->SetVectorParameterValue("BaseColor", FilledBlocksDefaultColor);
	EmptyBlockMaterial->SetVectorParameterValue("BaseColor", EmptyBlocksDefaultColor);

	for (int r = 0; r < Rows; ++r)
	{
		for (int c = 0; c < Collumns; ++c)
		{
			RenderField[r][c]->GetStaticMeshComponent()->SetMaterial(0, EmptyBlockMaterial);
		}
	}
	APlayerPawn* PP = Cast<APlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PP->GetMenuSettings()->BindMaterialForColorSettings("Blocks:Empty", EmptyBlockMaterial); 
	PP->GetMenuSettings()->BindMaterialForColorSettings("Blocks:Filled", FilledBlockMaterial);
}
///



void UTetrisRender_v2::StartDestroyRenderedBlocks()
{
	UE_LOG(Tetris_Render_2_Log, Display, TEXT("Destroying Field : Start"));

	CurDelR = Rows - 1;

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTetrisRender_v2::ClearField_Anim_Forward);
}

void UTetrisRender_v2::ClearField_Anim_Forward()
{
	if (Delay1Frame)
	{
		Delay1Frame = false;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTetrisRender_v2::ClearField_Anim_Forward);
		return;
	}
	else
	{
		Delay1Frame = true;
	}

	for (int c = 0; c < Collumns; ++c)
	{
		SetBlockInUse(CurDelR, c, true);
	}

	--CurDelR;
	if (CurDelR < 0)
	{
		CurDelR = 0;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTetrisRender_v2::ClearField_Anim_Reverse);
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTetrisRender_v2::ClearField_Anim_Forward);
}

void UTetrisRender_v2::ClearField_Anim_Reverse()
{
	if (Delay1Frame)
	{
		Delay1Frame = false;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTetrisRender_v2::ClearField_Anim_Reverse);
		return;
	}
	else
		Delay1Frame = true;

	for (int c = 0; c < Collumns; ++c)
	{
		SetBlockInUse(CurDelR, c, false);
	}

	++CurDelR;
	if (CurDelR >= Rows)
	{
		CurDelR = Rows - 1;

		TetrisCore->ProcessDesroying = false;
		UE_LOG(Tetris_Render_2_Log, Display, TEXT("Destroying Field : Success"));
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTetrisRender_v2::ClearField_Anim_Reverse);
}

