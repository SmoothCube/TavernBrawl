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
}

// Called every frame
void ABrawlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovementInput(DeltaTime);
	HandleRotationInput();
	//float Speed = GetMovementComponent()->Velocity.Size();
	////UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Tick] Speed: %f"), Speed);
	//if (Speed >= GetMovementComponent()->GetMaxSpeed())
	//{
	//	PlayControllerVibration(0.1f);
	//	Fall();
	//}
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

	if (!bHasFallen && !InputVector.IsNearlyZero())
	{
		InputVector.Normalize();
		GetMovementComponent()->AddInputVector(InputVector);
		FallVector += InputVector;
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
			
			ABrawlPlayerController* c = Cast<ABrawlPlayerController>(Controller);
			//c->PlayDynamicForceFeedback(Strength, 0.1f, true, true, true, true);
			//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::HandleMovementInput] Playing Force Feedback:! Strength: %f"), Strength);
		}
		else
		{
			ABrawlPlayerController* c = Cast<ABrawlPlayerController>(Controller);
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
		SetActorRotation(RotationVector.Rotation());
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
	ABrawlPlayerController* c = Cast<ABrawlPlayerController>(Controller);
	c->PlayDynamicForceFeedback(1, 0.5, true, true, true, true);
	GetMovementComponent()->StopActiveMovement();
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

FVector ABrawlCharacter::FindLeanVector()
{
	FVector LeanVector = GetActorRotation().RotateVector(InitialRelativeMeshRotation.RotateVector(GetVelocity()));
	//LeanVector = InitialRelativeMeshRotation.RotateVector(LeanVector);
	InitialRelativeMeshRotation.RotateVector(GetVelocity());
	return FVector(0);
}
