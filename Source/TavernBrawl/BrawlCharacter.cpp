// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "BrawlPlayerController.h"

const FName ABrawlCharacter::MoveForwardBinding("MoveForward");
const FName ABrawlCharacter::MoveRightBinding("MoveRight");
const FName ABrawlCharacter::RotateForwardBinding("FireForward");
const FName ABrawlCharacter::RotateRightBinding("FireRight");

// Sets default values
ABrawlCharacter::ABrawlCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetRootComponent(GetCapsuleComponent());
	PunchSphere = CreateDefaultSubobject<USphereComponent>("PunchSphere");
	PunchSphere->SetupAttachment(GetMesh(), "sphereCollisionHere");
}

// Called when the game starts or when spawned
void ABrawlCharacter::BeginPlay()
{
	Super::BeginPlay();
	PunchSphere->OnComponentBeginOverlap.AddDynamic(this, &ABrawlCharacter::OnPunchSphereOverlapBegin);
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
}

// Called every frame
void ABrawlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovementInput(DeltaTime);
	HandleRotationInput();
}



// Called to bind functionality to input
void ABrawlCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(RotateForwardBinding);
	PlayerInputComponent->BindAxis(RotateRightBinding);
	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &ABrawlCharacter::Punch);
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
		FallVector.GetClampedToMaxSize(1);
		
		GetMovementComponent()->AddInputVector(FallVector);
		
		float TimeBeforeFallSquared = TimeBeforeFall;	//Just so we can compare to SizeSquared, as this is cheaper
		float FallVectorSizeSquared = FallVector.Size();
		if (FallVectorSizeSquared >= TimeBeforeFallSquared)
		{
			//Fall();
		}
		else if (FallVectorSizeSquared >= TimeBeforeFallSquared / 2)
		{
			//PlayControllerVibration(FallVectorSizeSquared / TimeBeforeFallSquared);
			float Strength = FallVectorSizeSquared / TimeBeforeFallSquared;
			
			if(BrawlPlayerController)
				BrawlPlayerController->PlayDynamicForceFeedback(Strength, 0.1f, true, true, true, true);
			//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::HandleMovementInput] Playing Force Feedback:! Strength: %f"), Strength);
		}
		else
		{
			//FallVector = FVector(0)
		}
	}
}

void ABrawlCharacter::HandleRotationInput()
{
	FVector RotationVector(GetInputAxisValue(RotateForwardBinding), GetInputAxisValue(RotateRightBinding), 0);
	RotationVector.Normalize(RotationTiltCutoff);
	if (!RotationVector.IsNearlyZero(RotationTiltCutoff))
	{
		if (!SetActorRotation(RotationVector.Rotation()))
		{

			UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::HandleRotationInput] SetActorRotation() Failed: %s"), *GetNameSafe(this));
		}//  ETeleportType::TeleportPhysics
		PrevRotationVector = RotationVector;

	}
	else
	{
		SetActorRotation(PrevRotationVector.Rotation());
	}
}

void ABrawlCharacter::OnPunchSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABrawlCharacter* OtherPlayer = Cast<ABrawlCharacter>(OtherActor);
	if (OtherActor != this && OtherPlayer != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::OnPunchSphereOverlapBegin] Sphere Overlapped! Other Actor: %s"), *GetNameSafe(OtherActor));
		OtherPlayer->GetPunched(GetVelocity() * 10000);
	}
}

void ABrawlCharacter::Punch()
{
	if (!bHasFallen)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Punch] Punch Begin: %s"), *GetNameSafe(this));
		PunchSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bIsPunching = true;

		GetWorld()->GetTimerManager().SetTimer(
			TH_PunchHandle,
			this,
			&ABrawlCharacter::PunchEnd,
			PunchLength,
			false);
	}
}

void ABrawlCharacter::PunchEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Punch] Player Punch End: %s"), *GetNameSafe(this));
	//	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsPunching = false;
}

void ABrawlCharacter::GetPunched(FVector punchStrength)
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::GetPunched] %s Got Punched "), *GetNameSafe(this));
//	GetMovementComponent()->AddInputVector(punchStrength, true);
	PunchEnd();
	Fall();
	GetMesh()->AddForce(punchStrength, "ProtoPlayer_BIND_Head_JNT_center");
}

void ABrawlCharacter::Fall()
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Fall] Player Fell: %s"), *GetNameSafe(this));
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
	GetMesh()->AddForce(Velocity, "head");
	if(BrawlPlayerController)
		BrawlPlayerController->PlayDynamicForceFeedback(1, 0.5, true, true, true, true);
	GetMovementComponent()->StopActiveMovement();
	//FallVector = FVector(0);
}

void ABrawlCharacter::GetUp()
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::GetUp] Player Getting up: %s"), *GetNameSafe(this));

	//Note that if this component is currently attached to something, beginning simulation will detach it.
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget,false);
	GetMesh()->AttachToComponent(GetRootComponent(), Rule);


	//GetMesh()->SetWorldLocationAndRotationNoPhysics(GetActorLocation() + FVector(0, 0, -80), GetActorRotation() + FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocationAndRotation(InitialRelativeMeshLocation, InitialRelativeMeshRotation);
	
	//GetMesh()->SetRelativeLocation(InitialRelativeMeshLocation);
	//GetMesh()->SetRelativeRotation(InitialRelativeMeshRotation);
	bHasFallen = false;
}

FRotator ABrawlCharacter::GetPrevRotation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Actor Rotation: %s"), *PrevRotationVector.Rotation().ToString());
	return PrevRotationVector.Rotation(); //works but will always be delayed by a frame.. ugly
}
