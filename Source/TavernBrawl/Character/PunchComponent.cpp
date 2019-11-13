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
	if (OtherActor != GetOwner() && OtherPlayer != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::OnPunchSphereOverlapBegin] Sphere Overlapped! Other Actor: %s"), *GetNameSafe(OtherActor));
		OtherPlayer->PunchComponent->GetPunched(Player->GetVelocity() * 10000);
		Player->CurrentFallTimer = 0.f;
	}
}

void UPunchComponent::Punch()
{
	if (!Player->bHasFallen && !bIsPunching)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::Punch] Punch Begin: %s"), *GetNameSafe(this));
		Player->GetPunchSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bIsPunching = true;

		//normalizes manually :((( ugly
		float f = FVector::DotProduct(Player->PrevRotationVector / Player->PrevRotationVector.Size(), Player->GetCharacterMovement()->Velocity / Player->GetCharacterMovement()->Velocity.Size());
		//UE_LOG(LogTemp, Error, TEXT("[UPunchComponent::Punch] dot product: %f"), f);

		//clamp f+1.1 so max dodge is closer and min dodge is further
		Player->GetCharacterMovement()->Velocity = Player->PrevRotationVector * Player->GetCharacterMovement()->Velocity.Size() * DashLengthCurve->GetFloatValue(f);

		GetWorld()->GetTimerManager().SetTimer(
			TH_PunchHandle,
			this,
			&UPunchComponent::PunchEnd,
			PunchLength,
			false);
	}
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

void UPunchComponent::GetPunched(FVector punchStrength)
{
	//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::GetPunched] %s Got Punched "), *GetNameSafe(this));
	PunchEnd();
	Player->Fall();
	Player->GetMesh()->AddForce(punchStrength, "ProtoPlayer_BIND_Head_JNT_center");

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
