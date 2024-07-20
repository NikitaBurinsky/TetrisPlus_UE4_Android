// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisSystems/Test/TetrisRender_2_TestActor.h"

// Sets default values
ATetrisRender_2_TestActor::ATetrisRender_2_TestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TetrisRender = CreateDefaultSubobject<UTetrisRender_v2>("RenderTest");

}

// Called when the game starts or when spawned
void ATetrisRender_2_TestActor::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ATetrisRender_2_TestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

