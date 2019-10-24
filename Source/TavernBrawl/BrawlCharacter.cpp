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
	PunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

void ABrawlCharacter::OnPunchSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABrawlCharacter* OtherPlayer = Cast<ABrawlCharacter>(OtherActor);
	if (OtherActor != this && OtherPlayer != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::OnPunchSphereOverlapBegin] Sphere Overlapped! Other Actor: %s"), *GetNameSafe(OtherActor));
		OtherPlayer->GetPunched(GetVelocity() * 100);
	}
}

void ABrawlCharacter::Punch()
{
	//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Punch] Punch Begin: %s"), *GetNameSafe(this));

	PunchSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bIsPunching = true;

	GetWorld()->GetTimerManager().SetTimer(
		TH_FallHandle,
		this,
		&ABrawlCharacter::PunchEnd,
		PunchLength,
		false);
}

void ABrawlCharacter::PunchEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::Punch] Player Punch End: %s"), *GetNameSafe(this));

	//	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsPunching = false;
}

void ABrawlCharacter::GetPunched(FVector punchStrength)
{
	//UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::GetPunched] %s Got Punched "), *GetNameSafe(this));
	GetMovementComponent()->AddInputVector(punchStrength, true);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->AddForce(punchStrength, "head");

	//Fall only disables movement and sets timer for getting up atm. needs to be changed later.
	//TODO either refactor GetPunched code into Fall(), 
	//or make another function that fall and GetPunched both can use for setting the timer and movemnet
	//Fall();


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
	UE_LOG(LogTemp, Warning, TEXT("[ABrawlCharacter::GetUp] Player Getting up: %s"), *GetNameSafe(this));

	bIsMovementAllowed = true;
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetSimulatePhysics(false);
}




