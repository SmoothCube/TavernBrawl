// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "UMG/Public/Components/Image.h"
#include "BrawlPlayerController.h"
#include "Subsystems/ScoreSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ConfigCacheIni.h"

bool UHealthWidget::Initialize()
{
	bool s = Super::Initialize();
	
	Material = UMaterialInstanceDynamic::Create(Cast<UMaterialInterface>(Image), this);


	FSlateBrush brush;
	brush.SetResourceObject(Material);
	brush.DrawAs = ESlateBrushDrawType::Image;
	brush.SetImageSize(FVector2D(100, 100));
	HealthImage->Brush = brush;
	
	return s;
}

void UHealthWidget::SetupOwnerAndBindEvents(ABrawlPlayerController* inOwner)
{
	Owner = inOwner;

	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UHealthWidget::SetupOwnerAndBindEvents]. Can't find owner!"));
		return;
	}

	scoreSystem =  Owner->GetLocalPlayer()->GetSubsystem<UScoreSubsystem>();
	Material->SetScalarParameterValue("Health", scoreSystem->GetHealth());

	scoreSystem->OnHealthTaken.AddDynamic(this, &UHealthWidget::UpdateHealth);
}

void UHealthWidget::UpdateHealth(int Amount)
{
	if (Amount == 0)
	{
		HealthImage->SetVisibility(ESlateVisibility::Hidden);
		scoreSystem->OnHealthTaken.RemoveDynamic(this, &UHealthWidget::UpdateHealth);
		return;
	}
	if (!Owner) return;
	
	UE_LOG(LogTemp, Warning, TEXT("New health %i"), Amount);

	Material->SetScalarParameterValue("Health", scoreSystem->GetHealth());
}
