// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BrawlCharacter.generated.h"
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FVector FindLeanVector();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayControllerVibration(float strength);

	UFUNCTION(BlueprintCallable)
	void GetPunched(FVector punchStrength);


	UFUNCTION(BlueprintCallable)
	bool IsPunching() { return bIsPunching; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PunchSphere = nullptr;

private:
	
	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName RotateForwardBinding;
	static const FName RotateRightBinding;

	//How much you have to tilt the right stick before the character rotates to that direction
	void HandleMovementInput(float DeltaTime);
	void HandleRotationInput();
	void Fall();
	void GetUp();
	void Punch();
	void PunchEnd();

	UFUNCTION()
	void OnPunchSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Variables")
	float RecoveryTime= 0.5;

	UPROPERTY(EditAnywhere, Category = "Variables")
	float PunchLength = 0.3;

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
	
	
	FVector PrevRotationVector{ 0.f,0.f,0.f };
	FVector FallVector{ 0.f,0.f,0.f };
	

	bool bIsPunching = false;
	bool bHasFallen = false;

	FTimerHandle TH_FallHandle;
	FTimerHandle TH_PunchHandle;

	ABrawlPlayerController *BrawlPlayerController = nullptr;

	FVector InitialRelativeMeshLocation;
	FRotator InitialRelativeMeshRotation;
};
