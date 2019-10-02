// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlPlayer.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"

#include "BrawlerMovementComponent.h"

const FName ABrawlPlayer::MoveForwardBinding("MoveForward");
const FName ABrawlPlayer::MoveRightBinding("MoveRight");
const FName ABrawlPlayer::FireForwardBinding("FireForward");
const FName ABrawlPlayer::FireRightBinding("FireRight");


// Sets default values
ABrawlPlayer::ABrawlPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	SetRootComponent(Capsule);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	Mesh->SetupAttachment(Capsule);
	MovementComponent = CreateDefaultSubobject<UBrawlerMovementComponent>("MovementComponent");
}

// Called when the game starts or when spawned
void ABrawlPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrawlPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);

	MovementDirection = FVector(ForwardValue, RightValue, 0);
	MovementDirection.Normalize();

	FVector RotationDirection = FVector(FireForwardValue, FireRightValue, 0);
	RotationDirection.Normalize();
	if (MovementComponent)
	{
		MovementComponent->SetInputVector(MovementDirection);
		MovementComponent->SetRotationVector(RotationDirection);

	}
	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	//// Calculate  movement
	//const FVector Movement = MoveDirection * 1000 * DeltaTime;

	//// If non-zero size, move this actor
	//if (Movement.SizeSquared() > 0.0f)
	//{
	//	const FRotator NewRotation = Movement.Rotation();
	//	FHitResult Hit(1.f);
	//	RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

	//	if (Hit.IsValidBlockingHit())
	//	{
	//		const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
	//		const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
	//		RootComponent->MoveComponent(Deflection, NewRotation, true);
	//	}
	//}

}


// Called to bind functionality to input
void ABrawlPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}


float ABrawlPlayer::GetSpeed()
{ 
	return MovementComponent->GetSpeed(); 
}
