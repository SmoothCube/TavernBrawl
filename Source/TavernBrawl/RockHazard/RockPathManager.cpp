// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPathManager.h"

// Sets default values
ARockPathManager::ARockPathManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARockPathManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockPathManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

