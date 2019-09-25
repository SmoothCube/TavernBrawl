// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BrawlPlayer.generated.h"

UCLASS()
class TAVERNBRAWL_API ABrawlPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABrawlPlayer();

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBrawlerMovementComponent* MovementComponent = nullptr;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
