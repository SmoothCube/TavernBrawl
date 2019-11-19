// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "BrawlPlayerController.h"
#include "Character/PickupComponent.h"
#include "Character/PunchComponent.h"

const FName ABrawlCharacter::MoveForwardBinding("MoveForward");
const FName ABrawlCharacter::MoveRightBinding("MoveRight");
const FName ABrawlCharacter::RotateForwardBinding("FireForward");
const FName ABrawlCharacter::RotateRightBinding("FireRight");

// Sets default values
ABrawlCharacter::ABrawlCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PunchSphere = CreateDefaultSubobject<USphereComponent>("PunchSphere");
	PunchSphere->SetupAttachment(GetMesh(), "sphereCollisionHere");

	PickupSphere = CreateDefaultSubobject<USphereComponent>("PickupSphere");
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetSphereRadius(144);

	PickupComponent = CreateDefaultSubobject<UPickupComponent>("Pickup Component");
	PunchComponent = CreateDefaultSubobject<UPunchComponent>("Punch Component");
}

// Called when the game starts or when spawned
void ABrawlCharacter::BeginPlay()
{
	Super::BeginPlay();
	PunchSphere->OnComponentBeginOverlap.AddDynamic(PunchComponent, &UPunchComponent::OnOverlapBegin);
	PunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InitialRelativeMeshLocation = GetMesh()->RelativeLocation;
	InitialRelativeMeshRotation = GetMesh()->RelativeRotation;
	BrawlPlayerController = Cast<ABrawlPlayerController>(GetController());
	if(!BrawlPlayerController) 
	{
		UE_LOG(LogTemp, Error, TEXT("[ABrawlCharacter::BeginPlay] No Player Controller for player %s"), *GetNameSafe(this));
	}

	//Pickup Events
	PickupSphere->OnComponentBeginOverlap.AddDynamic(PickupComponent, &UPickupComponent::OnOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(PickupComponent, &UPickupComponent::OnOverlapEnd);
}

// Called every frame
void ABrawlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This is a temporary fix for our casting issue
	if (!BrawlPlayerController)
		BrawlPlayerController = Cast<ABrawlPlayerController>(GetController());


	HandleMovementInput(DeltaTime);
	HandleRotationInput();
}

USphereComponent* ABrawlCharacter::GetPunchSphere()
{
	return PunchSphere;
}

// Called to bind functionality to input
void ABrawlCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(RotateForwardBinding);
	PlayerInputComponent->BindAxis(RotateRightBinding);

	PlayerInputComponent->BindAction("Punch", IE_Pressed, PunchComponent, &UPunchComponent::PunchButtonPressed);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, PickupComponent, &UPickupComponent::OnPickupOrThrowTrigger);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, PickupComponent, &UPickupComponent::ReleaseHoldingItem);
}

void ABrawlCharacter::HandleMovementInput(float DeltaTime)
{
	FVector InputVector(GetInputAxisValue(MoveForwardBinding), GetInputAxisValue(MoveRightBinding), 0);

	if (!bHasFallen)
	{
		if (InputVector.SizeSquared() > 1.f)
		{
			InputVector.Normalize();
		}

		if (InputVector.Size() > 0.0001f)
		{
			FallVector = InputVector;// +FVector::DotProduct(FallVector.GetSafeNormal(), InputVector.GetSafeNormal()) * BrakeMultiplier;
		}
		if (FallVector.Size() < StopSpeed)
		{
			FallVector = FVector(0);
		}

		if (GetVelocity().Size() > GetMovementComponent()->GetMaxSpeed() * 0.9)
		{
			CurrentFallTimer += DeltaTime;

			if (CurrentFallTimer > TimeBeforeFall && !GetMovementComponent()->IsFalling())
			{
				Fall();
			}
			else if (CurrentFallTimer > TimeBeforeFall * 0.3)
			{
				float Strength = CurrentFallTimer / TimeBeforeFall;
				if (BrawlPlayerController)
					BrawlPlayerController->PlayDynamicForceFeedback(Strength, 0.1f, true, true, true, true);
			}
		}
		else
		{
			CurrentFallTimer = 0.f;
		}

		FallVector.GetClampedToMaxSize(1);
		GetMovementComponent()->AddInputVector(FallVector);
	}
}

void ABrawlCharacter::HandleRotationInput()
{
	FVector RotationVector(GetInputAxisValue(RotateForwardBinding), GetInputAxisValue(RotateRightBinding), 0);
	RotationVector.Normalize(RotationTiltCutoff);
	if (!PunchComponent->IsPunching() && !RotationVector.IsNearlyZero(RotationTiltCutoff))
	{
		SetActorRotation(RotationVector.Rotation());
		PrevRotationVector = RotationVector;
	}
	else
	{
		SetActorRotation(PrevRotationVector.Rotation());
	}
}

void ABrawlCharacter::Fall()
{
	//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Fall] Player Fell: %s"), *GetNameSafe(this));
	FVector Velocity = GetVelocity();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);

	GetWorld()->GetTimerManager().SetTimer(
		TH_FallHandle,
		this,
		&ABrawlCharacter::GetUp,
		RecoveryTime,
		false);
	bHasFallen = true;
	GetMesh()->AddForce(Velocity, "ProtoPlayer_BIND_Head_JNT_center");
	if (BrawlPlayerController)
		BrawlPlayerController->PlayDynamicForceFeedback(1, 0.5, true, true, true, true);
	GetMovementComponent()->Velocity = FVector(0);
	GetMovementComponent()->StopMovementImmediately();
	FallVector = FVector(0);
}

void ABrawlCharacter::GetUp()
{
	if (!bIsDead)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::GetUp] Player Getting up: %s"), *GetNameSafe(this));

		//Note that if this component is currently attached to something, beginning simulation will detach it.
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetRelativeTransform().GetLocation());
		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
		GetMesh()->AttachToComponent(GetRootComponent(), Rule);
		GetMesh()->SetRelativeLocationAndRotation(InitialRelativeMeshLocation, InitialRelativeMeshRotation);
		AddActorWorldOffset(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		bHasFallen = false;
	}
}

FRotator ABrawlCharacter::GetPrevRotation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Actor Rotation: %s"), *PrevRotationVector.Rotation().ToString());
	return PrevRotationVector.Rotation(); //works but will always be delayed by a frame.. ugly
}

void ABrawlCharacter::FellOutOfWorld(const UDamageType& DmgType)
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::FellOutOFWorld]: %s Fell Out Of World!"), *GetNameSafe(this));
	GetMovementComponent()->Velocity = FVector(0, 0, GetMovementComponent()->Velocity.Z);
	FHitResult hit;
	TArray<AActor*> Starts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
	int r = FMath::RandRange(0, Starts.Num()-1);

	FVector RespawnLoc = Starts[r]->GetActorLocation() + FVector(0.f,0.f,2500.f);
	SetActorLocation(RespawnLoc,false, &hit, ETeleportType::TeleportPhysics);

}
