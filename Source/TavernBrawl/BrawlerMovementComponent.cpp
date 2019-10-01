// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerMovementComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Components/SkeletalMeshComponent.h"

#include "BrawlPlayer.h"
#include "BrawlPlayerController.h"
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

	Owner = Cast<ABrawlPlayer>(GetOwner());
	// ...
	
}


// Called every frame
void UBrawlerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bAllowedToMove && Owner->HasLocalNetOwner())
		MoveActor(DeltaTime);


	//AccelerationConst = AccelerationCurve->GetFloatValue(PrevVelocity.Size());
	//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::TickComponent] Velocity: %f, AccelerationConst: %f"), PrevVelocity.Size(), AccelerationConst);
	// ...
}


void UBrawlerMovementComponent::MoveActor(float DeltaTime)
{
	if (Owner)
	{
		Owner->SetActorLocation(Owner->GetActorLocation() + CalculateVelocity() * DeltaTime, true);
		//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::MoveActor]: Moving Actor"));
	}
}

FVector UBrawlerMovementComponent::CalculateVelocity()
{
	FVector SafeVector = InputVector.GetSafeNormal();
	if (SafeVector.IsNearlyZero())
		SafeVector = Owner->GetActorForwardVector();
	FVector Acceleration = SafeVector * AccelerationConst;
	FVector Velocity = PrevVelocity + Acceleration;
	Owner->SetActorRotation(Velocity.Rotation());
	if (Velocity.Size() >MaxSpeed)
	{
		Fall(Velocity);
		UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: Falling"));
	}

	PrevVelocity = Velocity;

	return Velocity;
}

void UBrawlerMovementComponent::Fall(FVector& Velocity)
{
	Velocity = FVector(0);
	bAllowedToMove = false;
	if (Owner)
	{
		//Owner->SetActorRotation(FRotator(-90, 0, 0));
		Owner->Mesh->SetSimulatePhysics(true);
		Owner->Mesh->WakeAllRigidBodies();
	}
	GetWorld()->GetTimerManager().SetTimer(
		TH_FallHandle,
		this,
		&UBrawlerMovementComponent::GetUp,
		RecoveryTime,
		false);
	UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::Fall]: Falling"));



}

void UBrawlerMovementComponent::GetUp()
{
	if (Owner)
	{

		Cast<ABrawlPlayerController>(Owner->GetController())->RespawnPlayer();
		//Owner->SetSimulatePhysics(false);
		//Owner->Mesh->SetRelativeLocation(Owner->GetActorLocation());
		//Owner->SetActorRotation(FRotator(0,0,0));

	//	Owner->Mesh->PutAllRigidBodiesToSleep();
	//	Owner->Mesh->SetSimulatePhysics(false);
	//	//Owner->Mesh->
	//}
	}
	//bAllowedToMove = true;
	//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::GetUp]: Getting Up"));
}

