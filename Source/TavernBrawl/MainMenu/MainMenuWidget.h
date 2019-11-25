// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class TAVERNBRAWL_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 NumberOfPlayers = 4;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 Health = 4;

	UFUNCTION(BlueprintCallable)
	void UpdateConfig();
};
