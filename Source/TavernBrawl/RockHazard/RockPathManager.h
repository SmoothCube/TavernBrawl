// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RockPathManager.generated.h"
class ARockPath;
//class FTimerHandle;
UCLASS()
class TAVERNBRAWL_API ARockPathManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockPathManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<ARockPath*> Paths;
	//FTimerHandle TH_RockTimer;

};
