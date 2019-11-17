// Fill out your copyright notice in the Description page of Project Settings.

#include "BrawlPlayerController.h"


void ABrawlPlayerController::FellOutOfWorld(const UDamageType& DmgType)
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::FellOutOFWorld]: %s Fell Out Of World!"), *GetNameSafe(this));
}