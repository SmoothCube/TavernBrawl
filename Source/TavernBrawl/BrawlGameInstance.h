// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BrawlGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TAVERNBRAWL_API UBrawlGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int NumberOfPlayers = 2;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int Health = 5;
};
