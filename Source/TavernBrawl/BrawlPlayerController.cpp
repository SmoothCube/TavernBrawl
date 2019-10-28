// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlPlayerController.h"
#include "BrawlPlayer.h"

#include "Engine/World.h"


void ABrawlPlayerController::RespawnPlayer()
{
	FTransform tempLoc = GetPawn()->GetActorTransform();
	GetPawn()->Destroy();

	if (ActorToSpawn)
	{
		ABrawlPlayer* newPlayer = GetWorld()->SpawnActor<ABrawlPlayer>(ActorToSpawn, tempLoc.GetLocation(), FRotator(tempLoc.GetRotation()));
		//SetPawn(newPlayer);
		newPlayer->SetOwner(this);
		
		Possess(newPlayer);
		UE_LOG(LogTemp, Warning, TEXT("[ABrawlPlayerController::RespawnPlayer] new player lifespan: %f"), newPlayer->GetLifeSpan());
		//UE_LOG(LogTemp, Warning, TEXT("[ABrawlPlayerController::RespawnPlayer] Is new player controlled: %i"), newPlayer->IsPlayerControlled());
	}
	//FActorSpawnParameters SpawnInfo;
	//GetWorld()->SpawnActor<ABrawlPlayer>(tempLoc.GetLocation(), FRotator(tempLoc.GetRotation()), SpawnInfo);
	//ABrawlPlayer* newPlayer = GetWorld()->SpawnActor<ABrawlPlayer>(tempLoc.GetLocation(), FRotator(tempLoc.GetRotation()));
	//SetPawn(newPlayer);
	//newPlayer->SetupPlayerInputComponent(InputComponent);


}