// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AThrowableItem::AThrowableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

