// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrawlPlayerController.h"
#include "CameraFocusActor.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class ABrawlCharacter;

UCLASS()
class TAVERNBRAWL_API ACameraFocusActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraFocusActor();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UFUNCTION(BlueprintCallable)
	void SetupCamera();
	
	UFUNCTION()
		void RemovePlayer(ABrawlCharacter* Player);

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetSpringArmLength(float distanceToFurthestPlayer);


	TArray<ABrawlCharacter*> Players;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		float SmallestSpringArmLength = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		float LargestSpringArmLength = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float BorderWidth = 200.0f;
};
