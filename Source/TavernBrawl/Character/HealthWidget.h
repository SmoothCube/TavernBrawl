// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UImage;
class UScoreSubsystem;
class UMaterialInstanceDynamic;
class ABrawlPlayerController;

UCLASS()
class TAVERNBRAWL_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetupOwnerAndBindEvents(ABrawlPlayerController* inOwner);

	UFUNCTION()
	void UpdateHealth(int Amount);

protected:

	UPROPERTY(meta = (BindWidget))
	UImage* HealthImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* Image;

private:

	UMaterialInstanceDynamic* Material = nullptr;
	ABrawlPlayerController* Owner = nullptr;
	UScoreSubsystem* scoreSystem = nullptr;
	
};	
