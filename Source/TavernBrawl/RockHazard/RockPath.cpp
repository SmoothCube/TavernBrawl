// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPath.h"
#include "Components/SplineComponent.h"
#include "Math/UnrealMathUtility.h"

#include "RollingRock.h"
#include "Components/SphereComponent.h"

ARockPath::ARockPath()
{
	PrimaryActorTick.bCanEverTick = true;
	Path = CreateDefaultSubobject<USplineComponent>("Path");
}
// Called when the game starts or when spawned
void ARockPath::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARockPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<ARollingRock*> RocksToDelete;
	for (auto& r : ActiveRocks)
	{
		r->t+= r->speed*DeltaTime;
		r->SetActorTransform(Path->GetTransformAtTime(r->t, ESplineCoordinateSpace::World));

		//Rolling code
		FVector nextPos = Path->GetLocationAtTime(r->t+ r->speed*DeltaTime, ESplineCoordinateSpace::World);
		nextPos = nextPos - r->GetActorLocation();
		float d = nextPos.Size() / r->Sphere->GetScaledSphereRadius();
		d = d / (2.f* PI )* 360;
		r->degrees-=d;
		r->SetActorRotation(FRotator(r->degrees, r->GetActorRotation().Yaw, r->GetActorRotation().Pitch));

		//for disconnecting them from spline	
		if (r->t >= 1)
		{
			r->Fall();
			RocksToDelete.Add(r);
		}
	}
	for (auto& r : RocksToDelete)
		ActiveRocks.Remove(r);
}

void ARockPath::SpawnRock()
{	
	ARollingRock* newRock = GetWorld()->SpawnActor<ARollingRock>(RockClass, Path->GetTransformAtTime(0, ESplineCoordinateSpace::World));
	if (newRock)
		ActiveRocks.Add(newRock);
	else
		UE_LOG(LogTemp, Warning, TEXT("[ARockPath::SpawnRock]: Rock did not spawn properly!"));
}


