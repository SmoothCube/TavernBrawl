// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BrawlGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TAVERNBRAWL_API ABrawlGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
		virtual void BeginPlay() override;
protected:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "C++")
	int NumberOfPlayers = 2;

};
