// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchComponent.h"
#include "BrawlCharacter.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Subsystems/ScoreSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "BrawlPlayerController.h"
#include "Camera/CameraFocusActor.h"
#include "Kismet/GameplayStatics.h"

UPunchComponent::UPunchComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPunchComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ABrawlCharacter>(GetOwner());
	if (Player->BrawlPlayerController && !bAssignedEvent)
	{
		bAssignedEvent = true;
		UScoreSubsystem* subsystem = Player->BrawlPlayerController->GetLocalPlayer()->GetSubsystem<UScoreSubsystem>();
		subsystem->OnZeroHealth.AddDynamic(this, &UPunchComponent::KillCharacter);
	}
	NormalMaxWalkSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
}

void UPunchComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABrawlCharacter* OtherPlayer = Cast<ABrawlCharacter>(OtherActor);
	if (!bHasHit && OtherActor != GetOwner() && OtherPlayer != nullptr)
	{
		PunchHit(OtherPlayer);
	}
}

void UPunchComponent::PunchButtonPressed()
{
	if (!Player->bHasFallen && !bIsPunching)
	{
		bIsPunching = true;
		float f = FVector::DotProduct(Player->PrevRotationVector.GetSafeNormal(), Player->GetCharacterMovement()->Velocity.GetSafeNormal());
		Player->GetCharacterMovement()->Velocity = Player->PrevRotationVector * Player->GetCharacterMovement()->Velocity.Size() * DashLengthCurve->GetFloatValue(f);

		Player->GetPunchSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		GetWorld()->GetTimerManager().SetTimer(
			TH_PunchHandle,
			this,
			&UPunchComponent::PunchEnd,
			PunchLength,
			false);
	}
}

void UPunchComponent::PunchHit(ABrawlCharacter* OtherPlayer)
{
	//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::OnPunchSphereOverlapBegin] Sphere Overlapped! Other Actor: %s"), *GetNameSafe(OtherActor));
	OtherPlayer->PunchComponent->GetPunched(Player->GetVelocity());
	Player->CurrentFallTimer = 0.f;
	Player->GetMovementComponent()->Velocity *= PunchHitVelocityDamper;
	bHasHit = true;
}

void UPunchComponent::PunchEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchEnd] Player Punch End: %s"), *GetNameSafe(this));
	Player->GetPunchSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Player->GetCharacterMovement()->MaxWalkSpeed = NormalMaxWalkSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		TH_PunchAgainHandle,
		this,
		&UPunchComponent::setIsPunchingFalse,
		PunchWaitingTime,
		false);
}

void UPunchComponent::GetPunched(FVector InPunchStrength)
{
	//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::GetPunched] %s Got Punched "), *GetNameSafe(this));
	float strength = InPunchStrength.Size();
	UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::GetPunched]: punch strength: %f"), strength)

	if (strength > MinPunchStrengthToFall)
	{
		PunchEnd();
		Player->Fall();
		Player->GetMesh()->AddForce(InPunchStrength* PunchStrength, "ProtoPlayer_BIND_Head_JNT_center");
	}
	else
	{

		Player->GetMovementComponent()->Velocity = InPunchStrength;
		//Player->FallVector = InPunchStrength;
	}
	if (Player->BrawlPlayerController)
	{
		UScoreSubsystem* subsystem = Player->BrawlPlayerController->GetLocalPlayer()->GetSubsystem<UScoreSubsystem>();
		subsystem->DecrementHealth();
	}
	else
	{
		ABrawlPlayerController* TempPlayerController = Cast<ABrawlPlayerController>(Player->GetController());
		UScoreSubsystem* subsystem = TempPlayerController->GetLocalPlayer()->GetSubsystem<UScoreSubsystem>();
		subsystem->DecrementHealth();
		UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::GetPunched] %s Can't find the playercontroller"), *GetNameSafe(this));
	}
}

void UPunchComponent::KillCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::KillCharacter] %s is killed"), *GetNameSafe(this));
	Player->bIsDead = true;
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraFocusActor::StaticClass(), OutActors);
	Cast<ACameraFocusActor>(OutActors[0])->RemovePlayer(Player);
}
