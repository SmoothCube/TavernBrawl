// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSubsystem.h"

void UScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
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

	OnHealthTaken.Broadcast(Health);
	if (Health == 0)
	{
		OnZeroHealth.Broadcast();
	}
}
