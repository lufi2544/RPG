// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RPGAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectMacros.h"

#include "RPGBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class RPG_API URPGBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	public:

	/** Function That Will add the Targets or the Hit Results to the Target data to the @ContainerSpec
	 * This is meant to be used on a projectile for example, that the Target adding will be implmeneted outside the GameplayAbility
	 * @param ContainerSpec The Container Spec to add the Target Data.
	 * @param HitResults The hit Results that will be added to the Container Spec.
	 * @param Actors The Actos that will be added to the Container Spec.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPG|BlueprintLibrary")
	static void AddTargetsToGameplayEffectSpec(FRPGGameplayEffectContainerSpec ContainerSpec, TArray<FHitResult>HitResults, TArray<AActor*>Actors);

	/** This Function will apply the Container Spec to the Target Data in it.
	 *
	 * @param ContainerSpec The Container Spec that will be applied.
	 * @return The Active GameplayEffects that the Container Spec has added to the Targets on the Target Data.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPG|BlueprintLibrary")
	static TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectContainerSpec(FRPGGameplayEffectContainerSpec ContainerSpec);

	UFUNCTION(BlueprintPure, Category = "RPG|BlueprintLibrary")
	/** True if the @ContainerSpec has Effects to Apply to Targets. */
	static bool DoesContainerSpecHasEffects(FRPGGameplayEffectContainerSpec ContainerSpec);

	UFUNCTION(BlueprintPure , Category = "RPG|Ability|Filter")
	static FRPGGameplayTargetDataFilterHandle MakeRPGGameplayTargetDataFilterHandle(FRPGGameplayTargetDataFilter Filter , AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "RPG|BlueprintLibrary")
    static void ApplyEffectContainerSpecToTargetsFromTargetData(FRPGGameplayEffectContainerSpec ContainerSpec, const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	
};


