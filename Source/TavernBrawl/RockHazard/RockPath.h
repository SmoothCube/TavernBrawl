// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RockPath.generated.h"
class USplineComponent;
class ARollingRock;
UCLASS()
class TAVERNBRAWL_API ARockPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnRock();


	UPROPERTY(VisibleAnywhere)
	USplineComponent* Path;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARollingRock> RockClass;

private:
	TArray<ARollingRock*> ActiveRocks;



};
