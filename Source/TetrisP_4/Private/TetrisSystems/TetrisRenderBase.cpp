
#include "TetrisSystems/TetrisRenderBase.h"
#include "Tetris_Core.h"



UTetrisRenderBase::UTetrisRenderBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UTetrisRenderBase::BeginPlay()
{
	Super::BeginPlay();
}


