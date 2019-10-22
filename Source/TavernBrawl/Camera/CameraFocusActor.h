// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrawlPlayerController.h"
#include "CameraFocusActor.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class ABrawlPlayerController;

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
	

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	TArray<ABrawlPlayerController*> Players;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		float SmallestSpringArmLength = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float BorderWidth = 2.0f;
};
