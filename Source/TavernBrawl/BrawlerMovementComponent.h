// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrawlerMovementComponent.generated.h"

class UCurveFloat;
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
	
	float GetSpeed() { return SpeedCurve->GetFloatValue(LeanVector.Size()); }

	void SetInputVector(FVector inV) { InputVector = inV; }
	void SetRotationVector(FVector inV) { RotationVector = inV.GetSafeNormal(); }

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 1000;

	UPROPERTY(EditAnywhere)
	float RecoveryTime = 1.0f;

	bool bMoveIsAllowed = true;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* SpeedCurve;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* DecelerationCurve;

	UFUNCTION(BlueprintCallable)
	void Fall(FVector& Velocity);
private:
	FVector PrevVelocity{ 0,0,0 };
	FVector InputVector{ 0,0,0 };
	FVector PrevInputVector{ 0,0,0 };
	FVector RotationVector{ 0,0,0 };
	FVector LeanVector{ 0,0,0 };

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
