// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupComponent.generated.h"

class AThrowableItem;
class ABrawlCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAVERNBRAWL_API UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickupComponent();

	UFUNCTION()
	void OnPickupOrThrowTrigger();

	void PickupHoldingItem();
	void StartThrowingItem();

	AThrowableItem* GetHoldingItem();

	UFUNCTION()
	void ReleaseHoldingItem();

	UFUNCTION(BlueprintCallable)
	void ThrowHoldingItem();

	UFUNCTION(BlueprintPure)
	bool IsHoldingItem();

	UFUNCTION(BlueprintPure)
	bool IsAiming();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	
	virtual void BeginPlay() override;
	TArray<AThrowableItem*> ItemsInRange;
	AThrowableItem* HoldingItem = nullptr;
	ABrawlCharacter* Player = nullptr;

	bool bIsAiming = false;
};
