// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisSystems/TetrisRender_v2.h"
#include "TetrisRender_2_TestActor.generated.h"

UCLASS()
class TETRISP_4_API ATetrisRender_2_TestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrisRender_2_TestActor();

	UPROPERTY(EditAnywhere)
	UTetrisRender_v2* TetrisRender;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
