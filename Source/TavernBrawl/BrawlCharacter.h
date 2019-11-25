// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/PunchComponent.h"
#include "BrawlCharacter.generated.h"

class USphereComponent;
class USoundBase;
class ABrawlPlayerController;

UCLASS()
class TAVERNBRAWL_API ABrawlCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABrawlCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	USphereComponent* GetPunchSphere();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FRotator GetPrevRotation();

	virtual void FellOutOfWorld(const class UDamageType& DmgType) override;
	void Fall();
	void GetDamaged();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayControllerVibration(float strength);

	//How much you have to tilt the right stick before the character rotates to that direction
	void HandleMovementInput(float DeltaTime);
	void HandleRotationInput();

	void GetUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PunchSphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PickupSphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPickupComponent* PickupComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPunchComponent* PunchComponent = nullptr;
	
	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName RotateForwardBinding;
	static const FName RotateRightBinding;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float RecoveryTime= 0.5;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float MaxLeanValue = 90.f;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float BrakeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float RotationTiltCutoff = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Variables")
	float StopSpeed= 0.1f;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float TimeBeforeFall = 5.f;  //TODO: find a better name for this, it doesnt have anything to do with time

	UPROPERTY(EditAnywhere, Category = "Audio")
	TArray<USoundBase*> HitSounds;


	FVector FallVector{ 0.f,0.f,0.f };
	FVector PrevRotationVector{ 1.f,0.f,0.f };

	ABrawlPlayerController* BrawlPlayerController = nullptr;

	float CurrentFallTimer = 0.f;
	
	FTimerHandle TH_FallHandle;

	bool bIsDead = false;
	bool bHasFallen = false;

	FVector InitialRelativeMeshLocation;
	FRotator InitialRelativeMeshRotation;

	friend class UPunchComponent;
};
