// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFloatingStatusBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGFloatingStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()


	public:
	
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetCharacterName(const FText& NewName);
	
};
