// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "AbilitySystem/RPGTargetType.h"

#include "RPGAbilityTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGAbilityTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn = true), Category = Targeting)
    FRPGGameplayTargetDataFilterHandle RPGFilter;


	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
};
