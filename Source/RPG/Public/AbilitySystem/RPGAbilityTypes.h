// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RPGTargetType.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Engine/EngineTypes.h"
#include "Templates/SubclassOf.h"

#include "RPGAbilityTypes.generated.h"



/**
 * We are going to make this header file to include all the types that we need to include on the proyect.
 * Enums, maps...
 */

class UGameplayEffect;
class URPGTargetType;

/**Struct defining a list of gameplay effects, a tag, and targeting info
 * These containers are defined statically in blueprints or assets and then turn into Specs at runtime
 */
USTRUCT(BlueprintType)
struct FRPGGameplayEffectContainer
{

 GENERATED_BODY()
 
 public:

   FRPGGameplayEffectContainer() {}

    //The way the effects are going to be implemented.
    UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "RPG|AbilityTypes")
    TSubclassOf<URPGTargetType>TargetType;

    //Effects that are going to apply to the targets
    UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "RPG|AbilityTypes")
    TArray<TSubclassOf<UGameplayEffect>>EffectsToApply;
};


USTRUCT(BlueprintType)
struct  FRPGGameplayEffectContainerSpec
{
   GENERATED_BODY()

public:
 FRPGGameplayEffectContainerSpec(){}

 /** Computed target data */
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
 FGameplayAbilityTargetDataHandle TargetData;

 //Effects that are going to apply to the targets
 UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "RPG|AbilityTypes")
 TArray<FGameplayEffectSpecHandle>TargetGameplayEffectSpecsHandle;

 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RPG|AbilityTypes")
 TArray<FHitResult> HitActorsHitResults;

 UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category = "RPG|AbilityTypes")
 TArray<AActor*> HitActors;

UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "RPG|AbilityTypes")
 uint32 bHitEnemy :1;

 /** Returns true if this has any valid effect specs */
 bool HasValidEffects() const;

 /** Returns true if this has any valid targets */
 bool HasValidTargets() const;

 /** Adds new targets to target data */
 void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);


 
 
};

