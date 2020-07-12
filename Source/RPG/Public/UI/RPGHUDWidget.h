// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHUDWidget.generated.h"

/**
 * This is the Player HUD that will show up on the screen.
 */
UCLASS()
class RPG_API URPGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetRespawnCountDown(float RespawnTimeRemaining);

	/** Attributes Setters */


	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetMaxHealth(float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetHeroLevel(int32 Level);
	
};
