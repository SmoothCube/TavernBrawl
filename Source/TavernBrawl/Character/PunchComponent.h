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

	UFUNCTION()
	void Punch();

	UFUNCTION(BlueprintCallable)
	bool IsPunching() { return bIsPunching; }

protected:	

	void PunchEnd();

	UPROPERTY(EditAnywhere, Category = "Variables")
		float PunchLength = 0.3;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float PunchWaitingTime = 0.1;

	UFUNCTION(BlueprintCallable)
		void GetPunched(FVector punchStrength);

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		UCurveFloat* DashLengthCurve;

	UFUNCTION()
	void KillCharacter();
	
	virtual void BeginPlay() override;

	void setIsPunchingFalse() { bIsPunching = false; }

	ABrawlCharacter* Player = nullptr;
	bool bIsPunching = false;
	bool bAssignedEvent = false;

	float NormalMaxWalkSpeed = 0.f;

	FTimerHandle TH_PunchHandle;
	FTimerHandle TH_PunchAgainHandle; // want to wait a sec after punch has ended before u can punch again!!
};