// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingRock.h"

#include "Components/SphereComponent.h"

#include "BrawlCharacter.h"
#include "Character/PunchComponent.h"
#include "Subsystems/ScoreSubsystem.h"
#include "BrawlPlayerController.h"

// Sets default values
ARollingRock::ARollingRock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
}

// Called when the game starts or when spawned
void ARollingRock::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARollingRock::OnOverlapBegin);
}

// Called every frame
void ARollingRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARollingRock::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABrawlCharacter* Player = nullptr;
	Player= Cast<ABrawlCharacter>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARollingRock::OnOverlapBegin]: %s overlapped!"), *GetNameSafe(this));
		Player->Fall();
		Player->GetDamaged();
	}
}

void ARollingRock::Fall()
{
	Sphere->SetSimulatePhysics(true);
	Sphere->SetEnableGravity(true);
}
