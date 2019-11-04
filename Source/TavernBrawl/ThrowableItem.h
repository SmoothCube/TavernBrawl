// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableItem.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;

UCLASS()
class TAVERNBRAWL_API AThrowableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableItem();

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule = nullptr;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
