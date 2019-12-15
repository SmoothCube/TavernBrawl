// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchComponent.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/ScoreSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"

#include "BrawlCharacter.h"
#include "BrawlPlayerController.h"
#include "Camera/CameraFocusActor.h"
#include "PickupComponent.h"
#include "ThrowableItem.h"

UPunchComponent::UPunchComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPunchComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPunchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Player = Cast<ABrawlCharacter>(GetOwner());
	if (Player && Player->BrawlPlayerController && !bAssignedEvent)
	{
		bAssignedEvent = true;
		UScoreSubsystem* subsystem = Player->BrawlPlayerController->GetLocalPlayer()->GetSubsystem<UScoreSubsystem>();
		subsystem->OnZeroHealth.AddDynamic(this, &UPunchComponent::KillCharacter);
		UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::BeginPlay]: %s OnZeroHealth bound!"), *GetNameSafe(this));
		NormalMaxWalkSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
	}
}

void UPunchComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABrawlCharacter* OtherPlayer = Cast<ABrawlCharacter>(OtherActor);
	UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp);
	
	if (!bHasHit && OtherActor != GetOwner() && OtherPlayer != nullptr && Capsule != nullptr)
	{
		PunchHit(OtherPlayer);
	}
	
}

void UPunchComponent::PunchButtonPressed()
{
	if (Player && !Player->bHasFallen && !bIsPunching && !Player->GetMovementComponent()->IsFalling())
	{
		bIsPunching = true;
	}
}

void UPunchComponent::Punch()
{
	if (!Player) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::Punch]: %s No Player found for PunchComponent!"), *GetNameSafe(this)); return; }
	float f = FVector::DotProduct(Player->PrevRotationVector.GetSafeNormal(), Player->GetCharacterMovement()->Velocity.GetSafeNormal());
	Player->GetCharacterMovement()->Velocity = Player->PrevRotationVector * Player->GetCharacterMovement()->Velocity.Size() * DashLengthCurve->GetFloatValue(f);
	if(Player->GetPunchSphere())
		Player->GetPunchSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::Punch]: %s No PunchSphere found for player !"), *GetNameSafe(Player));
	if (PunchSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PunchSound, Player->PunchSphere->GetComponentLocation(), 1.0f, FMath::FRandRange(0.9f, 1.1f));
}

void UPunchComponent::PunchWithItem()
{
	if (!Player) { UE_LOG(LogTemp, Error, TEXT("[UPunchComponent::PunchWithItem]: %s No Player For PunchComponent!"), *GetNameSafe(this)); return; }
	if (!Player->PickupComponent) {UE_LOG(LogTemp, Error, TEXT("[UPunchComponent::PunchWithItem]: No PickupComponent for player %s!"), *GetNameSafe(Player)); return;}
	if (Player->PickupComponent->GetHoldingItem())
	{
		Player->PickupComponent->GetHoldingItem()->PunchCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UPunchComponent::PunchWithItem]: %s not holding item!"), *GetNameSafe(this));
	}
}

void UPunchComponent::PunchWithItemEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchWithItemEnd]: %s punching!"), *GetNameSafe(this));
	if (!Player) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchWithItemEnd]: %s No Player!"), *GetNameSafe(this)); }
	if(!Player->PickupComponent) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchWithItemEnd]: No PickupComponent for Player %s !"), *GetNameSafe(Player)); }
	if(Player->PickupComponent->IsHoldingItem())
		Player->PickupComponent->GetHoldingItem()->PunchCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(
		TH_PunchAgainHandle,
		this,
		&UPunchComponent::setIsPunchingFalse,
		PunchWaitingTime,
		false);
}

void UPunchComponent::PunchHit(ABrawlCharacter* OtherPlayer)
{
	//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::OnPunchSphereOverlapBegin] Sphere Overlapped! Other Actor: %s"), *GetNameSafe(OtherActor));
	if (!OtherPlayer) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchHit]: %s No OtherPlayer found!"), *GetNameSafe(this)); return; }
	if (!OtherPlayer->PunchComponent) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchHit]: No PunchComponent found for OtherPlayer %s!"), *GetNameSafe(OtherPlayer)); return; }
	if (!Player) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchHit]: No Player found for PunchComponent %s!"), *GetNameSafe(this)); return; }

	OtherPlayer->PunchComponent->GetPunched(Player->GetVelocity());
	Player->CurrentFallTimer = 0.f;
	Player->GetMovementComponent()->Velocity *= PunchHitVelocityDamper;
	bHasHit = true;

	if (Player->BrawlPlayerController)
		Player->BrawlPlayerController->PlayDynamicForceFeedback(0.7f, 0.1f, true, true, true, true);
}

void UPunchComponent::PunchEnd()
{
	if (!Player) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchEnd]: No Player found for PunchComponent %s!"), *GetNameSafe(this)); return; }

	UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::PunchEnd] Player Punch End: %s"), *GetNameSafe(this));
	Player->GetPunchSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Player->GetCharacterMovement()->MaxWalkSpeed = NormalMaxWalkSpeed;
	Player->GetCharacterMovement()->Velocity = Player->GetCharacterMovement()->Velocity.GetClampedToMaxSize(NormalMaxWalkSpeed);

	GetWorld()->GetTimerManager().SetTimer(
		TH_PunchAgainHandle,
		this,
		&UPunchComponent::setIsPunchingFalse,
		PunchWaitingTime,
		false);
}

void UPunchComponent::GetPunched(FVector InPunchStrength)
{
	if (!Player) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::GetPunched]: No Player found for PunchComponent %s!"), *GetNameSafe(this)); return; }

	//UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::GetPunched] %s Got Punched "), *GetNameSafe(this));
	float strength = InPunchStrength.Size();
	UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::GetPunched]: punch strength: %f"), strength)

	if (strength > MinPunchStrengthToFall)
	{
		PunchEnd();
		Player->Fall();
		Player->GetMesh()->AddForce(InPunchStrength* PunchStrength, "ProtoPlayer_BIND_Head_JNT_center");
		Player->GetDamaged();
	}
	else
	{
		Player->GetMovementComponent()->Velocity = InPunchStrength;
		//Player->FallVector = InPunchStrength;
	}
}

void UPunchComponent::KillCharacter()
{
	if (!Player) { UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::KillCharacter]: No Player found for PunchComponent %s!"), *GetNameSafe(this)); return; }

	UE_LOG(LogTemp, Warning, TEXT("[UPunchComponent::KillCharacter] %s is killed"), *GetNameSafe(Player));
	Player->bIsDead = true;
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraFocusActor::StaticClass(), OutActors);
	Cast<ACameraFocusActor>(OutActors[0])->RemovePlayer(Player);
}
