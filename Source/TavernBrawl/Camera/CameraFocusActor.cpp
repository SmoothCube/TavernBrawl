// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraFocusActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "BrawlPlayerController.h"
#include "BrawlPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraFocusActor::ACameraFocusActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(Root);
	SpringArm->SetRelativeRotation(FRotator(-60, 0, 0));
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 8.0;
	SpringArm->TargetArmLength = SmallestSpringArmLength;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ACameraFocusActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraFocusActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Players.Num() == 0)
		return;

	FVector sum = FVector::ZeroVector;
	float distanceToFurthestPlayer = 0.f;

	for (const auto PlayerController : Players)
	{
		auto Pawn = PlayerController->GetPawn();
		if (!Pawn) { UE_LOG(LogTemp, Warning, TEXT("Missing Pawn for PlayerController: %s"), *GetNameSafe(this)); continue; }
		sum += Pawn->GetActorLocation();
		auto distance = FVector::Dist(Pawn->GetActorLocation(), GetActorLocation());
		if (distance > distanceToFurthestPlayer)
			distanceToFurthestPlayer = distance;
	}
	if(Players.Num() != 0)
		sum /= Players.Num();

	if (distanceToFurthestPlayer > SmallestSpringArmLength)
	{
	SpringArm->TargetArmLength = distanceToFurthestPlayer * BorderWidth;
	}
	else
	{
		SpringArm->TargetArmLength = SmallestSpringArmLength*BorderWidth;
	}

	SetActorLocation(sum);
}

void ACameraFocusActor::SetupCamera()
{
	TArray<AActor*> TempPlayerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrawlPlayerController::StaticClass(), TempPlayerArray);
	
	Players.Empty();
	for (const auto TempPlayer : TempPlayerArray)
	{
		Players.Push(Cast<ABrawlPlayerController>(TempPlayer));
	}

	for (const auto Player : Players)
	{
		Player->SetViewTarget(this);
	}
}