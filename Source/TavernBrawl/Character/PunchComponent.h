// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PunchComponent.generated.h"

class ABrawlCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAVERNBRAWL_API UPunchComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPunchComponent();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PunchHit(ABrawlCharacter* OtherPlayer);

	UFUNCTION()
	void PunchButtonPressed();

	UFUNCTION(BlueprintCallable)
	void Punch();

	UFUNCTION(BlueprintCallable)
	void PunchWithItem();

	UFUNCTION(BlueprintCallable)
	void PunchWithItemEnd();

	UFUNCTION(BlueprintCallable)
	bool IsPunching() { return bIsPunching; }
	
	UFUNCTION(BlueprintCallable)
	void PunchEnd();
protected:	


	UPROPERTY(EditAnywhere, Category = "Variables")
		float PunchLength = 0.3;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float PunchWaitingTime = 0.1;

	UFUNCTION(BlueprintCallable)
	void GetPunched(FVector punchStrength);

	UPROPERTY(EditDefaultsOnly, Category = "Variables")
	UCurveFloat* DashLengthCurve;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float PunchStrength = 2500.f;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float MinPunchStrengthToFall = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float PunchHitVelocityDamper = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* PunchSound = nullptr;

	UFUNCTION()
	void KillCharacter();
	
	virtual void BeginPlay() override;

	virtual void TickComponent
	(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	void setIsPunchingFalse() { bIsPunching = false; bHasHit = false; }

	ABrawlCharacter* Player = nullptr;
	bool bIsPunching = false;
	bool bHasHit = false;
	bool bAssignedEvent = false;

	float NormalMaxWalkSpeed = 0.f;

	FTimerHandle TH_PunchHandle;
	FTimerHandle TH_PunchAgainHandle; // want to wait a sec after punch has ended before u can punch again!!
};