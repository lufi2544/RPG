// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RPGGameplayEffect.h"
#include "RPGCooldownEffect.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGCooldownEffect : public URPGGameplayEffect
{
	GENERATED_BODY()

	public:
	
	URPGCooldownEffect();

	FGameplayTag GetCoolDownTag();
	
	protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category = "RPG|Effect")
	FGameplayTag CooldownTag;
	
};
