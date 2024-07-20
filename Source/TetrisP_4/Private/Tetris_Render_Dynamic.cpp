// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris_Render_Dynamic.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerPawn.h"
#include "Tetris_Core.h"
#define LocationMultiplier 200 //используется в ToLocation() (Потом переведу в Property)

// Sets default values
UTetris_Render_Dynamic::UTetris_Render_Dynamic()
{
	PrimaryComponentTick.bCanEverTick = false;
	for (int i = 0; i < 4; ++i)
		FallingRenderedBlocks[i] = nullptr;

	TetrisCore = Cast<ATetris_Core>(GetOwner());
}



void UTetris_Render_Dynamic::ChangeFallingBlocksCoordinates(FVector2D *NewCoordinates)
{
	if (!NewCoordinates)
	{
		return;
	}
	UE_LOG(Tetris_Render_Log,Display , TEXT("ChangeFallingBlocksCoordinates : Start"));
	FVector NewLoc;
	for (int i = 0; i < 4; ++i)
	{
		NewLoc = ToLocation(NewCoordinates[i]);

		FallingRenderedBlocks[i]->SetActorLocation(NewLoc);
	}
	UE_LOG(Tetris_Render_Log, Display, TEXT("ChangeFallingBlocksCoordinates : End"));
}

void UTetris_Render_Dynamic::MakeFallingFigureStatic()
{
	for (int i = 0; i < 4; ++i)
	{
		StaticRenderedBlocks.Add(FallingRenderedBlocks[i]);
		FallingRenderedBlocks[i] = nullptr;
	}
}

void UTetris_Render_Dynamic::UpdateStaticBlocks_RowDestroy(int DelRow)
{
	UE_LOG(Tetris_Render_Log, Display, TEXT("UpdateStaticBlocks_RowDestroy : Start"));
	for (int i = StaticRenderedBlocks.Num() - 1; i >= 0; --i)
	{
		FVector2D temp(DelRow, 0);
		if (StaticRenderedBlocks[i]->GetActorLocation().Z == ToLocation(temp).Z)
		{
			StaticRenderedBlocks[i]->Destroy();
			StaticRenderedBlocks[i] = nullptr;
		}
		else if (StaticRenderedBlocks[i]->GetActorLocation().Z > ToLocation(temp).Z)
		{
			StaticRenderedBlocks[i]->AddActorWorldOffset(FVector(0, 0, -LocationMultiplier));
		}
	}
	StaticRenderedBlocks.Remove(nullptr);
	UE_LOG(Tetris_Render_Log,Display , TEXT("UpdateStaticBlocks_RowDestroy : Finish"));
}

FVector UTetris_Render_Dynamic::ToLocation(FVector2D Coords)
{
	FVector FV;
	FV.Y = Coords.Y * LocationMultiplier;
	FV.X = 0;
	FV.Z = (20 - Coords.X) * LocationMultiplier;
	if (TetrisCore)
		FV += TetrisCore->GetActorLocation();
	return FV;
}

void UTetris_Render_Dynamic::CreateNewRenderedFigure(FVector2D *Coords)
{
	UE_LOG(Tetris_Render_Log, Display, TEXT("CreateNewRenderedFigure : Start"));
	for (int i = 0; i < 4; ++i)
	{
		if (FallingRenderedBlocks[i] != nullptr)
		{
			UE_LOG(Tetris_Render_Log,Error , TEXT("CreateNewRenderedFigure : Canceled - FallingRenderedBlocks[%i] is not Empty"), i);
			return;
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		FallingRenderedBlocks[i] = GetWorld()->SpawnActor<AStaticMeshActor>(ToLocation(Coords[i]), FRotator(0,0,0));
		FallingRenderedBlocks[i]->SetMobility(EComponentMobility::Movable);
		FallingRenderedBlocks[i]->SetActorRelativeRotation(ExampleRotator);
		FallingRenderedBlocks[i]->SetActorScale3D(ExampleScale);
		FallingRenderedBlocks[i]->GetStaticMeshComponent()->SetStaticMesh(ExampleSM);
		FallingRenderedBlocks[i]->GetStaticMeshComponent()->SetVisibility(true);
	}
	UE_LOG(Tetris_Render_Log,Display , TEXT("CreateNewRenderedFigure : End"));
}

void UTetris_Render_Dynamic::BeginPlay()
{
	UActorComponent::BeginPlay();
}

/// DESTROY | LOSING ///
void UTetris_Render_Dynamic::StartDestroyRenderedBlocks()
{
	if (StaticRenderedBlocks.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTetris_Render_Dynamic::StartDestroyRenderedBlocks);
		StaticRenderedBlocks.Last()->Destroy();
		StaticRenderedBlocks.RemoveAt(StaticRenderedBlocks.Num() - 1);
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			if(FallingRenderedBlocks != nullptr)
			FallingRenderedBlocks[i]->Destroy();
			FallingRenderedBlocks[i] = nullptr;
		}
		Cast<APlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn())->Tetris->ProcessDesroying = false;
	}
}