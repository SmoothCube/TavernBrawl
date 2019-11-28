// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableItem.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class ABrawlCharacter;

UCLASS()
class TAVERNBRAWL_API AThrowableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableItem();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* PunchCapsule = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* ThrowCapsule = nullptr;

	UFUNCTION()
	void OnPunchOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnThrowOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetHoldingPlayer(ABrawlCharacter* newPlayer);

	ABrawlCharacter* GetHoldingPlayer();

	void SetThrowCollisionDisabled();

	FTimerHandle TH_ThrowHandle;

	UPROPERTY(EditDefaultsOnly)
		float ThrowTimer = 5.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ABrawlCharacter* HoldingPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
		float MinFallSpeed = 100.f;


	
};
