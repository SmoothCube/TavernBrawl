// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrawlerMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAVERNBRAWL_API UBrawlerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBrawlerMovementComponent();

	// Called when the game starts
	virtual void BeginPlay() override;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetInputVector(FVector inV) { InputVector = inV; }

	UPROPERTY(EditAnywhere)
		float MaxSpeed = 2500;

	bool bMoveIsAllowed = true;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* AccelerationCurve;
private:
	FVector GetVelocity() { return PrevVelocity; }
	FVector PrevVelocity{ 0,0,0 };
	FVector InputVector;
	UPROPERTY(EditAnywhere)
		float AccelerationConst{ 60 };

	UPROPERTY(EditAnywhere)
		float DecelerationConst{ 50 };

	float TurningBoost;

	void MoveActor(float DeltaTime);
	FVector CalculateVelocity();

	AActor* Owner = nullptr;
};
