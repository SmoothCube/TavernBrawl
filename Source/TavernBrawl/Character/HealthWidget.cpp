// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "BrawlPlayerController.h"
#include "Subsystems/ScoreSubsystem.h"

void UHealthWidget::SetupOwnerAndBindEvents(ABrawlPlayerController* inOwner)
{
	Owner = inOwner;

	if (!Owner) return;

	UScoreSubsystem* scoreSystem =  Owner->GetLocalPlayer()->GetSubsystem<UScoreSubsystem>();
	scoreSystem->OnHealthTaken.AddDynamic(this, &UHealthWidget::UpdateHealth);
}

void UHealthWidget::UpdateHealth(int Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("New health %i"), Amount);
	HealthBar->SetPercent(Step * Amount);
}
