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

		UFUNCTION(BlueprintCallable, Category = "Variables")
		void DecrementHealth();

		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthTaken, int32, Amount);
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZeroHealth);

		UPROPERTY(BlueprintAssignable, Category = ScoreSubsystem)
			FHealthTaken OnHealthTaken;

		UPROPERTY(BlueprintAssignable, Category = ScoreSubsystem)
			FZeroHealth OnZeroHealth;
	
private:

	int Health = 5;
};
