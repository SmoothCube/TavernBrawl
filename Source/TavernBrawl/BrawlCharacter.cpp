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
	PunchSphere = CreateDefaultSubobject<USphereComponent>("PunchSphere");
	PunchSphere->SetupAttachment(GetMesh(), "sphereCollisionHere");
}

// Called when the game starts or when spawned
void ABrawlCharacter::BeginPlay()
{
	Super::BeginPlay();
	PunchSphere->OnComponentBeginOverlap.AddDynamic(this, &ABrawlCharacter::OnPunchSphereOverlapBegin);

}

// Called every frame
void ABrawlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovementInput();
	HandleRotationInput();



	//float Speed = GetMovementComponent()->Velocity.Size();
	////UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Tick] Speed: %f"), Speed);
	//if (Speed >= GetMovementComponent()->GetMaxSpeed())
	//{
	//	PlayControllerVibration(0.1f);
	//	Fall();
	//}
}

void ABrawlCharacter::OnPunchSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::OnPunchSphereOverlapBegin] Sphere Overlapped! Other Actor: %s"), *GetNameSafe(OtherActor));
	ABrawlCharacter* OtherPlayer = Cast<ABrawlCharacter>(OtherActor);
	if (OtherPlayer != this && OtherPlayer != nullptr)
	{
		OtherPlayer->GetPunched(GetVelocity()*10);
	}
}

// Called to bind functionality to input
void ABrawlCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(RotateForwardBinding);
	PlayerInputComponent->BindAxis(RotateRightBinding);
	PlayerInputComponent->BindAction("Punch", IE_Released, this, &ABrawlCharacter::Punch);
}


void ABrawlCharacter::HandleMovementInput()
{
	FVector InputVector(GetInputAxisValue(MoveForwardBinding), GetInputAxisValue(MoveRightBinding), 0);

	if (bIsMovementAllowed && !InputVector.IsNearlyZero())
		GetMovementComponent()->AddInputVector(InputVector);
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

void ABrawlCharacter::Fall()
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Fall] Player Fell: %s"), *GetNameSafe(this));

	GetWorld()->GetTimerManager().SetTimer(
		TH_FallHandle,
		this,
		&ABrawlCharacter::GetUp,
		RecoveryTime,
		false);
	bIsMovementAllowed = false;

}

void ABrawlCharacter::GetUp()
{
	bIsMovementAllowed = true;
}


void ABrawlCharacter::Punch()
{
	//	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABrawlCharacter::GetPunched(FVector punchStrength)
{
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::GetPunched] Someone Got Punched: %s"), *GetNameSafe(this));
	GetMovementComponent()->AddInputVector(punchStrength, true);
}

