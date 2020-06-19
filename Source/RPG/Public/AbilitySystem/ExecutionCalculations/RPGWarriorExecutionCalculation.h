// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RPGWarriorExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGWarriorExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	URPGWarriorExecutionCalculation();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
