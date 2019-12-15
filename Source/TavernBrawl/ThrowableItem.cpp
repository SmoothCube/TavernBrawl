// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BrawlCharacter.h"

// Sets default values
AThrowableItem::AThrowableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	PunchCapsule = CreateDefaultSubobject<UCapsuleComponent>("PunchCapsule");
	PunchCapsule->SetupAttachment(RootComponent);
	ThrowCapsule = CreateDefaultSubobject<UCapsuleComponent>("ThrowCapsule");
	ThrowCapsule->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	if (PunchCapsule)
	{
		PunchCapsule->OnComponentBeginOverlap.AddDynamic(this, &AThrowableItem::OnPunchOverlapBegin);
		PunchCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::BeginPlay] %s PunchCapsule failed to begin play"), *GetNameSafe(this));
	}
	if (ThrowCapsule)
	{
		ThrowCapsule->OnComponentBeginOverlap.AddDynamic(this, &AThrowableItem::OnThrowOverlapBegin);
		ThrowCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::BeginPlay] %s ThrowCapsule failed to begin play"), *GetNameSafe(this));
	}

}

void AThrowableItem::OnPunchOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::OnPunchOverlapBegin] %s overlapped"), *GetNameSafe(this));
	ABrawlCharacter* Player = Cast<ABrawlCharacter>(OtherActor);
	if(!HoldingPlayer)
		UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::OnPunchOverlapBegin] %s No Holding Player!"), *GetNameSafe(this));
	if (Player && OtherComp->IsA(UCapsuleComponent::StaticClass()) && Player != HoldingPlayer)
	{
		Player->Fall();
		Player->GetDamaged();
	}
}

void AThrowableItem::OnThrowOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::OnThrowOverlapBegin] %s overlapped with %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));
	//ThrowCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ABrawlCharacter* Player = Cast<ABrawlCharacter>(OtherActor);

	if (!HoldingPlayer)
		UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::OnThrowOverlapBegin] %s No Holding Player!"), *GetNameSafe(this));
	if (Player && OtherComp->IsA(UCapsuleComponent::StaticClass()) && Player != HoldingPlayer)
	{
		if (GetVelocity().Size() >= MinFallSpeed)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::OnThrowOverlapBegin] %s Falling"), *GetNameSafe(OtherActor));
			Player->Fall();
			Player->GetDamaged();
		}
			ThrowCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AThrowableItem::SetHoldingPlayer(ABrawlCharacter* newPlayer)
{
	if(IsValid(newPlayer))
		HoldingPlayer = newPlayer;
	else
		UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::SetHoldingPlayer] new player invalid! for %s"), *GetNameSafe(this));
}

ABrawlCharacter* AThrowableItem::GetHoldingPlayer()
{
	if(!HoldingPlayer)
		UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::GetHoldingPlayer] No holding player! Return nullptr for %s"), *GetNameSafe(this));
	return HoldingPlayer;

}


// Called every frame
void AThrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowableItem::SetThrowCollisionDisabled()
{
	if (!ThrowCapsule)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::SetThrowCollisionDisabled] No ThrowCapsule! %s"), *GetNameSafe(this));
	}
	ThrowCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

