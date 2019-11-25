// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPathManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "RockPath.h"
// Sets default values
ARockPathManager::ARockPathManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARockPathManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		TH_RockTimer,
		this,
		&ARockPathManager::SpawnRock,
		RockDelay,
		true);
}

void ARockPathManager::SpawnRock()
{
	int index =  FMath::RandRange(0, Paths.Num() - 1);
	Paths[index]->SpawnRock();

}

