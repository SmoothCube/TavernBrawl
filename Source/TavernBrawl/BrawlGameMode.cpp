// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlGameMode.h"
#include "Engine/GameInstance.h" 
#include "BrawlGameInstance.h"

void ABrawlGameMode::BeginPlay()
{

	UBrawlGameInstance* GameInstance = Cast<UBrawlGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABrawlGameMode::BeginPlay] Can't find the game instance"));
		return;
	}

	NumberOfPlayers = GameInstance->NumberOfPlayers;
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlGameMode::BeginPlay] Jeg kom 2"));
	Super::BeginPlay();
}