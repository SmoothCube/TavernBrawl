// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPath.h"
#include "Components/SplineComponent.h"

#include "RollingRock.h"

ARockPath::ARockPath()
{
	PrimaryActorTick.bCanEverTick = true;
	Path = CreateDefaultSubobject<USplineComponent>("Path");
}
// Called when the game starts or when spawned
void ARockPath::BeginPlay()
{
	Super::BeginPlay();
	SpawnRock();
}

// Called every frame
void ARockPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto& r : ActiveRocks)
	{
		r->t+= r->speed*DeltaTime;
		r->SetActorTransform(Path->GetTransformAtTime(r->t,ESplineCoordinateSpace::World));
	}
}

void ARockPath::SpawnRock()
{	
	ARollingRock* newRock = GetWorld()->SpawnActor<ARollingRock>(RockClass, Path->GetTransformAtTime(0, ESplineCoordinateSpace::World));
	if (newRock)
		ActiveRocks.Add(newRock);
	else
		UE_LOG(LogTemp, Warning, TEXT("[ARockPath::SpawnRock]: Rock did not spawn properly!"));
}


