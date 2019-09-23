// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TavernBrawlGameMode.h"
#include "TavernBrawlPawn.h"

ATavernBrawlGameMode::ATavernBrawlGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ATavernBrawlPawn::StaticClass();
}

