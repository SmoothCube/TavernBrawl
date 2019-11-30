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
	PunchCapsule->OnComponentBeginOverlap.AddDynamic(this, &AThrowableItem::OnPunchOverlapBegin);
	ThrowCapsule->OnComponentBeginOverlap.AddDynamic(this, &AThrowableItem::OnThrowOverlapBegin);
	PunchCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ThrowCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AThrowableItem::OnPunchOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[AThrowableItem::OnPunchOverlapBegin] %s overlapped"), *GetNameSafe(this));
	ABrawlCharacter* Player = Cast<ABrawlCharacter>(OtherActor);
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
	if(newPlayer)
	HoldingPlayer = newPlayer;
}

ABrawlCharacter* AThrowableItem::GetHoldingPlayer()
{
	return HoldingPlayer;
}


// Called every frame
void AThrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowableItem::SetThrowCollisionDisabled()
{
	ThrowCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

