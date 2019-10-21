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
		Owner->AddActorWorldOffset(CalculateVelocity() * DeltaTime, true);
		
		//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::MoveActor]: Moving Actor"));
	}
}

FVector UBrawlerMovementComponent::CalculateVelocity()
{

	//float Deceleration = 0;
	//if (DecelerationCurve)
	//	Deceleration = DecelerationCurve->GetFloatValue(FMath::Abs((LeanVector.GetSafeNormal() - PrevInputVector).Size()));
	//else
	//	UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: Speed Curve Not Set!"));

	float Speed = 0;
	if (SpeedCurve)
		Speed = SpeedCurve->GetFloatValue(LeanVector.Size());
	else
		UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: Speed Curve Not Set!"));

	//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: DecelerationCurve.Speed! %f"), FMath::Abs((LeanVector.GetSafeNormal() - PrevInputVector).Size()));
	if (InputVector.IsNearlyZero())
	{
		LeanVector -= LeanVector.GetSafeNormal() * DecelerationConst;
		//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: Decelerating! %f"), FMath::Abs((InputVector - PrevInputVector).Size()));
	}
	else
	{
		LeanVector += InputVector;
	}
	//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: Input Vector!"));

	Owner->SetActorRotation(RotationVector.Rotation());


	FVector Velocity = (LeanVector.GetSafeNormal() * Speed);// -LeanVector.GetSafeNormal() * Deceleration;

	//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: Speed: %f, LeanVector: (%f, %f, %f)"), Speed, LeanVector.X, LeanVector.Y, LeanVector.Z);
	//UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: SpeedCurve: %f"), Speed);
	//if (Velocity.SizeSquared() >MaxSpeed*MaxSpeed)
	//{
	//	Velocity = FVector(0);
	//	Owner->MovementDirection = FVector(0);
	//	//Fall(Velocity);
	//	UE_LOG(LogTemp, Warning, TEXT("[UBrawlerMovementComponent::CalculateVelocity]: Falling"));
	//}

	PrevVelocity = Velocity;
	PrevInputVector = InputVector;
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

