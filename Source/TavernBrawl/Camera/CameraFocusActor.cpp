// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraFocusActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "BrawlPlayerController.h"
#include "BrawlCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

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

	for (const auto Player : Players)
	{
		if (!Player) { UE_LOG(LogTemp, Warning, TEXT("Missing Pawn for PlayerController: %s"), *GetNameSafe(this)); continue; }

		FVector PlayerMeshLocation = Player->GetMesh()->GetComponentLocation();
		sum += PlayerMeshLocation;
		auto distance = FVector::Dist(PlayerMeshLocation, GetActorLocation());
		if (distance > distanceToFurthestPlayer)
			distanceToFurthestPlayer = distance;
	}
	if(Players.Num() != 0)
		sum /= Players.Num();
	sum.Z = 0;
	SetActorLocation(sum);



	SetSpringArmLength(distanceToFurthestPlayer);
}

void ACameraFocusActor::SetSpringArmLength(float distanceToFurthestPlayer)
{
	float longestVector = 0.f;

	for (int i = 0; i < Players.Num(); i++)
	{
		for (int j = i + 1; j < Players.Num(); j++)
		{
			if (Players.IsValidIndex(i) && Players.IsValidIndex(j))
			{
				float Temp = FMath::Abs((Players[i]->GetMesh()->GetComponentLocation() - Players[j]->GetMesh()->GetComponentLocation()).X);
				if (longestVector < Temp)
					longestVector = Temp;
			}
		}
	}
	float OffsetX = longestVector;
	float ActualBorderWidth = BorderWidth + OffsetX;


	float newTargetLength = distanceToFurthestPlayer + ActualBorderWidth;

	if (newTargetLength <= SmallestSpringArmLength)
	{
		SpringArm->TargetArmLength = SmallestSpringArmLength;
		//UE_LOG(LogTemp, Warning, TEXT("[ACameraFocusActor::Tick] Distance smaller than smallest! %f"), OffsetX);
	}
	else if (newTargetLength >= LargestSpringArmLength)
	{
		SpringArm->TargetArmLength = LargestSpringArmLength;
		//UE_LOG(LogTemp, Warning, TEXT("[ACameraFocusActor::Tick] Distance larger than largest! %f"), OffsetX);
	}
	else
	{
		SpringArm->TargetArmLength = newTargetLength;
		//UE_LOG(LogTemp, Warning, TEXT("[ACameraFocusActor::Tick] Distance betweeen smallest and largest! %f	"), OffsetX);
	}
}

void ACameraFocusActor::SetupCamera()
{
	TArray<AActor*> TempPlayerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrawlCharacter::StaticClass(), TempPlayerArray);
	
	Players.Empty();
	for (const auto TempPlayer : TempPlayerArray)
	{
		Players.Push(Cast<ABrawlCharacter>(TempPlayer));
	}

	for (const auto Player : Players)
	{
		ABrawlPlayerController* PlayerController = Cast<ABrawlPlayerController>(Player->GetController());
		PlayerController->SetViewTarget(this);
	}
}

void ACameraFocusActor::RemovePlayer(ABrawlCharacter* Player)
{
	Players.Remove(Player);
}
