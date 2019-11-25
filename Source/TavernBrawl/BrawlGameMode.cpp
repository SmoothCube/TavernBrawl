// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlGameMode.h"
#include "ConfigCacheIni.h"

void ABrawlGameMode::BeginPlay()
{

	UE_LOG(LogTemp, Warning, TEXT("Starting"));


	FString test = "GameMode";
	GConfig->GetInt(
		*test,
		TEXT("NumberOfPlayers"),
		NumberOfPlayers,
		GGameIni
	);

	UE_LOG(LogTemp, Warning, TEXT("%i"), NumberOfPlayers);
	Super::BeginPlay();
}