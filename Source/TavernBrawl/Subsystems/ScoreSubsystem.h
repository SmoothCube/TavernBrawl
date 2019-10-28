// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "ScoreSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TAVERNBRAWL_API UScoreSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Variables")
		int GetHealth();

		UFUNCTION(BlueprintCallable, Category = "Variables")
		void SetHealth(int Value);
	
private:

	int Health = 5;
};
