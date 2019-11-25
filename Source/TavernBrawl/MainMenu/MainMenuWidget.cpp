// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "ConfigCacheIni.h"

void UMainMenuWidget::UpdateConfig()
{
	FString test = "GameMode";
	GConfig->SetInt(
		*test,
		TEXT("NumberOfPlayers"),
		NumberOfPlayers,
		GGameIni
	);

	GConfig->SetInt(
		*test,
		TEXT("Health"),
		Health,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);
}
