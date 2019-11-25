// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSubsystem.h"
#include "ConfigCacheIni.h"

void UScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GConfig->LoadFile(GGameIni);

	FString test = "GameMode";
	GConfig->GetInt(
		*test,
		TEXT("Health"),
		Health,
		GGameIni
	);
}

void UScoreSubsystem::Deinitialize()
{
}

int UScoreSubsystem::GetHealth()
{
	return Health;
}

void UScoreSubsystem::SetHealth(int Value)
{
	Health = Value;
}

void UScoreSubsystem::DecrementHealth()
{
	Health -= 1;
	if (Health <= 0)
		Health = 0;
	UE_LOG(LogTemp, Warning, TEXT("Took one damage %i"), Health);
	OnHealthTaken.Broadcast(Health);
	if (Health == 0)
	{
		OnZeroHealth.Broadcast();
	}
}
