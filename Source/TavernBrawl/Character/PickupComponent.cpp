// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"
#include "ThrowableItem.h"
#include "BrawlCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ABrawlCharacter>(GetOwner());
}

void UPickupComponent::OnPickupOrThrowTrigger()
{
	if (HoldingItem)
	{
		StartThrowingItem();
	}
	else
	{
		PickupHoldingItem();
	}
}

void UPickupComponent::ReleaseHoldingItem()
{
	if (!HoldingItem) return;
	FDetachmentTransformRules rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	HoldingItem->DetachFromActor(rules);
	HoldingItem->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HoldingItem->PunchCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HoldingItem->Mesh->SetSimulatePhysics(true);
	HoldingItem->SetHoldingPlayer(nullptr);
	HoldingItem = nullptr;
}

void UPickupComponent::PickupHoldingItem()
{
	if (HoldingItem) return;
	if (ItemsInRange.Num() == 0) return;

	ItemsInRange.Sort([&](const AThrowableItem& LeftSide, const AThrowableItem& RightSide)
		{
			float DistanceA = FVector::Dist(Player->GetActorLocation(), LeftSide.GetActorLocation());
			float DistanceB = FVector::Dist(Player->GetActorLocation(), RightSide.GetActorLocation());
			return DistanceA < DistanceB;
		});

	if (!ItemsInRange[0]) return;

	HoldingItem = ItemsInRange[0];

	UE_LOG(LogTemp, Warning, TEXT("[UPickupComponent::OnPickupSphereOverlapBegin] %s is overlapping with %s"), *GetNameSafe(HoldingItem), *GetNameSafe(this));
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	HoldingItem->Mesh->SetSimulatePhysics(false);
	HoldingItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HoldingItem->AttachToComponent(Cast<USceneComponent>(Player->GetMesh()), rules, FName("ChairRig_Throwable_JNTSocket"));
	HoldingItem->SetHoldingPlayer(Player);
}

void UPickupComponent::StartThrowingItem()
{
	bIsAiming = true;
}

AThrowableItem* UPickupComponent::GetHoldingItem()
{
	return HoldingItem;
}

void UPickupComponent::ThrowHoldingItem()
{
	if (!HoldingItem) return;

	FDetachmentTransformRules rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	HoldingItem->DetachFromActor(rules);
	HoldingItem->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HoldingItem->Mesh->SetSimulatePhysics(true);
	HoldingItem->Mesh->AddImpulse(Player->GetActorForwardVector() * 50000);
	HoldingItem->SetHoldingPlayer(nullptr);
	HoldingItem = nullptr;
	bIsAiming = false;


}

bool UPickupComponent::IsHoldingItem()
{
	if (HoldingItem)
		return true;
	return false;
}

bool UPickupComponent::IsAiming()
{
	return bIsAiming;
}

void UPickupComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HoldingItem) return;
	AThrowableItem* Item = Cast<AThrowableItem>(OtherActor);
	if (!Item) return;
	ItemsInRange.Add(Item);

}
void UPickupComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HoldingItem) return;
	AThrowableItem* Item = Cast<AThrowableItem>(OtherActor);
	if (!Item) return;
	ItemsInRange.Remove(Item);
}