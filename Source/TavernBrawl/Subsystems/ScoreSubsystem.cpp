// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "BrawlGameInstance.h"

void UScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UBrawlGameInstance* GameInstance = Cast<UBrawlGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABrawlGameMode::BeginPlay] Can't find the game instance"));
		return;
	}

	Health = GameInstance->Health;
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
		UE_LOG(LogTemp, Warning, TEXT("[UScoreSubsystem::DecrementHealth] 0 health!"));

		OnZeroHealth.Broadcast();
	}
}
