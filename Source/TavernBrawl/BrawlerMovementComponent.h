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

	UFUNCTION(BlueprintCallable)
	FVector GetVelocity() { return PrevVelocity; }


	UFUNCTION(BlueprintCallable)
	FVector GetRotation() { return RotationVector; }
	
	float GetSpeed() { return PrevVelocity.Size(); }

	void SetInputVector(FVector inV) { InputVector = inV; }
	void SetRotationVector(FVector inV) { RotationVector = inV.GetSafeNormal(); }

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 1000;

	UPROPERTY(EditAnywhere)
	float RecoveryTime = 1.0f;

	bool bMoveIsAllowed = true;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* AccelerationCurve;

	UFUNCTION(BlueprintCallable)
	void Fall(FVector& Velocity);
private:
	FVector PrevVelocity{ 0,0,0 };
	FVector InputVector;
	FVector RotationVector;

	UPROPERTY(EditAnywhere)
	float AccelerationConst{ 60 };

	UPROPERTY(EditAnywhere)
	float DecelerationConst{ 50 };

	float TurningBoost;
	bool bAllowedToMove = true;

	void MoveActor(float DeltaTime);
	FVector CalculateVelocity();
	


	void GetUp();

	class ABrawlPlayer* Owner = nullptr;

	FTimerHandle TH_FallHandle;
};
