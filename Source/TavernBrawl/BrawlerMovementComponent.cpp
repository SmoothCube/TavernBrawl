// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerMovementComponent.h"

#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
// Sets default values for this component's properties
UBrawlerMovementComponent::UBrawlerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrawlerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	// ...
	
}


// Called every frame
void UBrawlerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MoveActor(DeltaTime);

	AccelerationConst = AccelerationCurve->GetFloatValue(PrevVelocity.Size());
	UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::TickComponent] Velocity: %f, AccelerationConst: %f"), PrevVelocity.Size(), AccelerationConst);
	// ...
}


void UBrawlerMovementComponent::MoveActor(float DeltaTime)
{
	if (Owner)
	{
		Owner->SetActorLocation(Owner->GetActorLocation() + CalculateVelocity() * DeltaTime, true);
	}
}

FVector UBrawlerMovementComponent::CalculateVelocity()
{

	FVector Acceleration = InputVector * AccelerationConst;
	FVector Velocity = PrevVelocity + Acceleration - (PrevVelocity / DecelerationConst);
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
	PrevVelocity = Velocity;

	//TODO bytt til fvector, bruk ClampToMaxSize for å slippe if-testen



	return Velocity;
}

